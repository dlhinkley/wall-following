package wallfollowing;


public class WallFollowing {
	
	Status status = new Status();
	private Condition leftCond = new Condition();
	private Condition aheadCond = new Condition();
	private Condition rightCond = new Condition();
 	Action action = new Action();


        
	public void setStatus(String statusLine ) {
		
		status.setStatus(statusLine);
	}
	void follow() {
		
               
		leftCond.setCondition( status.left );
		aheadCond.setCondition( status.ahead );
		rightCond.setCondition( status.right );
                
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

	public static void main(String[] args) throws Exception {
            
                WallFollowing wallFollowing = new WallFollowing();
                
		SerialComm serial = new SerialComm(wallFollowing);
		serial.initialize();
		Thread t=new Thread() {
			public void run() {
				//the following line will keep this app alive for 1000 seconds,
				//waiting for events to occur and responding to them (printing incoming messages to console).
				try {Thread.sleep(1000000);} catch (InterruptedException ie) {}
			}
		};
		t.start();
                
                while (true ) {
                    
                    wallFollowing.follow();
                    Thread.sleep(4000);

                }
	}
}	
