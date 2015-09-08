#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SHELL "shellcita$ "
#define BUFFER_SIZE 1024

/*
line = "ls -la; sort -r 1.txt"
token = "sort -r 1.txt"
cmd
*/
int main(int argc, char *argv[]){
    
    char * buffer = malloc(sizeof(char)*BUFFER_SIZE); //buffer store the input from stdin, it has to be free at the end
    while(1){
        printf(SHELL);
        fgets(buffer, BUFFER_SIZE, stdin);
        
        //first sanitization of ;
        size_t max_num_token = 10;

        char ** tokens = malloc(sizeof(char*)*max_num_token);
        if(tokens == NULL){
            fprintf(stderr,"[malloc] Allocation failed\n");
            return 1;
        }
        char * token;

        token = strtok(buffer,";");
        size_t index = 0; //len of commands that will help at the end to not allocate more memory that nedded
        while(token != NULL){
            tokens[index] = token;
            token = strtok(NULL,";");
            index++;
        }
             
        
        /*
        printf("====================================\n");
        int j;
        for(j=0;j<index;j++){
            printf("%s \n",tokens[j]);
        }
        printf("====================================\n");
        */
 
        //second sanitation of white spaces
        char *** cmds = malloc(sizeof(char*)*index); //arreglo "3D" en donde se apunta a los distintos comandos y sus distintas partes      
        if(cmds == NULL){
            fprintf(stderr,"[malloc] Allocation failed\n");
            return 1;
        } 
        
        int* cmd_lens = malloc(sizeof(int)*index);//arreglo para guardar la cantidad de partes por comando
        if(cmd_lens == NULL){ 
            fprintf(stderr,"[malloc] Allocation failed\n");
            return 1;
        }
        size_t max_cmd_parts = 10;
        
        int i;
        for(i=0;i<index;i++){ //index nos ayuda a saber cuantos comandos en total se pasaron     
            char ** cmd = malloc(sizeof(char*)*max_cmd_parts); //cmd representa los punteros a las distintas partes del comando (ej: p1 --> "sort", p2 --> "-r")
            if(cmd_lens == NULL){ 
                fprintf(stderr,"[malloc] Allocation failed\n");
                return 1;
            }

            char *cmd_part;  //cmd_part representa un texto de una parte del comando
            
            cmd_part = strtok(tokens[i]," ");
            size_t cmd_index = 0;
            while(cmd_part != NULL){
                cmd[cmd_index] = cmd_part;
                cmd_part = strtok(NULL," ");
                cmd_index++;               
            }
            cmds[i]=cmd;
            cmd_lens[i] = cmd_index;
            
           // free(cmd_part);
           // free(cmd);
        }
        
        free(tokens);
/*
        for(i=0;i<index;i++){
            char ** cmd_tmp = cmds[i];
            int j;
            for(j=0;j<cmd_lens[i];j++){
                printf("%s ", cmd_tmp[j]);
            }
        }
        printf("======================================\nFree\n");        
*/      
        for(i=0;i<index;i++){
            free(cmds[i]);
        }
        free(cmds);
        free(cmd_lens);
    }
    free(buffer);
    return 0;
}
