package moving_objects;

import game.MyObserver;
import game.MyPair;
import game.UpdateType;
import javafx.util.Pair;

import java.util.Iterator;
import java.util.Random;
import java.util.Vector;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class EnemyShip implements Ship {
    double health;
    final double holdHealth;
    static String name = "ENEMYSHIP";
    int speedX,speedY;
    final int defaultSpeedX, defaultSpeedY;
    Vector<MyPair> points = new Vector<>();
    int centerX,centerY;
    final int r;
    final int holdFireTimer;
    int fireTimer;
    int timeToShoot;
    public EnemyShip(int speedX, int speedY,int startX, int startY, int R, double health, int fireTimer){
        this.r = R;
        this.speedX = speedX;
        this.speedY = speedY;
        defaultSpeedX = speedX;
        defaultSpeedY = speedY;
        holdFireTimer = fireTimer;
        this.fireTimer = fireTimer;
        timeToShoot = fireTimer;
        centerX = startX;
        centerY = startY;
        points.add(new MyPair(startX-R,startY));
        points.add(new MyPair(startX,startY-R));
        points.add(new MyPair(startX+R,startY));
        points.add(new MyPair(startX,startY+R));
        points.add(new MyPair(startX-R,startY));
        this.health =health;
        this.holdHealth = health;
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
                otherFirst =otherSecond ;
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
        updateCoords(speedX,speedY);
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
    public boolean hit(double damage){
        health = health-damage;
        Vector<Double> h= new Vector<>();
        h.add(health);
        if(health>0){
            return true;
        }else{
            return false;
        }
    }
    @Override
    public void setNewCoords(int startX, int startY) {
        centerX = startX;
        centerY = startY;
        points.clear();
        points.add(new MyPair(startX-r,startY));
        points.add(new MyPair(startX,startY-r));
        points.add(new MyPair(startX+r,startY));
        points.add(new MyPair(startX,startY+r));
        points.add(new MyPair(startX-r,startY));
    }
     public void changeSpeed(int newSpeedX, int newSpeedY){
        speedX = newSpeedX;
        speedY = newSpeedY;
    }
    public void setHealth(double health){
        this.health = health;
    }
    @Override
    public double getHealth() {
        return health;
    }
    public double getHoldHealth(){
        return holdHealth;
    }
    public int getR(){
        return r;
    }
    public int getDefaultSpeedX(){
        return defaultSpeedX;
    }
    public int getDefaultSpeedY(){
        return defaultSpeedY;
    }
    public int getSpeedX(){
        return speedX;
    }
    public int getSpeedY(){
        return speedY;
    }
    public int getHoldFireTimer() { return holdFireTimer; }
    public int getFireTimer() { return fireTimer; }
    public int getTimeToShoot() { return timeToShoot; }
    public int getCenterX() { return centerX; }
    public int getCenterY() { return centerY; }
}
