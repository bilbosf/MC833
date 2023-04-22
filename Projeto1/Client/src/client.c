#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netservice.h"
 
int main(){
    char user_input[1024] = {'\0'};
	char admin; //variável que guarda se o usuário é admin

	/* Verificacao do cliente */
    printf("*************** Bem vindo ao serviço de busca de perfis ***************\n");
	printf("Você é o(a) administrador(a) do sistema?[s/n]: ");
	scanf("%c", &admin);
	printf("\n");

    while(1){

		/* Mostrar menu de escolha */
        printf("*************** Menu de escolha ***************\n");
		printf("Insira o número de um dos comandos a seguir e pressione enter:\n");
		printf("\t1 - Listar todas as pessoas (email e nome) formadas em um determinado curso\n");
		printf("\t2 - Listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n");
		printf("\t3 - Listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n");
		printf("\t4 - Listar todas as informações de todos os perfis\n");
        printf("\t5 - Retornar informações de um perfil\n");
		if(admin == 's'){
			printf("\t6 - Cadastrar um novo perfil\n");
			printf("\t7 - Remover um perfil\n");
		}
		printf("\nPara sair do programa, pressione q\n");
		
		scanf("%s", user_input);
		printf("\n\n");

		/* Fazer requisicao */
		if(!strcmp(user_input, "1")){
			list_users_by_course();
		}else if(!strcmp(user_input, "2")){
			list_users_by_skill();
		}else if(!strcmp(user_input, "3")){
			list_users_by_year();
		}else if(!strcmp(user_input, "4")){
			list_all_users();
		}else if(!strcmp(user_input, "5") && admin == 's'){
			list_user_by_email();
		}else if(!strcmp(user_input, "6") && admin == 's'){
			add_user();
		}else if(!strcmp(user_input, "7") && admin == 's'){
			remove_user_by_email();
        }else if(!strcmp(user_input, "q")){
			printf("Programa terminado.\n");
			break;
		}else{
			printf("Comando inválido, tente novamente.\n\n");
		}
	}
    return (0);
}