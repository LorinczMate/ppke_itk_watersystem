import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.regex.Matcher;

public class ReadSerialPort extends Thread {
	private ArrayList<String> parentNodeID;
	private ArrayList<String> from;
	private ArrayList<String> to;
	private ArrayList<String> packetSerialNumber;
	private Matcher matcher;
	private ArrayList<String> distance;
	private ArrayList<String> messageType;
	private String portPath;
    private boolean threadEnabler = true;
	private ArrayList<String> sourceID;
	private ArrayList<String> measurementData;
    
    public ReadSerialPort(String portPath){
    	this.portPath = portPath;
    }
	
    String readFile(String fileName) throws IOException {
    	/*
    	 * void sendMeasurementDLPacket(char messageType, char to, char from, char payloadLength, char *payload){
	arrayShiftRight(payloadLength, payload, source);
	arrayShiftRight(payloadLength + 1, payload, parentnode);
	arrayShiftRight(payloadLength + 2, payload, distance); //elsőként a vermünkbe a saját címünket tesszük, de ekkor a main-ben írt üzenet már benne van!
	arrayShiftRight(payloadLength + 3, payload, messageType);
	arrayShiftRight(payloadLength + 4, payload, from); //második elem a címzett címe lesz
	arrayShiftRight(payloadLength + 5, payload, to);
	sendPPacket(payloadLength + 6, payload);
}
    	 */
		
	    BufferedReader br = new BufferedReader(new FileReader(fileName));
	    String line = null;
	    ArrayList<String> read = new ArrayList<>();
	    sourceID = new ArrayList<String>();
	    parentNodeID = new ArrayList<String>();
	    distance = new ArrayList<String>();
	    messageType = new ArrayList<String>();
	    from = new ArrayList<String>();
	    to = new ArrayList<String>();
	    measurementData = new ArrayList<String>();
	    try {
	        StringBuilder sb = new StringBuilder();
			while (threadEnabler) {
				line = br.readLine();
				System.out.println(line);
				read.add(line);
				line = line.replaceAll("@", "");
				line = line.replaceAll("H", "");
				
				
				String[] parts = line.split(";");
				if(parts.length < 7) continue;
				String ssourceID = parts[0];
				String sparentNodeID = parts[1]; 
				String sdistance = parts[2];
				String smessageType = parts[3];
				String sfrom = parts[4];
				String sto = parts[5];
				String smeasurementData = parts[6];
				/*
				 * 				int slqi = Integer.parseInt(parts[4]);
				 *				int srssi = Integer.parseInt(parts[5]);
				 */

				
				sourceID.add(ssourceID);
				parentNodeID.add(sparentNodeID);
				distance.add(sdistance);
				messageType.add(smessageType);
				from.add(sfrom);
				to.add(sto);
				measurementData.add(smeasurementData);
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

	public ArrayList<String> getDate() {
		return parentNodeID;
	}
	
	public void throwActual(){
		sourceID.remove(0);
		parentNodeID.remove(0);
		distance.remove(0);
		messageType.remove(0);
		from.remove(0);
		to.remove(0);
		measurementData.remove(0);
	}
	
	public String getActualDate(){
		if (parentNodeID.size()>0){
			return getDate().get(0);
		}else return null;
	}

	public ArrayList<String> getLqi() {
		return from;
	}
	
	public String getActualLqi(){
		if(from.size()>0){
			return getLqi().get(0);
		} else return null;
	}

	public ArrayList<String> getRssi() {
		return to;
	}
	
	public String getActualRssi(){
		if(to.size()>0){
			return getLqi().get(0);
		} else return null;
	}
	
	public ArrayList<String> getPacketSerialNumber() {
		return measurementData;
	}
	
	public String getActualPacketSerialNumber(){
		if(packetSerialNumber.size()>0){
			return getLqi().get(0);
		} else return null;
	}


	public Matcher getMatcher() {
		return matcher;
	}

	public ArrayList<String> getEuvalue() {
		return distance;
	}
	
	public String getActualEuvalue(){
		if(distance.size()>0){
			return getEuvalue().get(0);
		} else return null;
	}

	public ArrayList<String> getBattery() {
		return messageType;
	}
	
	public int getBatterySize(){
		return messageType.size();
	}
	
	public String getActualBattery(){
		if(getBatterySize()>0)
			return getBattery().get(0);
		else
			return null;
	}

	public ArrayList<String> getSensorName() {
		return sourceID;
	}
	
	public int getSensorNameSize(){
		return sourceID.size();
	}
	
	public String getActualSensorName(){
		if(getSensorNameSize()>0)
			return getSensorName().get(0);
		else
			return null;
	}
}
