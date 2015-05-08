package wallfollowing;

class Action {

    int prevSpeedRight;
    int prevSpeedLeft;
    int speed;
    int speedMax;
    int speedSlow;


    int speedLeft;
    int speedRight;
    int left;
    int right;
    int ahead;
    int correct;

    ActionCommands prevCmd;

    Action() {

        this.speedLeft = 0;
        this.speedRight = 0;
        this.prevSpeedRight = 0;
        this.prevSpeedLeft = 0;
        this.correct = 4;
        this.speed = 20;
        this.speedMax = 40;
        this.speedSlow = 5;
        this.prevCmd = ActionCommands.INIT;
    }

    /**
     * Save the distances since they may change while correcting
     */
    void setDistances(int left, int ahead, int right) {

        this.left = left;
        this.ahead = ahead;
        this.right = right;
    }

    void goLeft() {
        driveSpeed(-this.speed, this.speed);
    }

    void goStop() {
        driveSpeed(0, 0);
    }

    void goRight() {
        driveSpeed(this.speed, -this.speed);
    }

    void goBackward() {
        driveSpeed(-this.speed, -this.speed);
    }

    void goForward() {
        driveSpeed(this.speed, this.speed);
    }

    void driveGoto(int left, int right) {

	//if ( left != prevSpeedLeft || right != prevSpeedRight ) {
        Robot.drive_goto(left, right);
	//	prevSpeedLeft = left;
        //	prevSpeedRight = right;

	//dprint(term,"DEBUG:new driveGoto left=%d right=%d\n", left, right);
        //}
        //loc.update();
    }

    void driveSpeed(int left, int right) {

        if (left != this.prevSpeedLeft || right != this.prevSpeedRight) {

            Robot.drive_speed(left, right);
            this.prevSpeedLeft = left;
            this.prevSpeedRight = right;

            //dprint(term,"DEBUG:new driveSpeed left=%d right=%d\n", left, right);
        }
	//loc.update();
    }

    void trapped() {

        // stop
        this.speedRight = 0;
        this.speedLeft = 0;
    }

    void right90() {

        driveGoto(26, -25);
    }

    void left90() {

        driveGoto(-25, 26);
    }

    void backUp90Right() {

	// backup until ahead is just right
        // turn 90 to right
        // drive forward
    }

    void backward() {

        driveSpeed(-this.speed, -this.speed);
    }

    void forward() {

        driveSpeed(this.speed, this.speed);
    }

    void turnRightCorner() {

        // Go straight until past corner
        int dist = (int) (0.325 * (Condition.TARGET_WALL_DIST + 50));

        driveGoto(dist, dist);

        // Turn right 90 degrees
        right90();

        // Go straight to go past edge
        driveGoto(50, 50);

        // Keep going straigt
        forward();

    }

    void leftJustRight() {

	// correct based on dynamic adjustment right or left of right
        // 15 - 14 = 1 too close so left should go faster
        // 15 - 16 = -1 too far so left should go slower
        int adj = (Condition.TARGET_WALL_DIST - this.left);

        driveSpeed(this.speed + adj, this.speed);
    }

    void leftWayClose() {

        driveSpeed(this.speed + this.correct, this.speed);
    }

    void rightJustRight() {

	// correct based on dynamic adjustment right or left of right
        // 15 - 14 = 1 too close so left should go faster
        // 15 - 16 = -1 too far so left should go slower
//	if ( this.prevCmd == this.RIGHT_WAY_CLOSE ) {
//
//		driveSpeed(this.speed + adj, this.speed - adj);
//	}
//	else {
//	}
        int adj = (Condition.TARGET_WALL_DIST - this.right) * 2;
        driveSpeed(this.speed - adj, this.speed + adj);

        this.prevCmd = ActionCommands.RIGHT_JUST_RIGHT;
    }

    void rightWayClose() {

//	if ( this.prevCmd == this.RIGHT_WAY_CLOSE ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this.correct, this.correct); // 10 degrees
//	}
        int adj = (Condition.TARGET_WALL_DIST - this.right);
        driveSpeed(this.speed - adj, this.speed);

        this.prevCmd = ActionCommands.RIGHT_WAY_CLOSE;
    }

    void rightWayFar() {

        int adj = (Condition.TARGET_WALL_DIST - this.right) * 3;
        driveSpeed(this.speed - adj, this.speed + adj);

//	if ( this.prevCmd == this.RIGHT_WAY_FAR ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this.correct, this.correct); // 10 degrees
//	}
        this.prevCmd = ActionCommands.RIGHT_WAY_FAR;
    }

    void action() {
        // testing
    }
}
