#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

class Pessoa{
protected:
    string cpf;
    string nome;
    vector<char> reconhecimento_facial;
    int id;
    list<Pessoa*> parentes; 

public:
    Pessoa(string cpf, string nome, int id){
        this->cpf = cpf;
        this->nome = nome;
        this->id = id;

        reconhecimento_facial.resize(70);
        for(int i = 0; i < 70; i++){
            reconhecimento_facial[i] = rand() % 256;
        }
    }

    string get_nome(){
        return nome;
    }

    string get_cpf(){
        return cpf;
    }

    vector<char> get_reconhecimento_facial(){
        return reconhecimento_facial;
    }

    void adicionarParente(Pessoa* p){
        parentes.push_back(p);
    }
    
};

class Ingresso{
private:
    int numero;
    Pessoa* p;
    bool revistado;

public:
    Ingresso(int numero, Pessoa* p){
        this -> numero = numero;
        this -> p = p;
        this -> revistado = false;
    }

    int get_numero(){
        return numero;
    }

    Pessoa* get_Pessoa(){
        return p;
    }
    
    void revistar(){
        revistado = true;
        cout << "Revistado: CPF enviado ao sistema: " << p->get_cpf() << endl;
    }
};

class Ocorrencia{
private:
    int id;
    Pessoa* p;
    string detalhes_da_ocorrencia;

public:
    Ocorrencia(int id, Pessoa* p, string detalhes_da_ocorrencia){
        this -> id = id;
        this -> p = p;
        this -> detalhes_da_ocorrencia = detalhes_da_ocorrencia;
    }
    Pessoa* getPessoa(){
        return p;
    }

    string getDescricao(){
        return detalhes_da_ocorrencia;
    }

    void printar_ocorrencia(){
        cout << "Ocorrencia : " << detalhes_da_ocorrencia << " | Pessoa envolvida na ocorrencia : " << p->get_nome() << endl;
    }

};

class Sistema{
private:
    vector<Pessoa*> pessoas;
    vector<Ingresso*> ingressos;
    vector<Ocorrencia*> ocorrencias;

public: 
    ~Sistema() {
    for (auto p : pessoas) delete p;
    for (auto i : ingressos) delete i;
    for (auto o : ocorrencias) delete o;
    }


    Pessoa * create_person(string cpf, string nome, int id){
        Pessoa* p = new Pessoa(cpf, nome, id);
        pessoas.push_back(p);
        return p;
    }

    Ingresso * vender_ingresso(int numero, Pessoa * p){
        Ingresso* i = new Ingresso(numero, p);
        ingressos.push_back(i);
        return i;
    }

    void revistar_pessoa(int numero){
        for(auto i: ingressos ){
            if(i->get_numero() == numero){
                i-> revistar();
            }
        }
    }

    void gerarOcorrencia(int id, Pessoa * p,string detalhes_da_ocorrencia){
        Ocorrencia * o = new Ocorrencia(id, p, detalhes_da_ocorrencia);
        ocorrencias.push_back(o);
    }

    void gerar_arquivo(){
        ofstream arquivo("dados.txt");
        for (auto p : pessoas){
            arquivo << "Pessoa : " << p->get_nome()
                << " | CPF: " << p->get_cpf()
                << " | Leitura Facial : ";

            vector <char> temp = p->get_reconhecimento_facial();
            for (auto c: temp){
                arquivo << (int)c << " ";
            }
            arquivo << endl;
        }
        arquivo.close();   
    }

    void checar_ocorrencias(){
        if (!ocorrencias.empty()){
            ofstream arq("Ocorrencias.txt");
            cout << "Ocorrencias encontradas!" << endl;
            for (auto o : ocorrencias){
                arq << "Pessoa : " << o->getPessoa()->get_nome()
                << " | CPF: " << o->getPessoa()->get_cpf()
                << " | Descricao: " << o->getDescricao()
                << endl;
            }
            arq.close();
        }
        else{
            cout << "Nenhuma ocorrencia resgistrada no evento!" << endl;
        }
    }
};

int main() {
    srand(time(0));
    Sistema sistema;

    Pessoa* p1 = sistema.create_person("12345678900", "Felipe", 1);
    Pessoa* p2 = sistema.create_person("98765432100", "Julia", 2);

    p1->adicionarParente(p2);
    p2->adicionarParente(p1);

    sistema.vender_ingresso(1001, p1);
    sistema.vender_ingresso(1002, p2);

    sistema.revistar_pessoa(1001);
    sistema.gerar_arquivo();



    sistema.gerarOcorrencia(1, p1, "Documento suspeito");
    sistema.gerarOcorrencia(2, p2, "Comportamento estranho");

    sistema.checar_ocorrencias();


    return 0;
}