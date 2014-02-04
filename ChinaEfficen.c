#pragma config(I2C_Usage, I2C1, i2cSensors)

#pragma config(Sensor, in1,		 batVolt,				 sensorAnalog)
#pragma config(Sensor, in2,		 pot,						 sensorPotentiometer)
#pragma config(Sensor, dgtl1,	 Launcher,			 sensorDigitalOut)
#pragma config(Sensor, dgtl11, solenoid2,			 sensorDigitalOut)
#pragma config(Sensor, dgtl12, solenoid,			 sensorDigitalOut)
#pragma config(Sensor, I2C_1,	 leftFront,			 sensorQuadEncoderOnI2CPort,		, AutoAssign)
#pragma config(Sensor, I2C_2,	 rightFront,		 sensorQuadEncoderOnI2CPort,		, AutoAssign)
#pragma config(Sensor, I2C_3,	 leftBack,			 sensorQuadEncoderOnI2CPort,		, AutoAssign)
#pragma config(Sensor, I2C_4,	 rightBack,			 sensorQuadEncoderOnI2CPort,		, AutoAssign)
#pragma config(Motor,	 port1,						FrontLeft2,		 tmotorVex393HighSpeed_HBridge, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,	 port2,						Intake,				 tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,	 port3,						FrontLeft1,		 tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,	 port4,						LeftBack1,		 tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,	 port5,						LeftBack2,		 tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,	 port6,						RightBack1,		 tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,	 port7,						RightBack2,		 tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,	 port8,						RightFront1,	 tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,	 port9,						Arm,					 tmotorVex393_MC29, openLoop)
#pragma config(Motor,	 port10,					RightFront2,	 tmotorVex393HighSpeed_HBridge, openLoop, reversed, encoderPort, I2C_4)
//*!!Code automatically generated by 'ROBOTC' configuration wizard							 !!*//


#pragma platform(VEX)


//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"		//Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//													Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
//
////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	float Ki;
	float Kp;
	float Kd;
	float error;
	float integral;
	float derivative;
	float targetVal;
	float lastError;
	bool isRunning;
	}
PIDController;
int count = 0;
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;

//Wait for Press--------------------------------------------------
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------

//Wait for Release------------------------------------------------
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}

void displayBatteryVoltage() {
	string mainBattery, batteryVoltage;
	displayLCDString(0, 0, "Primary: ");
	displayLCDString(1, 0, "PowerExp: ");
	sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
	sprintf(batteryVoltage, "%1.2f%c", (SensorValue[batVolt] /280), 'V');//open the bomb bay dorrs hal!! im afriad i cant do that........
	setLCDPosition(0,10);
	displayNextLCDString(mainBattery);
	setLCDPosition(1, 10);
	displayNextLCDString(batteryVoltage);
}
void pre_auton()
{


	SensorValue[Launcher] = 1;
	clearLCDLine(0);
	clearLCDLine(1);
	SensorValue[Launcher]=1;
	//Loop while center button is not pressed
	while(nLCDButtons != centerButton)
	{
		//Switch case that allows the user to choose from 4 different options
		switch(count){
		case 0:
			//Display first choice
			displayLCDCenteredString(0, "Red Middle Mode");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count = 3;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 1:
			//Display second choice
			displayLCDCenteredString(0, "Blue Middle Mode");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 2:
			//Display third choice
			displayLCDCenteredString(0, "Red Middle Bash");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 3:
			//Display fourth choice
			displayLCDCenteredString(0, "Blue Middle Bash");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 0;
			}
			break;
		default:
			count = 0;
			break;
			startTask(usercontrol);

		}

	}

}


