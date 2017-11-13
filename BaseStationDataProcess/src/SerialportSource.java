import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created by marci on 2016.11.25..
 */



public class SerialportSource implements DataSource {
    private BufferedReader br;
    private final int PACKAGE_HEADER = 6;
    private final int PERIODICALLY_REPETITIVE_PACKAGE_CONTENT_LENGHT = 3;
    private final int ADDRESS_LENGTH = 1;
    private final int MEASUREMENT_LENGTH = 1;
    private final int RSSI_LENGTH = 1;

    public SerialportSource(String portPath)  {
        try {
            br = new BufferedReader(new FileReader(portPath));
        } catch (FileNotFoundException e) {
            System.err.println("Hibás a sorosport elérési útvonala.");
            System.exit(1);
        }
    }

    @Override
    public Measure getNextMeasure() {
        System.out.println("SerialPortról történő olvasás kezdődik \n");
        String line = "";
        while (line.equalsIgnoreCase("")) {
            try {
                line = br.readLine();
                System.out.println("de");
                System.out.println(line);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        //blokkoló olvasás.
        line = line.replaceAll("@","");
        line = line.replaceAll("H","");
        String[] parts = line.split(",");
        int farthestNodeDistance, to, from, messageType, distance, parentNode, source;
        ArrayList<Integer> address = new ArrayList();
        ArrayList<Integer> measurementData = new ArrayList();
        ArrayList<Integer> rssi = new ArrayList();
        String tempMeasurement = null, tempRssi = null;
        /*
        The farthest node from the BS is marked with n, the closest is 1.

        In that case if the message come from one hop, the message looks like this:
        |to|from|messageType|distance|parentNode|source|address_1|measurementData_1|rssi_1|
        |------------------- 6 byte -------------------|-------------- 8 byte ------------|
        |1 |1   |1          |1       |1         |1     |1        |4                |3     |

        If the message come from two hop, the message looks like this:
        |to|from|messageType|distance|parentNode|source|address_1|measurementData_1|rssi_2|address_2|measurementData_2|rssi_1|

        If the message come from n hop, the message looks like this:
        |to|from|messageType|distance|parentNode|source|address_1|measurementData_1|rssi_2|....|address_n|measurementData_n|rssi_1|
         */
        farthestNodeDistance = (parts.length-PACKAGE_HEADER)/PERIODICALLY_REPETITIVE_PACKAGE_CONTENT_LENGHT;
        to = Integer.parseInt(parts[0]);
        from = Integer.parseInt(parts[1]);
        messageType = Integer.parseInt(parts[2]);
        distance = Integer.parseInt(parts[3]);
        parentNode = Integer.parseInt(parts[4]);
        source = Integer.parseInt(parts[5]); // name

        for (int i = 0; i < farthestNodeDistance; i++) {
            address.add(Integer.parseInt(parts[PACKAGE_HEADER + i * PERIODICALLY_REPETITIVE_PACKAGE_CONTENT_LENGHT]));
            tempMeasurement = parts[PACKAGE_HEADER + ADDRESS_LENGTH + i * PERIODICALLY_REPETITIVE_PACKAGE_CONTENT_LENGHT];
            measurementData.add(Integer.parseInt(tempMeasurement));
            tempRssi = parts[PACKAGE_HEADER + ADDRESS_LENGTH + MEASUREMENT_LENGTH + i * PERIODICALLY_REPETITIVE_PACKAGE_CONTENT_LENGHT];
            rssi.add(Integer.parseInt(tempRssi));
        }

        Measure measure = new Measure(address, distance, measurementData, rssi);
        return measure;
    }
}
