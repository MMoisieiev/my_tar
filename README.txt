cat tar..


Error codes
    The tar utility returns 0 on success, and >0 if an error occurs.

option error codes
    1. attempt of specifying more than one options at once, only one option may be specified out of the following options -cf -rf -tf -uf -xf
    2. no option was inputted, one option must be specified out of the following options -cf -rf -tf -uf -xf
    3. option f must be specified along with one main option, see the following examples -cf -rf -tf -uf -xf

archive error codes
    10. archive tar file name is found, but not inputted in the correct place
    11. archive tar file name is neither correct nor inputted in the correct place
    12. inputting archive tar file name has failed

target error codes
    20. both option and archive settings have failed, tar target function has stopped
    21. reading name of the target(s) from input has failed
    22. checking target in current folder has failed, no target(s) was inputted
    23. checking target in current folder has failed, current folder cannot be opened
    24. certain target name is not found in current folder
    25. certain target name is not found in current folder, but searching for the other inputted targets in current folder
    26. searching for the inputted targets in the current folder has failed

option -attempt of calling more options at once
    //incorrect
    tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option

option -c +archive +targets
    //incorrect
    tar: Refusing to write archive contents to terminal (missing -f option?)
    tar: Error is not recoverable: exiting now

option -cf +archive +targets
    //correct
    makes .tar file

option -f +archive +targets
    //incorrect
    tar: You must specify one of the '-Acdtrux', '--delete' or '--test-label' options
    Try 'tar --help' or 'tar --usage' for more information.

option -rf +archive +targets
    //correct
    makes .tar file

option -t +argument
    //incorrect
    Refusing to read archive contents from terminal (missing -f option?)
   
option -tf
    //incorrect
    tar: option requires an argument -- 'f'

option -tf +argument
    //correct
    my_ls.c
    my_ls.h

option -u
    //incorrect
    tar: Options '-Aru' are incompatible with '-f -'

option -u +archive +targets
    //incorrect
    tar: Options '-Aru' are incompatible with '-f -'

option -uf +archive +targets //need tar file already created
    //correct
    makes .tar file

option -x
    //incorrect
    tar: Refusing to read archive contents from terminal (missing -f option?)

option -xf
    //incorrent
    tar: option requires an argument -- 'f'

option -xf +argument
    //correct
    extracts specified tar file


#include "my_tar.h"
#include "my_tar_debug.h"

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

int parsing_tar_option(int ac, char **argv)
{

    flag_t *flag = malloc(sizeof(flag_t));
    flag = set_flag_false(flag);
    int ret = 0;
    for(int i = 0; i < ac; i++){
        if(argv[i][0] == '-'){
            for(int j = 1; argv[i][j]; j++){ 
                switch(argv[i][j]){
                    case 'c':   flag->c = true;
                                flag->pass_option = true;
                                break;
                    case 'r':   flag->r = true;
                                flag->pass_option = true;
                                break;
                    case 't':   flag->t = true;
                                flag->pass_option = true;
                                break;
                    case 'u':   flag->u = true;
                                flag->pass_option = true;
                                break;
                    case 'x':   flag->x = true;
                                flag->pass_option = true;
                                break;
                    case 'f':   flag->f = true;
                                flag->pass_option = true;
                                break;
                    case 'D':   flag->D = true;
                                break;                                                                                                 
                    default:    printf("%sX%s %soption%s %s%c%s is not an available option in this project\n",red, reset, blue, reset ,red ,argv[i][j], reset);
                                break;
                }
            }
        }
    }
 
    ret = parsing_tar_archive(ac, argv, flag);
    if(ret > 0) return ret;
    ret = parsing_tar_targets(ac, argv, flag);
    debug D_parsing_tar_option(flag); //executed only in case debugging function is called
    
    return ret;
}

