import java.sql.SQLException;

/**
 * Created by marci on 2016.11.25..
 */
public class Main {
    private static final boolean readFromSerialPort = true;
    public static void main(String[] args)  {
        MeasureStore insertToDB = new MeasureStore("org.postgresql.Driver",
                "jdbc:postgresql://localhost:5432/postgres", "szama7", "szama7");
        try {
            insertToDB.buildConnection();
        } catch (SQLException e) {
            e.printStackTrace();
            System.exit(1);
        }

        DataSource dataSource;
        if(readFromSerialPort){
            dataSource = new SerialportSource("/dev/ttyACM0");
        }else {
            dataSource = new ConsolSource();
        }
        insertToDB.startDataListener(dataSource);
    }
}
