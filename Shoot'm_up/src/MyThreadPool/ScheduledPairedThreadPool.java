package MyThreadPool;

import javafx.util.Pair;

import java.util.*;
import java.util.concurrent.*;

public class ScheduledPairedThreadPool {
    private final LinkedList<PairedTask> pairsWorkQueue= new LinkedList<>();
    private volatile Boolean isRunning = true;
    private HashSet<Pair<WorkerThread,WorkerThread>> threadPool;
    private ControlThread controlThread;
    public ScheduledPairedThreadPool(){
        threadPool = new HashSet<Pair<WorkerThread,WorkerThread>>();
        controlThread = new ControlThread();
        controlThread.start();
    }

    public void shutdown(){
        synchronized (isRunning) {
            isRunning = false;
        }
        //controlThread.shutdown();
    }
    public void continueWork(){
        synchronized (isRunning) {
            isRunning = true;
        }
    }
    public void clearQueue(){
        synchronized (pairsWorkQueue){
            pairsWorkQueue.clear();
        }
    }
    public void scheduleAtFixedRate(Runnable firstPart,Runnable secondPart,long delay, long period){
        if(delay<0L){
            throw new IllegalArgumentException("Negative delay");
        } else if(period<=0L){
            throw new IllegalArgumentException("Non-positive period");
        } else{
            synchronized (pairsWorkQueue) {
                pairsWorkQueue.add(new PairedTask(firstPart, secondPart, System.currentTimeMillis() + delay, period));
            }
        }
    }
    public void setStop(boolean b){
        controlThread.setIsStopped(b);
    }


    class ControlThread extends Thread{
        volatile Boolean isStopped =false;

        public void setIsStopped(Boolean b){
            synchronized (isStopped) {
                isStopped = b;
            }
        }

       @Override
        public void run(){
           System.out.println("Start control thread");
          // LinkedList toEnd = new LinkedList();
           PairedTask buff;
            while(true){
                synchronized (isRunning){
                    if(isRunning){
                        synchronized (pairsWorkQueue) {
                            ListIterator<PairedTask> iter = pairsWorkQueue.listIterator();
                            while(iter.hasNext()){
                                buff = iter.next();
                                long curTime =System.currentTimeMillis();
                                long execTime = buff.nextExecutionTime;
                                if(execTime<=curTime){
                                    synchronized (threadPool) {
                                        /*if (buff.conectedThreads != null && threadPool.contains(buff.conectedThreads)) {
                                            System.out.println("Start threads");
                                            Pair<WorkerThread,WorkerThread> pair = buff.getConnectedThreads();
                                            if(pair.getKey().isWorking()==false&&pair.getValue().isWorking()==false){
                                                pair.getKey().setIsWorking(true);
                                                pair.getValue().setIsWorking(true);
                                                buff.setNextExecutionTime(execTime+buff.period);
                                            }else{

                                            }
                                        }
                                        else {
                                            WorkerThread w1 = new WorkerThread(buff.getFirstTask());
                                            WorkerThread w2 = new WorkerThread(buff.getSecondTask());
                                            Pair<WorkerThread,WorkerThread> pair = new Pair<>(w1,w2);
                                            threadPool.add(pair);
                                            buff.setConectedThreads(pair);
                                            w1.start();
                                            w2.start();
                                            buff.setNextExecutionTime(execTime+buff.period);
                                        }*/
                                        if (buff.conectedThreads != null && threadPool.contains(buff.conectedThreads) ) {

                                            Pair<WorkerThread,WorkerThread> pair = buff.getConnectedThreads();
                                            if(pair.getKey().isWorking()==false&&pair.getValue().isWorking()==false){
                                                System.out.println("Start threads");
                                                threadPool.remove(pair);
                                                WorkerThread w1 = new WorkerThread(buff.getFirstTask());
                                                WorkerThread w2 = new WorkerThread(buff.getSecondTask());
                                                Pair<WorkerThread,WorkerThread> newpair = new Pair<>(w1,w2);
                                                threadPool.add(newpair);
                                                buff.setConectedThreads(newpair);
                                                w1.start();
                                                w2.start();
                                                buff.setNextExecutionTime(execTime+buff.period);
                                            }else{
                                                System.out.println("Previous threads not finished");
                                            }
                                        }
                                        else {
                                            WorkerThread w1 = new WorkerThread(buff.getFirstTask());
                                            WorkerThread w2 = new WorkerThread(buff.getSecondTask());
                                            Pair<WorkerThread,WorkerThread> pair = new Pair<>(w1,w2);
                                            threadPool.add(pair);
                                            buff.setConectedThreads(pair);
                                            w1.start();
                                            w2.start();
                                            buff.setNextExecutionTime(execTime+buff.period);
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
                synchronized (isStopped) {
                    if (isStopped){
                        break;
                    }
                }
            }
            for(Pair<WorkerThread,WorkerThread> p:threadPool){
                p.getKey().shutdown();
                p.getValue().shutdown();
            }
        }
    }

    final class PairedTask {
        Part firstPart;
        Part secondPart;
        long period;
        long nextExecutionTime;
        Pair<WorkerThread,WorkerThread> conectedThreads = null;
        public PairedTask(Runnable firstPart, Runnable secondPart, long startTime ,long period){
            this.firstPart = new Part(this,firstPart);
            this.secondPart = new Part(this, secondPart);
            this.period = period;
            this.nextExecutionTime = startTime;
        }
        public void setPeriod(long period){
            this.period = period;
        }
        public void setNextExecutionTime(long nextExecutionTime){
            this.nextExecutionTime = nextExecutionTime;
        }
        public Runnable getFirstTask(){
            return firstPart;
        }
        public Runnable getSecondTask(){
            return secondPart;
        }
        public Pair<WorkerThread,WorkerThread> getConnectedThreads(){
            return conectedThreads;
        }
        public void setConectedThreads(Pair<WorkerThread,WorkerThread> threads){
            conectedThreads = threads;
        }

        private class Part implements Runnable{
            final PairedTask fullTask;
            final Runnable taskPart;
            public Part(PairedTask fullTask, Runnable taskPart){
                this.fullTask = fullTask;
                this.taskPart = taskPart;
            }
            @Override
            public void run() {
                if(isRunning){
                    if(taskPart!= null) {
                        taskPart.run();
                    }
                }
            }
        }

    }

}

class WorkerThread extends Thread{
    private Boolean isWorking = false;
    private Boolean stop = false;
    public WorkerThread(Runnable r){
       super(r);
    }
    @Override
    public void run() {
       /* synchronized (isWorking){
            isWorking = true;
        }*/
        /*while (true) {
            if(isWorking) {*/
                super.run();
               /* synchronized (isWorking){
                    isWorking = false;
                }
                synchronized (stop){
                    if(stop){
                        break;
                    }
                }
            }
        }
        synchronized (isWorking){
            isWorking = false;
        }*/
    }
    public boolean isWorking(){
        boolean res;
        synchronized (isWorking){
            res = isWorking;
        }
        return res;
    }
    public void setIsWorking(boolean isWorking){
        synchronized (this.isWorking){
            this.isWorking = isWorking;
        }
    }
    public void shutdown(){
        synchronized (stop) {
            stop = true;
        }
    }
}