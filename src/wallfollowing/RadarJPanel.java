/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wallfollowing;

import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JPanel;

/**
 *
 * @author dlhinkley
 */
public class RadarJPanel extends JPanel {
        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);

            int x =10;
            int y = 20;
            int width = 130;
            int height = 110;
            
            g.setColor(Color.white);
            //rectangle originated at 10,10 and end at 240,240
            g.drawRect(x, y, width, height);
                    //filled Rectangle with rounded corners.    
            g.fillRoundRect(x, y, width, height, 8, 8);
            
            // Draw origin dot
            //
            int originX = (width + x) / 2;
            int originY = (height + y) - 10;
            
            g.setColor( Color.red);
            g.fillOval(originX, originY, 10, 10);
            


            this.drawCircle(g, originX, originY, Status.aheadIr, 0, Color.blue);
            this.drawCircle(g, originX, originY, Status.ahead, 0, Color.orange);
            this.drawCircle(g, originX, originY, Status.right45, 45, Color.orange);
            this.drawCircle(g, originX, originY, Status.right, 90, Color.orange);
            this.drawCircle(g, originX, originY, Status.left, 270, Color.orange);
            this.drawCircle(g, originX, originY, Status.left45, 315, Color.orange);

         
        }
        private void drawCircle(Graphics g, int originX, int originY, int distance, int degrees, Color color) {
            
                        // Draw left45 dot
            double angle = degrees * Math.PI / 180;
             
            int x = (int) (originX - (distance * Math.sin(angle)));
            int y = (int) (originY - (distance * Math.cos(angle)));
            
            g.setColor( color);
            g.fillOval(x, y, 5, 5);
        }

}
