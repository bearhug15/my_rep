package graphic;

import javax.swing.*;
import java.awt.*;

public class BackPanel extends JPanel {
    Image img ;
    JFrame frame;
    public BackPanel(JFrame frame){
        this.frame = frame;
    }

    @Override
    public void paint(Graphics g) {
        g.drawImage(img,0,0,frame.getWidth(),frame.getHeight(),null);
    }

    @Override
    protected void paintComponent(Graphics g) {
        Image img = new ImageIcon("src/graphic/back.bmp").getImage();
        g.drawImage(img,0,0,null);
        //super.paintComponent(g);
    }
}
