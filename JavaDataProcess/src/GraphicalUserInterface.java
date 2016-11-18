

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

/*
 * ----------------------------------------------------------
 * **********************************************************
 *               ITT ÁT KELL ÍRNI
 * **********************************************************
 * ----------------------------------------------------------
 */


class Node{
	/*	    sourceID = new ArrayList<String>();
	    parentNodeID = new ArrayList<String>();
	    distance = new ArrayList<String>();
	    messageType = new ArrayList<String>();
	    from = new ArrayList<String>();
	    to = new ArrayList<String>();
	    measurementData = new ArrayList<String>();*/
	
	public String sourceID;
	public String parentNodeID;
	public String distance;
	public String messageType;
	public int from;
	public int to;
	public int measurementData;
	public Node(String sourceID, String parentNodeID, String distance, String messageType, int from, int to, int measurementData){
		this.sourceID = sourceID;
		this.parentNodeID = parentNodeID;
		this.distance = distance;
		this.messageType = messageType;
		this.from = from;
		this.to = to;
		this.measurementData = measurementData;
	}
}

class DataParser{
	MyTableModel model;
	JFrame frame;
	
	public DataParser(MyTableModel model, JFrame frame){
		this.model = model;
		this.frame = frame;
	}
	
	public void addNewPacket(String sourceID, String parentNodeID, String distance, String messageType, int from, int to, int measurementData){
		
//		String exploded[] = null;
//		date = exploded[0];
//		System.out.println(date + "************************************");
//		euvalue = exploded[1];
//		battery = exploded[2];
//		String slqi = Integer.toString(lqi);
//		slqi = exploded[3];
//		String srssi = Integer.toString(rssi);
//		srssi = exploded[4];
//		String spacketSerialNumber = Integer.toString(packetSerialNumber);
//		spacketSerialNumber = exploded[5];
		
		if (model.nodes.containsKey(sourceID)){
			Node existent = model.nodes.get(sourceID);
			existent.sourceID = sourceID;
			existent.distance = distance;
			existent.messageType = messageType;
			existent.from = from;
			existent.to = to;
			existent.measurementData = measurementData;
		} else{
			//model.nodes.put(sourceID, new Node(sourceID,parentNodeID,  distance, messageType, from, to, measurementData));
			//model.nodeNames.add(packetSerialNumber);
		}
		
		
		/*
		 * ----------------------------------------------------------
		 * **********************************************************
		 *               ITT ÁT KELL ÍRNI
		 * **********************************************************
		 * ----------------------------------------------------------
		 */

		
/*		
		if (model.nodes.containsKey(name)){
			Node existent = model.nodes.get(name);
			existent.lastMeasure = lastMeasure;
			existent.pos = pos;
		} else {
			model.nodes.put(spacketSerialNumber, new Node(spacketSerialNumber, date, euvalue, battery, slqi, srssi));
			model.nodeNames.add(name);
		}*/
		frame.repaint();
	}

	public void addNewPacket(String actualSensorName, String actualDate,
			String actualEuvalue, String actualBattery, String actualLqi,
			String actualRssi, String actualPacketSerialNumber) {
		// TODO Auto-generated method stub
		
	}
}

class MyTableModel extends AbstractTableModel{

	Map<Integer,Node> nodes = new HashMap<Integer,Node>();
	ArrayList<Integer> nodeNames = new ArrayList<Integer>();
	
	@Override
	public int getColumnCount() {
		return 7;
	}

	@Override
	public int getRowCount() {
		return nodes.size();
	}

	@Override
	public Object getValueAt(int row, int col) {
		Integer packetSerialNumber = nodeNames.get(row);
		
		switch (col){
			case 0: return packetSerialNumber;
			case 1: return nodes.get(packetSerialNumber).sourceID;
			case 2: return nodes.get(packetSerialNumber).parentNodeID;
			case 3: return nodes.get(packetSerialNumber).distance;
			case 4: return nodes.get(packetSerialNumber).messageType;
			case 5: return nodes.get(packetSerialNumber).from;
			default: return nodes.get(packetSerialNumber).to;
		}
	}

}


public class GraphicalUserInterface {

	private JFrame frame;
	private final int frameWidth = 1000;
	private final int frameHeight = 640;
	private JMenu menu;
	private JMenuItem exititem;
	private JMenuBar menubar;
	private Container c;
	private JPanel topPanel;
	private int topPanelWidth;
	private int topPanelHeight;
	private JPanel belowPanel;
	private int belowPanelWidth;
	private int belowPanelHeight;
	
	private ImagePanel imagePanel;
	
	private JTable table;
	DataParser parser;
	private MyTableModel model;
	
	
	
	public GraphicalUserInterface() {
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setTitle("szama7 - szakdolgozat");
		frame.setPreferredSize(new Dimension(frameWidth, frameHeight));
		c = frame.getContentPane();
		c.setLayout(new BorderLayout());
		
		menubar = new JMenuBar();
		frame.setJMenuBar(menubar);
		menubar.setBackground(Color.GRAY);
		
		menu = new JMenu("File");
		menu.setMnemonic(KeyEvent.VK_F);
		menubar.add(menu);
		exititem = new JMenuItem("Exit", KeyEvent.VK_E);
		menu.add(exititem);
		
		topPanel = new JPanel();
		topPanelHeight = frameHeight / 2;
		topPanelWidth = frameWidth;
		topPanel.setPreferredSize(new Dimension(topPanelWidth,
				topPanelHeight));
		belowPanel = new JPanel();
		belowPanelHeight = frameHeight / 4;
		belowPanel.setPreferredSize(new Dimension(belowPanelWidth,
				belowPanelHeight));
		
		imagePanel = new ImagePanel();
		//imagePanel.setMain(this);
		belowPanel.setPreferredSize(new Dimension(belowPanelWidth,
				belowPanelHeight));
		belowPanel.add(imagePanel);
		
		model = new MyTableModel();
		parser = new DataParser(model, frame);
		table = new JTable(model);
		
		//topPanel.add(table);
		
		
		frame.add(table, BorderLayout.WEST);
		frame.add(imagePanel, BorderLayout.CENTER);
		
		
		frame.pack();
		frame.setVisible(true);
		
		
		exititem.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				System.exit(0);
			}
		});
	}

	public static void main(String[] args) throws InterruptedException{
		GraphicalUserInterface e = new GraphicalUserInterface();
		Thread.sleep(1000);
		e.parser.addNewPacket("aaa", "asd", "jkl", "w", 1, 2, 3);
	}
}