void forward()
{
	motor[RightFront1] = 127;
	motor[RightFront2] = 127;

	motor[RightBack1] = 127;
	motor[RightBack2] = 127;

	motor[FrontLeft1] = 127;
	motor[FrontLeft2] = 127;

	motor[LeftBack1] = 127;
	motor[LeftBack2] = 127;
}
void reverse()
{
	motor[RightFront1] =-127;
	motor[RightFront2] =-127;

	motor[RightBack1] =-127;
	motor[RightBack2] =-127;

	motor[FrontLeft1] =-127;
	motor[FrontLeft2] =-127;

	motor[LeftBack1] =-127;
	motor[LeftBack2] =-127;
}
void turnRight()
{
	motor[RightFront1] = -127;
	motor[RightFront2] = -127;

	motor[RightBack1] = -127;
	motor[RightBack2] = -127;

	motor[FrontLeft1] =127;
	motor[FrontLeft2] =127;

	motor[LeftBack1] =127;
	motor[LeftBack2] =127;
}
void turnLeft()
{
	motor[RightFront1] = 127;
	motor[RightFront2] = 127;

	motor[RightBack1] = 127;
	motor[RightBack2] = 127;

	motor[FrontLeft1] =-127;
	motor[FrontLeft2] =-127;

	motor[LeftBack1] =-127;
	motor[LeftBack2] =-127;
}
void strafeRight()
{
	motor[RightFront1] = -127;
	motor[RightFront2] = -127;

	motor[RightBack1] = 127;
	motor[RightBack2] = 127;

	motor[FrontLeft1] =127;
	motor[FrontLeft2] =127;

	motor[LeftBack1] =-127;
	motor[LeftBack2] =-127;
}
void strafeLeft()
{
	motor[RightFront1] = 127;
	motor[RightFront2] = 127;

	motor[RightBack1] = -127;
	motor[RightBack2] = -127;

	motor[FrontLeft1] =-127;
	motor[FrontLeft2] =-127;

	motor[LeftBack1] =127;
	motor[LeftBack2] =127;
}

void halt()
{
	motor[RightFront1] = 0;
	motor[RightFront2] = 0;

	motor[RightBack1] = 0;
	motor[RightBack2] = 0;

	motor[FrontLeft1] =0;
	motor[FrontLeft2] =0;

	motor[LeftBack1] =0;
	motor[LeftBack2] =0;
}
void resetEncoders()
{
	nMotorEncoder[FrontLeft2]=0;
	nMotorEncoder[LeftBack1]=0;
	nMotorEncoder[RightBack2]=0;
	nMotorEncoder[RightFront2]=0;
}
//void UpArm()
//{
//	motor[Arm] = 127;
//}
//void DownArm()
//{
//	motor[Arm] =-127;
//}
void ArmStop()
{
	motor[Arm] = 0;
}
void intake()
{
	motor[Intake] = -127;
}
void outake()
{
	motor[Intake] = 127;
}
void StopIntake()
{
	motor[Intake] = 0;
}
void sustain()
{
	motor[Arm] = 64;
}

void UpArm(short x)
{
	motor[Arm] = x;
}
void DownArm(short x)
{
	int down = 0;
	while(down== 0)
	{
		motor[Arm] = -x;
		if(SensorValue[pot] <= 260)
		{
			down = 1;
		}
	}
	if(down > 0)
	{
		motor[Arm] = 0;
	}
}





task autonomous()
{
	clearLCDLine(0);
	clearLCDLine(1);
	//Switch Case that actually runs the user choice
	switch(count){
	case 0:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "Red Middle Mode");//red middle zone autonomous
		displayLCDCenteredString(1, "is running!");
		wait1Msec(2);
		outake();				 //outakes for 1 sec. then intakes for one sec.
		wait1Msec(1000);
		intake();
		wait1Msec(1000);
<<<<<<< HEAD
		SensorValue[Launcher] = 0;
		while(SensorValue[pot] < 2000)
		{
		UpArm(127);
		}

		sustain();//keeps the arm at level height
=======
		//StopIntake();//stops the intake and raises the arm
		//this will be controlled by potentiometer
		SensorValue[Launcher] = 0;
		UpArm();
		wait1Msec(1000);
		motor[Arm] = 40;//keeps the arm at level height
>>>>>>> Updated ChinaEfficen auto



		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//goes forward to knock over big ball on trough
		{
			forward();

		}
		halt();//stops the robot
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -500)//goes backwards
		{
			reverse();
		}
		halt();
		resetEncoders();
<<<<<<< HEAD
		while(nMotorEncoder[RightBack2] <	 560)//strafes to the right to knock over the other big ball
=======
		while(nMotorEncoder[RightBack2] <  1200)//strafes to the right to knock over the other big ball
