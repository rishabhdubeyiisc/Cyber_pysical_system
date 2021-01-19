In this exercise, we will create a commmand-line shell that can allow us to
control resources on the board from a host computer over a serial port.

Files:
startup_rvmdk.S     startup file; you may use CMSIS startup file as well
main.c              initializations
shell.c/.h          command processing
examples            directory containing example source code
    /blink.c        blink with tivaware
    /timers.c       blink with tiva timers
    /uart_echo.c    sample UART echo code
    /adc_test.c     ADC test code

Step 1. Understand the structure of the shell:
(Think through. Write if that helps. No points for writing.)
- What does UART interrupt handler do?
- Why are there two buffers, rxbuf and CmdBuf for storing the command?/
- How does main code know that a command is received?
- How is a function called that corresponding to the command entered?

Step 2. Add a command to read the temperature value. The usage is as follows:
    temp c  : print temperature in C
    temp f  : print temperature in F

Step 3. [Optional] Change the existing led command for brightness control
    instead of on/off control. Change the usage to: 
    led [r|g|b] [0-3]
    where [0-3] are brightness levels, from 0(off) to 3(full bright)
    
