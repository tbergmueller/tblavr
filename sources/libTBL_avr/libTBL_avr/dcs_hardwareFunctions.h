/*
 * dcs_hardwareFunctions.h
 *
 * Created: 20.12.2011 17:06:42
 *  Author: tbergmueller
 */ 


#ifndef DCS_HARDWAREFUNCTIONS_H_
	#define DCS_HARDWAREFUNCTIONS_H_


	#include "preProcessorDirectives.h"



	// ~~~~~~~~~~~~~~~~~~~~~~~ Anpassen an Hardwaregegebenheiten ~~~~~~~~~~~~~~~~~~~~~~~



	
	
	// BUILDING THE INIT-FUNCTION
	
	#define _FIRSTPART			CONCAT(dcs_,PROCESSOR)
	
	#ifdef USART_NR
		#define _BEFMIDDLEPART	CONCAT(_FIRSTPART,_usart)
		#define _MIDDLEPART		CONCAT(_BEFMIDDLEPART, USART_NR)
	#else	
		#define _MIDDLEPART		 _FIRSTPART
	#endif
		
	#define DCS_BASIC_INIT			CONCAT(_MIDDLEPART,_rw_init)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// ##################################### DO NOT CHANGE BELOW THIS LINE!!!

	// +++++++++++++++++++++++++++++++++++++++++++++++++++ ATMEGA 32
	#ifdef __AVR_ATmega32__
		#define DEVCOM_UART_PORT		PORTD	// Port, an dem die verwendete UART sitzt
		#define DEVCOM_UART_PIN 		PIND	// Analog dazu Pin, an dem die verwendete UART sitzt
		#define DEVCOM_UART_DDR			DDRD    // Datenrichtungsregister des Ports, an dem die verwendete UART sitzt

		#define DEVCOM_UART_TXD_bp 		PD1		// Bitposition (0..7) der TXD-Leitung
		#define DEVCOM_UART_RXD_bp 		PD0		// Bitposition (0..7) der RXD-Leitung

		#define DEVCOM_UART_UBRRH		UBRRH	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRRL	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSRA	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSRB	// UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRC		UCSRC	// UCSRC der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSZ1_bp	UCSZ1	// ?? Mode select oder so
		#define DEVCOM_UART_UCSZ0_bp	UCSZ0	// ?? Mode select oder so
		#define DEVCOM_UART_URSEL		URSEL	// register selection bit
		
		#define DEVCOM_UART_UDRE_bp		UDRE 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART_RXC_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART_TXC_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif

	// +++++++++++++++++++++++++++++++++++++++++++++++++++ ATMEGA 8
	#ifdef __AVR_ATmega8__
		#define DEVCOM_UART_PORT		PORTD	// Port, an dem die verwendete UART sitzt
		#define DEVCOM_UART_PIN 		PIND	// Analog dazu Pin, an dem die verwendete UART sitzt
		#define DEVCOM_UART_DDR			DDRD    // Datenrichtungsregister des Ports, an dem die verwendete UART sitzt

		#define DEVCOM_UART_TXD_bp 		PD1		// Bitposition (0..7) der TXD-Leitung
		#define DEVCOM_UART_RXD_bp 		PD0		// Bitposition (0..7) der RXD-Leitung

		#define DEVCOM_UART_UBRRH		UBRRH	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRRL	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSRA	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSRB	// UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRC		UCSRC	// UCSRC der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSZ1_bp	UCSZ1	// ?? Mode select oder so
		#define DEVCOM_UART_UCSZ0_bp	UCSZ0	// ?? Mode select oder so
		

		#define DEVCOM_UART_UDRE_bp		UDRE 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART_RXC_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART_TXC_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif
		
	#if defined __AVR_ATmega644__ 	
		#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

		#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART0_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART0_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif
		
	#if defined __AVR_ATmega640__ || defined __AVR_ATmega1280__ || defined __AVR_ATmega2560__ 
		#ifdef USART_NR
			#if USART_NR == 0
				#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART0_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART0_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
		#endif
		
		#if USART_NR == 3
				#define DEVCOM_UART_UBRRH		UBRR3H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR3L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR3A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR3B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X3		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN3	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN3	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE3	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE3	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE3 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR3		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART3_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART3_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
		#endif
		
		#if USART_NR == 2
				#define DEVCOM_UART_UBRRH		UBRR2H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR2L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR2A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR2B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X2		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN2	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN2	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE2	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE2	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE2 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR2		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART2_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART2_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
		#endif
		
		#if USART_NR == 1
				#define DEVCOM_UART_UBRRH		UBRR1H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR1L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR1A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR1B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X1		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN1	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN1	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE1	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE1	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE1 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR1		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART1_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART1_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
		#endif
	
			
		#else
			#error Compilationerror: USART_NR not definied. Define like n, e.g. usart0 would be 0
		#endif		//
	#endif	
		
		
	#if defined __AVR_ATmega644A__ || defined __AVR_ATmega644P__ || defined __AVR_ATmega644PA__ 
		#ifdef USART_NR
			#if USART_NR==0
				#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART0_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART0_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
			#endif
			#if USART_NR==1
				#define DEVCOM_UART_UBRRH		UBRR1H	// UBRRH der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UBRRL		UBRR1L	// UBRRL der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UCSRA		UCSR1A	// UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_UCSRB		UCSR1B	// UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_U2X			U2X1		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_TXEN		TXEN1	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
				#define DEVCOM_UART_RXEN		RXEN1	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_RXCIE		RXCIE1	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
				#define DEVCOM_UART_TXCIE		TXCIE1	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle
		

				#define DEVCOM_UART_UDRE_bp		UDRE1 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

				#define DEVCOM_UART_UDR			UDR1		// Datenregister der UDR

				#define DEVCOM_UART_RX_vect		USART1_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
				#define DEVCOM_UART_TX_vect		USART1_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
			#endif
		#else
			#error Compilationerror: USART_NR not definied. Define like n, e.g. usart0 would be 0
		#endif		//
	#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++ ATMEGA 88(P)(A)

	#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || __AVR_ATmega88P__ || __AVR_ATmega88PA__
		#define DEVCOM_UART_PORT		PORTD	// Port, an dem die verwendete UART sitzt
		#define DEVCOM_UART_PIN 		PIND	// Analog dazu Pin, an dem die verwendete UART sitzt
		#define DEVCOM_UART_DDR			DDRD    // Datenrichtungsregister des Ports, an dem die verwendete UART sitzt

		#define DEVCOM_UART_TXD_bp 		PD0		// Bitposition (0..7) der TXD-Leitung
		#define DEVCOM_UART_RXD_bp 		PD1		// Bitposition (0..7) der RXD-Leitung


		#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRC		UCSR0C	// UCSRC der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSZ1_bp	UCSZ01	// ?? Mode select oder so
		#define DEVCOM_UART_UCSZ0_bp	UCSZ00	// ?? Mode select oder so

		#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++ ATMEGA 2560 UART 0
	// +++++++++++++++++++++++++++++++++++++++++++++++++++ ATMEGA 162
