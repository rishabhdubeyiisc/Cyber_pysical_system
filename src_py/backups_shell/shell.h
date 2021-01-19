#ifndef _SHELL_H
#define _SHELL_H

#include <stdint.h>

extern void CmdInit(void);
extern void CmdPrompt(void);
extern uint32_t CmdReceived(void);
extern void CmdProcess(void);
extern void UARTcharput(char c);
extern void UARTStringPut(const char *string);
extern void UARTNumPut(const char *string);
void UARTIntHandler(void);

//ADC
void ftoa(float f,char *buf);


#endif	/* _SHELL_H */
