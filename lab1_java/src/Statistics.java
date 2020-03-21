import java.util.Collection;
import java.util.Iterator;

public class Statistics<T> extends java.util.HashSet<PairWrapper<T>> {
    Statistics(Collection<? extends PairWrapper<T>> c){
        super(c);
    }
    Statistics(){
        super();
    }

    @Override
    public boolean add(PairWrapper<T> tPairWrapper) {
        boolean next =super.add(tPairWrapper);
        if(!next){
            return false;
        }else{
            return true;
        }
    }

    /*@Override
    public Iterator<PairWrapper<T>> iterator() {
        return super.iterator();
    }*/

    /*@Override
    public void clear() {
        super.clear();
    }*/
}
