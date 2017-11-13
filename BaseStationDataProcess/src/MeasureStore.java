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
            parseMeasurementPackage(measure);
        }
    }

    public void parseMeasurementPackage(Measure measure){
        final int FARTHEST_NODE = measure.getAddress().size();
        if(!(measure.getAddress().size() == measure.getMeasurementData().size()) && !(measure.getAddress().size()== measure.getRssi().size())){
            System.err.println("the address, measurement data and rssi arrays has not the same length");
        }

        if(FARTHEST_NODE == 1){
            try {
                insertMeasureIntoDataBase(measure.getAddress().get(0), 1, measure.getMeasurementData().get(0), measure.getRssi().get(0));
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }else if(FARTHEST_NODE > 1){
            try {
                insertMeasureIntoDataBase(measure.getAddress().get(0), 1, measure.getMeasurementData().get(0),
                                          measure.getRssi().get(FARTHEST_NODE-1));
            } catch (SQLException e) {
                e.printStackTrace();
            }
            for (int i = 1; i < FARTHEST_NODE ; i++) {
                try {
                    insertMeasureIntoDataBase(measure.getAddress().get(i), i+1, measure.getMeasurementData().get(i),
                                              measure.getRssi().get(i-1));
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        } else{
            System.err.println("something is wrong, the farthest distance is smaller than 1..");
        }

    }

    private void insertMeasureIntoDataBase(int address, int distance, int measurementData, int rssi) throws SQLException {
            Integer sensorsid;
            String existsQuery = "select sensorsid from sensors where name = ?"; // name lehet unique az adatbázisban
            PreparedStatement exists = conn.prepareStatement(existsQuery);
            exists.setInt(1,address);
            ResultSet rset = exists.executeQuery();
            if (rset.next()){
                sensorsid = rset.getInt("sensorsid");
            } else {
                String lastInsertedIDQuery = "insert into sensors (name, distance, min_val, max_val) VALUES (?,?,?,?) RETURNING sensorsid;";
                PreparedStatement lastInsertedID = conn.prepareStatement(lastInsertedIDQuery);
                lastInsertedID.setInt(1, address);
                lastInsertedID.setInt(2, distance);
                lastInsertedID.setInt(3, 0);
                lastInsertedID.setInt(4, 1023);
                ResultSet lastInsertedIDResult = lastInsertedID.executeQuery(); // ide kell beszúrni a többi értéket
                lastInsertedIDResult.next();
                sensorsid = lastInsertedIDResult.getInt("sensorsid");
            }
            String insertMeasureQuery = "insert into measures(name, from_, distance, measurement_data, rssi) VALUES (?, ?, ?, ?, ?)";
            PreparedStatement insertMeasure = conn.prepareStatement(insertMeasureQuery);
            insertMeasure.setInt(1,sensorsid);
            insertMeasure.setInt(2,address);
            insertMeasure.setInt(3,distance);
            insertMeasure.setInt(4,measurementData);
            insertMeasure.setInt(5,rssi);
            insertMeasure.execute();
    }
}
/*
measureid| name | from_ | distance | measurement_data |  rssi  |            date
8078     |    4 |     1 |        1 |          1345222 | 255588 | 2017-11-13 00:48:46.359297

8080     |    4 |     1 |        1 |                1 |    100 | 2017-11-13 01:05:43.183321
8081     |    5 |    22 |        2 |                2 |    345 | 2017-11-13 01:05:43.212566
8082     |    6 |   555 |        3 |               88 |      2 | 2017-11-13 01:05:43.256557
8083     |    7 |    10 |        4 |             1022 |      3 | 2017-11-13 01:05:43.278713

0,1,1,1,1,5,1,345,22,2,555,88,3,10,1022,100

i should also put a parent field to the sensors table! i can calculate it.

address list:
[1, 22, 555, 10]
measurement list:
[1, 2, 88, 1022]
rssi list:
[345, 2, 3, 100]

correctly:
0,1,1,1,1,5,1,345,22,2,555,88,3,1022,100
measureid  | name | from_ | distance | measurement_data |  rssi  |            date
      8088 |    4 |     1 |        1 |              345 |   1022 | 2017-11-13 01:16:43.845803
      8089 |    3 |     2 |        2 |              555 |     22 | 2017-11-13 01:16:43.863529
      8090 |    1 |     3 |        3 |               10 |     88 | 2017-11-13 01:16:43.87453


 */