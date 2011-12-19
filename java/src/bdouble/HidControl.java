/**
 * Object to handle user input from a Gamepad or other HID
 * 
 * @author aknowles
 */
package bdouble;

import java.io.IOException;

import com.codeminders.hidapi.*;

public class HidControl {

	/**
	 * Static call to load native library for hidapi (JNI)
	 */
    static
    {
        System.loadLibrary("hidapi-jni");
    }	
	
	/**
	 * Test Main
	 * @param args
	 */
	public static void main(String[] args) {
		String property = System.getProperty("java.library.path");
		System.out.println(property);
		
		HIDDeviceInfo[] deviceInfos;
		try {
			deviceInfos = HIDManager.listDevices();
			
			// print each device info
			for (int i = 0; i < deviceInfos.length; ++i) {
				System.out.println(deviceInfos[i]);
			}
		} catch (IOException ioe) {
			System.err.println("Exception thrown");
			ioe.printStackTrace();
		}
	}
}