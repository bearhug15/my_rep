package moving_objects;

import game.MyPair;
import javafx.util.Pair;

import java.util.Iterator;
import java.util.Vector;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class EnemyBullet implements Bullet {
    static String name = "ENEMYBULLET";
    final int defaultSpeedX, defaultSpeedY;
    int speedX,speedY;
    int width,height;
    double damage;
    final double holdDamage;
    Vector<MyPair> points = new Vector<>();

    public EnemyBullet(int speedX, int speedY, int startX,int startY, int width, int height, double damage ){
        this.speedX = speedX;
        this.speedY = speedY;
        defaultSpeedX = speedX;
        defaultSpeedY = speedY;
        this.width = width;
        this.height = height;
        this.damage = damage;
        this.holdDamage = damage;
        points.add(new MyPair(startX - height/2, startY -width/2));
        points.add(new MyPair(startX - height/4, startY +width/2));
        points.add(new MyPair(startX + height/4, startY +width/2));
        points.add(new MyPair(startX + height/2, startY -width/2));
        points.add(new MyPair(startX - height/2, startY -width/2));
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
    public void nextFrame(){
        updateCoords(speedX,speedY);
    }
    @Override
    public void updateCoords(int changeX, int changeY) {
        for(MyPair p :points){
            p.updateCurent(speedX,speedY);
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
    @Override
    public boolean hit(double damage) {
        return false;
    }
    @Override
    public void setNewCoords(int startX, int startY) {
        points.clear();
        points.add(new MyPair(startX - height/2, startY -width/2));
        points.add(new MyPair(startX - height/4, startY +width/2));
        points.add(new MyPair(startX + height/4, startY +width/2));
        points.add(new MyPair(startX + height/2, startY -width/2));
        points.add(new MyPair(startX - height/2, startY -width/2));
    }
    public double getDamage(){
        return damage;
    }
    public double getHoldDamage(){
        return holdDamage;
    }
    public void setSpeedX(int speedX){
        this.speedX = speedX;
    }
    public void setSpeedY(int speedY){
        this.speedY = speedY;
    }
    public int getDefaultSpeedX(){
        return defaultSpeedX;
    }
    public int getDefaultSpeedY(){
        return defaultSpeedY;
    }
    public void setDamage(double damage) {
        this.damage = damage;
    }
}
