package wallfollowing;

class Condition {

    public CondValue condition;

    int distance;

    Condition() {
        this.condition = CondValue.NONE;
    }

    void setCondition(int distance) {

        this.distance = distance;
        // 6 | 0 - 2
        if (this.distance >= 0 && this.distance <= WallFollowing.TARGET_WALL_DIST) {

            this.condition = CondValue.JUST_RIGHT;
        } // Buffer either way depending on last condition to prevent flapping quickly
        // 6 | 3 - 4
        else if (this.distance >= WallFollowing.TARGET_WALL_DIST + 1 && this.distance <= WallFollowing.TARGET_WALL_DIST + 2) {

            if (this.condition != CondValue.JUST_RIGHT) {

                this.condition = CondValue.WAY_TOO_FAR;
            } else {

                this.condition = CondValue.JUST_RIGHT;
            }
        } // 6 | 10 - ~
        else if (this.distance >= WallFollowing.TARGET_WALL_DIST + 3) {

            this.condition = CondValue.WAY_TOO_FAR;
        }
	//dprint(term,"setCondition distance=%d condition=%d\n",distance,this.condition);
    }
}
