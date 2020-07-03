package moving_objects;

import MyThreadPool.ScheduledPairedThreadPool;
import game.Controller;
import game.MyObserver;
import game.MyPair;
import javafx.util.Pair;
import spawn_scenario.DifSpeedScenario;
import spawn_scenario.DoubleSpawnScenario;
import spawn_scenario.Scenario;
import spawn_scenario.SimpleSpawnScenario;

import javax.management.monitor.Monitor;
import java.io.IOException;
import java.io.InputStream;
import java.util.*;

public class FieldContainer implements Observed {
    final int fieldWidth,fieldHeight;
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
    MoveAndCheck moveAndCheck;
    HitsChecker hitsChecker;
    ScheduledPairedThreadPool myPool;
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

    public HitsChecker getHitsChecker() {
        return hitsChecker;
    }
    public MoveAndCheck getMoveAndCheck() {
        return moveAndCheck;
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
        for(int i=0; i<50;i++){
            holdEnemies.add(new EnemyShip(enemySpeedX,enemySpeedY,enemyStartX,enemyStartY,enemyR,enemyHealth,enemyFireSpeed));
        }
        for(int i=0;i<700;i++){
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
        moveAndCheck = new MoveAndCheck(players,deadPlayers,myFlyingBullets,myHoldBullets,flyingEnemies,holdEnemies,flyingEnemyBullets,holdEnemyBullets,leftUpPoint,rightUpPoint,leftDownPoint,rightDownPoint,playerMoved,playerCoordsChange,this);
        hitsChecker = new HitsChecker(players,deadPlayers,myFlyingBullets,myHoldBullets,flyingEnemies,holdEnemies,flyingEnemyBullets,holdEnemyBullets,leftUpPoint,rightUpPoint,leftDownPoint,rightDownPoint,this);
        moveAndCheck.setHitsChecker();
        hitsChecker.setMoveAndCheck();
        myPool = new ScheduledPairedThreadPool();
        myPool.scheduleAtFixedRate(moveAndCheck,hitsChecker,0,20);
    }
    public void continueGame(){
        myPool = new ScheduledPairedThreadPool();
        myPool.scheduleAtFixedRate(moveAndCheck,hitsChecker,0,20);
       /* if(gameEnd){
            for(Player p:players) {
                controller.endGame(p);
            }
        }*/
    }
    public void pauseGame(){
        isStopped = true;
        myPool.setStop(true);
        myPool.clearQueue();
    }
    public void endGame(){
        gameEnd = true;
        myPool.setStop(true);
        myPool.clearQueue();
        atEnd();
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

    void nextFrame(boolean isAlive){
        if(nextSpawnTimer==0){
            int spawnScenario = (int) (Math.random()*Scenaries.size());
            Scenaries.get(spawnScenario).spawn(holdEnemies,flyingEnemies,fieldWidth);
            nextSpawnTimer=defaultSpawnTimer;
        }
        nextSpawnTimer--;
        for(MyObserver o :observers){
            o.update(all);
        }
        if(!isAlive){
            gameEnd =true;
            atEnd();
        }
    }

    void atEnd()  {
        for(Player player:deadPlayers){
            scores.add(new Pair<>(player.getPlayerName(),player.getScore()));
        }
        for(Player player:players){
            scores.add(new Pair<>(player.getPlayerName(),player.getScore()));
        }
        myPool.shutdown();
    }

    public Vector<Pair<String, Integer>> getScores() {
        return scores;
    }
    public void movePlayer(Player player, int x, int y){
        synchronized (hitsChecker.players) {
            ListIterator playerIterator = players.listIterator();
            Player player1;
            ListIterator playerMoveIterator = playerMoved.listIterator();
            boolean move;
            ListIterator coordsIterator = playerCoordsChange.listIterator();
            while (playerIterator.hasNext()) {
                player1 = (Player) playerIterator.next();
                if (player1.equals(player)) {
                    playerMoveIterator.remove();
                    playerMoveIterator.add(true);
                    coordsIterator.remove();
                    coordsIterator.add(new MyPair(x, y));
                    break;
                } else {
                    playerMoveIterator.next();
                    coordsIterator.next();
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

class MoveAndCheck implements Runnable{
    LinkedList<Player> players ;
    LinkedList<Player> deadPlayers ;
    LinkedList<MyBullet> myFlyingBullets ;
    LinkedList<MyBullet> myHoldBullets ;
    LinkedList<EnemyShip> flyingEnemies;
    LinkedList<EnemyShip> holdEnemies;
    LinkedList<EnemyBullet> flyingEnemyBullets;
    LinkedList<EnemyBullet> holdEnemyBullets;
    final MyPair leftUpPoint;
    final MyPair rightUpPoint;
    final MyPair leftDownPoint;
    final MyPair rightDownPoint;
    Vector<Boolean> playerMoved ;
    Vector<MyPair> playerCoordsChange;
    FieldContainer fieldContainer;
    HitsChecker hitsChecker;
    public MoveAndCheck(LinkedList<Player> players ,
                        LinkedList<Player> deadPlayers ,
                        LinkedList<MyBullet> myFlyingBullets ,
                        LinkedList<MyBullet> myHoldBullets ,
                        LinkedList<EnemyShip> flyingEnemies,
                        LinkedList<EnemyShip> holdEnemies,
                        LinkedList<EnemyBullet> flyingEnemyBullets,
                        LinkedList<EnemyBullet> holdEnemyBullets,
                        MyPair leftUpPoint,
                        MyPair rightUpPoint,
                        MyPair leftDownPoint,
                        MyPair rightDownPoint,
                        Vector<Boolean> playerMoved,
                        Vector<MyPair> playerCoordsChange,
                        FieldContainer fieldContainer){
        this.players = players;
        this.deadPlayers = deadPlayers;
        this.myFlyingBullets = myFlyingBullets;
        this.myHoldBullets = myHoldBullets;
        this.flyingEnemies = flyingEnemies;
        this.holdEnemies = holdEnemies;
        this.flyingEnemyBullets = flyingEnemyBullets;
        this.holdEnemyBullets =holdEnemyBullets;
        this.leftDownPoint = leftDownPoint;
        this.leftUpPoint = leftUpPoint;
        this.rightDownPoint = rightDownPoint;
        this.rightUpPoint =rightUpPoint;
        this.playerMoved =playerMoved;
        this.playerCoordsChange =playerCoordsChange;
        this.fieldContainer = fieldContainer;
    }

    public HitsChecker getHitsChecker() {
        return hitsChecker;
    }

    public void setHitsChecker() {
        this.hitsChecker = fieldContainer.getHitsChecker();
    }

    @Override
    public void run() {
            System.out.println("Moving Objects");
            EnemyShip enemyShip;
            EnemyBullet enemyBullet = null;
            MyBullet myBullet;
            Player player;
            boolean isMoved;
            MyPair changeCoords;
            boolean isShoot=false;
            synchronized (hitsChecker.players) {
                ListIterator playerCoordsIterator = playerCoordsChange.listIterator();
                ListIterator playerMovedIterator = playerMoved.listIterator();
                ListIterator playerIterator = players.listIterator();
                while (playerIterator.hasNext()) {
                    player = (Player) playerIterator.next();
                    isMoved = (Boolean) playerMovedIterator.next();
                    changeCoords = (MyPair) playerCoordsIterator.next();
                    if (isMoved == true) {
                        player.setMoveX(changeCoords.getKey() * player.getSpeed());
                        player.setMoveY(changeCoords.getValue() * player.getSpeed());
                        player.setMoved(true);
                    }
                    isShoot = player.nextFrame();
                    if (isShoot && !myHoldBullets.isEmpty()) {
                        myBullet = myHoldBullets.remove();
                        myBullet.setShootedPlayer(player);
                        myBullet.setNewCoords(player.getCenterX(), player.getCenterY());
                        myFlyingBullets.add(myBullet);
                    }
                }
            }
            synchronized (hitsChecker.flyingEnemies) {
                ListIterator enemiesIterator = flyingEnemies.listIterator();
                while (enemiesIterator.hasNext()) {
                    enemyShip = (EnemyShip) enemiesIterator.next();
                    isShoot = enemyShip.nextFrame();
                    if (enemyShip.isCollide(leftDownPoint, rightDownPoint)) {
                        enemiesIterator.previous();
                        enemiesIterator.remove();
                        holdEnemies.add(enemyShip);
                        continue;
                    }
                    if (enemyShip.isCollide(leftUpPoint, leftDownPoint) || enemyShip.isCollide(rightUpPoint, rightDownPoint)) {
                        enemyShip.changeSpeed(-enemyShip.getSpeedX(), enemyShip.getSpeedY());
                    }
                    if (isShoot && !holdEnemyBullets.isEmpty()) {
                        enemyBullet = holdEnemyBullets.remove();
                        enemyBullet.setNewCoords(enemyShip.getCenterX(), enemyShip.getCenterY());
                        enemyBullet.setSpeedY(enemyShip.getSpeedY() + enemyBullet.getDefaultSpeedY());
                        flyingEnemyBullets.add(enemyBullet);
                    }
                }
                hitsChecker.setFlyingEnemiesReady(true);
                hitsChecker.flyingEnemies.notify();
            }

            synchronized (hitsChecker.flyingEnemyBullets) {
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
                hitsChecker.setFlyingEnemyBulletsReady(true);
                hitsChecker.flyingEnemyBullets.notify();
            }

            synchronized (hitsChecker.myFlyingBullets) {
                ListIterator myBulletsIterator = myFlyingBullets.listIterator();
                while (myBulletsIterator.hasNext()) {
                    myBullet = (MyBullet) myBulletsIterator.next();
                    myBullet.nextFrame();
                    if (myBullet.isCollide(leftDownPoint, rightDownPoint)) {
                        myBulletsIterator.previous();
                        myBulletsIterator.remove();
                        myHoldBullets.add(myBullet);
                    }
                }
                hitsChecker.setMyBulletsReady(true);
                hitsChecker.myFlyingBullets.notify();
            }

    }
}

class HitsChecker implements Runnable{
    public LinkedList<Player> players ;
    public LinkedList<Player> deadPlayers ;
    public LinkedList<MyBullet> myFlyingBullets ;
    public LinkedList<MyBullet> myHoldBullets ;
    public LinkedList<EnemyShip> flyingEnemies;
    public LinkedList<EnemyShip> holdEnemies;
    public LinkedList<EnemyBullet> flyingEnemyBullets;
    public LinkedList<EnemyBullet> holdEnemyBullets;
    final MyPair leftUpPoint;
    final MyPair rightUpPoint;
    final MyPair leftDownPoint;
    final MyPair rightDownPoint;
    FieldContainer fieldContainer;
    MoveAndCheck moveAndCheck;
    boolean isFlyingEnemiesReady =false;
    boolean isFlyingEnemyBulletsReady = false;
    boolean isMyBulletsReady = false;
    boolean isMeReady = false;
    public HitsChecker(LinkedList<Player> players ,
                       LinkedList<Player> deadPlayers ,
                       LinkedList<MyBullet> myFlyingBullets ,
                       LinkedList<MyBullet> myHoldBullets ,
                       LinkedList<EnemyShip> flyingEnemies,
                       LinkedList<EnemyShip> holdEnemies,
                       LinkedList<EnemyBullet> flyingEnemyBullets,
                       LinkedList<EnemyBullet> holdEnemyBullets,
                       MyPair leftUpPoint,
                       MyPair rightUpPoint,
                       MyPair leftDownPoint,
                       MyPair rightDownPoint,
                       FieldContainer fieldContainer){
        this.players = players;
        this.deadPlayers = deadPlayers;
        this.myFlyingBullets = myFlyingBullets;
        this.myHoldBullets = myHoldBullets;
        this.flyingEnemies = flyingEnemies;
        this.holdEnemies = holdEnemies;
        this.flyingEnemyBullets = flyingEnemyBullets;
        this.holdEnemyBullets =holdEnemyBullets;
        this.leftDownPoint = leftDownPoint;
        this.leftUpPoint = leftUpPoint;
        this.rightDownPoint = rightDownPoint;
        this.rightUpPoint =rightUpPoint;
        this.fieldContainer =fieldContainer;
    }

    public MoveAndCheck getMoveAndCheck() {
        return moveAndCheck;
    }

    public void setMoveAndCheck() {
        this.moveAndCheck = fieldContainer.getMoveAndCheck();
    }

    @Override
    public void run() {
            boolean isAlive = true;
            System.out.println("Checking hits");
            ListIterator playerIterator = players.listIterator();

            EnemyShip enemyShip;

            EnemyBullet enemyBullet;

            MyBullet myBullet;
            boolean hitDetector=false;
            synchronized (flyingEnemies) {
                while (!isFlyingEnemiesReady) {
                    try {
                        flyingEnemies.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
               // try {
                ListIterator enemiesIterator = flyingEnemies.listIterator();
                    while (playerIterator.hasNext()) {
                        Player player = (Player) playerIterator.next();
                        while (enemiesIterator.hasNext()) {
                            enemyShip = (EnemyShip) enemiesIterator.next();
                            hitDetector = player.isCollide(enemyShip);
                            if (hitDetector) {
                                enemiesIterator.previous();
                                enemiesIterator.remove();
                                holdEnemies.add(enemyShip);
                                if (!player.hit(30)) {
                                    playerIterator.previous();
                                    playerIterator.remove();
                                    deadPlayers.add(player);
                                    break;
                                }
                            }
                        }
                    }
               // }catch(Exception e){ }
            }
            synchronized (flyingEnemyBullets) {
                while(!isFlyingEnemyBulletsReady){
                    try {
                        flyingEnemyBullets.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
               // try {
                ListIterator enemyBulletsIterator = flyingEnemyBullets.listIterator();
                    playerIterator = players.listIterator();
                    while (playerIterator.hasNext()) {
                        Player player = (Player) playerIterator.next();
                        while (enemyBulletsIterator.hasNext()) {
                            enemyBullet = (EnemyBullet) enemyBulletsIterator.next();
                            hitDetector = player.isCollide(enemyBullet);
                            if (hitDetector) {
                                System.out.println("HIT");
                                enemyBulletsIterator.previous();
                                enemyBulletsIterator.remove();
                                holdEnemyBullets.add(enemyBullet);
                                if (!player.hit(enemyBullet.getDamage())) {
                                    playerIterator.previous();
                                    playerIterator.remove();
                                    deadPlayers.add(player);
                                    break;
                                }
                            }
                        }
                    }
                //}catch(Exception e){}
            }
            synchronized (myFlyingBullets) {
                while (!isMyBulletsReady){
                    try {
                        myFlyingBullets.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
               // try {
                ListIterator enemiesIterator = flyingEnemies.listIterator();
                ListIterator myBulletsIterator = myFlyingBullets.listIterator();
                    enemiesIterator = flyingEnemies.listIterator();
                    while (myBulletsIterator.hasNext()) {
                        myBullet = (MyBullet) myBulletsIterator.next();
                        while (enemiesIterator.hasNext()) {
                            enemyShip = (EnemyShip) enemiesIterator.next();
                            hitDetector = myBullet.isCollide(enemyShip);
                            if (hitDetector) {
                                if (!enemyShip.hit(myBullet.getDamage())) {
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
                //}catch (Exception e){}
            }

            if(players.isEmpty()){
                isAlive =false;
            }
            isFlyingEnemiesReady =false;
            isFlyingEnemyBulletsReady =false;
            isMeReady = false;
            isMyBulletsReady =false;
            fieldContainer.nextFrame(isAlive);
    }

    public void setMeReady(boolean meReady) {
        isMeReady = meReady;
    }

    public void setFlyingEnemiesReady(boolean flyingEnemiesReady) {
        isFlyingEnemiesReady = flyingEnemiesReady;
    }

    public void setMyBulletsReady(boolean myBulletsReady) {
        isMyBulletsReady = myBulletsReady;
    }

    public void setFlyingEnemyBulletsReady(boolean flyingEnemyBulletsReady) {
        isFlyingEnemyBulletsReady = flyingEnemyBulletsReady;
    }
}
