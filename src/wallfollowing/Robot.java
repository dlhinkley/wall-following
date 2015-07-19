/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wallfollowing;

/**
 *
 * @author dlhinkley
 */
public class Robot {
    static int drive_speed_left;
    static int drive_speed_right;
    static int drive_goto_right;
    static int drive_goto_left;
    static int speed = 20;
    static final String DRIVE_SPEED_CMD = "drive_speed %d %d\n";
    static final String DRIVE_GOTO_CMD = "drive_goto %d %d\n";

    static void drive_speed(int left, int right) {
  
        System.out.println("Robot.drive_speed left=" + left + " right=" + right);

        drive_speed_left = left;
        drive_speed_right = right;
        
        sendCommand(DRIVE_SPEED_CMD, left, right);
    }

    static void drive_goto(int left, int right) {
        
        drive_goto_right = right;
        drive_goto_left = left;
        
        sendCommand(DRIVE_GOTO_CMD, left, right);
    }
    private static void sendCommand(String commandTemplate) {
      
        String command = String.format( commandTemplate );
        
        SerialComm.serial.sendCommand(command);
    }
    private static void sendCommand(String commandTemplate, int var1) {
      
        String command = String.format( commandTemplate, var1 );
        
        SerialComm.serial.sendCommand(command);
    }
    private static void sendCommand(String commandTemplate, int var1, int var2) {
      
        String command = String.format( commandTemplate, var1, var2 );
        
        SerialComm.serial.sendCommand(command);
    }
    static void turretRight() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    static void turretLeft() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    static void turretForward() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    static void turnLeft() {
        drive_speed( -speed, speed);
    }

    static void turnRight() {
        drive_speed( speed, -speed);
    }

    static void driveBackward() {
        drive_speed( -speed, -speed);
    }

    static void driveForward() {
        drive_speed(speed, speed);
    }

    static void stop() {
        drive_speed(0, 0);
    }
}
