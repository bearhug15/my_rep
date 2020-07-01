package graphic;

import game.Controller;
import game.MyObserver;
import game.MyPair;
import moving_objects.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.AbstractCollection;
import java.util.ListIterator;
import java.util.Vector;

public class MainPanel extends JPanel implements MyObserver,KeyListener {
    MyPair firstPoint,secondPoint;
    PaintPurpose purpose = PaintPurpose.paintOther;
    Graphics graphics;
    //FieldContainer fieldContainer;
    final Controller controller;
    Player myPlayer;
    Boolean is_moving = true;
    public MainPanel(Controller controller/*FieldContainer fieldContainer*/){
        this.controller = controller;
    }
    public void setMyPlayer(Player player){
        myPlayer = player;
        controller.addPlayer(player);
    }
    @Override
    public void update(AbstractCollection<? extends AbstractCollection<? extends MovingObject>> data) {
        synchronized (is_moving) {
            purpose = PaintPurpose.paintLine;
            ListIterator pointsIterator;
            Graphics graph = this.getGraphics();
            super.paintComponent(graph);
            for (AbstractCollection i : data) {
                synchronized (i) {
                    for (Object g : i) {
                        Vector<MyPair> figure = ((MovingObject) g).getPoints();
                        if (figure == null) {
                            System.out.println(((MovingObject) g).getName());
                        }
                        pointsIterator = figure.listIterator();
                        firstPoint = (MyPair) pointsIterator.next();
                        while (pointsIterator.hasNext()) {
                            secondPoint = (MyPair) pointsIterator.next();
                            graph.drawLine(firstPoint.getKey(), firstPoint.getValue(), secondPoint.getKey(), secondPoint.getValue());
                            firstPoint = secondPoint;
                        }
                    }
                }
            }
            Vector<MyPair> figure = myPlayer.getPoints();
            pointsIterator = figure.listIterator();
            firstPoint = (MyPair) pointsIterator.next();
            if (figure == null) {
                System.out.println(myPlayer.getPlayerName() + " dead");
            }
            while (pointsIterator.hasNext()) {
                secondPoint = (MyPair) pointsIterator.next();
                graph.drawLine(firstPoint.getKey(), firstPoint.getValue(), secondPoint.getKey(), secondPoint.getValue());
                firstPoint = secondPoint;
            }
        }
    }

    @Override
    public void keyTyped(KeyEvent keyEvent) {
        /*if(keyEvent.getExtendedKeyCode() == 27){
            //TODO open menu
        }
        System.out.println((int)keyEvent.getKeyChar());*/
    }

    @Override
    public void keyPressed(KeyEvent keyEvent) {
        synchronized (is_moving) {
            switch (keyEvent.getExtendedKeyCode()) {
                case 37:
                    controller.moveLeft(myPlayer);
                    System.out.println("Left");
                    //fieldContainer.movePlayer(myPlayer,0,-1);
                    break;
                case 38:
                    controller.moveUp(myPlayer);
                    System.out.println("Up");
                    //fieldContainer.movePlayer(myPlayer,-1,0);
                    break;
                case 39:
                    controller.moveRight(myPlayer);
                    System.out.println("Right");
                    //fieldContainer.movePlayer(myPlayer,0,1);
                    break;
                case 40:
                    controller.moveDown(myPlayer);
                    System.out.println("Down");
                    //fieldContainer.movePlayer(myPlayer,1,0);
                    break;
                default:
            }
        }
    }

    @Override
    public void keyReleased(KeyEvent keyEvent) {

    }


}
enum PaintPurpose{
    paintLine,
    paintOther;
}
