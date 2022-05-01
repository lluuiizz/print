#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

int print(char *, ...);

enum TYPES{
   SIGNED_INT, UNSIGNED_INT, LONG, LONG_LONG, DOUBLE, FLOAT
};

typedef struct t_arg_types{
    enum TYPES type;
    struct t_arg_types *next_arg;

} t_arg_types;

int main(void) {
    print("Olá bebe \n");
    return 0;    

}


int print(char *string, ...) {
    int length = strlen(string);
    int num_of_args = 0;
    char place_holders[] = {
        'd', 'f', 'u',
    };
    
    va_list arguments;
    bool has_an_error = false;

    t_arg_types *arg_types_head = NULL;
    t_arg_types *arg_types_tail = NULL;
    
    for (size_t i = 0; i < length; ++i) {
        if (string[i] == '%'){
           int previous_num_args = num_of_args;
           for (size_t j = 0; j < strlen(place_holders); ++j) {
                if(string[i + 1] == place_holders[j]) {
                    num_of_args += 1;
              
                }
          
           } 
           if (num_of_args == previous_num_args) {
                print("There is an error!!!\n");
                has_an_error = true;
                break;
           }
        
        }
    }
    
    if (has_an_error) return -1;
    
    register int syscall_no asm("rax") = 1;
    register int arg1 asm("rdi") = 1;
    register char* arg2 asm("rsi") = string;
    register int arg3 asm("rdx") = length;
    asm("syscall");
    return 0;
}
