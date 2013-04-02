#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>

void error(const char *msg)	//función que retorna mensajes descriptivos de fallos en el programa
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;	//inicialización de variables
	struct hostent *server;
	char buffer[256];
	
	if (argc < 4)	//Evaluar cantidad de argumentos recibidos del usuario
		{
		printf("\nUso: \n1)Digite la dirección IP del cliente, 2) El número de puerto del cliente y 3) El número de puerto de escucha de éste equipo\n");
		exit(0);
	}		
	
	portno = atoi(argv[2]);	//se le asigna el puerto recibido por parametro al puerto establecido para recepción
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//se crea el socket encargado de las transacciones

	if (sockfd < 0)
		error("ERROR de apertura de socket");	//en caso de que falle la creación del socket
    
    
    if(argc != 4){
        printf("Error: digite solo el puerto");		//En caso de recibir una cantidad diferente de parámetros, salir y especificar el problema
        return 0;
		}
							//inicialización de variables
    char BUFFER2[8192];									//Tamaño del buffer para el envio de datos
    int socketfd, clientfd;								//Sockets de emisor y receptor respectivamente
    struct sockaddr_in serv_addr, client_addr;  		//Structs de emisor y receptor donde se guardan sus datos
	socklen_t client_addr_len;							//Largo del struct que mantiene la información del socket del receptor
    FILE * recv_file;									//puntero a una dirección de memoria donde se almacenará el archivo enviado por el emisor
    char *filename = "Archivo_Recibido";				//nombre del archivo recibido por el servidor, el cual se almacena en la ruta donde se ejecuto el programa
	    
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){				//CREACION DEL SOCKET DEL SERVIDOR	
		fprintf(stderr,"Error: No se pudo crear el socket");		//En caso de que no se pueda crear el socket adecuadamente
		close(socketfd);											//cerrar el socket
		return 0;		  											//terminar el programa
		}
		
	printf("\nSe ha creado el socket.");		//Si no falla la creación del socket
    
    serv_addr.sin_family = AF_INET;                         //Struct con la información del emisor
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[2]));
    printf("Puerto #%d ",serv_addr.sin_port);

    if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){			//Enlazamos (binding) el socket con la información del struct de los detalles del emisor
		fprintf(stderr, "Error de entrelazar (binding)");		//En caso de que falle el binding
		close(socketfd);
		return 0;
		} 
	printf("\nSe ha enlazado el socket");	//Si se logra el binding
	
    listen(socketfd, 5); 			//se establece que el socket receptor inicia funciones de recepción
        
    client_addr_len = sizeof(client_addr);     //Largo del struct que mantiene la dirección del socket del cliente
    printf("\nEsperando una conexión...\n");
	if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){ //Se verifica si hay algún archivo recibiendo en este socket
		printf("Error: No se puede aceptar la conexión \n");
	    close(socketfd);
	    return 1;
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

				bzero((char *) &serv_addr, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;

				bcopy((char *)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);

				serv_addr.sin_port = htons(portno);

				if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
					error("ERROR de conexión");

				//Envio de mensaje de respuesta al servidor despues de finalizada la transmisión proveniente de ella
				printf("Por favor, introduzca el mensaje:");

				bzero(buffer,256);
				fgets(buffer,255,stdin);
				n = write(sockfd,buffer,strlen(buffer));

				if (n < 0)
					error("ERROR al escribir en socket");
					
				bzero(buffer,256);
				n = read(sockfd,buffer,255);

				if (n < 0)
					error("ERROR al leer del socket");

				printf("%s\n",buffer);
				close(sockfd);
				close(socketfd);
				fclose(recv_file);
				printf("\nRecepción de archivo éxitosa! Socket terminado.\n");
				return 1;		
				}	
			}
		close(socketfd); // exitoso o no, se procede a cerrar el socker
	    return 0;	//se termina el programa
	}
