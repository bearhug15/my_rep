public class PairWrapper<T> /*extends javafx.util.Pair<T,Integer>*/{
    T key;
    Integer value;
    PairWrapper(T key){
        this.key=key;
        value =0;
    }

    @Override
    public boolean equals(Object obj) {
        if(obj.getClass()==this.getClass()){
            boolean b = key.equals(((PairWrapper<T>) obj).getKey());
            if(b){
                ((PairWrapper<T>) obj).increment();
            }
            return b;
        }else{
            return super.equals(obj);
        }
    }

    /*public boolean equals(PairWrapper o){
        if(key.equals(o.getKey())){
            return true;
        }else{
            return false;
        }
    }*/

    public T getKey(){
        return key;
    }

    /*public boolean equals(Object o){
        if(super.getKey().equals(o)){
            return true;
        }else{
            return false;
        }
    }*/

    public void increment(){
        value++;
    }


    public int hashCode(){
        int h = key.hashCode();
        return h;
    }

    @Override
    public String toString() {
        return new String(key.toString()+':'+value.toString());
    }
}