int parsing_tar_archive(int ac, char **argv, flag_t *flag)
{
    int m = 0;
    int ret = 0;    //error code needs to be figured out, if no .tar was found print something like {no valid tar file name was given}
    char *buffer = malloc(sizeof(char*) *30);
    if(flag->pass_option == true){
        for(int i = 1; i < ac; i++) {
            for(int j = 0; argv[i][j]; j++){
                if(argv[i][j] == '.' && flag->pass_archive == false){  //.tar
                    for(int k = j; argv[i][k]; k++){
                        buffer[m] = argv[i][k]; //.tar
                        m++;
                    }
                    buffer[m] = '\0';
                    if(strcmp(buffer, ".tar") == 0){
                        debug D_parsing_tar_archive(flag, ret, argv[i]);
                        if(i == 2){
                            flag->pass_archive = true;
                            debug D_parsing_tar_archive(flag, ret, argv[i]);                            
                        }else{
                            ret = 10; // error code, needs to be figured out!!!!! according to the error code, something should be printed like {tar file name must be given after options like -ct file.tar target1.txt target2.txt}
                            debug D_parsing_tar_archive(flag, ret, argv[i]);
                        }
                        break;
                    }else{
                        m = 0;
                        ret = 11;
                        debug D_parsing_tar_archive(flag, ret, argv[i]);
                    } 
                }
            }
        }      
    }

    if(flag->pass_archive == false){ 
        ret = 12;
        debug D_parsing_tar_archive(flag, ret, NULL);
    }

return ret;
}

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

listnode_target *get_name_of_target_from_argv(int ac, char **argv, flag_t *flag)
{
    listnode_target *ret = NULL;
    if(flag->pass_option == true && flag->pass_archive == true){
        for(int i = 3; i < ac; i++){
            ret = list_name_of_target(ret, argv[i], (i - 3));
        }
    }

    debug D_get_name_of_target_from_argv(ret, flag);

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
        ret = 32; // check target has failed, target(s) has not been found
        return ret;
    }
    listnode_target *head = target;

    DIR *file;
    struct dirent *dp;
    char *path = ".";

    while(head){
        if ((file = opendir(path)) == NULL){
            ret = 20;
            return ret;
        }else{
            while((dp = readdir(file))){
                if(strcmp(dp->d_name, head->name) == 0){
                    ret = -1;
                    debug D_check_target_in_folder(flag, ret, head->name);
                    if(head->next == NULL){
                        ret = 0;
                        flag->pass_target = true;
                        return ret;
                    }
                }
            }
            if(ret == 0){
                ret = 21;   // target not found in current folder
                debug D_check_target_in_folder(flag, ret, head->name);
            }
            if(head->next != NULL && ret == 21){
                ret = 22;   // target not found but there is more to search
                debug D_check_target_in_folder(flag, ret, head->name);
            }
            if(head->next == NULL && ret == 21){
                ret = 23;   // target not found and no more to search
                return ret;
            }
            head = head->next;
            ret = 0;
        }
    }
    closedir(file);
    return ret;
}

int parsing_tar_targets(int ac, char **argv, flag_t *flag) // [ C:\Users\mszabo\Desktop\ELU\my_tar\my_tar.exe ] [ -fhg ] [ file.tar ] [ file1.txt file2.txt ] (options, tar-file, arguments)
{
    int ret = 0;
    if(flag->pass_option == false || flag->pass_archive == false){
        ret = 30; // both option and archive settings have failed
        debug D_parsing_tar_targets(flag, ret, NULL);
        return ret;
    }
    listnode_target *target = get_name_of_target_from_argv(ac, argv, flag);
    if(target == NULL){
        ret = 31; // get name of the targets from input has failed
        debug D_parsing_tar_targets(flag, ret, NULL);
        return ret;        
    }else{
        debug D_parsing_tar_targets(flag, ret, NULL); // target name(s) is in linked list      
    }
    ret = check_target_in_folder(target, flag);
    
    debug D_parsing_tar_targets(flag, ret, NULL);

    return ret;   
}

int main(int ac, char **argv)
{
    int ret = 0;
    ret = parsing_tar_option(ac, argv);

printf("ret value: %d\n", ret);

    return ret;

}