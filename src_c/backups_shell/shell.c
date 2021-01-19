#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "shell.h"

// prototypes of all the command functions
static void doHelp(int argc, char *argv[]);
static void doLed(int argc, char *argv[]);
static void readTemp(int argc, char *argv[]);

typedef void (*pfn_t)(int argc, char *argv[]);

typedef struct
{
    char *cmd;
    char *usage;
    pfn_t pfn;
} cmdtable_t;

static cmdtable_t CmdTable[] =
{
    {"help", "help: this command \n", doHelp},
    {"led", "led [r|g|b] [on|off] \n", doLed},
	{"readTemp", "readTemp c/f \n", readTemp}
};
static const uint32_t NumCmds = ((sizeof(CmdTable)/sizeof(CmdTable[0])));

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
    int i;

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
    char color = argv[1][0];
    uint32_t pin;
    
    if (color == 'r')
        pin = GPIO_PIN_1;
    else if (color == 'b')
        pin = GPIO_PIN_2;
    else if (color == 'g')
        pin = GPIO_PIN_3;
    else
        pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;     // all pins
    
    if (strcmp(argv[2], "on") == 0)
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, pin, pin);
    else    // off by default
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, pin, 0);
    //UARTStringPut("led_done")
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
