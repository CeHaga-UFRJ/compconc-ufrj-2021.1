public class Monitor {
    private int n;
    private int leit;
    private int escr;

    public Monitor(){
        this.leit = 0;
        this.escr = 0;
        this.n = 0;
    }

    public synchronized int getN() {
        return n;
    }

    public synchronized void setN(int n) {
        this.n = n;
    }

    // Entrada para leitores
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.escr > 0) {
                System.out.println ("X Bloqueando L"+id);
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.leit++;  //registra que ha mais um leitor lendo
            System.out.println ("? Lendo L"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para leitores
    public synchronized void SaiLeitor (int id) {
        this.leit--; //registra que um leitor saiu
        if (this.leit == 0)
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println ("> Saindo L"+id);
    }

    // Entrada para escritores
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("X Bloqueando E"+id);
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println ("! Escrevendo E"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para escritores
    public synchronized void SaiEscritor (int id) {
        this.escr--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("> Saindo E"+id);
    }

    // Entrada para leitoresescritores
    public synchronized void EntraLeitorEscritor (int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("X Bloqueando LE"+id);
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println ("! Escrevendo LE"+id);
        } catch (InterruptedException e) { }
    }

    // Saida para leitoresescritores
    public synchronized void SaiLeitorEscritor (int id) {
        this.escr--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("> Saindo LE"+id);
    }
}
