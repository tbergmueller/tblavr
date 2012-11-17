/*
 * DevComSlave.h
 *
 * Created: 12.12.2011 22:42:47
 *  Author: Thomas Bergmueller
 *  Mail:	info@thomasbergmueller.com
 *  
 *
 * Description: DevCom is an bipolar half duplex inter-device communication module with Master-Slave-Architecture using M3S-Protocol via interrupt driven USARTs.
 *				This DevCom-Module covers error-checking and acknowledge/response of frames if necessary, probably rudimentary comparable to a mix of TCP/IP and Ethernet over all layers.
 *              Basic Protocols are Command and Data (Broadcast, Multicast and Unicast each), in larger Implementations Filetransfer.
 *				For commands a CommandHandler-Callback-Method is available, New Data (or other updated Parameters) are announced by setting a 
 *				Flag in devComSlave_t.NewReceived.
 *				
 *				devComSlave_t.Data is a Pointer to valid Data that can be either set by Application (via dcs_setData) or is set via Bus communication by Master. DataUpperBound thereby 
 *              determines the length of Data. Set DataUpperBound 0 if Data of variable Length should be accepted. Additional algorithm has to be implemented to determine the length of Data then.
 *
 * Details:
 *				- A Slave is identified through its bus-unique address or its multicast-address. 
 *				- If Unicast-Transfer is issued, Slave always acknowledges it with a 3 Byte Frame (as long as AckRequ-Flag in Controlbyte of received Frame was set)
 */ 


