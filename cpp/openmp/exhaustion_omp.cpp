#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>
#include <omp.h>


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

std::set<std::vector<int>> EncontrarCliquesMaximos(const std::vector<std::vector<int>>& grafo) {
    std::set<std::vector<int>> cliquesMaximos;
    #pragma omp parallel
    {
        std::set<std::vector<int>> cliquesMaximosLocal;
        #pragma omp for nowait
        for (int v = 0; v < grafo.size(); v++) {
            std::vector<int> cliqueAtual = {v};
            std::vector<int> candidatos;
            for (int i = v + 1; i < grafo.size(); i++) {
                if (grafo[v][i] == 1) {
                    candidatos.push_back(i);
                }
            }
            BacktrackingClique(cliqueAtual, candidatos, grafo, cliquesMaximosLocal);
        }
        #pragma omp critical
        cliquesMaximos.insert(cliquesMaximosLocal.begin(), cliquesMaximosLocal.end());
    }
    return cliquesMaximos;
}


int main() {
    int numGraphs = 100;

    for (int i = 3; i < 10; ++i) {
        std::string outputFileName = "../../results/openmp/times/openmp_times_" + std::to_string(i) + ".txt";

        std::ofstream outputFile(outputFileName);

        for (int graphIndex = 1; graphIndex <= numGraphs; ++graphIndex) {
            int numVertices;
            std::string nomeArquivo = "../../graphs/s"+ std::to_string(i) +  "/grafo_" + std::to_string(graphIndex) + ".txt";

            std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

            auto start = std::chrono::high_resolution_clock::now();
            std::set<std::vector<int>> cliquesMaximais = EncontrarCliquesMaximos(grafo);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;

            // Get the number of threads used
            int numThreads = 0;
            #pragma omp parallel
            {
                #pragma omp single
                numThreads = omp_get_num_threads();
            }

            outputFile << nomeArquivo << " " << duration.count() << " " << numThreads << std::endl;

            // PRINT graph index, time and number of threads
            std::cout << "Graph " << graphIndex << " took " << duration.count() << " seconds using " << numThreads << " threads" << std::endl;
        }

        outputFile.close();
    }

    return 0;
}
