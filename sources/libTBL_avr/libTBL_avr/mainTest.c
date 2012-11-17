/*
 * mainTest.c
 *
 * Created: 20.12.2011 17:04:51
 *  Author: tbergmueller
 */ 

#include <avr/io.h>
#include "DevComSlave.h"

uint8_t fooCmdHandler(const unsigned char* cmd, unsigned char lastCtrlByte, unsigned char upperBound, unsigned char responseAllowed)
{		
	switch(cmd[0])
	{
		case 0x0D: 
				// Handle Command 0x0D
				return(DCS_CMDHANDLER_SUCCESS); 
				break;
		default: break;
	}		
	// Command is unknown	
	return(DCS_CMDHANDLER_ERROR);
}
void dcs_atmega88_rw_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);
	
#define SLAVE_ADDRESS	10

int main()
{
	DevComSlave_t* dc = dcs_create(SLAVE_ADDRESS);	
	
	dcs_start(UART0, dc, F_CPU, 500000);	// Start @ UART0 with 500kbits	
				
	while(1)
	{
		if(dc->NewReceived & DCS_REC_DATA_gm)
		{
			// Do something, access Data ranging from dc.Data[0] .. dc.Data[dc.DataUpperBound]			
			dc->NewReceived &= ~(DCS_REC_DATA_gm); // Reset Flags
		}
	}	
}