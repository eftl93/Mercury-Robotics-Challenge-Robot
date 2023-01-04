# MERCURY ROBOTICS ROBOT

## Overall Project
This repository contains code, schematics and other material for a mobile robot. The objective of the robot is to accomplish the following tasks:
1. It needs to be controlled remotely
2. It should be capable of going through an obstacle course
3. It should be able to perform maneuvers in the minimum amount of time
4. It must be able to pick-up and drop-off a payload
5. It must be designed in a modular manner for easy future upgrades


I'm also posting the Mecury Robotics project in hackster.io: https://www.hackster.io/ederfernandotorres3/exploration-robot-with-beaglebone-black-msp430-and-pic-mcu-deeb33

I'm also posting details on:
- How to setup the BeagleBone Black with UART Device Tree Overlay: https://www.hackster.io/ederfernandotorres3/setup-beaglebone-black-with-device-tree-overlays-9e0ded
- How to compile and install drivers for the Wi-Fi card: https://www.hackster.io/ederfernandotorres3/8821au-wifi-card-and-bbb-22a7ac
- How to compile and install GStreamer and enable uvh264 plug-in: https://www.hackster.io/ederfernandotorres3/compilation-of-gstreamer-to-stream-h264-with-linux-3187eb


![IMG_7032-small](https://user-images.githubusercontent.com/86902176/210489498-19b74113-9f76-4846-aa71-b09597b8fc18.jpg)



## Software Tools and Libraries
To develop this robot, the following software and hardware were used:
#### 1. MSP430 and PIC MCUs on my own PCBs
  - Code Composer Studio and MSP-FET were used to program and debug the M430G2550
  - MPLABX and Pickit 3 were used to program and debug the PIC18F46K22
#### 2. AM335X MPU on a BeagleBone Black
  - Open-Source Linux
  - Open-Source Device Tree Overlays
  - Open-Source GStreamer (compiled from source code)
  - Open-Source 8821AU Wi-Fi Chip Driver (compiled from source)
#### 3. RP2040 MCU on the Raspberry Pi Pico
  - Open-Source CircuitPython (Variation of Python to run in microcontrollers)
  - Open-Source HID Device Library
#### 4. Altium's CircuitMaker
  - Used to design the schematics and PCBs for the robot

## Level 0 Design
In order to accomplish the listed goals, the robot is implemented with the following features:
1. Use Wi-Fi for the remote communication between the robot and the host computer
2. Send video from a USB camera to the host computer
3. Use servos to pan and tilt the camera
4. Use servos to raise and lower a robotic arm
5. Use servos to open and close a robotic claw
6. Use motor with quadrature encoders to control the speed and number of rotations per wheel
7. Use a high-power LED in order to illuminate dark tunnels
8. The robot will receive movement commands from a host computer, the host computer will use an intuitive controller for ease of use

![image](https://user-images.githubusercontent.com/86902176/210486303-3b0617af-7adf-4650-8a77-cf55e66d81d8.png)

Level 0 Block Diagram

## Theory of Operation
In order to complete the project in a modular manner, the mobile robot has been divided in two parts, the "Front-End" and the "Back-End". 

The front-end of the project is where the end-user controls the robot (on a host computer). The end-user will be able to see what the robot sees with the camera. And the end-user will be sending commands to the robot to move, pan/tilt the camera, raise/lower the robotic arm, open/close the claw, and turn on/off the headlights. 

The back-end of the project is the actual mobile robot and is where all the "essential" modules are located. The back-end has been divided even further to accomplish the modular philosophy, the modules are: 

The "BeagleBone Black", which is in charge of connecting to the network and receive commands through SSH from the front-end. It also fetches video feed from a USB camera and sends the video stream to the front end-through the network.

The "Signal Distribution Board" (SDB), which is in charge of receiving commands in form of data packages from the BeagleBone through UART, then it decides who should receive the commands and forwards it to the correct module.

The "Motor Controller Boards", which are composed of one digital signals PCB and an analog signals PCB. The digital board is in charge of receiving speed and direction commands forwarded by the SDB through SPI communication. The digital board will then create the PWM and direction signals and forward them to the analog PCB. The analog signal PCB is in charge of sending power to the motors and receive quadrature signals from the motors, then forward these quadrature signals to the digital signal board for processing. 

And the "Servo Controller Board", which is in charge of receiving commands from the SDB through UART. This module will generate the appropriate waveforms to control the servos. 



## Level 1 Design
### 1. Block layout showing data flow direction of both "front-end" and "back-end" of the project.
![image](https://user-images.githubusercontent.com/86902176/210492465-e26cfaff-28dc-49c3-8fb0-0217a030049c.png)


## Level 2 Design
### 1. Block layout shows the dataflow of the front-end of the project
![image](https://user-images.githubusercontent.com/86902176/210492917-448a8e00-8317-46eb-9bfd-d9d7d7a50a31.png)

The front-end is comprised of a Windows or Linux machine with GStreamer installed. The machine must be connected to the same network as the robot (back-end). The front-end will connect to the robot using the SSH protocol. To send direction and magnitude of the speed of the motors and the position for the servos, a game console controller is used (it's a hacked controller with an mcu in order to emulate an HID keyboard device).

![image](https://user-images.githubusercontent.com/86902176/210493377-5b98bf4d-e686-45a8-b703-e88e048c15a0.png)![image](https://user-images.githubusercontent.com/86902176/210493387-8cb24274-f5bb-44df-920d-1609c03f2373.png)


### 2. More detail description on how the motor controller boards and servo controller boards are designed and interconnected and major contents
![image](https://user-images.githubusercontent.com/86902176/174633484-6e45d11e-2d77-4a2a-971e-131381582f39.png)
### 3. More detail description on how the signal distribution board and Beaglebone boards are designed and interconnected and major contents
![image](https://user-images.githubusercontent.com/86902176/174633784-b99495cb-5e77-45e7-bfd6-55dcb09a5f9e.png)

## Level 3 Design
### 1. Schematic for signal distribution board
![image](https://user-images.githubusercontent.com/86902176/176084972-1773b4e3-cb35-4c02-b73e-a2e0e7342a48.png)
### 2. Schematic for Motor Controller board 1: Microcontroller and LM629
![image](https://user-images.githubusercontent.com/86902176/176233278-bef5f5db-fc33-4c7e-8434-e325176a3fe3.png)
### 3. Schematic for Motor Controller board 2: H-Bridges and Digital Isolators
![image](https://user-images.githubusercontent.com/86902176/174715582-542cd244-dcf4-4c64-b069-e2e6f9961d11.png)
### 4. Schematic for Servo Controller
![image](https://user-images.githubusercontent.com/86902176/174934736-737fa402-96cc-45e1-9538-3f4c4714205a.png)




## Goals
The goal of the project was to design and build a mobile robot that could be controlled from at least 50 miles away. One major constraint is that the robot has to fit inside a narrow tunnel. 

The robot will be controlled through an arbitrary maze, pick up a load at a "pickup location" and drop it off at a later section of the maze, the "drop off" location. Points were awarded for avoiding obstacles and time of completion.

## Process
The motor controller was designed using the PIC18F46K22 from Microchip. The MCU controls four precision motor-controllers which are the LM629 from TI.  The PWM output signal from the LM629 and the PWM inputs from the LMD18200Ts (H-Bridge from TI) are interfaced using an ISO7220 (Digital Isolator from TI). 

The servos were controlled by signals generated by an MSP430G2553 from TI. Optocouplers were used to interface the MCU output and the servos' input. 

The PIC18F46K22 was controlled via SPI coming from a PIC18F46K22 and the MSP430G2553 using the UART port. This controller also controlled a relay to turn on and off the main light in the robot. This board had a header pins to easily expand with additional boards. For example, add telemetry boards. 

The commands were sent through remote connection to the BeagleBone Dev board throuh ssh. A terminal was open and the serial port was controlled via keyboard characters. The characters would be sent through UART to the PIC18F46K22 board. This board would then decide of the command belonged to the servos, motors, or lights. It would then "translate" it and send it to the right controller. 

In order to control the robot remotely, a network router was hacked and a new OS was installed which added a network bridge capability. In summary, the router (now bridge) would connect to a network through Wifi and then give internet (or local network) connection to the BeagleBone through ethernet cable. 
This was chosen in order to take advantage of the wifi signal "power" performance by this device in comparison to the XBee (which was a popular wifi module in the time this project was being built).

In order to make the design robust, it was taken into account that having many brushed DC motor for the main movement and servos was going to  introduce noise into the microcontrollers and microprocessor. Also, the pcbs were made on pre-punch board and a cnc milling machine, so no proper multilayer board with ground plane was used. Therefore, it was decided that using two separate powersupplies would make the design more robust. One battery pack would power the motors and a smaller one would power the logic controllers. Both were Isolated by using both optocouplers and highspeed digital isolators. 

Since Lithium Polymer batteries were used, the competition required that the robot included an undercharge protection circuit. A simple comparator circuit powered with a 9V battery was used with a relay in order to disconnect the lithium batteries from the circuit in case they would reach a threshold voltage of 3.4V. This was not part of the original Sr. Design project but I am planning on designing an elegant (and modern) power management solution after all other files have been updated and tested. 
