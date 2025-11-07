#include <cstdio>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctype.h>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

struct Vertice; 

struct NoVizinho {
    Vertice *PonteiroParavizinho;
    NoVizinho *proximoVizinho;
};

struct Vertice {
    int id;
    NoVizinho *listaDevizinhos;
    NoVizinho * ultimoVizinho;
};

Vertice *grafo;

void inicializa(int tamanho){
    grafo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafo[i].id=i;
        grafo[i].listaDevizinhos = NULL;
        grafo[i].ultimoVizinho = NULL;
    }
}

//assinaturas
void inicializa(int tamanho);
int escolhaInicial();
int gerarNumAleatorio(int limite);
void controle();
void registrarVizinhancaGND(Vertice& x, Vertice& y);
void imprimir(int tamanho);
bool saoVizinhos(Vertice x, Vertice y);
bool classificacaoDoGradoND(int tamanho, int indice);
void leitura_de_arquivo_dot_direcional();
bool percorreGrafo(bool * vetor, int indice);
bool classificacaoDoGrafoD(int tamanho);

int escolhaInicial(){
    system("cls");
    cout << "\n\n\t[1] - Criar um grafo nao direcionado" << endl;
    cout << "\t[2] - Criar um grafo direcionado" << endl;
    cout << "\t[3] - Importar um arquivo .dot" << endl;
    cout << "\tEscolha: " ;
    int op;
    cin >> op;
    return op;
}

int gerarNumAleatorio(int limite){
    int n = 0;
    for (int i = 0; i < limite; i++)
    {
        n = rand() % limite;
    }
    return n;
    
}

bool saoVizinhos(Vertice x, Vertice y){

    if(x.listaDevizinhos == NULL){
        return false;
    }
    else{
        NoVizinho * percorreVizinhanca = x.listaDevizinhos;
    
        while(percorreVizinhanca != NULL){
            if(percorreVizinhanca->PonteiroParavizinho->id == y.id){
                return true;
            }
            else{
                percorreVizinhanca = percorreVizinhanca->proximoVizinho;
            }
        }
    }
    
    cout << "chegou ao final" << endl;
    return false;
};

void registrarVizinhancaGND(Vertice& x, Vertice& y){

    NoVizinho * novox = new NoVizinho;
    novox->PonteiroParavizinho = &y;
    novox->proximoVizinho = NULL;

    NoVizinho * novoy = new NoVizinho;
    novoy->PonteiroParavizinho = &x;
    novoy->proximoVizinho = NULL;

    // para o x
    if(x.listaDevizinhos == NULL){
        x.listaDevizinhos = novox;
        x.ultimoVizinho = novox;
    }
    else{
        x.ultimoVizinho->proximoVizinho = novox;
        x.ultimoVizinho = novox;
    }

    // para o y
    if(y.listaDevizinhos == NULL){
        y.listaDevizinhos = novoy;
        y.ultimoVizinho = novoy;
    }
    else{
        y.ultimoVizinho->proximoVizinho = novoy;
        y.ultimoVizinho = novoy;

    }

}

void registrarVizinhancaGD(Vertice& x, Vertice& y){

    NoVizinho * novox = new NoVizinho;
    novox->PonteiroParavizinho = &y;
    novox->proximoVizinho = NULL;

    // para o x
    if(x.listaDevizinhos == NULL){
        x.listaDevizinhos = novox;
        x.ultimoVizinho = novox;
    }
    else{
        x.ultimoVizinho->proximoVizinho = novox;
        x.ultimoVizinho = novox;
    }

}

void criarGrafoNaoDirecional(int tamanho, int porcentagem){

    int n1, n2;

    for (int i = 0; i < porcentagem; i++)
    {
        n1 = gerarNumAleatorio(tamanho);
        n2 = gerarNumAleatorio(tamanho);
        
        if(n1 == n2){
            i--;
        }
        else if(saoVizinhos(grafo[n1],grafo[n2])){
            i--;
        }
        else{
            registrarVizinhancaGND(grafo[n1],grafo[n2]);
        }
    }
    
}

void criarGrafoDirecional(int tamanho, int porcentagem){

    int n1, n2;

    for (int i = 0; i < porcentagem; i++)
    {
        n1 = gerarNumAleatorio(tamanho);
        n2 = gerarNumAleatorio(tamanho);
        
        if(n1 == n2){
            i--;
        }
        else if(saoVizinhos(grafo[n1], grafo[n2])){
            i--;
        }
        else{
            registrarVizinhancaGD(grafo[n1],grafo[n2]);
        }
    }
    
}

void gerarDotGND(int tamanho){
    ofstream arq("../../grafos/grafoNaoDirecionado.dot");
    if(!arq.is_open()){
        cout << "nao aberto" << endl;
    }
    else{
        arq << "Graph {" << endl;
        for (int i = 0; i < tamanho; i++)
        {
            arq << i << ";" << endl;
        }
        for (int j = 0; j < tamanho; j++)
        {
            NoVizinho * percorre = grafo[j].listaDevizinhos;
            while (percorre != NULL)
            {
                if(percorre->PonteiroParavizinho->id < j){
                    percorre = percorre->proximoVizinho;
                }
                else{
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << endl;
                    percorre = percorre->proximoVizinho;
                }
            }
            
        }
        arq << "}";
    }

    arq.close();
}

