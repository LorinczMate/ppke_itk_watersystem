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
}
