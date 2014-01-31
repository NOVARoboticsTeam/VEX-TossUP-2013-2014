#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  enc1,           sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  enc2,           sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_6,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           leftRear,      tmotorVex393_HBridge, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port2,           leftFront,     tmotorVex393_MC29, openLoop, encoderPort, I2C_6)
#pragma config(Motor,  port3,           leftShooter,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftArm,       tmotorVex393_MC29, openLoop, encoderPort, I2C_5)
#pragma config(Motor,  port7,           rightArm,      tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port8,           rightShooter,  tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           rightFront,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port10,          rightRear,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_3)
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
void forward()
{
	motor[rightFront] = 127;
	motor[rightRear] = 127;
	motor[leftFront] = 127;
	motor[leftRear] = 127;
}
void reverse()
{
	motor[rightFront] = -127;
	motor[rightRear] = -127;
	motor[leftFront] = -127;
	motor[leftRear] = -127;


}
void turnRight()
{
	motor[rightFront] = -127;
	motor[rightRear] = -127;
	motor[leftFront] = 127;
	motor[leftRear] = 127;
}
void turnLeft()
{
	motor[rightFront] = 127;
	motor[rightRear] = 127;
	motor[leftFront] = -127;
	motor[leftRear] = -127;
}
void halt()
{
	motor[rightFront] = 0;
	motor[rightRear] = 0;
	motor[leftFront] = 0;
	motor[leftRear] = 0;
}
void restEncoders()
{
	nMotorEncoder[leftFront]=0;
	nMotorEncoder[leftRear]=0;
	nMotorEncoder[rightFront]=0;
	nMotorEncoder[rightRear]=0;
}
void UpArm()
{
	motor[leftArm] = 127;
	motor[rightArm] = 127;
}
void DownArm()
{
	motor[leftArm] =-127;
	motor[rightArm] = -127;
}
void ArmStop()
{
	motor[leftArm] = 0;
	motor[rightArm] = 0;
}

void shooter(int time, int currSpeed, int maxSpeed, int ms)
{
	int res = 10;
	for(int t = 0; t < ms; t+=res)
	{
		int pwr = currSpeed + t *(maxSpeed - currSpeed) / ms;
		motor[leftArm] = pwr;
		motor[rightArm] = pwr;
		wait1Msec(ms);
	}
}
void strafeRight()
{
	motor[rightFront] = -127;
	motor[rightRear] = 127;
	motor[leftFront] = 127;
	motor[leftRear] = -127;
}
void strafeLeft()
{
	motor[rightFront] = 127;
	motor[rightRear] = -127;
	motor[leftFront] = -127;
	motor[leftRear] = 127;
}


void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
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
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
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
	 if(vexRT[Btn7U] == 1)
		{
			mode = 2;
		}
		if(vexRT[Btn8U] == 1)
		{
			mode = 1;
		}
		switch(mode)
		{
			case 1:// Drive Mode
				motor[leftFront] = vexRT[Ch3] + vexRT[Ch4];
				motor[rightFront] = vexRT[Ch2] - vexRT[Ch1];
				motor[leftRear] = vexRT[Ch3] - vexRT[Ch4];
				motor[rightRear] = vexRT[Ch2] + vexRT[Ch1];
				break;
			case 2: // Shooter Mode

				motor[leftShooter] = vexRT[Ch2];
				if(vexRT[Btn5U] == 1)
				{
					motor[leftArm] = 95;
					motor[rightArm] = 95;
				}
				else if(vexRT[Btn5D] == 1)
				{
					motor[leftArm] = -95;
					motor[rightArm] = -95;
				}
				else
				{
					motor[leftArm] = 0;
					motor[rightArm] = 0;
				}

				motor[rightShooter] = vexRT[Ch2];

				break;
			}
	}
}