/*
	#if PROCESSOR == atmega2560_usart0
		#define DEVCOM_UART_PORT		PORTE	// Port, an dem die verwendete UART sitzt
		#define DEVCOM_UART_PIN 		PINE	// Analog dazu Pin, an dem die verwendete UART sitzt
		#define DEVCOM_UART_DDR			DDRE    // Datenrichtungsregister des Ports, an dem die verwendete UART sitzt

		#define DEVCOM_UART_TXD_bp 		PE1		// Bitposition (0..7) der TXD-Leitung
		#define DEVCOM_UART_RXD_bp 		PE0		// Bitposition (0..7) der RXD-Leitung


		#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRC		UCSR0C	// UCSRC der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSZ1_bp	UCSZ01	// ?? Mode select oder so
		#define DEVCOM_UART_UCSZ0_bp	UCSZ00	// ?? Mode select oder so

		#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART0_RX_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART0_TX_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif

	#if PROCESSOR == atmega162_usart0
		#define DEVCOM_UART_PORT		PORTD	// Port, an dem die verwendete UART sitzt
		#define DEVCOM_UART_PIN 		PIND	// Analog dazu Pin, an dem die verwendete UART sitzt
		#define DEVCOM_UART_DDR			DDRD    // Datenrichtungsregister des Ports, an dem die verwendete UART sitzt

		#define DEVCOM_UART_TXD_bp 		PD1		// Bitposition (0..7) der TXD-Leitung
		#define DEVCOM_UART_RXD_bp 		PD0		// Bitposition (0..7) der RXD-Leitung


		#define DEVCOM_UART_UBRRH		UBRR0H	// UBRRH der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UBRRL		UBRR0L	// UBRRL der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSRA		UCSR0A	// UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRB		UCSR0B	// UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_UCSRC		UCSR0C	// UCSRC der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_U2X			U2X0		// U2X (double speed mode) im UCSRA der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_TXEN		TXEN0	// Transmit enable im UCSRB der entsprechenden seriellen Schnittstelle
		#define DEVCOM_UART_RXEN		RXEN0	// Receive enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_RXCIE		RXCIE0	// Receive Interrupt enable im UCSRB der entsprechenden seriellen Schnittstele 
		#define DEVCOM_UART_TXCIE		TXCIE0	// Transmit interrupt enable im UCSRB der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UCSZ1_bp	UCSZ01	// ?? Mode select oder so
		#define DEVCOM_UART_UCSZ0_bp	UCSZ00	// ?? Mode select oder so
		#define DEVCOM_UART_URSEL		URSEL0

		#define DEVCOM_UART_UDRE_bp		UDRE0 	// Data register empty im UCSRA der entsprechenden seriellen Schnittstelle

		#define DEVCOM_UART_UDR			UDR0		// Datenregister der UDR

		#define DEVCOM_UART_RX_vect		USART0_RXC_vect		// Receive-Interrupt Vektor der entsprechenden seriellen Schnittstelle	
		#define DEVCOM_UART_TX_vect		USART0_TXC_vect		// Transmit-Interrupt vektor der entsprechenden seriellen Schnittstelle
	#endif
*/

	// Send-Function, Regardless if Max or not Max
	void dcs_send(unsigned char* chArr, unsigned short anzBytes);
	// Implemented in dcs_hardwareFunctions.h
	
	void dcs_setBaudrate();
	// Sets baudrate...
	
	// Schreibt ein Zeichen auf die Usart
	void dcs_usart_putc(unsigned char);
	
	
	
	#define DCS_RW_TO_READ_macro	if(currentSlave->RWPolarity) *currentSlave->RWPort &= ~(1<<currentSlave->RW_bp); else *currentSlave->RWPort |= (1<<currentSlave->RW_bp);
	#define DCS_RW_TO_WRITE_macro	if(currentSlave->RWPolarity) *currentSlave->RWPort |= (1<<currentSlave->RW_bp); else *currentSlave->RWPort &= ~(1<<currentSlave->RW_bp);
	


#endif /* DCS_HARDWAREFUNCTIONS_H_ */