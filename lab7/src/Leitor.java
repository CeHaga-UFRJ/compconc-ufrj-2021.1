public class Leitor extends Thread{
    private int id;
    private Monitor monitor;
    private int sleepTime;
    private int iter;

    public Leitor(int id, Monitor monitor, int sleepTime, int iter) {
        this.id = id;
        this.monitor = monitor;
        this.sleepTime = sleepTime;
        this.iter = iter;
    }

    public boolean isPrime(int n){
        if(n == 1) return false;
        if(n == 2) return true;
        if(n % 2 == 0) return false;
        for(int i = 3; i*i < n; i+=2){
            if(n % i == 0) return false;
        }
        return true;
    }

    @Override
    public void run() {
        try{
            for(int i = 0; i < iter; i++){
                this.monitor.EntraLeitor(this.id);
                int n = this.monitor.getN();
                if(isPrime(n)){
                    System.out.println("*L"+id+" O número "+n+" é primo");
                }else{
                    System.out.println("*L"+id+" O número "+n+" não é primo");
                }
                this.monitor.SaiLeitor(this.id);
                sleep(sleepTime);
            }
        }catch (InterruptedException e){

        }
    }
}
