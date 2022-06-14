
#include "my_tar.h"
#include "my_tar_debug.h"

listnode_target * list_name_of_target(listnode_target *head, char *name, int index)
{
    listnode_target *new = malloc(sizeof(listnode_target*));
    new->name = name;
    new->index = index;
    new->next = NULL;

    if(!head){
        head = new;
        return head;
    }

    listnode_target *ptr = head;

    while (ptr->next) ptr = ptr->next;
    ptr->next = new;

    return head;

}

listnode_target *read_name_of_target_from_argv(int ac, char **argv, flag_t *flag)
{
    listnode_target *ret = NULL;
    if(flag->pass_option == true && flag->pass_archive == true){
        for(int i = 3; i < ac; i++){
            ret = list_name_of_target(ret, argv[i], (i - 3));
        }
    }

    debug D_read_name_of_target_from_argv(ret, flag);

return ret;
}

void print_list(listnode_target *head)
{
    listnode_target *print = head;

    for(int i = 0; print; i++){
        printf("%s\n", print->name);
        print = print->next;
    }
}

int check_target_in_folder(listnode_target *target, flag_t *flag)
{
    int ret = 0;
    if(!target){
        ret = 22; //checking target in current folder has failed, no target(s) was inputted
        return ret;
    }
    listnode_target *head = target;

    DIR *file;
    struct dirent *dp;
    char *path = ".";
    bool not_found_flag = false;

    while(head){
        if ((file = opendir(path)) == NULL){
            ret = 23; //target in current folder has failed, current folder cannot be opened
            return ret;
        }else{
            while((dp = readdir(file))){
                if(strcmp(dp->d_name, head->name) == 0){
                    ret = 1;
                    debug D_check_target_in_folder(flag, ret, head->name);
                    if(head->next == NULL && not_found_flag == false){
                        ret = 0;
                        flag->pass_target = true;
                        return ret;
                    }
                }
            }
            if(ret == 0){
                ret = 24;   //certain target name is not found in current folder
                debug D_check_target_in_folder(flag, ret, head->name);
            }
            if(head->next != NULL && ret == 24){
                ret = 25;   //searching for the other inputted targets in current folder
                not_found_flag = true;
                debug D_check_target_in_folder(flag, ret, head->name);
            }
            if(head->next == NULL && ret == 24){
                ret = 26;   //searching for the inputted targets in the current folder has failed
                return ret;
            }
            head = head->next;
            ret = 0;
        }
    }

    if(not_found_flag == true) ret = 26;
    
    closedir(file);
    return ret;
}

int parsing_tar_targets(int ac, char **argv, flag_t *flag) // [ C:\Users\mszabo\Desktop\ELU\my_tar\my_tar.exe ] [ -fhg ] [ file.tar ] [ file1.txt file2.txt ] (options, tar-file, arguments)
{
    int ret = 0;
    if(flag->pass_option == false || flag->pass_archive == false){
        ret = 20; // both option and archive settings have failed
        debug D_parsing_tar_targets(flag, ret, NULL);
        return ret;
    }
    listnode_target *target = read_name_of_target_from_argv(ac, argv, flag);
    if(target == NULL){
        ret = 21; // get name of the targets from input has failed
        debug D_parsing_tar_targets(flag, ret, NULL);
        return ret;
    }else{
        debug D_parsing_tar_targets(flag, ret, NULL); // target name(s) is in linked list      
    }
    ret = check_target_in_folder(target, flag);
    
    debug D_parsing_tar_targets(flag, ret, NULL);

    return ret;   
}

