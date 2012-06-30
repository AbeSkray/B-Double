package bdouble;

import java.awt.Rectangle;

/**
 * Interface for a B-Double Truck model.
 * This is what the View expects from the Model.
 */
public interface Model {

    /**
     * Get a bounding box around the cab of the truck.
     *
     * @return a {@link Rectangle} with upper-left corner at (x, y)
     */
    public Rectangle getCabBoundingBox();

    /**
     * Compass heading of the cab.<br>
     * 0 degrees is North, 90 degrees is East, etc...
     *
     * @return  compass heading in degrees
     */
    public double getCabHeading();

    /**
     * Get a bounding box around the "A" Trailer.
     *
     * @return a {@link Rectangle} with upper-left corner at (x, y)
     */
    public Rectangle getTrailerABoundingBox();

    /**
     * Compass heading of the "A" Trailer.<br>
     * 0 degrees is North, 90 degrees is East, etc...
     *
     * @return  compass heading in degrees
     */
    public double getTrailerAHeading();

    /**
     * Get a bounding box around the "B" Trailer.
     *
     * @return a {@link Rectangle} with upper-left corner at (x, y)
     */
    public Rectangle getTrailerBBoundingBox();

    /**
     * Compass heading of the "B" Trailer.<br>
     * 0 degrees is North, 90 degrees is East, etc...
     *
     * @return  compass heading in degrees
     */
    public double getTrailerBHeading();
}
