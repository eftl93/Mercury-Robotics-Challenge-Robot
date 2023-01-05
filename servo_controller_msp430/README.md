# Servo Controller Board
This is a modified version of my servo controller code found in: https://github.com/eftl93/servo_controller_msp430.

The adaptations made are the commands needed to change the waveform to increase or decrease the angle of the servos and the neutral values on reset. Also, the ranges allowed for the servos to move in oder to protect the camera and harm to move to a position that can damage the robot. In this version of the code, the bytes that affect the position of the servos are: 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8. If any other characters are received, the servos should not change position.

## Servo controller board with optocouplers (4-Servos, UART only)
![image](https://user-images.githubusercontent.com/86902176/174622239-b1181c09-46c7-4f20-95b8-11c4fdaaf4e2.png)

## Servo controller board with optocouplers (5th Servo connector is not programmed and the SPI function is not included)
![image](https://user-images.githubusercontent.com/86902176/174934166-783462df-71e4-4b8a-afad-639ecdafa3f0.png)

## This PCB was used for the Servo controller. A small modification was made on this version, the node connecting all the transistorss collectors to the positive battery terminal was cut, and a wire was hand soldered to tie the collectors to the output of the voltage regulator. 
![IMG_7205 small](https://user-images.githubusercontent.com/86902176/210846045-ffa0ca04-06fa-4905-8359-624b0056e45d.jpg)

