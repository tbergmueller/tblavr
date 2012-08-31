/*
###############################################################
######################## M3S.c ################################
################ Multi Master Multi Slave######################
###############################################################

-	Version: 1.0a
-	Last Changed: 1.12.2010
-	Programmierer: Thomas Bergmüller

+ 	Liste mit Änderungen siehe unten

- 	Funktion:  			Einige protokollspezifische Funktionen 
						zum M3S-Protokoll, die hier ausprogrammiert wurden,
						da sie in der devCom öfter als einmal benötigt werden.

-	Bekannte Mängel:
						momentan keine
						
#################################################################				
*/

#include "m3s_basicFunctions.h"

unsigned long m3s_checkSumXor(unsigned long pChkSum)
{
	// folgende drei Zeilen brauchen nicht mehr als return(pCheckSum ^ 0x544F4D), so ist es aber wartbarer
	pChkSum ^= ((unsigned long)(M3S_CHKSUM_CODE_MSB) << 16);
	pChkSum ^= ((unsigned long)(M3S_CHKSUM_CODE_SCND) << 8);
	pChkSum ^= ((unsigned long)(M3S_CHKSUM_CODE_LSB));

	return(pChkSum);
}


// ############################# MASTER FUNCTIONS ##########################

unsigned char m3s_checkSumIsEqual(unsigned long pChkSum, unsigned long pChkSumRec)
{	
	pChkSumRec = m3s_checkSumXor(pChkSumRec);
	
	if(pChkSum == pChkSumRec)
	{
		return(1);
	}
	else
	{
		return(0);
	}	
}

unsigned char m3s_getProtocolOutOf(unsigned char pCheck)
{
	// Prüft ob es sich um ein gültiges Ctrlbyte handelt...

	if((pCheck & M3S_CTRLBYTE_PROTOCOL_gm) == 0xF0)
	{
		pCheck &= 0x0F;
	}

	return(pCheck & M3S_CTRLBYTE_PROTOCOL_gm);

}