>>>>>>> Updated ChinaEfficen auto
		{
			strafeRight();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 650)//goes forward to knock over big ball
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -400)//backs away from the trough while lowering arm
		{
			reverse();
<<<<<<< HEAD
			DownArm(127);
=======

>>>>>>> Updated ChinaEfficen auto
		}
		DownArm();
		halt();
		ArmStop();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -980 && SensorValue[leftBumper] == 0 )//strafes to the left to line up with goal
		{
			strafeLeft();
		}
		halt();

		resetEncoders();
		while(nMotorEncoder[RightBack2] < 250)
		{
			strafeRight();
		}
		halt();
		wait1Msec(500);
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 1100)//goes forward to knock over big ball on trough
		{
			forward();

		}
		halt();//stops the robot
		resetEncoders();
		while(SensorValue[pot] < 2200)
		{
<<<<<<< HEAD
			forward();
			UpArm(127);
=======
			UpArm();
		}
		while(nMotorEncoder[RightBack2] <  995)//strafes to the right to knock over the other big ball
		{
			strafeRight();
>>>>>>> Updated ChinaEfficen auto

		}
		resetEncoders();
		/*
		while(nMotorEncoder[RightBack2] <  1120)//strafes to the right to knock over the other big ball
		{
			strafeRight();

<<<<<<< HEAD
		halt();//robot stops at goal and arm is kept level by small motor resistance
		sustain();
=======
		}*/
		while(nMotorEncoder[RightBack2] < 375)
		{
			forward();
		}
		halt();
		resetEncoders();
>>>>>>> Updated ChinaEfficen auto
		outake();
		wait1Msec(1500);

		break;
	case 1:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "Blue Middle Mode");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(2);
		outake();
		wait1Msec(1000);
		intake();
		wait1Msec(1000);//outakes then intakes and then raises arm
		SensorValue[Launcher] = 0;
		while(SensorValue[pot] < 2000)
		{
		UpArm(127);
	}

	sustain();
		resetEncoders();
		while(nMotorEncoder[RightBack2] >- 500)//moves forward to raise arm
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -500)//backs up
		{
			reverse();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -560)//strafes left to knock over other big ball
		{
			strafeLeft();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//moves forward to knock over bigball
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -300)//backs up while lowering arm
		{
			reverse();
			DownArm(127);
		}
		halt();
		ArmStop();
		resetEncoders();
		while(nMotorEncoder[RightBack2] <	 260)//strafes right to line up with goal
		{
			strafeRight();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 360)//moves forward to go under tough
		{
			forward();

		}
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//continues out of trough to raise arm till it reaches goal
		{
			forward();
			UpArm(127);

		}
		halt();
		sustain();//keeps arm at level
		outake();
		wait1Msec(2000);//outakes for 2 sec.
		StopIntake();
		break;
	case 2:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "Red Middle Bash");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(2);
		outake();				 //outakes for 1 sec. then intakes for one sec.
		wait1Msec(1000);
		intake();
		wait1Msec(1000);
		SensorValue[Launcher] = 0;
		while(SensorValue[pot] < 2000)
		{
		UpArm(127);
	}//this will be controlled by potentiometer

	sustain();//keeps the arm at level height



		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//goes forward to knock over big ball on trough
		{
			forward();

		}
		halt();//stops the robot
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -500)//goes backwards
		{
			reverse();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] <	 560)//strafes to the right to knock over the other big ball
		{
			strafeRight();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//goes forward to knock over big ball
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -300)//backs away from the trough while lowering arm
		{
			reverse();
			DownArm(127);
		}
		halt();
		ArmStop();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -300)//strafes to the left to line up with goal
		{
			strafeLeft();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 200)//goes forward underneath the trough
		{
			forward();

		}//does not include a stop command because i want it to move under the trough and then to raise the arm
		//while continuing to move forward
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//continues to move forward while raising the arm
		{
			forward();
			UpArm(127);

		}

		halt();//robot stops at goal and arm is kept level by small motor resistance
		sustain();
		outake();
		wait1Msec(2000);//outakes preload into goal
		StopIntake();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -360)//goes backwards
		{
			reverse();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -500)
		{
			turnLeft();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -1000)
		{
			reverse();
		}
		halt();



		break;
	case 3:
		//If count = 3, run the code correspoinding with choice 4
		displayLCDCenteredString(0, "Blue Middle Bash");
		displayLCDCenteredString(1, "is running!");
		wait1Msec(2);
		outake();
		wait1Msec(1000);
		intake();
		wait1Msec(1000);//outakes then intakes and then raises arm
		SensorValue[Launcher] = 0;
		while(SensorValue[pot] < 2000)
		{
		UpArm(127);
	}

		sustain();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//moves forward to raise arm
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -500)//backs up
		{
			reverse();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -560)//strafes left to knock over other big ball
		{
			strafeLeft();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//moves forward to knock over bigball
		{
			forward();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -300)//backs up while lowering arm
		{
			reverse();
			DownArm(127);
		}
		halt();
		ArmStop();
		resetEncoders();
		while(nMotorEncoder[RightBack2] <	 260)//strafes right to line up with goal
		{
			strafeRight();

		}
		halt();
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 360)//moves forward to go under tough
		{
			forward();

		}
		resetEncoders();
		while(nMotorEncoder[RightBack2] < 500)//continues out of trough to raise arm till it reaches goal
		{
			forward();
			UpArm(127);

		}
		halt();
		sustain();//keeps arm at level
		outake();
		wait1Msec(2000);//outakes for 2 sec.
		StopIntake();
		resetEncoders();
		while(nMotorEncoder[RightBack2] > -360)//goes backwards
		{
			reverse();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -500)
		{
			turnRight();
		}
		halt();
		resetEncoders();
		while(nMotorEncoder[FrontLeft2] > -1000)
		{
			reverse();
		}
		halt();

		break;
	default:
		displayLCDCenteredString(0, "No valid choice");
		displayLCDCenteredString(1, "was made!");
		break;
	}

}

