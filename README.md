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


<img src="https://user-images.githubusercontent.com/86902176/210908299-32d6d7c0-e4d3-4a94-a6c7-3028bff9f2bf.jpg" width="400">


## Software Tools and Libraries
To develop this robot, the following software and hardware were used:
#### 1. MSP430 and PIC MCUs on custom PCBs
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
  
  
## Theory of Operation
In order to complete the project in a modular manner, the mobile robot has been divided in two parts, the "Front-End" and the "Back-End". 

The front-end of the project is where the end-user controls the robot (on a host computer). The end-user will be able to see what the robot sees with the camera. And the end-user will be sending commands to the robot to move, pan/tilt the camera, raise/lower the robotic arm, open/close the claw, and turn on/off the headlights. 

The back-end of the project is the actual mobile robot and is where all the "essential" modules are located. The back-end has been divided even further to accomplish the modular philosophy, the modules are: 

The "BeagleBone Black", which is in charge of connecting to the network and receive commands through SSH from the front-end. It also fetches video feed from a USB camera and sends the video stream to the front end-through the network.

The "Signal Distribution Board" (SDB), which is in charge of receiving commands in form of data packages from the BeagleBone through UART. This module separates and normalizes the commands depending on who the recipient should be and then it forwards them to the correct module.

The "Motor Controller Boards", which are composed of one digital signals PCB and an analog signals PCB. The digital board is in charge of receiving speed and direction commands forwarded by the SDB through SPI communication. The digital board will then create the PWM and direction signals and forward them to the analog PCB. The analog signal PCB is in charge of sending power to the motors and receive quadrature signals from the motors, then forward these quadrature signals to the digital signal board for processing. 

And the "Servo Controller Board", which is in charge of receiving commands from the SDB through UART. This module will generate the appropriate waveforms to control the servos. 


## Level 0 Design
In order to accomplish the listed goals, the robot is implemented with the following features:
- Use Wi-Fi for the remote communication between the robot and the host computer
- Send video from a USB camera to the host computer
- Use servos to pan and tilt the camera
- Use servos to raise and lower a robotic arm
- Use servos to open and close a robotic claw
- Use motor with quadrature encoders to control the speed and number of rotations per wheel
- Use a high-power LED in order to illuminate dark tunnels
- The robot will receive movement commands from a host computer, the host computer will use an intuitive controller for ease of use

1. Level 0 Block Diagram.\
![image](https://user-images.githubusercontent.com/86902176/210486303-3b0617af-7adf-4650-8a77-cf55e66d81d8.png)


## Level 1 Design
The level 1 design shows how the front-end and the back-end communicate each other (through Wi-Fi). It also exposes the inputs and outputs of both blocks of the robot. Level 1 block diagram also exposes the datapath between the modules inside each "end".
1. Block layout showing data flow direction of both "front-end" and "back-end" of the project.\
![image](https://user-images.githubusercontent.com/86902176/210908444-fa9e6243-dd37-4d05-9dc9-58c161157343.png)


## Level 2 Design - Front-End
The front-end is comprised of a Windows or Linux machine with GStreamer installed. The machine must be connected to the same network as the robot (back-end). The front-end will connect to the robot using the SSH protocol. To send direction and magnitude of the speed of the motors and the position for the servos, a game console controller is used (it's a hacked controller with an mcu in order to emulate an HID keyboard device).
1. Block layout shows the dataflow of the front-end of the project.\
![image](https://user-images.githubusercontent.com/86902176/210492917-448a8e00-8317-46eb-9bfd-d9d7d7a50a31.png)

2. Host computer showing the camera view and the terminal where commands get entered for movement. Also the back of the gaming-controller showing the microcontroller used to desguise the controller as an HID keyboard.\
<img src="https://user-images.githubusercontent.com/86902176/210908971-016f76a6-f4bb-4fb2-b1e3-9ace71dcdac5.png" height="250"><img src="https://user-images.githubusercontent.com/86902176/210908633-5c49d080-269e-4061-a423-69dc508bfc7e.jpg" height="250">


## Level 2 Design - Back-End
The back-end is composed of 5 different systems/modules to makes the design modular but some can be added or removed. The current modules are: BeagleBone Black, Signal Distribuition Board (SDB), Servo Controller Board, Motor Controller Boards (digital and analog/power), and Power system.
1. Block layout shows the dataflow of the modules of the back-end of the project.\
![image](https://user-images.githubusercontent.com/86902176/210696106-f420226a-bdaa-43b8-8d37-a618314dc8e8.png)

## Level 3 Design - Back-End - PCB Schematics
All PDF schematics for each module can be found in their folder. These are just a quick snapshot of each schematic.

1. Signal Distribution Board Schematic\
<img src="https://user-images.githubusercontent.com/86902176/210705994-c06d46e0-a386-4648-9054-63de097d0f2b.png" width="400">

2. Servo Controller Board Schematic - Digital side of the PCB and analog side are separated\
<img src="https://user-images.githubusercontent.com/86902176/210706305-5fbbff53-45f5-462f-97bf-1a352f3a2b14.png" width="400"><img src="https://user-images.githubusercontent.com/86902176/210706352-409e2e4a-9fb5-405f-b2b5-e00aa16158ad.png" width="400">

3. Motor Controller Board Schematic - Digital Board\
![image](https://user-images.githubusercontent.com/86902176/210706708-271431d6-ad51-4ac0-a936-6fe1c7243b5f.png)
4. Motor Controller Board Schematic - Analog/Power Board\
![image](https://user-images.githubusercontent.com/86902176/210706596-0c030e02-dabd-4e11-a56a-b1e2b7f5723c.png)


## Actual PCBs - Back-End
These are the PCBs after being manufactured. All parts are hand soldered.

1. Signal Distribution PCB\
![IMG_7205 small](https://user-images.githubusercontent.com/86902176/210707529-e5189f8d-d040-4c28-9440-9db708ec286e.jpg)

2. Servo Controller PCB\
![IMG_7215 small](https://user-images.githubusercontent.com/86902176/210707273-ee87da58-5249-4ac9-8f3a-dcf85bbc1875.jpg)

3. Motor Controller PCBs stacked on top of each other and connected with a ribbon cable\
![IMG_7198 small](https://user-images.githubusercontent.com/86902176/210707456-f860a59f-0f26-4ab8-9321-9175c162ece5.jpg)




