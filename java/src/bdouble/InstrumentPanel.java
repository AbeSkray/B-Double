package bdouble;

import java.awt.GridLayout;
import java.awt.LayoutManager;

import javax.swing.JButton;
import javax.swing.JPanel;

/**
 * View to display truck speed, transmission status, etc...
 * 
 * @author Alex Knowles
 */
public class InstrumentPanel extends JPanel {

	/**
	 * UID for serialization
	 */
	private static final long serialVersionUID = 7360404245516556855L;

	/**
	 * Default Constructor
	 */
	public InstrumentPanel() {
		initialize();
	}

	/**
	 * Initializing Constructor
	 * @param layout
	 */
	public InstrumentPanel(LayoutManager layout) {
		super(layout);
		initialize();
	}

	/**
	 * Initializing Constructor
	 * @param isDoubleBuffered
	 */
	public InstrumentPanel(boolean isDoubleBuffered) {
		super(isDoubleBuffered);
		initialize();
	}

	/**
	 * Initializing Constructor
	 * @param layout
	 * @param isDoubleBuffered
	 */
	public InstrumentPanel(LayoutManager layout, boolean isDoubleBuffered) {
		super(layout, isDoubleBuffered);
		initialize();
	}
	
	/**
	 * Helper method to setup components
	 * Should only be called by constructors
	 */
	protected void initialize() {
		this.setLayout(new GridLayout(1, 3));
		this.add(new JButton("1"));
		this.add(new JButton("2"));
		this.add(new JButton("3"));
		this.add(new JButton("4"));
		this.add(new JButton("5"));
		this.add(new JButton("6"));
	}

}
