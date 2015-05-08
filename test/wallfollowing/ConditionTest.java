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
public class ConditionTest {
    
    public ConditionTest() {
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
     * Test of setCondition method, of class Condition.
     */
    @Test
    public void testSetCondition() {
        System.out.println("setCondition");
        Condition instance = new Condition();
        
        
        instance.setCondition(0);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(1);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(2);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(3);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(4);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(5);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(6);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(7);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(8);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(9);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(8);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(7);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(6);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(5);
        assertEquals( instance.condition, CondValue.WAY_TOO_FAR);
        
        instance.setCondition(4);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(3);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(2);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(1);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        instance.setCondition(0);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
        
       instance.setCondition(5);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
        
       instance.setCondition(4);
        assertEquals( instance.condition, CondValue.JUST_RIGHT);
    }
    
}
