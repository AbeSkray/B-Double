package bdouble;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GraphicsConfiguration;

import javax.swing.JButton;
import javax.swing.JFrame;

public class BDoubleFrame extends JFrame {

	// Constructors
	
	/**
	 * UID for serialization
	 */
	private static final long serialVersionUID = 1431124527015618316L;

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
		this.setBounds(0, 0, 600, 400);
		//this.pack();
		
		this.setLayout(new BorderLayout());
		//this.add(new JButton("1"), BorderLayout.PAGE_START);
		InstrumentPanel instrument1 = new InstrumentPanel();
		instrument1.setPreferredSize(new Dimension(200, 400));
		this.add(new InstrumentPanel(), BorderLayout.PAGE_END);
		//this.add(new JButton("3"), BorderLayout.WEST);
		//this.add(new JButton("4"), BorderLayout.EAST);
		//this.add(new JButton("5"), BorderLayout.CENTER);
	}
	
}
