public class Escritor extends Thread{
    private int id;
    private Monitor monitor;
    private int sleepTime;
    private int iter;

    public Escritor(int id, Monitor monitor, int sleepTime, int iter) {
        this.id = id;
        this.monitor = monitor;
        this.sleepTime = sleepTime;
        this.iter = iter;
    }

    @Override
    public void run() {
        try{
            for(int i = 0; i < iter; i++){
                this.monitor.EntraEscritor(this.id);
                this.monitor.setN(id);
                System.out.println("*E"+id+" Sobreescrevendo para "+id);
                this.monitor.SaiEscritor(this.id);
                sleep(sleepTime);
            }
        }catch (InterruptedException e){

        }
    }
}
