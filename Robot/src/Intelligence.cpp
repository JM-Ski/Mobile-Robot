#include <iostream>
#include <stdlib.h>
#include <Aria.h>
#include <deque>
#include "Intelligence.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
// Implementation

#define AVOID_OBSTACLE 650
#define WANDER_LIMIT 1500
#define DETECT_WALL 1000
#define FOLLOW_WALL 700
#define MAX_SPEED 250
#define WANDER_SPEED 200

// Constructor
Intelligence::Intelligence() : ArAction("Follow Edge")
{

	i_Speed = WANDER_SPEED; // Set the robots speed to 50 mm/s. 200 is top speed
	state = "";
	d_DeltaHeading = 0; // Straight line
	
	b_Condition[0] = false;
	d_Sonar[0] = 0;

	d_EdgeFollow[0] = 0;	
	d_EdgeFollow[Edge_Follow::P_Gain] = 0.02f;
	d_EdgeFollow[Edge_Follow::I_Gain] = 0.00f;
	d_EdgeFollow[Edge_Follow::D_Gain] = 1.f;
	
	d_Wander[Wander::X_Pos] = 0;
	d_Wander[Wander::Y_Pos] = 0;
	d_Wander[Wander::Last_X_Pos] = 0;
	d_Wander[Wander::Last_Y_Pos] = 0;
	d_Wander[Wander::Rand_Angle] = 0;
	d_Wander[Wander::Required_Heading] = 0;
	d_Wander[Wander::Distance_Travelled] = 0;
	d_Wander[Wander::Rand_Dist] = rand() % (1500 - 500 + 1) + 500;

	i_FileNum = 0;
	f_Distance = 0;
	f_Angle = 0;
}

