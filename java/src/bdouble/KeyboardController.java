package bdouble;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import bdouble.Controller.Steering;

/**
 * Controller that receives input from keyboard events
 */
public class KeyboardController
    extends KeyAdapter
    implements Controller
{

    private Transmission transmission;

    private boolean leftKeyIsDown = false;
    private boolean rightKeyIsDown = false;

    /**
     * Constructor
     */
    public KeyboardController() {
        super();
        this.transmission = Transmission.NEUTRAL;
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
        return this.transmission;
    }

    @Override
    public void keyPressed(KeyEvent e) {
        final int keyCode = e.getKeyCode();

        if ( (keyCode & KeyEvent.KEY_LOCATION_LEFT) 
                == KeyEvent.KEY_LOCATION_LEFT)
        {
            this.leftKeyIsDown = true;
        }
        if ( (keyCode & KeyEvent.KEY_LOCATION_RIGHT)
                == KeyEvent.KEY_LOCATION_RIGHT)
        {
            this.rightKeyIsDown = true;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        final int keyCode = e.getKeyCode();

        if ( (keyCode & KeyEvent.KEY_LOCATION_LEFT) 
                == KeyEvent.KEY_LOCATION_LEFT)
        {
            this.leftKeyIsDown = false;
        }
        if ( (keyCode & KeyEvent.KEY_LOCATION_RIGHT)
                == KeyEvent.KEY_LOCATION_RIGHT)
        {
            this.rightKeyIsDown = false;
        }
    }
}
