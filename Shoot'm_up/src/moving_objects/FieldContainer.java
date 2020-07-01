package moving_objects;

import game.Controller;
import game.MyObserver;
import game.MyPair;
import javafx.util.Pair;
import spawn_scenario.DifSpeedScenario;
import spawn_scenario.DoubleSpawnScenario;
import spawn_scenario.Scenario;
import spawn_scenario.SimpleSpawnScenario;

import java.io.IOException;
import java.io.InputStream;
import java.util.*;

public class FieldContainer implements Observed {
    int fieldWidth,fieldHeight;
    int difficulty=0;
    Vector<MyObserver> observers = new Vector<>();
    LinkedList<Player> players = new LinkedList<>();
    LinkedList<Player> deadPlayers = new LinkedList<>();
    LinkedList<MyBullet> myFlyingBullets = new LinkedList<>();
    LinkedList<MyBullet> myHoldBullets = new LinkedList<>();
    LinkedList<EnemyShip> flyingEnemies = new LinkedList<>();
    LinkedList<EnemyShip> holdEnemies = new LinkedList<>();
    LinkedList<EnemyBullet> flyingEnemyBullets = new LinkedList<>();
    LinkedList<EnemyBullet> holdEnemyBullets = new LinkedList<>();
    Vector<LinkedList<? extends MovingObject>> all = new Vector<>();
    ArrayList<Scenario> Scenaries = new ArrayList<>();
    Controller controller;
    final MyPair leftUpPoint;
    final MyPair rightUpPoint;
    final MyPair leftDownPoint;
    final MyPair rightDownPoint;
    int nextSpawnTimer=0;
    int defaultSpawnTimer;
    Vector<Boolean> playerMoved = new Vector<>();
    Vector<MyPair> playerCoordsChange = new Vector<>();
    Vector<Pair<String,Integer>> scores = new Vector<>();
    boolean isStopped =false;
    boolean gameEnd = false;

    {
        all.add(players);
        all.add(myFlyingBullets);
        all.add(flyingEnemies);
        all.add(flyingEnemyBullets);
    }

