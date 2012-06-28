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
     * Scalar for scaling the world view
     */
    private double scaleFactor;

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

    private void setScaleFactor(double scaleFactor) {
        assert(scaleFactor > 0);
        this.scaleFactor = scaleFactor;
    }

    private enum ZoomLevel {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE
    }

    private ZoomLevel zoomLevel;

    private void setZoomLevel(ZoomLevel zoomLevel) {
        this.zoomLevel = zoomLevel;
        switch (zoomLevel) {
        case ONE:
            setScaleFactor(1.0);
            break;
        case TWO:
            setScaleFactor(0.8);
            break;
        case THREE:
            setScaleFactor(0.5);
            break;
        case FOUR:
            setScaleFactor(0.25);
            break;
        case FIVE:
            setScaleFactor(0.1);
            break;
        }
    }

    public void zoomIn() {
        switch (zoomLevel) {
        case FIVE:
            setZoomLevel(ZoomLevel.FOUR);
            break;
        case FOUR:
            setZoomLevel(ZoomLevel.THREE);
            break;
        case THREE:
            setZoomLevel(ZoomLevel.TWO);
            break;
        case TWO:
            setZoomLevel(ZoomLevel.ONE);
            break;
        case ONE:
        default:
            // do nothing
        }
    }

    public void zoomOut() {
        switch (zoomLevel) {
        case ONE:
            setZoomLevel(ZoomLevel.TWO);
            break;
        case TWO:
            setZoomLevel(ZoomLevel.THREE);
            break;
        case THREE:
            setZoomLevel(ZoomLevel.FOUR);
            break;
        case FOUR:
            setZoomLevel(ZoomLevel.FIVE);
            break;
        case FIVE:
        default:
            // do nothing
        }
    }

    /**
     * Helper method to setup components.<p>
     * Should only be called by constructors
     */
    protected void initialize() {
        truckPosition = new Point(0, 0);
        scaleFactor = 1.0;
        zoomLevel = ZoomLevel.ONE;
        this.setBackground(Color.WHITE);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // get a Graphics2D reference
        Graphics2D g2 = (Graphics2D) g;

        // set the clipping area
        g2.setClip(0, 0, this.getWidth(), this.getHeight());

        // save the original transform
        final AffineTransform initialTransform = g2.getTransform();

        // create a truck transform
        // translate the origin to the center of display area
        // flip the y-axis so north is up
        // apply scale factor controlled by zoom
        final int dx = this.getWidth() / 2;
        final int dy = this.getHeight() / 2;
        AffineTransform truckTansform = new AffineTransform(initialTransform);
        truckTansform.translate(dx, dy);
        truckTansform.scale(scaleFactor, -scaleFactor);
        g2.setTransform(truckTansform);

        // create a world transform
        // translate the world relative to the truck's position
        AffineTransform worldTransform = new AffineTransform(truckTansform);
        worldTransform.translate(-this.truckPosition.getX(), -this.truckPosition.getY());

        // draw the world
        g2.setTransform(worldTransform);
        paintWorld(g2);

        // draw the truck
        g2.setTransform(truckTansform);
        paintTruck(g2);

        // restore original transform
        g2.setTransform(initialTransform);
    }

    /**
     * Helper method called by {@link paintComponent}
     *
     * @param g2
     */
    private void paintWorld(Graphics2D g2) {
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
    }

    /**
     * Helper method called by {@link paintComponent}
     *
     * @param g2
     */
    private void paintTruck(Graphics2D g2) {
        // draw truck around the origin
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
        buttonPanel.setPreferredSize(new Dimension(120, 120));
        frame.add(buttonPanel, BorderLayout.EAST);

        // Add button to start/stop animation
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

        // Add buttons to control zoom
        JButton zoomInButton = new JButton("Zoom In");
        JButton zoomOutButton = new JButton("Zoom Out");
        zoomInButton.setPreferredSize(new Dimension(110, 30));
        zoomOutButton.setPreferredSize(new Dimension(110, 30));
        ActionListener zoomInListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent ae) {
                display.zoomIn();
                display.repaint();
            }
        };
        zoomInButton.addActionListener(zoomInListener);
        ActionListener zoomOutListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent ae) {
                display.zoomOut();
                display.repaint();
            }
        };
        zoomOutButton.addActionListener(zoomOutListener);
        buttonPanel.add(zoomInButton);
        buttonPanel.add(zoomOutButton);

        frame.setVisible(true);
    }

}
