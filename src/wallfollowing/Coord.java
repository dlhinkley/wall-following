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
public class Coord {
    public int x;
    public int y;
    public CellType type;

    Coord(int x, int y, CellType type) {
        this.x = x;
        this.y = y;
        this.type = type;
    }
}
