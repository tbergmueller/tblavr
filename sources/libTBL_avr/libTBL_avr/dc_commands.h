/*
 * dc_commands.h
 *
 * Created: 20/03/2012 21:36:18
 *  Author: thomas
 */ 


#ifndef DC_COMMANDS_H_
#define DC_COMMANDS_H_
	
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PRE-DEFINED COMMAND CODES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
																
																
	//			N A M E   							Code				Resp.requ.	Description
	// ------------------------------------------------------------------------------------	
	//  ~~~~~~~~~~~~ Internal implemented Commands
															
		#define DC_CMD_GET_INFORMATION				'i'				//	  Yes		Returns Version Information and Serialized DevCom-Struct
		#define DC_CMD_SET_MULTICASTADDR			'M'				//    No		Sets Multicast Address to a new value
		#define DC_CMD_FILEANNOUNCE					'G'				//	  Yes		A File is announced, slave checks ressources and responds  (Parameters: Filename, Size, ..)
		#define DC_CMD_CHANGE_BAUDRATE				'B'				//	  No		Changes the Baudrate to passed Value, only reachable with new baudrate after executing this command...
		#define DC_CMD_PING							'P'				//	  No		Via Pinging the reachability of a Slave can be determined, answered by Acknowledge
		#define DC_CMD_GETDATA						'd'				//	  Yes		Returns Data (as long as Data is set..)	
	// -------------------------------------------------------------------------------------	
	// ~~~~~~~~~~~~~ Not Implemented Commands, these are passed to DevComSlave_t.CommandHandler() - Callback Method
	
	//	#define DCS_CMD_GETPIXELS					0x0A			//	  Yes		OUTDATED, isn't used any more since V1.2, use GetInformation ('i') instead
	
		#define DC_CMD_DATASCALE					0x0D			//	  N o		Scales Data, processed internally, 
		
		
		#define DC_CMD_CONFIGDATA					'S'				//	  N o		As Parameter Configuration Data is Received
		#define DC_CMD_CONFIGDATALENGTHREQUEST		'L'				//	  Yes		Master requests the Length of Slaves Configuration Data, Slave should respond
		#define DC_CMD_CONFIGDATAREQUEST			'T'				//    Yes		Master requests the Slaves Configuration Data, Slave should respond
		
		#define DC_CMD_PWM_PRESCALER				'p'				//	  N o		As a parameter the PWM-Prescaler is passed, used for controlling LED or other power devices		
		#define DC_CMD_UPDATE						'U'				//	  N o		Global update Command (e.g. Write Configuration to EEPROM, ...)
		
		#define DC_CMD_SET_DATETIME					'd'				//	  N o		Write Date and Time to Realtime-Clock
		#define DC_CMD_GET_DATETIME					'D'				//    Yes		Read Date and Time
		
		#define DC_CMD_GET_DATALOGENTRY				'l'				//	  Yes		Read specific Data log Entry	
		#define DC_CMD_GET_DATALOG_INFO				'I'				//	  Yes		Get Information about Data log
	
		void dcs_atmega88_rw_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate); // DELETE, was for debugging..




#endif /* DC_COMMANDS_H_ */