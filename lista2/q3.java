import java.util.LinkedList;

class FilaTarefas {
  private int nThreads;
  private MyPoolThreads[] threads;
  //similar a um vetor de objetos Runable
  private LinkedList<Runnable> queue;
  private boolean shutdown;

  public FilaTarefas(int nThreads) {
    this.shutdown=false;
    this.nThreads=nThreads;
    queue=new LinkedList<Runnable>();
    threads = new MyPoolThreads[nThreads];
    for (int i=0; i<nThreads; i++) {
      threads[i] = new MyPoolThreads();
      threads[i].start();
    }
  }

  public void execute(Runnable r) {
    synchronized(queue) {
      if (this.shutdown) return;
      queue.addLast(r); //inclui um novo elemento na lista ’queue’
      queue.notify();
    }
  }

  public void shutdown() {
    System.out.println("Shutdown");
    synchronized(queue) {
      this.shutdown=true;
      queue.notifyAll();
    }
    for (int i=0; i<nThreads; i++)
      try { threads[i].join(); }
      catch (InterruptedException e) {return;}
  }

  private class MyPoolThreads extends Thread {
    public void run() {
      Runnable r;
      while (true) {
        synchronized(queue) {
        //verifica se a lista est´a vazia...
          while (queue.isEmpty() && (!shutdown)) {
            try { queue.wait(); }
            catch (InterruptedException ignored){}
            System.out.println("Thread saindo do wait");
          }
          if(queue.isEmpty() && shutdown) return;
          //retira o primeiro elemento da lista e o retorna
          r = (Runnable) queue.removeFirst();
        }
        try { r.run(); } catch (RuntimeException e) {}
      }
    }
  }
}

class Tarefa implements Runnable{
  private int id;
  public static int total = 0;

  public Tarefa(int id){
    this.id = id;
  }
  public void run(){
    System.out.println("Tarefa "+id);
    total++;
  }
}

class q3{
  public static void main(String[] args) {
    FilaTarefas ft = new FilaTarefas(2);
    for(int i = 0; i < 5; i++){
      ft.execute(new Tarefa(i));
    }
    try {Thread.sleep(1000);}
    catch(Exception e){ }
    ft.shutdown();
    System.out.println("Realizou "+Tarefa.total);
  }
}