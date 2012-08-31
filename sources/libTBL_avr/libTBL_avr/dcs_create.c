/*
 * dcs_setDefaultParameters.c
 *
 * Created: 27/03/2012 23:13:13
 *  Author: thomas
 */ 


#include "DevComSlave.h"

DevComSlave_t hiddenGlobalDevComSlave;	// This is definetly the worst way I allocated memory, but malloc needs way to much time and code space ... Better Option here?

DevComSlave_t* dcs_create(unsigned char SlaveAddress)
{
	unsigned char cnt;
	
	for(cnt=0; cnt<sizeof(hiddenGlobalDevComSlave); cnt++)
	{
		*(unsigned char*)((&hiddenGlobalDevComSlave+cnt))=0;
	}
	
	hiddenGlobalDevComSlave.Address = SlaveAddress;
	
	// Default Values
	hiddenGlobalDevComSlave.RWPolarity = 1;
	hiddenGlobalDevComSlave.MCAddress = 1;
		
	return(&hiddenGlobalDevComSlave);
}