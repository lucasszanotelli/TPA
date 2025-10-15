#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h> // Para SetConsoleOutputCP
#include <time.h>
using namespace std;

struct Aluno {
    Aluno *ante;
    Aluno *prox;

    string matricula;
    string cpf;
    string nome;
    float nota;
    int idade;
    string curso;
    string cidade;
};

struct Alunos {
    Aluno *inicio;
    Aluno *fim;
    int qtd;
};

// Agora é variável global normal, não ponteiro
Alunos alunos;

void inicializar() {
    alunos.inicio = NULL;
    alunos.fim = NULL;
    alunos.qtd = 0;
}

bool inserir_ordenado(Aluno* novo_aluno) {
    if (novo_aluno == NULL) return false;

    if (alunos.inicio == NULL) {
        alunos.inicio = alunos.fim = novo_aluno;
        novo_aluno->ante = novo_aluno->prox = NULL;
        return true;
    }

    // verifica duplicados
    Aluno *atual = alunos.inicio;
    while (atual != NULL) {
        if (atual->matricula == novo_aluno->matricula || atual->cpf == novo_aluno->cpf) {
            return false;
        }
        atual = atual->prox;
    }

    //insere no primeiro elemento da lista
    if (novo_aluno->nome <= alunos.inicio->nome) {
        novo_aluno->prox = alunos.inicio;
        novo_aluno->ante = NULL;
        alunos.inicio->ante = novo_aluno;
        alunos.inicio = novo_aluno;
    }
    //insere no ultimo elemento da lista
    else if (novo_aluno->nome >= alunos.fim->nome) {
        novo_aluno->ante = alunos.fim;
        novo_aluno->prox = NULL;
        alunos.fim->prox = novo_aluno;
        alunos.fim = novo_aluno;
    }
    else {
        atual = alunos.inicio;
        while (atual != NULL && novo_aluno->nome >= atual->nome) {
            atual = atual->prox;
        }
        novo_aluno->prox = atual;
        novo_aluno->ante = atual->ante;
        atual->ante->prox = novo_aluno;
        atual->ante = novo_aluno;
    }
    return true;
}

string termo() {
    string termo;
    cout << "digite o termo: ";
    getline(cin >> ws, termo);
    return termo;
}

bool exclui_aluno() {
    string aluno_excluir = termo();

    if (aluno_excluir.empty() || alunos.qtd == 0) return false;

    Aluno *remover = NULL;

    // único elemento
    if (alunos.inicio->nome == aluno_excluir && alunos.fim->nome == aluno_excluir) {
        remover = alunos.inicio;
        alunos.inicio = alunos.fim = NULL;
    }
    // primeiro elemento
    else if (alunos.inicio->nome == aluno_excluir) {
        remover = alunos.inicio;
        alunos.inicio = alunos.inicio->prox;
        if (alunos.inicio != NULL) {
            alunos.inicio->ante = NULL;
        }
    }
    // último elemento
    else if (alunos.fim->nome == aluno_excluir) {
        remover = alunos.fim;
        alunos.fim = alunos.fim->ante;
        if (alunos.fim != NULL) {
            alunos.fim->prox = NULL;
        }
    }
    // elemento do meio
    else {
        Aluno *atual = alunos.inicio;
        while (atual != NULL && atual->nome != aluno_excluir) {
            atual = atual->prox;
        }
        if (atual == NULL) {
            cout << "não encontrado" << endl;
            return false;
        }
        remover = atual;
        atual->ante->prox = atual->prox;
        if (atual->prox != NULL) {
            atual->prox->ante = atual->ante;
        }
    }

    // libera memória do aluno encontrado
    if (remover != NULL) {
        delete remover;
        alunos.qtd--;
        return true;
    }

    return false;
}


void ler_aluno(string arquivo) {
    ifstream file(arquivo);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Ignora cabeçalho

    while (getline(file, line)) {
        vector<string> colunas;
        size_t start = 0;
        size_t end;
        while ((end = line.find(',', start)) != string::npos) {
            colunas.emplace_back(line.substr(start, end - start));
            start = end + 1;
        }
        colunas.emplace_back(line.substr(start));

        if (colunas.size() < 7) continue;

        Aluno *novo_aluno = new Aluno{
            nullptr, nullptr,
            colunas[0],
            colunas[1],
            colunas[2],
            stof(colunas[3]),
            stoi(colunas[4]),
            colunas[5],
            colunas[6]
        };
        inserir_ordenado(novo_aluno);
        alunos.qtd += 1;
        
        if (alunos.qtd % 100 == 0) {
            cout << "Nº inseridos : " << alunos.qtd << endl;
        }
    }

    file.close();
}
void print_aluno(Aluno *aluno){
    cout << aluno->matricula << " - "
         << aluno->cpf << " - "
         << aluno->nome << " - " 
         << aluno->nota << " - "
         << aluno->idade << " - "
         << aluno->curso << " - " 
         << aluno->cidade << endl;
}
void print_lista(){
    if (alunos.inicio == NULL){
        /* code */
        cout<<"Lista vazia";
        return;
    }else{
        Aluno *atual = alunos.inicio;
        while (atual!=NULL){
            /* code */
            print_aluno(atual);
            atual=atual->prox;
        }
        
    }
    
}

int menu() {
    int opc;
    cout << "1 - inserir" << endl
         << "2 - excluir" << endl
         << "3 - listar"<<endl
         << "0 - sair"<<endl
         << "digite a opção: ";
    cin >> opc;

    return opc;
}

int main() {
    SetConsoleOutputCP(65001);
    inicializar();
    int opc;
    do {
        opc = menu();
        switch (opc) {
        case 1:
            cout << "inserindo alunos..." << endl;
            ler_aluno("../../arquivos/busca_10_alunos.csv");
            cout << "Nº inseridos : " << alunos.qtd << endl;
            break;
        case 2:
            cout << "removendo alunos..." << endl;
            exclui_aluno();
            break;
        case 3:
            cout<<"imprimindo alunos...";
            print_lista();
        default:
            break;
        }
    } while (opc != 0);

    return 0;
}
