/*
 * m3s_masterFunctions.c
 *
 * Created: 28.12.2011 10:27:39
 *  Author: Thomas Bergmüller
 */ 

#include "m3sV2_basicFunctions.h"
#include "m3s_masterFunctions.h"


unsigned char m3s_calcCtrlByte(unsigned char vMasterAddr, unsigned char vM3sProtocol_gc, unsigned char vFlags)
{
	unsigned char ctrlByte = vM3sProtocol_gc; // comes in Groupcodes
	
	ctrlByte |= vFlags;
	ctrlByte |= (vMasterAddr << M3S_CTRLBYTE_MASTERADDR_gp);	
	
	return(ctrlByte);
}

unsigned char m3s_checkFrame(unsigned char* rFrame)
{
	// Get Length
	// Check Checksum
		
	if((rFrame[0] & M3S_CTRLBYTE_PROTOCOL_gm) == M3S_CTRLBYTE_PROTOCOL_ACK_gc) // If acknowledge
	{		
		if(m3s_validateFrame(rFrame, M3S_ACK_FRAME_LENGTH))
		{
			return(M3S_ERROR_NOERROR_gc); // everything fine
		}
		else
		{
			return(M3S_ERROR_FRAME_NOT_ACK_FRAME_gc); // checksume error...
		}		
	}
	// ELSE: Normale Frames prüfen
	
	// ERROR; weil noch nicht fertig...
	return(1);	
}

unsigned char m3s_CheckAcknowledgeFrame(unsigned char* rFrameToCheck, unsigned char* rSentFrame)
{
	unsigned char error;
	error = m3s_checkFrame(rFrameToCheck); // Check if it's a valid frame...
	
	if(error)
	{
		return(error);
	}
	
	unsigned char expectedCtrlByte = M3S_CTRLBYTE_PROTOCOL_ACK_gc | (rSentFrame[0] & 0x0f) | (1<<M3S_CTRLBYTE_ACK_OR_MC_bp);
	
	if(rFrameToCheck[1] != rSentFrame[1]) // Address comparison
	{
		return(M3S_ERROR_WRONG_SLAVEADDR_gc);
	}
	
	if(rFrameToCheck[0] == expectedCtrlByte)
	{
		return(M3S_ERROR_NOERROR_gc); // everything fine...		
	}
	else
	{
		return(M3S_ERROR_FRAME_NOT_ACK_FRAME_gc); // Acknowledge error...
	}
}