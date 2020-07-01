package moving_objects;

import game.MyPair;

import java.util.Iterator;
import java.util.Vector;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class Player implements Ship {
    static String name = "PLAYER";
    double health;
    final String playerName;
    int speed;
    Vector<MyPair> points = new Vector<>();
    int score = 0;
    int R;
    boolean isMoved = false;
    int moveX, moveY;
    int centerX, centerY;
    final int holdFireTimer;
    int fireTimer;
    int timeToShoot;

    public Player(int speed, int startX, int startY, int R, int fireTimer, String playerName) {
        centerX = startX;
        centerY = startY;
        this.fireTimer = fireTimer;
        holdFireTimer = fireTimer;
        timeToShoot = fireTimer;
        this.playerName = playerName;
        this.R = R;
        health = 100;
        points.add(new MyPair(startX, startY - R));
        points.add(new MyPair(startX - R / 2, startY));
        points.add(new MyPair(startX - R, startY - R));
        points.add(new MyPair(startX - R, startY + R));
        points.add(new MyPair(startX + R, startY + R));
        points.add(new MyPair(startX + R, startY - R));
        points.add(new MyPair(startX + R / 2, startY));
        points.add(new MyPair(startX, startY - R));
        this.speed = speed;
    }

    int area(MyPair a, MyPair b, MyPair c) {
        return (b.getKey() - a.getKey()) * (c.getValue() - a.getValue()) - (b.getValue() - a.getValue()) * (c.getKey() - a.getKey());
    }
    boolean intersectSub(MyPair a, MyPair b, MyPair c, MyPair d, boolean isKeys) {
        if(isKeys) {
            int buffA=a.getKey(),buffB=b.getKey(),buffC=c.getKey(),buffD=d.getKey();
            int buff;
            if (buffA > buffB) {
                buff=buffA;
                buffA=buffB;
                buffB=buff;
            }
            if (buffC > buffD) {
                buff=buffC;
                buffC=buffD;
                buffD=buff;
            }
            return Integer.max(buffA,buffC)<= Integer.min(buffB,buffD);
        }else{
            int buffA=a.getValue(),buffB=b.getValue(),buffC=c.getValue(),buffD=d.getValue();
            int buff;
            if (buffA > buffB) {
                buff=buffA;
                buffA=buffB;
                buffB=buff;
            }
            if (buffC > buffD) {
                buff=buffC;
                buffC=buffD;
                buffD=buff;
            }
            return Integer.max(buffA,buffC)<= Integer.min(buffB,buffD);
        }
    }
    boolean intersect (MyPair a, MyPair b, MyPair c, MyPair d) {
        return intersectSub(a, b, c, d,true)
                && intersectSub (a, b, c, d,false)
                && (area(a,b,c) * area(a,b,d) <= 0)
                && (area(c,d,a) * area(c,d,b) <= 0);
    }
    @Override
    public boolean isCollide(MovingObject obj) {
        Vector<MyPair> otherPoints = obj.getPoints();
        Iterator otherIterator = otherPoints.iterator();
        Iterator myIterator = points.iterator();
        MyPair myFirst = (MyPair) myIterator.next(), mySecond;
        MyPair otherFirst = (MyPair) otherIterator.next(),otherSecond;
        boolean hit=false;
        while(myIterator.hasNext()){
            mySecond =  (MyPair) myIterator.next();
            while(otherIterator.hasNext()){
                otherSecond = (MyPair) otherIterator.next();
                if(intersect(myFirst,mySecond,otherFirst,otherSecond)){
                    hit = true;
                    break;
                }
                otherFirst = otherSecond;
            }
            if(hit){
                break;
            }
            myFirst =mySecond;
        }
        return hit;
    }
    @Override
    public boolean isCollide(MyPair firstPoint,MyPair secondPoint) {
        Iterator myIterator = points.iterator();
        MyPair myFirst = (MyPair) myIterator.next(), mySecond;
        boolean hit=false;
        while(myIterator.hasNext()){
            mySecond =  (MyPair) myIterator.next();
                hit = intersect(myFirst,mySecond,firstPoint,secondPoint);
            if(hit){
                break;
            }
            myFirst =mySecond;
        }
        return hit;
    }
    public boolean nextFrame(){
        if(isMoved){
            updateCoords(moveX,moveY);
            moveX =0;
            moveX=0;
            isMoved = false;
        }
        if(timeToShoot ==0){
            timeToShoot = fireTimer;
            return true;
        }else {
            timeToShoot--;
            return false;
        }
    }
    @Override
    public void updateCoords(int changeX, int changeY) {
        centerX+=changeX;
        centerY+=changeY;
        for(MyPair p:points){
            p.updateCurent(changeX,changeY);
        }
    }
    @Override
    public Vector<MyPair> getPoints() {
        return points;
    }
    @Override
    public String getName() {
        return name;
    }
    public String getPlayerName(){
        return playerName;
    }
    public boolean hit(double damage){
        health = health-damage;
        if(health>0){
            return true;
        }else{
            return false;
        }
    }
    @Override
    public void setNewCoords(int startX, int startY) {
        centerX += startX;
        centerY += startY;
        points.clear();
        points.add(new MyPair(startX,startY- R));
        points.add(new MyPair(startX- R /2,startY));
        points.add(new MyPair(startX- R,startY- R));
        points.add(new MyPair(startX- R,startY+ R));
        points.add(new MyPair(startX+ R,startY+ R));
        points.add(new MyPair(startX+ R,startY- R));
        points.add(new MyPair(startX+ R /2,startY));
        points.add(new MyPair(startX,startY- R));
    }
    public void setFireSpeed(int fireTimer){ this.fireTimer = fireTimer; }
    public int getFireTimer(){ return fireTimer; }
    public int getHoldFireTimer() { return holdFireTimer; }
    public int getTimeToShoot() { return timeToShoot; }
    public int getSpeed() { return speed; }
    @Override
    public double getHealth() { return health; }
    public void updateScore(int s){ score+=s; }
    public int getScore(){ return score; }
    public void setMoveX(int moveX) { this.moveX = moveX; }
    public void setMoveY(int moveY) { this.moveY = moveY; }
    public void setMoved(boolean moved) { isMoved = moved; }
    public int getCenterX() { return centerX; }
    public int getCenterY() { return centerY; }
}
