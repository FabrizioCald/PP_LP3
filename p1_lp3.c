/*p1_lp3.c*/
/*
Nombre y Apellido: Fabrizio Rafael Calderoli Pavón
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
/*Recordar: para compilar en linux y utilizar math.h debe ser gcc pp_lp3.c -lm -o pp_lp3*/
/*void merge_sort(int arreglo, int cant_elementos, int *ptr_proceso);*/
void merge_sort_recursion(int arreglo[], int izq, int der, int cant_proceso, int n_nivel, int nivel_arbol, int tam_n, int id_padre);
void merge_sort_arreglos(int arreglo[], int izq, int medio, int der);

int main(int argc, char *argv[])
{
	/*---------*/
	if (argc < 3) {
        printf("Uso: %s <num_procesos> <lista_enteros>\n", argv[0]);
        return 1;
    }
	/*---------*/
	printf("Prueba numero 14\n");
	int n_procesos = atoi(argv[1]);//Número de procesos a realizar
	int n_proceso = 0;
	int* ptr_n_proceso = &n_proceso;//Para incrementar el valor de la variable en los demás procesos
	/*---------*/
	/*------------------------------------*/
	double x = (double)n_procesos;
	int nivel_arbol = 0;
	double valor_nivel = 0.0;
	/*------------*/
	double valor_resp1 = 0.0;
	double valor_resp2 = 0.0;
	/*------------*/
	valor_resp1 = log(x);
	valor_resp2 = log(2);
	/* log2 (x) = logy (x) / logy (2) */
	valor_nivel = valor_resp1 / valor_resp2;
	nivel_arbol = (int)valor_nivel;
	/*------------*/
	int n_nivel = 0;
	/*------------------------------------*/
	char *input_list = argv[2];
    int arr[100];
    int n = 0;

    char *token = strtok(input_list, ",");
    while (token != NULL) {
        arr[n] = atoi(token);
        n++;
        token = strtok(NULL, ",");
    }
	/*---------*/
	int arreglo_de_n[n];
	for (int i = 0; i < n; ++i)
	{
		arreglo_de_n[i] = arr[i];//Para tener un arreglo de n elementos
	}
	/*int *ptr_arreglo = &arreglo_de_n[0];*///Ultimo cambio
	/*---------*/
	printf("\n");
	for (int j = 0; j < n; ++j)
	{
		printf("%d ", arreglo_de_n[j]);
	}
	/*------------------------------------------------------*/
	/*merge_sort(ptr_arreglo, n, n_procesos);*/
	/*merge_sort_recursion(ptr_arreglo, 0, n - 1, &n_procesos, n_nivel, nivel_arbol);*///Ultimo cambio
	int id_actual = (int)getpid();

	merge_sort_recursion(arreglo_de_n, 0, n - 1, n_procesos, n_nivel, nivel_arbol, n, id_actual);
	/*------------------------------------------------------*/

	/*-------------------Para probar------------------------*/
	printf("Luego de terminado merge sort:\n");
	for (int k = 0; k < n; ++k)
	{
		printf("%d ", arreglo_de_n[k]);
	}
	printf("Numero de procesos: %d\n", n_proceso);
	printf("Numero de procesos: %d\n",*ptr_n_proceso);
	printf("Numero de procesos: %d\n", n_procesos);
	/*------------------------------------------------------*/
	return 0;
}

