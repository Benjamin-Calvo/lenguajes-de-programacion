/*Tecnológico de Costa Rica
 * Tarea Programada I - Programa Transmisor
 * Transfiere archivos por medio de sockets y direcciones IP en una red
 * Integrantes:
 * Andrew Araya
 * Benjamín Calvo
 * Alexander Durán
 * 4 de abril 2013
 * */

#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>	
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>

#define ROJO "\033[1;31m"
#define VERDE "\033[1;32m"
#define AMARILLO "\033[1;33m"
#define AZUL "\033[1;34m"
#define BLANCO "\033[0m"
#define BRILLANTE "\033[1;1m"

int puertoServidor, puertoCliente;
char ipCliente[15];

//SERVIDOR

//Definicion de Variables
struct sockaddr_in serv_addr;					//struct sockaddr del servidor.
int sockfd;
float enviados =1; 								// almacenara la cantidad de bytes enviados 
FILE * archivo_recibido;						//(Puntero) Archivo a recibir
int ch;											//Variables que alamacenan los datos del socket
char toSEND[1];
char lfile[200];
int procesoHijo;
//fin de variables del servidor

//Declaracion de variables Cliente
float contador_bytes = 1;
int sockfd, portno, n;	//inicialización de variables
struct hostent *server;
char buffer[256];

char BUFFER2[8192];									//Tamaño del buffer para el envio de datos
int socketfd, clientfd;								//Sockets de emisor y receptor respectivamente
struct sockaddr_in serv_addr, client_addr;  		//Structs de emisor y receptor donde se guardan sus datos
socklen_t client_addr_len;							//Largo del struct que mantiene la información del socket del receptor
FILE * recv_file;									//puntero a una dirección de memoria donde se almacenará el archivo enviado por el emisor
char *filename = "Archivo_Recibido";				//nombre del archivo recibido por el servidor, el cual se almacena en la ruta donde se ejecuto el programa

int eleccion;  						/*Declaracion de variable local para eleccion de menu*/
int salida = 0;					/*usado en la confirmacion de SALIR*/

//prototipos de funciones
void iniciarServidor(char *argv[]);
void iniciarCliente(char *argv[]);
int menu();

//funcion para informar de los errores del programa
void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}
int main( int argc, char *argv[]){
	
	//validar la cantidad de parametros que le entran al programa.
	if(argc != 4){ // si los argumentos son distintos de 4 (nombre_archivo, IP, #puerto, #puerto)
        printf("\nUso: \n1)Digite la dirección IP del cliente, 2) El número de puerto del cliente y 3) El número de puerto de escucha de éste equipo\n");
        return 1; 
    }
    //./transmisor ipRemota puertoRemoto puertoLocal

	printf(AZUL"\n\n\nPROGRAMA TRANSMISOR\n"BLANCO"     Transfiere archivos de un equipo a otro.\n"VERDE"Inicializar el Transmisor \n"BLANCO);
	printf("Éste Transmisor se conectará al equipo "AMARILLO"%s"BLANCO", por el puerto "AMARILLO"%s"BLANCO" \n",argv[1], argv[2]);
	printf("Éste Transmisor escuchará en el puerto "AMARILLO"%s"BLANCO" \n",argv[3]);
	
	//iniciarServidor(argv);
	//iniciarServidor(argv);
	
	do{
		eleccion = menu();
		
		switch(eleccion){			
			case 1: 
				//ENVIAR un archivo a un cliente				
				iniciarServidor(argv);
				sleep(10);			 
				printf("\nFin de conexion con cliente (IP)");
				continue;
			
			case 2:  								
				//CONECTAR con el equipo remoto	
				iniciarCliente(argv);
				sleep(10);
				continue;
			case 3: 	
				//SALIR DEL PROGRAMA
				printf("¿Realmente desea salir?\n");
				printf("	1 - SALIR\n");
				printf("	2 - CANCELAR\nDigite una opción: ");
				scanf("%d", &salida);
				if(salida == 1){				//Finalizar
					printf("Fin de las operaciones\n");
					break;
					}
				else{							//No hacer nada, continuar
					printf("\n --Continuar--\n");
					continue;
				}					
			default: 				
				printf("Error de menu!\n");
			}
		} while (salida != 1);
	return 0;
	}
	

//INICIAR SERVIDOR

