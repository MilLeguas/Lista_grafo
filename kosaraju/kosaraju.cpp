#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm> 

using namespace std;

void exibirAjuda() {
    cout << "\nUso: ./kosaraju -f <arquivo_entrada> [-o <arquivo_saida>] [-h]" << endl;
    cout << "Opções:" << endl;
    cout << "  -h\t\t : Mostra esta mensagem de ajuda." << endl;
    cout << "  -o <arquivo>\t : Redireciona a saida para o <arquivo> especificado." << endl;
    cout << "  -f <arquivo>\t : Indica o <arquivo> que contem o grafo de entrada." << endl;
}

void dfs_passo1(int u, vector<vector<int>>& adj, vector<bool>& visitado, stack<int>& pilha) {
    visitado[u] = true;
    for (int v : adj[u]) {
        if (!visitado[v]) {
            dfs_passo1(v, adj, visitado, pilha);
        }
    }
    pilha.push(u);
}

void dfs_passo2(int u, vector<vector<int>>& adjT, vector<bool>& visitado, vector<int>& componente_atual) {
    visitado[u] = true;
    componente_atual.push_back(u);
    for (int v : adjT[u]) {
        if (!visitado[v]) {
            dfs_passo2(v, adjT, visitado, componente_atual);
        }
    }
}

int main(int argc, char* argv[]) {
    string arqEntrada;
    string arqSaida;

    
    for (int i = 1; i < argc; ++i) {
        string arg = string(argv[i]);
        if (arg == "-h") {
            exibirAjuda();
            return 0;
        } else if (arg == "-f" && i + 1 < argc) {
            arqEntrada = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            arqSaida = argv[++i];
        }
    }

    if (arqEntrada.empty()) {
        cerr << "Erro: Arquivo de entrada não especificado. Use -h." << endl;
        return 1;
    }

    ifstream fin(arqEntrada); 
    if (!fin.is_open()) {
        cerr << "Erro ao abrir arquivo de entrada: " << arqEntrada << endl;
        return 1;
    }

    int V, A; 
    fin >> V >> A;

    vector<vector<int>> adj(V);  
    vector<vector<int>> adjT(V); 

    for (int i = 0; i < A; ++i) {
        int u, v;
        fin >> u >> v;
        adj[u - 1].push_back(v - 1);
        adjT[v - 1].push_back(u - 1);
    }
    fin.close();

    
    stack<int> pilha_dfs;
    vector<bool> visitado(V, false);

    for (int i = 0; i < V; ++i) {
        if (!visitado[i]) {
            dfs_passo1(i, adj, visitado, pilha_dfs);
        }
    }

    fill(visitado.begin(), visitado.end(), false);

    vector<vector<int>> componentes;

    while (!pilha_dfs.empty()) {
        int u = pilha_dfs.top();
        pilha_dfs.pop();

        if (!visitado[u]) {
            vector<int> componente_atual;
            dfs_passo2(u, adjT, visitado, componente_atual);
            
            sort(componente_atual.begin(), componente_atual.end());
            
            componentes.push_back(componente_atual);
        }
    }

    sort(componentes.begin(), componentes.end());

    
    if (arqSaida.empty()) {
        for (const auto& comp : componentes) {
            for (size_t i = 0; i < comp.size(); ++i) {
                cout << comp[i] + 1 << (i == comp.size() - 1 ? "" : " ");
            }
            cout << '\n'; 
        }
    } else {
        ofstream fout(arqSaida); 
        if (!fout.is_open()) {
            cerr << "Erro ao criar arquivo de saida: " << arqSaida << endl;
            return 1;
        }
        for (const auto& comp : componentes) {
            for (size_t i = 0; i < comp.size(); ++i) {
                fout << comp[i] + 1 << (i == comp.size() - 1 ? "" : " ");
            }
            fout << '\n'; 
        }
        fout.close();
    }

    return 0;
}