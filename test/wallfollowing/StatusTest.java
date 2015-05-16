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
public class StatusTest {
    
    public StatusTest() {
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
     * Test of setStatus method, of class Status.
     */
    @Test
    public void testSetStatus() {
        System.out.println("setStatus");
        int right = 0, right45 = 0, ahead = 0, left45 = 0, left = 0, speedLeft = 0, speedRight = 0, x = 0, y = 0, heading = 0;
        
        String statusLine = String.format( "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				      right,  right45, ahead, left45,  left,   speedLeft,   speedRight, x, y, heading);

        Status.setStatus(statusLine);
        
        assertEquals( (int)Status.right, right );
        assertEquals( (int)Status.right45, right45 );
        assertEquals( (int)Status.ahead, ahead );
        assertEquals( (int)Status.left45, left45 );
        assertEquals( (int)Status.left, left );
        assertEquals( (int)Status.speedLeft, speedLeft );
        assertEquals( (int)Status.speedRight, speedRight );
        assertEquals( (int)Status.x, x );
        assertEquals( (int)Status.y, y );
        assertEquals( (int)Status.heading, heading );
        
    }
    /**
     * Test of setStatus method, of class Status.
     */
    @Test
    public void testSetStatusNulls() {
        System.out.println("setStatus");
        Integer right = null, right45 = null, ahead = null, left45 = null, left = null, speedLeft = null, speedRight = null, x = null, y = null, heading = null;
        
        String statusLine = String.format( "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				      right,  right45,  ahead,  left45, left,   speedLeft,   speedRight, x, y, heading);
                         
        boolean isException = false;
        
        try {
        Status.setStatus(statusLine);
        
        }
        catch (NumberFormatException e ) {
            
            isException = true;
        }
        assertTrue(isException);
        
    }
     
}
