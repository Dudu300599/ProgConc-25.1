# Análise dos Resultados: Produto Interno Sequencial vs Paralelo

### Observações Utilizando `float`

- O resultado do produto interno calculado de forma sequencial e paralela apresentou pequenas diferenças numéricas.

Produto Interno (sequencial) = 2772580829962.53271484375000000000000000
Produto Interno (concorrente) = 2772580829964.16455078125000000000000000
Variação Relativa = 0.00000000000058856207900783

---

### Observações Utilizando `double`

- A diferença entre o valor sequencial e o paralelo diminuiu de forma significativa
---

## Conclusão

- O uso de `float` é suficiente em muitos casos, principalmente quando o desempenho e o consumo de memória são mais importantes do que a precisão extrema.
- No entanto, quando se deseja um resultado mais preciso e com menor erro numérico, recomenda-se o uso de `double`.

