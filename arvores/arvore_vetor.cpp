#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;
#define SIZE 1048576

#define EXCLUI 0
#define BUSCA 1


struct Student {
    string registration;
    string cpf;
    string name;
    float pontuation;
    int age;
    string course;
    string city;

};

struct Node {
    Student* student[SIZE];

    int altura;
    int qtd;
};
Node* node = NULL;

void inicializa() {
    node = new Node;
    node->qtd = 0;
    for (int i = 0; i < SIZE; i++) {
        node->student[i] = nullptr;
    }
}

bool insere_vetor(Student* aluno, Node* no, int indice){
    if (indice >= SIZE){
        return false;// arvore cheia
    }
    if(no->student[indice] == NULL){
        no->student[indice] = aluno;

    }else if (no->student[indice]->name > aluno->name){
        int esq = 2 * indice + 1;
        return insere_vetor(aluno, no, esq);
    }else if (no->student[indice]->name < aluno->name){
        int dir = 2 * indice + 2;
        return insere_vetor(aluno, no, dir);
    }

    node->qtd ++;
    return true;
}

void ler_csv(const string& arq){
    ifstream file(arq);
    if(!file.is_open()){
        cout << "Erro ao abrir o aquivo"<<endl;
        return;
    }
    string linha;
    getline(file, linha);//ignora a primeira linha do arquivo

    while (getline(file, linha)) {
        vector<string> colunas;

        size_t start = 0;
        size_t end;
        while ((end = linha.find(',', start)) != string::npos) {
            colunas.emplace_back(linha.substr(start, end - start));
            start = end + 1;
        }
        colunas.emplace_back(linha.substr(start));

        if (colunas.size() < 7) continue;

        Student *new_student = new Student{
            colunas[0], //matrucula
            colunas[1], //cpf
            colunas[2], //nome
            stof(colunas[3]), //pontuação
            stoi(colunas[4]), //idade
            colunas[5], //curso
            colunas[6] //cidade
            
        };

        insere_vetor(new_student, node, 0);
        if(!insere_vetor(new_student, node, 0)){
            cout << "Vetor cheio, não foi possível inserir o aluno: " << new_student->name << endl;
            break;
        }

    }

}
string what_termo(int modo){
    string perg, termo;
    if(modo == 1){
        perg = "buscar: ";
    }else if(modo == 0){
        perg = "excluir: ";
    }
    cout << "Digite o termo que deseja "<< perg;
    cin >> termo;

    return termo;
}

Node buscar(const string& busca, Node* no){
    if(no == NULL){
        cout << "Nó vazio"<<endl;
        return;
    }
    if(busca == node->student[0]->name){
        cout << "Encontrado na posição 0" << endl;
        return 0;
    }
}

bool remove_aluno(){
    int i;
    string termo = what_termo(EXCLUI);
    if (termo.empty()){
        cout<<"impossível excluir. Termo vazio!"<<endl;
        return false;
    }
    for(i=0; i<SIZE; i++){
        if(node->student[i] != nullptr && node->student[i]->name == termo){
            cout << "Aluno encontrado: "<< node->student[i]->name << endl;

            delete node->student[i];
            node->student[i] = nullptr;
            cout << "Aluno removido com sucesso!" << endl;
        }else{
            cout << "Aluno não encontrado"<<endl;
            return false;
        }
    }
    return true;

}
void print_alunos(Node* no, int indice){
    if(indice >= SIZE || no->student[indice] == NULL){
        return;
    }
    cout << "Nome: " << no->student[indice]->name << ", CPF: " << no->student[indice]->cpf << ", Matrícula: " << no->student[indice]->registration << ", Pontuação: " << no->student[indice]->pontuation << ", Idade: " << no->student[indice]->age << ", Curso: " << no->student[indice]->course << ", Cidade: " << no->student[indice]->city << endl;
    int esq = 2 * indice + 1;
    int dir = 2 * indice + 2;
    print_alunos(no, esq);
    print_alunos(no, dir);
}

int menu(){
    cout << " ==== Menu: ==== \n" << endl;
    cout << "1 - inserir aluno" << endl
         << "2 - Listar todos os alunos" << endl
         << "3 - Quantidade de alunos inseridos" << endl
         << "0 - Sair" << endl
         << "\nOpção: ";

    int opcao;
    cin >> opcao;
    return opcao;
}

int main(){
    SetConsoleOutputCP(65001);
    int opcao;
    do{
        opcao = menu();
        switch (opcao) {
            case 1: {
                cout << "==== 1 - Inserindo alunos ==== " << endl;
                inicializa();
                ler_csv("../../arquivos/alunos_completosV2.csv");
                break;
            }
            case 2:
                cout << "==== 2 - Listando alunos ==== " << endl;
                print_alunos(node, 0);
                break;
            case 3:
                cout << "==== 3 - Alunos inseridos ==== " << endl;
                cout << "QTD alunos inseridos: " << node->qtd<< endl;
                break;
            case 4:
                cout << "==== 4 - Remover Aluno por nome ====" << endl;
                remove_aluno();
                
            case 0:
                cout << "\nSaindo..." << endl;
                break;
            default:
                cout << "\nOpção inválida." << endl;
        }
    } while (opcao != 0);

    return 0;
}