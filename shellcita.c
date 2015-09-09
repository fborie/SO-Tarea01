#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <linux/limits.h>

#define SHELL "shellcita$ "
#define BUFFER_SIZE 1024


char * get_current_working_directory(){
    char * cwd = malloc(sizeof(char)*PATH_MAX);
    getcwd(cwd,PATH_MAX);
    return cwd;        
}

int change_directory(const char * path){
   return chdir(path);
}

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
            cmd[cmd_index] = NULL;
            cmds[i]=cmd;
            cmd_lens[i] = cmd_index;
            
           // free(cmd_part);
           // free(cmd);
        }
        free(tokens);
            
        int status;
        for(i=0;i<index;i++){
            char ** cmd = cmds[i];
            if(strcmp(cmd[0],"cd") == 0){//CD
                int cd = change_directory(cmd[1]);
                if(cd != 0){
                    fprintf(stdout,"cd: %s: %s\n",cmd[1],strerror(errno));
                    printf("\n");
                    fprintf(stdout,"# error: cd returned %d\n", errno);              
                }
            }
            else if(strcmp(cmd[0],"pwd") == 0){//PWD
                char * cwd = get_current_working_directory();
                if(cwd == NULL){
                    free(cwd);
                    fprintf(stdout,"pwd: %s\n",strerror(errno));
                    printf("\n");
                    fprintf(stdout,"# error: pwd returned %d\n",errno);
                }
                printf("%s \n",cwd);  
                free(cwd);
            }
            else if(strcmp(cmd[0],"exit") == 0){//EXIT
                return 0;
            }
            
            else{ //OTHER COMMANDS
                pid_t c_pid = fork();
                if(c_pid<0){ //fork error
                    fprintf(stderr,"[fork] error: %s \n",strerror(errno));
                    exit(EXIT_FAILURE);
                }
                else if(c_pid == 0){ //child process
                    //execvp(cmd[0],cmd);
                     if(execvp(cmd[0],cmd)<0){
                        fprintf(stderr,"[exec] error: %s \n",strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                    //exit(EXIT_SUCCESS);
                         
                }
                else{ // parent process
                    wait(&status);
                   // printf("STATUS: %d \n",status);
                    if(status!=0){
                        printf("\n");
                        fprintf(stdout,"# error: %s returned %d\n",cmd[0],status);
                    }
                }
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
