package bdouble;

import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JPanel;

import net.miginfocom.swing.MigLayout;

import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDManager;

public class HidControlPanel extends JPanel implements ActionListener {

	/**
	 * Serialization
	 */
	private static final long serialVersionUID = 196848833536050999L;

	static {
		// load library for HID
		System.loadLibrary("hidapi-jni");
	}

	public HidControlPanel() {
		super();
		initialize();
	}

	public HidControlPanel(LayoutManager layout) {
		super(layout);
		initialize();
	}

	public HidControlPanel(boolean isDoubleBuffered) {
		super(isDoubleBuffered);
		initialize();
	}

	public HidControlPanel(LayoutManager layout, boolean isDoubleBuffered) {
		super(layout, isDoubleBuffered);
		initialize();
	}
	
	// Member variables
	private JList deviceListView;
	private DefaultListModel deviceListModel;
	private JButton listButton;
	private JButton testButton;
	private JButton clearButton;
	private JButton clearSelButton;
	
	private void initialize() {
		this.setLayout(new MigLayout());
		
		// initialize JList
		Integer[] ints = new Integer[] {1, 2, 3, 4};
		//List<Integer> intList = java.util.Arrays.asList(ints);
		//JList deviceListView = new JList(ints);
		deviceListModel = new DefaultListModel();
		deviceListView = new JList();
		deviceListView.setModel(deviceListModel);
		
		// initialize components
		listButton = new JButton("List Devices");
		listButton.addActionListener(this);
		testButton = new JButton("Test");
		testButton.addActionListener(this);
		clearButton = new JButton("Clear");
		clearButton.addActionListener(this);
		clearSelButton = new JButton("Clear Selection");
		
		// arrange components
		this.add(listButton);
		this.add(testButton, "wrap");
		this.add(clearButton);
		this.add(clearSelButton, "wrap");
		this.add(deviceListView, "span 4");
	}

	public void actionPerformed(ActionEvent actionEvent) {
		Object source = actionEvent.getSource();
		if (source == this.testButton) {
			deviceListModel.addElement(new Integer(deviceListModel.getSize() + 1));
		} else if (source == this.listButton) {
			// button pressed: list devices
			try {
				HIDDeviceInfo[] deviceInfos = HIDManager.listDevices();
				List<HIDDeviceInfo> list = Arrays.asList(deviceInfos);
				for (Iterator<HIDDeviceInfo> i = list.iterator(); i.hasNext(); ) {
					deviceListModel.addElement(i.next());
				}
			} catch (IOException ioException) {
				ioException.printStackTrace();				
			}
		} else if (source == this.clearButton) {
			deviceListModel.clear();
		} else if (source == this.clearSelButton) {
			deviceListView.clearSelection();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// set up JFrame
		JFrame frame = new JFrame("HID Control Panel Test");
		HidControlPanel devicePanel = new HidControlPanel();
		frame.setContentPane(devicePanel);
		frame.setSize(400, 300);
		frame.setVisible(true);
		
	}

}
