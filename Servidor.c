#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>   
//SERVIDOR

	
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
    
    int sockfd;									
	struct sockaddr_in serv_addr, cli_addr;		
	FILE * archivo_recibido;						
	int ch, n, newsockfd;
	char toSEND[1];
	char buffer[256];
	socklen_t clilen;
	//-----
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	//---------
	char lfile[200];
	printf("Digite el nombre del archivo:\n");
	scanf("%s",lfile);
	printf("%s",lfile);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){				
       fprintf(stderr,"\n Error : Could not create socket \n");
       return 1;
		}
	 printf("\nSe ha creado el socket\nPresione Enter para continuar...");
	
	serv_addr.sin_family = AF_INET;									
    serv_addr.sin_port = htons(atoi(argv[2])); 						
	
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){  		
        printf("\n inet_pton error occured\n");
        return 1;
		}
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){   
       printf("\n Error : Connect Failed, no se pudo conectar al ser \n");
       return 1;
		}
	
	printf("\nSe ha realizado la conexion con el servidor");
	
	archivo_recibido = fopen (lfile,"r");							
															   
	if(!archivo_recibido){   					
		 printf("Error opening file\n");
	     close(sockfd);
	     return 1;
			}	
	
	else {				
		printf("\nSe ha abierto el archivo a enviar");		
		printf("\nEnviado archivo, por favor espere...\n");
			

			while((ch=getc(archivo_recibido))!=EOF){		
				toSEND[0] = ch;						
				send(sockfd, toSEND, 1, 0);	
				}
			}
		
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		bzero(buffer,256);
		n = write(newsockfd,"Archivo enviado desde el servidor",18);
		
		if (n < 0)
			error("ERROR al escribir en el socket");
			
	    //Cierra el archivo y el socket
	    fclose(archivo_recibido);	
	    close(sockfd);
		printf("\nSe ha cerrado el socket\n");
	    return 0;
	}
	// fin del envio de datos.
