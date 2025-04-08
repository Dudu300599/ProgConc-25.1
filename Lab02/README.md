# Análise dos Resultados: Produto Interno Sequencial vs Paralelo

### Observações Utilizando `float`

- O resultado do produto interno calculado de forma sequencial e paralela apresentou pequenas diferenças numéricas.
- Isso acontece porque operações com ponto flutuante (`float`) não são associativas — ou seja, a ordem de soma influencia o resultado final.
- No caso do cálculo paralelo, as somas parciais ocorrem em diferentes threads e em ordens diferentes da versão sequencial.


---

### Observações Utilizando `double`

- A diferença entre o valor sequencial e o paralelo diminuiu consideravelmente.
- O uso de `double` aumenta a precisão dos cálculos e reduz o impacto da ordem de somas nas operações de produto interno.

---

## Conclusão

- O uso de `float` é suficiente em muitos casos, principalmente quando o desempenho e o consumo de memória são mais importantes do que a precisão extrema.
- No entanto, quando se deseja um resultado mais preciso e com menor erro numérico, recomenda-se o uso de `double`.

