/*
 * dcs_basicFunctions.c
 *
 * Created: 12.12.2011 22:42:12
 *  Author: tbergmueller
 */ 
 
#include <stdlib.h>

#include "dcs_basicFunctions.h"
#include "dcs_internal.h"
#include "dcs_config.h"
#include "dc_commands.h"
#include "m3sV2_basicFunctions.h"
#include "devComSlave.h"
#include "dc_conventions.h"
#include "preProcessorDirectives.h"
#include "crc8.h"

#include "dcs_hardwareFunctions.h"
 

// Stellt einen Pointer auf verwendbaren Speicher zur Verfügung (wählt aus den 2 Spaces aus)
unsigned char* dcs_getAvailableWorkspace()
{
	if(currentSlave->Data == NULL)
	{		
		return(dcs_space1);
	}
	if(currentSlave->Data == (dcs_space1+M3S_HEADER_LENGTH)) // if in space 1 is valid data then
	{
		return dcs_space2; // use space 2
	}
	else
	{
		return dcs_space1; // otherwise use space 1
	}
}

// Aktuelle Version M3SV2:
void dcs_processReceived(unsigned char pData)
{
	static  unsigned short byteCnt = 0; // 0 beim ersten Programmaufruf
	static  unsigned char devComStatus = DCS_STATUS_DEFAULT;
	static  unsigned char crc = M3S_CRC_INITVAL;
	static  unsigned short upperBound = 0;			// höchster NUTZDATEN Arrayindex, Anzahl der Nutzdaten
	static  unsigned char* workspace = NULL;
	static  unsigned char devComReady = FALSE;
	
	unsigned char tmpProtocol;
	// Abfrage unten kann ich mir sparen, da ich in einer Bibliothek bin und die Interrupts sowieso nicht aufgerufen werden, solange ich die devCom nicht initialisiert habe..
	//if(currentSlave == NULL)
	//{
	//	return; // escape, Devcom not initialized!!
	//}
	
	byteCnt++;	// increase for next Byte, note beneath this line, Byte Numbering starts with 1, like good old VB-Times ;)
	
	
		
	if(devComReady && ((pData != 0x10)||(byteCnt!=1)))
	{
		if(byteCnt==1 || workspace==NULL)	// if first byte, select 
		{
			workspace = dcs_getAvailableWorkspace();
		}	
	
		workspace[byteCnt-1] = pData;		// save @ receive Buffer								
		crc = crc8(pData, crc);				// permanently calculate CRC, fastest implementation (?)
		
		if(byteCnt==3)						// Das dritte Byte ist entweder der CRC des Acknowledge oder das upperBound
		{
			if((workspace[0] & M3S_CTRLBYTE_PROTOCOL_gm) == M3S_CTRLBYTE_PROTOCOL_ACK_gc) // wenn Acknowledge war
			{				
				if(crc != 0)
				{
					devComReady = FALSE; // Framefehler, gehe in Undefinierten Modus
				}
				
				devComStatus |= DCS_STATUS_DORESET_bm;
			}
			else
			{
				upperBound = pData;
			}
		}
		
		if(byteCnt == ((upperBound+1) + M3S_OVERHEAD_LENGTH)) // Wenn erwartetes Frame empfangen wurde
		{
			devComStatus |= DCS_STATUS_DORESET_bm;
			
			tmpProtocol = workspace[M3S_BYTENUM_CTRLBYTE-1] & M3S_CTRLBYTE_PROTOCOL_gm;
				
			if(crc==0) // no error...
			{				
					switch(tmpProtocol)
					{
						case M3S_CTRLBYTE_PROTOCOL_DATA_gc:
								if(workspace[1] == currentSlave->Address)
								{
									if((workspace[M3S_BYTENUM_UPPERBOUND-1] == currentSlave->DataUpperBound) || (currentSlave->DataUpperBound==M3S_PARAM_UPPERBOUND_RECEIVE_ALL)) // Daten nur akzeptieren wenn passend oder wenn nichts eingestellt wurde..
									{
										dcs_sendAcknowledgeIfRequired(M3S_ACK_YES_gc, workspace[0]); // Acknowledge senden
										currentSlave->Data = (workspace + M3S_HEADER_LENGTH);
										currentSlave->NewReceived |= (1<<DCS_REC_DATA_bp);  				// Signal: Du hast neue DAten erhalten
									}
									else
									{
										dcs_sendAcknowledgeIfRequired(M3S_ACK_NO_gc, workspace[0]); // Acknowledge senden
									}
								}							
								break;


						case M3S_CTRLBYTE_PROTOCOL_CMD_gc:
								if(workspace[1] == currentSlave->Address)
								{								
									dcs_processCmd(workspace);						// Kommando verarbeiten								
								}								
																											
								
								break;

						case M3S_CTRLBYTE_PROTOCOL_CMDBROADCAST_gc:	
								if((workspace[1] == M3S_BROADCAST_ADDRESS) || (workspace[1] == currentSlave->MCAddress))	// wenn kein Multicast oder für meine Multicastadresse							
								{
									dcs_processCmd(workspace);						// Kommando verarbeiten
								}								
													
								break;
						
						case M3S_CTRLBYTE_PROTOCOL_BROADCAST_gc:									
								
								if((workspace[M3S_BYTENUM_SLAVEADDRESS-1] == currentSlave->MCAddress) || (workspace[M3S_BYTENUM_SLAVEADDRESS-1] == M3S_BROADCAST_ADDRESS)) // Broadcast oder meine Multicastadresse
								{									
									if(workspace[M3S_BYTENUM_UPPERBOUND-1] == currentSlave->DataUpperBound ||workspace[M3S_BYTENUM_UPPERBOUND-1] == M3S_PARAM_UPPERBOUND_RECEIVE_ALL) // Richtige länge oder ich darf eh alles empfangen
									{
										currentSlave->Data = (workspace + M3S_HEADER_LENGTH);
										
										if(workspace[M3S_BYTENUM_SLAVEADDRESS-1] == M3S_BROADCAST_ADDRESS)
										{
											currentSlave->NewReceived |= (1<<DCS_REC_DATA_BC_bp);			// signalsiere Daten wurden via Broadcast empfangen									
										}
										else
										{
											currentSlave->NewReceived |= (1<<DCS_REC_DATA_MC_bp);			// signalsiere Daten wurden via Multicast empfangen									
										}										
									}	
								}									
																									
								// else wars ein Multicast
																								
								break;
					}
					
					workspace = NULL;								
			}	
			else
			{
				devComReady = 0; // enter idle mode..				
			}		
		}		
	}	
	else
	{
		// Wait for valid Reset	
		devComReady = FALSE;	
		switch(byteCnt)
		{
			case 1: if(pData != M3S_RESETFRAME_BYTE1)
					{
						devComStatus |= DCS_STATUS_DORESET_bm;
					}
					break;
					
			case 2: if(pData != M3S_RESETFRAME_BYTE2)
					{
						devComStatus |= DCS_STATUS_DORESET_bm;
					}
					break;
					
			case 3: if(pData != M3S_RESETFRAME_BYTE3)
					{
						 devComStatus |= DCS_STATUS_DORESET_bm;
					}
					break;
					
			case 4: if(pData != M3S_RESETFRAME_BYTE4)
					{
						devComStatus |= DCS_STATUS_DORESET_bm; 
					}
					break;	
										
			case 5: if(pData == M3S_RESETFRAME_BYTE5)
					{
						devComReady = TRUE;
					}
					
					devComStatus |= DCS_STATUS_DORESET_bm;
					break; 
					
			default: devComStatus |= DCS_STATUS_DORESET_bm; devComReady = FALSE; break;
		}
	}
	
	
	if(devComStatus & DCS_STATUS_DORESET_bm)
	{
		crc = M3S_CRC_INITVAL;		
		devComStatus = DCS_STATUS_DEFAULT;
		byteCnt = 0;								
		upperBound = 0;			
		workspace = NULL;
	}	
}

