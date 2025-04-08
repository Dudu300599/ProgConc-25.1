# Análise dos Resultados: Produto Interno Sequencial vs Paralelo

### Observações Utilizando `float`

- O resultado do produto interno calculado de forma sequencial e paralela apresentou pequenas diferenças numéricas.
- Isso acontece porque operações com ponto flutuante (`float`) não são associativas — ou seja, a ordem de soma influencia o resultado final.
- No caso do cálculo paralelo, as somas parciais ocorrem em diferentes threads e em ordens diferentes da versão sequencial.
- Utilizando `float`, o erro relativo ficou em torno de `10^-5` a `10^-4` (dependendo do tamanho do vetor utilizado). Esse valor é considerado esperado e aceitável em operações de ponto flutuante.

---

### Observações Utilizando `double`

- A diferença entre o valor sequencial e o paralelo diminuiu consideravelmente.
- O erro relativo ficou em torno de `10^-12` a `10^-15`, praticamente insignificante.
- Isso demonstra que o uso de `double` aumenta a precisão dos cálculos e reduz o impacto da ordem de somas nas operações de produto interno.

---

## Conclusão

- O uso de `float` é suficiente em muitos casos, principalmente quando o desempenho e o consumo de memória são mais importantes do que a precisão extrema.
- No entanto, quando se deseja um resultado mais preciso e com menor erro numérico, recomenda-se o uso de `double`.
- Em cálculos paralelos que envolvem muitas somas (como o produto interno de vetores), o uso de `double` é especialmente recomendado se o problema exigir alta precisão.

