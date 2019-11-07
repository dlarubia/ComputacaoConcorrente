/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Produtor/consumidor em Java */
/* -------------------------------------------------------------------*/
import java.util.*;

// Monitor
class Buffer {
  static final int N = 10;    //tamanho do buffer
  private int[] buffer = new int[N];  //reserva espaco para o buffer 
  private int count=0, in=0, out=0;   //variaveis compartilhadas
  
  // Construtor
  Buffer() { 
    for (int i=0;i<N;i++)  buffer[i] = -1; 
  } // inicia o buffer

  // Insere um item
  public synchronized void Insere (int item, int id) {
    try {
      while(count == N) {
        wait();
        System.out.println("Thread " + id + "tentou inserir com Buffer cheio.");
      } 
      this.buffer[in] = item;
      this.count++;
      this.in = (in + 1) % N;
      System.out.println("Inserindo valor " + item + " na posição " + in);
      System.out.println(Arrays.toString(buffer));
      notifyAll();
    } catch (InterruptedException e) { 
      System.out.println(e);
      }
  }
  
  // Remove um item
  public synchronized int Remove (int id) {
   int aux;
   try {
      while(count == 0) {
        wait();
        System.out.println("Thread " + id + "tentou retirar com Buffer vazio.");
      }
      this.count--;
      aux = this.buffer[out];
      this.buffer[out] = -1;
      this.out = (out +1) % N;
      System.out.println("Retirando valor " + aux + " da posição " + out);
      System.out.println(Arrays.toString(this.buffer));
      notifyAll();
    } catch (InterruptedException e) { 
      return -1;
      }
    return aux;
  }
}

//--------------------------------------------------------
// Consumidor
class Consumidor extends Thread {
  int id;
  int delay;
  Buffer buffer;

  // Construtor
  Consumidor (int id, int delayTime, Buffer b) {
    this.id = id;
    this.delay = delayTime;
    this.buffer = b;
  }

  // Método executado pela thread
  public void run () {
    int item;
    try {
      for (;;) {
        item = this.buffer.Remove(this.id);
        sleep(this.delay); //...simula o tempo para fazer algo com o item retirado
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Produtor
class Produtor extends Thread {
  int id;
  int delay;
  Buffer buffer;

  // Construtor
  Produtor (int id, int delayTime, Buffer b) {
    this.id = id;
    this.delay = delayTime;
    this.buffer = b;
  }

  // Método executado pelo thread
  public void run () {
    try {
      for (;;) {
        this.buffer.Insere(this.id, this.id); //simplificacao: insere o proprio ID
        sleep(this.delay);
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Classe principal
class PC {
  static final int P = 5;
  static final int C = 5;

  public static void main (String[] args) {
    int i;
    Buffer buffer = new Buffer();      // Monitor
    Consumidor[] cons = new Consumidor[C];   // Consumidores
    Produtor[] prod = new Produtor[P];       // Produtores

    for (i=0; i<C; i++) {
       cons[i] = new Consumidor(i+1, 1000, buffer);
       cons[i].start(); 
    }
    for (i=0; i<P; i++) {
       prod[i] = new Produtor(i+1, 1000, buffer);
       prod[i].start(); 
    }
  }
}
