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
//SERVIDOR

//funcion para informar de los errores del programa
void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}

int main( int argc, char *argv[]){
	
	if(argc != 4){ // si los argumentos son distintos de 4 (nombre_archivo, IP, #puerto, #puerto)
        printf("\nUso: \n1)Digite la dirección IP del cliente, 2) El número de puerto del cliente y 3) El número de puerto de escucha de éste equipo\n");
        return 1; 
    }
    
    						
	struct sockaddr_in serv_addr, cli_addr;				//struct sockaddr del servidor.
	int sockfd;	
	FILE * archivo_recibido;						//(Puntero) Archivo a recibir
	int ch, n, newsockfd;							//Variables que alamacenan los datos del socket
	char toSEND[1];
	char buffer[256];								//Variable que almacena el buffer del socket
	socklen_t clilen;
	//-----
	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Socket inicializado que se conectara al otro equipo
	listen(sockfd,5);								//Espera que el otro puerto se incialice
	clilen = sizeof(cli_addr);
	//---------
	char lfile[200];
	printf("Digíte el nombre del archivo: ");
	scanf("%s",lfile);								//Se asegura de tener el nombre del archivo que incluye su extension
	printf("%s",lfile);
	
	//crear el socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
       fprintf(stderr,"\n Error \n  No se pudo crear el socket \n");
       return 1;
		}
	printf("\nSe ha creado el socket\nPresione Enter para continuar...");	
	serv_addr.sin_family = AF_INET;					//Se emplea el protocolo TCP de forma stream
    serv_addr.sin_port = htons(atoi(argv[2])); 			//especifica el puerto, con htons convierte el entero a bytes de red.
	
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){  	//Paso el IPv4 del servidor al structca
        printf("\n inet_pton error occured\n");
        return 1;
		}
    //Conectar el cliente con el servidor
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){   //Indica si hay un error en la conexion
       printf("\n Error : Conexion fallida, no se pudo conectar al servidor \n");
       return 1;
		}
	
	printf("\nSe ha realizado la conexion con el servidor");			
	
	archivo_recibido = fopen (lfile,"r");							//Abrir el archivo a enviar, ahora el archivo_recibido es el FD de lfile
															   // y lo leo
	if(!archivo_recibido){   					
		 printf("Error opening file\n");
	     close(sockfd);
	     return 1;
			}	
	
	else {				//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
		printf("\nEl archivo que se va a  enviar ha sido abierto");		
		printf("\nEnviado archivo, por favor espere...\n");
			
		//Inicio del envio de datos.
			while((ch=getc(archivo_recibido))!=EOF){	//se obtienen los datos
				toSEND[0] = ch;				//se almacenan en un arreglo
				send(sockfd, toSEND, 1, 0);			
				}
			}
		// lee del socket el mensaje de confirmacion
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);	
	    if (n < 0)
			error("ERROR al leer del socket");
		
		printf("Aquí está el mensaje: %s\n",buffer);	//Indica que el mensaje si se envio
		
		n = write(newsockfd,"Tengo el mensaje",18);
		
		if (n < 0)
			error("ERROR al escribir en el socket");
			
	    //Cierra el archivo y el socket
	    fclose(archivo_recibido);	
	    close(sockfd);
		printf("\nSe ha cerrado el socket\n");
	    return 0;
	}
	// fin del envio de datos.
