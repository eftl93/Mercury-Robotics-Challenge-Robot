# Servo Controller Board
This is a modified version of my servo controller code found in: https://github.com/eftl93/servo_controller_msp430.

The adaptations made are the commands needed to change the waveform to increase or decrease the angle of the servos and the neutral values on reset. Also, the ranges allowed for the servos to move in oder to protect the camera and harm to move to a position that can damage the robot. In this version of the code, the bytes that affect the position of the servos are: 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8. If any other characters are received, the servos should not change position. A small modification was made on the hardware for this project, the node connecting all the transistors' collectors to the positive battery terminal was cut, and a wire was hand soldered to tie the collectors to the output of the voltage regulator. 

The baudrate in for the controller to receive commands from the Signal Distribution Board is 115200 Baud, 8-bit, no parity bit, and 1 stop bit. 

- 0xC1: Pan the camera to the right
- 0xC2: Pan the camera to the left
- 0xC3: Tilt the camera up
- 0xC4: Tilt the camera down
- 0xC5: Lower the robotic arm
- 0xC6: Raise the robotic arm
- 0xC7: Open the robotic claw
- 0xC8: Close the robotic claw

Every time one of the commands it's received, the servo will move one "notch" until it reaches the programmed limit

## Servo controller board with optocouplers (4-Servos, UART only)
![IMG_7215 small](https://user-images.githubusercontent.com/86902176/210853678-27d59dab-7ffd-4b7e-b3c8-7b95e04f6652.jpg)



