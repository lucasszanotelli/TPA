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

struct Vizinho{
    Vertice *vizinho;
    Vizinho *prox;
    int peso;
}

struct Vertice{
    int valor;
    Vizinho *vizinhos;
}

struct Grafo {
    vector<Vertice> vertices;               // lista de vértices
    bool direcionado = false;
    int qtd_arestas = 0;
};
//===============================================


// Função para remover espaços extras
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Função para ler o arquivo DOT e preencher o grafo
// lerArquivoDOT("../arquivos/arquivo.dot", grafo);
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
    grafo->weight.clear();

    while (getline(arquivo, linha)) {
        linha = trim(linha);
        if (linha.empty() || linha.find("graph") != string::npos || linha == "{" || linha == "}")
            continue;
        if (linha.back() == ';') linha.pop_back();

        if (linha.find("--") != string::npos || linha.find("->") != string::npos) {
            stringstream ss(linha);
            string v1, ignore, v2;
            ss >> v1 >> ignore >> v2;

            if (ignore == "->")grafo->direcionado = true;
            
            int peso = 1;
            size_t posLabel = linha.find("label=");
            size_t posWeight = linha.find("weigth=");
            if (posWeight != string::npos) {
                peso = stoi(linha.substr(posWeight + 7)); // depois de 'weight='
            } else if (posLabel != string::npos) {
                peso = stoi(linha.substr(posLabel + 6)); // depois de 'label='
            }

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
    cout <<"==== INFORMAÇÕES DO GRAFO ===="<<endl;
    cout << "Tipo: " << (grafo->direcionado ? "Direcionado" : "Não Direcionado") << "\n";
    cout << "Número de vértices: " << grafo->vertices.size() << "\n";
    cout << "Número de arestas: " << grafo->arestas.size() << "\n\n";

    cout << "Vértices:\n";
    for (const auto &v : grafo->vertices)
        cout << v << " ";
    cout << "\n\n";

    // ---- ARESTAS COM PESOS ----
    cout << "Arestas (com pesos):\n";
    for (size_t i = 0; i < grafo->arestas.size(); i++) {
        cout << grafo->arestas[i].first << " - "
             << grafo->arestas[i].second
             << "  (peso = " << grafo->weight[i] << ")\n";
    }

    cout << "\n";
    
    // ---- LISTA DE ADJACÊNCIA ----
    cout << "Lista de adjacência (com pesos):\n";
    for (size_t i = 0; i < grafo->vertices.size(); i++) {
        cout << grafo->vertices[i] << " -> ";
        bool temAdj = false;
        for (size_t j = 0; j < grafo->arestas.size(); j++) {
            // Verifica conexões do vértice atual
            if (grafo->arestas[j].first == grafo->vertices[i]) {
                cout << grafo->arestas[j].second << "(" << grafo->weight[j] << ") ";
                temAdj = true;
            } else if (!grafo->direcionado && grafo->arestas[j].second == grafo->vertices[i]) {
                cout << grafo->arestas[j].first << "(" << grafo->weight[j] << ") ";
                temAdj = true;
            }
        }
        if (!temAdj) cout << "(nenhum)";
        cout << "\n";
    }
}

 // Salvar o grafo en arquivo .dot
void salvarGrafoEmDOT(Grafo *grafo, const string &nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << " para escrita.\n";
        return;
    }

    // Cabeçalho DOT
    if (grafo->direcionado)
        arquivo << "digraph G {\n";
    else
        arquivo << "graph G {\n";

    string conector = grafo->direcionado ? " -> " : " -- ";

    // Escreve vértices
    for (const auto &v : grafo->vertices)arquivo << "    " << v << ";\n";

    // Escreve arestas com peso
    for (size_t i = 0; i < grafo->arestas.size(); i++) {
        string v1 = grafo->arestas[i].first;
        string v2 = grafo->arestas[i].second;
        int peso = grafo->weight[i];

        // Evita duplicar arestas em grafos não direcionados
        if (grafo->direcionado || v1 < v2)
            arquivo << "    " << v1 << conector << v2
                    << " [label=" << peso << ", weight=" << peso << "];\n";
    }

    arquivo << "}\n";
    arquivo.close();

    cout << "Grafo salvo em \"" << nomeArquivo << "\" com sucesso!\n";
}

// Função para gerar grafo aleatório
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
    grafo->weight.clear();

    for (int i = 0; i < num_vertices; i++) {
        string nome(1, 'A' + i);
        grafo->vertices.push_back(nome);
    }

    grafo->lista_adj.assign(num_vertices, vector<int>());

    srand(10);
    //srand(time(NULL));// gerador de numero aleatório com base na hora atal

    int criadas = 0;
    while (criadas < num_arestas) {
        int i = rand() % num_vertices;
        int j = rand() % num_vertices;
        int peso = rand() % 10+1;

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

