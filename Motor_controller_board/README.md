# Motor Controller Boards
There are two Motor Controller boards, one is in charge receiving commands from the Signal Distribution board, process the commands, and send the required PWM signals to the other second controller board (the analog/power side). The second controller board is in charge of receving the PWM signals and send the power to the motors. 

The main components of this module are: a microcontroller (PIC18F46K22), precission motor controllers (LM629), an H-Bridges that can take sign-magnitude signals (one pin determines direction of motion and the other pin send a PWM wave), and motors with quadrature encoders. 

The heart of this module is the LM629 IC, this precission motor controller contains an internal PID controller and takes commands from a microcontroller. The microcontrollers sets up the LM629 by sending the Kp, Ki, and Kd coefficients and telling it in which mode it will be running (absolute or relative velocity for example or maybe just trajectory). The the microcontroller will send a motion command through parallel communication by specifying various motion characteristics such as acceleration, max speed, direction and distance. The motor controller will send motion signals in the form of sign-magnitude to an H-Bridge. The H-Bridge then powers the motor and the motion starts. The motors start sending the quadrature encoder signals back to the LM629. The LM629 compares the frequency and the phase of the two square waves and adjusts its output (sign-magnitude) to accomplish the trajectory set by the microcontroller.

### Board 1 - Microcontroller and speed controllers - Most recent revision
![IMG_7177 small](https://user-images.githubusercontent.com/86902176/210856783-05f43429-3915-47e5-b956-bd7599c376cf.jpg)

### Board 2 - H-Bridges with digital isolators  - Most recent revision
![IMG_7178 small](https://user-images.githubusercontent.com/86902176/210856749-2419b5b7-5c5d-4934-94ed-4af678ede0d9.jpg)

### Both Board 1 and Board 2 connected and stacked together
![IMG_7198 small](https://user-images.githubusercontent.com/86902176/210856992-382ad774-9248-41c2-8b7d-1c00f1276f60.jpg)
