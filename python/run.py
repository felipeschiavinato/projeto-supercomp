from grapho_gen import grapho_gen
import subprocess

n_graphs = 100
for e in range(10):
    for i in range(n_graphs+1):
        if i>=1:
            grapho_gen(f"s{e}/grafo_{i}.txt", i, 0.7)
            print(f"Grafo {i} em {e} gerado.")



