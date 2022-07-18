
#include "project.h"
#include "global.h"




/*handels a string type line */
boolean lineOfString(char* cur, char* last, int line)
{
    char* left;
    char* right;
    cur += 7; /* '.string' = 7 characters */
    left = cur;
    right = last;
    
    while (((*left) == ' ') || ((*left) == '\t'))
        left++;
        
    while (((*right) == ' ') || ((*right) == '\t'))
        right--;

    if ((*right != '"') || (*left != '"') || (right == left))
    {
        fprintf(stderr,"\nerror: in line %d missing: \" ", line);
        return FALSE;
    }
    
    for (left += 1; left < right; left++)
    {
        data_space_allocator(DATA_STRING, *left, 0, line);
        DC++;
    }
    
    data_space_allocator(DATA_STRING, '\0', 0, line);
    DC++;
    
    return TRUE;
}


		
		
/*handels a data type line */
boolean lineOfData(char* cur, int length, int line)
{
    char ch;
    int sum;
    boolean plus;
    boolean minus;
    boolean digit;
    boolean legal = TRUE;
    cur += 5; /* '.data' = 5 characters */
    ch = *cur;
    
    
    while (length)
    {
        sum = 0;
        plus = FALSE;
        minus = FALSE;
        digit = FALSE;
        
        while ((ch == ' ' || ch == '\t'))
        {
            ch = *(++cur);
            length--;
        }
        
        if (ch == '+')
        {
            plus = TRUE;
            ch = *(++cur);
            length--;
        }
        
        if ((ch == '-') && (plus == FALSE))
        {
            minus = TRUE;
            ch = *(++cur);
            length--;
        }
        
        while (isdigit(ch))
        {
            digit = TRUE;
            sum = (sum * 10) + (ch - '0');
            ch = *(++cur);
            length--;
        }
        
        if ((ch == ' ') || (ch == '\t') || (ch == ',') || (ch == '\n'))
        {
            if (minus == TRUE)
                sum *= (-1);
            if (digit == FALSE)
                legal = FALSE;
        }
        
        if (legal == TRUE)
        {
            data_space_allocator(DATA_INT, '0', sum, line);
            DC++;
        }
         else
	{
	    fprintf(stderr,"\nerror: in line %d missing a number between the two commas",line);
                  return FALSE;
	}
        while ((ch == ' ' || ch == '\t') && (length > 0))
        {
            ch = *(++cur);
            length--;
        }
        if (ch == '\n')
        {
            if (legal == TRUE)
                return TRUE;
            return FALSE;
        }
        if (ch == ',')
        {
            ch = *(++cur);
            length--;
        }
        else
        { 
            fprintf(stderr,"\nerror: in line %d Missing comma\",\"",line);
                  return FALSE;
        }
             
    }
    return TRUE;
}





/*handels a code type line. in use in the first move*/
boolean lineOfCode(char* start, int length, int line)
{
    int L;
    boolean legalname = FALSE;
    int i;
    int j;
    int command;
    char action_word[5]; /*operation name max length*/
    
    while ((*start == ' ') || (*start == '\t'))
        start++;
        
    for (i = 0; ((*start != ' ') && (*start != '\t') && (*start != '\n')); i++, start++)
        action_word[i] = *start;
    action_word[i] = '\0';
    
    for (j = 0; j < 16; j++)
    {
        if (strcmp(action_details[j].op_name, action_word) == 0)
        {
            legalname = TRUE;
            command = j;
        }
    }
    
    if (legalname == FALSE)
    {
        fprintf(stderr,"\nerror: in line %d invalid command", line);
        return FALSE;
    }
    else
    {
        L = checkAddress(command, start, line);
        
        if (L == -1)
            return FALSE;
        IC += L;
        
        return TRUE;
    }
}




/* allocates space for data */
void data_space_allocator(type_of_word data, char ch, int num, int line)
{
    if (numOfData == 0)
    {
        data_lines = (memory_code*)malloc(sizeof(memory_code));
        
        if (data_lines == NULL)
        {
            printf("\nerror: in line %d Allocation failed", line);
            exit(1);
        }
    }
    else
    {
        data_lines = (memory_code*)realloc(data_lines, (numOfData + 1) * sizeof(memory_code));
        
        if (data_lines == NULL)
        {
            printf("\nerror: in line %d Allocation failed", line);
            exit(1);
        }
    }
    
    
    if (data == DATA_INT)
        data_lines[numOfData].machine_code_bits = num;
        
    if (data == DATA_STRING)
        data_lines[numOfData].machine_code_bits = ch;
        
        
    numOfData++;
}






