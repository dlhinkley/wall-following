
class Action {
	int prevSpeedRight;
	int prevSpeedLeft;
	int speed;
	int speedMax;
	int speedSlow;
	enum ActionCommands {RIGHT_WAY_CLOSE = 0, RIGHT_JUST_RIGHT, RIGHT_WAY_FAR, INIT };

	void turnRightCorner(  );
	void driveSpeed( int, int);
	void driveGoto( int, int);

public:
	Action();
	void trapped();
	void right90();
	void left90();
	void backward();
	void forward();
	void backUp90Right();
	void rightJustRight();
	void leftJustRight();
	void leftWayClose();
	void rightWayFar();
	void rightWayClose();
	void action();
	void drive();
	void goLeft();
	void goStop();
	void goRight();
	void goBackward();
	void goForward();
	void setDistances(int,int,int);
	int speedLeft;
	int speedRight;
	int left;
	int right;
	int ahead;
	int correct;

	Location loc;
	ActionCommands prevCmd;

};
Action::Action() {

	this->speedLeft = 0;
	this->speedRight = 0;
	this->prevSpeedRight = 0;
	this->prevSpeedLeft = 0;
	this->correct = 4;
	this->speed = 20;
	this->speedMax = 40;
	this->speedSlow = 5;
	this->prevCmd = INIT;
}
/**
 * Save the distances since they may change while correcting
 */
void Action::setDistances(int left, int ahead, int right) {

	this->left = left;
	this->ahead = ahead;
	this->right = right;
}
void Action::goLeft() {
	driveSpeed(-this->speed, this->speed);
}
void Action::goStop() {
	driveSpeed(0, 0);
}
void Action::goRight() {
	driveSpeed(this->speed, -this->speed);
}
void Action::goBackward() {
	driveSpeed(-this->speed, -this->speed);
}
void Action::goForward() {
	driveSpeed(this->speed, this->speed);
}
void Action::driveGoto(int left,int right) {

	//if ( left != prevSpeedLeft || right != prevSpeedRight ) {

		drive_goto(left, right);
	//	prevSpeedLeft = left;
	//	prevSpeedRight = right;

	//dprint(term,"DEBUG:new driveGoto left=%d right=%d\n", left, right);
	//}
	//loc.update();
}
void Action::driveSpeed(int left,int right) {

	if ( left != this->prevSpeedLeft || right != this->prevSpeedRight ) {

		drive_speed(left, right);
		this->prevSpeedLeft = left;
		this->prevSpeedRight = right;

		//dprint(term,"DEBUG:new driveSpeed left=%d right=%d\n", left, right);
	}
	//loc.update();
}
void Action::trapped() {

	// stop
	this->speedRight = 0;
	this->speedLeft  = 0;
}
void Action::right90() {

	driveGoto(26, -25);
}
void Action::left90() {

	driveGoto(-25, 26);
}
void Action::backUp90Right() {

	// backup until ahead is just right
	// turn 90 to right
	// drive forward

}
void Action::backward() {

	driveSpeed(-this->speed, -this->speed);
}
void Action::forward() {

	driveSpeed(this->speed, this->speed);
}

void Action::turnRightCorner(  ) {

	// Go straight until past corner
	int dist = 0.325 * TARGET_WALL_DIST + 50;

	driveGoto(dist,dist);

	// Turn right 90 degrees
	right90();

	// Go straight to go past edge
	driveGoto( 50, 50);

	// Keep going straigt
	forward();

}

void Action::leftJustRight() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower
	int adj = (TARGET_WALL_DIST - this->left) ;

	driveSpeed(this->speed + adj, this->speed);
}
void Action::leftWayClose() {

	driveSpeed(this->speed + this->correct,this-> speed);
}


void Action::rightJustRight() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower

//	if ( this->prevCmd == this->RIGHT_WAY_CLOSE ) {
//
//		driveSpeed(this->speed + adj, this->speed - adj);
//	}
//	else {

//	}
	int adj = (TARGET_WALL_DIST - this->right) * 2;
	driveSpeed(this->speed - adj, this->speed + adj);

	this->prevCmd = this->RIGHT_JUST_RIGHT;
}

void Action::rightWayClose() {

//	if ( this->prevCmd == this->RIGHT_WAY_CLOSE ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this->correct, this->correct); // 10 degrees
//	}
	int adj = (TARGET_WALL_DIST - this->right) ;
	driveSpeed(this->speed - adj, this->speed);

	this->prevCmd = this->RIGHT_WAY_CLOSE;
}

void Action::rightWayFar( ) {

	int adj = (TARGET_WALL_DIST - this->right) * 3;
	driveSpeed(this->speed - adj, this->speed + adj);

//	if ( this->prevCmd == this->RIGHT_WAY_FAR ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this->correct, this->correct); // 10 degrees
//	}
	this->prevCmd = this->RIGHT_WAY_FAR;
}
void Action::action() {

}

