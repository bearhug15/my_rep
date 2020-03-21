public class StatAdder extends Thread{
    final Statistics stat;
    final java.util.Vector str;
    private volatile boolean mFinish= false;
    private volatile boolean doThings =false;
    //private volatile boolean init =false;
    StatAdder(Statistics stat){
        super();
        str = new java.util.Vector<>();
        this.stat =stat;
    }
    public void finish(){
        mFinish =true;
    }
    @Override
    public void run(){
        do{
           if(!mFinish || !str.isEmpty()){
               if(!str.isEmpty()){
                   //doThings=false;
                   stat.add(new PairWrapper(str.remove(0)));

               }
           }else{
               return;
           }
           try {Thread.sleep(100);}
               catch(InterruptedException e){}
        }while(true);

    }

    public void addToStat(String str){
        this.str.add(str);
        //doThings = true;
    }
}
