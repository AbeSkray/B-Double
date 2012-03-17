package bdouble;

import java.awt.Component;
import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import net.miginfocom.swing.MigLayout;

import com.codeminders.hidapi.HIDDevice;
import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDDeviceNotFoundException;
import com.codeminders.hidapi.HIDManager;

public class HidControlPanel extends JPanel
	implements ActionListener, ListSelectionListener
{

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
	
	// Member variables GUI components
	private JList deviceListView;
	private DefaultListModel deviceListModel;
	private JButton listButton;
	private JButton testButton;
	private JButton clearButton;
	private JButton clearSelButton;
	private JButton readDevButton;
	private JTextArea textArea;
	
	// Member variables
	private HIDDevice hidDevice;
	boolean isReadingDevice;
	
	private void initialize() {
		isReadingDevice = false;
		this.setLayout(new MigLayout());
		
		// initialize JList
		deviceListModel = new DefaultListModel();
		deviceListView = new JList();
		deviceListView.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		deviceListView.setModel(deviceListModel);
		deviceListView.setCellRenderer(new DeviceListCellRenderer());
		deviceListView.addListSelectionListener(this);
		
		// initialize components
		listButton = new JButton("List Devices");
		listButton.addActionListener(this);
		testButton = new JButton("Test");
		testButton.addActionListener(this);
		clearButton = new JButton("Clear");
		clearButton.addActionListener(this);
		clearSelButton = new JButton("Clear Selection");
		clearSelButton.addActionListener(this);
		readDevButton = new JButton("Read Device");
		readDevButton.addActionListener(this);
		textArea = new JTextArea("sample text");
		
		// arrange components
		this.add(listButton);
		this.add(testButton, "wrap");
		this.add(clearButton);
		this.add(clearSelButton, "wrap");
		this.add(readDevButton, "wrap");
		this.add(deviceListView, "span 4");
		this.add(textArea, "span 3");
	}
	
	protected class DeviceListCellRenderer extends DefaultListCellRenderer
		implements ListCellRenderer
	{
		public Component getListCellRendererComponent(
				JList list,              // the list
				Object value,            // value to display
				int index,               // cell index
				boolean isSelected,      // is the cell selected
				boolean cellHasFocus)    // does the cell have focus)
		{
			Component result = super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
			if (value instanceof HIDDeviceInfo) {
				HIDDeviceInfo deviceInfo = (HIDDeviceInfo) value;
				String s = deviceInfo.getProduct_string();
				((JLabel) result).setText(s);
			}
			return result;
		}		
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
					HIDDeviceInfo deviceInfo = i.next();
					deviceListModel.addElement(deviceInfo);
					System.out.println(deviceInfo.getProduct_string());
				}
			} catch (IOException ioException) {
				ioException.printStackTrace();				
			}
		} else if (source == this.clearButton) {
			deviceListModel.clear();
		} else if (source == this.clearSelButton) {
			deviceListView.clearSelection();
		} else if (source == this.readDevButton) {
			toggleReadDevice();
		}
	}
	
	private void toggleReadDevice()	{
		if (isReadingDevice == false) {
			// attempt to start reading from device
			Object selectedValue = deviceListView.getSelectedValue();
			if (selectedValue == null) {
				return;
			}
			if (selectedValue instanceof HIDDeviceInfo) {
				HIDDeviceInfo devInfo = (HIDDeviceInfo) selectedValue;
				try {
					hidDevice = HIDManager.openById(devInfo.getVendor_id(), devInfo.getProduct_id(), devInfo.getSerial_number());
				} catch (HIDDeviceNotFoundException ex) {
					System.err.println("Device not found!");
					ex.printStackTrace();
					return;
				} catch (IOException ex) {
					ex.printStackTrace();
					return;
				}
				// TODO: start reading here
				byte[] buffer = new byte[256];
				isReadingDevice = true;
				try {
					int retVal = 0;
					hidDevice.disableBlocking();
					System.out.println("About to make read call...");
					retVal = hidDevice.read(buffer);
					System.out.println("Returned from read call; returned " + retVal);
					this.textArea.setText(buffer.toString());
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		} else {
			// stop reading from device
			if (hidDevice != null) {
				try {
					hidDevice.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
				isReadingDevice = false;
				hidDevice = null;
			}
		}
	}
	
	@Override
	public void valueChanged(ListSelectionEvent e) {
		// TODO
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// set up JFrame
		JFrame frame = new JFrame("HID Control Panel Test");
		HidControlPanel devicePanel = new HidControlPanel();
		frame.setContentPane(devicePanel);
		frame.setSize(800, 600);
		frame.setVisible(true);
		
	}

}
