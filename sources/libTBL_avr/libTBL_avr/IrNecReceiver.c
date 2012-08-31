/*
 * irNec.c
 *
 * Created: 15/02/2012 18:34:10
 *  Author: thomas
 */ 

#include "IrNecReceiver.h"
#include "preProcessorDirectives.h"
 
#define NULL 0      
 
// ################## GENERAL TIMING 
#define LEADING_PULSE_DURATION_250US_MIN			15*2				//7.5ms		// Zeit in n*250US, Dauer des Startpulses, MINIMUM
#define LEADING_PULSE_DURATION_250US_MAX			10*4				// 10ms	// Zeit in n*250US, Dauer des Startpulses, MAXIMUM

#define PULSE_DURATION_250US_MIN					1			// 250us	// Zeit in n*250us, Dauer eines Pulses Minimum
#define PULSE_DURATION_250US_MAX					3			// 750us	// Zeit in n*250us, DAuer eines Pulses Maximum
 
#define SPACE_DURATION_HIGH_250US_MIN				4			// 1.5ms	// Zeit in n*250us, Dauer des Break nach Pulse für Logisch 1, MINIMUM
#define SPACE_DURATION_HIGH_250US_MAX				8			// 2ms		// Zeit in n*250us, Dauer des Break nach Pulse für Logisch 1, MAXIMUMK

#define SPACE_DURATION_LOW_250US_MIN				PULSE_DURATION_250US_MIN			// 250us	// Zeit in n*250us,Dauer des Break nach Pulse für Logisch 0, MINIMUM
#define SPACE_DURATION_LOW_250US_MAX				PULSE_DURATION_250US_MAX			// 750us	// Zeit in n*250us,Dauer des Break nach Pulse für Logisch 0, MAXIMUM


// ################## MESSAGE

#define MESSAGE_TIMEOUT_250US						15*4		// 15ms		// Zeit in n*250US Timeout in der Initial Message, höchste Statusdauer (Startpulse)

#define MESSAGE_SPACE_DURATION_250US_MIN			7*2			// 3.5ms		// Zeit in n*250US, Dauer des Space nach Startpuls, MINIMUM
#define MESSAGE_SPACE_DURATION_250US_MAX			10*2		// 5ms		// Zeit in n*250US, Dauer des Space nach Startpuls, MAXIMUM


// ################### REPEAT
#define REPEAT_TIMEOUT_250US						100*4		// 100ms		// Zeit in n*250us, die maximal vergehen darf bis 						
#define REPEAT_CODE_TIMEOUT_250US					13*4		// 13ms		// Zeit in n*250US, Timeout des Repeat Codes
#define REPEAT_SPACE_DURATION_250US_MIN				3*2			// 1.5ms	// Zeit in n*250US, Dauer des Space nach Startpuls, MINIMUM
#define REPEAT_SPACE_DURATION_250US_MAX				7*2			// 3.5ms		// Zeit in n*250US, Dauer des Space nach Startpuls, MAXIMUM		





typedef enum inrState {Idle,LeadingPulse,Space,Address,AddressInverse,Command,CommandInverse,EndPulse, WaitForRepeat, RepeatLeadingPulse,RepeatSpace} inrState_t;
typedef enum inrBitState {High, Low} inrBitState_t;

// Members


static void (*inr_intEn)()=NULL;
static void (*inr_intDis)()=NULL;
static void (*inr_timerOn)()=NULL;
static void (*inr_timerOff)()=NULL;

static uint8_t inr_invertedMode=0;
static uint8_t* inr_port=0;
static uint8_t inr_pin_bm=0;

inrState_t inr_status; 
inrBitState_t inr_bitstate;


static volatile unsigned short inr_overflowCnt = 0;		// Zähler in 250us Schritte (siehe Timer)
static volatile unsigned short inr_recTimeout=0;

#define INR_KEYSTATE_REPEATING_bm		(1<<0)
#define INR_KEYSTATE_ISPRESSED_bm		(1<<1)

static volatile uint8_t inr_keystate=0;


uint8_t inr_logicPinstate();
void inr_reset();
uint8_t inr_checkDuration(unsigned short min, unsigned short max);
uint8_t inr_receiveBit(uint8_t* rRecByte);
void inr_nextState(inrState_t eStatus);

static  volatile uint8_t inr_bitcnt=0;				// Empfangszähler der Bits
	
