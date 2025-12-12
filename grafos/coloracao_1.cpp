#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <ctime>
#include <windows.h>
#include <climits> //INT_MAX

using namespace std;
struct Vertice;

struct Vizinho{
    Vertice *vizinho;
    Vizinho *prox;
    int cor = 0;
    int peso;
};

struct Vertice{
    int valor;
    Vizinho *vizinhos;
    int cor = 0;
};
struct Aresta{
    int origem;
    int destino;
    int peso;
};

struct Grafo{
    vector<Vertice> vertices; // lista de vértices
    vector<Aresta> arestas;   // lista de arestas
    bool direcionado = false;
};

void inicializa(Grafo *grafo, int tamanho){

    grafo->direcionado = false;
    grafo->vertices.resize(tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        grafo->vertices[i].valor = i;
        grafo->vertices[i].vizinhos = nullptr;
    }
}

void adicionaVizinho(Vertice *origem, Vertice *destino, int peso){
    Vizinho *novo = new Vizinho;
    novo->vizinho = destino;
    novo->peso = peso;
    novo->prox = origem->vizinhos;
    origem->vizinhos = novo;
}

bool eh_conexo(Grafo *grafo){
    int tamanho = grafo->vertices.size();

    vector<bool> visitados(tamanho, false);
    stack<int> pilha;
    pilha.push(0); // insere o numero 0 na pilha
    visitados[0] = true;

    while (!pilha.empty())    {
        int first = pilha.top(); // pega o elemento do topo da pilha
        pilha.pop();             // remove o elemento da pilha

        Vizinho *vizinho = grafo->vertices[first].vizinhos;
        while (vizinho != NULL)        {
            int aux = vizinho->vizinho->valor;

            if (!visitados[aux])            {
                visitados[aux] = TRUE;
                pilha.push(aux); // insere o elemento no topo da pilha
            }
            vizinho = vizinho->prox;
        }
    }
    for (bool v : visitados)    {
        if (!v)
            return false;
    }

    return true;
}

void imprime_vertice(const Vertice &vertice){
    cout << vertice.valor << " - ";

    Vizinho *atual = vertice.vizinhos;

    while (atual != nullptr)    {
        cout << atual->vizinho->valor << "(" << atual->peso << ") cor="<<atual->cor;
        atual = atual->prox; // avança na lista encadeada
    }
    cout << endl;
}

void imprime_grafo(Grafo *grafo){
    for (const Vertice &v : grafo->vertices)    {
        imprime_vertice(v);
    }
    if (eh_conexo(grafo))    {
        cout << "O grafo é conexo" << endl;
        return;
    }
    cout << "O grafo NÃO é conexo" << endl;
}

void salvar_DOT(Grafo *grafo, const string &nomeDOT, int qtd_vertices){
    vector<string> lista_cor={"black","red","blue","green","orange","purple","brown","pink","yellow"};

    ofstream arquivo(nomeDOT);
    if (!arquivo.is_open())    {
        cout << "Erro ao abrir o arquivo " << nomeDOT << " para escrita.\n";
        return;
    }

    string ligacao;

    if (grafo->direcionado){
        arquivo << "digraph G {\n";
        ligacao = " -> ";
    }
    else{
        arquivo << "graph G {\n";
        ligacao = " -- ";
    }

    // escreve vértices
    for (int i = 0; i < qtd_vertices; i++){
        arquivo << "    " << grafo->vertices[i].valor << " [color="<< lista_cor[grafo->vertices[i].cor]<<"];\n";
    }

    // escreve arestas
    for (int i = 0; i < qtd_vertices; i++){
        Vizinho *atual = grafo->vertices[i].vizinhos;

        while (atual != nullptr){
            int origem = grafo->vertices[i].valor;
            int destino = atual->vizinho->valor;
            int peso = atual->peso;

            // Evita duplicar arestas no grafo não direcionado
            if (grafo->direcionado || origem < destino){
                if (atual->cor!=0){
                    arquivo << origem << " -- " << destino
                            << " [label=\"" << peso << "\", color="<< lista_cor[atual->cor] <<", penwidth=3.0];\n";
                }
                else{
                    arquivo << origem << " -- " << destino
                            << " [label=\"" << peso << "\"];\n";
                }
            }

            atual = atual->prox;
        }
    }

    arquivo << "}\n";
    arquivo.close();

    cout << "Arquivo " << nomeDOT << " salvo com sucesso!\n";
    system("dot -Tpng ../arquivos/grafoAleatorio.dot -o ../arquivos/grafoAleatorio.png");
}

void ordenarArestas(vector<Aresta> &lista_arestas){
    sort(lista_arestas.begin(), lista_arestas.end(),
         [](const Aresta &a, const Aresta &b)
         {
             return a.peso < b.peso; // ordena da menor para a maior aresta
         });
}

