/**
 * Created by marci on 2016.11.25..
 */
public class SerialportSource implements DataSource {

    private String portPath;

    public SerialportSource(String portPath) {
        this.portPath = portPath;
    }

    @Override
    public Measure getNextMeasure() {
        return null;
    }
}
