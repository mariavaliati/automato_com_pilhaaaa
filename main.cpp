
//ira ler todas as cadeias e não só uma//
#include "automato.h"
#include "validacao.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
    Automato automato{};
    string nome;

    cout <<"Digite o nome do arquivo txt do automato\n";
    cin>>nome;

    ler_arquivo(nome, automato);
    if(!validar_automato(automato)) return 1;

    string nomeCadeias;
    cout<<"Digite o nome do arquivo txt das cadeias\n";
    cin>>nomeCadeias;

    ifstream arquivo(nomeCadeias);
    if(!arquivo.is_open()){
        cout<<"Arquivo de cadeias nao encontrado!\n";
        return 1;
    }

    string cadeia;
    while(getline(arquivo, cadeia)){
        // remove o '\r' que sobra em arquivos com final de linha do Windows
        if(!cadeia.empty() && cadeia.back() == '\r') cadeia.pop_back();

        if(cadeia.empty()) continue;

        if(cadeia == "@"){
            cadeia = "";
        }

        cout<<"\nCadeia: "<<(cadeia.empty() ? "@" : cadeia)<<endl;
        
        if(simular(automato, cadeia)){
            cout<<"ACEITA"<<endl;
        }else{
            cout<<"REJEITADA"<<endl;
        }
    }
    arquivo.close();
    return 0;
}
