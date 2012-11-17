/*
 * mainTest.c
 *
 * Created: 20.12.2011 17:04:51
 *  Author: tbergmueller
 */ 

#include <avr/io.h>
#include "DevComSlave.h"
#include "avr_compiler.h"
#include "clksys_driver.h"

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
//void dcs_atmega88_rw_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);
void dcs_atxmega128a1_usart7_rw_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);

	
#define SLAVE_ADDRESS	10



char system_clocks_init(void)
{

	 /******************************************************************
   * System Clock 32MHz (XOSC Quarz 16MHz, PLL Faktor 2)
   ******************************************************************/

  /* Nach dem Reset ist die Quelle des Systemtaktes der interne
     2MHz RC-Oszillator (System Clock Selection: RC2MHz)
  */

  // Oszillator XOSC konfigurieren (12..16MHz, 256 clocks startup time)
  CLKSYS_XOSC_Config( OSC_FRQRANGE_12TO16_gc,
                      false,
                      OSC_XOSCSEL_XTAL_256CLK_gc );

  // Oszillator XOSC enable
  CLKSYS_Enable( OSC_XOSCEN_bm );

  // Warten bis der Oszillator bereit ist
  do {} while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );

  // PLL source ist XOSC, Multiplikator x2
  CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, 2 );

  // Enable PLL
  CLKSYS_Enable( OSC_PLLEN_bm );

  // Prescalers konfigurieren
  CLKSYS_Prescalers_Config(CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );

  // Warten bis PLL locked
  do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );

  // Main Clock Source ist Ausgang von PLL
  CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );

  // Nun ist der System Clock 32MHz !

  /* Hinweis:
     32kHz TOSC kann nicht in Verbindung mit PLL genutzt werden, da
     die minimale Eingangsfrequenz des PLLs 400kHz beträgt.
  */

	return(1);	
} // Sets System Clock to 32MHz



int main()
{
	system_clocks_init();
	
	
	DevComSlave_t* dc = dcs_create(SLAVE_ADDRESS);		
	
	// R/W Line of RS485 buscoupler
	dc->RWPort = ((uint8_t*)(&(PORTD.OUT)));
	dc->RW_bp = PIN5_bp;
	
	dcs_start(UARTD1, dc, F_CPU, 500000);	// Start @ UART0 with 500kbits	
				
	while(1)
	{
		if(dc->NewReceived & DCS_REC_DATA_gm)
		{
			// Do something, access Data ranging from dc.Data[0] .. dc.Data[dc.DataUpperBound]			
			dc->NewReceived &= ~(DCS_REC_DATA_gm); // Reset Flags
		}
	}	
}