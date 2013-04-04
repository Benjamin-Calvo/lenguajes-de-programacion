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
int sockfd;	//inicialización de variables
struct hostent *server;
float contador_bytes = 1;

char BUFFER2[8192];									//Tamaño del buffer para el envio de datos
int socketfd, clientfd;								//Sockets de emisor y receptor respectivamente
struct sockaddr_in serv_addr, client_addr;  		//Structs de emisor y receptor donde se guardan sus datos
socklen_t client_addr_len;							//Largo del struct que mantiene la información del socket del receptor
FILE * recv_file;									//puntero a una dirección de memoria donde se almacenará el archivo enviado por el emisor
char *filename = "Archivo_Recibido";				//nombre del archivo recibido por el servidor, el cual se almacena en la ruta donde se ejecuto el programa

int pid; //sera usado en el fork()

//prototipos de funciones
void iniciarServidor(char *argv[]);
void iniciarCliente(char *argv[]);

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
	iniciarServidor(argv);
	return 0;
	}
	
	
//INICIALIZAR EL CLIENTE
void iniciarCliente(char *argv[]){ 
	printf(VERDE"\nInicializacion del cliente\n"BLANCO);
	if (sockfd < 0)
		error("ERROR al abrir el socket receptor");	//en caso de que falle la creación del socket
	//INICIALIZACION DE VARIABLES
    serv_addr.sin_family = AF_INET;                         //Struct con la información del emisor
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    //serv_addr.sin_addr.s_addr = htons(atoi(argv[1]));  //paso la ip del servidor
    //serv_addr.sin_port = htons(atoi(argv[2]));		//el puerto del servidor
    serv_addr.sin_port = htons(atoi(argv[3]));
    	
	//Creacion del socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);	//se crea el socket encargado de las transacciones
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){				//CREACION DEL SOCKET DEL SERVIDOR	
		fprintf(stderr,ROJO"Error: No se pudo abrir el socket receptor"BLANCO);		//En caso de que no se pueda crear el socket adecuadamente
		close(socketfd);											//cerrar el socket
		return;
		}
    if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){			//Enlazamos (binding) el socket con la información del struct de los detalles del emisor
		fprintf(stderr, VERDE"Error de entrelazar (binding)"BLANCO);		//En caso de que falle el binding
		close(socketfd);
		return;
		} 
	//printf("\nSe ha enlazado el socket de recepcion");	//Si se logra el binding	
    listen(socketfd, 5); 			//el socket receptor se inicializa        
    client_addr_len = sizeof(client_addr);     //Largo del struct que mantiene la dirección del socket del cliente
    printf(VERDE"\nRecepcion de archivos en espera de una conexión...\n"BLANCO);
	if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){ //Se verifica si hay algún archivo recibiendo en este socket
		printf(ROJO"Error: No se puede aceptar la conexión \n"BLANCO);
	    close(socketfd);
	    return;
		}	
	printf(VERDE"\nSe ha aceptado la conexión, procesando...\n"BLANCO);
		
	recv_file = fopen ( filename,"w" );	//se inicia el archivo donde se almacenaran los bits enviados por el emisor
	while(1){	//Recepción de datos		
		if( recv(clientfd, BUFFER2, 1, 0) != 0 ) {		
			fwrite (BUFFER2 , sizeof(BUFFER2[0]) , 1 , recv_file );
			BUFFER2[0] = 0;
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
			close(socketfd);
			fclose(recv_file);
			printf("\nRecepción de archivo éxitosa! Socket terminado.\n");
			return;
			}
		}
	printf("3.%f MB recibidos! \n",contador_bytes/1024);
	contador_bytes = 1;
	}
	
void iniciarServidor(char *argv[]){
	//inicializa variables del servidor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Socket inicializado que se conectara al otro equipo
	listen(sockfd,5);								//Espera que el otro puerto se incialice
	//crear el socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	   fprintf(stderr,AZUL"\n Error \n  No se pudo crear el socket \n"BLANCO);
	   return;
		}
	printf(AZUL"\nSe ha creado el socket del servidor\n"BLANCO);	
	serv_addr.sin_family = AF_INET;					//Se emplea el protocolo TCP de forma stream
	serv_addr.sin_port = htons(atoi(argv[3])); 			//especifica el puerto, con htons convierte el entero a bytes de red.
	
	//Paso el IPv4 del cliente destino
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){  	
		printf(AZUL"\n inet_pton error occured\n"BLANCO);
		return;
		}
	

	printf(AZUL"\nSe ha realizado la conexion con el servidor\n"BLANCO);	
	
	fflush(stdout);
	//pid_t procesoHijo = fork();
	//procesoHijo = fork();
	//printf(ROJO"pid1 %d\n"BLANCO, procesoHijo);
	
	//if (procesoHijo >= 0)
	printf ("Inicio del proceso padre. PID=%d\n", getpid ());
	
	//{		
	//if(procesoHijo==0)
	int confirmacion = 0;
	if (fork())
	{	
		printf ("Inicio proceso padre. PID=%d, PPID=%d\n", getpid (), getppid ());		
		printf(ROJO"pid2 %d\n "BLANCO, procesoHijo);		
		printf(ROJO"Esperando confirmación para crear el cliente...digite 1.\n"BLANCO);
		fflush(stdin);//limpiar
		fflush(stdout);//limpiar	
		//
		scanf("%d", &confirmacion);
		printf("confirmacion= %d\n", confirmacion);
		if(confirmacion == 1)
		{
			iniciarCliente(argv);			
		}
		else
		{
			printf(AZUL"Confirmación incorrecta. Se cerrará el programa.\n"BLANCO);
			exit(1);
		}
		sleep (20);/**/
	}
	else{
		printf ("Proceso hijo. PID=%d\n", getpid ());
		sleep (20);
		
		printf(ROJO"pid3 %d \n "BLANCO, procesoHijo);
		printf(AZUL"Servidor inicializado correctamente.\n"BLANCO);	
		//Conectar el cliente con el servidor
		if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){   //Indica si hay un error en la conexion
		   printf(AZUL"\n Error : Conexion fallida, no se pudo conectar al servidor \n"BLANCO);
		   //return 1;
		   return;
			}
		sleep (5);
		printf(AZUL"Digíte el nombre del archivo: "BLANCO);
		scanf("%s",lfile);								//Se asegura de tener el nombre del archivo que incluye su extension
		printf("%s",lfile);
		
		archivo_recibido = fopen (lfile,"r");				//Abrir el archivo a enviar, ahora el archivo_recibido es el FD de lfile
		if(!archivo_recibido){   					
			 printf(AZUL"Error opening file\n"BLANCO); // en caso de que no logre abrir el archivo
			 close(sockfd);
			 //return 1;
			 return;
				}	

		else {				//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
			printf(AZUL"\nEl archivo que se va a  enviar ha sido abierto"BLANCO);		
			printf(AZUL"\nEnviando archivo, por favor espere...\n"BLANCO);

			//Inicio del envio de datos.
				while((ch=getc(archivo_recibido))!=EOF){	//se obtienen los datos
					toSEND[0] = ch;				//se almacenan en un arreglo
					enviados++; 
					send(sockfd, toSEND, 1, 0);			
					}
				}
		printf(AZUL"Enviados: %.3fMB \n"BLANCO, enviados/1024); 
		enviados =1;
		//Cierra el archivo y el socket
		fclose(archivo_recibido);		
	}
	//}
	}
