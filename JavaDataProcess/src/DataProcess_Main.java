
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class DataProcess_Main {

	private static boolean tableConnection;

	public static void main(String[] args) throws ClassNotFoundException,
			SQLException, InterruptedException, IOException {
	//	ReadSerialPort readacm = new ReadSerialPort("/dev/ttyACM0");

	//	readacm.start();
		Thread.sleep(100);

		//GraphicalUserInterface e = new GraphicalUserInterface();
		//e.parser.addNewPacket("sourceID", "parentNode", "distance", "messageTpye", "from", "to", "measurementData"); // 1=lqi; 2= rssi; 3 = packetserial
		/*
		 * PosgreSQL eléri része
		*/
		//readacm.threadEnabler = false;
		/*
		String database = "org.postgresql.Driver";
		Class.forName(database);
		String url = "jdbc:postgresql://localhost:5432/postgres";
		String username = "postgres-xc";
		String password = "";
		Connection conn = DriverManager.getConnection(url, username, password);
		Statement stmt = conn.createStatement();
		
		while (true) {
			Thread.sleep(1000);
			System.out.println("select id from sensors where name = '" + readacm.getActualSensorName() + "'");
			ResultSet rset = stmt.executeQuery("select id from sensors where name = '" + readacm.getActualSensorName() + "'");
			if (readacm.getActualSensorName() == null)
				continue;
			if (rset.next()) {
				stmt.execute("insert into measures (sensorsid, date, euvalue, battery, lq, rssi, packetserialnumber) VALUES (" + "'" + rset.getString("id")	+ "'" + "," + "'" + "now()" + "'" + "," + readacm.getActualEuvalue() + "," + "'" + readacm.getActualBattery() + "'" + "," + readacm.getActualLqi() + "," + readacm.getActualRssi() + "," + readacm.getActualPacketSerialNumber() + ");");
//				e.parser.addNewPacket(readacm.getActualSensorName(),
//						readacm.getActualDate(), readacm.getActualEuvalue(),
//						readacm.getActualBattery(), readacm.getActualLqi(),
//						readacm.getActualRssi(),
//						readacm.getActualPacketSerialNumber());
				System.out.println("*");
			} else {
				stmt.execute("insert into sensors (name) VALUES (" + "'"
						+ readacm.getActualSensorName() + "');");
			}
			readacm.throwActual();

		}
	*/
	}

}