void dcs_sendAcknowledgeIfRequired(unsigned char pYesNo, unsigned char pLastCtrlByte)
{
	unsigned char AckFrame[M3S_ACK_FRAME_LENGTH];
	unsigned char protocol = (pLastCtrlByte & M3S_CTRLBYTE_PROTOCOL_gm);

	if(protocol == M3S_CTRLBYTE_PROTOCOL_CMD_gc || protocol == M3S_CTRLBYTE_PROTOCOL_DATA_gc || protocol == M3S_CTRLBYTE_PROTOCOL_FILETRANSFER_gc) // Wenn unicastprotokoll
	{
		if(pLastCtrlByte & M3S_CTRLBYTE_ACK_OR_MC_bm) // if acknowledge was requested
		{
			pLastCtrlByte &= ~M3S_CTRLBYTE_PROTOCOL_gm; 	// clear protocol
			pLastCtrlByte |= M3S_CTRLBYTE_PROTOCOL_ACK_gc; 	// Protocol Acknowledge setzen
			pLastCtrlByte &= ~(3<<M3S_CTRLBYTE_SS_bp);		// clear lower 2 flags
			
			pLastCtrlByte |= pYesNo;						// Acknowledge oder NOT Acknowledge
			pLastCtrlByte |= (1<<M3S_CTRLBYTE_SS_bp);		// Slave Send
	
			AckFrame[0] = pLastCtrlByte;					// Datenframe zusammenstellen...
			AckFrame[1] = currentSlave->Address;
			AckFrame[2] = crc8_frameBased(AckFrame, M3S_ACK_FRAME_LENGTH-M3S_CRC_LENGTH, M3S_CRC_INITVAL);

			dcs_send(AckFrame, sizeof(AckFrame)); 	
		}					
	}	
}