// Body of action
ArActionDesired * Intelligence::fire(ArActionDesired d)
{
	desiredState.reset(); // reset the desired state (must be done)

	// Get sonar readings
	d_Sonar[Sonar::Left_Sonar] = myRobot->getClosestSonarRange(-20, 100);
	d_Sonar[Sonar::Right_Sonar] = myRobot->getClosestSonarRange(-100, 20);
	d_Sonar[Sonar::Front_Sonar] = myRobot->getClosestSonarRange(-20, 20);

	//Check for the nearest object
	if (d_Sonar[Sonar::Left_Sonar] < 5000 && d_Sonar[Sonar::Left_Sonar] < d_Sonar[Sonar::Right_Sonar] && d_Sonar[Sonar::Left_Sonar] < d_Sonar[Sonar::Front_Sonar]) f_NearObject = d_Sonar[Sonar::Left_Sonar];
	if (d_Sonar[Sonar::Right_Sonar] < 5000 && d_Sonar[Sonar::Right_Sonar] < d_Sonar[Sonar::Left_Sonar] && d_Sonar[Sonar::Right_Sonar] < d_Sonar[Sonar::Front_Sonar]) f_NearObject = d_Sonar[Sonar::Right_Sonar];
	if (d_Sonar[Sonar::Front_Sonar] < 5000 && d_Sonar[Sonar::Front_Sonar] < d_Sonar[Sonar::Right_Sonar] && d_Sonar[Sonar::Front_Sonar] < d_Sonar[Sonar::Left_Sonar]) f_NearObject = d_Sonar[Sonar::Front_Sonar];

	//Display information
	std::cout << "---------------------------------------" << "\n";
	std::cout << "State: " << state << "\n";
	if (d_Sonar[Sonar::Left_Sonar] >= 5000 && d_Sonar[Sonar::Right_Sonar] >= 5000 && d_Sonar[Sonar::Front_Sonar] >= 5000) std::cout << "Distance to nearest object: None" << "\n";
	else std::cout << "Distance to nearest object: " << f_NearObject << "\n";
	std::cout << "Robot speed: " << i_Speed << "\n";
	std::cout << "Robot heading: " << myRobot->getTh() << "\n";
	std::cout << "Robot position: (X: " << myRobot->getX() << ", Y: " << myRobot->getY() << ")" << "\n";
	std::cout << "---------------------------" << "\n";
	std::cout << "Any extra information about state..." << "\n";

	if (!b_Condition[Condition::Avoid]) d_Output = 0; // Reset output if no longer avoiding

	//Follow left edge if left sonar is the closest and we are not avoiding anything
	if (d_Sonar[Sonar::Left_Sonar] < DETECT_WALL && d_Sonar[Sonar::Left_Sonar] < d_Sonar[Sonar::Right_Sonar] && !b_Condition[Condition::Avoid])
	{
		 i_Speed = MAX_SPEED;
		 state = "Following left edge...";
		 updatePID(d_Sonar[Sonar::Left_Sonar], d_Output, b_Condition[Condition::Follow_Left]);
		 d_Output *= -1; // Note that we have to negate PID
	}
	else
	{
		 b_Condition[Condition::Follow_Left] = false; // We can't follow the edge
	}
	
	//Follow right edge if right sonar is the closest and we are not avoiding anything
	if (d_Sonar[Sonar::Right_Sonar] < DETECT_WALL && d_Sonar[Sonar::Right_Sonar] < d_Sonar[Sonar::Left_Sonar] && !b_Condition[Condition::Avoid])
	{
		 i_Speed = MAX_SPEED;
		 d_Output = 0;
		 state = "Following right edge...";
		 updatePID(d_Sonar[Sonar::Right_Sonar], d_Output, b_Condition[Condition::Follow_Right]);
	}
	else
	{
		 b_Condition[Condition::Follow_Right] = false;  // We can't follow the edge
	} 
	
	//If nothing is detected then we will wonder
	if (!b_Condition[Condition::Avoid] && !b_Condition[Condition::Follow_Right] && !b_Condition[Condition::Follow_Left] && 
		 d_Sonar[Sonar::Front_Sonar] > WANDER_LIMIT && d_Sonar[Sonar::Left_Sonar] > WANDER_LIMIT && d_Sonar[Sonar::Right_Sonar] > WANDER_LIMIT)
	{
		 state = "Wandering...";
		 wander();
	}
	else if (!b_Condition[Condition::Follow_Right] && !b_Condition[Condition::Follow_Left]) // Else we won't do anything if something is detected within 1.5m
	{
		state = "Not doing anything...";
		std::cout << "Robot detected something within 1.5m" << "\n";
	}

	//If the front sonar detected an obstacle
	if (d_Sonar[Sonar::Front_Sonar] < AVOID_OBSTACLE)
	{
		 state = "Avoiding...";
		 i_Speed = 0;
		 b_Condition[Condition::Avoid] = true;
		 double d_Angle;
		 myRobot->checkRangeDevicesCurrentPolar(-10, 10, &d_Angle); // Check which angle of turn to take

		 if (!b_Condition[Condition::Lock_Rotation])
		 {
			 (d_Angle <= 0) ? d_Output = 10 : d_Output = -10;
			 b_Condition[Condition::Lock_Rotation] = true; // Make sure to lock it so that robot will not try to decide the angle again
		 }
	}
	else // Ignore
	{
		 b_Condition[Condition::Avoid] = false;
		 b_Condition[Condition::Lock_Rotation] = false;
	}

	//We need positions that are relevant now
	float f_RobotAngle = myRobot->getTh();
	Vector2D v2_CurrentRobotPos = Vector2D(myRobot->getX(), myRobot->getY());

	//Reset variables
	f_Distance = 0;
	f_Angle = 0;

	//Check all 16 sonars
	for (int i = 0; i < 16; i++)
	{
		//Get the reading
		reading = myRobot->getSonarReading(i);

		//Extract information
		f_Distance = reading->getRange();
		f_Angle = reading->getSensorTh();

		//Calculate the coordinates
		v2_Cords = get_Coordinates(f_Distance, f_RobotAngle, v2_CurrentRobotPos, f_Angle);

		if (v2_Cords.x != -99999999999 && v2_Cords.y != -99999999999) // If coordinates are valid
		{
			std::cout << "---------------------------------------" << "\n";
			std::cout << "Sonar at: " << reading->getSensorTh() << " degrees:" << "\n"; 
			std::cout << "Adding coordinates, (X: " << v2_Cords.x << ", Y: " << v2_Cords.y << ")" << "\n";
			v_ObstacleCoordinates.push_back(v2_Cords);
		}
	}

	v2_Cords = get_Coordinates(0, f_RobotAngle, v2_CurrentRobotPos, f_Angle, true);
	v_RobotCoordinates.push_back(v2_Cords);

	//Id we manage to get more than 100 coordinates then save this to file!
	if (v_ObstacleCoordinates.size() > 100 && v_RobotCoordinates.size() > 0)
	{
		std::cout << "Sending coordinates to file..." << "\n";
		
		std::string path = "data/file" + std::to_string(i_FileNum) + ".csv"; //The path
		std::ofstream outfile(path); // Open the file

		//Write categories
		outfile << "X, Y" << "\n";
		outfile << "o" << "\n";
		//Write all coordinate data to a file
		for (int i = 0; i < v_ObstacleCoordinates.size(); i++) outfile << std::to_string(v_ObstacleCoordinates.at(i).x) << ", " << std::to_string(v_ObstacleCoordinates.at(i).y) << "\n";

		outfile << "r" << "\n";
		for (int i = 0; i < v_RobotCoordinates.size(); i++) outfile << std::to_string(v_RobotCoordinates.at(i).x) << ", " << std::to_string(v_RobotCoordinates.at(i).y) << "\n";


		outfile.close(); // Close the file
		i_FileNum++; // Make sure to increase the file ID

	    // Get rid of the saved coordinates
		v_ObstacleCoordinates.clear();
		v_RobotCoordinates.clear();
	}

	d_DeltaHeading = d_Output; // Implement control action
	desiredState.setVel(i_Speed); // set the speed of the robot in the desired state
	desiredState.setDeltaHeading(d_DeltaHeading); // Set the heading change of the robot

	return &desiredState; // give the desired state to the robot for actioning
}

