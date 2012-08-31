/*
 * devcom_basicFunctions.h
 *
 * Created: 12.12.2011 22:42:27
 *  Author: tbergmueller
 */ 


#ifndef DEVCOM_BASICFUNCTIONS_H_
	#define DEVCOM_BASICFUNCTIONS_H_
	

	void dcs_processReceived(unsigned char pData);
	
	// Prototypen
	void dcs_sendAcknowledgeIfRequired(unsigned char pYesNo, unsigned char pLastCtrlByte); // Sendet Acknowledge zurück an den Master, braucht letztes Controlbyte und YES / NO
	unsigned char* dcs_processDataMultiBroadcast(const unsigned char* rDataPtr); // Processes Broadcasts und Multicasts of Data
	void dcs_processCmd(unsigned char* pCmdFrame);
	// Interpretiert das empfangene Kommando (evtl. mehrere Bytes) und leitet entsprechende Schritte ein
	// Parameter: Zeiger auf empfangene Daten (Kommandocode + Parameter), Controlbyte des Frames
	// Rückgabe: -
	
	void dcs_sendInformation(unsigned char pLastCtrlByte);
	// sendet einen Datenrahmen mit Verfügbaren Informationen über die DevCom-Implementierung
	// Parameter: letztes Controlbyte
	// Rückgabe: -
	
	void dcs_sendFrame(unsigned char pCtrlByte, unsigned char* pData, unsigned char pDataLength); // Sendet ein Frame vom Slave an den Master (dh. keine Ack-Behandlung)
	



#endif /* DEVCOM_BASICFUNCTIONS_H_ */