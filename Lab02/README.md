# Análise dos Resultados: Produto Interno Sequencial vs Paralelo

### Observações Utilizando `float`

- O resultado do produto interno calculado de forma sequencial e paralela apresentou pequenas diferenças numéricas.
---
```bash
Produto Interno (sequencial) = 2772580829962.53271484375000000000000000
Produto Interno (concorrente) = 2772580829964.16455078125000000000000000
Variação Relativa = 0.00000000000058856207900783
```
```bash
Produto Interno (sequencial) = 2771864267780.95751953125000000000000000
Produto Interno (concorrente) = 2771864267783.25000000000000000000000000
Variação Relativa = 0.00000000000082705365316653
```


---

### Observações Utilizando `double`

- A diferença entre o valor sequencial e o paralelo diminuiu de forma significativa
---

```bash
Produto Interno (sequencial) = 2772383992476.08300781250000000000000000
Produto Interno (concorrente) = 2772383992477.00585937500000000000000000
Variação Relativa = 0.00000000000033287292272806
```
```bash
Produto Interno (sequencial) = 2771938693507.64892578125000000000000000
Produto Interno (concorrente) = 2771938693508.60107421875000000000000000
Variação Relativa = 0.00000000000034349548917878
```

## Conclusão

- O uso de `float` é suficiente em muitos casos, principalmente quando o desempenho e o consumo de memória são mais importantes do que a precisão extrema.
- No entanto, quando se deseja um resultado mais preciso e com menor erro numérico, recomenda-se o uso de `double`.