void Intelligence::updatePID(double d_Reading, double &output, bool& b_Condition)
{
	b_Condition = true;
	d_EdgeFollow[Edge_Follow::Error] = FOLLOW_WALL - d_Reading; // Calculate an error
	d_EdgeFollow[Edge_Follow::Proportional] = d_EdgeFollow[Edge_Follow::P_Gain] * d_EdgeFollow[Edge_Follow::Error]; // Calculate proportional
	d_EdgeFollow[Edge_Follow::Last_Error] += d_EdgeFollow[Edge_Follow::Error]; // Calculate the last error
	d_EdgeFollow[Edge_Follow::Integral] = d_EdgeFollow[Edge_Follow::Last_Error] * d_EdgeFollow[Edge_Follow::I_Gain]; // Calculate integral
	d_EdgeFollow[Edge_Follow::Derivative] = (d_Reading - d_EdgeFollow[Edge_Follow::Last_Reading]) * d_EdgeFollow[Edge_Follow::D_Gain]; // Calculate derivative
	d_EdgeFollow[Edge_Follow::Last_Reading] = d_Reading; // Save current reading
	output = d_EdgeFollow[Edge_Follow::Proportional] + d_EdgeFollow[Edge_Follow::Integral] - d_EdgeFollow[Edge_Follow::Derivative]; // Calculate PID

	d_EdgeFollow[Edge_Follow::RMSE] = sqrt(pow(d_Reading - FOLLOW_WALL, 2)); // RMSE

	std::cout << "Distance to a wall: " << d_Reading << "\n";
}