void dcs_sendFrame(unsigned char pCtrlByte, unsigned char* pData, unsigned char pDataLength)
{
	
	unsigned char* Frame = dcs_getAvailableWorkspace(); 
	unsigned short i;
	
	
	Frame[0] = pCtrlByte | (1<<M3S_CTRLBYTE_SS_bp) | (1<<M3S_CTRLBYTE_ACK_OR_MC_bp);		// Auf alle Fälle nochmal das Slave Send bit und implizites Acknowledge versenden.. setzen...
	Frame[1] = currentSlave->Address;
	Frame[2] = pDataLength-1;										// Datenframe zusammenstellen...
	
	for(i=0; i<pDataLength; i++)
	{
		Frame[i+M3S_HEADER_LENGTH] = pData[i];								// Daten kopieren und Verpacken...
		
	}
	
		
	Frame[M3S_HEADER_LENGTH+pDataLength] = crc8_frameBased(Frame, pDataLength+M3S_HEADER_LENGTH, M3S_CRC_INITVAL);	
	
	dcs_send(Frame, M3S_OVERHEAD_LENGTH + pDataLength); 			 						
}


void dcs_sendInformation(unsigned char pLastCtrlByte)
{	
	unsigned char* frame;			// frame das versendet wird
	
	frame = dcs_getAvailableWorkspace();
		
	frame[0] = pLastCtrlByte | (1<<M3S_CTRLBYTE_PROTOCOL_FLAG_RESPONSE_bp) | (1<<M3S_CTRLBYTE_SS_bp) | (1<<M3S_CTRLBYTE_ACK_OR_MC_bp); 
	frame[1] = currentSlave->Address;	
	frame[2] = 9-1;							// UpperBound
	frame[3] = DCS_VERSION;
	frame[4] = DCS_SUBVERSION;
	frame[5] = DCS_REVISION;	
	frame[6] = DEVCOM_TYPE_SLAVE_ATMEGA;
	frame[7] = currentSlave->Address;
	frame[8] = currentSlave->MCAddress;
	frame[9] = (uint8_t)((currentSlave->ID >> 8) & 0xFF);
	frame[10] = (uint8_t)((currentSlave->ID) & 0xFF);
	frame[11] = currentSlave->DataUpperBound;
	
	frame[12] = crc8_frameBased(frame, 12, M3S_CRC_INITVAL);	// CRC

	dcs_send(frame, 13);	
}

// Based on success of setting data, returns 1 (successful) or 0 (not successful)
uint8_t dcs_setData(uint8_t* rData, uint8_t vDataLength)
{	
	unsigned short cnt=0;
	
	if((vDataLength-1 != currentSlave->DataUpperBound) && (currentSlave->DataUpperBound != 0))
	{
		return(0); // false
	}
	
	if(rData == NULL)
	{
		return(0);
	}
	
	if(currentSlave->Data == NULL) // wenn bis jetzt noch nicht initialisiert, nimm dir einen workspace
	{
		currentSlave->Data = dcs_getAvailableWorkspace() + M3S_HEADER_LENGTH;
	}
	
	for(cnt=0; cnt<vDataLength; cnt++)
	{
		currentSlave->Data[cnt] = rData[cnt]; // kopieren
	}
	
	return(1); // return success..
}