#ifndef DEVCOMSLAVE_H_
	#define DEVCOMSLAVE_H_
 
	#include <inttypes.h> 
	
	
	
	typedef uint8_t (*DevComSlaveCommandHandler_t) (const void* rSlave, const unsigned char* pCmd, unsigned char pControlbyte, unsigned char pUpperBound, unsigned char pAllowsResponse);
	typedef void (*DevComSlaveFrameTransmitter_t)(unsigned char pCtrlByte, unsigned char* pData, unsigned char pDataLength); // Sendet ein Frame vom Slave an den Master (dh. keine Ack-Behandlung)
	
	typedef void (*DevComSlaveRxCCallback_t)(unsigned char pReceivedByte);
	typedef void (*DevComSlaveTxCCallback_t)();
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ DevComSlave-Object ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
	typedef struct devComSlaveStruct
	{
		unsigned char				Address;				// Slave Address
		volatile unsigned char		MCAddress;				// Multicast Address
		unsigned short				ID;						// Device-ID, configurable by User, could be used to identify device types etc.
				
		volatile unsigned char*		Data;					// Pointer to valid Data		
		volatile unsigned char		DataUpperBound;			// Length-1 (highest index) of acceptable Data Packages. Set 0 if Data of any length up to 256 Bytes should be accepted
		volatile unsigned char		NewReceived;			// 0 ... no Data available, != 0: New Received (see "New Received Bitmapping" below)
								
		// RW-Output (if enabled)
		volatile uint8_t*			RWPort;					// Port, where RW-Wire is connected (e.g. for Buscoupler,...)
		uint8_t						RW_bp;					// Bit Position, where RW-Wire is connected (Pn0...Pn7), e.g. PC3
		unsigned char				RWPolarity;				// 0... when writing, RW-Wire is logic 0, 1 when reading    
															// 1... when writing, RW-Wire is logic 1, 0 when reading
		
		void						(*Stop)(DevComSlaveRxCCallback_t rRxcFunction, DevComSlaveTxCCallback_t rTxcFunction);
		
		DevComSlaveCommandHandler_t CommandHandler;			
		// A callback-function, that handles all Commands that are NOT handled internally. Feedback Execution State (either DCS_CMDHANDLER_SUCCESS or DCS_CMDHANDLER_ERROR).
		// parameters:
		//		pCmd			Pointer to Command (pCmd[0]) and its parameters (pCmd[1]...pCmd[pUpperBound])
		//		pLastCtrlByte	Controlbyte of the Frame the command was received in
		//		pUpperBound		UpperBound of pCmd
		//		pAllowsResponse	Indicates whether the command allows responding (Cmd received via Unicast) or not (Cmd received via Broad-/Multicast)
		//
		// return:
		//		DCS_CMDHANDLER_SUCCESS		if Execution of Command was successful
		//		DCS_CMDHANDLER_ERROR		if Execution of Command was unsuccessful or command is unknown
		
		DevComSlaveFrameTransmitter_t SendFrame;
	} 
	DevComSlave_t;
	
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ New Received Bitmapping~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Bit Positions in devCom_t.NewReceived
	
	#define DCS_REC_DATA_gm						0x07		// group mask for Data
	
	#define DCS_REC_DATA_bp						0			// Data Received by Unicast
	#define DCS_REC_DATA_MC_bp					1			// Data Received by Multicast
	#define DCS_REC_DATA_BC_bp					2			// Data Received by Broadcast
	
	
	#define DCS_CMDHANDLER_SUCCESS				0
	#define DCS_CMDHANDLER_ERROR				1
	// Reserved Bit 4-7, used for files maybe?
	
	
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~ Init-Function-Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// Initializes DevCom according to passed parameters (see code Example below). After executing initialization, DevComSlave-Module is active and waits for Reset from Master
	// Parameters
	//	- rSlave:				Reference to DevComSlave_t-Object
	//  - rInterfaceDefinition: Pass one of the Implementation codes listet in "Implementations" without () at the end!! (Make sure Processor and USART match your expectations)
	//  - vFcpu:				Clock rate of CPU in Hz. If defined in Project Settings, pass F_CPU, otherwise e.g. 16000000 for 16MHz Crystal
	//  - vBaudrate:			Baudrate of the communication in bps, e.g. 9600	
	
	
		
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ INITIALIZING (=Start Listening on Bus) ~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*
		This library provides a lot of precompiled objectfiles in a library. You can choose to use one of those by calling the appropriate 
		init-function that fits your device, USART and RW-Output.
	
	
	*/
	
	// ######### ENCODING OF INIT-FUNCTIONS #############
	
	// dcs_PROCESSOR_*USARTn*_*RWOUTPUT*_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);			whereby USARTn and RWOUTPUT optional
	
	// ~~~~~~~~ Anatomy of Initfunction ~~~~~~~~~~
	//	dcs_		... Prefix
	//	PROCESSOR	... fill in processor like it is passed in -mmcu to compiler
	//	USARTn		... optional parameter, in devices with more than one USART define USART here, e.g. usart2
	//	RWOUTPUT	... optional parameter, if you'd like to have a Processorpin signaling DataDirection (Send/Receive), add this and define in devComSlave_t.RW* - Fields
	// _init(..)	... Postfix with Parameters
	
	// ~~~~~~~ Parameters ~~~~~~~~~~~~~~~~~~~~~~~~
	// rSlave		...	Pointer to devComSlave_t-Object
	// vFcpu		... CPU-Frequency, recommended to pass F_CPU (as it should be definied for compilation anyway in most projects...)
	// vBaudrate	... Baudrate USART should be operating on in bits per second.
	
		
	
	// Examples:
	//			dcs_atmega644p_usart0_init(...);		// Starts listening @ usart0 on atmega644p without RW-Output
	//			dcs_atmega88_rw_init(...);				// Starts listening @ usart on atmega88 with RW-Output	
	
	
	DevComSlave_t* dcs_create(unsigned char SlaveAddress);		// Creates new instance of DevComSlave-Module with passed address
	uint8_t dcs_setData(uint8_t* rData, uint8_t vDataLength);	// Set DevComSlave_t.Data , may be used to prepare Data for Readout
		
	
	#define UART0	0
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define UART4	4
	
	
	#define UARTA0	0
	#define UARTA1	1
	#define UARTB0	2
	#define UARTB1	3
	#define UARTC0	4
	#define UARTC1	5
	#define UARTD0	6
	#define UARTD1	7
	
	
	
	#define DCS_CMDHANDLER_IDX_CMDBYTE			0
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_1		1
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_2		2
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_3		3
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_4		4
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_5		5
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_6		6
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_7		7
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_8		8
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_9		9
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_10		10
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_11		11
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_12		12
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_13		13
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_14		14
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_15		15
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_16		16
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_17		17
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_18		18
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_19		19
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_20		20
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_21		21
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_22		22
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_23		23
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_24		24
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_25		25
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_26		26
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_27		27
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_28		28
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_29		29
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_30		30
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_31		31
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_32		32
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_33		33
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_34		34
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_35		35
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_36		36
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_37		37
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_38		38
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_39		39
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_40		40
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_41		41
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_42		42
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_43		43
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_44		44
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_45		45
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_46		46
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_47		47
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_48		48
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_49		49
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_50		50
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_51		51
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_52		52
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_53		53
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_54		54
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_55		55
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_56		56
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_57		57
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_58		58
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_59		59
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_60		60
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_61		61
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_62		62
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_63		63
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_64		64
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_65		65
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_66		66
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_67		67
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_68		68
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_69		69
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_70		70
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_71		71
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_72		72
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_73		73
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_74		74
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_75		75
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_76		76
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_77		77
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_78		78
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_79		79
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_80		80
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_81		81
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_82		82
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_83		83
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_84		84
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_85		85
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_86		86
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_87		87
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_88		88
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_89		89
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_90		90
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_91		91
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_92		92
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_93		93
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_94		94
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_95		95
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_96		96
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_97		97
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_98		98
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_99		99
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_100	100
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_101	101
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_102	102
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_103	103
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_104	104
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_105	105
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_106	106
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_107	107
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_108	108
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_109	109
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_110	110
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_111	111
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_112	112
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_113	113
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_114	114
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_115	115
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_116	116
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_117	117
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_118	118
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_119	119
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_120	120
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_121	121
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_122	122
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_123	123
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_124	124
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_125	125
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_126	126
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_127	127
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_128	128
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_129	129
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_130	130
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_131	131
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_132	132
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_133	133
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_134	134
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_135	135
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_136	136
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_137	137
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_138	138
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_139	139
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_140	140
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_141	141
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_142	142
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_143	143
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_144	144
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_145	145
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_146	146
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_147	147
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_148	148
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_149	149
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_150	150
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_151	151
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_152	152
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_153	153
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_154	154
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_155	155
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_156	156
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_157	157
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_158	158
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_159	159
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_160	160
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_161	161
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_162	162
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_163	163
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_164	164
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_165	165
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_166	166
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_167	167
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_168	168
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_169	169
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_170	170
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_171	171
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_172	172
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_173	173
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_174	174
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_175	175
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_176	176
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_177	177
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_178	178
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_179	179
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_180	180
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_181	181
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_182	182
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_183	183
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_184	184
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_185	185
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_186	186
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_187	187
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_188	188
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_189	189
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_190	190
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_191	191
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_192	192
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_193	193
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_194	194
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_195	195
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_196	196
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_197	197
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_198	198
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_199	199
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_200	200
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_201	201
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_202	202
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_203	203
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_204	204
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_205	205
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_206	206
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_207	207
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_208	208
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_209	209
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_210	210
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_211	211
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_212	212
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_213	213
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_214	214
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_215	215
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_216	216
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_217	217
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_218	218
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_219	219
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_220	220
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_221	221
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_222	222
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_223	223
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_224	224
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_225	225
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_226	226
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_227	227
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_228	228
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_229	229
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_230	230
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_231	231
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_232	232
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_233	233
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_234	234
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_235	235
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_236	236
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_237	237
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_238	238
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_239	239
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_240	240
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_241	241
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_242	242
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_243	243
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_244	244
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_245	245
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_246	246
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_247	247
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_248	248
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_249	249
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_250	250
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_251	251
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_252	252
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_253	253
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_254	254
	#define DCS_CMDHANDLER_IDX_PARAMBYTE_255	255

	
	
	
	
		
	#if defined (__AVR_ATmega88__)
		#define prototype(uN) dcs_atmega88_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)		
	#elif defined(__AVR_ATmega32__)
		#define prototype(uN) dcs_atmega32_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined(__AVR_ATmega8__)
		#define prototype(uN) dcs_atmega8_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)	
	#elif defined(__AVR_ATmega644__)
		#define prototype(uN) dcs_atmega644_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644A__) 
		#define prototype(uN) dcs_atmega644a_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644P__)
		#define prototype(uN) dcs_atmega644p_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644PA__)
		#define prototype(uN) dcs_atmega644pa_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega640__)
		#define prototype(uN) dcs_atmega640_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega1280__)
		#define prototype(uN) dcs_atmega1280_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega2560__)
		#define prototype(uN) dcs_atmega2560_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATxmega128A1__)
		#define prototype(uN) dcs_atxmega128a1_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#else	
		#ifndef COMPILELIB	
			#error "Sorry, TBL.DevComSlave-Module is not available for this processor..."
		#endif			
	#endif
	
	
	
	//{dcs_prototype_list}
	void DCS_BASIC_INIT(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);
		
	
#endif /* DEVCOMSLAVE_H_ */