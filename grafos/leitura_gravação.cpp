#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <stack>
#include <ctime>

using namespace std;

struct Matriz {
    vector<vector<int>> matriz; // matriz dinâmica de adjacência
};

struct Grafo {
    vector<string> vertices;               // lista de vértices
    vector<pair<string, string>> arestas;  // lista de arestas (pares de vértices)
    Matriz *matriz = nullptr;              // ponteiro para a matriz
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
void lerArquivoDOT(const string &nomeArquivo, Grafo *grafo) {
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        retur
        ;
    }

    grafo->vertices.clear();
    grafo->arestas.clear();

    while (getline(arquivo, linha)) {
        linha = trim(linha);
        if (linha.empty() || linha.find("graph") != string::npos || linha == "{" || linha == "}")
            continue;
        if (linha.back() == ';') linha.pop_back();

        if (linha.find("--") != string::npos || linha.find("->") != string::npos) {
            stringstream ss(linha);
            string v1, ignore, v2;
            ss >> v1 >> ignore >> v2;

            if (ignore == "->") {
                grafo->direcionado = true;
                grafo->arestas.push_back({v1, v2});
            } else {
                grafo->arestas.push_back({v1, v2});
            }

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
}

bool ehConexo(Grafo *grafo) {
    int size = grafo->vertices.size();
    if (size == 0) return true;

    vector<bool> visitado(size, false);
    stack<int> s;

    s.push(0);
    visitado[0] = true;

    while (!s.empty()) {
        int v = s.top();
        s.pop();
        for (int i = 0; i < size; i++) {
            if (grafo->matriz->matriz[v][i] != 0 && !visitado[i]) {
                visitado[i] = true;
                s.push(i);
            }
        }
    }

    for (bool v : visitado)
        if (!v) return false;

    return true;
}

void imprime_matriz(Grafo *grafo, int size) {
    cout << "\nMatriz de Adjacência:\n   ";
    for (auto &v : grafo->vertices)
        cout << v << " ";
    cout << endl;

    for (int i = 0; i < size; i++) {
        cout << grafo->vertices[i] << " ";
        for (int j = 0; j < size; j++) {
            cout << " " << grafo->matriz->matriz[i][j];
        }
        cout << endl;
    }
}

void salvarGrafoEmDOT(Grafo *grafo, const string &nomeArquivo) {
    if (grafo->matriz == nullptr) {
        cout << "Erro: o grafo ainda não foi criado!\n";
        return;
    }

    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << " para escrita.\n";
        return;
    }

    if (grafo->direcionado)
        arquivo << "digraph G {\n";
    else
        arquivo << "graph G {\n";

    int n = grafo->vertices.size();

    for (int i = 0; i < n; i++)
        arquivo << "    " << grafo->vertices[i] << ";\n";

    for (int i = 0; i < n; i++) {
        for (int j = (grafo->direcionado ? 0 : i + 1); j < n; j++) {
            if (grafo->matriz->matriz[i][j] == 1) {
                if (grafo->direcionado)
                    arquivo << "    " << grafo->vertices[i] << " -> " << grafo->vertices[j] << ";\n";
                else
                    arquivo << "    " << grafo->vertices[i] << " -- " << grafo->vertices[j] << ";\n";
            }
        }
    }

    arquivo << "}\n";
    arquivo.close();
}

void gerar_matriz(Grafo *grafo) {
    int num_vertices, percent_arestas;
    char direcao;

    cout << "Número de vértices: ";
    cin >> num_vertices;
    cout << "\n% de arestas: ";
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
    if (grafo->matriz != nullptr) {
        delete grafo->matriz;
        grafo->matriz = nullptr;
    }

    for (int i = 0; i < num_vertices; i++) {
        string nome(1, 'A' + i);
        grafo->vertices.push_back(nome);
    }

    grafo->matriz = new Matriz;
    grafo->matriz->matriz.assign(num_vertices, vector<int>(num_vertices, 0));
    srand(time(nullptr));

    int arestas_criadas = 0;
    while (arestas_criadas < num_arestas) {
        int i = rand() % num_vertices;
        int j = rand() % num_vertices;

        if (i != j && grafo->matriz->matriz[i][j] == 0) {
            grafo->matriz->matriz[i][j] = 1;
            if (!grafo->direcionado)
                grafo->matriz->matriz[j][i] = 1;

            grafo->arestas.push_back({grafo->vertices[i], grafo->vertices[j]});
            arestas_criadas++;
        }
    }

    grafo->qtd_arestas = grafo->arestas.size();
    imprime_matriz(grafo, num_vertices);

    if (!ehConexo(grafo))
        cout << "O grafo NÃO é conexo.\n";
    else
        cout << "O grafo é conexo!\n";

    salvarGrafoEmDOT(grafo, "../arquivos/arquivo2.dot");
    system("dot -Tpng ../arquivos/arquivo2.dot -o ../arquivos/arquivo2.png");
}

void montarMatriz(Grafo *grafo) {
    int size = grafo->vertices.size();

    if (grafo->matriz != nullptr) {
        delete grafo->matriz;
        grafo->matriz = nullptr;
    }

    grafo->matriz = new Matriz;
    grafo->matriz->matriz.assign(size, vector<int>(size, 0));

    for (auto &a : grafo->arestas) {
        int i = find(grafo->vertices.begin(), grafo->vertices.end(), a.first) - grafo->vertices.begin();
        int j = find(grafo->vertices.begin(), grafo->vertices.end(), a.second) - grafo->vertices.begin();

        if (i < size && j < size) {
            grafo->matriz->matriz[i][j] = 1;
            if (!grafo->direcionado)
                grafo->matriz->matriz[j][i] = 1;
        }
    }

    imprime_matriz(grafo, size);

    if (!ehConexo(grafo))
        cout << "O grafo NÃO é conexo.\n";
    else
        cout << "O grafo é conexo!\n";
}

int menu() {
    cout << "\n==== Menu ====\n";
    cout << "1 - Novo Grafo (gerar aleatório)\n";
    cout << "2 - Ler Grafo Existente (DOT)\n";
    cout << "3 - Gerar Imagem do Grafo\n";
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
                gerar_matriz(grafo);
                break;
            case 2:
                lerArquivoDOT("../arquivos/arquivo.dot", grafo);
                montarMatriz(grafo);
                break;
            case 3:
                system("dot -Tpng ../arquivos/arquivo.dot -o ../arquivos/arquivo.png");
                cout << "Imagem gerada em ../arquivos/arquivo.png\n";
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
