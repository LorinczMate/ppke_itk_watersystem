

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class ImagePanel extends JPanel {
@SuppressWarnings("unused")
private static final long serialVersionUID = 1L;
	
	private BufferedImage image;

//	Main main;
//	public void setMain(Main main) {
//		this.main = main;
//	}
	
	public ImagePanel() {
		// TODO Auto-generated constructor stub
		try {
			image =  ImageIO.read(new File("background.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		

	}
	private int R;

	private int G;

	private int B;
	public boolean add(Shape arg0) {
		R = (int)(Math.random()*256);
		G = (int)(Math.random()*256);
		B = (int)(Math.random()*256);
		shapeColor.add(new int[]{R,G,B});
		return aList.add(arg0);
	}

	private ArrayList<Shape> aList = new ArrayList<>();
	


	ArrayList<int[]> shapeColor = new ArrayList<>();

	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		
		
		
		Graphics2D gg = (Graphics2D)g;
		
		gg.drawImage(image, 0, 0,getWidth(),getHeight(), null);


		Color color = new Color(R, G, B);
		gg.setColor(color);
		for (Shape s : aList) {
			gg.fill(s);
		}
		
	}

	
}
