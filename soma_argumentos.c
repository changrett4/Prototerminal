#include <stdio.h>
#include <stdlib.h>


//arquivo .c de soma usado como teste para o comando exec_file
int main(int argc, char *argv[]) {
	    if (argc != 3) {
		printf("Uso: %s <a> <b>\n", argv[0]);
		return EXIT_FAILURE;
	    }
	    int a = atoi(argv[1]);
	    int b = atoi(argv[2]);
	    int soma = a+b;

	    printf("%d\n",soma);
	    return EXIT_SUCCESS;
}
