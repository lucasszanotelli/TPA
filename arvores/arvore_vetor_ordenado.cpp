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
    int altura_no;
    int altura_arvore;
};
Student* node[SIZE] = NULL;

bool insere_vetor(Student* aluno, int indice, int alt){
    if (indice >= SIZE){
        return false;// arvore cheia
    }
    if(student[indice] == NULL){
        student[indice] = aluno;
        tudent[indice]->altura_no = alt;
        cout << "altura do nó inserido: "<< student[indice]->altura_no << endl;

    }else if (student[indice]->name > aluno->name){
        int esq = 2 * indice + 1;
        alt += 1;
        return insere_vetor(aluno, esq, alt);

    }else if (student[indice]->name < aluno->name){
        int dir = 2 * indice + 2;
        alt += 1; //adiciona 1 para saber a altura do nó inserido
        return insere_vetor(aluno, dir, alt);
    }
    
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
            colunas[6], //cidade
            0 //altura do nó  
        };

        insere_vetor(new_student, 0, 1);
        if(!insere_vetor(new_student, 0, 1)){
            cout << "Vetor cheio, não foi possível inserir o aluno: " << new_student->name << endl;
            break;
        }
        qtd++;
    }
}

int buscar_menor(int indice_menor){
    if (student[indice_menor]!= NULL){
        return buscar_menor(indice_menor*2+1);
    }
    return indice_menor;
}

bool remove_balanceando(int indice_excluir){
    char res;
    int esq = indice_excluir*2 + 1;
    int dir = indice_excluir*2 + 2;

    if(indice_excluir == -1){
        cout << "Termo não encontrado" << endl;
        return false;
    }
    if (student[esq]==NULL && student[dir]==NULL){
        delete student[indice_excluir];
        return true;
    }else{
        int menor = buscar_menor(indice_excluir);
        if(student[menor*2 + 2] != NULL){
            student[indice_excluir] = student[menor];
            delete student[menor];
            return true;
        }
    }

    return true;

}

bool remove_aluno_simples(int indice_excluir){
    char res;
    if(indice_excluir == -1){
        cout << "Termo não encontrado" << endl;
    }else{
        cout << "Deseja mesmo exluir "<<student[indice_excluir]->name <<" ? S/N" << endl;
        cin >> res;
        if(tolower(res) == 'n'){
            cout << "operação cancelada"<<endl;
        }else{
            delete student[indice_excluir];
            student[indice_excluir] = NULL;
            qtd --;
            cout << "aluno excuido"<<endl;
        }         
    }
    return true;
}

int buscar(const string& busca, int indice) {
    if (student[indice] == NULL || indice >= SIZE) {
        return -1;
    }
    if (busca == student[indice]->name) {
        cout << "Encontrado na posição [" << indice <<"]" << endl;
        return remove_aluno_simples(indice);
    } 
    else if (busca < student[indice]->name) {
        int esq = 2 * indice + 1;
        return buscar(busca, esq);
    } 
    else { 
        int dir = 2 * indice + 2;
        return buscar(busca, dir);
    }
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
    cout << "1 - Inserir aluno" << endl
         << "2 - Listar todos os alunos" << endl
         << "3 - Quantidade de alunos inseridos" << endl
         << "4 - Buscar aluno por nome"<<endl
         << "5 - Altura da árvore"<< endl
         << "0 - Sair" << endl
         << "\nOpção: ";

    int opcao;
    cin >> opcao;
    return opcao;
}

int main(){
    SetConsoleOutputCP(65001);
    inicializa();
    int opcao, alt;
    string termo;
    do{
        opcao = menu();
        switch (opcao) {
            case 1: {
                cout << "==== 1 - Inserindo alunos ==== " << endl;
                ler_csv("../../arquivos/alunos_completosV2.csv");
                break;
            }
            case 2:
                cout << "==== 2 - Listando alunos ==== " << endl;
                print_alunos(node, 0);
                break;
            case 3:
                cout << "==== 3 - Alunos inseridos ==== " << endl;
                cout << "QTD alunos inseridos: " << qtd<< endl;
                break;
            case 4:
                cout << "==== 4 - Buscar alunos ====" << endl;
                termo = what_termo(BUSCA);
                buscar(termo, 0);
                break; 
            case 5:
                cout <<"==== 5 - Altura: ===="<<endl;
                alt = get_atura_arvore();
                cout <<"Altura: "<< alt <<endl;

            case 0:
                cout << "\nSaindo..." << endl;
                break;
            default:
                cout << "\nOpção inválida." << endl;
        }
    } while (opcao != 0);

    return 0;
}