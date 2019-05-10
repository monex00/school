import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.*;

import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created by inf.multaris3001 on 08/02/2018.
 */
public class Jframe extends JFrame {
    public static String s;
    public static int i=1;

    public Jframe() {
        initUI();
    }

    private void initUI() {

        setTitle("Text editor");
        setSize(700, 500);
        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        setLocationRelativeTo(null);


        JLabel row=new JLabel("Righe:");
        JLabel nrow=new JLabel();
        JLabel caratteri=new JLabel("Caratteri:");
        JLabel ncaratteri=new JLabel();
        /*parte north*/
        JPanel north1=new JPanel(new GridLayout(2,1));
        JPanel north2=new JPanel(new GridLayout(1,3));
        JPanel sud=new JPanel(new FlowLayout(FlowLayout.RIGHT));
        sud.add(row);
        sud.add(nrow);
        sud.add(caratteri);
        sud.add(ncaratteri);

        north1.add(north2);
        /*parte center*/
        JTextArea txtArea=new JTextArea("");
        JScrollPane scrPane=new JScrollPane(txtArea);
        JPanel center=new JPanel();
        center.add(scrPane);
        s=txtArea.getText();
        /*menu*/
        JMenuBar menuBar=new JMenuBar();
        JMenu menuFile=new JMenu("file");
        JMenu menuHelp=new JMenu("Help");
        JMenuItem menuOpen=new JMenuItem("Open");
        menuOpen.setMnemonic('o');
        JMenuItem menuSave=new JMenuItem("Save");
        menuSave.setMnemonic('s');
        JMenuItem menuExit=new JMenuItem("Exit");
        menuExit.setMnemonic('e');
        menuOpen.setIcon( UIManager.getIcon("FileView.directoryIcon"));
        menuSave.setIcon(UIManager.getIcon("FileView.floppyDriveIcon"));
        menuExit.setIcon(UIManager.getIcon("InternalFrame.closeIcon"));

        JMenuItem menuAbout=new JMenuItem("About");

        menuFile.add(menuOpen);
        menuFile.add(menuSave);
        menuFile.addSeparator();
        menuFile.add(menuExit);
        menuHelp.add(menuAbout);

        menuBar.add(menuFile);
        menuAbout.setIcon(UIManager.getIcon("OptionPane.questionIcon"));
        menuBar.add(menuHelp);
        txtArea.setFont(txtArea.getFont().deriveFont(17f));
        menuOpen.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

                txtArea.setText(Open.run(txtArea.getText()));
                s = txtArea.getText();
                ncaratteri.setText("" + Open.catteri);
                nrow.setText(""+(txtArea.getLineCount()-1));
                Open.catteri=0;
                setTitle(Open.getPath() + "-EditorTesto");

            }
        });

        menuSave.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Save.run(txtArea.getText());
                setTitle(Open.getPath() + "-EditorTesto");
            }
        });

        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                Exit.run(txtArea.getText());
                if(Exit.getI()==0) {
                    super.windowClosing(e);
                    dispose();
                    System.exit(0);
                }
            }
        });

        menuExit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                txtArea.setText(Exit.run(txtArea.getText()));
                if(Exit.getI()==0){
                    dispose();
                    System.exit(0);
                    System.out.println(Exit.getI());
                }
            }
        });
        menuAbout.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JOptionPane.showMessageDialog(null,"made by Monex");
            }
        });

        txtArea.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                Open.catteri=0;
                ncaratteri.setText(""+Open.getCatteri(txtArea.getText()));
                nrow.setText(""+(txtArea.getLineCount()-1));
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                Open.catteri=0;
                ncaratteri.setText(""+Open.getCatteri(txtArea.getText()));
                nrow.setText(""+(txtArea.getLineCount()-1));
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                Open.catteri=0;
                ncaratteri.setText(""+Open.getCatteri(txtArea.getText()));
                nrow.setText(""+(txtArea.getLineCount()-1));
            }
        });

        add(menuBar,BorderLayout.NORTH);
        add(scrPane,BorderLayout.CENTER);
        add(sud,BorderLayout.SOUTH);

        this.setVisible(true);
    }
}
