#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netservice.h"
 
#define PORT 8080
#define IP "127.0.0.1"
#define USER_FIELD_LEN 50
#define USER_SKILLS_LEN 200


int take_argument(char *arg, int size);

void take_user(char *arg);

int main(){
    printf("*************** Bem vindo ao serviço de busca de perfis ***************\n");

    while(1){
		/* Show menu selection */
        printf("\n*************** Menu de escolha ***************\n");
		printf("Insira o número de um dos comandos a seguir e pressione enter:\n");
		printf("\t1 - Listar todas as pessoas (email e nome) formadas em um determinado curso\n");
		printf("\t2 - Listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n");
		printf("\t3 - Listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n");
		printf("\t4 - Listar todas as informações de todos os perfis\n");
        printf("\t5 - Retornar informações de um perfil\n");
		printf("\t6 - Cadastrar um novo perfil\n");
		printf("\t7 - Remover um perfil\n");
		printf("\t8 - Upload de uma imagem de perfil\n");
		printf("\t9 - Download de uma imagem de perfil\n");
		printf("\ts - Sair do programa\n");

		printf("\nComando: ");
		char command;
		scanf("%c", &command);

		//Clear input buffer to prevent overflow
		int c;
		while ((c = getchar()) != '\n' && c != EOF); 

		/* Directs to the right function if it needs to take an extra argument */
		char arg[MAXLINE - 1] = {'\0'};
		switch (command){
			case '1':
				printf("Curso que deseja buscar: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '2':
				printf("Habilidade que deseja buscar: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '3':
				printf("Ano que deseja buscar: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '4':
				break;
			case '5':
				printf("Email do perfil que deseja consultar: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '6':
				take_user(arg);
				break;
			case '7':
				printf("Email do perfil que deseja remover: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '8':
				printf("Email do perfil que deseja associar a imagem: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case '9':
				printf("Email que deseja cosultar para receber a imagem de perfil: ");
				take_argument(arg, MAXLINE - 1);
				break;
			case 's':
				printf("Programa terminado.\n");
				return 0;
				break;
			default:
				printf("Comando inválido, tente novamente.\n\n");
				continue;
		}

		
		send_request(command, arg, IP, PORT);

		printf("Pressione enter para continuar.");
		while ((c = getchar()) != '\n' && c != EOF); 
	}
    return (0);
}

char *remove_newline(char *str){
	int len = strlen(str);
	if(len <= 1) {
		return NULL;
	}
	str[len - 1] = '\0';
	return str;
}

int take_argument(char *arg, int size) {
	fgets(arg, size - 1, stdin);
	remove_newline(arg);
	return 0;
}

void take_user(char *arg) {
	printf("Email: ");
	fgets(arg, USER_FIELD_LEN, stdin);
	remove_newline(arg);

	printf("Nome: ");
	fgets(arg + USER_FIELD_LEN, USER_FIELD_LEN, stdin);
	remove_newline(arg + USER_FIELD_LEN);

	printf("Sobrenome: ");
	fgets(arg + 2 * USER_FIELD_LEN, USER_FIELD_LEN, stdin);
	remove_newline(arg +  2 * USER_FIELD_LEN);

	printf("Residência: ");
	fgets(arg + 3 * USER_FIELD_LEN, USER_FIELD_LEN, stdin);
	remove_newline(arg +  3 * USER_FIELD_LEN);

	printf("Formação Acadêmica: ");
	fgets(arg + 4 * USER_FIELD_LEN, USER_FIELD_LEN, stdin);
	remove_newline(arg +  4 * USER_FIELD_LEN);

	printf("Ano de formatura: ");
	fgets(arg + 5 * USER_FIELD_LEN, USER_FIELD_LEN, stdin);
	remove_newline(arg +  5 * USER_FIELD_LEN);

	printf("Habilidades: ");
	fgets(arg + 6 * USER_FIELD_LEN, USER_SKILLS_LEN, stdin);
	remove_newline(arg +  6 * USER_FIELD_LEN);
}
