#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define texto "%[^\n]s"

int contadorPassos = 0; // Variável global para contar os passos

int chamadaFuncao(char *linha) {
	if(strstr(linha,"bool ")==NULL && strstr(linha,"int ")==NULL && strstr(linha,"float ")==NULL &&
		strstr(linha,"char ")==NULL && strstr(linha,"void ")==NULL && strstr(linha,"if")==NULL && strstr(linha,"else")==NULL && strstr(linha,"for(")==NULL &&
		strstr(linha,"while")==NULL &&
		strstr(linha,"(")!=NULL) {
			return 1;
	}
	return 0;
}

bool ehDeclaracao(char *linha) {
	int i = 0, tamanhoLista;
		if((strstr(linha,"bool")!=NULL || strstr(linha,"int")!=NULL || strstr(linha,"float")!=NULL || 
		strstr(linha,"char")!=NULL) && strstr(linha,";")!=NULL && strstr(linha,"(")==NULL && strstr(linha,")")==NULL) {
			return true;
		}
	return false;
}

bool verificaOperadores(char * palavra, char * linha) {
	char operadores[7]={'!','<','>','=','+','-','/'};
	
	int i = 0, tamanhoLista, contadorOperadores = 0;
	for(i=0, tamanhoLista = 7; i<tamanhoLista; i++) {
		while(strchr(palavra,operadores[i])!=NULL) {
			if(strstr(palavra,"include")==NULL) {
				contadorOperadores++;
			}
			palavra++;
		}
	}
	return contadorOperadores;
}

bool verificaLoop(char *linha) {
    if(strstr(linha, "for(") != NULL || strstr(linha, "while") != NULL) {
        return true;
    }
    return false;
}

void percorreArquivo(char nome_arquivo[50]) {
	FILE *file = fopen(nome_arquivo, "r");

	char line[250], aux[250];
	int contadorDeclaracoes = 0;
	int contadorOperadores = 0;
	int contadorChamadasFuncoes = 0;
	int contadorLoops = 0;
		
    while (fgets(line, sizeof(line), file) != NULL) {
    	strcpy(aux,line);
    	bool existeDeclaracaoVariavel = false;
		
        // Usa strtok para separar palavras
        char *token = strtok(aux, ",\t\n,");
 		
        // Itera sobre as palavras da linha
        while (token != NULL) {    
				if(!existeDeclaracaoVariavel) {
					if(ehDeclaracao(line)) {
            			existeDeclaracaoVariavel = true;
					}
				}
		        
				if(existeDeclaracaoVariavel) {
					contadorDeclaracoes++;        	
				}
				else {
					contadorOperadores += verificaOperadores(token, line);
					contadorChamadasFuncoes += chamadaFuncao(token);
				}	
            // Obtém a próxima palavra
            token = strtok(NULL, " \t\n,");
        } 
        
        if (verificaLoop(line)){
        	contadorLoops++;
		}
		
        if (line[0] != '\n' && line[0] != '}' && strncmp(line, "#include", 8) != 0) {
            // Código dentro do loop
            contadorPassos++;
        }

	}
	
	printf("\nContador de declaracoes: %i", contadorDeclaracoes);
	printf("\nContador de operadores: %i", contadorOperadores);
	printf("\nContador de chamadas de funcoes: %i", contadorChamadasFuncoes);
	printf("\nContador de Loops: %i", contadorLoops);
	printf("\nContador de Passos: %i", contadorPassos);
	
	fclose(file);
}

void main() {	
	
	int num;
	char nome_arquivo[50];
	printf("\nInforme o nome do arquivo (e extensao) que deseja analisar\nDeve estar na mesma pasta que esse programa: ");
    scanf(texto,nome_arquivo);
    printf("\n");
    percorreArquivo(nome_arquivo);
   
}
