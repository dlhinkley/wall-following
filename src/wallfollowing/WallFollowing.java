package wallfollowing;


public class WallFollowing {
	
	private Condition leftCond = new Condition();
	private Condition aheadCond = new Condition();
	private Condition rightCond = new Condition();
 	Action action = new Action();

        
	void follow() {
		
               
		leftCond.setCondition( Status.left );
		aheadCond.setCondition( Status.ahead );
		rightCond.setCondition( Status.right );
                
		action.setDistances(leftCond.distance, aheadCond.distance, rightCond.distance);

/*
		if ( leftCond.condition == Condition::WAY_TOO_CLOSE ) {

			if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
			}

			else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
			}

			else if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.leftWayClose();
			}
		}
		else if ( leftCond.condition == Condition::JUST_RIGHT ) {

			if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
			}

			else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
			}

			else if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.right90();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.rightWayFar(  );
			}
		}
		 if ( leftCond.condition == Condition::WAY_TOO_FAR ) {

			if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.backward();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backward();
			}

			else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

				if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
				if ( rightCond.condition == Condition::JUST_RIGHT    ) action.left90();
				if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.rightWayFar(  );
			}


*/
//			 if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {
//
				if ( rightCond.condition == CondValue.JUST_RIGHT    ) action.rightJustRight();
				if ( rightCond.condition == CondValue.WAY_TOO_FAR   ) action.rightWayFar( );
//			}

//		}


//		dprint(term, "x=%f y=%f heading=%f right=%d ahead=%d left=%d rightCond=%d aheadCond=%d leftCond=%d prevWallOnRight=%d wallOnRight=%d speedLeft=%d speedRight=%d\n",
//				      action.loc.x, action.loc.y, action.loc.heading, right,   ahead,   left,   (int)rightCond.condition,   (int)aheadCond.condition,   (int)leftCond.condition,   action.prevWallOnRight,   action.wallOnRight,   action.speedLeft,   action.speedRight);

		//TODO pause(250);

	//printf("Angle=%d Distance=%d\n", scanAngle[scanPtr], scanPing[scanPtr]);

	}

}	
