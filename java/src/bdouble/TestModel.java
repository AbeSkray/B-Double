package bdouble;

import java.awt.Point;
import java.awt.Rectangle;

class TestModel implements Model {
    private Rectangle cabBoundingBox = new Rectangle();
    private double cabHeading;
    
    public TestModel() {
        cabBoundingBox = new Rectangle(-40, 0, 80, 120);
        cabHeading = 0.0;
    }

    public void setCabBoundingBox(Rectangle boundingBox) {
        cabBoundingBox = boundingBox;
    }

    public void setCabHeading(double heading) {
        while(heading < 0) {
            heading += 360.0;
        }
        cabHeading = heading % 360.0;
    }

    public void moveCab(Point vector){
        cabBoundingBox.translate(vector.x, vector.y);
    }

    @Override
    public Rectangle getCabBoundingBox() {
        return cabBoundingBox;
    }

    @Override
    public double getCabHeading() {
        return cabHeading;
    }

    @Override
    public Rectangle getTrailerABoundingBox() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public double getTrailerAHeading() {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public Rectangle getTrailerBBoundingBox() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public double getTrailerBHeading() {
        // TODO Auto-generated method stub
        return 0;
    }

}
