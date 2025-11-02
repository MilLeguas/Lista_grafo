#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void mostrarAjuda() {
    cout << '\n' << "Uso: ./kruskal -f <arquivo> [-o <arquivo>] [-s] [-h]" << '\n';
    cout << "-h\t\t : Mostra esta mensagem de ajuda." << '\n';
    cout << "-o <arquivo>\t : Redireciona a saida para o <arquivo>." << '\n';
    cout << "-f <arquivo>\t : Indica o <arquivo> que contem o grafo de entrada." << '\n';
    cout << "-s\t\t : Mostra a solucao (arvore)." << '\n';
}

struct Aresta {
    int u, v;
    double custo;
    bool operator<(const Aresta& outra) const {
        return custo < outra.custo;
    }
};

int buscarRaiz(int i, vector<int>& parentes) {
    if (parentes[i] == i)
        return i;
    return parentes[i] = buscarRaiz(parentes[i], parentes);
}

void unirConjuntos(int u, int v, vector<int>& parentes, vector<int>& ranking) {
    int raizU = buscarRaiz(u, parentes);
    int raizV = buscarRaiz(v, parentes);

    if (raizU != raizV) {
        if (ranking[raizU] < ranking[raizV]) {
            parentes[raizU] = raizV;
        } else if (ranking[raizU] > ranking[raizV]) {
            parentes[raizV] = raizU;
        } else {
            parentes[raizV] = raizU;
            ranking[raizU]++;
        }
    }
}

int main(int argc, char* argv[]) {
    string arqEntrada;
    string arqSaida;
    bool exibirSolucao = false;

    for (int i = 1; i < argc; ++i) {
        string arg = string(argv[i]);
        if (arg == "-h") {
            mostrarAjuda();
            return 0;
        } else if (arg == "-f" && i + 1 < argc) {
            arqEntrada = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            arqSaida = argv[++i];
        } else if (arg == "-s") {
            exibirSolucao = true;
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

    vector<Aresta> listaArestas;
    vector<int> parentes(numV);
    vector<int> ranking(numV, 0);

    for (int i = 0; i < numV; ++i) {
        parentes[i] = i;
    }

    for (int i = 0; i < numA; ++i) {
        int u, v;
        double p;
        leitor >> u >> v >> p;
        listaArestas.push_back({u - 1, v - 1, p});
    }
    leitor.close();

    sort(listaArestas.begin(), listaArestas.end());

    double custoTotal = 0;
    vector<Aresta> mst;

    for (auto& aresta : listaArestas) {
        if (buscarRaiz(aresta.u, parentes) != buscarRaiz(aresta.v, parentes)) {
            custoTotal += aresta.custo;
            unirConjuntos(aresta.u, aresta.v, parentes, ranking);
            mst.push_back(aresta);
        }
    }

    if (arqSaida.empty()) {
        if (exibirSolucao) {
            for (auto& ramo : mst) cout << "(" << ramo.u + 1 << "," << ramo.v + 1 << ") ";
            cout << '\n';
        } else {
            cout << custoTotal << '\n';
        }
    } else {
        ofstream escritor(arqSaida);
        if (!escritor.is_open()) {
            cerr << "Erro ao abrir o arquivo de saida: " << arqSaida << '\n';
            return 1;
        }
        if (exibirSolucao) {
            for (auto& ramo : mst) escritor << "(" << ramo.u + 1 << "," << ramo.v + 1 << ") ";
            escritor << '\n';
        } else {
            escritor << custoTotal << '\n';
        }
        escritor.close();
    }

    return 0;
}