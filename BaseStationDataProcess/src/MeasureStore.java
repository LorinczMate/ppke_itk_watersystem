import java.io.IOException;
import java.sql.*;

/**
 * Created by marci on 2016.11.25..
 */
public class MeasureStore {
    private String dataBasePath;
    private String url;
    private String username;
    private String password;
    private Connection conn;
    private Statement stmt;

    public MeasureStore(String dataBasePath, String url, String username, String password) {
        this.dataBasePath = dataBasePath;
        this.url = url;
        this.username = username;
        this.password = password;
    }

    public void buildConnection() throws SQLException {
        try{
            Class.forName(dataBasePath);
        } catch (ClassNotFoundException e) {
            System.err.println("Nincs telepítve a postgresql driver a JDBC-hez.");
            System.exit(1);
        }
        conn = DriverManager.getConnection(url, username, password);
        stmt = conn.createStatement();
    }

    public void startDataListener(DataSource dataSource)  {
        while(true) {
            Measure measure = dataSource.getNextMeasure();
            try {
                insertMeasureIntoDataBase(measure);
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    private void insertMeasureIntoDataBase(Measure measure) throws SQLException {
            Integer sensorsid;
            String existsQuery = "select sensorsid from sensors where name = ?"; // name lehet unique az adatbázisban
            PreparedStatement exists = conn.prepareStatement(existsQuery);
            exists.setInt(1,measure.getName());
            ResultSet rset = exists.executeQuery();
            if (rset.next()){
                sensorsid = rset.getInt("sensorsid");
            } else {
                String lastInsertedIDQuery = "insert into sensors (name, distance, min_val, max_val) VALUES (?,?,?,?) RETURNING sensorsid;";
                PreparedStatement lastInsertedID = conn.prepareStatement(lastInsertedIDQuery);
                lastInsertedID.setInt(1, measure.getName());
                lastInsertedID.setInt(2, measure.getDistance());
                lastInsertedID.setInt(3, 0);
                lastInsertedID.setInt(4, 1023);
                ResultSet lastInsertedIDResult = lastInsertedID.executeQuery(); // ide kell beszúrni a többi értéket
                lastInsertedIDResult.next();
                sensorsid = lastInsertedIDResult.getInt("sensorsid");
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
