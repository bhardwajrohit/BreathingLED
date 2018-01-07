# Hardware Software Interfacing

# Breathing LED

● STM32F303 - ARM Cortex-M4 MCU ● LED with series resistor 

● Digital Multimeter ● Oscilloscope and probes ● Linux Environment 

● The code is for  STM32F303 board, it configure, and write to the Timers in PWM mode. Moreover,Commands present, 
  and work to implement a 'Breathing' LED that autonomously cycles smoothly between 0 and 100%, and back to 0% again
  
 This project is a simple monitor program for the STM32F303VCT6 on the STM32F3 Discovery board.  This processor has 256K of 
 flash, and 48K of RAM. The Discovery board has 10 LEDs, a 3 axis Gyroscope, 3 axis accelerometer, and a 3 axis magnetic sensor.

 The Simple monitor program provides a command line parser, with facilities for the user to add new commands on a module by module
 basis.  The parser is capable of recognizing one word commands, and calling a function that implements that command.  The function 
 may then retrieve additional arguments from the command line, and act accordingly.  Online brief help is provided automatically, 
 extended help is available optionally on a command by command basis.

 The structure of the simple monitor program is one of a foreground/background system.  The main loop of the program initializes all 
 the peripherals, enables interrupts, sets up I/O conditions, and then calls a series of 'Tasks' in sequence.  The user can extend 
 this by adding their own background tasks to the main loop. Caution should be exercised by the user to ensure that every task returns 
 back to the main loop if there is no work to do. Any task that blocks waiting for an I/O event to occur will stall the whole system. 
 (For example, calling the delay() function inside your task is frowned upon, use a timer or virtual timer instead.)
