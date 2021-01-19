#ifndef _SHELL_H
#define _SHELL_H

#include <stdint.h>
#include "driverlib/pwm.h"
extern void CmdInit(void);
extern void CmdPrompt(void);
extern uint32_t CmdReceived(void);
extern void CmdProcess(void);
extern void UARTcharput(char c);
extern void UARTStringPut(const char *string);
extern void UARTNumPut(const char *string);
void UARTIntHandler(void);
extern void set_pwm_duty(uint32_t , uint32_t );

//ADC
void ftoa(float f,char *buf);
//pwm
uint32_t num_char_to_int(char );

#endif	/* _SHELL_H */
