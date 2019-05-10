import javax.swing.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Open {
    public static boolean open=false;
    public static String s;
    public static int catteri=0;
    private static String[]  path;
    public static String run(String txt){
         s = "";
        open=false;
        if(isOk(txt)) {
            /*open dialog*/
            JFileChooser openFile = new JFileChooser();
            int ret = openFile.showOpenDialog(null);

            if (ret != JFileChooser.APPROVE_OPTION) {
                return txt;
            }
            /*apertura file*/
            File filename = openFile.getSelectedFile();
            path=filename.toString().split("\\\\");
            try {
                FileReader reader = new FileReader(filename);
                BufferedReader buffer = new BufferedReader(reader);
                String line;
                while ((line = buffer.readLine()) != null) {
                    s += line + "\n";
                    getCatteri(s);
                }
                Save.salva = false;
                buffer.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }else {
            s=txt;
        }
            return s;
        }

    public static Boolean isOk(String txt){
        Boolean isok=false;

        if(txt.equals(Jframe.s)==false && txt.equals("")==false) {
            if (Save.salva == false ) {
                System.out.println("ok");
                JOptionPane.showMessageDialog(null, "Non hai salvato");
                Jframe.i=1;
                open=true;
                Save.run(txt);
            }
        }else {
            isok=true;
        }
        return isok;
    }

    public static int getCatteri(String txt){
        txt=txt.replace("\n","");
        if(txt!=null)
            catteri+=catteri + txt.length();
        return catteri;
    }

    public static String getPath() {
        return path[path.length-1];
    }

    public static void setPath(String[] path) {
        Open.path = path;
    }
}

