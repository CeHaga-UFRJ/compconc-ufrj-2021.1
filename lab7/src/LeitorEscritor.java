public class LeitorEscritor extends Thread{
    private int id;
    private Monitor monitor;
    private int sleepTime;
    private int iter;

    public LeitorEscritor(int id, Monitor monitor, int sleepTime, int iter) {
        this.id = id;
        this.monitor = monitor;
        this.sleepTime = sleepTime;
        this.iter = iter;
    }

    @Override
    public void run() {
        try{
            for(int i = 0; i < iter; i++) {
                this.monitor.EntraLeitorEscritor(this.id);
                int n = this.monitor.getN();
                if (n % 2 == 0) {
                    System.out.println(";LE" + id + " O número " + n + " é par");
                } else {
                    System.out.println(";LE" + id + " O número " + n + " é ímpar");
                }
                System.out.println("$LE" + id + " Sobreescrevendo para " + (n * 2));
                this.monitor.setN(n * 2);
                this.monitor.SaiLeitorEscritor(this.id);
                sleep(sleepTime);
            }
        }catch (InterruptedException e){

        }
    }
}
