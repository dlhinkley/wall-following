
class Condition {

	public:
		enum CondValue { WAY_TOO_CLOSE = 0, JUST_RIGHT, WAY_TOO_FAR, NONE };
		CondValue condition;
		Condition();
		void setCondition(int);
		int distance;
};
Condition::Condition() {
	this->condition = NONE;
}

void Condition::setCondition(int distance) {

	this->distance = distance;
	// 6 | 0 - 2
	if ( this->distance  >= 0 && this->distance  <= TARGET_WALL_DIST  ) {

		this->condition  = this->JUST_RIGHT;
	}
	// Buffer either way depending on last condition to prevent flapping quickly
	// 6 | 3 - 4
	else if ( this->distance  >= TARGET_WALL_DIST + 1  && this->distance  <= TARGET_WALL_DIST + 2 ) {

		if ( this->condition != this->JUST_RIGHT ) {

			this->condition  = this->WAY_TOO_FAR;
		}
		else {

			this->condition = this->JUST_RIGHT;
		}
	}

	// 6 | 10 - ~
	else if ( this->distance  >= TARGET_WALL_DIST + 3  ) {

		this->condition  = this->WAY_TOO_FAR;
	}
	//dprint(term,"setCondition distance=%d condition=%d\n",distance,this->condition);
}