void dcs_sendData(unsigned char vLastCtrlByte)
{
	// daten liegen ja noch im nicht verwendeten Buffer, verwende also gleich die Bytes davor
	unsigned char* frameStartPtr = (unsigned char*)(currentSlave->Data) - M3S_HEADER_LENGTH;
	unsigned char sendUpperBound;
	
	if(currentSlave->DataUpperBound == 0)
	{
		sendUpperBound= 0xff; // Undefinierter Upperbound	=> ganzen Buffer senden
	}
	else
	{
		sendUpperBound = currentSlave->DataUpperBound; // Definierten Upperbound
	
	}
	
	*(frameStartPtr+2) = sendUpperBound;	
	
	*(frameStartPtr+1) = currentSlave->Address;		// Slaveadresse
	*(frameStartPtr) = vLastCtrlByte | (1<<M3S_CTRLBYTE_ACK_OR_MC_bp) |(1<<M3S_CTRLBYTE_PROTOCOL_FLAG_RESPONSE_bp) | (1<<M3S_CTRLBYTE_SS_bp);
	
	*(currentSlave->Data + sendUpperBound+1) = crc8_frameBased(frameStartPtr, M3S_HEADER_LENGTH+(sendUpperBound+1), M3S_CRC_INITVAL);
	
	dcs_send(frameStartPtr, sendUpperBound+1+M3S_OVERHEAD_LENGTH);
}

void dcs_processCmd(unsigned char* pCmdFrame)
{
	ackToSend = TRUE;		// Prinzipiell ist noch ein Ack zu senden, sollte allerdings irgendwer schon ein implizites Acknowledge raushauen, dann brauch ich nicht mehr...
	unsigned char ackYesNo = M3S_ACK_YES_gc;		// prinzipiell gehts gut
		
	switch(pCmdFrame[0+M3S_HEADER_LENGTH])
	{
				case DC_CMD_GET_INFORMATION:		if((pCmdFrame[0] & M3S_CTRLBYTE_PROTOCOL_gm) == M3S_CTRLBYTE_PROTOCOL_CMD_gc) // Nur wenns ein Singlecast war beantworten
													{
														dcs_sendInformation(pCmdFrame[0]); // returns version information and serialized object
													}														
													break;
													
				case DC_CMD_SET_MULTICASTADDR:		currentSlave->MCAddress = pCmdFrame[1+M3S_HEADER_LENGTH];
													break;
													
				case DC_CMD_CHANGE_BAUDRATE:		dc_baudrate = ((uint32_t)(pCmdFrame[1+M3S_HEADER_LENGTH])<<24) | ((uint32_t)(pCmdFrame[2+M3S_HEADER_LENGTH])<<16) | ((uint32_t)(pCmdFrame[3+M3S_HEADER_LENGTH])<<8) | (uint32_t)(pCmdFrame[4+M3S_HEADER_LENGTH]);
													changeBaudrateFlag=1; // Kann hier gesetzt werden, da der Interrupt Transmit Complete eh erst nach dem Senden aufgerufen wird...
													break;
													
				case DC_CMD_PING:					// Do nothing, ping wird eh unten beantwortet...
													break;
				case DC_CMD_GETDATA:				if(currentSlave->Data != NULL)
													{
														dcs_sendData(pCmdFrame[0]);
													}
													else
													{
														ackYesNo = M3S_ACK_NO_gc; // signal not acknowledge...
													}
													break;									
				// last parameter in this function: Answer only allowed, if it was a singlecast...
				default:	if(currentSlave->CommandHandler != NULL) 
							{
								if(currentSlave->CommandHandler(currentSlave, (pCmdFrame+M3S_HEADER_LENGTH),pCmdFrame[0],pCmdFrame[2], ((pCmdFrame[0] & M3S_CTRLBYTE_PROTOCOL_gm) == M3S_CTRLBYTE_PROTOCOL_CMD_gc)) == DCS_CMDHANDLER_SUCCESS)
								{
									 ackYesNo = M3S_ACK_YES_gc;
								}					
								else
								{
									ackYesNo = M3S_ACK_NO_gc;
								}												
							}								
							break;										
	}	
	
	if(ackToSend && ((pCmdFrame[0]& M3S_CTRLBYTE_PROTOCOL_gm) == M3S_CTRLBYTE_PROTOCOL_CMD_gc))	// wenn acknowledge noch zu versenden und es ein unicast war...
	{
		dcs_sendAcknowledgeIfRequired(ackYesNo, pCmdFrame[0]);	// Acknowledge senden
	}			
}

	  

