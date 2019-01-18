import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.SQLException;

/**
 * Created by inf.delmastroa0612 on 27/11/2018.
 */
public class FormPanelInsert extends JPanel {

    JTextField txtStore_id;
    JTextField txtFirst_name;
    JTextField txtLast_name;
    JTextField txtEmail;
    JTextField txtAddress_id;
    JTextField txtCreate_date;
    JCheckBox chkActive;
    JButton btnInsert;

    public FormPanelInsert() {
        /* Costruzione dell'interfaccia grafica */
        JPanel pnlCenter = new JPanel(new GridLayout(6, 2));

        pnlCenter.setLayout(new GridLayout(7, 2));
        pnlCenter.setBorder(new EmptyBorder(10, 10, 10, 10));

        JLabel lblStrore_id = new JLabel("strore_id");
        JLabel lblFirst_name = new JLabel("first_name");
        JLabel lblLast_name = new JLabel("last_name");
        JLabel lblEmail = new JLabel("email");
        JLabel lblAddress_id = new JLabel("address_id");
        JLabel lblCreate_date = new JLabel("create_date");
        JLabel lblActive = new JLabel("active");

        txtStore_id = new JTextField(20);
        txtFirst_name = new JTextField(20);
        txtLast_name = new JTextField(20);
        txtEmail = new JTextField(20);
        txtAddress_id = new JTextField(20);
        txtCreate_date = new JTextField(20);
        chkActive = new JCheckBox();

        pnlCenter.add(lblStrore_id); pnlCenter.add(txtStore_id);
        pnlCenter.add(lblFirst_name); pnlCenter.add(txtFirst_name);
        pnlCenter.add(lblLast_name); pnlCenter.add(txtLast_name);
        pnlCenter.add(lblEmail); pnlCenter.add(txtEmail);
        pnlCenter.add(lblAddress_id); pnlCenter.add(txtAddress_id);
        pnlCenter.add(lblCreate_date); pnlCenter.add(txtCreate_date);
        pnlCenter.add(lblActive); pnlCenter.add(chkActive);

        btnInsert = new JButton("INSERT");

        this.add(new JLabel("Insert one customer to customer table of SakilaDB"), BorderLayout.NORTH);
        this.add(pnlCenter, BorderLayout.CENTER);
        this.add(btnInsert, BorderLayout.SOUTH);

        /* Listener di click sul pulsante execute */
        btnInsert.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Tutti i campi vengono colorati di bianco */
                refreshColor();
                /* Viene controllato che nessun campo sia nullo */
                if(checkFields()){
                    try {
                        /* Query di inserimento del record nel database */
                        String query = "INSERT INTO customer (store_id, first_name, last_name, email, address_id, active, create_date) VALUES(?, ?, ?, ?, ?, ?, ?)";
                        PreparedStatement ps = DatabaseConnection.connection.prepareStatement(query);
                        ps.setByte(1, Byte.parseByte(txtStore_id.getText()));
                        ps.setString(2, txtFirst_name.getText());
                        ps.setString(3, txtLast_name.getText());
                        ps.setString(4, txtEmail.getText());
                        ps.setShort(5, Short.parseShort(txtAddress_id.getText()));
                        ps.setBoolean(6, chkActive.isSelected());
                        ps.setDate(7, Date.valueOf(txtCreate_date.getText()));

                        ps.execute();
                    } catch (SQLException sqle) {
                        System.out.println("Errore nell'inserimento dei dati nel database");
                    }
                }
            }
        });
    }

    private void refreshColor() {
        txtStore_id.setBackground(Color.WHITE);
        txtFirst_name.setBackground(Color.WHITE);
        txtLast_name.setBackground(Color.WHITE);
        txtEmail.setBackground(Color.WHITE);
        txtAddress_id.setBackground(Color.WHITE);
    }

    private boolean checkFields() {
        boolean ret = false;
        if(txtStore_id.getText().equals("")) txtStore_id.setBackground(Color.RED);
        if(txtFirst_name.getText().equals("")) txtFirst_name.setBackground(Color.RED);
        if(txtLast_name.getText().equals("")) txtLast_name.setBackground(Color.RED);
        if(txtEmail.getText().equals("")) txtEmail.setBackground(Color.RED);
        if(txtAddress_id.getText().equals("")) txtAddress_id.setBackground(Color.RED);
        else ret = true;
        return  ret;
    }
}