static volatile irNecReceiver_t* inr_receiver=0; 

void inr_init(volatile irNecReceiver_t* rReceiver, volatile uint8_t* vPort, uint8_t vPin_bp, uint8_t vInverted, void (*InterruptEnable)(), void (*InterruptDisable)(), void(*Timer250usOn)(), void (*Timer250usOff)())
{
	inr_port = (uint8_t*)(vPort);
	inr_pin_bm = (1<<vPin_bp);
	inr_invertedMode = vInverted;
	inr_receiver = rReceiver;
	
	inr_receiver->State = 0;
	inr_receiver->Command = 0;
	
	*DDR(inr_port) &= ~inr_pin_bm;			// Auf Eingang
	
	inr_intEn = InterruptEnable;
	inr_intDis = InterruptDisable;
	inr_timerOn = Timer250usOn;
	inr_timerOff = Timer250usOff;
	
	inr_reset();
}	

uint8_t inr_logicPinstate()
{	
	if(inr_invertedMode)
	{
		return(!(*PIN(inr_port) & inr_pin_bm));
	} 
	else
	{
		return((*PIN(inr_port) & inr_pin_bm));
	}
}
	 
void inr_reset()
{
	inr_timerOff();	
	
	if(inr_keystate & INR_KEYSTATE_ISPRESSED_bm)
	{
		inr_receiver->State |= (1<<INR_STATE_MSG_END_bp);
	}		
	
	inr_overflowCnt = 0;
	inr_recTimeout = MESSAGE_TIMEOUT_250US;
	inr_status = Idle;
	inr_keystate = 0;
	inr_intEn();
	
}		
	
void inr_pinChangedInterrupt()
{
	static  uint8_t addr=0;
	static  uint8_t _addr=0;

	static  uint8_t cmd=0;
	static  uint8_t _cmd=0;
	
	unsigned char notReset=0;
	
	inr_timerOn();	
	
	switch(inr_status)
	{
		case Idle:	
					inr_nextState(LeadingPulse);
					notReset=1;								
					break;						
					
		case LeadingPulse:
						if(inr_checkDuration(LEADING_PULSE_DURATION_250US_MIN, LEADING_PULSE_DURATION_250US_MAX))
						{
							inr_nextState(Space);
							notReset=1;
						}
						break;
					
		case Space: 
					if(inr_checkDuration(MESSAGE_SPACE_DURATION_250US_MIN, MESSAGE_SPACE_DURATION_250US_MAX))
					{						
						
							inr_nextState(Address);
							inr_bitcnt = 0;
							inr_bitstate = High;
							addr = 0;	// reset
							notReset=1;
											
					}		
					break;
		
		case Address:
					   if(inr_receiveBit((uint8_t*)&addr))
					   {
						   if(inr_bitcnt >= 8)
						   {
							   
								   inr_nextState(AddressInverse);
								   inr_bitcnt = 0;
								   _addr = 0x00;	// reset
								   inr_bitstate=High;
								   notReset=1;
							   								   
						   }
						   else
						   {
							    notReset=1;
						   }
						  					  
					   }	
					   break;
		case AddressInverse:
					   if(inr_receiveBit((uint8_t*)&_addr))
					   {
						   if(inr_bitcnt >= 8)
						   {
							   // fertig
							   if(((_addr^addr) == 0xff) && (addr==inr_receiver->Address))
							   {
								   inr_nextState(Command);
								   inr_bitcnt = 0;
								   inr_bitstate=High;
								   cmd = 0;
								   notReset=1;
							   }								   
						   } 
						   else
						   {
							    notReset=1;
						   }						  
					   }	
					   break; 
					   
			case Command:
					   if(inr_receiveBit((uint8_t*)&cmd))
					   {
						   if(inr_bitcnt >= 8)
						   {
							   // fertig
							  
								   inr_nextState(CommandInverse);
								   inr_bitcnt = 0;
								   inr_bitstate=High;
								   _cmd = 0;
								   notReset=1;
							   							   
						   }
						   else
						   {
							    notReset=1;
						   }					  
					   }	
					   break; 
					   
			
			case CommandInverse:
					   if(inr_receiveBit((uint8_t*)&_cmd))
					   {
						   if(inr_bitcnt >= 8)
						   {
							   // fertig
							   if(((cmd^_cmd) == 0xff))
							   {
								   inr_nextState(EndPulse);									   							   
								   notReset=1;
							   }								   
						   }
						   else
						   {
							    notReset=1;
						   }					  
					   }	
					   break; 
					   
			case EndPulse:						
						if(inr_checkDuration(PULSE_DURATION_250US_MIN, PULSE_DURATION_250US_MAX))
						{
							
								if(inr_keystate & INR_KEYSTATE_ISPRESSED_bm)
								{
									inr_receiver->State |= (1<<INR_STATE_MSG_REPEAT_bp);									
								}
								else
								{
									inr_receiver->State |= (1<<INR_STATE_MSG_START_bp);									
								}
									
															
								inr_receiver->Command = cmd;
								inr_keystate |= INR_KEYSTATE_ISPRESSED_bm;								
								inr_nextState(WaitForRepeat);
								inr_recTimeout=REPEAT_TIMEOUT_250US;
								notReset=1;				
						}
						
						break;	   
					   
			case WaitForRepeat:
					
						// Timout has not triggered yet..
						inr_nextState(RepeatLeadingPulse);
						notReset = 1;
						inr_recTimeout = REPEAT_CODE_TIMEOUT_250US;
					
						
					
					break;
					
			case RepeatLeadingPulse:
					if(inr_checkDuration(LEADING_PULSE_DURATION_250US_MIN, LEADING_PULSE_DURATION_250US_MAX))
					{						
							inr_nextState(RepeatSpace);
							notReset=1;
							
					}
					break;
					
			case RepeatSpace:
					if(inr_checkDuration(REPEAT_SPACE_DURATION_250US_MIN, REPEAT_SPACE_DURATION_250US_MAX))
					{
						inr_nextState(EndPulse);
						notReset=1;							
											
																
					}
					break;
				
		default:								// Sollte eigtl. nie auftreten
					inr_reset(); // reset all
					break;	
	}	
	
	inr_overflowCnt = 0;
	
	if(!notReset)
	{
		inr_reset();
	}
	
}

