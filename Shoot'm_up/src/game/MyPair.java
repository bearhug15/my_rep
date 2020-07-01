package game;

import javafx.util.Pair;

public class MyPair {
    int x,y;
    public MyPair(){
        x=0;
        y=0;
    }
    public MyPair(int startX, int startY){
        x=startX;
        y=startY;
    }
    public int getKey(){return x;}
    public int getValue(){return y;}
    public void updateCurent(int x, int y){
        this.x +=x;
        this.y +=y;
    }
    public void update(int x, int y){
        this.x = x;
        this.y = y;
    }
}
