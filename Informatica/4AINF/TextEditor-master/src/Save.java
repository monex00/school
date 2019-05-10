import javax.swing.*;
import java.io.*;
import java.nio.Buffer;
import java.util.ArrayList;

public class Save {
    static boolean salva=false;

    public static void run(String txt){
        if(isOk(txt)) {
            /*open dialog*/
            JFileChooser saveFile = new JFileChooser();
            int ret = saveFile.showSaveDialog(null);
            /*salvo riga x riga*/
            String[] s = (txt.split("\n"));

            if (ret != JFileChooser.APPROVE_OPTION) {
                return;
            }
            /*apertura file*/
            File path = saveFile.getSelectedFile();
            Open.setPath(path.toString().split("\\\\"));
            Jframe.i=0;
            try {
                FileWriter writer = new FileWriter(path);
                BufferedWriter buffer = new BufferedWriter(writer);
                PrintWriter out = new PrintWriter(buffer);

                for (int i = 0; i < s.length; i++) {
                    out.println(s[i]);
                }

                buffer.flush();
                buffer.close();
                Jframe.s=txt;
                salva = true;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static boolean isOk(String txt) {
        boolean isOk=false;
        if (txt.equals("")) {
            JOptionPane.showMessageDialog(null, "Nessun testo da salvare");
        } else {
            if (Jframe.s.equals(txt) == false) {
                isOk=true;
            } else {
                JOptionPane.showMessageDialog(null, "Non hai modificato il testo");
            }
        }
        return isOk;
    }
}

