#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();

    return grafo;
}
void BacktrackingClique(std::vector<int> &cliqueAtual,
                        std::vector<int> &candidatos,
                        const std::vector<std::vector<int>>& grafo,
                        std::set<std::vector<int>>& cliquesMaximos) {
    if (candidatos.empty()) {
        bool isMaximal = true;
        for (int i = 0; i < grafo.size(); ++i) {
            if (std::find(cliqueAtual.begin(), cliqueAtual.end(), i) == cliqueAtual.end()) {
                bool isConnectedToAll = true;
                for (int v : cliqueAtual) {
                    if (grafo[v][i] == 0) {
                        isConnectedToAll = false;
                        break;
                    }
                }
                if (isConnectedToAll) {
                    isMaximal = false;
                    break;
                }
            }
        }
        if (isMaximal) {
            std::sort(cliqueAtual.begin(), cliqueAtual.end());
            cliquesMaximos.insert(cliqueAtual);
        }
        return;
    }

    std::vector<int> candidatosCopy = candidatos;
    while (!candidatosCopy.empty()) {
        int v = candidatosCopy.back();
        candidatosCopy.pop_back();

        std::vector<int> newClique = cliqueAtual;
        newClique.push_back(v);
        
        std::vector<int> newCandidatos;
        for (int u : candidatosCopy) {
            if (grafo[v][u] == 1) {
                newCandidatos.push_back(u);
            }
        }
        
        BacktrackingClique(newClique, newCandidatos, grafo, cliquesMaximos);
    }
}


int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int numGraphs = 100;
    std::ofstream outputFile; // Declare outputFile here

    for (int i = 0; i < 10; ++i) {
        if (world_rank == 0) {
            std::string outputFileName = "../../results/parallel/times/parallel_times_" + std::to_string(i) + ".txt";
            outputFile.open(outputFileName);
        }

        for (int graphIndex = 1; graphIndex <= numGraphs; ++graphIndex) {
            int numVertices;
            std::string nomeArquivo = "../../graphs/s" + std::to_string(i) + "/grafo_" + std::to_string(graphIndex) + ".txt";
            
            std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

            auto start = MPI_Wtime();

            std::set<std::vector<int>> cliquesMaximos;
            for (int v = world_rank; v < grafo.size(); v += world_size) {
                std::vector<int> cliqueAtual = {v};
                std::vector<int> candidatos;
                for (int j = v + 1; j < grafo.size(); j++) {
                    if (grafo[v][j] == 1) {
                        candidatos.push_back(j);
                    }
                }
                BacktrackingClique(cliqueAtual, candidatos, grafo, cliquesMaximos);
            }

            auto end = MPI_Wtime();

            if (world_rank == 0) {
                auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                outputFile << nomeArquivo << " " << duration.count() << " seconds" << std::endl;
                std::cout << "Graph " << graphIndex << " took " << duration.count() << " seconds" << std::endl;
            }
        }

        if (world_rank == 0) {
            outputFile.close();
        }
    }

    MPI_Finalize();
    return 0;
}
