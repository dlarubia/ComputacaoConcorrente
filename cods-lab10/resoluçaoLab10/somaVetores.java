class vetor {
    int vetor[] = new int[10];
    vetor = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    public vetor(int tid) {
        for(int i = 0; i < this.vetor.lenght; i++) {
            this.vetor[i] = this.vetor[i] * tid;
        } 
    }

    public imprime() {
        for (int i = 0; i < this.vetor.lenght; i++) {
            system.out.print(" %d, ", this.vetor[i]);
        }
    }

    public void run(int vetorA[], int vetorB[], int tid, int nthreads) {
        int bloco = vetorA.lenght / nthreads;
        for(int i = tid * bloco ; i < bloco * (tid + 1) ; i++) {
            vetorC[i] = vetorA[i] + vetorB[i];
        }
    }

    public void somaVetor(int vetorA[], int vetorB[], int vetorC[], i) {
        vetorC[i] = vetorA[i] + vetorB[i];
    }
}

class somaVetor extends Runnable {

    int vetorC[] = new int[10];

    
}