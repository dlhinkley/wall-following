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
public class WallFollowingTest {
    
    public WallFollowingTest() {
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
     * Test of setStatus method, of class WallFollowing.
     */
    @Test
    public void testSetStatus() {
        System.out.println("setStatus");
        
        int right = 0, right45 = 0, ahead = 0, left45 = 0, left = 0, speedLeft = 0, speedRight = 0, x = 0, y = 0, heading = 0;
        
        String statusLine = String.format( "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				      right, right45,  ahead,  left45, left,   speedLeft,   speedRight, x, y, heading);
        
        
        WallFollowing wallFollowing = new WallFollowing();
        wallFollowing.setStatus( statusLine );
        
        assertEquals(wallFollowing.status.right, right );
        assertEquals(wallFollowing.status.right45, right45 );
        assertEquals(wallFollowing.status.ahead, ahead );
        assertEquals(wallFollowing.status.left, left );
        assertEquals(wallFollowing.status.left45, left45 );
        assertEquals(wallFollowing.status.speedLeft, speedLeft );
        assertEquals(wallFollowing.status.speedRight, speedRight );
        assertEquals(wallFollowing.status.x, x );
        assertEquals(wallFollowing.status.y, y );
        assertEquals(wallFollowing.status.heading, heading );
    }

    /**
     * Test of follow method, of class WallFollowing.
     */
    @Test
    public void testFollow() {
        System.out.println("follow");
        
       int right = 0, right45 = 0, ahead = 0, left45 = 0, left = 0, speedLeft = 0, speedRight = 0, x = 0, y = 0, heading = 0;
        
        String statusLine = String.format( "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				      right, right45,  ahead,  left45, left,   speedLeft,   speedRight, x, y, heading);
         
        WallFollowing wallFollowing = new WallFollowing();
        wallFollowing.setStatus( statusLine );
 
        wallFollowing.follow();
        
        assertEquals(wallFollowing.action.prevActionCommand, ActionCommands.RIGHT_JUST_RIGHT);

    }
    
}
