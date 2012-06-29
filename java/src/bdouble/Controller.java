package bdouble;

/**
 * This defines a Controller interface.
 * In particular, this outlines what a Model can expect
 * from a Controller.
 */
public interface Controller {
    
    /**
     * Steering input values
     */
    public enum Steering {
        LEFT_1,
        CENTER,
        RIGHT_1
    };

    /**
     * Transmission input values
     */
    public enum Transmission {
        REVERSE_1,
        NEUTRAL,
        DRIVE_1
    };

    /**
     * Get the current steering input state
     */
    public Steering getSteeringInput();

    /**
     * Get the current transmission input state
     */
    public Transmission getTransmissionInput();
}
