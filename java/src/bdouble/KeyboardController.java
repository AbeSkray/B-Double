package bdouble;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.Timer;

/**
 * Controller that receives input from keyboard events
 */
public class KeyboardController
    extends KeyAdapter
    implements Controller
{
    private boolean driveKeyIsDown = false;
    private boolean reverseKeyIsDown = false;

    private boolean leftKeyIsDown = false;
    private boolean rightKeyIsDown = false;

    /**
     * Constructor
     */
    public KeyboardController() {
        super();
    }

    @Override
    public Steering getSteeringInput() {
        Steering result = Steering.CENTER;
        if (leftKeyIsDown && !rightKeyIsDown) {
            result = Steering.LEFT_1;
        } else if (rightKeyIsDown && !leftKeyIsDown) {
            result = Steering.RIGHT_1;
        }
        return result;
    }

    @Override
    public Transmission getTransmissionInput() {
        Transmission result = Transmission.NEUTRAL;
        if (driveKeyIsDown && !reverseKeyIsDown) {
            result = Transmission.DRIVE_1;
        } else if (reverseKeyIsDown && !driveKeyIsDown) {
            result = Transmission.REVERSE_1;
        }
        return result;
    }

    @Override
    public void keyPressed(KeyEvent e) {
        final int keyCode = e.getKeyCode();

        switch (keyCode) {
        case KeyEvent.VK_LEFT:
        case KeyEvent.VK_A:
            this.leftKeyIsDown = true;
            e.consume();
            break;
        case KeyEvent.VK_RIGHT:
            this.rightKeyIsDown = true;
            e.consume();
        case KeyEvent.VK_D:
            break;
        case KeyEvent.VK_UP:
        case KeyEvent.VK_W:
            this.driveKeyIsDown = true;
            e.consume();
            break;
        case KeyEvent.VK_DOWN:
        case KeyEvent.VK_S:
            this.reverseKeyIsDown = true;
            e.consume();
            break;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        final int keyCode = e.getKeyCode();

        switch (keyCode) {
        case KeyEvent.VK_LEFT:
        case KeyEvent.VK_A:
            this.leftKeyIsDown = false;
            e.consume();
            break;
        case KeyEvent.VK_RIGHT:
            this.rightKeyIsDown = false;
            e.consume();
        case KeyEvent.VK_D:
            break;
        case KeyEvent.VK_UP:
        case KeyEvent.VK_W:
            this.driveKeyIsDown = false;
            e.consume();
            break;
        case KeyEvent.VK_DOWN:
        case KeyEvent.VK_S:
            this.reverseKeyIsDown = false;
            e.consume();
            break;
        }
    }

    /**
     * Test Main
     * @param args
     */
    public static void main(String[] args) {
        // set up a frame
        JFrame frame = new JFrame("KeyboardController Test");
        frame.setLayout(new FlowLayout());
        frame.setBounds(0, 0, 300, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // add in a crude view
        final JTextField steeringView = new JTextField("Steering");
        final JTextField transmissionView = new JTextField("Transmission");
        steeringView.setFocusable(false);
        transmissionView.setFocusable(false);
        steeringView.setPreferredSize(new Dimension(100, 30));
        transmissionView.setPreferredSize(new Dimension(100, 30));
        frame.add(steeringView);
        frame.add(transmissionView);

        // wire in a keyboard controller
        final KeyboardController keyboardController = new KeyboardController();
        KeyboardFocusManager focusManager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        KeyEventDispatcher dispatcher = new KeyEventDispatcher() {
            @Override
            public boolean dispatchKeyEvent(KeyEvent e) {
                if (e.getID() == KeyEvent.KEY_PRESSED) {
                    keyboardController.keyPressed(e);
                } else if (e.getID() == KeyEvent.KEY_RELEASED) {
                    keyboardController.keyReleased(e);
                }
                return e.isConsumed();
            }
        };
        focusManager.addKeyEventDispatcher(dispatcher);
        frame.getContentPane().addKeyListener(keyboardController);

        // set up a driver
        final int delayMs = 2000;
        final Controller controller = keyboardController;
        ActionListener timerListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent arg0) {
                final Steering steering = controller.getSteeringInput();
                switch (steering) {
                case LEFT_1:
                    steeringView.setText("Steer left");
                    break;
                case RIGHT_1:
                    steeringView.setText("Steer right");
                    break;
                case CENTER:
                default:
                    steeringView.setText("Steer center");
                }

                final Transmission transmission = controller.getTransmissionInput();
                switch (transmission) {
                case REVERSE_1:
                    transmissionView.setText("R");
                    break;
                case DRIVE_1:
                    transmissionView.setText("D");
                    break;
                case NEUTRAL:
                    transmissionView.setText("N");
                    break;
                }
            }
        };
        Timer governor = new Timer(delayMs, timerListener);
        governor.start();

        frame.setVisible(true);
    }
}
