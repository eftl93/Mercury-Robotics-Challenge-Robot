# TI INNOVATION ROBOT

- This project contains a description and some recovered source files from 2014 Sr. Design project for ELEE at UTPA
- Due to corruption of storage device and only partially had backed up the project on the cloud not all source code is included, in this repository I will either try to recreate the source files to complete the project again or explain the approach taken originally. 
- The Sr. Design name is: TI Innovation Challenge since it was originally intended to enter the TI competition that year. 
- At the end the project was only able to compete in the Mercury Robotic Challenge hosted in the University of Oklahoma
- It received the Judge's Award since most circuits were designed from scratch and not based on of the shelf solutions. The only development board used was the BeagleBone Black. Also the Buck converters circuits were bought. 

## Goals
The goal of the project was to design and build a mobile robot that could be controlled from at least 50 miles away it was to be dimensionaly constrained since it had to be able to fit inside a narrow tunnel. 

The robot was supposed to be controlled through an arbitrary maze, pick up a load in a "pickup location" and drop it off in a later portion of the maze in a "drop off" location. Points were awarded for avoiding obstacles and time of completion.

## Process
The motor controller was designed using the PIC18F46K22 from Microchip. The MCU control four precision motor controllers which are the LM629 from TI.  The PWM output signal from the LM629 and the PWM inputs from the LMD18200Ts (H-Bridge from TI) are interfaced using an ISO7220 (Digital Isolator from TI). 

The servos were controlled by signals generated by an MSP430G2553 from TI. Optocouplers were used to interface the MCU output and the servos' input. 

The PIC18F46K22 was controlled via SPI coming from a PIC18F4550 and the MSP430G2553 using the UART port. This controller also controlled a relay to turn on and off the main light in the robot. This board had a header pins to easily expand with additional boards. For example, add telemetry boards. 

The commands were sent through remote connection to the BeagleBone Dev board throuh ssh. A terminal was open and the serial port was controlled via keyboard characters. The characters would be sent through UART to the PIC18F4550 board. This board would then decide of the command belonged to the servos, motors, or lights. It would then "translate" it and send it to the right controller. 

In order to control the robot remotely, a network router was hacked and a new OS was installed which added a network bridge capability. In summary, the router (now bridge) would connect to a network through Wifi and then give internet (or local network) connection to the BeagleBone through ethernet cable. 
This was chosen in order to take advante of the wifi signal "power" performance by this device in comparison to the XBee (which was a popular wifi module in the time this project was being built).

In order to make the design robust, it was taken into account that having many brushed DC motor for the main movement and servos was going to  introduce noise into the microcontrollers and microprocessor. Also, the pcbs were made on pre-punch board and a cnc milling machine, so no proper multilayer board with ground plane was used. Therefore, it was decided that using two separate powersupplies would make the design more robust. One battery pack would power the motors and a smaller one would power the logic controllers. Both were Isolated by using both optocouplers and highspeed digital isolators. 

Since Lithium Polymer batteries were used, competition required that the robot included an undercharge protection circuit. A simple comparator circuit powered with a 9V battery was used with a relay in order to disconnect the lithium batteries from the circuit in case they would reach a threshold voltage of 3.4V. This was not part of the Sr. Desing but I'm planning on designing an elegant (and modern) power management solution after all other files have been updated and tested. 

![image](https://user-images.githubusercontent.com/86902176/165432875-5e5c86cd-96c4-4a19-b4c6-c7a826541480.png)
