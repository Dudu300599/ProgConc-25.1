import java.util.concurrent.*;
import java.util.*;
import java.lang.Math;

// ----------- Tarefa 1: Soma simples -----------
class MyCallable implements Callable<Long> {
    @Override
    public Long call() {
        long s = 0;
        for (long i = 1; i <= 100; i++) {
            s++;
        }
        return s;
    }
}

// ----------- Tarefa 2: Verificar se um número é primo -----------
class VerificaPrimo implements Callable<Integer> {
    private final int numero;

    public VerificaPrimo(int numero) {
        this.numero = numero;
    }

    @Override
    public Integer call() {
        if (ehPrimo(numero)) {
            return 1; // É primo
        } else {
            return 0; // Não é primo
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

// ----------- Classe principal -----------
public class FutureHello {
    private static final int N = 100_000;      // Limite para primos
    private static final int NTHREADS = 10;    // Número de threads
    private static final int N_SUM = 3;        // Quantas tarefas de soma

    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);

        List<Future<Long>> tarefasSoma = new ArrayList<>();
        List<Future<Integer>> tarefasPrimo = new ArrayList<>();

        // Submete tarefas de soma
        for (int i = 0; i < N_SUM; i++) {
            Callable<Long> tarefa = new MyCallable();
            Future<Long> future = executor.submit(tarefa);
            tarefasSoma.add(future);
        }

        // Submete tarefas de verificação de primos
        for (int i = 1; i <= N; i++) {
            Callable<Integer> tarefa = new VerificaPrimo(i);
            Future<Integer> future = executor.submit(tarefa);
            tarefasPrimo.add(future);
        }

        // Resultado das somas
        long somaTotal = 0;
        for (Future<Long> future : tarefasSoma) {
            try {
                somaTotal += future.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        // Resultado da verificação de primos
        int totalPrimos = 0;
        for (Future<Integer> future : tarefasPrimo) {
            try {
                totalPrimos += future.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        System.out.println("Resultado das " + N_SUM + " somas: " + somaTotal);
        System.out.println("Total de primos entre 1 e " + N + ": " + totalPrimos);

        executor.shutdown();
    }
}