    public FieldContainer(int fieldHeight, int fieldWidth, int difficulty/*,Vector<String> playerName*/){
        leftUpPoint = new MyPair(0,0);
        rightUpPoint = new MyPair(fieldWidth,0);
        leftDownPoint = new MyPair(0,fieldHeight);
        rightDownPoint = new MyPair(fieldWidth,fieldHeight);
        this.fieldHeight =fieldHeight;
        this.fieldWidth = fieldWidth;
        this.difficulty = difficulty;
        prepareField(difficulty);
        Scenaries.add(new SimpleSpawnScenario());
        Scenaries.add(new DoubleSpawnScenario());
        Scenaries.add(new DifSpeedScenario());
    }
    void prepareField(/*Vector<String> playerName,*/ int difficulty){
        Properties gameProps = new Properties();
        InputStream stream = this.getClass().getResourceAsStream("config");
        if(stream == null){
            throw new RuntimeException("Config not exists");
        }
        try {
            gameProps.load(stream);
        } catch (IOException e) {
            throw new RuntimeException("Something turned bad in properties");
        }
        int playerStartX  = (int) (fieldHeight*0.9),playerStartY = fieldWidth/2;
        //int playerSpeed, playerR, playerFireSpeed;
        int myBulletHeight, myBulletWidth, myBulletSpeedX, myBulletSpeedY;
        double myBulletDamage;
        int enemyBulletHeight, enemyBulletWidth, enemyBulletSpeedX, enemyBulletSpeedY;
        double enemyBulletDamage,enemyHealth;
        int enemyR, enemySpeedX, enemySpeedY,enemyFireSpeed;
        switch(difficulty){
            case 1:
            case 2:
            case 3:
                //playerFireSpeed = Integer.parseInt(gameProps.getProperty("playerFireSpeed" + difficulty));
                //playerSpeed =Integer.parseInt(gameProps.getProperty("playerSpeed"+difficulty));
                //playerR =Integer.parseInt(gameProps.getProperty("playerR"+difficulty));
                myBulletHeight =Integer.parseInt(gameProps.getProperty("myBulletHeight"+difficulty));
                myBulletWidth =Integer.parseInt(gameProps.getProperty("myBulletWidth"+difficulty));
                myBulletSpeedX =Integer.parseInt(gameProps.getProperty("myBulletSpeedX"+difficulty));
                myBulletSpeedY =Integer.parseInt(gameProps.getProperty("myBulletSpeedY"+difficulty));
                myBulletDamage = Double.parseDouble(gameProps.getProperty("myBulletDamage"+difficulty));
                enemyBulletHeight =Integer.parseInt(gameProps.getProperty("enemyBulletHeight"+difficulty));
                enemyBulletWidth =Integer.parseInt(gameProps.getProperty("enemyBulletWidth"+difficulty));
                enemyBulletSpeedX =Integer.parseInt(gameProps.getProperty("enemyBulletSpeedX"+difficulty));
                enemyBulletSpeedY =Integer.parseInt(gameProps.getProperty("enemyBulletSpeedY"+difficulty));
                enemyBulletDamage = Double.parseDouble(gameProps.getProperty("enemyBulletDamage"+difficulty));
                enemySpeedX =Integer.parseInt(gameProps.getProperty("enemySpeedX"+difficulty));
                enemySpeedY =Integer.parseInt(gameProps.getProperty("enemySpeedY"+difficulty));
                enemyR =Integer.parseInt(gameProps.getProperty("enemyR"+difficulty));
                enemyHealth = Double.parseDouble(gameProps.getProperty("enemyHealth"+difficulty));
                enemyFireSpeed = Integer.parseInt(gameProps.getProperty("enemyFireSpeed"+difficulty));
                defaultSpawnTimer = Integer.parseInt(gameProps.getProperty("spawnTimer"+difficulty));
                break;
            default:
                throw new RuntimeException("Wrong difficulty");
        }
        int enemyStartX = fieldHeight-enemyR, enemyStartY = fieldWidth/2;
        /*for(int i=0;i<playerName.size();i++) {
            players.add(new Player(playerSpeed, playerStartX, playerStartY, playerR, playerFireSpeed, playerName.get(i)));
            playerMoved.add(false);
            playerCoordsChange.add(new MyPair());
        }//*/
        for(int i =0; i<100;i++){
            myHoldBullets.add(new MyBullet(-myBulletSpeedX,myBulletSpeedY,playerStartX,playerStartY,myBulletWidth,myBulletHeight,myBulletDamage));
        }
        for(int i=0; i<20;i++){
            holdEnemies.add(new EnemyShip(enemySpeedX,enemySpeedY,enemyStartX,enemyStartY,enemyR,enemyHealth,enemyFireSpeed));
        }
        for(int i=0;i<500;i++){
            holdEnemyBullets.add(new EnemyBullet(enemyBulletSpeedX,enemyBulletSpeedY,enemyStartX,enemyStartY, enemyBulletWidth, enemyBulletHeight,enemyBulletDamage));
        }
    }

    public Player generatePlayer(String playerName){
        Properties gameProps = new Properties();
        InputStream stream = this.getClass().getResourceAsStream("config");
        if(stream == null){
            throw new RuntimeException("Config not exists");
        }
        try {
            gameProps.load(stream);
        } catch (IOException e) {
            throw new RuntimeException("Something turned bad in properties");
        }
        int playerStartY  = (int) (fieldHeight*0.9),playerStartX = fieldWidth/2;
        int playerSpeed, playerR, playerFireSpeed;

        switch(difficulty) {
            case 1:
            case 2:
            case 3:
                playerFireSpeed = Integer.parseInt(gameProps.getProperty("playerFireSpeed" + difficulty));
                playerSpeed = Integer.parseInt(gameProps.getProperty("playerSpeed" + difficulty));
                playerR = Integer.parseInt(gameProps.getProperty("playerR" + difficulty));
                break;
            default:
                throw new RuntimeException("Wrong difficulty");
        }
            return new Player(playerSpeed, playerStartX, playerStartY, playerR, playerFireSpeed, playerName);

    }

