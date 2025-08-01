#include <cstdio>
#include <cstdlib>

struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[50];
    float nota;
    int idade;
    char curso[20];
    char cidade[20];

    Aluno *prox;
    Aluno *ant;
};

struct Alunos{
    Aluno *head;
    Aluno *end;
    int quantidade;
};

Alunos a;

void inicializa(){
    a.quantidade = 0;
    a.head = NULL;
    a.end = NULL;

}


bool inserir(Aluno *x){
    if (x == NULL) return false;

    if(a.quantidade == 0){
        a.head = NULL;
        a.end = NULL;
    }
        
    return true;

}


Aluno *ler_aluno(){
    Aluno *b = new Aluno;

    return b;



}








int main(){
   



    system("pause");
    
}