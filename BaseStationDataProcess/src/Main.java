import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by marci on 2016.11.25..
 */
public class Main {
    public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException, InterruptedException {
       /* ReadSerialPort readacm = new ReadSerialPort("/dev/ttyACM0");
        readacm.start(); */

        InsertDataIntoDataBase insertToDB = new InsertDataIntoDataBase("org.postgresql.Driver",
                "jdbc:postgresql://localhost:5432/postgres", "szama7", "szama7");
        insertToDB.BuildConnection();
        insertToDB.TestInsertListener();
    }
}
