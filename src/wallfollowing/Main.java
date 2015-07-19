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
public class Main {
    
        static SerialComm serial;  
        static WallFollowing wallFollowing;
        
	public static void main(String[] args) throws Exception {
            
                
                GUI.startGui();
                SerialComm.startSerial();
                
//                 wallFollowing = new WallFollowing();
//
//                
//                while (true ) {
//                    
//                    wallFollowing.follow();
//                    Thread.sleep(4000);
//
//                }
	}

    static void exit() {
        SerialComm.stopSerial();
        System.exit(0);
    }



}
