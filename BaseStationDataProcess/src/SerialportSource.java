import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created by marci on 2016.11.25..
 */
public class SerialportSource implements DataSource {
    private String line;
    private String to, from, messageType, distance, parentNode, source, measurementData, rssi;

    public SerialportSource(String portPath) throws FileNotFoundException {
        System.out.println("SerialPortról történő olvasás kezdődik \n");
        BufferedReader br = new BufferedReader(new FileReader(portPath));
        StringBuilder sb = new StringBuilder();
        try {
            line = br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public Measure getNextMeasure() {
        line = line.replaceAll("@","");
        line = line.replaceAll("H","");
        String[] parts = line.split(",");
        //|to|from|messageType|distance|parentNode|source|measurementData|rssi|
        int to = Integer.parseInt(parts[0]);
        int from = Integer.parseInt(parts[1]);
        int messageType = Integer.parseInt(parts[2]);
        int distance = Integer.parseInt(parts[3]);
        int parentNode = Integer.parseInt(parts[4]);
        int source = Integer.parseInt(parts[5]); // name
        int measurementData = Integer.parseInt(parts[6]);
        int rssi = Integer.parseInt(parts[7]);

        Measure measure = new Measure(source, from, distance, measurementData, rssi);
        return measure;
    }
}
