#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

enum TYPES{
   SIGNED_INT, UNSIGNED_INT, LONG, LONG_LONG, DOUBLE, FLOAT, STRING, LONG_UNSIGNED_INT
};

typedef struct t_arg_types{
    enum TYPES type;
    struct t_arg_types *next_arg;

} t_arg_types;

t_arg_types *arg_types_head = NULL;
t_arg_types *arg_types_tail = NULL;
    
int print(char *, ...);
void insert_in_type_list (char);
void convert_arguments_to_string (va_list, char *[]);

int main(void) {

    int x = 10;
    size_t y = 124124123;

    print("Unsigned long integer variable: %t", y);
    return 0;    

}


int print(char *string, ...) {
    int length = strlen(string);
    int length_without_place_holders = length;
    int num_of_args = 0;
    char place_holders[] = {
        'd', 'f', 'u', 's', 't'
    };
    
    va_list arguments;
    bool has_an_error = false;

   
    for (size_t i = 0; i < length; ++i) {
        if (string[i] == '%'){
           int previous_num_args = num_of_args;
           for (size_t j = 0; j < strlen(place_holders); ++j) {
                if(string[i + 1] == place_holders[j]) {
                    num_of_args += 1;
                    insert_in_type_list(place_holders[j]); 
                    length_without_place_holders -= 2;
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
    
    va_start(arguments, string);
    char *converted_str[num_of_args];
    convert_arguments_to_string(arguments, converted_str);
    
    int string_final_length = length_without_place_holders ;

    for (int i = 0; i < num_of_args; ++i) string_final_length += strlen(converted_str[i]); 
    char *final_string = (char*) malloc(string_final_length);

    for (int i = 0, final_string_id = 0, arg_index = 0; i < string_final_length; ++i, ++final_string_id) {
        if (string[i] == '%') {
            i += 2;
            for (int z = 0; z < strlen(converted_str[arg_index]); ++z) {
                final_string[final_string_id] = converted_str[arg_index][z];
                final_string_id += 1;
            
            }
            arg_index += 1;
        } 
        final_string[final_string_id] = string[i];
    }
   
    va_end(arguments);
    register int syscall_no asm("rax") = 1;
    register int arg1 asm("rdi") = 1;
    register char* arg2 asm("rsi") = final_string;
    register int arg3 asm("rdx") = string_final_length ; asm("syscall"); return 0;
}

void insert_in_type_list(char place_holder) {
    t_arg_types *new_type = (t_arg_types*) malloc(sizeof(t_arg_types));
    
    if (place_holder == 'd') new_type->type = SIGNED_INT;
    else if (place_holder == 'f') new_type->type = FLOAT;
    else if (place_holder == 'u') new_type->type = UNSIGNED_INT;
    else if (place_holder == 's') new_type->type = STRING;
    else if (place_holder == 't') new_type->type = LONG_UNSIGNED_INT;

    if (arg_types_head == NULL) {
        arg_types_head = new_type;
        arg_types_head->next_arg = NULL;
        arg_types_tail = arg_types_head;
    
    } else {
        arg_types_tail->next_arg = new_type;
        arg_types_tail = new_type;
        arg_types_tail->next_arg = NULL;
    }

}


void convert_arguments_to_string (va_list arguments, char *buffer[]) {
    int i = 0;
    
   while (arg_types_head != NULL) {
        if (arg_types_head->type == SIGNED_INT) {
            int arg = va_arg(arguments, int );
            int length = snprintf(NULL, 0, "%d", arg);
            buffer[i] = (char *) malloc(length + 1);
            snprintf(buffer[i], length + 1, "%d", arg );
        } 
        else if (arg_types_head->type == UNSIGNED_INT) {
            
            unsigned int arg = va_arg(arguments, unsigned int );
            int length = snprintf(NULL, 0, "%u", arg);
            buffer[i] = (char *) malloc(length + 1);
            snprintf(buffer[i], length + 1, "%u", arg );
        }
        else if (arg_types_head->type == STRING) {
            buffer[i] = va_arg(arguments, char*);

        }
        else if (arg_types_head->type == LONG_UNSIGNED_INT) {
            size_t arg = va_arg(arguments, size_t);
            int length = snprintf(NULL, 0, "%lu", arg);
            buffer[i] = (char*) malloc(length + 1);
            snprintf(buffer[i], length + 1, "%lu", arg);
        }
        i += 1;
        arg_types_head = arg_types_head->next_arg;
    }
}
