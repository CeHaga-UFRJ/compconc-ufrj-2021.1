public class LE{
  private int leit; // Quantidade de leitores lendo
  private int escr; // Quantidade de escritores escrevendo
  private int escrQuer; // Quantos escritores querem escrever

  public LE(){
    this.leit = 0;
    this.escr = 0;
    this.escrQuer = 0;
  }

  // Entrada de leitores
  public synchronized void EntraLeitor (int id) {
    try {
      // Enquanto tiver um escritor querendo escrever, se bloqueia
      while (this.escrQuer > 0) {
        System.out.println ("le.leitorBloqueado("+id+")");
        wait();
      }
      this.leit++; // Leitor lendo
      System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }

  // Saida de leitores
  public synchronized void SaiLeitor (int id) {
    this.leit--; // Saiu o leitor
    // Se nao tiver mais leitores, libera apenas o proximo escritor
    if (this.leit == 0)
      this.notify();
    System.out.println ("le.leitorSaindo("+id+")");
  }

  // Entrada de escritores
  public synchronized void EntraEscritor (int id) {
    try {
      this.escrQuer++; // Escritor quer escrever
      // Enquanto tiver alguem lendo ou escrevendo, se bloqueia
      while ((this.leit > 0) || (this.escr > 0)) {
        System.out.println ("le.escritorBloqueado("+id+")");
        wait(); 
      }
      this.escr++; // Escritor escrevendo
      System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }

  // Saida de escritores
  public synchronized void SaiEscritor (int id) {
    this.escr--; // Escritor terminou
    this.escrQuer--; // Escritor nao quer mais escrever
    notifyAll();
    System.out.println ("le.escritorSaindo("+id+")");
  }
}
