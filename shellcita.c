#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SHELL "shellcita$ "
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    

    while(1){
        printf(SHELL);
        char * buffer = malloc(sizeof(char)*BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        
        //first sanitation of ;
        size_t max_num_token = 10;

        char ** tokens = malloc(sizeof(char*)*max_num_token);
        size_t token_length = 20;
        char * token = malloc(sizeof(char)*token_length);
        token = strtok(buffer,";");
        size_t index = 0;
        while(token != NULL){
            tokens[index] = token;
            token = strtok(NULL,";");
            index++;
        }       
        free(token);
        
        //second sanitation of white spaces
        size_t max_cmd_parts = 10;
        char *** cmds = malloc(sizeof(char*)*index); //arreglo "3D" en donde se apunta a los distintos comandos y sus distintas partes
        
        int* cmd_lens = malloc(sizeof(int)*index);//arreglo para guardar la cantidad de partes por comando
        int i;
        for(i=0;i<index;i++){ //index nos ayuda a saber cuantos comandos en total se pasaron
            char ** cmd = malloc(sizeof(char*)*10); //cmd representa los punteros a las distintas partes del comando (ej: p1 --> "sort", p2 --> "-r")
            char *cmd_part = malloc(sizeof(char)*max_cmd_parts); //cmd_part representa un texto de una parte del comando
            cmd_part = strtok(tokens[i]," ");
            size_t cmd_index = 0;
            while(cmd_part != NULL){
                cmd[cmd_index] = cmd_part;
                cmd_part = strtok(NULL," ");
                cmd_index++;               
            }
            cmds[i]=cmd;
            cmd_lens[i] = cmd_index;
        }
        
     /*   for(i=0;i<index;i++){
            char ** cmd_tmp = cmds[i];
            int j;
            for(j=0;j<cmd_lens[i];j++){
                printf("%s ", cmd_tmp[j]);
            }
        }
     */
    }

    return 0;
}
