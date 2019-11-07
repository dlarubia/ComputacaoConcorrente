class Vetor {
    int vetor[] = new int[10];
    vetor = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    public Vetor(int tid) {
        for(int i = 0; i < vetor[].lenght; i++) {
            defineElemento(i * this.vetor[i]);
            // vetor[i] = i * this.vetor[i];
        }
    }


    public void defineElemento(int i) {
        this.vetor[i] = i;
    }

    public void imprimeVetor() {
        for(int i = 0; i < this.vetor.lenght, i++) {
            system.out.println(" %d," vetor[i]);
        }
    }


    public int tamanho() {
        return this.vetor.lenght;
    }



}

class SomaVetores extends Thread {
    static final int N = 2;

    public static void main (String[] args) {
        Thread[] threads = new Thread[N];
        Vetor A = new Vetor[];


        for (int i = 0; i < threads.lenght; i++) {
            threads[i] = new Vetor(i);
            system.out.print("Vetor da Thread %d: ", i);
            threads[i].imprimeVetor();
        }

        for (int i = 0; i < threads.lenght; i++) {
            threads[i].start();
        }

        for (int i = 0; i < threads.lenght; i++) {
            try { 
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
    }

}
