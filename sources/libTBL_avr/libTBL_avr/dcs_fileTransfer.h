/*
 * dcs_fileTransfer.h
 *
 * Created: 13.12.2011 14:50:14
 *  Author: tbergmueller
 */ 


#ifndef DCS_FILETRANSFER_H_
	#define DCS_FILETRANSFER_H_
	
	#include "dcs_config.h"
	
		
	#if DCS_CONF_FILETRANSFER == TRUE
	// File Handling
		#define DCS_FILETRANSFER_ANNOUNCED		0x01 // Neues File würde Angekündigt
		#define DCS_FILETRANSFER_RECEIVING		0x02 // Aktuell wurde eine Datei empfangen
	
		// Fehler
		#define DCS_FILETRANSFER_ERROR_NOERROR			0x00	// Analog zu TBL.Communication.Protocol.m3sExecutionError - Enum
		#define DCS_FILETRANSFER_ERROR_NOT_ENOUGH_MEM	0x05	// Analog zu TBL.Communication.Protocol.m3sExecutionError - Enum
		#define DCS_FILETRANSFER_ERROR_PACKAGE_NUMBER 	0x06	

		#define DCS_FILETRANSFER_DATABYTESINPACKAGE		252		// Anzahl der Datenbytes in einem M3S-Datenpaket. 256 Datenbytes - 4 Bytes Packagenumber

		// Prototypen
		unsigned char dcs_fileReceive(unsigned char* pData, unsigned char upperBound, unsigned char pReceiveStatus);
	#endif



#endif /* DCS_FILETRANSFER_H_ */