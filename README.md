# MERCURY ROBOTICS ROBOT

## Overall Project
This repository contains code, schematics and other material for a mobile robot. The objective of the robot was to accomplish the following tasks:
### 1. It needs to be controlled remotely
### 2. It should be capable of goiang through an obstacle course
### 3. It should be able to perform maneuvers in the minimum amount of time
### 4. It must be able to pick-up and drop-off a payload

![IMG_7032-25](https://user-images.githubusercontent.com/86902176/210485465-72bf009b-7da3-450a-979c-4d90989f97b9.jpg)

## Level 0 Design
In order to accomplish the listed goals, the robot was implemented with the following features:
### 1. Use Wi-Fi for the remote communication between the robot and the host computer
### 2. Send video from a USB camera to the host computer
### 3. Use servos to pan and tilt the camera
### 4. Use servos to raise and lower a robotic arm
### 5. Use servos to open and close a robotic claw
### 6. Use motor with quadrature encoders to control the speed and number of rotations per wheel
### 7. Use a high-power LED in order to illuminate dark tunnels
### 8. The robot will receive movement commands from a host computer, the host computer will use an intuitive controller for ease of use

![image](https://user-images.githubusercontent.com/86902176/210486303-3b0617af-7adf-4650-8a77-cf55e66d81d8.png)
Level 0 Block Diagram

## Level 1 Design
### 1. Block layout showing data flow direction
![image](https://user-images.githubusercontent.com/86902176/174416674-cc60901d-9219-4426-ac0f-3b5184c8a9f3.png)
### 2. Boards and main components interconnected with an overview of the power sources
![image](https://user-images.githubusercontent.com/86902176/174632935-cef2efe2-cfb8-4333-b616-4f848ba05241.png)


## Level 2 Design
### 1. Block layout showing data flow and broken down in smaller modules with a more detailed description of each block actions
![image](https://user-images.githubusercontent.com/86902176/174446758-db5e3fb6-9d6c-4597-a5c5-6b0e32ab003f.png)
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
