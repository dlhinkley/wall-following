package wallfollowing;

public class Status {
	
	public int right;
	public int right45;
	public int ahead;
	public int left;
	public int left45;
	public int speedLeft;
	public int speedRight;
	public int x;
	public int y;
	public int heading;
	
	public void setStatus( String statusLine ) {
		

		
		System.out.println("Status.setStatus statusLine=" + statusLine);

		String[] statusList = statusLine.trim().split("\t");
		
		int pos = 0;
		this.right = Integer.parseInt( statusList[pos++] );
		this.right45 = Integer.parseInt( statusList[pos++] );
		this.ahead = Integer.parseInt( statusList[pos++] );
		this.left45 = Integer.parseInt( statusList[pos++] );
		this.left = Integer.parseInt( statusList[pos++] );
		this.speedLeft = Integer.parseInt( statusList[pos++] );
		this.speedRight = Integer.parseInt( statusList[pos++] );
		this.x = Integer.parseInt( statusList[pos++] );
		this.y = Integer.parseInt( statusList[pos++] );
		this.heading = Integer.parseInt( statusList[pos++] );
		
		System.out.println("Status.setStatus right=" + right);
		System.out.println("Status.setStatus heading=" + heading);

	}
	
}