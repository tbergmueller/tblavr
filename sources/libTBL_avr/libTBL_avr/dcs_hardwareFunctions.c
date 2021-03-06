/*
 * dcs_hardwarefunctions.c
 *
 * Created: 20/03/2012 18:39:42
 *  Author: thomas
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "dcs_hardwareFunctions.h"
#include "dcs_internal.h"
#include "dcs_basicFunctions.h"


#include <avr/pgmspace.h>
#include "crc8.h"

	
// ########## CRC 8 gem. Dallas
/*
const unsigned char crc8_array[256] PROGMEM= {
      0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
      0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
      0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
      0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
      0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
      0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
      0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
      0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
      0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
      0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
      0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
      0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
      0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
      0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
      0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
      0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
      0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
      0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
      0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
      0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
      0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
      0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
      0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
      0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
      0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
      0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
      0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
      0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
      0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
      0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
      0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
      0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
      }; */ 
	 
	 const unsigned char crc8_array[256] = {
      0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
      0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
      0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
      0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
      0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
      0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
      0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
      0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
      0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
      0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
      0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
      0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
      0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
      0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
      0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
      0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
      0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
      0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
      0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
      0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
      0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
      0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
      0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
      0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
      0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
      0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
      0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
      0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
      0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
      0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
      0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
      0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
      }; 
 
 
 void dcs_processTxcInterrupt()
 {
	 if(currentSlave->RWPort != NULL)
	 {
		 DCS_RW_TO_READ_macro		// Wieder zur�ck auf Lesen schalten
	 }
	 
	 if(changeBaudrateFlag)
	 {
		 changeBaudrateFlag=0;
		 dcs_setBaudrate();
	 }
	 
	 #ifdef XMEGA_ARCH
		DEVCOM_UART.CTRLA &= ~(USART_TXCINTLVL_MED_gc);
	 #else
		 DEVCOM_UART_UCSRB &=  ~(1<<DEVCOM_UART_TXCIE);	// Interrupt wieder deaktivieren
	#endif
 }

 
 
 
 unsigned char crc8_frameBased(unsigned char* rFrame, unsigned short vFrameLength, unsigned char vInitialValue)
 {
    unsigned char curCrc = vInitialValue; // startwert
    unsigned short cnt;
    
    for(cnt=0; cnt < vFrameLength; cnt++)
    {
        curCrc = crc8(rFrame[cnt], curCrc);
    }   
    
    return(curCrc);                   
 }
 /*
 unsigned char crc8(unsigned char vByte, unsigned char vCurrentCrc)
 {
	 return(pgm_read_byte (&crc8_array[vByte ^ vCurrentCrc]));
 }*/
 
unsigned char crc8(unsigned char vByte, unsigned char vCurrentCrc)
{
	return(crc8_array[vByte ^ vCurrentCrc]);
}

void dcs_send(unsigned char* ch, unsigned short dataLength)
{
	unsigned short i;
	
		if(currentSlave->RWPort!= NULL)
		{
			DCS_RW_TO_WRITE_macro
		}	
	
	
	for(i=0; i<dataLength; i++)
	{		
		if(i == (dataLength - 1))
		{
			#ifdef XMEGA_ARCH
				DEVCOM_UART.CTRLA |= (USART_TXCINTLVL_MED_gc);
			#else
				DEVCOM_UART_UCSRB |=  (1<<DEVCOM_UART_TXCIE);	// enablen... sobald dann das letzte gesendet wurde gehts raus...
			#endif
		}	
		
		
		dcs_usart_putc(ch[i]);	
	}
	
	
	ackToSend = FALSE; // Wann immer ich etwas sende (i.d.R. Kommandoantworten oder Acknowledgeframes), ist das ACK-Bit gesetzt. Somit braucht kein explizites Ack mehr verschickt werden

}


