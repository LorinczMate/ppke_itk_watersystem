/**
 * Created by marci on 2016.11.25..
 */
public class Measure {
    private int name;
    private int from;
    private int distance;
    private int measurementData;
    private int rssi;

    public Measure(int name, int from, int distance, int measurementData, int rssi) {
        this.name = name;
        this.from = from;
        this.distance = distance;
        this.measurementData = measurementData;
        this.rssi = rssi;
    }

    public int getName() {
        return name;
    }

    public int getFrom() {
        return from;
    }

    public int getDistance() {
        return distance;
    }

    public int getMeasurementData() {
        return measurementData;
    }

    public int getRssi() {
        return rssi;
    }
}
