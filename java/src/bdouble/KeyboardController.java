package bdouble;

import java.awt.event.KeyAdapter;

/**
 * Controller that receives input from keyboard events
 */
public class KeyboardController
    extends KeyAdapter
    implements Controller
{
    private Steering steering;
    private Transmission transmission;

    /**
     * Constructor
     */
    public KeyboardController() {
        super();
        this.steering = Steering.CENTER;
        this.transmission = Transmission.NEUTRAL;
    }

    @Override
    public Steering getSteeringInput() {
        return this.steering;
    }

    @Override
    public Transmission getTransmissionInput() {
        return this.transmission;
    }

}