// set the uart baud rate
void dcs_setBaudrate()
{	
	cli();
	
	#ifdef XMEGA_ARCH
	
	
	
		//#error "Hy mann, hier ist noch was zu tun.. baudrate momentan absolut"
	
		// *DEBUG* Fixe Baudrate & Scale 38400
		// TODOOO!!!
		DEVCOM_UART.BAUDCTRLA = (uint8_t) (3269 & 0x00FF);
		DEVCOM_UART.BAUDCTRLB = (uint8_t)(((0b1010 << USART_BSCALE_gp) & USART_BSCALE_gm) | ((3269>>8) & ~USART_BSCALE_gm));
	
	
	
	
	#else
		uint16_t bauddiv = ((dc_fcpu+(dc_baudrate*4L))/(dc_baudrate*8L)-1); // try it with doublespeed mode, rounded
	
		DEVCOM_UART_UCSRA  &= ~(1<<DEVCOM_UART_U2X); // double speed mode ausschalten...
	
		if(bauddiv>255L) // if prescaler is too high
		{
			bauddiv = ((dc_fcpu+(dc_baudrate*8L))/(dc_baudrate*16L)-1); // try it with normal mode, rounded
		}
		else// if doublespeed mode is ok
		{
			DEVCOM_UART_UCSRA  |= (1<<DEVCOM_UART_U2X); // double speed mode aktivieren...
		}		
	
		// Set baudrate registers
	 
		DEVCOM_UART_UBRRL = (uint8_t)(bauddiv);	
	
		#ifdef DEVCOM_UART_UBRRH
			DEVCOM_UART_UBRRH = (uint8_t)(bauddiv >> 8);
		#endif
	
	#endif	
	sei();
}


void dcs_stop(DevComSlaveRxCCallback_t rRxcCallbackFunction, DevComSlaveTxCCallback_t rTxcCallbackFunction)
{
	dcs_rxc_function = rRxcCallbackFunction;
	dcs_txc_function = rTxcCallbackFunction;
}



// TODO: undo...
void DCS_BASIC_INIT(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate)
{
	// Software Init
	currentSlave = rSlave;
	dc_fcpu = vFcpu;
	dc_baudrate = vBaudrate;
	changeBaudrateFlag = FALSE;	
	ackToSend = FALSE;
	
	currentSlave->SendFrame = dcs_sendFrame;	
	if(currentSlave->RWPort != NULL)
	{
		*DDR(currentSlave->RWPort) |= (1<<currentSlave->RW_bp); // RW-Line to Output	
			
		DCS_RW_TO_READ_macro	// Set to Reading/Receiving default		
	}	
	
	
	#ifdef XMEGA_ARCH
	
	
	
	DEVCOM_UART.CTRLA = DEVCOM_UART.CTRLB = DEVCOM_UART.CTRLC = 0; // init
	
	DEVCOM_UART.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;						// Receiver und Transmitter aktivieren
	DEVCOM_UART.CTRLA |= USART_RXCINTLVL_MED_gc;							// Receive Interrupt aktivieren
	DEVCOM_UART.CTRLC |= USART_CHSIZE_8BIT_gc;
		
	
	DEVCOM_UART_PORT.DIRSET = PIN7_bm;  // Txd auf Ausgang
	DEVCOM_UART_PORT.DIRCLR = PIN6_bm;  // RxD auf Eingang
	
	if(USART_NR %2)
	{
		DEVCOM_UART_PORT.PIN6CTRL = 0b011 << 3; // Pullup @ OPC register...
	}
	else
	{
		DEVCOM_UART_PORT.PIN2CTRL = 0b011 << 3; // Pullup @ OPC register...
	}	
	
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	
	
	dcs_setBaudrate(); // Baudrate setzen, calls sei();
	
	#else
	
	
		DEVCOM_UART_UCSRB   = 0;
		DEVCOM_UART_UCSRB  |= (1<<DEVCOM_UART_TXEN) | (1<<DEVCOM_UART_RXEN); 			// Receiver und Transmitter aktivieren
		DEVCOM_UART_UCSRB  |=  (1<<DEVCOM_UART_RXCIE);									// Receive Interrupt aktivieren
	#endif
	
	dcs_rxc_function = dcs_processReceived;
	dcs_txc_function = dcs_processTxcInterrupt;
	currentSlave->Stop = dcs_stop;
	// UCSRC remains default values... (8-N-1)
			
	dcs_setBaudrate(); // Baudrate setzen, calls sei();	
}


void dcs_usart_putc(unsigned char c)
{
	
	#ifdef XMEGA_ARCH
	
		while ((DEVCOM_UART.STATUS & USART_DREIF_bm)==0); 
	
		
		DEVCOM_UART.DATA = c; 										// Senden
		
	#else
		while ( !(DEVCOM_UART_UCSRA & (1<<DEVCOM_UART_UDRE_bp))); 		// warte bis Sendebuffer leer.
		
		DEVCOM_UART_UDR = c; 										// Senden
	#endif
}





// ############# Interrupt Vektoren ####################
SIGNAL(DEVCOM_UART_RX_vect) 
{	
	#ifdef XMEGA_ARCH
		dcs_rxc_function(DEVCOM_UART.DATA);
	#else
		dcs_rxc_function(DEVCOM_UART_UDR);
	#endif
}


 SIGNAL(DEVCOM_UART_TX_vect)  
{		
	dcs_txc_function();		
}
	