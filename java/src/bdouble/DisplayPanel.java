package bdouble;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.LayoutManager;
import java.awt.Point;
import java.awt.Stroke;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.AffineTransform;
import java.awt.geom.Point2D;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;

public class DisplayPanel extends JPanel {

    /**
     * UID for serialization
     */
    private static final long serialVersionUID = -1003207808361971057L;

    /**
     * Demo: location of truck relative to origin
     */
    private Point truckPosition;

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

    public Point getTruckPosition() {
        return truckPosition;
    }

    public void setTruckPosition(Point truckPosition) {
        this.truckPosition = truckPosition;
    }

    public void setTruckPosition(int x, int y) {
        setTruckPosition(new Point(x, y));
    }

    /**
     * Helper method to setup components.<p>
     * Should only be called by constructors
     */
    protected void initialize() {
        truckPosition = new Point(0, 0);
        this.setBackground(Color.WHITE);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // get a Graphics2D reference
        Graphics2D g2 = (Graphics2D) g;

        // set the clipping area
        g2.setClip(0, 0, this.getWidth(), this.getHeight());

        // save the transform
        final AffineTransform transform0 = g2.getTransform();

        // translate the origin to display center and invert it
        final int dx = this.getWidth() / 2;
        final int dy = this.getHeight() / 2;
        AffineTransform transform1 = new AffineTransform(transform0);
        transform1.translate(dx, dy);
        transform1.scale(1, -1);
        g2.setTransform(transform1);

        // translate the road relative to the "truck"
        AffineTransform transform2 = new AffineTransform(transform1);
        transform2.translate(-this.truckPosition.getX(), -this.truckPosition.getY());
        g2.setTransform(transform2);

        // draw some "road markers"
        Stroke roadStroke = new BasicStroke(10);
        g2.setStroke(roadStroke);
        g2.setColor(Color.BLACK);
        final int roadLeft = -150;
        final int roadRight = 150;
        final int roadStart = -500;
        final int roadEnd = 500;
        g2.drawLine(roadLeft, roadStart, roadLeft, roadEnd);
        g2.drawLine(roadRight, roadStart, roadRight, roadEnd);
        g2.setColor(Color.YELLOW);
        final int dividerLength = 35;
        final int dividerSpace = 50;
        for (int i = roadStart; i < roadEnd; ) {
            // draw median
            int j = i + dividerLength;
            g2.drawLine(0, i, 0, j);
            i = j + dividerSpace;
        }

        // draw rectangle around the origin
        g2.setTransform(transform1);
        final int rectWidth = 80;
        final int rectHeight = 120;
        final int rectX = 0 - rectWidth / 2;
        final int rectY = 0 - rectHeight / 2;
        g2.setColor(Color.GREEN);
        g2.fill3DRect(rectX, rectY, rectWidth, rectHeight, true);

        // restore original transform
        g2.setTransform(transform0);
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
        frame.setLayout(new BorderLayout());

        final DisplayPanel display = new DisplayPanel();
        frame.add(display, BorderLayout.CENTER);

        // Demo: make a timer to demo animation
        final int delayMs = 100;
        ActionListener timerListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent ae) {
                Point truckPosition = display.getTruckPosition();
                final int x = (int) truckPosition.getX();
                final int y = (int) truckPosition.getY() + 5;
                display.setTruckPosition(x, y);
                display.repaint();
            }
        };
        final Timer demoTimer = new Timer(delayMs, timerListener);

        // Add a button panel
        JPanel buttonPanel = new JPanel();
        buttonPanel.setPreferredSize(new Dimension(100, 100));
        final JButton goButton = new JButton("Start");
        ActionListener goButtonListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent ae) {
                JButton button = (JButton) ae.getSource();
                if (demoTimer.isRunning()) {
                    // stop!
                    demoTimer.stop();
                    button.setText("Start");
                } else {
                    // go!
                    demoTimer.start();
                    button.setText("Stop");
                }
            }
        };
        goButton.addActionListener(goButtonListener);
        buttonPanel.add(goButton);
        frame.add(buttonPanel, BorderLayout.EAST);
        frame.setVisible(true);
    }

}