task usercontrol()
{
	bLCDBacklight = true;

	clearLCDLine(0);
	clearLCDLine(1);

	string batteryVoltage;
	string mainBattery;
	int driverspeed;
	displayBatteryVoltage();

	while (true)
	{

		if(vexRT[Btn5U]==1)
		{
			driverspeed = 1;
		}

		else if (vexRT[Btn5D] ==1)
		{
			driverspeed = 2;
		}


		if(vexRT[Btn5UXmtr2] == 1)
		{
			SensorValue[solenoid] = 0;
		}

		else if(vexRT[Btn5DXmtr2] == 1)
		{
			SensorValue[solenoid] = 1 ;
		}
		if(vexRT[Btn5UXmtr2] == 1)
		{
			SensorValue[solenoid2] = 0;
		}
		else if(vexRT[Btn5DXmtr2] == 1)
		{
			SensorValue[solenoid2] = 1 ;
		}
		if(vexRT[Btn8U] == 1)
		{
			SensorValue[Launcher] = 1;
		}
		else if(vexRT[Btn8D] ==1)
		{
			SensorValue[Launcher] = 0 ;
		}

		motor[Intake] = vexRT[Ch2Xmtr2];
		motor[Arm] = vexRT[Ch3Xmtr2];


		switch(driverspeed)
		{
		case 1:
			motor[RightFront1] = (vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1])/2;
			motor[RightFront2] = (vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1])/2;

			motor[RightBack1] = (vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1])/2;
			motor[RightBack2] = (vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1])/2;

			motor[FrontLeft1] = (vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1])/2;
			motor[FrontLeft2] = (vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1])/2;

			motor[LeftBack1] = (vexRT [Ch3] + vexRT[Ch4] - vexRT	[Ch1])/2;
			motor[LeftBack2] = (vexRT [Ch3] + vexRT[Ch4] - vexRT	[Ch1])/2;
			break;

		case 2:
			motor[RightFront1] = (vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1])*.75;
			motor[RightFront2] = (vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1])*.75;

			motor[RightBack1] = (vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1])*.75;
			motor[RightBack2] = (vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1])*.75;

			motor[FrontLeft1] = (vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1])*.75;
			motor[FrontLeft2] = (vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1])*.75;

			motor[LeftBack1] = (vexRT [Ch3] + vexRT[Ch4] - vexRT	[Ch1])*.75;
			motor[LeftBack2] = (vexRT [Ch3] + vexRT[Ch4] - vexRT	[Ch1])*.75;
			break;

		default:
			motor[RightFront1] = vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1];
			motor[RightFront2] = vexRT [Ch3] - vexRT [Ch4] - vexRT [Ch1];

			motor[RightBack1] = vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1];
			motor[RightBack2] = vexRT [Ch3] - vexRT [Ch4] + vexRT [Ch1];

			motor[FrontLeft1] = vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1];
			motor[FrontLeft2] = vexRT [Ch3]+ vexRT [Ch4] + vexRT [Ch1];

			motor[LeftBack1] = vexRT [Ch3] + vexRT[Ch4] - vexRT	 [Ch1];
			motor[LeftBack2] = vexRT [Ch3] + vexRT[Ch4] - vexRT	 [Ch1];
			break;
		}
	}
}
