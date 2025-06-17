/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;


/*
Classe que implementa um pool de threads para execução concorrente de tarefas (Runnables).
As tarefas são enfileiradas e processadas por um conjunto fixo de threads.
*/
class FilaTarefas {
    private final int nThreads;                   // Número de threads no pool
    private final MyPoolThreads[] threads;        // Array de threads do pool
    private final LinkedList<Runnable> queue;     // Fila de tarefas a serem executadas
    private boolean shutdown;                     // Flag para indicar se o pool está sendo encerrado

    //Construtor da classe. Inicializa a fila e as threads do pool. Recebe o número de threads que a pool irá utilizar
    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    //Enfileira uma nova tarefa para ser executada pelo pool de threads. Implementa a interface runnable
    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    

    //Encerra o pool de threads após todas as tarefas pendentes serem concluídas.
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    /*
    Classe interna que representa as threads do pool.
    Cada thread retira tarefas da fila e as executa.
    */
    private class MyPoolThreads extends Thread {
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty()) return;   
             r = (Runnable) queue.removeFirst();
           }
           try { r.run(); }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class Hello implements Runnable {
   String msg;
   public Hello(String m) { msg = m; }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg);
   }
}

class Primo implements Runnable {
    private final int numero;

    public Primo(int numero) {
        this.numero = numero;
    }

    @Override
    public void run() {
        if (ehPrimo(numero)) {
            System.out.println(numero + " é primo.");
        } else {
            System.out.println(numero + " não é primo.");
        }
    }

    private boolean ehPrimo(int n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        int raiz = (int) Math.sqrt(n);
        for (int i = 3; i <= raiz; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
}

//Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 10;

    public static void main (String[] args) {
      //--PASSO 2: cria o pool de threads
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      
      //--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
      for (int i = 0; i < 25; i++) {
        final String m = "Hello da tarefa " + i;
        Runnable hello = new Hello(m);
        pool.execute(hello);
        Runnable primo = new Primo(i);
        pool.execute(primo);
      }

      //--PASSO 4: esperar pelo termino das threads
      pool.shutdown();
      System.out.println("Terminou");
   }
}

