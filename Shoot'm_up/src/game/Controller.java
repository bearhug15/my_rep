package game;

import javafx.util.Pair;
import moving_objects.FieldContainer;
import moving_objects.Player;

import java.awt.event.KeyListener;
import java.io.*;
import java.lang.reflect.Constructor;
import java.util.*;

public class Controller {
    FieldContainer fieldContainer;

    public Controller(FieldContainer fieldContainer){
        this.fieldContainer = fieldContainer;
    }
    public void addPlayer(Player player){
        fieldContainer.addPlayer(player);
    }
    public void movePlayer(Player movedPlayer, int changeX, int changeY){
        fieldContainer.movePlayer(movedPlayer, changeX, changeY);
    }
    public void moveUp(Player player){
        fieldContainer.movePlayer(player,-1,0);
    }
    public void moveDown(Player player){
        fieldContainer.movePlayer(player,1,0);
    }
    public void moveLeft(Player player){
        fieldContainer.movePlayer(player,0,-1);
    }
    public void moveRight(Player player){
        fieldContainer.movePlayer(player,0,1);
    }
    public void startGame(){
       fieldContainer.startGame();
    }
    public void continueGame(){
       fieldContainer.continueGame();
    }
    public void pauseGame(){
        fieldContainer.pauseGame();
    }
    public void endGame(Player player){
        System.out.println("END GAME");
        fieldContainer.endGame();
        BufferedReader br;
        BufferedWriter bw;
        String line;
        List<String> entries = new LinkedList();
        try{
            br = new BufferedReader(new FileReader("src/game/scoreTable"));
            while ((line = br.readLine()) != null)
            {
                entries.add(line);
            }
            br.close();
            int score = getPlayerScore(player);
            entries.add(score+" "+player.getPlayerName());
            Collections.sort(entries,Collections.reverseOrder());
            Iterator iter = entries.iterator();
            bw = new BufferedWriter(new FileWriter("src/game/scoreTable"));
            int i=0;
            while(iter.hasNext()&&i<5){
                bw.write((String) iter.next());
                bw.newLine();
                i++;
            }
            bw.close();
        } catch (FileNotFoundException e) {
            System.out.println("No score file");
        } catch (IOException e) {
            System.out.println("Error");
            e.printStackTrace();
        }
    }
    public int getPlayerScore(Player player){
        Vector<Pair<String,Integer>> v = fieldContainer.getScores();
        String playerName = player.getPlayerName();
        for(Pair<String,Integer> p:v){
            if(p.getKey().equals(playerName)){
                return p.getValue();
            }
        }
        throw new RuntimeException("WTF?");
    }
    public Vector<Pair<String,Integer>> getScores(){
       return fieldContainer.getScores();
    }
    public void setObserver(MyObserver obs){
       fieldContainer.setObserver(obs);
    }
}
