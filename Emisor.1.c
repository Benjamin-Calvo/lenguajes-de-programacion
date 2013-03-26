#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 




#define NULO 0

/*SERVIDOR DE ARCHIVOS*/

/*PROTOTIPOS de funciones*/
int menu(void);							
int conectar_a(char[], int);
//int sockfd = socket(int domain, int type, int protocol); /*Para la creacion del socket*/
//int bind(int sockfd,struct sockaddr *my_addr, socklen_t addrlen);


/*VARIABLES de control*/
//int sockfd;





//INICIALIZACION de variables de control
//sockfd = socket(AF_INET, SOCK_STREAM, 0);
/*
//DECLARACION de estructuras
struct sockaddr{
	unsigned short 	sa_family; //AF_*
	char sa_data[14]; //Direccion de protocolo
};

struct sockaddr_in{
	short int sin_family; //AF_INET
	unsigned short sin_port; // Numero de puerto
	struct in_addr sin_addr; // Direccion IP
	unsigned char sin_zero[8]; // Relleno
};

struct in_addr{
	unsigned long s_addr; //4 bytes
};
*/

	

int main(){
	
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 

	char sendBuff[1025];
	time_t ticks; 

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	listen(listenfd, 10); 

	char *buffer = NULL;
	int n = 0;
	//fin
	
	int eleccion;  						/*Declaracion de variable local para eleccion de menu*/
	int salida = 0;					/*usado en la confirmacion de SALIR*/
	
	do{
		eleccion = menu();
		char serv[15];				/*arreglo para almacenar la ip */
		int  puert;					/*almacenar el num de puerto*/		
		
		switch(eleccion){
			/*CONECTAR con el servidor*/		
			case 1:  								
				printf("\nDigite la direccion ip del equipo a conectar (ejem: 192.168.0.1):");
				scanf(" %[^\n]", serv);
				printf("\nDigite el numero de puerto del equipo a conectar: ");
				scanf("%d",&puert);
				printf("\nLa ip del equipo es: %s \nEl puerto es: %d", serv, puert);
				printf("\n");
				
				continue;
			/*ENVIAR un archivo a un cliente*/
			case 2:  				
				//prin = insertar(prin);
				printf("\n");
				while(1)
				{
					printf("\nEsperando...");
					connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

					ticks = time(NULL);
					snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
					write(connfd, sendBuff, strlen(sendBuff)); 

					close(connfd);
					sleep(1);
				 }
				//mostrar(prin);
				continue;
			/*RECIBIR archivo*/
			case 3:  				
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

