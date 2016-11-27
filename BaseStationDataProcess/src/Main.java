import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by marci on 2016.11.25..
 */
public class Main {
    private static final boolean enableFast = true;
    public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException, InterruptedException {
        InsertDataIntoDataBase insertToDB = new InsertDataIntoDataBase("org.postgresql.Driver",
                "jdbc:postgresql://localhost:5432/postgres", "szama7", "szama7");
        insertToDB.BuildConnection();

        if(enableFast){
            insertToDB.SerialportInsertListener();
        }else {
            insertToDB.ConsolInsertListener();
        }


    }
}