uint8_t inr_receiveBit(uint8_t* rRecByte)
{
	if(inr_bitstate==High)
	{
		if(inr_checkDuration(PULSE_DURATION_250US_MIN, PULSE_DURATION_250US_MAX))
		{
			if(!inr_logicPinstate())
			{
				inr_bitstate = Low;
				return(1); // valid
			}
		}
	}
	else // bitstate low
	{
		if(!inr_logicPinstate()) // hier müssts wieder high sein
		{
			return(0);
		}
		inr_bitstate = High;
		
		if(inr_checkDuration(SPACE_DURATION_HIGH_250US_MIN, SPACE_DURATION_HIGH_250US_MAX))
		{
			*rRecByte |= (1<<inr_bitcnt);
			inr_bitcnt++;
			return(1);			
		}
		else if(inr_checkDuration(SPACE_DURATION_LOW_250US_MIN,SPACE_DURATION_LOW_250US_MAX))
		{
			*rRecByte &= ~(1<<inr_bitcnt);
			inr_bitcnt++;
			return(1);
		}	
		// else faulty			
	}
		
	return(0);	
}


void inr_nextState(inrState_t eStatus)
{
	inr_status = eStatus;
}

uint8_t inr_checkDuration(unsigned short min, unsigned short max) // TODO: In MAKRO auslagern!!!
{
	return((inr_overflowCnt >= min) && (inr_overflowCnt <= max));
}


void inr_250usElapsed()
{
	inr_overflowCnt++;
		
	if(inr_overflowCnt >= inr_recTimeout) // Timeout zieht
	{		
		if(inr_keystate & INR_KEYSTATE_ISPRESSED_bm)
		{
			inr_receiver->State |= (1<<INR_STATE_MSG_END_bp);
		}
		
		inr_reset();
		return;
	}
	
	
	if(inr_status == LeadingPulse || inr_status==EndPulse)			// MUSS HIGH SEIN
	{
		if(!inr_logicPinstate())
		{
			inr_reset();
		} 
		 	
	}
	
	if(inr_status == Space || inr_status==RepeatSpace || inr_status==WaitForRepeat)
	{
		if(inr_logicPinstate()) 
		{
			inr_reset();
		}
	}	
		
	inr_timerOn(); // reenable
	inr_intEn(); // reenable
}
