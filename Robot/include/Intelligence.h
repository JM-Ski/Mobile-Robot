// Signatures
#include <Matrix2D.h>

namespace Condition
{
	enum Condition_ID
	{
		Avoid,
		Lock_Rotation,
		Turning,
		Follow_Left,
		Follow_Right,
		Num
	};
}

namespace Sonar
{
	enum Sonar_ID
	{
		Left_Sonar,
		Right_Sonar,
		Front_Sonar,
		Num
	};
}

namespace Edge_Follow
{
	enum Follow_ID
	{
		Error,
		Last_Error,
		Last_Reading,
		Proportional,
		Integral,
		Derivative,
		P_Gain,
		I_Gain,
		D_Gain,
		RMSE,
		Num
	};
}

namespace Wander
{
	enum Wander_ID
	{
		Distance_Travelled,
		X_Pos,
		Y_Pos,
		Last_X_Pos,
		Last_Y_Pos,
		Rand_Dist,
		Rand_Angle,
		Rand_Turn,
		Required_Heading,
		Num
	};
}

class Intelligence : public ArAction // Class action inherits from ArAction
{
 public:
   Intelligence(); // Constructor
   virtual ~Intelligence() {}  // Destructor
   virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
   ArActionDesired desiredState; // Holds state of the robot that we wish to action
  
 protected:
   void updatePID(double d_Reading, double &output, bool& b_Condition);
   void wander();
   Vector2D get_Coordinates(float f_Distance_In, float f_Angle_In, Vector2D v2_Position_In, float f_RoboAngle_In, bool b_RobotCoord = false);

   int i_Speed; // Speed of the robot in mm/s
   double d_DeltaHeading; // Change in heading
   double d_Output;
   short i_CurrentState;
   float f_NearObject;

   // Reading
   bool b_Condition[Condition::Num];
   double d_Sonar[Sonar::Num];
   double d_EdgeFollow[Edge_Follow::Num];
   double d_Wander[Wander::Num];
   std::string state;

   //Mapping
   float f_Distance;
   float f_Angle;;
   ArSensorReading* reading;
   Vector2D v2_Cords;
   int i_FileNum;
   std::vector<Vector2D> v_ObstacleCoordinates;
   std::vector<Vector2D> v_RobotCoordinates;

};
