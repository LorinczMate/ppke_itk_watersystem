import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.*;

/**
 * Created by marci on 2016.11.25..
 */
public class InsertDataIntoDataBase {
    private ReadSerialPort readacm;
    private String dataBasePath;
    private String url;
    private String username;
    private String password;
    private Connection conn;
    private Statement stmt;
    private DataSource dataSource;

    public InsertDataIntoDataBase(String dataBasePath, String url, String username, String password) {
        this.dataBasePath = dataBasePath;
        this.url = url;
        this.username = username;
        this.password = password;
    }

    public void BuildConnection() throws SQLException, ClassNotFoundException {
        Class.forName(dataBasePath);
        conn = DriverManager.getConnection(url, username, password);
        stmt = conn.createStatement();
    }

        //ezt a mainben egy while(true)-ba tenni majd.
    public void InsertListener() throws SQLException, InterruptedException {
        while(true){
            Thread.sleep(1000);
            System.out.println("select id from sensors where name = '" + readacm.getActualSource() + "'");
            ResultSet rset = stmt.executeQuery("select id from sensors where name = '" + readacm.getActualSource() + "'");
            if (readacm.getActualSource() == null) continue;
            if (rset.next()){
                stmt.execute("insert into measures(sensorid, soucre, from, distance, measurementData, rssi)");
            } else{
                stmt.execute("insert into sensors (name) VALUES (" + "''" + readacm.getActualSource() + "');");
            }
            readacm.throwActual();
        }
    }
    public void TestInsertListener() throws InterruptedException, IOException, SQLException {
        dataSource = new ConsolSource();
        while(true){
            Integer sensorsid;
            Measure measure = dataSource.getNextMeasure();
            ResultSet rset = stmt.executeQuery("select sensorsid from sensors where name = '" + measure.getName() + "'"); // name lehet unique
            if (rset.next()){
                sensorsid = rset.getInt("sensorsid");
            } else {
                ResultSet lastInsertedID = stmt.executeQuery("insert into sensors (name) VALUES ('" + measure.getName() + "') RETURNING sensorsid;"); // ide kell beszúrni a többi értéket
                lastInsertedID.next();
                sensorsid = lastInsertedID.getInt("sensorsid");
            }
            String insertMeasureQuery = "insert into measures(source, from_, distance, measurement_data, rssi) VALUES (?, ?, ?, ?, ?)";
            PreparedStatement insertMeasure = conn.prepareStatement(insertMeasureQuery);
            insertMeasure.setInt(1,sensorsid);
            insertMeasure.setInt(2,measure.getFrom());
            insertMeasure.setInt(3,measure.getDistance());
            insertMeasure.setInt(4,measure.getMeasurementData());
            insertMeasure.setInt(5,measure.getRssi());
            insertMeasure.execute();
        }
    }
}
