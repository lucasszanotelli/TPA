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

// Cada célula representa uma aresta (vértice destino)
struct Celula {
    string destino;
    Celula* prox;
};

// Cada vértice tem uma lista encadeada de adjacência
struct Vertice {
    string nome;
    Celula* listaAdj;
};

// Estrutura principal do grafo
struct Grafo {
    vector<Vertice> vertices;
    bool direcionado = false;
    int qtd_arestas = 0;
};

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Busca vértice pelo nome
int encontrarVertice(Grafo *grafo, const string &nome) {
    for (int i = 0; i < (int)grafo->vertices.size(); i++)
        if (grafo->vertices[i].nome == nome)
            return i;
    return -1;
}

// Adiciona uma aresta
void adicionarAresta(Grafo *grafo, const string &origem, const string &destino) {
    int i = encontrarVertice(grafo, origem);
    if (i == -1) {
        grafo->vertices.push_back({origem, nullptr});
        i = grafo->vertices.size() - 1;
    }

    int j = encontrarVertice(grafo, destino);
    if (j == -1) {
        grafo->vertices.push_back({destino, nullptr});
        j = grafo->vertices.size() - 1;
    }

    // Cria célula para o destino
    Celula *nova = new Celula{destino, grafo->vertices[i].listaAdj};
    grafo->vertices[i].listaAdj = nova;

    // Se for não direcionado, adiciona o inverso também
    if (!grafo->direcionado) {
        Celula *nova2 = new Celula{origem, grafo->vertices[j].listaAdj};
        grafo->vertices[j].listaAdj = nova2;
    }

    grafo->qtd_arestas++;
}

void lerArquivoDOT(const string &nomeArquivo, Grafo *grafo) {
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    grafo->vertices.clear();
    grafo->qtd_arestas = 0;

    while (getline(arquivo, linha)) {
        linha = trim(linha);
        if (linha.empty() || linha == "{" || linha == "}") continue;

        if (linha.find("graph") != string::npos) {
            grafo->direcionado = false;
            continue;
        } else if (linha.find("digraph") != string::npos) {
            grafo->direcionado = true;
            continue;
        }

        if (linha.back() == ';') linha.pop_back();

        if (linha.find("--") != string::npos || linha.find("->") != string::npos) {
            stringstream ss(linha);
            string v1, seta, v2;
            ss >> v1 >> seta >> v2;

            adicionarAresta(grafo, v1, v2);
        } else {
            if (encontrarVertice(grafo, linha) == -1)
                grafo->vertices.push_back({linha, nullptr});
        }
    }
    arquivo.close();
}

void imprime_matriz(Grafo *grafo, int size) {
    cout << "\nMatriz de Adjacência:\n   ";
    for (auto &v : grafo->vertices) cout << v << " ";
    cout << endl;

    for (int i = 0; i < size; i++) {
        cout << grafo->vertices[i] << " ";
        for (int j = 0; j < size; j++) {
            cout << " " << grafo->matriz->matriz[i][j];
        }
        cout << endl;
    }
}


void imprimirGrafo(Grafo *grafo) {
    cout << "\nLista de Adjacência:\n";
    for (auto &v : grafo->vertices) {
        cout << v.nome << " -> ";
        Celula *aux = v.listaAdj;
        while (aux) {
            cout << aux->destino;
            if (aux->prox) cout << ", ";
            aux = aux->prox;
        }
        cout << endl;
    }
}

void salvarGrafoEmDOT(Grafo *grafo, const string &nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao salvar arquivo DOT.\n";
        return;
    }

    arquivo << (grafo->direcionado ? "digraph G {\n" : "graph G {\n");
    string seta = grafo->direcionado ? " -> " : " -- ";

    for (auto &v : grafo->vertices) {
        Celula *aux = v.listaAdj;
        while (aux) {
            arquivo << "    " << v.nome << seta << aux->destino << ";\n";
            aux = aux->prox;
        }
        if (v.listaAdj == nullptr)
            arquivo << "    " << v.nome << ";\n";
    }

    arquivo << "}\n";
    arquivo.close();
}

bool ehConexo(Grafo *grafo) {
    if (grafo->vertices.empty()) return true;

    vector<bool> visitado(grafo->vertices.size(), false);
    stack<int> s;
    s.push(0);
    visitado[0] = true;

    while (!s.empty()) {
        int v = s.top();
        s.pop();
        Celula *aux = grafo->vertices[v].listaAdj;
        while (aux) {
            int j = encontrarVertice(grafo, aux->destino);
            if (j != -1 && !visitado[j]) {
                visitado[j] = true;
                s.push(j);
            }
            aux = aux->prox;
        }
    }

    for (bool v : visitado)
        if (!v) return false;
    return true;
}

void gerarGrafo(Grafo *grafo) {
    int num_vertices, percent_arestas;
    char direcao;

    cout << "Número de vértices: ";
    cin >> num_vertices;
    cout << "\n% de arestas: ";
    cin >> percent_arestas;
    cout << "\nDirecionado (S/N): ";
    cin >> direcao;

    grafo->direcionado = (toupper(direcao) == 'S');
    grafo->vertices.clear();
    grafo->qtd_arestas = 0;

    // Cria vértices
    for (int i = 0; i < num_vertices; i++) {
        string nome(1, 'A' + i);
        grafo->vertices.push_back({nome, nullptr});
    }

    int max_arestas = grafo->direcionado
        ? num_vertices * (num_vertices - 1)
        : (num_vertices * (num_vertices - 1)) / 2;

    int num_arestas = static_cast<int>(max_arestas * (percent_arestas / 100.0));
    srand(time(nullptr));

    while (grafo->qtd_arestas < num_arestas) {
        int i = rand() % num_vertices;
        int j = rand() % num_vertices;
        if (i != j) {
            adicionarAresta(grafo, grafo->vertices[i].nome, grafo->vertices[j].nome);
        }
    }

    imprimirGrafo(grafo);

    if (ehConexo(grafo))
        cout << "O grafo é conexo!\n";
    else
        cout << "O grafo NÃO é conexo!\n";

    salvarGrafoEmDOT(grafo, "../arquivos/arquivo2.dot");
    system("dot -Tpng ../arquivos/arquivo2.dot -o ../arquivos/arquivo2.png");
}

int menu() {
    cout << "\n==== Menu ====\n";
    cout << "1 - Novo Grafo (aleatório)\n";
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
    int opcao;

    do {
        opcao = menu();
        switch (opcao) {
            case 1:
                gerarGrafo(grafo);
                break;
            case 2:
                lerArquivoDOT("../arquivos/arquivo.dot", grafo);
                imprimirGrafo(grafo);
                if (ehConexo(grafo))
                    cout << "O grafo é conexo!\n";
                else
                    cout << "O grafo NÃO é conexo!\n";
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
    } while (opcao != 0);

    delete grafo;
    return 0;
}
