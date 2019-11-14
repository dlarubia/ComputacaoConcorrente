import java.io.*; 
import java.util.*; 

class Vetor {
    int quantidadeElementos;
    int vetor[];

    Vetor(int quantidadeElementos) {
        this.quantidadeElementos = quantidadeElementos;
        this.vetor = new int[quantidadeElementos];
        for(int i = 0; i < quantidadeElementos; i++) {
            this.vetor[i] = i;
        }
    }

    void alteraElemento(int n, int posicao) {
        this.vetor[posicao] = n;
    } 

    void imprimeVetor() {
        System.out.println(Arrays.toString(this.vetor));
    }
}

class SomaVetor implements Runnable {

    public Vetor vetorA;
    public Vetor vetorB;
    public Vetor vetorC;
    public int tid, nthreads;
  
    SomaVetor(Vetor vetorA, Vetor vetorB, Vetor vetorC, int nthreads, int tid) {
        this.tid = tid;
        this.nthreads = nthreads;
        this.vetorA = vetorA;
        this.vetorB = vetorB;
        this.vetorC = vetorC;
    }
    
    public void run() {
        for (int i = tid; i < this.vetorA.vetor.length; i += this.nthreads) {
            this.vetorC.vetor[i] = this.vetorA.vetor[i] + this.vetorB.vetor[i];
        }
    }
}

class Principal {
    static final int N = 10;
    static final Vetor vetorA = new Vetor(20);
    static final Vetor vetorB = new Vetor(20);
    static final Vetor vetorC = new Vetor(20);

    public static void main (String[] args) {
        Thread[] threads = new Thread[N];

        for(int i = 0; i < threads.length; i++) {
            threads[i] = new Thread(new SomaVetor(vetorA, vetorB, vetorC, N, i));
        }

        for(int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        for(int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        vetorA.imprimeVetor();
        vetorB.imprimeVetor();
        vetorC.imprimeVetor();

    }
}