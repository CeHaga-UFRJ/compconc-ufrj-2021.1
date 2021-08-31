import java.util.Random;

public class Main {
    public final static int NTHREADS = 4;
    public final static int NARRAY = 100000000;

    public static void main(String[] args) {
        Thread[] threads = new Thread[NTHREADS];
        int array[] = new int[NARRAY];
        Random random = new Random();

        for(int i = 0; i < NARRAY; i++){
            array[i] = random.nextInt();
        }

        for(int i = 0; i < NTHREADS; i++){
            threads[i] = new Thread(new EvenCounter(i, array, NTHREADS));
        }

        for(int i = 0; i < NTHREADS; i++){
            threads[i].start();
        }

        for(int i = 0; i < NTHREADS; i++){
            try {
                threads[i].join();
            }catch (InterruptedException e){
                System.err.println(e);
                System.exit(1);
            }
        }

        System.out.println("A quantidade de pares é: "+EvenCounter.getTotal());
        System.out.println("A quantidade esperada é: "+EvenCounter.seqTotal(array));
        if(EvenCounter.getTotal() == EvenCounter.seqTotal(array)){
            System.out.println("Está correto! :)");
        }else{
            System.err.println("Está errado!!!");
        }
    }
}
