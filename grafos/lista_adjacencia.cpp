#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <ctime>
#include <windows.h>

using namespace std;

struct Grafo {
    vector<string> vertices;               // lista de vértices
    vector<pair<string, string>> arestas;  // lista de arestas
    vector<vector<int>> lista_adj;         // lista de adjacência
    bool direcionado = false;
    int qtd_arestas = 0;
};

// Função para remover espaços extras
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Função para ler o arquivo DOT e preencher o grafo
//lerArquivoDOT("../arquivos/arquivo.dot", grafo);
void lerArquivoDOT(const string &nomeArquivo, Grafo *grafo) {
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    grafo->vertices.clear();
    grafo->arestas.clear();
    grafo->lista_adj.clear();

    while (getline(arquivo, linha)) {
        linha = trim(linha);
        if (linha.empty() || linha.find("graph") != string::npos || linha == "{" || linha == "}")
            continue;
        if (linha.back() == ';') linha.pop_back();

        if (linha.find("--") != string::npos || linha.find("->") != string::npos) {
            stringstream ss(linha);
            string v1, ignore, v2;
            ss >> v1 >> ignore >> v2;

            if (ignore == "->")
                grafo->direcionado = true;

            grafo->arestas.push_back({v1, v2});
            grafo->vertices.push_back(v1);
            grafo->vertices.push_back(v2);
        } else {
            grafo->vertices.push_back(linha);
        }
    }

    arquivo.close();

    // Remove duplicatas
    sort(grafo->vertices.begin(), grafo->vertices.end());
    grafo->vertices.erase(unique(grafo->vertices.begin(), grafo->vertices.end()), grafo->vertices.end());

    // Cria a lista de adjacência vazia
    int n = grafo->vertices.size();
    grafo->lista_adj.assign(n, vector<int>());

    // Preenche a lista de adjacência
    for (auto &a : grafo->arestas) {
        int i = find(grafo->vertices.begin(), grafo->vertices.end(), a.first) - grafo->vertices.begin();
        int j = find(grafo->vertices.begin(), grafo->vertices.end(), a.second) - grafo->vertices.begin();
        if (i < n && j < n) {
            grafo->lista_adj[i].push_back(j);
            if (!grafo->direcionado)
                grafo->lista_adj[j].push_back(i);
        }
    }
}

// Função para verificar se o grafo é conexo
bool ehConexo(Grafo *grafo) {
    int n = grafo->vertices.size();
    if (n == 0) return true;

    vector<bool> visitado(n, false);
    stack<int> s;

    s.push(0);
    visitado[0] = true;

    while (!s.empty()) {
        int v = s.top();
        s.pop();
        for (int viz : grafo->lista_adj[v]) {
            if (!visitado[viz]) {
                visitado[viz] = true;
                s.push(viz);
            }
        }
    }

    for (bool v : visitado)
        if (!v) return false;

    return true;
}

// Função para imprimir a lista de adjacência
void imprime_lista(Grafo *grafo) {
    cout << "\nLista de Adjacência:\n";
    for (int i = 0; i < (int)grafo->vertices.size(); i++) {
        cout << grafo->vertices[i] << " -> ";
        for (int viz : grafo->lista_adj[i])
            cout << grafo->vertices[viz] << " ";
        cout << endl;
    }
}

 // Salvar o grafo en arquivo .dot
void salvarGrafoEmDOT(Grafo *grafo, const string &nomeArquivo) {
    
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << " para escrita.\n";
        return;
    }

    if (grafo->direcionado)
        arquivo << "digraph G {\n";
    else
        arquivo << "graph G {\n";

    string conector = grafo->direcionado ? " -> " : " -- ";
    int n = grafo->vertices.size();

    for (int i = 0; i < n; i++) arquivo << "    " << grafo->vertices[i] << ";\n";

    for (int i = 0; i < n; i++) {
        for (int j : grafo->lista_adj[i]) {
            if (grafo->direcionado || i < j) {
                arquivo << "    " << grafo->vertices[i] << conector << grafo->vertices[j] << ";\n";
            }
        }
    }

    arquivo << "}\n";
    arquivo.close();
    
}


void gerar_lista(Grafo *grafo) {
    int num_vertices, percent_arestas;
    char direcao;

    cout << "Número de vértices: ";
    cin >> num_vertices;
    cout << "\nPorcentagem de arestas (0-100): ";
    cin >> percent_arestas;
    cout << "\nDirecionado (S/N): ";
    cin >> direcao;

    grafo->direcionado = (toupper(direcao) == 'S');

    int max_arestas = grafo->direcionado ?
        (num_vertices * (num_vertices - 1)) :
        (num_vertices * (num_vertices - 1)) / 2;

    int num_arestas = static_cast<int>(max_arestas * (percent_arestas / 100.0));

    grafo->vertices.clear();
    grafo->arestas.clear();
    grafo->lista_adj.clear();

    for (int i = 0; i < num_vertices; i++) {
        string nome(1, 'A' + i);
        grafo->vertices.push_back(nome);
    }

    grafo->lista_adj.assign(num_vertices, vector<int>());

    srand(time(nullptr)); // gerador de numero aleatório com base na hora atal

    int criadas = 0;
    while (criadas < num_arestas) {
        int i = rand() % num_vertices;
        int j = rand() % num_vertices;

        if (i != j && find(grafo->lista_adj[i].begin(), grafo->lista_adj[i].end(), j) == grafo->lista_adj[i].end()) {
            grafo->lista_adj[i].push_back(j);
            if (!grafo->direcionado)
                grafo->lista_adj[j].push_back(i);

            grafo->arestas.push_back({grafo->vertices[i], grafo->vertices[j]});
            criadas++;
        }
    }

    grafo->qtd_arestas = grafo->arestas.size();
    imprime_lista(grafo);

    if (!ehConexo(grafo))
        cout << "O grafo NÃO é conexo.\n";
    else
        cout << "O grafo é conexo!\n";
    salvarGrafoEmDOT(grafo, "../arquivos/arquivo1.dot");
}

// Menu
int menu() {
    cout << "\n==== Menu ====\n";
    cout << "1 - Novo Grafo (gerar aleatório)\n";
    cout << "2 - Ler Grafo Existente (DOT)\n";
    cout << "0 - Sair\n";
    cout << "Opção: ";

    int opcao;
    cin >> opcao;
    return opcao;
}

int main() {
    SetConsoleOutputCP(65001);
    Grafo *grafo = new Grafo;
    int option;

    do {
        option = menu();
        switch (option) {
            case 1:
                gerar_lista(grafo);
                system("dot -Tpng ../arquivos/arquivo1.dot -o ../arquivos/arquivo1.png");
                break;
            case 2:
                lerArquivoDOT("../arquivos/arquivo.dot", grafo);
                imprime_lista(grafo);
                system("dot -Tpng ../arquivos/arquivo.dot -o ../arquivos/arquivo.png");
                cout << (ehConexo(grafo) ? "O grafo é conexo!\n" : "O grafo NÃO é conexo.\n");
                break;
            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while (option != 0);

    delete grafo;
    return 0;
}
