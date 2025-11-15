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
struct Vertice;

struct Vizinho{
    Vertice *vizinho;
    Vizinho *prox;
    int peso;
};

struct Vertice{
    int valor;
    Vizinho *vizinhos;
};

struct Grafo {
    vector<Vertice> vertices;               // lista de vértices
    bool direcionado = false;
};

//===============================================

void inicializa(Grafo *grafo, int tamanho){

    grafo->direcionado = false;
    grafo->vertices.resize(tamanho);

    for (int i = 0; i < tamanho; i++) {
        grafo->vertices[i].valor = i;
        grafo->vertices[i].vizinhos = nullptr;
    }

}

void adicionaVizinho(Vertice &origem, Vertice *destino, int peso) {
    Vizinho *novo = new Vizinho;
    novo->vizinho = destino;
    novo->peso = peso;
    novo->prox = origem.vizinhos;
    origem.vizinhos = novo;
}


void imprime_vertice(const Vertice &vertice) {
    cout << vertice.valor << " - ";

    Vizinho *atual = vertice.vizinhos;

    while (atual != nullptr) {
        cout << atual->vizinho->valor << "(" << atual->peso << ") ";
        atual = atual->prox; // avança na lista encadeada
    }

    cout << endl;
}

void imprime_grafo(Grafo *grafo){
    for (const Vertice &v : grafo->vertices) {
        imprime_vertice(v);
    }
}



void salvar_DOT(Grafo *grafo, const string &nomeDOT, int qtd_vertices) {
    ofstream arquivo(nomeDOT);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo " << nomeDOT << " para escrita.\n";
        return;
    }

    string ligacao;
    if (grafo->direcionado) {
        arquivo << "digraph G {\n";
        ligacao = " -> ";
    } else {
        arquivo << "graph G {\n";
        ligacao = " -- ";
    }

    // escreve vértices
    for (int i = 0; i < qtd_vertices; i++) {
        arquivo << "    " << grafo->vertices[i].valor << ";\n";
    }

    // escreve arestas
    for (int i = 0; i < qtd_vertices; i++) {
        Vizinho *atual = grafo->vertices[i].vizinhos;

        while (atual != nullptr) {
            int origem = grafo->vertices[i].valor;
            int destino = atual->vizinho->valor;
            int peso = atual->peso;

            // Evita duplicar arestas no grafo não direcionado
            if (grafo->direcionado || origem < destino) {
                arquivo << "    " << origem << ligacao << destino
                        << " [label=" << peso << ", weight=" << peso << "];\n";
            }

            atual = atual->prox;
        }
    }

    arquivo << "}\n";
    arquivo.close();

    cout << "Arquivo " << nomeDOT << " salvo com sucesso!\n";
    system("dot -Tpng ../arquivos/arquivo3.dot -o ../arquivos/arquivo3.png");

}

void gerar_prin(Grafo *grafo, Grafo *grafo_PRIM){
    int tamanho_grafo = grafo->vertices.size();
    if(tamanho_grafo==0 || !grafo->direcionado){
        cout << "IMPOSSÍVEL GERAR PRIM";
        return;
    }

    inicializa(grafo_PRIM, tamanho_grafo);
    if(!grafo->direcionado){
        cout << "O grafo é direcionado, não é possível construir PRIM";
        return;
    }

    

}

bool existeAresta(Grafo *grafo,int a, int b) {
    Vizinho* atual = grafo->vertices[a].vizinhos;

    while (atual != nullptr) {
        if (atual->vizinho->valor == b) {
            return true;
        }
        atual = atual->prox;
    }
    return false;
}

void gerar_grafo(Grafo *grafo){
    int num_vertices, percent_arestas;
    char direcao;

    cout << "Número de vértices: ";
    cin >> num_vertices;
    cout << "\nPorcentagem de arestas (0-100): ";
    cin >> percent_arestas;
    cout << "\nDirecionado (S/N): ";
    cin >> direcao;

    grafo->direcionado = (toupper(direcao) == 'S');// torna true o grafo->direcionado
    grafo->vertices.clear();
    // grafo->vertices.resize(num_vertices);

    inicializa(grafo, num_vertices);
    
    // for (int i = 0; i < num_vertices; i++) {
    //     grafo->vertices[i].valor = i;
    //     grafo->vertices[i].vizinhos = nullptr;
    // }

    // número máximo e real de arestas
    int max_arestas = grafo->direcionado ?
        (num_vertices * (num_vertices - 1)) :
        (num_vertices * (num_vertices - 1)) / 2;

    int num_arestas = static_cast<int>(max_arestas * (percent_arestas / 100.0));

    srand(time(nullptr)); // semente para aleatoriedade

    // gera arestas aleatórias
    for (int i = 0; i < num_arestas; i++) {
        int a = rand() % num_vertices;
        int b = rand() % num_vertices;
        int peso = 1 + rand() % 10; // peso entre 1 e 10

        if (a == b) { // evita laços
            i--;
            continue;
        }
        if(existeAresta(grafo, a, b)){
            i--;
            continue;
        }

        // adiciona aresta
        adicionaVizinho(grafo->vertices[a], &grafo->vertices[b], peso);

        if (!grafo->direcionado) {
            adicionaVizinho(grafo->vertices[b], &grafo->vertices[a], peso);
        }

        
    }

    cout << "\nGrafo gerado com sucesso!" << endl;
    imprime_grafo(grafo); // mostra o grafo completo
    //imprime_grafo();
    salvar_DOT(grafo,"../arquivos/arquivo3.dot", num_vertices-1);
    
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
    Grafo *grafo_PRIM = new Grafo;
    int option;

    do {
        option = menu();
        switch (option) {
            case 1:
                gerar_grafo(grafo);
                system("dot -Tpng ../arquivos/arquivo1.dot -o ../arquivos/arquivo1.png");
                break;
            case 2:
                //lerDOT("../arquivos/arquivo.dot");
                //imprime_lista();
                system("dot -Tpng ../arquivos/arquivo.dot -o ../arquivos/arquivo.png");
                //cout << (ehConexo() ? "O grafo é conexo!\n" : "O grafo NÃO é conexo.\n");
                break;
            case 3:
                gerar_prin(grafo, grafo_PRIM);
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

