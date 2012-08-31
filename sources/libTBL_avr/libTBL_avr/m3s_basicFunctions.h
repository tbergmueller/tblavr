/*
 * m3s_basicFunctions.h
 *
 * Created: 13.12.2011 14:46:41
 *  Author: tbergmueller
 */ 


#ifndef M3S_BASICFUNCTIONS_H_
	#define M3S_BASICFUNCTIONS_H_


	#define M3S_CHKSUM_CODE_MSB						'T'
	#define M3S_CHKSUM_CODE_SCND					'O'
	#define M3S_CHKSUM_CODE_LSB						'M'

	// Teilrahmen, Rahmenl�ngen
	#define M3S_CHECKSUM_LENGTH						3
	#define M3S_STARTSEQUENCE_LENGTH 				3
	#define M3S_ACK_FRAME_LENGTH					5

	// Controlbyte
	#define M3S_CTRLBYTE_PROTOCOL_gm				0xF0


	#define M3S_CTRLBYTE_PROTOCOL_RESET_gc			0x10
	#define M3S_CTRLBYTE_PROTOCOL_BROADCAST_gc		0x20
	#define M3S_CTRLBYTE_PROTOCOL_DATA_gc			0x30
	#define M3S_CTRLBYTE_PROTOCOL_CMD_gc			0x40
	#define M3S_CTRLBYTE_PROTOCOL_ACK_gc			0x80
	#define M3S_CTRLBYTE_PROTOCOL_CMDBROADCAST_gc   0x90
	#define M3S_CTRLBYTE_PROTOCOL_FILETRANSFER_gc	0xA0

	// Bit Positions	
	#define M3S_CTRLBYTE_ACK_OR_MC_bp					1
	#define M3S_CTRLBYTE_ACK_OR_MC_bm					(1<<M3S_CTRLBYTE_ACK_OR_MC_bp)
	#define M3S_CTRLBYTE_SS_bp							0
	#define M3S_CTRLBYTE_PROTOCOL_FLAG_RESPONSE_bp 		4

	// Group positions
	#define M3S_CTRLBYTE_MASTERADDR_gp					2
	 
	// Acknowledge
	#define M3S_ACK_YES_gc								0x02
	#define M3S_ACK_NO_gc								0x00
	#define M3S_MC_YES_gc								M3S_ACK_YES
	#define M3S_MC_NO_gc								M3S_ACK_NO


	
	#define M3S_CTRLBYTE_RESET						0x10
	#define M3S_DATABYTE_RESET						'R'

	#define M3S_CTRLBYTE_SLAVEREQUEST				0xD1

	// Positionen im �bertragungsrahmen
	#define M3S_CTRLBYTE_BYTENUM					1
	#define M3S_SLAVEADDRESS_BYTENUM				2
	#define M3S_UPPERBOUND_BYTENUM					3

	#define M3S_UPPERBOUND_IDX						2



	#define M3S_ERROR_FRAME_NOT_ACK_FRAME_gc		0x0A
	#define M3S_ERROR_CHECKSUM_ERROR_gc				0x0C
	#define M3S_ERROR_NOERROR_gc					0x00
	#define M3S_ERROR_WRONG_SLAVEADDR_gc			0x01



	
	// Prototypen
	unsigned char m3s_controlByte_isReset(unsigned char pData);			
	// �berpr�ft ob ein �bergebenes Byte dem Bitmuster eines Controlbytes f. Restfolge entspricht
	// Parameter unsigned char Control Byte
	// R�ckgabe: 0 oder 1

	unsigned char m3s_checkSumIsEqual(unsigned long pChkSum, unsigned long pChkSumRec);
	// Vergleicht zwei longChecksummen, wobei die erste gerechnet ist (wird intern mit dem Code XORed) und die zweite die empfangene.
	// Parameter: gerechnete Pr�fsumme, empfangene Pr�fsumme
	// R�ckgabe: 0 oder 1

	unsigned long m3s_checkSumXor(unsigned long pChkSum);
	// Codiert / Decodiert eine Pr�fsumme (3 Byte) XOR mit dem oben definierten Code
	// Parameter: zu XORende Pr�fsumme in unsigned long
	// R�ckgabe: 1x XORed Pr�fsumme in unsigned long

	unsigned char m3s_getProtocolOutOf(unsigned char pCheck);
	// Legt eine Bitmaske �ber ein �bergebenes Byte und liefert das Protokoll an der URSPR�NGLICHEN Position wieder zur�ck (restliche Bits 0)
	// Parameter: unsigned char Control Byte
	// R�ckgabe: modifiziertes pData-Byte



#endif /* M3S_BASICFUNCTIONS_H_ */