void imprimir_arestas(vector<Aresta> &lista_arestas){
    cout << "Arestas encontradas:\n";
    for (auto &a : lista_arestas){
        cout << a.origem << " -- " << a.destino << " (peso " << a.peso << ")\n";
    }
}

void gerar_dijkstra(Grafo *grafo){
    int tamanho_grafo = grafo->vertices.size();
    int origem, destino;

    if (tamanho_grafo == 0)    {
        cout << "IMPOSSÍVEL GERAR GRAFO DIJKSTRA" << endl;
        return;
    }

    vector<pair<int, int>> iter(tamanho_grafo); // (distancia, predecessor)
    vector<bool> visitado(tamanho_grafo, false);

    for (int i = 0; i < tamanho_grafo; i++)    {
        iter[i].first = INT_MAX;
        iter[i].second = -1;
    }

    cout << "Digite a origem: ";
    cin >> origem;
    cout << "Digite o destino: ";
    cin >> destino;

    iter[origem].first = 0;
    iter[origem].second = -1; // origem não tem pai

    for (int step = 0; step < tamanho_grafo; step++){

        int u = -1;
        int menorDist = INT_MAX;

        for (int v = 0; v < tamanho_grafo; v++){
            if ((!visitado[v]) && (iter[v].first < menorDist)){
                menorDist = iter[v].first;
                u = v;
            }
        }

        if (u == -1)
            break;

        visitado[u] = true;

        Vizinho *viz = grafo->vertices[u].vizinhos;

        while (viz != nullptr){
            int prox = viz->vizinho->valor;
            int peso = viz->peso;

            if (!visitado[prox] && iter[u].first + peso < iter[prox].first){
                iter[prox].first = iter[u].first + peso;
                iter[prox].second = u; // pai
            }

            viz = viz->prox;
        }
    }
    // até aqui eu descobri a distância mínima entre a origem e o destino

    cout << "\nDistância mínima até destino = " << iter[destino].first << endl;

    vector<int> caminho;
    int atual = destino;

    while (atual != -1){
        caminho.push_back(atual);
        atual = iter[atual].second;
    }

    reverse(caminho.begin(), caminho.end()); // inverte o vetor de caminho


    for (int i = 0; i < (int)caminho.size() - 1; i++){
        int u = caminho[i];
        int v = caminho[i + 1];

        // Recuperar peso da aresta original
        Vizinho *viz = grafo->vertices[u].vizinhos;
        while (viz != nullptr && viz->vizinho->valor != v)
            viz = viz->prox;

        if (viz != nullptr){

            Vizinho *p = grafo->vertices[u].vizinhos;
            while (p != nullptr && p->vizinho->valor != v)
                p = p->prox;

            if (p != nullptr){
                p->cor = 1;
            }

            // Se o grafo for não-direcionado marque a outra ponta também
            if (!grafo->direcionado){
                Vizinho *r = grafo->vertices[v].vizinhos;
                while (r != nullptr && r->vizinho->valor != u)
                    r = r->prox;
                if (r != nullptr)
                {
                    r->cor = 1;
                }
            }
        }
    }

    cout << "\nGrafo Dijkstra criado com sucesso!\n";
    imprime_grafo(grafo);
    salvar_DOT(grafo, "../arquivos/DIJKSTRA.dot", grafo->vertices.size());
    system("dot -Tpng ../arquivos/DIJKSTRA.dot -o ../arquivos/DIJKSTRA.png");
}

bool existeAresta(Grafo *grafo, int a, int b){
    Vizinho *atual = grafo->vertices[a].vizinhos;

    while (atual != nullptr){
        if (atual->vizinho->valor == b){
            return true;
        }
        atual = atual->prox;
    }
    return false;
}

