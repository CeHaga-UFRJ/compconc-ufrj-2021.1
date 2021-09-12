public class Principal {
    public static int L = 5;
    public static int E = 5;
    public static int LE = 5;
    public static final int sleepTimeLeitor = 100;
    public static final int iterLeitor = 10;
    public static final int sleepTimeEscritor = 100;
    public static final int iterEscritor = 5;
    public static final int sleepTimeLeitorEscritor = 100;
    public static final int iterLeitorEscritor = 3;

    public static void main(String[] args) {
        Monitor monitor = new Monitor();
        Leitor[] l = new Leitor[L];
        Escritor[] e = new Escritor[E];
        LeitorEscritor[] le = new LeitorEscritor[LE];

        for(int i = 0; i < L; i++){
            l[i] = new Leitor(i, monitor, sleepTimeLeitor, iterLeitor);
            l[i].start();
        }

        for(int i = 0; i < E; i++){
            e[i] = new Escritor(i, monitor, sleepTimeEscritor, iterEscritor);
            e[i].start();
        }

        for(int i = 0; i < LE; i++){
            le[i] = new LeitorEscritor(i, monitor, sleepTimeLeitorEscritor, iterLeitorEscritor);
            le[i].start();
        }

    }
}
