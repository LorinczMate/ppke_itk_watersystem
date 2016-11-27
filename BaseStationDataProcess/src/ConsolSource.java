import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by marci on 2016.11.25..
 */
public class ConsolSource implements DataSource {
    private String  data;

    public ConsolSource() {
        System.out.println("Írja be az adatbázisba írni kívánt adatokat a következő formában: \n source,from,distance,measurementData,rssi");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        try {
            data = br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public Measure getNextMeasure() {
        String[] parts = data.split(",");
        int name = Integer.parseInt(parts[0]);
        int from = Integer.parseInt(parts[1]);
        int distance = Integer.parseInt(parts[2]);
        int measurementData = Integer.parseInt(parts[3]);
        int rssi = Integer.parseInt(parts[4]);

        Measure measure = new Measure(name, from, distance, measurementData, rssi);
        return measure;
    }
}
