#include "project.h"
#include "global.h"



/*Handles a line of code. in use in second move*/
boolean handle_codeLine2(char* start, int line)
{
    boolean clean_from_errors = TRUE;
    int i;
    int j;
    int command;
    char action_word[5]; /*operation name max length*/
    
    while ((*start == ' ') || (*start == '\t'))
        start++;

/*inserting next word into the array*/        
    for (i = 0; ((*start != ' ') && (*start != '\t') && (*start != '\n')); i++, start++)
        action_word[i] = *start;
    action_word[i] = '\0';
    
    for (j = 0; j < 16; j++)
    {
        if (strcmp(action_details[j].op_name, action_word) == 0)
        {
            command = j;
        }
    }
    enterCode(line);
    
    
    /*Checks addresssing method and whether it is legal*/
    clean_from_errors = checkaddresss2(command, start, line);
    
    return clean_from_errors;
}




/*allocates another line in code_table*/
void enterCode(int line)
{
    second_move_IC++;
    
    /*code_table is empty*/
    if (numOfCode == 0)
        code_tabel = (word*)malloc(sizeof(word));
    else
        code_tabel = (word*)realloc(code_tabel, (numOfCode + 1) * sizeof(word));
        
    if (code_tabel == NULL)
    {
        printf("\nerror: in line %d \" Allocation failed \"  \n", line);
        exit(1);
    }
    
    /*line in code_table counter*/
    numOfCode++;
   
}



/*checks legality of addressing methods. in use in second move*/
boolean checkaddresss2(int action, char* start, int line)
{
    boolean isOK = TRUE;
    boolean minus = FALSE;
    int jumpLines;
    char code_word[MAX_LABEL + 2];
    char label[MAX_LABEL + 1];
    int sum = 0;
    int addressing_type = 0;
    char location_pointer;
    int i = 0;
    int code;
    int second_move_IC_saved = second_move_IC;
    int mask = 1;
    int add;
    	
    	
    code = numOfCode - 1;
    code_tabel[code].ww.w1.opcode = action_details[action].opcode;
    code_tabel[code].ww.w1.funct = action_details[action].funct;
    code_tabel[code].select.ARE = 'A'; 

    /*If there is no operand for the command*/
    while ((action_details[action].addressing[addressing_type][0] == 0) && (action_details[action].addressing[addressing_type][1] == 0) &&
        (action_details[action].addressing[addressing_type][2] == 0) && (action_details[action].addressing[addressing_type][3] == 0) && (addressing_type < 2))
    {
    		/*Enter zeros for the addressing method*/
        operands_bits(addressing_type, code,  0);
        addressing_type++;
    }
    
    while (addressing_type < 2)
    {
        while ((*start == ' ') || (*start == '\t'))
            start++;
        location_pointer = *start;
        
        i = 0;
        
        while ((*start != ' ') && (*start != '\t') && (*start != '\n') && (*start != ','))
        {
            code_word[i] = *start;
            i++;
            start++;
        }
        code_word[i] = '\0';
        
        
        /*register is contained in code_word right now*/
        if ((location_pointer == 'r') && ((code_word[1] - '0') <= 7) && ((code_word[1] - '0') >= 0) && (code_word[2] == '\0')){
        
		operands_bits(addressing_type, code, 3);
		enterCode(line);
		code_tabel[numOfCode-1].ww.mem1.machine_code_bits = (mask << (code_word[1]-'0'));
		code_tabel[numOfCode - 1].select.ARE='A';

        }
        
        /*Immediate addresss*/
        else if (location_pointer == '#')
        {
            operands_bits(addressing_type, code,  0);
            i = 1;
            
            if ((code_word[i]) == '-')
            {
                minus = TRUE;
                i++;
            }
		for (; i < strlen(code_word); i++)
                sum = (sum * 10) + (code_word[i] - '0');
                
                
            if (minus == TRUE)
                sum *= -1;
                
                
            enterCode(line);
            code_tabel[numOfCode - 1].select.ARE = 'A';
            code_tabel[numOfCode-1].ww.mem1.machine_code_bits = sum;
        
        }
        
        
        /*Relative addresssing*/
        else if (location_pointer == '%')
        {
            strcpy(label, code_word + 1);
            i = 0;
            
            
            if (notInSymbolTable(symbol_table, &i, label))
            {
                printf("\nerror: in line %d The label %s does not exist", line, label);
                isOK = FALSE;
            }
            else
             {
                operands_bits(addressing_type, code,  2);
                jumpLines = (symbol_table[i].address - second_move_IC_saved);
                enterCode(line);
                code_tabel[numOfCode - 1].select.ARE = 'A';
                code_tabel[numOfCode-1].ww.mem1.machine_code_bits = jumpLines;
            }
        }
        else /*label*/
        {
            i = 0;
            
            if (notInSymbolTable(symbol_table, &i, code_word))
            {
                printf("\nerror: in line %d not identified label", line);
                isOK = FALSE;
            }
            else
            {
                
                add = symbol_table[i].address;
                
                /*the external label is added to external labels table*/
                if (symbol_table[i].isextern == TRUE)
                {
                /*no externals yet*/
                    if (numOfExtern == 0)
                        extPrint = (ext*)malloc(sizeof(ext));
                    else
                        extPrint = (ext*)realloc(extPrint, (numOfExtern + 1) * sizeof(ext));
                        
                    if (extPrint == NULL)
                    {
                        printf("\nerror: in line %d \" Allocation failed \"  \n", line);
                        exit(1);
                    }
                    
                    
                    numOfExtern++;
                    strcpy(extPrint[numOfExtern - 1].name, symbol_table[i].label_name);
                    extPrint[numOfExtern - 1].line = second_move_IC;
                }
                
                
                
                operands_bits(addressing_type, code,  1);
                enterCode(line);
                code_tabel[numOfCode-1].ww.mem1.machine_code_bits = add;
                
                }
                
                if (symbol_table[i].isextern == TRUE)
                  code_tabel[numOfCode - 1].select.ARE ='E';
                  
                else if (symbol_table[i].isextern == FALSE)
                  code_tabel[numOfCode - 1].select.ARE ='R';
                 
            }
       
        addressing_type++;
        
        while ((*start == ' ') || (*start == '\t'))
            start++;
            
            
        if (addressing_type == 1)
        {
            location_pointer = *start;
            
            if (location_pointer == ',')
                start++;
            else
            {
                printf("\nerror: in line %d missing comma\n", line);
                return FALSE;
            }
        }
      
    } /* end of outer while */
    
    while ((*start == ' ') || (*start == '\t'))
        start++;
        
        
    if (*start != '\n')
    {
        printf("\nerror: in line %d Unnecessary operand", line);
        return FALSE;
    }
    
    
    
    return isOK;
}






