#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl11, solenoid1,      sensorDigitalOut)
#pragma config(Sensor, dgtl12, solenoid2,      sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           leftRear,      tmotorVex393_HBridge, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port2,           leftFront,     tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port3,           flash1,        tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port4,           flash2,        tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port9,           rightFront,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_4)
#pragma config(Motor,  port10,          rightRear,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

// Directional and motor speed functions - Enable the robot to move in a certain direction, stop motors, turn, etc.
void forward(int speed)
{
	motor[rightFront] = speed;
	motor[rightRear] = speed;
	motor[leftFront] = speed;
	motor[leftRear] = speed;
}
void reverse(int speed)
{
	motor[rightFront] = -speed;
	motor[rightRear] = -speed;
	motor[leftFront] = -speed;
	motor[leftRear] = -speed;


}
void turnRight(int speed)

{
	motor[rightFront] = -speed;
	motor[rightRear] = -speed;
	motor[leftFront] = speed;
	motor[leftRear] = speed;
}
void turnLeft()
{
	motor[rightFront] = speed;
	motor[rightRear] = speed;
	motor[leftFront] = -speed;
	motor[leftRear] = -speed;
}
void halt()
{
	motor[rightFront] = 0;
	motor[rightRear] = 0;
	motor[leftFront] = 0;
	motor[leftRear] = 0;
}
void resetEncoders()
{
	nMotorEncoder[leftFront]=0;
	nMotorEncoder[leftRear]=0;
	nMotorEncoder[rightFront]=0;
	nMotorEncoder[rightRear]=0;
}
// Strafing Functions - Enables the robot to move laterally at an indicated speed by the user.
void strafeRight()
{
	motor[rightFront] = -speed;
	motor[rightRear] = speed;
	motor[leftFront] = speed;
	motor[leftRear] = -speed;
}
void strafeLeft()
{
	motor[rightFront] = speed;
	motor[rightRear] = -speed;
	motor[leftFront] = -speed;
	motor[leftRear] = speed;
}

//Function to actuate and de-actuate the pneumatics
void tossUp()
{
	SensorValue[solenoid1] = 1;
	SensorValue[solenoid2] = 1;
}

void tossDown()
{

	SensorValue[solenoid1] = 0;
	SensorValue[solenoid2] = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{


	clearTimer(T1);
	tossUp();
  wait1Msec(2000);
  tossDown(); // Brings pneumatic scoop down and in position to move game objects
  wait1Msec(2000);
	resetEncoders();

  while(nMotorEncoder[leftRear] < 450) // Robot moves forward to pick up game first game object directly in front of it
  {
  	forward(63);
  }
  halt();
  wait1Msec(500);
  resetEncoders();
  while(nMotorEncoder[leftRear] > -300 || time1[T1] < 2000) // Robot backs up with game object
  {
  	reverse(63);
  }
  halt();
	tossUp();  // Flings game object with pneumatic scoop.
	wait1Msec(2000);
	tossDown();
	halt();
	wait1Msec(500);
	resetEncoders();

	//Following features are a repetition of the earlier sequence
	while(nMotorEncoder[leftRear] < 400)
	{
		forward(63);
	}
	halt();
	wait1Msec(500);
	clearTimer(T1); // Timer is used as a time out and backup in case integrated motor encoders are inoperational.
	resetEncoders();
	while(nMotorEncoder[leftRear] > -300 || time1[T1] < 2000)
	{
		reverse(63);
	}
	halt();
	tossUp();
	wait1Msec(2000);
	tossDown();
	wait1Msec(2000);
	halt();
	resetEncoders();
	/*while(nMotorEncoder[leftRear] >  -1000)
	{
		turnLeft();
	}
	halt();
	wait1Msec(500);*/


}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{
				// Using a tank-style mecanum drive. Ch3 is the left vertical channel, Ch4 is the left horizontal channel
				// Likewise, Ch2 and Ch1 are the right vertical and right horizontal channels, respectively
				motor[leftFront] = vexRT[Ch3] + vexRT[Ch4];
				motor[rightFront] = vexRT[Ch2] - vexRT[Ch1];
				motor[leftRear] = vexRT[Ch3] - vexRT[Ch4];
				motor[rightRear] = vexRT[Ch2] + vexRT[Ch1];

				if(vexRT[Btn5U] == 1)
				{
					motor[flash1] = 127;
					motor[flash2] = 127;
				}
				else if(vexRT[Btn5D] == 1)
				{
					motor[flash1] = 63;
					motor[flash2] = 63;
				}
				else {
					motor[flash1] = 0;
					motor[flash2] = 0;
				}

		if(vexRT[Btn6U] == 1 )
		{
			tossUp();
		}
		else if (vexRT[Btn6D] == 1)
		{
			tossDown();
		}
	}
}
