package bdouble;

import java.awt.FlowLayout;
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

    }

    /**
     * Test Main
     *
     * @param args
     */
    public static void main(String[] args) {
        JFrame frame = new JFrame("DisplayPanel Test");
        frame.setLayout(new FlowLayout());
        frame.setBounds(0, 0, 600, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.add(new DisplayPanel());
        frame.setVisible(true);
    }

}
