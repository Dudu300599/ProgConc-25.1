/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;

import java.lang.Math;

// Soma simples
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

// Verificar se um número é primo
class VerificaPrimo implements Callable<Integer> {
    private final int numero;

    public VerificaPrimo(int numero) {
        this.numero = numero;
    }

    @Override
    public Integer call() {
        if (ehPrimo(numero)) {
            return 1;
        } else {
            return 0;
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

public class FutureHello {
    private static final int N = 100_000;   
    private static final int NTHREADS = 10;   
    private static final int N_SUM = 3;        

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

