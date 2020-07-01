package moving_objects;

import game.MyPair;
import javafx.util.Pair;

import java.util.Vector;

public interface MovingObject {
    public boolean isCollide(MovingObject obj);
    public boolean isCollide(MyPair firstPoint,MyPair secondPoint);
    public void updateCoords(int changeX, int changeY);
    public Vector<MyPair> getPoints();
    public String getName();
    public boolean hit(double damage);
    public void setNewCoords(int x, int y);
}
