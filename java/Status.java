package com.duanehinkley;

public class Status {
	
	public int right;
	public int ahead;
	public int left;
	public int speedLeft;
	public int speedRight;
	public int x;
	public int y;
	public int heading;
	
	public void setStatus( String statusLine ) {
		

		
		System.out.println("Status.setStatus statusLine=" + statusLine);

		String[] statusList = statusLine.split("\t");
		
		
		this.right = Integer.parseInt( statusList[0] );
		this.ahead = Integer.parseInt( statusList[1] );
		this.left = Integer.parseInt( statusList[2] );
		this.speedLeft = Integer.parseInt( statusList[3] );
		this.speedRight = Integer.parseInt( statusList[4] );
		this.x = Integer.parseInt( statusList[5] );
		this.y = Integer.parseInt( statusList[6] );
		this.heading = Integer.parseInt( statusList[7] );
		
		System.out.println("Status.setStatus right=" + right);
		System.out.println("Status.setStatus heading=" + heading);

	}
	
}