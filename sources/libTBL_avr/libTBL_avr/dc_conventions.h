/*
 * dc_conventions.h
 *
 * Created: 04.01.2012 22:53:18
 *  Author: tbergmueller
 */ 


#ifndef DC_CONVENTIONS_H_
	#define DC_CONVENTIONS_H_

	/* public enum DevComType: byte
	{
		SlaveAtmega = 0x01,
		SlaveXmega=0x02,
		SlaveWindows = 0x03,
		MasterAtmega=0x81,
		MasterXmega=0x82,
		MasterWindows=0x03,
	} */

	// Definitions of DevcomTypes, depends on implementation
	
	#define DEVCOM_TYPE_SLAVE_ATMEGA	0x01
	#define DEVCOM_TYPE_SLAVE_XMEGA		0x02
	#define DEVCOM_TYPE_SLAVE_WINDOWS	0x03
	
	#define DEVCOM_TYPE_MASTER_ATMEGA	0x81
	#define DEVCOM_TYPE_MASTER_XMEGA	0x82
	#define DEVCOM_TYPE_MASTER_WINDOWS	0x83


#endif /* DC_CONVENTIONS_H_ */