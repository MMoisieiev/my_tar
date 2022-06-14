#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#define red "\033[1;31m"
#define green "\033[1;32m"
#define yellow "\033[1;33m"
#define blue "\033[1;34m"
#define purple "\033[1;35m"
#define cyan "\033[1;36m"
#define reset "\033[0m"
#define OK "\033[1;32mX\033[0m"
#define NOK "\033[1;31mX\033[0m"
#define debug if(flag->D == true)

#ifndef STRUCTs_flag
#define STRUCTs_flag
typedef struct s_flag
{
    bool c; /*Creates Archive [need arguments]
              Create a new archive containing the specified items.*/
    bool r; /*update or add file or directory in already existed .tar file
              Like -c, but new entries are appended to the archive. The -f option is required.*/
    bool t; /*displays or lists files in archived file
              List archive contents to stdout.*/
    bool u; /*archives and adds to an existing archive file 
              Like -r, but new entries are added only if they have a modification 
              date newer than the corresponding entry in the archive. The -f option is required.*/
    bool x; /*Extract the archive 
              Extract to disk from the archive. If a file with the same name appears more than once in the archive, 
              each copy will be extracted, with later copies overwriting (replacing) earlier copies.*/
    bool f; /*creates archive with given filename
              Read the archive from or write the archive to the specified file. 
              The filename can be standard input or standard output.*/
    bool D; // only debugging option [no, D is not referring to dick]
    bool pass_option; //pass_option is true whenever a valid option combination is given

    bool pass_archive; //pass_tar is true whenever the given tar file name is valid

    bool pass_target; //pass_target is true whenever given targets' names are existing and valid

    int size;

} flag_t;
#endif

#ifndef STRUClistnodeNODE
#define STRUClistnodeNODE
typedef struct s_listnode
{
    char    *name;
    int     index;

    struct s_listnode *next;
} listnode_target;
#endif

int parsing_tar_option(int ac, char **argv, flag_t *flag);
int parsing_tar_archive(int ac, char **argv, flag_t *flag);
int parsing_tar_targets(int ac, char **argv, flag_t *flag);
int check_target_in_folder(listnode_target *target, flag_t *flag);
void print_list(listnode_target *head);
listnode_target *list_name_of_target(listnode_target *head, char *name, int index);
listnode_target *read_targets_from_argv(int ac, char **argv, flag_t *flag);
void D_parsing_tar_option(flag_t *flag);
void D_parsing_tar_archive(flag_t *flag, int print, char *archive);
flag_t *set_flag_false(flag_t *flag);
void D_read_name_of_target_from_argv(listnode_target *head, flag_t *flag);
void D_check_target_in_folder(flag_t *flag, int print, char *name);
void D_parsing_tar_targets(flag_t *flag, int print, char *name);
void D_check_valid_option(flag_t *flag, int print);
int check_valid_option(flag_t *flag);