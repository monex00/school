import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class FormPanelRemove extends JPanel {

    private JTextField txtCustomer_id;
    private JButton btnRemove;

    public FormPanelRemove() {
        /* Costruzione dell'interfaccia grafica */
        JPanel pnlCenter = new JPanel();

        pnlCenter.setLayout(new GridLayout(1, 2));
        pnlCenter.setBorder(new EmptyBorder(10, 10, 10, 10));

        JLabel lblCustomer_id = new JLabel("customer_id (key)");
        txtCustomer_id = new JTextField(20);

        pnlCenter.add(lblCustomer_id); pnlCenter.add(txtCustomer_id);

        btnRemove = new JButton("REMOVE");

        this.add(new JLabel("Remove one customer from customer table of SakilaDB"), BorderLayout.NORTH);
        this.add(pnlCenter, BorderLayout.CENTER);
        this.add(btnRemove, BorderLayout.SOUTH);

        /* Listener di click sul pulsante execute */
        btnRemove.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Tutti i campi vengono colorati di bianco */
                refreshColor();
                /* Viene controllato che nessun campo sia nullo */
                if(checkFields()){
                    try {
                        /* Viene eseguita la query di rimozione dal db */
                        String query = "DELETE FROM customer WHERE customer_id = ?";
                        PreparedStatement ps = DatabaseConnection.connection.prepareStatement(query);
                        ps.setShort(1, Short.parseShort(txtCustomer_id.getText()));

                        ps.execute();
                    } catch (SQLException sqle) {
                        System.out.println("Errore nell'inserimento dei dati nel database");
                    }
                }
            }
        });
    }

    private void refreshColor() {
        txtCustomer_id.setBackground(Color.WHITE);
    }

    private boolean checkFields() {
        boolean ret = false;
        if(txtCustomer_id.getText().equals("")) txtCustomer_id.setBackground(Color.RED);
        else ret = true;
        return  ret;
    }
}