void coloracaoDSATUR(Grafo *grafo) {
    int n = grafo->vertices.size();
    if (n == 0) {
        cout << "Grafo vazio. Não é possível colorir DSATUR.\n";
        return;
    }

    // grau dos vértices
    vector<int> grau(n, 0);
    for (int i = 0; i < n; ++i) {
        Vizinho* p = grafo->vertices[i].vizinhos;
        while (p != nullptr) { grau[i]++; p = p->prox; }
    }

    // cores atribuídas (0 = não colorido). Usamos cores >=1
    vector<int> cor(n, 0);

    // conjunto de cores dos vizinhos (para calcular saturação)
    vector<unordered_set<int>> coresVizinhos(n);

    // saturação = número de cores distintas nos vizinhos (size do set)

    // 1) escolhe vértice inicial: maior grau
    int primeiro = 0;
    for (int i = 1; i < n; ++i) if (grau[i] > grau[primeiro]) primeiro = i;

    // colore o primeiro com cor 1
    cor[primeiro] = 1;

    // atualiza conjuntos dos vizinhos
    Vizinho* p = grafo->vertices[primeiro].vizinhos;
    while (p != nullptr) {
        int id = p->vizinho->valor;
        if (coresVizinhos[id].insert(1).second) {
            // inseriu nova cor nos vizinhos
        }
        p = p->prox;
    }

    // 2) enquanto houver vértice não colorido, escolha por maior saturação
    for (int passo = 1; passo < n; ++passo) {
        int escolhido = -1;
        int melhorSatur = -1;
        int melhorGrau = -1;

        for (int v = 0; v < n; ++v) {
            if (cor[v] != 0) continue; // já colorido
            int sat = (int)coresVizinhos[v].size();
            if (escolhido == -1
                || sat > melhorSatur
                || (sat == melhorSatur && grau[v] > melhorGrau)) {
                escolhido = v;
                melhorSatur = sat;
                melhorGrau = grau[v];
            }
        }

        if (escolhido == -1) break; // todos coloridos

        // encontra menor cor disponível (a partir de 1)
        int c = 1;
        while (coresVizinhos[escolhido].count(c)) ++c;
        cor[escolhido] = c;

        // atualiza conjuntos dos vizinhos (adicionar cor quando necessário)
        Vizinho* q = grafo->vertices[escolhido].vizinhos;
        while (q != nullptr) {
            int id = q->vizinho->valor;
            if (cor[id] == 0) {
                coresVizinhos[id].insert(c);
            }
            q = q->prox;
        }
    }

    // copia as cores para o grafo
    for (int i = 0; i < n; ++i) grafo->vertices[i].cor = cor[i];

    cout << "\n=== Coloração DSATUR concluída ===\n";
    salvar_DOT(grafo, "../arquivos/DSATUR.dot", n);
    system("dot -Tpng ../arquivos/DSATUR.dot -o ../arquivos/DSATUR.png");
}


void coloracao_gulosa(Grafo *grafo) {
    int n = grafo->vertices.size();
    if (n == 0) {
        cout << "Grafo vazio. Não é possível colorir.\n";
        return;
    }

    vector<int> cor(n, -1);      // cor de cada vértice
    vector<bool> usado(n, false); // marcadores temporários

    // Vértice 0 recebe cor 0
    cor[0] = 0;

    // Para todos os outros vértices
    for (int u = 1; u < n; u++) {

        // marca cores usadas pelos vizinhos
        Vizinho *v = grafo->vertices[u].vizinhos;
        while (v != nullptr) {
            int idViz = v->vizinho->valor;
            if (cor[idViz] != -1)
                usado[cor[idViz]] = true;
            v = v->prox;
        }
        int corEscolhida = 0;
        while (corEscolhida < n && usado[corEscolhida]) corEscolhida++;//descobre a menor cor disponível
        cor[u] = corEscolhida;
        // limpa o vetor “usado” para próxima iteração
        v = grafo->vertices[u].vizinhos;
        while (v != nullptr) {
            int idViz = v->vizinho->valor;
            if (cor[idViz] != -1)
                usado[cor[idViz]] = false;
            v = v->prox;
        }
    }

    for (int i = 0; i < n; i++)
        grafo->vertices[i].cor = cor[i];

    cout << "\n=== Coloração Gulosa (vértices) ===\n";
    for (int i = 0; i < n; i++)
        cout << "Vértice " << i << " => cor " << cor[i] << endl;

    salvar_DOT(grafo, "../arquivos/GULOSA.dot", n);
    system("dot -Tpng ../arquivos/GULOSA.dot -o ../arquivos/GULOSA.png");

    cout << "\nColoração salva em GULOSA.dot e GULOSA.png\n";
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

    inicializa(grafo, num_vertices);

    grafo->direcionado = (toupper(direcao) == 'S') ? true : false; // torna true o grafo->direcionado se receber sim
    grafo->vertices.clear();
    grafo->vertices.resize(num_vertices);

    for (int i = 0; i < num_vertices; i++){
        grafo->vertices[i].valor = i;
        grafo->vertices[i].vizinhos = nullptr;
    }

    // número máximo e real de arestas
    int max_arestas = grafo->direcionado ? (num_vertices * (num_vertices - 1)) : (num_vertices * (num_vertices - 1)) / 2;

    int num_arestas = static_cast<int>(max_arestas * (percent_arestas / 100.0));

    srand(time(nullptr)); // semente para aleatoriedade

    // gera arestas aleatórias
    for (int i = 0; i < num_arestas; i++){
        int a = rand() % num_vertices;
        int b = rand() % num_vertices;
        int peso = 1 + rand() % 10; // peso entre 1 e 10

        if (a == b){ // evita laços
            i--;
            continue;
        }
        if (existeAresta(grafo, a, b)){
            i--;
            continue;
        }

        // adiciona aresta
        adicionaVizinho(&grafo->vertices[a], &grafo->vertices[b], peso);

        if (!grafo->direcionado){
            adicionaVizinho(&grafo->vertices[b], &grafo->vertices[a], peso);
        }
    }

    cout << "\nGrafo gerado com sucesso!" << endl;
    cout << "Tamanho: " << grafo->vertices.size() << endl;
    imprime_grafo(grafo); // mostra o grafo completo

    salvar_DOT(grafo, "../arquivos/grafoAleatorio.dot", num_vertices);
}

