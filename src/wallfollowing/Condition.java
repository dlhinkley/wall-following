package wallfollowing;

class Condition {

    public CondValue condition = null;
    public static final int TARGET_WALL_DIST = 4;
    public static final int JUST_RIGHT_DIST = 4;
    public static final int JUST_RIGHT_MAX_DIST = 6;
    public static final int JUST_RIGHT_FLAPPING = 2;
    public static final int TOO_FAR_DIST = 8;
    int distance;

    Condition() {
    }

    void setCondition(int distance) {

        this.distance = distance;
        // 6 | 0 - 2
        if (this.distance >= 0 && this.distance <= JUST_RIGHT_DIST) {

            this.condition = CondValue.JUST_RIGHT;
        } // Buffer either way depending on last condition to prevent flapping quickly
        // 6 | 3 - 4
        else if (this.distance >= JUST_RIGHT_DIST + 1 && this.distance <= JUST_RIGHT_DIST + JUST_RIGHT_FLAPPING ) {

            // If it's previously way to far, keep it there
            if (this.condition != CondValue.JUST_RIGHT) {

                this.condition = CondValue.WAY_TOO_FAR;
            } else {

                this.condition = CondValue.JUST_RIGHT;
            }
        } // 6 | 10 - ~
        else if (this.distance >  JUST_RIGHT_DIST + JUST_RIGHT_FLAPPING ) {

            this.condition = CondValue.WAY_TOO_FAR;
        }
	//dprint(term,"setCondition distance=%d condition=%d\n",distance,this.condition);
    }
}