    public void startGame(){
        gameEnd=false;
        myHoldBullets.addAll(myFlyingBullets);
        myFlyingBullets.clear();
        holdEnemies.addAll(flyingEnemies);
        flyingEnemies.clear();
        holdEnemyBullets.addAll(flyingEnemyBullets);
        flyingEnemyBullets.clear();
        //players.addAll(deadPlayers);
        deadPlayers.clear();
        boolean isAlive = true;
        do{
            isAlive = nextFrame();
        }while(isAlive && !isStopped);
        if(isAlive!=true){
            atEnd();
            gameEnd = true;
        }
       /* if(gameEnd){
            for(Player p:players) {
                controller.endGame(p);
            }
        }*/
    }
    public void continueGame(){
        boolean isAlive= true;
        while(!gameEnd && !isStopped && isAlive){
            isAlive = nextFrame();
        }
        if(isAlive!=true){
            atEnd();
            gameEnd = true;
        }
       /* if(gameEnd){
            for(Player p:players) {
                controller.endGame(p);
            }
        }*/
    }
    public void pauseGame(){
        isStopped = true;
    }
    public void endGame(){
        gameEnd = true;
    }
    public void setController(Controller controller){
        this.controller=controller;
    }
    @Override
    public void setObserver(MyObserver observer) {
        observers.add(observer);
    }

    void moveAndCheckBorders(){
        ListIterator enemiesIterator = flyingEnemies.listIterator();
        EnemyShip enemyShip;
        EnemyBullet enemyBullet = null;
        MyBullet myBullet;
        Player player;
        boolean isMoved;
        MyPair changeCoords;
        boolean isShoot=false;
        while(enemiesIterator.hasNext()){
            enemyShip = (EnemyShip) enemiesIterator.next();
            isShoot=enemyShip.nextFrame();
            if(enemyShip.isCollide(leftDownPoint,rightDownPoint)){
                enemiesIterator.previous();
                enemiesIterator.remove();
                holdEnemies.add(enemyShip);
                continue;
            }
            if(enemyShip.isCollide(leftUpPoint,leftDownPoint) || enemyShip.isCollide(rightUpPoint,rightDownPoint)){
                enemyShip.changeSpeed(-enemyShip.getSpeedX(),enemyShip.getSpeedY());
            }
            if(isShoot && !holdEnemyBullets.isEmpty()){
                enemyBullet =holdEnemyBullets.remove();
                enemyBullet.setNewCoords(enemyShip.getCenterX(),enemyShip.getCenterY());
                enemyBullet.setSpeedY(enemyShip.getSpeedY()+enemyBullet.getDefaultSpeedY());
                flyingEnemyBullets.add(enemyBullet);
            }
        }
       // synchronized (flyingEnemyBullets) {
            ListIterator enemyBulletsIterator = flyingEnemyBullets.listIterator();
            while (enemyBulletsIterator.hasNext()) {
                enemyBullet = (EnemyBullet) enemyBulletsIterator.next();
                enemyBullet.nextFrame();
                if (enemyBullet.isCollide(leftDownPoint, rightDownPoint)) {
                    enemyBulletsIterator.previous();
                    enemyBulletsIterator.remove();
                    holdEnemyBullets.add(enemyBullet);
                }
            }
       // }
        ListIterator myBulletsIterator = myFlyingBullets.listIterator();
        while(myBulletsIterator.hasNext()){
            myBullet = (MyBullet) myBulletsIterator.next();
            myBullet.nextFrame();
            if(myBullet.isCollide(leftDownPoint,rightDownPoint)){
                myBulletsIterator.previous();
                myBulletsIterator.remove();
                myHoldBullets.add(myBullet);
            }
        }
        ListIterator playerCoordsIterator = playerCoordsChange.listIterator();
        ListIterator playerMovedIterator = playerMoved.listIterator();
        ListIterator playerIterator = players.listIterator();
        while (playerIterator.hasNext()){
            player = (Player) playerIterator.next();
            isMoved = (Boolean) playerMovedIterator.next();
            changeCoords = (MyPair) playerCoordsIterator.next();
            if(isMoved == true){
                player.setMoveX(changeCoords.getKey() * player.getSpeed());
                player.setMoveY(changeCoords.getValue() * player.getSpeed());
                player.setMoved(true);
            }
            isShoot = player.nextFrame();
            if(isShoot && !myHoldBullets.isEmpty()){
                myBullet = myHoldBullets.remove();
                myBullet.setShootedPlayer(player);
                myBullet.setNewCoords(player.getCenterX(),player.getCenterY());
                myFlyingBullets.add(myBullet);
            }
        }
    }

