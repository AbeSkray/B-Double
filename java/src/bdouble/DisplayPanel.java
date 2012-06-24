package bdouble;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.LayoutManager;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class DisplayPanel extends JPanel {

    /**
     * UID for serialization
     */
    private static final long serialVersionUID = -1003207808361971057L;

    /**
     * Default Constructor
     */
    public DisplayPanel() {
        initialize();
    }

    /**
     * Initializing Constructor
     *
     * @param layout
     */
    public DisplayPanel(LayoutManager layout) {
        super(layout);
        initialize();
    }

    /**
     * Initializing Constructor
     *
     * @param isDoubleBuffered
     */
    public DisplayPanel(boolean isDoubleBuffered) {
        super(isDoubleBuffered);
        initialize();
    }

    /**
     * Initializing Constructor
     *
     * @param layout
     * @param isDoubleBuffered
     */
    public DisplayPanel(LayoutManager layout, boolean isDoubleBuffered) {
        super(layout, isDoubleBuffered);
        initialize();
    }

    /**
     * Helper method to setup components.<p>
     * Should only be called by constructors
     */
    protected void initialize() {
        this.setBackground(Color.WHITE);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // try drawing a rectangle
        int x = 30;
        int y = 50;
        final int width = 80;
        final int height = 120;
        g.setColor(Color.GREEN);
        g.fill3DRect(x, y, width, height, true);
    }

    /**
     * Test Main
     *
     * @param args
     */
    public static void main(String[] args) {
        JFrame frame = new JFrame("DisplayPanel Test");
        frame.setBounds(0, 0, 600, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        DisplayPanel display = new DisplayPanel();
        frame.add(display);
        frame.setVisible(true);
    }

}