Vector2D Intelligence::get_Coordinates(float f_Distance_In, float f_Angle_In, Vector2D v2_Position_In, float f_RoboAngle_In, bool b_RobotCoord)
{
	if (f_Distance_In < 5000 && !b_RobotCoord) // If reading is valid
	{

		//Rotate around robot's local space
		double xResult;
		double yResult;
		xResult = cosf(Deg2Rad(f_Angle_In)) * (((f_Distance_In + myRobot->getRobotRadius()) / 1000.f));
		yResult = sinf(Deg2Rad(f_Angle_In)) * (((f_Distance_In + myRobot->getRobotRadius()) / 1000.f));

		//Setup transformation matrices
		Matrix2D rotMat, tranMat;
		rotMat.setRotMat(f_RoboAngle_In);
		tranMat.setTransMat(v2_Position_In.x / 1000.f, v2_Position_In.y / 1000.f);

		//Rotate to global space
		double xRotResult;
		double yRotResult;
		xRotResult = (xResult * rotMat.getRow(0).x) + (yResult * rotMat.getRow(0).y);
		yRotResult = (xResult * rotMat.getRow(1).x) + (yResult * rotMat.getRow(1).y);

		//Translate to global space
		double xTranResult;
		double yTranResult;
		xTranResult = xRotResult + tranMat.getRow(0).x;
		yTranResult = yRotResult + tranMat.getRow(0).y;

		//Final result
		v2_Cords = Vector2D(xTranResult, yTranResult);
		return v2_Cords;

	}
	else if (b_RobotCoord)
	{
		//Rotate around robot's local space
		double xResult;
		double yResult;
		xResult = cosf(Deg2Rad(f_Angle_In)) * (((f_Distance_In + myRobot->getRobotRadius()) / 1000.f));
		yResult = sinf(Deg2Rad(f_Angle_In)) * (((f_Distance_In + myRobot->getRobotRadius()) / 1000.f));

		//Setup transformation matrices
		Matrix2D rotMat, tranMat;
		rotMat.setRotMat(f_RoboAngle_In);
		tranMat.setTransMat(v2_Position_In.x / 1000.f, v2_Position_In.y / 1000.f);

		//Rotate to global space
		double xRotResult;
		double yRotResult;
		xRotResult = (xResult * rotMat.getRow(0).x) + (yResult * rotMat.getRow(0).y);
		yRotResult = (xResult * rotMat.getRow(1).x) + (yResult * rotMat.getRow(1).y);

		//Translate to global space
		double xTranResult;
		double yTranResult;
		xTranResult = xRotResult + tranMat.getRow(0).x;
		yTranResult = yRotResult + tranMat.getRow(0).y;

		//Final result
		v2_Cords = Vector2D(xTranResult, yTranResult);
		return v2_Cords;
	}
	else // Coordinates are not valid
	{
		v2_Cords = Vector2D(-99999999999, -99999999999);
		return v2_Cords;
	}

}

void Intelligence::wander()
{
	i_Speed = WANDER_SPEED;
	d_Wander[Wander::X_Pos] = myRobot->getX();
	d_Wander[Wander::Y_Pos] = myRobot->getY();
	d_Wander[Wander::Distance_Travelled] += abs(sqrtf(pow(d_Wander[Wander::X_Pos], 2) + pow(d_Wander[Wander::Y_Pos], 2)) - sqrtf(pow(d_Wander[Wander::Last_X_Pos], 2) + pow(d_Wander[Wander::Last_Y_Pos], 2)));

	if (d_Wander[Wander::Distance_Travelled] > d_Wander[Wander::Rand_Dist] && !b_Condition[Condition::Turning])
	{
		d_Wander[Wander::Rand_Turn] = rand() % 2;
		d_Wander[Wander::Rand_Angle] = rand() % 141;
		b_Condition[Condition::Turning] = true;
	}
	else if (b_Condition[Condition::Turning] && d_Wander[Wander::Required_Heading] > d_Wander[Wander::Rand_Angle])
	{
		d_DeltaHeading = 0;
		b_Condition[Condition::Turning] = false;
		d_Wander[Wander::Rand_Dist] = rand() % (1500 - 500 + 1) + 500;
		d_Wander[Wander::Distance_Travelled] = 0;
		d_Wander[Wander::Required_Heading] = 0;
		d_Wander[Wander::Rand_Angle] = 0;
	}
	else if (b_Condition[Condition::Turning])
	{
		(d_Wander[Wander::Rand_Turn] < 0) ? d_Output = 10 : d_Output = -10;
		d_Wander[Wander::Required_Heading] += 0.93;
	}

	d_Wander[Wander::Last_X_Pos] = d_Wander[Wander::X_Pos];
	d_Wander[Wander::Last_Y_Pos] = d_Wander[Wander::Y_Pos];

	std::cout << "Generated distance: " << d_Wander[Wander::Rand_Dist] << "\n";
	std::cout << "Generated angle: " << d_Wander[Wander::Rand_Angle] << "\n";
	std::cout << "Distance travelled: " << d_Wander[Wander::Distance_Travelled] << "\n";
	std::cout << "Required heading: " << d_Wander[Wander::Required_Heading] << "\n"; 

}
