#include "project.h"
#include "global.h"



/* check the type of the current word */
type_of_word wordType(char* start, int* h){


    char str[MAX_LABEL];
    int i;
    
    while ((*start == ' ') || (*start == '\t')){
    
        start++;
        (*h)++;
    }
    
    if (*start == '\n')
        return EMPTY_LINE;
        
    if (*start == ';')
        return NOTE;
        
    for (i = 0; (*start != ' ') && (*start != '\t') && (*start != '\n'); i++, start++)
        str[i] = *start;
    str[i] = '\0';
    
    if ((strcmp(str, ".data")) == 0)
        return DATA_INT;
        
    if ((strcmp(str, ".string")) == 0)
        return DATA_STRING;
        
    if ((strcmp(str, ".extern")) == 0)
        return EXTERN;
        
    if ((strcmp(str, ".entry")) == 0)
        return ENTRY;
        
    if (islabel(str))
        return LABEL;
        
    return OTHER;
    
}



/* check if label is legal */
boolean islabel(char word[])
{
    int i;
    if (word[strlen(word) - 1] != ':')
        return FALSE;
    if (isalpha(word[0]))
    {
        for (i = 1; i < (strlen(word) - 1); i++)
            if ((!(isalpha(word[i]))) && (!(isdigit(word[i]))))
                return FALSE;
        return TRUE;
    }
    return FALSE;
}



/* return the label */
char* getlable(char start[], int* j)
{
    static char label[MAX_LABEL];
    int i;
    while ((*start == ' ') || (*start == '\t'))
    {
        start++;
        (*j)++;
    }
    for (i = 0; (start[i] != ':'); i++)
        label[i] = start[i];
    label[i] = '\0';
    return label;
}



/* enter a symbol into symbols table */
boolean enterInSymbolTable(char* name_of_label, int address_of_label, int kindl, boolean isexternal, int line){

    int i = 0;
    
    if ((notInSymbolTable(symbol_table, &i, name_of_label)) == FALSE)/*if symbol exists*/
    {
    
        if (isexternal == TRUE)/*if external */{
              
		if (symbol_table[i].isextern == FALSE){
		
			printf("\nerror: in line %d  An internal label with this name already exists\n", line);
			return FALSE;
		}
        }
	else
	{
             printf("\nerror: in line %d  the label already exists\n", line);
             return FALSE;
	}
    }
    if ((numOfSymbols++) == 0)
    {
        symbol_table = (symbol_struct*)malloc(sizeof(symbol_struct));
        
        if (symbol_table == NULL)
	{
		printf("\nerror in line %d Allocation failed\n", line); 
		return FALSE;
	}
    }
    
    else{
    
        symbol_table = (symbol_struct*)realloc(symbol_table, numOfSymbols * sizeof(symbol_struct));
        
        if (symbol_table == NULL){
		printf("\nerror in line %d Allocation failed\n", line);
		return FALSE;
        }
    }
    strcpy(symbol_table[numOfSymbols-1].label_name, name_of_label);
    symbol_table[numOfSymbols-1].address = address_of_label;
    symbol_table[numOfSymbols-1].isextern = isexternal;
    symbol_table[numOfSymbols-1].entry = FALSE;
    
    if (kindl != -1)
        symbol_table[numOfSymbols-1].label_type = kindl;
        
    return TRUE;
}




/* check if symbol is'nt in symbol-table, 
save a pointer to the position of the label in the table */
boolean notInSymbolTable(symbol_struct * table, int * num, char name_of_label[])
{
    int i;
    for (i = 0; i < numOfSymbols; i++)
    {
        if (strcmp(table[i].label_name, name_of_label) == 0)
            return FALSE;
        (*num)++;/*//to delete//*/
    }
    return TRUE;
}




/* check if extern label is legal */
char* legal_symbol(char* start, int length){

    static char str[MAX_LABEL];
    int i;
    
    while (((*start) == ' ') || ((*start) == '\t'))
        start++;
    start += 7;
    
    while ((*start == ' ') || (*start == '\t'))
        start++;
        
    for (i = 0; (*start != ' ') && (*start != '\t') && (*start != '\n'); i++, start++)
        str[i] = *start;
    str[i] = ':';
    
    str[i + 1] = '\0';
    
    if (islabel(str))
    {
        str[i] = '\0';
        return str;
    }
    return NULL;
}

































