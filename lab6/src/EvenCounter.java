public class EvenCounter implements Runnable{
    private int id;
    private int[] array;
    private int nthreads;

    private static int total = 0;

    public EvenCounter(int id, int[] array, int nthreads){
        this.id = id;
        this.array = array;
        this.nthreads = nthreads;
    }

    public synchronized static int getTotal() {
        return total;
    }

    public synchronized static void addTotal() {
        total++;
    }

    public void run(){
        for(int i = id; i < array.length; i+=nthreads){
            if(array[i] % 2 == 0) addTotal();
        }
    }

    public synchronized static int seqTotal(int[] array){
        int total = 0;
        for(int n : array){
            if(n % 2 == 0) total++;
        }
        return total;
    }
}
