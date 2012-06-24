package bdouble;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.LayoutManager;

import javax.swing.JPanel;

/**
 * Test view to display input commands from game pad.
 *
 * @author aknowles
 */
public class InputViewPanel extends JPanel {

    /**
     * Default Constructor
     */
    public InputViewPanel() {
        initialize();
        // TODO Auto-generated constructor stub
    }

    /**
     * Initializing Constructor
     *
     * @param layout
     */
    public InputViewPanel(LayoutManager layout) {
        super(layout);
        initialize();
        // TODO Auto-generated constructor stub
    }

    /**
     * Initializing Constructor
     *
     * @param isDoubleBuffered
     */
    public InputViewPanel(boolean isDoubleBuffered) {
        super(isDoubleBuffered);
        initialize();
        // TODO Auto-generated constructor stub
    }

    /**
     * Initializing Constructor
     *
     * @param layout
     * @param isDoubleBuffered
     */
    public InputViewPanel(LayoutManager layout, boolean isDoubleBuffered) {
        super(layout, isDoubleBuffered);
        initialize();
        // TODO Auto-generated constructor stub
    }

    /**
     * Helper method called by constructors
     */
    private void initialize() {
        this.setLayout(new GridBagLayout());
        GridBagConstraints c = null;

        // Steering view

        // Throttle view

        // Transmission view
    }

    /**
     * Test Main
     *
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub

    }

}
