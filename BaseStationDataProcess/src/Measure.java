import java.util.ArrayList;

/**
 * Created by marci on 2016.11.25..
 */
public class Measure {
    private ArrayList<Integer> address;
    private int distance;
    private ArrayList<Integer> measurementData;
    private ArrayList<Integer> rssi;


    public Measure(ArrayList<Integer> address, int distance,
                   ArrayList<Integer> measurementData, ArrayList<Integer> rssi) {
        this.address = address;
        this.distance = distance;
        this.measurementData = measurementData;
        this.rssi = rssi;
    }

    public ArrayList<Integer> getAddress() {
        return address; }

    public int getDistance() {
        return distance;
    }

    public ArrayList<Integer> getMeasurementData() {
        return measurementData;
    }

    public ArrayList<Integer> getRssi() {
        return rssi;
    }
}
