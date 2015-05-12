/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wallfollowing;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author dlhinkley
 */
public class ActionTest {
    
    public ActionTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }
    
    @Before
    public void setUp() {
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of setDistances method, of class Action.
     */
    @Test
    public void testSetDistances() {
        System.out.println("setDistances");
        int left = 0;
        int ahead = 0;
        int right = 0;
        Action instance = new Action();
        instance.setDistances(left, ahead, right);
        // TODO review the generated test code and remove the default call to fail.
        assertTrue(true);
    }

    /**
     * Test of driveSpeed method, of class Action.
     */
    @Test
    public void testDriveSpeed() {
        System.out.println("driveSpeed");
        int left = 20;
        int right = 21;
        Action instance = new Action();
        instance.driveSpeed(left, right);
        
        assertEquals(Robot.drive_speed_left, left);
        assertEquals(Robot.drive_speed_right, right);
    }
    /**
     * Test of rightJustRight method, of class Action.
     */
    @Test
    public void testRightJustRightAgainstWall() {
        System.out.println("testRightJustRightAgainstWall");
        
        Action instance = new Action();
        int left = 50;
        int ahead = 50;
        int right = 1;
        instance.setDistances(left, ahead, right);
        instance.rightJustRight();

        assertEquals(instance.prevActionCommand, ActionCommands.RIGHT_JUST_RIGHT );
        
        // Should drive away from wall
        assertTrue( Robot.drive_speed_right > instance.speed );
        assertTrue( Robot.drive_speed_left < instance.speed );
    }
    /**
     * Test of rightJustRight method, of class Action.
     */
    @Test
    public void testRightJustRight() {
        System.out.println("rightJustRight");
        
        Action instance = new Action();
        int left = 50;
        int ahead = 50;
        int right = 4;
        instance.setDistances(left, ahead, right);
        instance.rightJustRight();

        assertEquals(instance.prevActionCommand, ActionCommands.RIGHT_JUST_RIGHT );
        
        // Should drive away from wall
        assertTrue( Robot.drive_speed_right == instance.speed );
        assertTrue( Robot.drive_speed_left == instance.speed );
    }

    /**
     * Test of rightWayFar method, of class Action.
     */
    @Test
    public void testRightWayFar() {
        System.out.println("rightWayFar");
        Action instance = new Action();
        int left = 50;
        int ahead = 50;
        int right = 8;
        instance.setDistances(left, ahead, right);
        instance.rightWayFar();

        assertEquals(instance.prevActionCommand, ActionCommands.RIGHT_WAY_FAR );

        // Should drive to wall
        assertTrue( Robot.drive_speed_right < instance.speed );
        assertTrue( Robot.drive_speed_left > instance.speed );
    }
    
}
