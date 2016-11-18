import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.regex.Matcher;

public class ReadSerialPort extends Thread {

	private ArrayList<String> to;
	private ArrayList<String> from;
	private ArrayList<String> messageType;
	private ArrayList<String> distance;
	private ArrayList<String> parentNode;
	private ArrayList<String> source;
	private ArrayList<String> measurementData;
	private ArrayList<String> rssi;

	private Matcher matcher;
	private String portPath;
    private boolean threadEnabler = true;

    public ReadSerialPort(String portPath){
    	this.portPath = portPath;
    }
	
    String readFile(String fileName) throws IOException {
		/*
		 * ----------------------------------------------------------
		 *               BaseStation receiveDLPacket
		 *|to|from|messageType|distance|parentNode|source|measurementData|rssi|
		 * ----------------------------------------------------------
		 */

	    BufferedReader br = new BufferedReader(new FileReader(fileName));
	    String line = null;
	    ArrayList<String> read = new ArrayList<>();

		to = new ArrayList<>();
		from = new ArrayList<>();
		messageType = new ArrayList<>();
		distance = new ArrayList<>();
		parentNode = new ArrayList<>();
		source = new ArrayList<>();
	    measurementData = new ArrayList<>();
		rssi = new ArrayList<>();

	    try {
	        StringBuilder sb = new StringBuilder();
			while (threadEnabler) {
				line = br.readLine();
				System.out.println(line);
				read.add(line);
				line = line.replaceAll("@", "");
				line = line.replaceAll("H", "");
				
				
				String[] parts = line.split("|");
				if(parts.length < 8) continue;

				//Hint: itt most mind integerként fog megjelenni -- Integer.parseInt(parts[n]);
				//int srssi = Integer.parseInt(parts[5]);
				String toString = parts[0];
				String fromString = parts[1];
				String messageTypeString = parts[2];
				String distanceString = parts[3];
				String parentNodeString = parts[4];
				String sourceString = parts[5];
				String measurementDataString = parts[6];
				String rssiString = parts[7];

				to.add(toString);
				from.add(fromString);
				messageType.add(messageTypeString);
				distance.add(distanceString);
				parentNode.add(parentNodeString);
				source.add(sourceString);
				measurementData.add(measurementDataString);
				rssi.add(rssiString);
			}
	        return sb.toString();
	    } finally {
	        br.close();
	    }
	}
	
	@Override
	public void run(){
		try {
			readFile(portPath);
			System.out.println("Véget ért a végtelen.");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}


	public void throwActual(){
		to.remove(0);
		from.remove(0);
		messageType.remove(0);
		distance.remove(0);
		parentNode.remove(0);
		source.remove(0);
		measurementData.remove(0);
		rssi.remove(0);
	}

	public ArrayList<String> getTo() {
		return to;
	}

	public ArrayList<String> getFrom() {
		return from;
	}

	public ArrayList<String> getMessageType() {
		return messageType;
	}

	public ArrayList<String> getDistance() {
		return distance;
	}

	public ArrayList<String> getParentNode() {
		return parentNode;
	}

	public ArrayList<String> getSource() {
		return source;
	}

	public ArrayList<String> getMeasurementData() {
		return measurementData;
	}

	public ArrayList<String> getRssi() {
		return rssi;
	}

	public Matcher getMatcher() {
		return matcher;
	}

	//értelme nagyon nincs, mert BS esetében a To minden esetben meg fog egyezni saját címével
	public String getActualTo(){
		if (to.size()>0){
			return getTo().get(0);
		}else return null;
	}

	public String getFrom(){
		if(from.size()>0){
			return getFrom().get(0);
		} else return null;
	}

	public String getActualMessageType(){
		if(messageType.size()>0){
			return getMessageType().get(0);
		} else return null;
	}

	public String getActualDistance(){
		if(distance.size()>0){
			return getDistance().get(0);
		} else return null;
	}

	public String getActualParentNode(){
		if(parentNode.size()>0){
			return getParentNode().get(0);
		} else return null;
	}

	public String getActualSource(){
		if(source.size()>0){
			return getSource().get(0);
		} else return null;
	}

	public String getActualMeasurementData(){
		if(measurementData.size()>0){
			return getMeasurementData().get(0);
		} else return null;
	}

	public String getActualRssi(){
		if(rssi.size()>0){
			return getRssi().get(0);
		} else return null;
	}
}
