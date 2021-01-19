#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "shell.h"
#define FAN_BASE_SPEED 100
// prototypes of all the command functions
static void doHelp  ( int argc, char *argv[]);
static void doLed   ( int argc, char *argv[]);
static void readTemp( int argc, char *argv[]);
static void fanCtrl	( int argc, char *argv[]);
//static char DEBUG_CHAR = 'a';
typedef void (*pfn_t)(int argc, char *argv[]);

typedef struct
{
	char *cmd;
	char *usage;
	pfn_t pfn;
}cmdtable_t;

static cmdtable_t CmdTable[] =
{
	{"help"			, "help: this command      \n", doHelp	},
	{"led"			, "led [r|b] on-off        \n", doLed		},
	{"readTemp"	, "readTemp c/f            \n", readTemp},
	{"fanCtrl"	, "fanCtrl [duty {0 - 10}] \n", fanCtrl	}
};
static const uint32_t NumCmds = ((sizeof(CmdTable)/sizeof(CmdTable[0])));


/*
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,200);
		MAP_PWMOutputState(PWM1_BASE, ( PWM_OUT_5_BIT ), true);
		return;

		MAP_GPIOPinConfigure(GPIO_PF1_M1PWM5);
		MAP_GPIOPinConfigure(GPIO_PF2_M1PWM6);
		MAP_GPIOPinConfigure(GPIO_PF3_M1PWM7);
*/
typedef struct
{
    char num_string;
    uint32_t number;
} char_to_int_table_t;

static char_to_int_table_t MapTable[] =
{
        {'0' - '0', 0},
		{'1' - '0', 1},
		{'2' - '0', 2},
		{'3' - '0', 3},
		{'4' - '0', 4},
		{'5' - '0', 5},
		{'6' - '0', 5},
		{'7' - '0', 6},
		{'8' - '0', 8},
		{'9' - '0', 9}
};

static const uint32_t MapTable_len = ((sizeof(MapTable)/sizeof(MapTable[0])));

uint32_t num_char_to_int(char x){
	uint32_t i = 0;
	for( i = 0 ; i < MapTable_len ; i++){
		if ( MapTable[i].num_string == x){
			//DEBUG_CHAR = MapTable[i].num_string;
			return MapTable[i].number * 40;
		}
	}
	return 200;
}
void UARTStringPut(const char *string);
void UARTNumPut(const char *string);

#define CMDLEN 80
static char CmdBuf[CMDLEN];
static uint32_t CmdFlag;

static void CmdExecute(int argc, char *argv[]);

#define CMD_GREETINGS "\n\n\nhello, world!\n(Type 'help' for command list.)\n"
void CmdInit()
{
    CmdFlag = 0;
    //TODO
		//Rishabh
		//UARTStringPut(CMD_GREETINGS);
    CmdPrompt();
}

void UARTStringPut(const char *str)
{
    while(*str != 0)
    {
        ROM_UARTCharPut(UART0_BASE, *str);
        str++;
    }
}

void UARTcharput(char c){
		ROM_UARTCharPut(UART0_BASE, c);
}

void UARTNumPut(const char *str)     // print an integer - may come handy in debugging
{
    while(*str != 0)
    {
        ROM_UARTCharPut(UART0_BASE, *str + '0');
        str++;
    }
}

#define CMD_PROMPT "> "
void CmdPrompt(void)
{
    UARTStringPut(CMD_PROMPT);
}

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        static char rxbuf[CMDLEN+1];      // one extra for null termination
        static uint32_t rxlen;
        char ch;
        
        ch = ROM_UARTCharGetNonBlocking(UART0_BASE);
				//Rishabh
        //ROM_UARTCharPutNonBlocking(UART0_BASE, ch);     // echo
        
        if (ch == 0x0D)     // 0x0D = carriage return - command received
        {
            rxbuf[rxlen] = 0;        // null-terminate the command string
            strcpy(CmdBuf, rxbuf);    // make a copy for processing
            CmdFlag = 1;

            rxlen = 0;         // start receiving next command
        }
        else
        {
            rxbuf[rxlen] = ch;
            rxlen++;           // next character
        }
    }
}

uint32_t CmdReceived(void)
{
    uint32_t rc;

    rc = 0;
    if (CmdFlag)    // has the handler set the flag?
    {
        // if so, clear the flag and return 1
        CmdFlag = 0;
        rc = 1;
    }
    
    return rc;
}

#define ARGMAX 10
void CmdProcess(void)
{
    int argc;
    char *argv[ARGMAX];
    
    char *p = CmdBuf;
    
    argc = 0;
    while (1)
    {
        // skip initial blanks
        while ((*p != 0) && (*p == ' '))
            p++;
    
        if (*p == 0)    // no more args
            break;

        argv[argc] = p;

        while ((*p != 0) && (*p != ' '))
            p++;

        if (*p == 0)    // end of string
            break;

        *p = 0;     // null-terminate the current argument
        p++;
        
        argc++;     // next argument
    }

    CmdExecute(argc, argv);
}

static void CmdExecute(int argc, char *argv[])
{
    uint32_t i;

    for (i = 0; i < NumCmds; i++)
    {
        if (strcmp(argv[0], CmdTable[i].cmd) == 0)
            break;
    }

    if (i == NumCmds)
    {
        UARTStringPut(argv[0]);
        UARTStringPut(": ");
        UARTStringPut("command not found.\n");
    }
    else
    {
        CmdTable[i].pfn(argc, argv);
    }
}

