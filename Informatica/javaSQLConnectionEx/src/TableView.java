import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;
import java.awt.*;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by inf.delmastroa0612 on 29/11/2018.
 */
public class TableView extends JPanel {
    JTable table;

    public TableView() {
        table = new JTable();
        JScrollPane scrollPane = new JScrollPane(table,  JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        /* Costruzione dei campi della tabella */
        DefaultTableModel tableModel = (DefaultTableModel) table.getModel();
        tableModel.addColumn("customer_id");
        tableModel.addColumn("store_id");
        tableModel.addColumn("first_name");
        tableModel.addColumn("last_name");
        tableModel.addColumn("email");
        tableModel.addColumn("address_id");
        tableModel.addColumn("active");
        tableModel.addColumn("create_date");
        tableModel.addColumn("last_update");
        tableModel.addColumn("store_id");
        tableModel.addColumn("manager_staff_id");
        tableModel.addColumn("address_id");
        tableModel.addColumn("last_update");

        table.updateUI();
        this.add(new JLabel("Joining customer and store tables of SakilaDB"), BorderLayout.NORTH);
        this.add(scrollPane, BorderLayout.CENTER);

        /* Caricamento dei dati */
        try {
            String query = "SELECT * from customer JOIN store ON customer.store_id = store.store_id";
            PreparedStatement ps = DatabaseConnection.connection.prepareStatement(query);
            ResultSet res = ps.executeQuery();

            /* Per ciascun dato viene convertito in stringa e aggiunto all'array */
            while(res.next()) {
                String[] arr = new String[13];

                String customer_id = Short.toString(res.getShort(1));
                arr[0] = customer_id;

                String customer_store_id = Byte.toString(res.getByte(2));
                arr[1] = customer_store_id;

                String first_name = res.getString(3);
                arr[2] = first_name;

                String last_name = res.getString(4);
                arr[3] = last_name;

                String email = res.getString(5);
                arr[4] = email;

                String customer_address_id = Short.toString(res.getShort(6));
                arr[5] = customer_address_id;

                String active = Boolean.toString(res.getBoolean(7));
                arr[6] = active;

                String create_date = res.getDate(8).toString();
                arr[7] = create_date;

                String customer_last_update = res.getDate(9).toString();
                arr[8] = customer_last_update;

                String store_store_id = Byte.toString(res.getByte(10));
                arr[9] = store_store_id;

                String manager_staff_id = Short.toString(res.getShort(11));
                arr[10] = manager_staff_id;

                String store_address_id = Short.toString(res.getShort(12));
                arr[11] = store_address_id;

                String store_last_update = res.getDate(13).toString();
                arr[12] = store_last_update;

                tableModel.addRow(arr);
            }
        } catch (SQLException sqle) {
            System.out.println("Errore nell'inserimento dei dati nel database");
        }
    }
}
