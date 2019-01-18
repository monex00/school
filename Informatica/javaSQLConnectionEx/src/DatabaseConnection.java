import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class DatabaseConnection {

    public static Connection connection = null;

    /** Metodo utilizzato per il collegamento con un database */
    public static void connectToDatabase(String username, String password) throws SQLException, ClassNotFoundException {
        Class.forName("com.mysql.jdbc.Driver");
        connection = DriverManager.getConnection("jdbc:mysql://localhost/sakila?user=" + username + "&password=" + password);
    }

    public static void disconnectFromDatabase() throws SQLException {
        connection.close();
    }
}