static void doHelp(int argc, char *argv[])
{
    uint32_t i;

    for (i = 0; i < NumCmds; i++)
    {
        UARTStringPut(CmdTable[i].usage);
        UARTStringPut("\n");
    }
}

void ftoa(float f,char *buf)
{
    int pos=0,ix,dp,num;
    if (f<0)
    {
        buf[pos++]='-';
        f = -f;
    }
    dp=0;
    while (f>=10.0)
    {
        f=f/10.0;
        dp++;
    }
    for (ix=1;ix<8;ix++)
    {
            num = (int)f;
            f=f-num;
            if (num>9)
                buf[pos++]='#';
            else
                buf[pos++]='0'+num;
            if (dp==0) buf[pos++]='.';
            f=f*10.0;
            dp--;
    }
		for (ix=0;ix<7;ix++)
			//character put
			UARTcharput(buf[ix]);
		//string put
		UARTStringPut("\n");
}
void doLed(int argc, char *argv[])
	{
  //led r on/off
    char color = argv[1][0];
    uint32_t pin;
    
    if (color == 'r'){
        pin = PWM_OUT_5_BIT;
		}
    else if (color == 'b'){
        pin = PWM_OUT_6_BIT;
			}
    //else if (color == 'g'){
    //    pin = PWM_OUT_7_BIT;
		//	}
    else{
        pin = PWM_OUT_5_BIT | PWM_OUT_6_BIT ; // | PWM_OUT_7_BIT;     // all pins
			}
		//UARTNumPut("99");
		// argv[2][0] is for duty
		if ( strcmp(argv[2], "on") == 0){
			//UARTNumPut("98");
			// duty is given
			//uint32_t duty = num_char_to_int(argv[2][0]);
			//UARTNumPut("97");
			MAP_PWMPulseWidthSet(PWM1_BASE, pin , 200);
			MAP_PWMOutputState(PWM1_BASE, ( pin ), true);
			//UARTNumPut("96");
		}
		else {
			//UARTNumPut("300");
			MAP_PWMOutputState(PWM1_BASE, ( pin ), false);
			//UARTNumPut("299");
			//duty not given thus turning it off
		}		
    UARTStringPut("led_done");
    return;
}

static void fanCtrl( int argc, char *argv[]){
		
    uint32_t fan_pin = PWM_OUT_7_BIT;
    if (argv[1][0]){
			uint32_t duty = num_char_to_int(argv[1][0]);
			/*
			char debug_str[2] ;
			debug_str[0] = DEBUG_CHAR ;
			debug_str[1] = argv[1][0] - 0xff - 0x1;
			if (debug_str[1] == '1'){
				ROM_UARTCharPut(UART0_BASE, *(debug_str+1) );
			}
			*/
			//UARTNumPut("a");
			if (duty == 0){
				//UARTNumPut("b");
				MAP_PWMOutputState(PWM1_BASE, ( fan_pin ), false);
				UARTStringPut("fan_ctrl_FAN_OFF");
				return ;
			}
			//UARTNumPut("c");
      MAP_PWMPulseWidthSet(PWM1_BASE, fan_pin , duty);
			MAP_PWMOutputState(PWM1_BASE, ( fan_pin ), true);
			//UARTNumPut("d");
			UARTStringPut("fan_ctrl_duty_SET");
		}
    else {
      MAP_PWMPulseWidthSet(PWM1_BASE, fan_pin , FAN_BASE_SPEED);
			MAP_PWMOutputState(PWM1_BASE, ( fan_pin ), true);
			UARTStringPut("fan_ctrl_BASE_set");
		}
    return;

}
static void readTemp(int argc, char *argv[]){
			
    char select = 'c';
    if (argc > 0){
            select = argv[1][0];
    }
    
    char tempbuf[8]={"0","0","0","0","0","0","0","0"};
    uint32_t ADCValues[1];

    float TempValueC ;
    float TempValueF ;
        //
    // Trigger the ADC conversion.
    //
    MAP_ADCProcessorTrigger(ADC0_BASE, 3);
    //
    // Wait for conversion to be completed.
    //
    while(!MAP_ADCIntStatus(ADC0_BASE, 3, false)){ }
    //
    // Clear the ADC interrupt flag.
    //
    MAP_ADCIntClear(ADC0_BASE, 3);
    //
    // Read ADC Value.
    //
    MAP_ADCSequenceDataGet(ADC0_BASE, 3, ADCValues);
    // Use non-calibrated conversion provided in the data sheet. I use floats in intermediate
    // math but you could use intergers with multiplied by powers of 10 and divide on the end
    // Make sure you divide last to avoid dropout.
    //
    TempValueC = (147.5 - ((75.0*3.3 *(float)ADCValues[0])) / 4096.0);
    if (select == 'f'){
            TempValueF = ((TempValueC * 9) + 160) / 5;
            ftoa(TempValueF,tempbuf);
    }
    else {
            ftoa(TempValueC,tempbuf);
    }
    
    MAP_SysCtlDelay(2*MAP_SysCtlClockGet() / 3);
}
