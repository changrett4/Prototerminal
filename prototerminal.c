#include <stdio.h>
#include <dirent.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/types.h>
#include <errno.h>
#include<sys/wait.h>

#define clear() printf("\033[H\033[J")
#define MAX_LEN_LINE 150
#define MAX_ARGS 10

void get_actual_directory(char* cwd, size_t size){
    if (getcwd(cwd, size) == NULL) {
        perror("Error to get directory");
    }
}


int execute_ls(char* arg ){

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(arg)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
        return 1;
    } else {
        perror("Error");
        return 0;
    }
}

void execute_pwd(){
    char cwd[1024];
    get_actual_directory(cwd, sizeof (cwd));
    printf("%s\n", cwd);
}

void execute_cd(char* dir){
    char cwd[1024];
    get_actual_directory(cwd, sizeof(cwd));
    char oldpwd[1024];
    strcpy(oldpwd, cwd);

    int ret = chdir(dir);
    if(ret == -1){
        printf("Error to change directory: %s\n", strerror(errno));
    } else {
        get_actual_directory(cwd, sizeof(cwd));
        printf("Current directory: %s\n", cwd);
    }
}

void initial_message(){
    clear();
    printf("______________________________________");
    printf("\n\n\tProtoshell - OS");
    printf("\n\n______________________________________");
    printf("\n");
    sleep(1);
}

void take_input(char* buffer){
    fgets(buffer, MAX_LEN_LINE, stdin);
    buffer[strcspn(buffer,"\n")] = '\0'; // remove o caracter '\n' da entrada
}

int execute_commands(int n_commands,char** comandos){
    int result;
    for(int i=0;i<n_commands;i++){
        if(strcmp(comandos[i], "ls") == 0){
             result =execute_ls((comandos[i + 1] == NULL) ? "." : comandos[i + 1]); //if ternario que verifica se o ls vem acompanhado de um diretorio ou é o atual

             if(result==0){
                 break; //esse break é necessário pois esse é o comportamento desse comando no linux
             }
        }
        if(strcmp(comandos[i],"pwd")==0){
            execute_pwd();
            break; //esse break é necessário pois esse é o comportamento desse comando no linux
        }
        if(strcmp(comandos[i],"cd") ==0){
            execute_cd(comandos[i+1]);
        }
        if(strcmp(comandos[i],"exit")==0){
            exit(0);
        }

    }
    return 0;
}



int get_separate_commands(char* buffer,char** commands){
    int n_commands = 0;
    char* command = strtok(buffer, " ");

    while (command != NULL) {
        if (n_commands == MAX_ARGS) {
            printf("Error: Max %d args exceeded\n", MAX_ARGS);
            for (int i = 0; i < n_commands; i++) {
                free(commands[i]);
            }
            return -1;
        }
        commands[n_commands] = malloc((strlen(command) + 1) * sizeof(char));
        strcpy(commands[n_commands], command);
        n_commands++;
        command = strtok(NULL, " ");
    }
    commands[n_commands]=NULL;
    return n_commands;

}


void clear_commands(int n_commands,char **commands){
    for(int i=0; i< n_commands;i++){
        free(commands[i]);
    }
}


int main() {
    char buffer[MAX_LEN_LINE];
    int n_commands;
    char **commands= malloc((MAX_ARGS+1)*sizeof (char*)) ;
    initial_message();
    while(1){
        printf("> ");
        take_input(buffer);
        n_commands = get_separate_commands(buffer,commands);
        if(n_commands==-1){
            continue;
        }
        execute_commands(n_commands,commands);
        clear_commands(n_commands,commands);
    }
    free(commands);
	return 0;
}
