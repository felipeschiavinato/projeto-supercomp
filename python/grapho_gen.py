import networkx as nx

def grapho_gen(nome_arquivo, num_vertices, probabilidade_conexao):

    path = f"../graphs/{nome_arquivo}"

    grapho = nx.fast_gnp_random_graph(num_vertices, probabilidade_conexao)
    with open(path, 'w') as arquivo:
        arquivo.write(f"{num_vertices} {grapho.number_of_edges()}\n")
        for aresta in grapho.edges():
            arquivo.write(f"{aresta[0]+1} {aresta[1]+1}\n")
    print(f"Grafo densamente conectado gerado e salvo em '{nome_arquivo}'.")
