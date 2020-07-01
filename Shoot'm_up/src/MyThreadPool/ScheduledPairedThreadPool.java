package MyThreadPool;

import javafx.util.Pair;

import java.util.Queue;
import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ScheduledPairedThreadPool {
    private final Queue<Pair<Callable,Callable>> pairsWorkQueue= new ConcurrentLinkedQueue<Pair<Callable, Callable>>();
    private volatile boolean isRunning =true;
    public ScheduledPairedThreadPool(int nThreads){
        for(int i=0;i<nThreads*2;i++){
            new Thread();
        }
    }

    public void shutdown(){
        isRunning = false;
    }



}
final class PairedTask {
    Part firstPart;
    Part secondPart;
    public PairedTask(){
        firstPart = new Part(this);
        secondPart = new Part(this);
    }

    private class Part implements Callable{
        final PairedTask fullTask;
        public Part(PairedTask fullTask){
            this.fullTask = fullTask;
        }

        @Override
        public Object call() throws Exception {
            return null;
        }
    }

}