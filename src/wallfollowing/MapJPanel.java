/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wallfollowing;

import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;
import java.util.Iterator;
import javax.swing.JPanel;

/**
 *
 * @author dlhinkley
 */
public class MapJPanel extends JPanel {
    
        private ArrayList<Coord> coords = new ArrayList<Coord>();
        private int minX = 0;
        private int maxX = 0;
        private int minY = 0;
        private int maxY = 0;
        
        public void addCoord(int x,int y) {
            
            System.out.println("MapJPanel.addCoord x=" + x + " y=" + y);

            // See if coordinate already exists
            Iterator<Coord> it = coords.iterator();
            boolean found = false;
            while ( ! found && it.hasNext() ) {
                
                Coord coord = it.next();
                
                if ( coord.x == x && coord.y == y ) {
                    
                    found = true;
                }
            }
            // If doesn't exist, add it
            if ( ! found ) {
                
                coords.add( new Coord(x,y));
                
                if ( y > maxY ) maxY = y;
                if ( y < minY ) minY = y;
                if ( x > maxX ) maxX = x;
                if ( x < minX ) minX = x;
            }
            
            this.repaint();
        }
        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            
            
            // Create white background
            int x = 10;
            int y = 20;
            int width = 310;
            int height = 260;
            
            g.setColor(Color.white);
            //rectangle originated at 10,10 and end at 240,240
            g.drawRect(x, y, width, height);
                    //filled Rectangle with rounded corners.    
            g.fillRoundRect(x, y, width, height, 8, 8);
            
            // Draw dots
            
            
            int centerX = (width  - x) / 2;
            int centerY = (height - y) / 2;

            System.out.println("centerX=" + centerX + " centerY=" + centerY);
            
            // Put dot in center
            g.setColor( Color.green);
            g.fillOval(centerX, centerY, 10, 10);                
            
            
            g.setColor( Color.red);

            Iterator<Coord> it = coords.iterator();
            
            while ( it.hasNext() ) {
                
                Coord coord = it.next();
                
                int circleX = coord.x + centerX;
                int circleY = coord.y + centerX;

                g.fillOval(circleX, circleY, 3, 3);                
            }
        }
}
