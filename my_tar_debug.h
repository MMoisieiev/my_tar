

void D_parsing_tar_option(flag_t *flag) // option D is only for debugging
{
    if(flag->D == true){
        printf("----------%sDebugging option%s----------\n", blue, reset);
        printf("%soption%s c      is  %s\n", blue, reset, flag->c ? green "on" reset : red "off" reset);
        printf("%soption%s r      is  %s\n", blue, reset, flag->r ? green "on" reset : red "off" reset);
        printf("%soption%s t      is  %s\n", blue, reset, flag->t ? green "on" reset : red "off" reset);
        printf("%soption%s u      is  %s\n", blue, reset, flag->u ? green "on" reset : red "off" reset);
        printf("%soption%s x      is  %s\n", blue, reset, flag->x ? green "on" reset : red "off" reset);
        printf("%soption%s f      is  %s\n", blue, reset, flag->f ? green "on" reset : red "off" reset);
        printf("%soption%s D      is  %s\n", blue, reset, flag->D ? green "on" reset : red "off" reset);
        printf("%spass_option%s   is  %s\n", blue, reset, flag->pass_option ? green "passed" reset : red "failed" reset);
        printf("%spass_archive%s  is  %s\n", yellow, reset, flag->pass_archive ? green "passed" reset : red "failed" reset);
        printf("%spass_target%s   is  %s\n", purple, reset, flag->pass_target ? green "passed" reset : red "failed" reset);                    
        printf("------------------------------------\n");
    }

}

void D_parsing_tar_archive(flag_t *flag, int print, char *archive)
{
    if(flag->D == true){
        if(flag->pass_archive == true){
            printf("----------%sDebugging archive%s----------\n", yellow, reset);
            printf("%sX%s %s is inputted correctly\n",green, reset, archive);
            printf("------------------------------------\n");
        }else if(print == 0 && flag->pass_archive == true){
            printf("----------%sDebugging archive%s----------\n", yellow, reset);
            printf("%sX%s archive name: %s is accepted\n",green, reset, archive);
            printf("------------------------------------\n");
        }else if(print == 10){
            printf("----------%sDebugging archive%s----------\n", yellow, reset);
            printf("%sX%s wrong input order, %s must be inputted after options\n",red, reset, archive);
            printf("example: -cf %sarchive.tar%s target_file_1.txt target_file_2.txt\n", red, reset);
            printf("------------------------------------\n");
        }else if(print == 11){
            printf("----------%sDebugging archive%s----------\n", yellow, reset);
            printf("%sX%s %s is inputted in the place of archive file name, %s is neither correct nor inputted in correct place\n",red, reset, archive, archive);
            printf("------------------------------------\n");
        }else if(print == 12){
            printf("----------%sDebugging archive%s----------\n", yellow, reset);
            printf("%sX%s inputting archive tar file name has failed\n", red, reset);
            printf("------------------------------------\n");
        }
    }

}

void D_read_name_of_target_from_argv(listnode_target *head, flag_t *flag)
{
    if(flag->D == true){
        listnode_target *str = head;
        printf("----------%sDebugging target%s----------\n", purple, reset);
        if(str){
            printf("%sX%s inputted target:  ", green ,reset);
        }else{
            printf("%sX%s no valid target(s) was inputted", red ,reset);
        }
        while(str){
            printf("%s  ", str->name);
            str = str->next;
        }
        printf("\n------------------------------------\n");
    }
}

void D_check_target_in_folder(flag_t *flag, int print, char *name)
{
    if(flag->D == true){
        if(print == 1){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s target name: %s is found in current folder\n",green, reset, name);
            printf("------------------------------------\n");
        }else if(print == 23){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s checking target in current folder has failed, current folder cannot be opened\n", red, reset);
            printf("------------------------------------\n");
        }else if(print == 24){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s target name: %s is not found in current folder\n", red, reset, name);
            printf("------------------------------------\n");        
        }else if(print == 25){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s searching for the other inputted targets in current folder\n", green, reset);
            printf("------------------------------------\n"); 
        }
    }
}

void D_parsing_tar_targets(flag_t *flag, int print, char *name)
{
    if(flag->D == true){
        if(print == 0 && flag->pass_target == false){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s target name(s) is in linked list\n", green, reset);
            printf("------------------------------------\n");
        }else if(print == 0 && flag->pass_target == true){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s all target name(s) is found in current folder\n", green, reset);
            printf("------------------------------------\n");
        }else if(print == 20){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s both option and archive settings have failed, tar target function has stopped\n", red, reset);
            printf("------------------------------------\n");   
        }else if(print == 21){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s reading name of the target(s) from input has failed\n", red, reset);
            printf("------------------------------------\n");
        }else if(print == 22){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s check target(s) is in current folder has failed, no target(s) was inputted\n",red ,reset); 
            printf("------------------------------------\n");
        }else if(print == 26){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s searching for the inputted target(s) in the current folder has failed\n",red ,reset); 
            printf("------------------------------------\n");
        }else if(print == -1){
            printf("----------%sDebugging target%s----------\n", purple, reset); 
            printf("%sX%s target name: %s is found in current folder\n",green ,reset, name); 
            printf("------------------------------------\n"); 
        }
    }
}

void D_check_valid_option(flag_t *flag, int print)
{
    if(flag->D == true){
        if(print == 1){
            printf("----------%sDebugging option%s----------\n", blue, reset); 
            printf("%sX%s attempt of specifying more than one options at once, only one option may be specified out of the following options: -cf -rf -tf -uf -xf\n",red ,reset); 
            printf("------------------------------------\n"); 
        }else if(print == 2){
            printf("----------%sDebugging option%s----------\n", blue, reset); 
            printf("%sX%s no option was inputted, one option must be specified out of the following options: -cf -rf -tf -uf -xf\n",red ,reset); 
            printf("------------------------------------\n"); 
        }else if(print == 3){
            printf("----------%sDebugging option%s----------\n", blue, reset); 
            printf("%sX%s option f must be specified along with one main option, see the following examples: -cf -rf -tf -uf -xf\n",red ,reset); 
            printf("------------------------------------\n"); 
        }
    }
}

