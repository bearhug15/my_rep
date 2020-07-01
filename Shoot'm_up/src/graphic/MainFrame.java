package graphic;

import javax.swing.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class MainFrame extends JFrame implements KeyListener {

    public MainFrame(){
        this.addKeyListener(this);
        setSize(200,200);
        setLayout(null);
        setVisible(true);
    }

    @Override
    public void keyTyped(KeyEvent keyEvent) {
        System.out.println("Typed");
    }

    @Override
    public void keyPressed(KeyEvent keyEvent) {
        System.out.println("Pressed");
    }

    @Override
    public void keyReleased(KeyEvent keyEvent) {
        System.out.println("Released");
    }
}
