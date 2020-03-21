import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Scanner;

import java.util.HashMap;
public class MorzeConverter {
    final Scanner reader ;
    final HashMap<String,String> morzeMap;
    final Statistics<String> stat ;
    final mode m;
    final StatAdder sa;
    MorzeConverter(Scanner reader,mode m) throws FileFormatException{

        this.reader = reader;
        this.stat =new Statistics<String>();
        sa = new StatAdder(stat);
        sa.start();
        this.m = m;
        morzeMap = new HashMap<String,String>();
        //stat = new Statistics<String>();
        String s="";
        if(reader.hasNext()){
            s = reader.nextLine();
            if(!s.equals("Alphabet start")){
                throw new FileFormatException("Wrong alphabet format");
            }
        }
        //Character ch= new Character(' ');
            if(reader.hasNext()){
                s= reader.nextLine();
            }
        while (reader.hasNext() && !s.equals("Alphabet end")){
                int p =s.indexOf('=');
                if(p==-1){
                    throw new FileFormatException("Wrong alphabet format");
                }
                int pr1 = p;

                int pr2= s.lastIndexOf(' ',p);
                pr2 = pr2<p?p+1:pr2;
                morzeMap.put(s.substring(0,pr1),s.substring(pr2));
                if(m==mode.decode){
                    addToStat(s.substring(pr2));
                }else{
                    addToStat(s.substring(0,pr1));
                }
                s = reader.nextLine();
        }
    }
    public String convertNextLine() throws FileFormatException {
        if(m == mode.code){
            return convertToMorze();
        }else{
            return convertToLang();
        }
    }
    private String convertToLang() throws FileFormatException {
        String s = reader.nextLine();
        String buff;
        StringBuilder words =new StringBuilder();
        int firstInd  = s.indexOf(' ',0);
        int lastInd= 0;
        while(firstInd !=-1){
            buff=morzeMap.get(s.substring(lastInd,firstInd));
            if(buff==null)throw new FileFormatException("Translation to symbol "+s.substring(lastInd,firstInd)+" not exists");
            addToStat(buff);
            words.append(buff);
            lastInd = firstInd+1;
            firstInd = this.firstNotOf(s,' ',lastInd);
            if(firstInd ==-1){
                break;
            }
            switch(firstInd - lastInd){
                case 0:
                case 1:break;
                default:words.append(' ');break;

            }
            lastInd = firstInd;
            firstInd = s.indexOf(' ',lastInd);
        }
        return words.toString();
    }
    private String convertToMorze() throws FileFormatException {
        StringBuilder words = new StringBuilder();
        String s = reader.nextLine();
        s = s.toLowerCase();
        String buff;
        Character ch;
        for(int i = 0; i<s.length();i++){
            ch = s.charAt(i);
            if(ch.equals(' ')){
                words.append("   ");
                continue;
            }
            addToStat(ch.toString());
            buff = morzeMap.get(ch.toString());
            if(buff==null) throw new FileFormatException("Translation to symbol "+ch+" not exists");
            words.append(buff+' ');
        }
        return words.toString();
    }
    private int firstNotOf(String str,char ch, int from){
        int ind=from;
        for(;ind<str.length();ind++){
            if(str.charAt(ind)!=ch)break;
        }
        if(ind>=str.length()) {
            ind=-1;
        }
        return ind;
    }
    private void addToStat(String s){

        //sa.addToStat(s);
        stat.add(new PairWrapper<String>(s));
    }
    public boolean hasNext(){
        return reader.hasNext();
    }
    public void printStatistics(){
        sa.finish();
        try{
        sa.join();
        }catch(InterruptedException e){}

        System.out.println(stat.toString());

    }
}
enum mode{
    code,
    decode
}
