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

    static void drive_speed(int left, int right) {
  
        System.out.println("Robot.drive_speed left=" + left + " right=" + right);

        drive_speed_left = left;
        drive_speed_right = right;
    }

    static void drive_goto(int left, int right) {
        
        drive_goto_right = right;
        drive_goto_left = left;
    }
}