    boolean checkHits(){
        boolean isAlive = true;
        /*ListIterator moveIterator = playerMoved.listIterator();
        ListIterator coordsChangeIterator = playerCoordsChange.listIterator();
        MyPair coordsChange;
        for(int i=0;i<playerMoved.size();i++){
            coordsChange = (MyPair) coordsChangeIterator.next();
            if(moveIterator.next().equals(true)){
                players.get(i).updateCoords(coordsChange.getKey(),coordsChange.getValue());
            }
            if(!(moveIterator.hasNext() && coordsChangeIterator.hasNext())){
                break;
            }
        }*/
        ListIterator playerIterator = players.listIterator();
        ListIterator enemiesIterator = flyingEnemies.listIterator();
        EnemyShip enemyShip;
        ListIterator enemyBulletsIterator = flyingEnemyBullets.listIterator();
        EnemyBullet enemyBullet;
        ListIterator myBulletsIterator = myFlyingBullets.listIterator();
        MyBullet myBullet;
        boolean hitDetector=false;
        while(playerIterator.hasNext()){
            Player player = (Player) playerIterator.next();
            while(enemiesIterator.hasNext()){
                enemyShip =(EnemyShip)enemiesIterator.next();
                hitDetector = player.isCollide(enemyShip);
                if(hitDetector){
                    enemiesIterator.previous();
                    enemiesIterator.remove();
                    holdEnemies.add(enemyShip);
                    if(!player.hit(30)){
                        playerIterator.previous();
                        playerIterator.remove();
                        deadPlayers.add(player);
                        break;
                    }
                }
            }
            while(enemyBulletsIterator.hasNext()){
                enemyBullet =(EnemyBullet)enemyBulletsIterator.next();
                hitDetector = player.isCollide(enemyBullet);
                if(hitDetector){
                    System.out.println("HIT");
                    enemyBulletsIterator.previous();
                    enemyBulletsIterator.remove();
                    holdEnemyBullets.add(enemyBullet);
                    if(!player.hit(enemyBullet.getDamage())){
                        playerIterator.previous();
                        playerIterator.remove();
                        deadPlayers.add(player);
                        break;
                    }
                }
            }
        }
        enemiesIterator = flyingEnemies.listIterator();
        while(myBulletsIterator.hasNext()){
            myBullet = (MyBullet) myBulletsIterator.next();
            while(enemiesIterator.hasNext()){
                enemyShip =(EnemyShip)enemiesIterator.next();
                hitDetector = myBullet.isCollide(enemyShip);
                if(hitDetector){
                    if(!enemyShip.hit(myBullet.getDamage())){
                        enemiesIterator.previous();
                        enemiesIterator.remove();
                        holdEnemies.add(enemyShip);
                        myBullet.getShootedPlayer().updateScore(1);
                    }
                    myBulletsIterator.previous();
                    myBulletsIterator.remove();
                    break;
                }
            }
        }
        if(players.isEmpty()){
            isAlive =false;
        }
        return isAlive;
    }
    boolean nextFrame(){
        if(nextSpawnTimer==0){
            int spawnScenario = (int) (Math.random()*Scenaries.size());
            Scenaries.get(spawnScenario).spawn(holdEnemies,flyingEnemies,fieldWidth);
            nextSpawnTimer=defaultSpawnTimer;
        }
        moveAndCheckBorders();
        boolean isAlive = true;
        isAlive = checkHits();

        nextSpawnTimer--;
        for(MyObserver o :observers){
            o.update(all);
        }
        return isAlive;
    }

    void atEnd()  {
        for(Player player:deadPlayers){
            scores.add(new Pair<>(player.getPlayerName(),player.getScore()));
        }
        for(Player player:players){
            scores.add(new Pair<>(player.getPlayerName(),player.getScore()));
        }

    }

    public Vector<Pair<String, Integer>> getScores() {
        return scores;
    }
    public void movePlayer(Player player, int x, int y){
        synchronized (playerMoved){
            synchronized (playerCoordsChange){
                ListIterator playerIterator = players.listIterator();
                Player player1;
                ListIterator playerMoveIterator = playerMoved.listIterator();
                boolean move;
                ListIterator coordsIterator = playerCoordsChange.listIterator();

                while(playerIterator.hasNext()){
                    player1 = (Player) playerIterator.next();
                    if(player1.equals(player)){
                        playerMoveIterator.remove();
                        playerMoveIterator.add(true);
                        coordsIterator.remove();
                        coordsIterator.add(new MyPair(x,y));
                        break;
                    }else{
                        playerMoveIterator.next();
                        coordsIterator.next();
                    }
                }
            }
        }
    }
    public void addPlayer(Player player){
        players.add(player);
        playerMoved.add(false);
        playerCoordsChange.add(new MyPair());
    }
    public LinkedList<Player> getPlayers() { return players; }
}
