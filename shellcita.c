#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

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
       // buffer = chop_newline(buffer); //chop newline from line      
        size_t len = strlen(buffer);
        if(buffer[len-1] == '\n'){
            buffer[len-1] = '\0';
        }
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
        size_t max_cmd_parts = 100;
        
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
        printf("============="); 
        free(tokens);
            
        int status;
        for(i=0;i<index;i++){
            printf("%d \n",i);
            char ** cmd = cmds[i];
            pid_t c_pid = fork();
            if(c_pid<0){ //fork error
                perror("error: ");
                fprintf(stdout,"error: %s \n",strerror(errno));
                _exit(1);
            }
            else if(c_pid == 0){ //child process
                if(strcmp(cmd[0],"cd") == 0){
                }
                else if(strcmp(cmd[0],"pwd") == 0){
                }
                else if(strcmp(cmd[0],"exit") == 0){
                }
                else{
                    printf("%s \n",cmd[0]);   
                }
                _exit(0);
            
            }
            else{ // parent process
                wait(&status);   
                printf("%d \n",status); 
            }
        }
            
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
