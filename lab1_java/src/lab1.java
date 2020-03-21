import java.io.*;
import java.util.Scanner;

public class lab1 {



    int main(){
        Scanner cin = new Scanner(System.in);
        while(true) {
            String comand = cin.nextLine();
            if (comand.equals("end")) {
                break;
            }

            int ind = comand.indexOf(" ");
            if (ind == -1) {
                System.out.println("Wrong input.\nPlease write input in form:\ncomand:{code,decode,end} filename.txt");
                continue;
            }

            String filename = comand.substring(ind + 1);
            Scanner reader = null;
            try {
                reader = new Scanner(new FileInputStream(filename));

                //
            } catch (IOException e) {
                System.err.println("Error while reading file " + e.getLocalizedMessage());
            } finally {
                if (null != reader) {
                    reader.close();
                }
            }
            try {
                if (comand.substring(0, ind).contains("decode")) {
                    MorzeConverter converter = new MorzeConverter(reader,mode.decode);

                } else if (comand.substring(0, ind).contains("code")) {
                    MorzeConverter converter = new MorzeConverter(reader,mode.code);
                } else {
                    System.out.println("Wrong input.\nPlease write input in form:\ncomand:{code,decode,end} filename.txt");
                    continue;
                }
            }catch(Exception e){

            }
        }
        return 0;
    }
}
