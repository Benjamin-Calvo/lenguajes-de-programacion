#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define NULO 0

/*SERVIDOR DE ARCHIVOS*/

int socket(int domain, int type, int protocol);

//int bind(int sockfd,struct sockaddr *my_addr, socklen_t addrlen);


int menu(void);							/*Prototipo de funcion*/
int conectar_a(char[], int);			/*Prototipo de funcion*/

char *buffer = NULL;


int main(){
	int eleccion;  						/*Declaracion de variable local para eleccion de menu*/
	int salida =0;					/*usado en la confirmacion de SALIR*/
	
	do{
		eleccion = menu();
		char serv[15];				/*arreglo para almacenar la ip */
		int  puert;					/*almacenar el num de puerto*/		
		
		switch(eleccion){
			case 1:  				/*CONECTAR con el servidor*/						
				printf("\nDigite la direccion ip del equipo a conectar (ejem: 192.168.0.1):");
				scanf(" %[^\n]", serv);
				
				//leer();
				
				printf("\nDigite el numero de puerto del equipo a conectar: ");
				scanf("%d",&puert);
				printf("\nLa ip del equipo es: %s \nEl puerto es: %d", serv, puert);
				
				/*if (conectar_a(serv, puert) == 0){
					printf("\nConexion establecida con ");
				}*/
				printf("\n");
				
				continue;
			case 2:  				/*ENVIAR un archivo a un cliente*/
				//prin = insertar(prin);
				printf("\n");
				//mostrar(prin);
				continue;
			case 3:  				/*RECIBIR archivo*/
				//prin = eliminar(prin);
				printf("\n");
				//mostrar(prin);
				continue;
			case 4: 				
				printf("¿Realmente desea salir?\n");
				printf("	1 - SALIR\n");
				printf("	2 - CANCELAR\nDigite una opción: ");
				scanf("%d", &salida);
				if(salida == 1){				/*Finalizar*/
					printf("Fin de las operaciones\n");
					break;
					}
				else{							/*No hacer nada, continuar*/
					printf("\n --Continuar--\n");
					continue;
				}		
				
			default: 				
				printf("woops!\n");
			}
		} while (salida != 1);
	return 0;
	}
	
int menu(void) /*generar el menu principal*/
	{
		int eleccion;		
		do {
			printf("\nTRANSMISOR DE ARCHIVOS EN C\n Menu principal:\n");
			printf("	1 - CONECTAR con el servidor\n");
			printf("	2 - ENVIAR un archivo\n");
			printf("	3 - RECIBIR un archivo\n");
			printf("	4 - FIN\n");
			printf("Por favor, introduzca su eleccion (1-4): ");
			scanf("%d", &eleccion);
			if(eleccion <1 || eleccion > 4)
				printf("\nERROR - Por favor, intentelo de nuevo\n");
		} while (eleccion <1 || eleccion >5);
		printf("\n");
		return (eleccion);
	}



/*int conectar_a(char[] ip, int puerto)				//Funcion
{
	//Conectar a servidor

return 0;	
}*/

