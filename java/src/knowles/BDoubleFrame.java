package knowles;

import java.awt.GraphicsConfiguration;
import javax.swing.JFrame;

public class BDoubleFrame extends JFrame {

	// Constructors
	
	/**
	 * Default Constructor
	 */
	public BDoubleFrame() {
		super();
		initialize();
	}
	
	/**
	 * Initializing Constructor
	 * @param gc
	 */
	public BDoubleFrame(GraphicsConfiguration gc) {
		super(gc);
		initialize();
	}
	
	/**
	 * Initializing Constructor
	 * @param title
	 */
	public BDoubleFrame(String title) {
		super(title);
		initialize();
	}
	
	/**
	 * Initializing Constructor
	 * @param title
	 * @param gc
	 */
	public BDoubleFrame(String title, GraphicsConfiguration gc) {
		super(title, gc);
		initialize();
	}
	
	/**
	 * GUI initialization helper -- only call from a constructor
	 */
	private void initialize() {
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setBounds(0, 0, 400, 300);
		this.pack();
	}
	
}