int parsing_tar_archive(int ac, char **argv, flag_t *flag)
{
    int m = 0;
    int ret = 0;
    bool not_found_flag = false;
    char *buffer = malloc(sizeof(char*) *50);
    if(flag->pass_option == true){
        for(int i = 1; i < ac; i++){
            for(int j = 0; argv[i][j]; j++){
                if(argv[i][j] == '.' && flag->pass_archive == false){  //.tar
                    for(int k = j; argv[i][k]; k++){
                        buffer[m] = argv[i][k]; //.tar
                        m++;
                    }
                    buffer[m] = '\0';
                    m = 0;
                    //printf("%s\n",buffer );
                    if(strcmp(buffer, ".tar") == 0){
                        ret = 0;
                        debug D_parsing_tar_archive(flag, ret, argv[i]);
                        if(i == 2){
                            flag->pass_archive = true;
                            debug D_parsing_tar_archive(flag, ret, argv[i]);
                            free(buffer);                            
                            return ret;
                        }else{
                            ret = 10; // 10. archive tar file name is found, but keep searching // tar file name must be given after options like -ct file.tar target1.txt target2.txt}
                            debug D_parsing_tar_archive(flag, ret, argv[i]);
                        }
                    }else if(not_found_flag == false){
                        ret = 11; //archile file name is not inputted in the place of archive file name, archive file name is neither correct nor inputted in the correct place
                        not_found_flag = true;
                        debug D_parsing_tar_archive(flag, ret, argv[i]);
                    }
                }
            }
        }      
    }
    if(ret == 11){
        free(buffer);
        return ret;
    }
    if(flag->pass_archive == false){ 
        ret = 12; //inputting archive tar file name has failed
        debug D_parsing_tar_archive(flag, ret, NULL);
    }

free(buffer);
return ret;
}

flag_t *set_flag_false(flag_t *flag)
{
    flag->c = false;
    flag->r = false;
    flag->t = false;
    flag->u = false;
    flag->x = false;
    flag->f = false;
    flag->D = false;
    flag->pass_option = false;
    flag->pass_archive = false;
    flag->pass_target = false;

    return flag;
}

int check_valid_option(flag_t *flag)
{
    int ret = 0;
    if((flag->c + flag->r + flag->t + flag->u + flag->x) > 1){
        ret = 1; //only one option may be specified out of the following options -cf -rf -tf -uf -xf
        D_check_valid_option(flag, ret);
        flag->pass_option = false;
        return ret; // return because later there might be more error codes given
    }else if((flag->c + flag->r + flag->t + flag->u + flag->x) == 0){
        ret = 2;
        D_check_valid_option(flag, ret);
        flag->pass_option = false;
        return ret;
    }else if(flag->f == false){
        ret = 3;
        D_check_valid_option(flag, ret);
        flag->pass_option = false;
        return ret;        
    }


return ret;
}

int parsing_tar_option(int ac, char **argv, flag_t *flag)
{
    flag = set_flag_false(flag);
    int ret = 0;
    int check_argv_2X = 2;
    for(int i = 0; i < ac; i++){
        if(argv[i][0] == '-'){
            while(check_argv_2X){
                for(int j = 1; argv[i][j]; j++){
                    switch(argv[i][j]){
                        case 'c':   flag->c = true;
                                    if(flag->f == true) flag->pass_option = true;
                                    break;
                        case 'r':   flag->r = true;
                                    if(flag->f == true) flag->pass_option = true;
                                    break;
                        case 't':   flag->t = true;
                                    if(flag->f == true) flag->pass_option = true;
                                    break;
                        case 'u':   flag->u = true;
                                    if(flag->f == true) flag->pass_option = true;
                                    break;
                        case 'x':   flag->x = true;
                                    if(flag->f == true) flag->pass_option = true;
                                    break;
                        case 'f':   flag->f = true;
                                    break;
                        case 'D':   flag->D = true;
                                    break;                                                                                                 
                        default:    if(check_argv_2X == 2) printf("%sX%s %soption%s %s%c%s is not an available option in this project\n",red, reset, blue, reset ,red ,argv[i][j], reset);
                                    break;
                    }
                }
                check_argv_2X--;            
            }
        }
    }

    ret = check_valid_option(flag);
    if(ret == 0 && flag->pass_option == true){
        ret = parsing_tar_archive(ac, argv, flag);
    }
    if(ret == 0 && (flag->c == true || flag->r == true || flag->u == true)){
        ret = parsing_tar_targets(ac, argv, flag);
    }
    debug D_parsing_tar_option(flag); //executed only in case debugging function is called
    
    return ret;
}

int main(int ac, char **argv)
{
    flag_t *flag = malloc(sizeof(flag_t));
    int ret;
    if((ret = parsing_tar_option(ac, argv, flag)) > 0) return ret;

    if(flag->c == true){
        //do this
    }else if(flag->r == true){
        //do this
    }else if(flag->t == true){
        //do this
    }else if(flag->u == true){
        //do this
    }else if(flag->u == true){
        //do this
    }
printf("ret value: %d\n", ret);

    return ret;

}