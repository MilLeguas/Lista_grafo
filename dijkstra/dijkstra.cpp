#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <utility> 

using namespace std;

using Aresta = pair<double, int>;

void mostrarAjuda() {
    cout << '\n' << "Uso: ./dijkstra -f <arquivo> [-o <arquivo>] [-i <vertice>] [-h]" << '\n' << '\n';
    cout << "-h\t\t : Mostra esta mensagem de ajuda." << '\n';
    cout << "-o <arquivo>\t : Redireciona a saida para o <arquivo>." << '\n';
    cout << "-f <arquivo>\t : Indica o <arquivo> que contem o grafo de entrada." << '\n';
    cout << "-i <vertice>\t : Indice do vertice inicial (padrao: 1)." << '\n' << '\n';
}

void dijkstra(vector<vector<Aresta>>& grafo, int origem, ostream& streamSaida) {
    int numV = grafo.size();
    vector<double> distancias(numV, -1.0);
    
    priority_queue<Aresta, vector<Aresta>, greater<Aresta>> filaPrio;

    distancias[origem] = 0.0;
    filaPrio.push({0.0, origem});

    while (!filaPrio.empty()) {
        double distAtual = filaPrio.top().first;
        int u = filaPrio.top().second;
        filaPrio.pop();

        if (distAtual > distancias[u] && distancias[u] != -1.0) {
            continue;
        }

        for (auto& aresta : grafo[u]) {
            double pesoAresta = aresta.first;
            int v = aresta.second;

            if (distancias[v] == -1.0 || distancias[v] > distancias[u] + pesoAresta) {
                distancias[v] = distancias[u] + pesoAresta;
                filaPrio.push({distancias[v], v});
            }
        }
    }

    for (int i = 0; i < numV; ++i) {
        streamSaida << i + 1 << ":" << distancias[i] << " ";
    }
    streamSaida << '\n';
}

int main(int argc, char* argv[]) {
    string arqEntrada;
    string arqSaida;
    int vInicial = 0; 

    for (int i = 1; i < argc; ++i) {
        string arg = string(argv[i]);
        if (arg == "-h") {
            mostrarAjuda();
            return 0;
        } else if (arg == "-f" && i + 1 < argc) {
            arqEntrada = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            arqSaida = argv[++i];
        } else if (arg == "-i" && i + 1 < argc) {
            vInicial = stoi(string(argv[++i])) - 1;
        }
    }

    if (arqEntrada.empty()) {
        cerr << "Arquivo de entrada ausente. Use -h para ajuda." << '\n';
        return 1;
    }

    ifstream leitor(arqEntrada);
    if (!leitor.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada: " << arqEntrada << '\n';
        return 1;
    }

    int numV, numA;
    leitor >> numV >> numA;
    vector<vector<Aresta>> listaAdj(numV);

    for (int i = 0; i < numA; ++i) {
        int u, v;
        double p;
        leitor >> u >> v >> p;
        listaAdj[u - 1].push_back({p, v - 1});
        listaAdj[v - 1].push_back({p, u - 1});
    }
    leitor.close();

    if (arqSaida.empty()) {
        dijkstra(listaAdj, vInicial, cout);
    } else {
        ofstream escritor(arqSaida);
        if (!escritor.is_open()) {
            cerr << "Erro ao abrir o arquivo de saida: " << arqSaida << '\n';
            return 1;
        }
        dijkstra(listaAdj, vInicial, escritor);
        escritor.close();
    }

    return 0;
}