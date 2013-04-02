#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>

void error(const char *msg)
{
  perror(msg);
	exit(0);
}

int main(int argc, char *argv[]){
    //inicio msj
    int sockfd, portno, n;
	struct hostent *server;
	
	char buffer[256];
	char buff[] = {"Archivos recibidos, gracias."};
	if (argc < 4)
		{
		printf("\nUso: \n1)Digite la dirección IP del cliente, 2) El número de puerto del cliente y 3) El número de puerto de escucha de éste equipo\n");        
		exit(0);
	}		
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR al abrir el socket");	
    //fin msj
    
    
    if(argc != 4){
        printf("Error: digite solo el puerto");	
        return 0;
		}
  
    char BUFFER2[8192];									
    int socketfd, clientfd;								
    struct sockaddr_in serv_addr, client_addr;  		
	socklen_t client_addr_len;							
    FILE * recv_file;									
    char *filename = "Archivo_Recibido";				
	    
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){				
		fprintf(stderr,"Error: No se pudo crear el socket");		
		close(socketfd);											
		return 0;		  											
		}
		
	printf("\nSe ha creado el socket.");		
   
    
   
    serv_addr.sin_family = AF_INET;                          
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);       	
    serv_addr.sin_port = htons(atoi(argv[2]));
    printf("Puerto #%d ",serv_addr.sin_port);

    if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){			
		fprintf(stderr, "Error de entrelazar (binding)");
		close(socketfd);
		return 1;
		} 
	printf("\nSe ha enlazado el socket");
	
    listen(socketfd, 5); 			
        
    client_addr_len = sizeof(client_addr);      
    printf("\nEsperando una conexión...\n");
	if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){
		printf("Error: No se puede aceptar la conexión \n");
	    close(socketfd);
	    return 1;
		}
	
	printf("\nSe ha aceptado la conexión, procesando...");
		
		recv_file = fopen ( filename,"w" );	
		int contador_bytes = 1;
		
		while(1){
			if( recv(clientfd, BUFFER2, 1, 0) != 0 ) {		
				fwrite (BUFFER2 , sizeof(BUFFER2[0]) , 1 , recv_file );	
				BUFFER2[0] = 0;
				printf("\n%d bits recibidos hasta este momento	",contador_bytes);
				contador_bytes++;
			}
			else {				
				//inicio msj
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

				printf("Por favor, introduzca el mensaje:");

				bzero(buffer,256);
				fgets(buffer,255,stdin);
				n = write(sockfd,buff,strlen(buffer));
				
				if (n < 0)
					error("ERROR al escribir en socket");
					
				bzero(buffer,256);
				n = read(sockfd,buffer,255);

				if (n < 0)
					error("ERROR al leer del socket");

				printf("%s\n",buffer);
				close(sockfd);
				//fin msj
				close(socketfd);
				fclose(recv_file);
				printf("\nRecepción de archivo éxitosa! Socket terminado.\n");
				return 1;		
				}	
			}
		close(socketfd);
	    return 0;
	}
