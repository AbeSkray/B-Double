package bdouble;

import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.LayoutManager;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSlider;

/**
 * Controls for debugging
 * Takes user input controls in place of a game controller
 * @author aknowles
 */
public class TestControlPanel extends JPanel {

	/**
	 * Default Constructor
	 */
	public TestControlPanel() {
		initialize();
	}

	/**
	 * @param layout
	 */
	public TestControlPanel(LayoutManager layout) {
		super(layout);
		initialize();
	}

	/**
	 * @param isDoubleBuffered
	 */
	public TestControlPanel(boolean isDoubleBuffered) {
		super(isDoubleBuffered);
		initialize();
	}

	/**
	 * @param layout
	 * @param isDoubleBuffered
	 */
	public TestControlPanel(LayoutManager layout, boolean isDoubleBuffered) {
		super(layout, isDoubleBuffered);
		initialize();
	}

	/**
	 * Helper method called by constructor to set up the panel components
	 */
	private void initialize() {
		this.setLayout(new GridBagLayout());
		GridBagConstraints c = null;
		
		// Steering control
		JSlider steeringControl = new JSlider(-10, 10);
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = GridBagConstraints.REMAINDER;
		c.insets = new Insets(20, 20, 0, 0);
		this.add(steeringControl, c);
		
		// Throttle control
		JCheckBox throttleControl = new JCheckBox("Throttle");
		c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 1;
		c.gridy = 1;
		c.insets = new Insets(20, 20, 0, 0);
		this.add(throttleControl, c);

		// Transmission control
		JButton transmissionControl = new JButton("Shift");
		c = new GridBagConstraints();
		c.gridx = 1;
		c.gridy = 2;
		c.insets = new Insets(20, 20, 0, 0);
		this.add(transmissionControl, c);
	}

	/**
	 * Test Main
	 * @param args
	 */
	public static void main(String[] args) {
		JFrame frame = new JFrame("TestControlPanel Test");
		frame.setLayout(new FlowLayout());
		frame.setBounds(0, 0, 300, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		frame.add(new TestControlPanel());
		frame.setVisible(true);
	}

}