void merge_sort_recursion(int arreglo[], int izq, int der, int cant_proceso, int n_nivel, int nivel_arbol, int tam_n, int id_padre){
	/*if (izq == der) {
        return;
    }*/

    if (izq < der)
    {
    	int med = izq + (der - izq) / 2;
    	pid_t proceso_izq = -1;
		pid_t proceso_der = -1;
		pid_t proceso_izq_op = -1;
		pid_t proceso_der_op = -1;
		int id_actual = (int)getpid();
		int id_proceso = id_actual - id_padre;
		/*------------------------*/
		/*int cant_izq = med - izq + 1;*/
		/*int cant_der = der - med;*/
		int aux_arreglo[tam_n];

		/*------------------------*/
		/*int pipe_padre_abuelo[2];*/// Pipe para comunicación padre -> abuelo
		/*------------------------*/
		int pipe_hijo_padre_izq[cant_proceso][2]; // Pipe para comunicación hijo -> padre
		int pipe_hijo_padre_der[cant_proceso][2]; // Pipe para comunicación hijo -> padre
		/*------------------------*/

		/*-----Para conservar las modificaciones del arreglo-----*/
		/*int pipe_hijo_padre_izq[2]; // Pipe para comunicación hijo -> padre

		if (pipe(pipe_hijo_padre_izq) == -1) 
		{
        	perror("Error al crear el pipe");
        	exit(EXIT_FAILURE);
    	}*/

    	/*if (n_nivel < nivel_arbol)
    	{

			if (pipe(pipe_padre_abuelo) == -1) 
			{
        		perror("Error al crear el pipe de padre a abuelo");
        		exit(EXIT_FAILURE);
    		}
    	}*/

    	if (n_nivel < nivel_arbol && id_proceso < cant_proceso)
    	{
    		/*int pipe_hijo_padre_izq[2];*/ // Pipe para comunicación hijo -> padre

			if (pipe(pipe_hijo_padre_izq[id_proceso]) == -1) 
			{
        		perror("Error al crear el pipe izquierdo");
        		exit(EXIT_FAILURE);
    		}
    	}

    	if (n_nivel < nivel_arbol && id_proceso < cant_proceso)
    	{
    		/*int pipe_hijo_padre_der[2];*/ // Pipe para comunicación hijo -> padre

			if (pipe(pipe_hijo_padre_der[id_proceso]) == -1) 
			{
        		perror("Error al crear el pipe derecho");
        		exit(EXIT_FAILURE);
    		}
    	}

		/*-------------------------------------------------------*/

		/*(*ptr_proceso > 0 && n_nivel < nivel_arbol)*/
		if (n_nivel < nivel_arbol)
		{
			if (proceso_izq_op == -1)
			{
				proceso_izq = fork();
			}
		}
		proceso_izq_op = proceso_izq;

		switch(proceso_izq_op){
			case -1:
				break;
			case 0:
				/*-----Para conservar las modificaciones del arreglo-----*/
				close(pipe_hijo_padre_izq[id_proceso][0]); // Cierra el extremo de lectura del pipe
				/*-------------------------------------------------------*/

				printf("Proceso numero: %d\n", id_proceso);
				for (int i = 0; i <= med; ++i)
				{
					printf("%d ", arreglo[i]);
				}

				merge_sort_recursion(arreglo, izq, med, cant_proceso, n_nivel + 1, nivel_arbol, tam_n, id_padre);

				/*-----Para conservar las modificaciones del arreglo-----*/

				 // Escribe los datos modificados en el pipe
        		write(pipe_hijo_padre_izq[id_proceso][1], arreglo, tam_n * sizeof(int));
       			close(pipe_hijo_padre_izq[id_proceso][1]); // Cierra el extremo de escritura del pipe
        		/*exit(EXIT_SUCCESS);*/

				/*-------------------------------------------------------*/

				printf("Proceso numero: %d\n", id_proceso);
				for (int j = 0; j <= med; ++j)
				{
					printf("%d ", arreglo[j]);
				}
			default:
				/*-----Para conservar las modificaciones del arreglo-----*/

				/* Proceso padre */
        		close(pipe_hijo_padre_izq[id_proceso][1]); // Cierra el extremo de escritura del pipe

        		// Lee los datos del pipe
        		read(pipe_hijo_padre_izq[id_proceso][0], arreglo, tam_n * sizeof(int));

        		for (int pos1 = izq; pos1 <= med; ++pos1)
        		{
        			aux_arreglo[pos1] = arreglo[pos1];
        		}

        		close(pipe_hijo_padre_izq[id_proceso][0]); // Cierra el extremo de lectura del pipe
				/*-------------------------------------------------------*/
				break;
		}

		/*(*ptr_proceso > 0 && n_nivel < nivel_arbol)*/
		if (n_nivel < nivel_arbol)
		{
			/*(proceso_der_op == -1 && proceso_izq_op > 0)*/
			if (proceso_der_op == -1)
			{
				proceso_der = fork();
			}
		}
		proceso_der_op = proceso_der;

		switch(proceso_der_op){
			case -1:
				break;
			case 0:
				/*-----Para conservar las modificaciones del arreglo-----*/
				close(pipe_hijo_padre_der[id_proceso][0]); // Cierra el extremo de lectura del pipe
				/*-------------------------------------------------------*/

				printf("Proceso numero: %d\n", id_proceso);
				for (int k = med + 1; k <= der; ++k)
				{
					printf("%d ", arreglo[k]);
				}

				merge_sort_recursion(arreglo, med + 1, der, cant_proceso, n_nivel + 1, nivel_arbol, tam_n, id_padre);

				/*-----Para conservar las modificaciones del arreglo-----*/

				 // Escribe los datos modificados en el pipe
        		write(pipe_hijo_padre_der[id_proceso][1], arreglo, tam_n * sizeof(int));
       			close(pipe_hijo_padre_der[id_proceso][1]); // Cierra el extremo de escritura del pipe
        		/*exit(EXIT_SUCCESS);*/

				/*-------------------------------------------------------*/

				printf("Proceso numero: %d\n", id_proceso);
				for (int l = med + 1; l <= der; ++l)
				{
					printf("%d ", arreglo[l]);
				}
			default:
				/*-----Para conservar las modificaciones del arreglo-----*/

				/* Proceso padre */
        		close(pipe_hijo_padre_der[id_proceso][1]); // Cierra el extremo de escritura del pipe

        		// Lee los datos del pipe
        		read(pipe_hijo_padre_der[id_proceso][0], arreglo, tam_n * sizeof(int));

        		for (int pos2 = med + 1; pos2 <= der; ++pos2)
        		{
        			aux_arreglo[pos2] = arreglo[pos2];
        		}

        		close(pipe_hijo_padre_der[id_proceso][0]); // Cierra el extremo de lectura del pipe
				/*-------------------------------------------------------*/
				break;
		}

		if (proceso_izq_op > 0 && proceso_der_op > 0)
		{
			/*------------------------------------------------------------------------*/
			// Lee los datos del pipe
        		/*read(pipe_hijo_padre_izq[0], arreglo, tam_n * sizeof(int));*/

        		/*close(pipe_hijo_padre_izq[0]);*/ // Cierra el extremo de lectura del pipe

        		// Lee los datos del pipe
        		/*read(pipe_hijo_padre_der[0], arreglo, tam_n * sizeof(int));*/

        		/*close(pipe_hijo_padre_der[0]);*/ // Cierra el extremo de lectura del pipe
			/*------------------------------------------------------------------------*/

			
			waitpid(proceso_izq, NULL, 0);
			waitpid(proceso_der, NULL, 0);

			for (int pos3 = izq; pos3 <= der; ++pos3)
			{
				arreglo[pos3] = aux_arreglo[pos3];
			}

			printf("\n");
			printf("Proceso numero: %d ", id_proceso);
			printf("lista izquierda: ");
			for (int a = izq; a <= med ; ++a)
			{
				printf("%d ", arreglo[a]);
			}
			printf("lista derecha: ");
			for (int b = med + 1; b <= der ; ++b)
			{
				printf("%d ", arreglo[b]);
			}
			merge_sort_arreglos(arreglo, izq, med, der);
			printf("=> ");
			for (int c = izq; c <= der; ++c)
			{
				printf("%d ", arreglo[c]);
			}
			printf("\n");
			/*merge_sort_arreglos(arreglo, izq, med, der, *ptr_proceso);*/
			/*merge_sort_arreglos(arreglo, izq, med, der);*/

		}else if (proceso_izq_op == -1 && proceso_der_op == -1)
		{
			merge_sort_recursion(arreglo, izq, med, cant_proceso, n_nivel + 1, nivel_arbol, tam_n, id_padre);
			merge_sort_recursion(arreglo, med + 1, der, cant_proceso, n_nivel + 1, nivel_arbol, tam_n, id_padre);

			merge_sort_arreglos(arreglo, izq, med, der);
		}
    }

    /*int med = izq + (der - izq) / 2;
    pid_t proceso_izq = -1;
	pid_t proceso_der = -1;
	pid_t proceso_izq_op = -1;
	pid_t proceso_der_op = -1;*/

	/*(*ptr_proceso > 0 && n_nivel < nivel_arbol)*/
	/*if (n_nivel < nivel_arbol)
	{
		if (proceso_izq_op == -1)
		{
			proceso_izq = fork();
		}
	}
	proceso_izq_op = proceso_izq;

	switch(proceso_izq_op){
		case -1:
			break;
		case 0:
			printf("Proceso numero: %d\n", *ptr_proceso);
			for (int i = 0; i <= med; ++i)
			{
				printf("%d ", arreglo[i]);
			}

			merge_sort_recursion(arreglo, izq, med, ptr_proceso, n_nivel + 1, nivel_arbol);

			printf("Proceso numero: %d\n", *ptr_proceso);
			for (int j = 0; j <= med; ++j)
			{
				printf("%d ", arreglo[j]);
			}
		default:
			break;
	}*/

	/*(*ptr_proceso > 0 && n_nivel < nivel_arbol)*/
	/*if (n_nivel < nivel_arbol)
	{
		if (proceso_der_op == -1 && proceso_izq_op > 0)
		{
			proceso_der = fork();
		}
	}
	proceso_der_op = proceso_der;

	switch(proceso_der_op){
		case -1:
			break;
		case 0:
			printf("Proceso numero: %d\n", *ptr_proceso);
			for (int k = med + 1; k <= der; ++k)
			{
				printf("%d ", arreglo[k]);
			}

			merge_sort_recursion(arreglo, med + 1, der, ptr_proceso, n_nivel + 1, nivel_arbol);

			printf("Proceso numero: %d\n", *ptr_proceso);
			for (int l = med + 1; l <= der; ++l)
			{
				printf("%d ", arreglo[l]);
			}
		default:
			break;
	}

	if (proceso_izq_op > 0 && proceso_der_op > 0)
	{
		waitpid(proceso_izq, NULL, 0);
		waitpid(proceso_der, NULL, 0);
		printf("\n");
		printf("Proceso numero: %d ", *ptr_proceso);
		printf("lista izquierda: ");
		for (int a = izq; a <= med ; ++a)
		{
			printf("%d ", arreglo[a]);
		}
		printf("lista derecha: ");
		for (int b = med + 1; b <= der ; ++b)
		{
			printf("%d ", arreglo[b]);
		}
		printf("=> ");
		for (int c = izq; c <= der; ++c)
		{
			printf("%d ", arreglo[c]);
		}
		printf("\n");
		merge_sort_arreglos(arreglo, izq, med, der, *ptr_proceso);
		merge_sort_arreglos(arreglo, izq, med, der);//este para el ultimo asterisco

	}else if (proceso_izq_op == -1 && proceso_der_op == -1)
	{
		merge_sort_recursion(arreglo, izq, med, ptr_proceso, n_nivel + 1, nivel_arbol);
		merge_sort_recursion(arreglo, med + 1, der, ptr_proceso, n_nivel + 1, nivel_arbol);

		merge_sort_arreglos(arreglo, izq, med, der);
	}*/
}

void merge_sort_arreglos(int arreglo[], int izq, int medio, int der){
	int cant_izq = medio - izq + 1;
	int cant_der = der - medio;

	int aux_izq[cant_izq];
	int aux_der[cant_der];

	int i, j, k;

	for (int a = 0; a < cant_izq; a++)
	{
		aux_izq[a] = arreglo[izq + a];
	}

	for (int b = 0; b < cant_der; b++)
	{
		aux_der[b] = arreglo[medio + 1 + b];
	}

	for (i = 0, j = 0, k = izq; k <= der; k++)
	{
		if ( ((i < cant_izq) && (j >= cant_der)) || ((aux_izq[i] <= aux_der[j]) && (i < cant_izq)) )
		{
			arreglo[k] = aux_izq[i];
			i++;
		}else{
			arreglo[k] = aux_der[j];
			j++;
		}
	}
}

/*void merge_sort(int arreglo, int cant_elementos, int *ptr_proceso){
	merge_sort_recursion(arreglo, 0, cant_elementos, ptr_proceso);
}*/