void lerDOT(Grafo *grafo, string nome_arquivo){
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!\n";
        return;
    }

    grafo->vertices.clear();
    string linha;

    // 1 — lê a primeira linha: graph G {
    getline(arquivo, linha);
    grafo->direcionado = (linha.find("digraph") != string::npos);

    vector<int> lista_vertices;

    while (getline(arquivo, linha)){

        linha.erase(remove_if(linha.begin(), linha.end(), ::isspace), linha.end());

        if (linha == "}" || linha.empty())
            continue;

        if (linha.find("--") != string::npos || linha.find("->") != string::npos)
            break;

        if (linha.back() == ';')
            linha.pop_back();

        lista_vertices.push_back(stoi(linha));
    }

    int n = lista_vertices.size();
    inicializa(grafo, n);

    for (int i = 0; i < n; i++){
        grafo->vertices[i].valor = i;
        grafo->vertices[i].vizinhos = nullptr;
    }

    do{
        if (linha.empty() || linha == "}")
            continue;
        if (linha.back() == ';')
            linha.pop_back();

        // se não tem aresta, ignora
        if (linha.find("--") == string::npos && linha.find("->") == string::npos)
            continue;

        // separar v1 e v2
        size_t pos = linha.find("--");
        int offset = 2;

        if (pos == string::npos){
            pos = linha.find("->");
            offset = 2;
        }

        string v1 = linha.substr(0, pos);
        string resto = linha.substr(pos + offset);

        // limpar v1 e v2
        v1.erase(remove_if(v1.begin(), v1.end(), ::isspace), v1.end());
        size_t fim_v2 = resto.find("[");
        string v2 = resto.substr(0, fim_v2);
        v2.erase(remove_if(v2.begin(), v2.end(), ::isspace), v2.end());

        int a = stoi(v1);
        int b = stoi(v2);

        // extrair weight=
        int peso = 1;
        size_t posW = linha.find("weight=");
        if (posW != string::npos){
            posW += 7;
            string num;
            while (posW < linha.size() && isdigit(linha[posW])){
                num.push_back(linha[posW]);
                posW++;
            }
            peso = stoi(num);
        }

        // adicionar aresta
        adicionaVizinho(&grafo->vertices[a], &grafo->vertices[b], peso);
        if (!grafo->direcionado)
            adicionaVizinho(&grafo->vertices[b], &grafo->vertices[a], peso);

    } while (getline(arquivo, linha));

    arquivo.close();

    imprime_grafo(grafo);
}

// Menu
int menu(){
    cout << "\n==== Menu ====\n";
    cout << "1 - Novo Grafo (gerar aleatório)\n";
    cout << "2 - Ler Grafo Existente (DOT)\n";
    cout << "3 - Gerar DIJKSTRA\n";
    cout << "4 - Coloracao Gulosa\n";
    cout << "5 - Coloracao DSATUR\n";
    cout << "0 - Sair\n";
    cout << "Opção: ";

    int opcao;
    cin >> opcao;
    return opcao;
}

int main(){
    SetConsoleOutputCP(65001);
    Grafo *grafo = new Grafo;

    //lerDOT(grafo, "../arquivos/grafoAleatorio.dot");

    int option;

    do{
        option = menu();
        switch (option){
        case 1:
            gerar_grafo(grafo);
            break;
        case 2:
            lerDOT(grafo, "../arquivos/pesos.dot");
            system("dot -Tpng ../arquivos/pesos.dot -o ../arquivos/pesos.png");
            break;
        case 3:
            gerar_dijkstra(grafo);
            break;
        case 4:
            coloracao_gulosa(grafo);
            break;
        case 5:
            coloracaoDSATUR(grafo);
            break;
        case 0:
            cout << "\nSaindo...\n";
            break;
        default:
            cout << "Opção inválida.\n";
        }
    } while (option != 0);

    delete grafo;
    return 0;
}
