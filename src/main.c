#ifndef PART_TM4C123GH6PM1
#define PART_TM4C123GH6PM1
#endif
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
//pwm inc
#include "driverlib/pwm.h"
//pwm inc done
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "shell.h"
#include "string.h"

//#include "my_func.h"

#ifndef MY_PWM_PERIOD
#define MY_PWM_PERIOD 400
#endif
#ifndef MY_PWM_INIT_T_ON
#define MY_PWM_INIT_T_ON 0
#endif

void init_adc(void);
void init_pwm(void);
void init_uart(void);
// void set_pwm_duty(uint32_t value , uint32_t delay );
//void init_adc(void);
//void set_pwm_duty(int value = 200 , int delay = 10000 );
// TODO get GPIO key and its address from base 
int main(void)
{   
    init_pwm();
		init_uart();
		//ADC INIT 
    init_adc();
    // command loop
		UARTNumPut("1234");
    CmdInit();
    while(1)
    {
        if (CmdReceived())
        {
            CmdProcess();
            CmdPrompt();
        }
    }
}

// void set_pwm_duty(uint32_t value , uint32_t delay ){
// 	// value should be 0 to 400
//   if (value <= 0 || value > MY_PWM_PERIOD){
//     return ; 
//   }
//   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4,value);
//   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,value);
//   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,value);
//   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,value);
//   SysCtlDelay(delay);
// }

void init_adc(void){
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);//enabling ADC0
  MAP_SysCtlDelay(3);
  MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
  MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
  MAP_ADCSequenceEnable(ADC0_BASE, 3);
  MAP_ADCIntClear(ADC0_BASE, 3);
}

void init_pwm(void){
  //pwm ENABLING functionality
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	  //control register and log
    //HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    //HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    MAP_GPIOPinConfigure(GPIO_PF0_M1PWM4);
    MAP_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    MAP_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    MAP_GPIOPinConfigure(GPIO_PF3_M1PWM7);
    MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    //ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   // RBG
    //ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);     // SW1 and SW2

    // config PWM
    MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    //400 is T
    MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, MY_PWM_PERIOD);
    MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, MY_PWM_PERIOD);

    MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, MY_PWM_INIT_T_ON);
    MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, MY_PWM_INIT_T_ON);
    MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, MY_PWM_INIT_T_ON);
    MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, MY_PWM_INIT_T_ON);
    
    MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    
    MAP_PWMOutputState(PWM1_BASE, (PWM_OUT_4_BIT | PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT  ), false);
}

void init_uart(void){
	    // Configure UART0 on Port A
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART parameters and enable interrupts
    ROM_UARTConfigSetExpClk(UART0_BASE, MAP_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
