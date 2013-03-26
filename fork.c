# include <stdio.h>
# include <unistd.h>


int main(){
	pid_t pid;
	pid = fork();
	printf("Variable creada\n");
	if(pid == -1){
		printf("Error al crear proceso hijo \n");
	}
	if(pid){
		//Proceso Padre
		printf("Soy el padre, y el PID de mi hijo es %d\n",pid);
		printf("Aqui funciona el proceso de emision");
		
	}else{
		//Proceso Hijo
		printf("Soy el hijo y voy a ejecutar la herramienta date\n");
		printf("Aqui funciona el proceso de rececpcion");
	}
	return 0;
}
	
	