void gerarDotGD(int tamanho){
    ofstream arq("../../grafos/grafoDirecionado.dot");
    if(!arq.is_open()){
        cout << "nao aberto" << endl;
    }
    else{
        arq << "Digraph {" << endl;
        for (int i = 0; i < tamanho; i++)
        {
            arq << i << ";" << endl;
        }
        for (int j = 0; j < tamanho; j++)
        {
            NoVizinho * percorre = grafo[j].listaDevizinhos;
            while (percorre != NULL)
            {
                arq << j << " -> " << percorre->PonteiroParavizinho->id << endl;
                percorre = percorre->proximoVizinho;
            }
            
        }
        arq << "}";
    }

    arq.close();
}

void controle(){
    int tamanho = 0; //quantidade de vertices (nos)
    int porcentagem = 0; //quantidade de arestas (ligaçoes)
    int op = escolhaInicial();
    if(op == 1 || op == 2){
        system("cls");
        cout << "\tInforme o tamanho do grafo: ";
        cin >> tamanho;
        inicializa(tamanho);
        cout << "\tInforme a porcentagem do grafo: ";
        cin >> porcentagem;
    }
    switch (op)
    {
        case 1:{ // função para grafos nao direcionados
            porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
            criarGrafoNaoDirecional(tamanho, porcentagem);
            imprimir(tamanho);
            gerarDotGND(tamanho);
            break;
        }
        case 2:
            porcentagem = ((tamanho * (tamanho - 1)) * porcentagem) / 100 ;
            criarGrafoDirecional(tamanho, porcentagem);
            imprimir(tamanho);
            gerarDotGD(tamanho);
            break;

        case 3:
            leitura_de_arquivo_dot_direcional();
            break;
        default:
        cout << "\topcao invalida!!" << endl;
        break;
    }
}

void imprimir(int tamanho){
    for (int i = 0; i < tamanho; i++){
        NoVizinho* atual = grafo[i].listaDevizinhos;
        cout << "\n";
        cout << i << ": - ";
        while (atual != NULL) {
            cout << atual->PonteiroParavizinho->id << " ";
            atual = atual->proximoVizinho;
        }
    }
}

void leitura_de_arquivo_dot_direcional(){

    ifstream arq("../arquivos/arquivo2.dot");
    if(!arq.is_open()){
        cout << "erro ao abrir arquivo!" << endl;
        return;
    } 
    string linha;
    string tpArq;
    string teste;
    string var1;
    string var2;
    string var3;
    
    getline(arq, linha);
    
    istringstream iss(linha);
    
    iss >> tpArq;
    
    int maior = 0;


    do
    {
        iss.clear();
        getline(arq, linha);
        iss.str(linha);
        iss >> var1;
        iss >> var2;//esta aqui apenas para dar erro
        if(stoi(var1) > maior) maior = stoi(var1);
    } while (iss.fail());
    
    int tamanho = maior + 1;

    inicializa(tamanho);
    iss.str(linha); //resetando a linha
    
    if(tpArq == "Graph"){
        do
        {
            iss >> var1;
            iss >> var2;
            iss >> var3;
            if(!iss.fail()){
                registrarVizinhancaGND(grafo[stoi(var1)], grafo[stoi(var3)]);
                getline(arq, linha);
                iss.clear();
                iss.str(linha);
            }
        } while (!iss.fail());
        
        if(classificacaoDoGradoND(tamanho, 0)){
            cout << "\tgrafo nao direcional e conexo" << endl;
        }else{
            cout << "\tgrafo nao direcional e desconexo" << endl;
        }
    }
    else if(tpArq == "Digraph"){
        do
        {
            iss >> var1;
            iss >> var2;
            iss >> var3;
            if(!iss.fail()){
                registrarVizinhancaGD(grafo[stoi(var1)], grafo[stoi(var3)]);
                getline(arq, linha);
                iss.clear();
                iss.str(linha);
        }
        } while (!iss.fail());
        if(classificacaoDoGrafoD(tamanho)){
            cout << "grafo direcional e conexo" << endl;
        }
        else{
            cout << "grafo direcional e desconexo" << endl;
        }
    }  
    else{
        cout << "nao e nem grafo nem digrafo" << endl;
    }  
    
    arq.close(); 
    
}

bool classificacaoDoGrafoD(int tamanho){

    bool var;

    for (int i = 0; i < tamanho; i++)
    {
        var = classificacaoDoGradoND(tamanho, i);
        if(var == false) return false;
    }
    
    return true;

}

bool classificacaoDoGradoND(int tamanho, int indice){

    bool *vetor = new bool[tamanho]();

    percorreGrafo(vetor, indice);

    for (int i = 0; i < tamanho; i++)
    {
        if(vetor[i] == 0) return false; 
    }

    return true;
}

bool percorreGrafo(bool * vetor, int indice){

    if(vetor[indice] == 1){
        return true;
    }

    vetor[indice] = 1;

    NoVizinho * percorre = grafo[indice].listaDevizinhos;

    while (percorre != NULL)
    {
        percorreGrafo(vetor, percorre->PonteiroParavizinho->id);
        percorre = percorre->proximoVizinho;
    }
    
    return true;

}

main(){
    srand(time(NULL));
    
    controle();

    // esta comentado por que eu nao consegui resover o erro de permissao para fazer isso dentro da funcao
    // system("dot -Tpng ../../grafos/grafoDirecionado.dot -o ../../grafos/grafoDirecionado.png");

    return 0;
}