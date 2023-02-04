#include <AccelStepper.h>

#define FULLSTEP 4
#define in1 5
#define in2 6
#define in3 7
#define in4 8


AccelStepper myStepper(FULLSTEP, in1 , in2 , in3 , in4 );

void setup() {
  Serial.begin(9600);

  //init stepper motor
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(100.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(move_distance);
}

void loop() {

    if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

    myStepper.run();

}

