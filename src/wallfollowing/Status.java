package wallfollowing;

import java.util.Arrays;

public class Status {
	
	public static Integer right;
	public static Integer right45;
	public static Integer ahead;
	public static Integer left;
	public static Integer left45;
	public static Integer speedLeft;
	public static Integer speedRight;
	public static Integer x;
	public static Integer y;
	public static Integer heading;
        public static Integer aheadIr;
        public static Integer turret;
	
	public static void setStatus( String statusLine ) {
		 

		
		System.out.println("Status.setStatus start statusLine=" + statusLine);

		String[] statusList = statusLine.trim().split("\t");
		
		System.out.println("Status.setStatus statusList=" + Arrays.asList(statusList));

                Integer pos = 0;
		Status.right = Integer.parseInt( statusList[pos++] );
		Status.right45 = Integer.parseInt( statusList[pos++] );
		Status.ahead = Integer.parseInt( statusList[pos++] );
		Status.aheadIr = Integer.parseInt( statusList[pos++] );
		Status.left45 = Integer.parseInt( statusList[pos++] );
		Status.left = Integer.parseInt( statusList[pos++] );
		Status.speedLeft = Integer.parseInt( statusList[pos++] );
		Status.speedRight = Integer.parseInt( statusList[pos++] );
		Status.x = Integer.parseInt( statusList[pos++] );
		Status.y = Integer.parseInt( statusList[pos++] );
		Status.heading = Integer.parseInt( statusList[pos++] );
		Status.turret = Integer.parseInt( statusList[pos++] );
		
		System.out.println("Status.setStatus status=" + right);
		System.out.println("Status.setStatus right=" + right);
		System.out.println("Status.setStatus heading=" + heading);
                
                GUI.gui.setStatus();

		System.out.println("Status.setStatus end");
	}
	
}