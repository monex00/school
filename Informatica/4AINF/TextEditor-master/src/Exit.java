import javax.swing.*;

public class Exit {
    private static int i;

    public static int getI() {
        return i;
    }

    public static String run(String txt){
        String ret="";
        Object[] options = { "Esci", "Salva","Annulla" };
        i= JOptionPane.showOptionDialog (null, "Sei sicuro di voler uscire?", "Exit", JOptionPane.DEFAULT_OPTION,
                JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);

       if(i==0){
           System.out.println("esci");
           if(isOk(txt)) {
                   if (Save.salva == false) {
                       int a = JOptionPane.showConfirmDialog(null, "Non hai salvato, salvare?");
                       if (a == 0) {
                           Save.run(txt);
                       }
                   }

           }
       }if(i==1){
           if(Save.isOk(txt)) {
               if(Jframe.s!=txt) {
                   Save.salva=false;
                   if (Save.salva == false) {
                       Save.run(txt);
                       Save.salva = true;
                       ret = txt;
                   } else {
                       JOptionPane.showMessageDialog(null, "Hai gia salvato");
                   }
                   System.out.println("Salva");
                   ret = txt;
               }
           }else {
               ret=txt;           }
         }
       if(i==2){
           ret=txt;
        }

        return ret;
    }
    public static boolean isOk(String txt){
        Boolean isok;
        if(txt.equals("")==true || txt.equals(Jframe.s)==true){
            isok=false;
        }else {
            isok=true;
        }
        return isok;
    }
}
