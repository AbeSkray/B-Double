package bdouble;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
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

        // get a Graphics2D reference
        Graphics2D g2 = (Graphics2D) g;

        // translate the rectangle to display center
        final int dx = this.getWidth() / 2;
        final int dy = this.getHeight() / 2;
        g2.translate(dx, dy);

        // draw rectangle around the origin
        final int rectWidth = 80;
        final int rectHeight = 120;
        final int rectX = 0 - rectWidth / 2;
        final int rectY = 0 - rectHeight / 2;
        g2.setColor(Color.GREEN);
        g2.fill3DRect(rectX, rectY, rectWidth, rectHeight, true);

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
