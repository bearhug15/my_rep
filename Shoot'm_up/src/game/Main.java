package game;
import com.sun.source.tree.Scope;
import graphic.BackPanel;
import graphic.MainFrame;
import graphic.MainPanel;
import moving_objects.FieldContainer;
import moving_objects.Player;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.ImageObserver;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.text.AttributedCharacterIterator;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Timer;
import java.util.Vector;

import static java.lang.Thread.sleep;

public class Main {
    public static void main(String [] argv){
        //this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        int WIDTH = 500, HEIGHT =1000;


        JFrame frame = new JFrame("TRy TO LivE");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(WIDTH,HEIGHT);


        //frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
        frame.setResizable(false);
        FieldContainer fieldContainer = new FieldContainer(HEIGHT,WIDTH,1);

        //Player myPlayer = fieldContainer.generatePlayer("Player1");

        Controller mainController = new Controller(fieldContainer);
        fieldContainer.setController(mainController);
        JMenuBar inGameMenu = new JMenuBar();
        JMenuItem startMenu = new JMenuItem("Start");
        JMenuItem continueMenu = new JMenuItem("Continue");
        continueMenu.setEnabled(false);
        JMenuItem pauseMenu = new JMenuItem("Pause");
        pauseMenu.setEnabled(false);
        JMenuItem stopMenu = new JMenuItem("Stop");
        stopMenu.setEnabled(false);
        JMenuItem scoresMenu = new JMenuItem("Scores");
        //startMenu.addActionListener(new myStartActionListener(myPlayer));
        PlayerWrapper localPlayerWrapper = new PlayerWrapper();
        MainPanel panel = new MainPanel(mainController);
        MyStartActionListaner aL= new MyStartActionListaner(localPlayerWrapper,startMenu,pauseMenu,stopMenu,scoresMenu,mainController,fieldContainer, panel);
        startMenu.addActionListener(aL);
        /*startMenu.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                String playername  = JOptionPane.showInputDialog("Please input a player name");
                //myPlayer = fieldContainer.generatePlayer(playername);
                startMenu.setEnabled(false);
                pauseMenu.setEnabled(true);
                stopMenu.setEnabled(true);
                scoresMenu.setEnabled(true);
                mainController.startGame();

            }
        });*/

        continueMenu.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("game continue");
                continueMenu.setEnabled(false);
                pauseMenu.setEnabled(true);
                stopMenu.setEnabled(true);
                scoresMenu.setEnabled(true);
                mainController.continueGame();

            }
        });

        pauseMenu.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("game pause");
                continueMenu.setEnabled(true);
                pauseMenu.setEnabled(false);
                mainController.pauseGame();


            }
        });
        stopMenu.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("game stop");
                startMenu.setEnabled(true);
                pauseMenu.setEnabled(false);
                stopMenu.setEnabled(false);
                scoresMenu.setEnabled(true);
                mainController.endGame(localPlayerWrapper.getPlayer());

            }
        });
        scoresMenu.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                try {
                    LinkedList<String> lines =new LinkedList();
                    String line=null;
                    BufferedReader br = new BufferedReader(new FileReader("src/game/scoreTable"));
                    while((line = br.readLine()) != null){
                        lines.add(line);
                    }
                    br.close();

                    JOptionPane.showMessageDialog(null, lines,"Scores",JOptionPane.INFORMATION_MESSAGE);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        inGameMenu.add(startMenu);
        inGameMenu.add(continueMenu);
        inGameMenu.add(pauseMenu);
        inGameMenu.add(stopMenu);
        inGameMenu.add(scoresMenu);

        frame.setJMenuBar(inGameMenu);


        mainController.setObserver(panel);
        frame.add(panel);
        frame.addKeyListener(panel);
        frame.setVisible(true);


        //frame.addKeyListener(panel);
    }

}

class MyStartActionListaner implements ActionListener {
    PlayerWrapper playerWrapper;
    JMenuItem startMenu;
    JMenuItem pauseMenu;
    JMenuItem stopMenu;
    JMenuItem scoresMenu;
    Controller mainController;
    FieldContainer fieldContainer;
    MainPanel panel;
    public MyStartActionListaner(PlayerWrapper playerWrapper,
                                 JMenuItem startMenu,
                                 JMenuItem pauseMenu,
                                 JMenuItem stopMenu,
                                 JMenuItem scoresMenu,
                                 Controller mainController,
                                 FieldContainer fieldContainer,
                                 MainPanel panel){
        this.playerWrapper = playerWrapper;
        this.startMenu = startMenu;
        this.pauseMenu= pauseMenu;
        this.stopMenu= stopMenu;
        this.scoresMenu = scoresMenu;
        this.mainController = mainController;
        this.fieldContainer = fieldContainer;
        this.panel = panel;
    }
    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        String playername  = JOptionPane.showInputDialog("Please input a player name");
        Player player=fieldContainer.generatePlayer(playername);
        playerWrapper.setPlayer(player);
        panel.setMyPlayer(player);
        startMenu.setEnabled(false);
        pauseMenu.setEnabled(true);
        stopMenu.setEnabled(true);
        scoresMenu.setEnabled(true);
        mainController.startGame();
    }
}
class PlayerWrapper{
    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    Player player;

}