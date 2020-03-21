import java.io.*;
import java.util.Scanner;

import static java.lang.System.exit;

public class Main {



    public static void main(String[] input){
        Scanner cin = new Scanner(System.in);
        while(true) {
            String comand = cin.nextLine();
            comand = comand.trim();
            String comandName = comand.substring(0,comand.indexOf(' ')==-1?comand.length():comand.indexOf(' '));
            if (comandName.equals("end")) {
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
                reader = new Scanner(new File("src/"+filename));

                //
            } catch (IOException e) {
                System.out.println("Error while reading file " + e.getLocalizedMessage()+"\n Try again.");
                continue;
            }
            finally {
               /* if (null != reader) {
                    reader.close();
                }*/

            }
            MorzeConverter converter;
            try {
                if (comandName.substring(0, ind).equals("decode")) {
                    converter = new MorzeConverter(reader,mode.decode);

                } else if (comandName.substring(0, ind).equals("code")) {
                    converter = new MorzeConverter(reader,mode.code);
                } else {
                    System.out.println("Wrong input.\nPlease write input in form:\ncomand:{code,decode,end} filename.txt");
                    continue;
                }
            }catch(FileFormatException e){
                reader.close();
                System.out.println(e.getMessage());
                return ;
            }
            try {
                while (converter.hasNext()) {
                    System.out.println(converter.convertNextLine());
                }
                converter.printStatistics();
            }catch(FileFormatException e){

                System.out.println(e.getMessage());
                return ;
            }finally {
                reader.close();
            }

        }
        return ;
    }
}
