package bdouble;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class BDoubleApp implements Runnable {

    @Override
    public void run() {
        BDoubleFrame frame = new BDoubleFrame("B-Double");
        frame.setVisible(true);
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new BDoubleApp());
    }
}
