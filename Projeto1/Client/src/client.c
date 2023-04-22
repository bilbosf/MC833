#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netservice.h"
 
#define PORT 8081
#define IP "127.0.0.1"

int main(){
    printf("*************** Bem vindo ao serviço de busca de perfis ***************\n");

    while(1){
		/* Mostrar menu de escolha */
        printf("*************** Menu de escolha ***************\n");
		printf("Insira o número de um dos comandos a seguir e pressione enter:\n");
		printf("\t1 - Listar todas as pessoas (email e nome) formadas em um determinado curso\n");
		printf("\t2 - Listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n");
		printf("\t3 - Listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n");
		printf("\t4 - Listar todas as informações de todos os perfis\n");
        printf("\t5 - Retornar informações de um perfil\n");
		printf("\t6 - Cadastrar um novo perfil\n");
		printf("\t7 - Remover um perfil\n");
		printf("\nPara sair do programa, pressione q\n");
		
		char command;
		scanf("%c", &command);
		printf("\n");

		/* Fazer requisicao */
		char arg[1023] = {'\0'};
		switch (command){
			case '1':
				printf("Curso que deseja buscar: ");
				break;
			case '2':
				printf("Habilidade que deseja buscar: ");
				break;
			case '3':
				printf("Ano que deseja buscar: ");
				break;
			case '4':
				break;
			case '5':
				printf("Email do perfil que deseja consultar: ");
				break;
			case '6':
				// Falta adicionar
				add_user();
				break;
			case '7':
				printf("Email do perfil que deseja remover: ");
				break;
			case 'q':
				printf("Programa terminado.\n");
				return 0;
				break;
			default:
				printf("Comando inválido, tente novamente.\n\n");
				continue;
		}

		fgetc(stdin); // Necessário para limpar o buffer antes do fgets
		fgets(arg, 1023, stdin);
		send_request(command, arg, IP, PORT);
	}
    return (0);
}