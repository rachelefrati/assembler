#include "project.h"
#include "global.h"


/*project made by: Hadas Wagschal 212036602 Rachel Efrat Gabay 212010961

 
The aim of the project is to translate code written in assembly language into machine language code 
according to the data coding rules in the course booklet.
The translation operation is done using a two-pass algorithm.

When in the first move-
Identifying the labels that appear in the program and give them a numerical value that expresses their address in memory.

In the second move-
Using the values ​​of the symbols, as well as the operation codes, and the registers-
the machine code is built.

If errors occur during translation, error messages will be printed on the screen.

Otherwise-
Output files will be generated. 
When the file  ob. Contains: Addresses in memory, machine code words in hexadecimal base and A-R-E property.
If there are external labels, another file will be generated with the .ext extension.
And if there are internal labels, another file will be generated with the .ent extension.
*/

 action_table action_details[16] = { /* table of all commands which contains: string, code, number of arguments, permittable params in source destination.
	the 4 permittable params are: immediate, direct, indirect, register. 1 means permitted 0 means not permitted. */
    {"mov",0,0,{{0,1,0,1},{1,1,0,1}}},
    {"cmp",1,0,{{1,1,0,1},{1,1,0,1}}},
    {"add",2,10,{{1,1,0,1},{0,1,0,1}}},
    {"sub",2,11,{{1,1,0,1},{0,1,0,1}}},
    {"lea",4,0,{{0,1,0,0},{0,1,0,1}}},
    {"clr",5,10,{{0,0,0,0},{0,1,0,1}}},
    {"not",5,11,{{0,0,0,0},{0,1,0,1}}},
    {"inc",5,12,{{0,0,0,0},{0,1,0,1}}},
    {"dec",5,13,{{0,0,0,0},{0,1,0,1}}},
    {"jmp",9,10,{{0,0,0,0},{0,1,1,0}}},
    {"bne",9,11,{{0,0,0,0},{0,1,1,0}}},
    {"jsr",9,12,{{0,0,0,0},{0,1,1,0}}},
    {"red",12,0,{{0,0,0,0},{0,1,0,1}}},
    {"prn",13,0,{{0,0,0,0},{1,1,0,1}}},
    {"rts",14,0,{{0,0,0,0},{0,0,0,0}}},
    {"stop",15,0,{{0,0,0,0},{0,0,0,0}}}
	};  

	int numOfSymbols; /* number of symbols */
	int numOfData; /* number of data lines */
	int numOfCode; /* number of code lines*/
	int numOfEntry; /* number of entry symbols*/
	int numOfExtern; /* number of extern symbols*/
	int second_move_IC; /* IC to the second passage */
	symbol_struct* symbol_table; /* pointer to the symbol table */
	memory_code* data_lines; /* pointer to  location of the data lines in memory */
	ext* extPrint; /* pointer to the storage location of the external symbols - that we need to print - in  memory */
	word* code_tabel;



int main(int argc, char* argv[]){


    FILE* file_pointer;
    int error;
 
	char name[MAX_FILE_NAME];
	char namer[MAX_FILE_NAME];
	
	if (argc == 1)
        fprintf(stderr,"\nNo file name is given\n");
        
	while (--argc){
    
       
        numOfSymbols = 0;
        numOfData = 0;
        numOfCode = 0;
        numOfEntry = 0;
        numOfExtern = 0;
        second_move_IC = MEMORY_START_ADDRESS; 
        
        strcpy(name, argv[argc]);
        strcpy(namer, name);
        strcat(namer, ".as");
        file_pointer = fopen(namer, "r");
        
		if (file_pointer == NULL)
			fprintf(stderr,"\nThe file\"%s.as\"does not exist.\n", name);
            
		else
        {
            error = first_step(file_pointer);
           
            /*no error occured in first move*/
            if (error == 0)   
            {
                rewind(file_pointer);
                error = second_step(file_pointer);
                
                
             /*no error occured in second move*/   
                if (error == 0)   
                  outputter(name);
            }
            
            fclose(file_pointer);

        }
    }
    
    free(code_tabel);
    free(symbol_table);
    
    
    
    return 0;
    
    
}  







