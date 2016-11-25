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
        String name, source, from, distance, measurementData, rssi, data;
        while(true){
            Thread.sleep(1000);
            System.out.println("Írja be az adatbázisba írni kívánt adatokat a következő formában: \n name,source,from,distance,measurementData,rssi");
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            data = br.readLine();
            String[] parts = data.split(",");
            name = parts[0];
            source = parts[1];
            from = parts[2];
            distance = parts[3];
            measurementData = parts[4];
            rssi = parts[5];

            System.out.println("select id from sensors where name = '" + source + "'");
            ResultSet rset = stmt.executeQuery("select id from sensors where name = '" + source + "'");
            if (readacm.getActualSource() == null) continue;
            if (rset.next()){
                stmt.execute("insert into measures(sensorid, soucre, from, distance, measurementData, rssi)");
            } else{
                stmt.execute("insert into sensors (name) VALUES (" + "''" + source + "');");
            }
            readacm.throwActual();


        }
    }
}
