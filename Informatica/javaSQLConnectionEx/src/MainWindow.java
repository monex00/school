import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.sql.*;

/**
 * Created by inf.delmastroa0612 on 26/11/2018.
 */
public class MainWindow extends JFrame {

    JFrame frame = this;
    private JTextField txtTable = null;
    private JButton btnTable = null;
    private Connection dbConnection = null;
    private JPanel mainPanel = null;

    public static void main(String[] args) {
        /* Avvio del thread di costruzione della grafica */
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new MainWindow();
            }
        });
    }

    private MainWindow() {
        /* Collegamento con il database */
        try {
            DatabaseConnection.connectToDatabase("root", "");
        } catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        } catch(SQLException sqle) {
            System.out.println("[Errore nell'apertura della comunicazione con il database]\n");
            sqle.printStackTrace();
        }

        /* Costruzione interfaccia grafica */
        setTitle("Sakila DB editor");
        setSize(500, 500);
        setLocationRelativeTo(null);

        this.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent e) {}

            @Override
            public void windowClosing(WindowEvent e) {
                /* Quando la finestra viene chiusa la comunicazione con il server viene interrotta e
                 * il processo terminato */
                try {
                    DatabaseConnection.disconnectFromDatabase();
                    System.exit(0);
                } catch (SQLException sqle) {
                    System.out.println("Errore nella disconnessione al database");
                }
            }

            @Override
            public void windowClosed(WindowEvent e) {}

            @Override
            public void windowIconified(WindowEvent e) {}

            @Override
            public void windowDeiconified(WindowEvent e) {}

            @Override
            public void windowActivated(WindowEvent e) {}

            @Override
            public void windowDeactivated(WindowEvent e) {}
        });

        /* == Costruzione della barra del menu == */
        JMenuBar menuBar = new JMenuBar();

        JMenu menuFile = new JMenu("File");
        menuFile.setMnemonic('F');
        JMenuItem menuItemExit = new JMenuItem("Exit");
        menuItemExit.setMnemonic('x');
        menuFile.add(menuItemExit);

        JMenu menuEdit = new JMenu("Edit");
        menuEdit.setMnemonic('E');
        JMenuItem menuItemAddData = new JMenuItem("Add data");
        menuItemAddData.setMnemonic('A');
        JMenuItem menuItemRemoveData = new JMenuItem("Remove data");
        menuItemRemoveData.setMnemonic('R');
        JMenuItem menuItemEditData = new JMenuItem("Edit data");
        menuItemEditData.setMnemonic('E');
        menuEdit.add(menuItemAddData);
        menuEdit.add(menuItemRemoveData);
        menuEdit.add(menuItemEditData);

        JMenu menuView = new JMenu("View");
        menuView.setMnemonic('V');
        JMenuItem menuItemTables = new JMenuItem("Tables");
        menuItemTables.setMnemonic('T');
        menuView.add(menuItemTables);

        menuBar.add(menuFile);
        menuBar.add(menuEdit);
        menuBar.add(menuView);

        this.setJMenuBar(menuBar);
        /* ==== */

        mainPanel = new JPanel();
        frame.add(mainPanel, BorderLayout.CENTER);

        this.setVisible(true);

        menuItemExit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    /* Quando viene premuto il tasto exit viene terminata la comunicazione con il server e
                     * il processo terminato */
                    DatabaseConnection.disconnectFromDatabase();
                    System.exit(0);
                } catch (SQLException sqle) {
                    System.out.println("Errore nella disconnessione al database");
                }
            }
        });

        /* Listener barra dei menu */
        menuItemAddData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Quando viene cliccato viene caricato il form di inserimento nella tabella */
                frame.remove(mainPanel);
                mainPanel = new FormPanelInsert();
                frame.add(mainPanel, BorderLayout.CENTER);
                frame.validate();
                frame.repaint();
            }
        });

        menuItemRemoveData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Quando viene cliccato viene caricato il form di rimozione dalla tabella */
                frame.remove(mainPanel);
                mainPanel = new FormPanelRemove();
                frame.add(mainPanel, BorderLayout.CENTER);
                frame.validate();
                frame.repaint();
            }
        });

        menuItemEditData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Quando viene cliccato viene caricato il form di modifica della tabella */
                frame.remove(mainPanel);
                mainPanel = new FormPanelEdit();
                frame.add(mainPanel, BorderLayout.CENTER);
                frame.validate();
                frame.repaint();
            }
        });

        menuItemTables.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /* Quando viene cliccato viene caricato il panel contenente la tabella */
                frame.remove(mainPanel);
                mainPanel = new TableView();
                frame.add(mainPanel, BorderLayout.CENTER);
                frame.validate();
                frame.repaint();
            }
        });
    }
}
