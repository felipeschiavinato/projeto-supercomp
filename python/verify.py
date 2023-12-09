import networkx as nx

# Nome do arquivo de entrada
nome_arquivo = "grafo.txt"

# Abrir o arquivo e pular a primeira linha
with open(nome_arquivo, 'r') as arquivo:
    next(arquivo)  # Pula a primeira linha

    # Lê o grafo a partir das linhas restantes
    G = nx.parse_adjlist(arquivo)

# Encontrar todas as cliques maximais
cliques_maximais = list(nx.find_cliques(G))

for clique in cliques_maximais:
    int_clique_maxima = []
    for clique_item in clique:
        clique_item = int(clique_item)
        int_clique_maxima.append(clique_item)
    int_clique_maxima.sort()

    print("Cliques maximais encontradas:", int_clique_maxima)

# Encontrar a clique máxima (a maior)
clique_maxima = max(cliques_maximais, key=len)
int_clique_maxima = []
for item in clique_maxima:
    item = int(item)
    int_clique_maxima.append(item)
int_clique_maxima.sort()
print("Clique máxima encontrada:", int_clique_maxima)