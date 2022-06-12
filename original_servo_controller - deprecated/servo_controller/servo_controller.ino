/*
 * This program controls 3 servos:
 * One to raise the arm,
 * One to close the claw,
 * and one to pan the camera
 * 
 * UART communication is used at 19200 Baud Rate
 * Libraries used are included in the Arudione Environment
 */
#include<Servo.h>

Servo claw, arm, cam;
unsigned int max_cam;
unsigned int min_cam;
unsigned int max_arm;
unsigned int min_arm;
unsigned int arm_degree;
unsigned int cam_degree;

void setup() {
  //Servos connected to their corresponding pins
  claw.attach(10);
  arm.attach(11);
  cam.attach(6);
  
  //Setup Serial
  Serial.begin(19200);

  //Initial values
  max_cam = 130; //sets the max value the servo for the cam in order to be aimed forward
  min_cam = 92;  //sets the min value the servo for the cam in order to be aimed towards the claw
  max_arm = 145; //sets the max value so the arm can be fully risen
  min_arm = 42;  //sets the min value so the arm can be lower to the height of the load
  arm_degree = min_arm;
  cam_degree = min_cam;
}

void loop() {
  //scan for command
  char command;
  command = Serial.read();
  switch(command)
  {
    //close the claw controlled by the servo
    case 'j':
      claw.write(160);
      delay(15);
      break;

      //open the claw controlled by the servo
    case 'l':
      claw.write(0);
      delay(15);
      break;

      //rise the arm to the maximum angle
    case 'i':
      for(arm_degree = min_arm; arm_degree<=max_arm; arm_degree++)
      {
        arm.write(arm_degree);
        delay(15);
      }
      break;

      //lower the arm to the min angle
    case 'k':
      for(arm_degree=max_arm; arm_degree>=min_arm; arm_degree--)
      {
        arm.write(arm_degree);
        delay(15);
      }
      break;

      //move the cam so it can face forward
    case 'o':
      for(cam_degree=min_cam; cam_degree<=max_cam; cam_degree++)
      {
        cam.write(cam_degree);
        delay(15);
      }
      break;

      //move the cam so it can face the load
    case 'u':
      for(cam_degree=max_cam; cam_degree>=min_cam; cam_degree--)
      {
        cam.write(cam_degree);
        delay(15);
 
      }
      break;

    default :
      command = 'l';
      break;
      
    
  }

}
