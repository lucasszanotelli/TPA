#include <cstdio>
#include <cstdlib>
#include <string>

struct Aluno{
    std::string matricula;
    std::string cpf[15];
    std::string nome[50];
    float nota;
    int idade;
    std::string curso[20];
    std::string cidade[20];

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
    if (a.head == NULL){ //nessa condição, x é o primeiro e único elemento da lista, logo, seu prox e seu ant é NULL e o início e fim serão iguais
        a.head = a.end = x; 
        x->ant = x->prox = NULL;
    }else if(a.end == NULL) { 

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
