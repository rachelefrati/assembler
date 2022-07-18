#include "project.h"
#include "global.h"

/* the second move in translation */
int second_step(FILE* fd2)
{
    boolean isOK = TRUE;
    int line_counter = 1;
    
    while (feof(fd2) == 0)
    {
        char curr_line[MAX_LINE_LENGTH];
        char *entrys;
        char *save_label;
        char *curr_position;
        int i;
        int h;
        int j = 0;
        type_of_word  wt;
        
        
        fgets(curr_line, MAX_LINE_LENGTH, fd2);
        
       /* if (feof(fd2) != 0)
        {
            break;
        }!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! no need!!!!!!!!!!!!!!!!!!!!!!!!!!*/
        
        curr_position = curr_line; /* pointer to the current position in line */
        h = 0;
        wt = wordType(curr_position, &h); 
             
        if (wt == LABEL) 
        {
            save_label= getlable(curr_position, &j); /* get the name of the label */
            curr_position += strlen(save_label) + j + 1; /* updates the current position in the line - after the label */
            h = 0;
            wt = wordType(curr_position, &h); /* skips the label */
        }
        
        
        
        if (wt == NOTE || wt == EMPTY_LINE) /* chek if the line is an empty line */
            line_counter++;
            
        else if ((wt == EXTERN) || (wt == DATA_STRING) || (wt == DATA_INT)) 
            line_counter++;
            
        else if (wt == ENTRY) /* add to 'entry' table */
        {
            numOfEntry++;
            curr_position += (6 + h);/* 6 = word entry length*/
            entrys = stringWord(curr_position);
            
            i = 0;
            if (notInSymbolTable(symbol_table, &i, entrys))
            {
                fprintf(stderr,"\nerror: in line %d label does not exist\n", line_counter);
                isOK = FALSE;
            }
            else
            {
                symbol_table[i].entry = TRUE;
            }
            line_counter++;
        }
        else
        {
            if (handle_codeLine2(curr_position, line_counter) == FALSE)/*Handles a line of code*/{
            	 fprintf(stderr,"\nerror exist in second move");
                isOK = FALSE;
                }
            line_counter++;
        }
        
    } /* end of while */
    
    
    if (isOK == FALSE)
    {

        return 1;
    }
    return 0;
}

/*return the next word*/
char *stringWord(char* start)
{
    int i = 0;
    static char str[MAX_LABEL + 2];
    while ((*start == '\t') || (*start == ' '))
        start++;
        
    while ((*start != ' ') && (*start != '\t') && (*start != '\n'))
    {
        str[i] = *start;
        start++;
        i++;
    }
    str[i] = '\0';
    
    return str;
}




	/*Fills the bits of the address*/
void operands_bits(int operand, int numIC,  int address)
{
	if (operand == 0)
		code_tabel[numIC].ww.w1.source_address = address;
    	
     else
     	code_tabel[numIC].ww.w1.target_address = address;
    
}
  