void iniciarServidor(char *argv[]){
	//inicializa variables del servidor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Socket inicializado que se conectara al otro equipo
	listen(sockfd,5);								//Espera que el otro puerto se incialice
	//---------	
	printf("Digíte el nombre del archivo: ");
	scanf("%s",lfile);								//Se asegura de tener el nombre del archivo que incluye su extension
	printf("%s",lfile);

	//crear el socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
       fprintf(stderr,"\n Error \n  No se pudo crear el socket \n");
       //return 1;
       return;
		}
	printf("\nSe ha creado el socket\nPresione Enter para continuar...");	
	serv_addr.sin_family = AF_INET;					//Se emplea el protocolo TCP de forma stream
    serv_addr.sin_port = htons(atoi(argv[2])); 			//especifica el puerto, con htons convierte el entero a bytes de red.
	
	//Paso el IPv4 del cliente destino
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){  	
        printf("\n inet_pton error occured\n");
        //return 1;
        return;
		}
    //Conectar el cliente con el servidor
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){   //Indica si hay un error en la conexion
       printf("\n Error : Conexion fallida, no se pudo conectar al servidor \n");
       //return 1;
       return;
		}

	printf("\nSe ha realizado la conexion con el servidor");			

	archivo_recibido = fopen (lfile,"r");				//Abrir el archivo a enviar, ahora el archivo_recibido es el FD de lfile
	if(!archivo_recibido){   					
		 printf("Error opening file\n"); // en caso de que no logre abrir el archivo
	     close(sockfd);
	     //return 1;
	     return;
			}	

	else {				//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
		printf("\nEl archivo que se va a  enviar ha sido abierto");		
		printf("\nEnviando archivo, por favor espere...\n");

		//Inicio del envio de datos.
			while((ch=getc(archivo_recibido))!=EOF){	//se obtienen los datos
				toSEND[0] = ch;				//se almacenan en un arreglo
				enviados++; 
				send(sockfd, toSEND, 1, 0);			
				}
			}
			printf("Enviados: %.3f KB \n", enviados/1024); 
			enviados =1;

	}
	
int menu(void) /*generar el menu principal*/
	{
		int eleccion;		
		do {
			printf("\nTRANSMISOR DE ARCHIVOS EN C\n Menu principal:\n");
			printf("	1 - CONECTAR y enviar archivos\n");
			printf("	2 - RECIBIR un archivo\n");
			printf("	3 - FIN\n");
			printf("Por favor, introduzca su eleccion (1-3): ");
			scanf("%d", &eleccion);
			if(eleccion <1 || eleccion > 3)
				printf("\nERROR - Por favor, intentelo de nuevo\n");
		} while (eleccion <1 || eleccion >4);
		printf("\n");
		return (eleccion);
	}

//Funcion cliente
void iniciarCliente(char *argv[]){
								//inicialización de variables
    char BUFFER2[15360];									//Tamaño del buffer para el envio de datos
    int socketfd, clientfd;								//Sockets de emisor y receptor respectivamente
    struct sockaddr_in serv_addr, client_addr;  		//Structs de emisor y receptor donde se guardan sus datos
	socklen_t client_addr_len;							//Largo del struct que mantiene la información del socket del receptor
    FILE * recv_file;									//puntero a una dirección de memoria donde se almacenará el archivo enviado por el emisor
    char *filename = "Archivo_Recibido";				//nombre del archivo recibido por el servidor, el cual se almacena en la ruta donde se ejecuto el programa

	portno = atoi(argv[2]);	//se le asigna el puerto recibido por parametro al puerto establecido para recepción
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//se crea el socket encargado de las transacciones

	if (sockfd < 0)
		error("ERROR de apertura de socket");	//en caso de que falle la creación del socket
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){				//CREACION DEL SOCKET DEL SERVIDOR	
		fprintf(stderr,"Error: No se pudo crear el socket");		//En caso de que no se pueda crear el socket adecuadamente
		close(socketfd);											//cerrar el socket
		return;		  											//terminar el programa
		}

	printf("\nSe ha creado el socket.");		//Si no falla la creación del socket
    
    serv_addr.sin_family = AF_INET;                         //Struct con la información del emisor
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[2]));
    printf("Puerto #%d ",serv_addr.sin_port);

    if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){			//Enlazamos (binding) el socket con la información del struct de los detalles del emisor
		fprintf(stderr, "Error de entrelazar (binding)");		//En caso de que falle el binding
		close(socketfd);
		return;
		} 
	printf("\nSe ha enlazado el socket");	//Si se logra el binding

    listen(socketfd, 5); 			//se establece que el socket receptor inicia funciones de recepción
        
    client_addr_len = sizeof(client_addr);     //Largo del struct que mantiene la dirección del socket del cliente
    printf("\nEsperando una conexión...\n");
	if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){ //Se verifica si hay algún archivo recibiendo en este socket
		printf("Error: No se puede aceptar la conexión \n");
	    close(socketfd);
	    return;
		}

	printf("\nSe ha aceptado la conexión, procesando...");

		recv_file = fopen ( filename,"w" );	//se inicia el archivo donde se almacenaran los bits enviados por el emisor
		int contador_bytes = 1;
		while(1){	//Recepción de información

			if( recv(clientfd, BUFFER2, 1, 0) != 0 ) {		//Recepción de dato desde el receptor
				fwrite (BUFFER2 , sizeof(BUFFER2[0]) , 1 , recv_file );
				BUFFER2[0] = 0;
				printf("\n%d bits recibidos hasta este momento	",contador_bytes);
				contador_bytes++;
			}
			else {				//en caso de fallo
				server = gethostbyname(argv[1]);
						if (server == NULL)
					{
						fprintf(stderr,"ERROR, no hay host \n");
						exit(0);
					}
				close(sockfd);				
				fclose(recv_file);
				close(socketfd); // se cierra el socket	
				printf("\nRecepción de archivo éxitosa! Socket terminado.\n");
				return;		
				}	
			}
		contador_bytes =1;
		
	}
