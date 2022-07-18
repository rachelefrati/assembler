
#include "project.h"
#include "global.h"
int IC;
int DC;
int ICF;
int DCF;

/*first move in the translation*/
int first_step(FILE *fd){
	
	boolean isOK = TRUE;
	int line_counter = 1;
	
	
	/* array that contains the current line */
	char curr_line[MAX_LINE_LENGTH];  
	
	/* array that contains the current label (if any) */
	char curr_label[MAX_LABEL];  
	
	char* curr_position;
	int end_of_line;
	char *lastchar;
	int flag_label = FALSE;
	char *save_label;
	char* outer_label;
	int h,c;
	int j = 0;
	type_of_word type;
	IC = MEMORY_START_ADDRESS;
     DC = 0;
		
	while (feof(fd) == 0)
	{
		fgets(curr_line, MAX_LINE_LENGTH, fd);
		
		
		/* pointer to the current position in line */	 
		curr_position = curr_line; 
		end_of_line = strlen(curr_line);
		lastchar = (curr_position + end_of_line - 2);
		/*pointer to the begining of the current word*/
		h = 0;
		
		type = wordType(curr_position, &h);
		
		
		if(type == LABEL){
			
			flag_label = TRUE;
			
			/* keep the label */
			save_label= getlable(curr_position, &j); 
			strcpy(curr_label, save_label);
			end_of_line -= (strlen(curr_label) + j + 1);
			
			/* updates the current position in the line - after the label */
			curr_position += strlen(curr_label) + j + 1; 
        
			
		}
		
		type = wordType(curr_position, &h);
		
		if (type == NOTE || type == EMPTY_LINE)
            line_counter++;
            
		 else if (type == DATA_INT || type == DATA_STRING){
		 
            if (flag_label == TRUE)
                if (enterInSymbolTable(curr_label, DC, DATA, FALSE, line_counter) == FALSE)
                    isOK = FALSE;
            
            /*an integer number */         
            if (type == DATA_INT)
           	/*handling the data (integers only)*/
                if (lineOfData(curr_position + h, end_of_line, line_counter) == FALSE)
                    isOK = FALSE;
                    
            if (type == DATA_STRING)
            /*handling the data (string only)*/
                if (lineOfString(curr_position + h, lastchar, line_counter) == FALSE)
                    isOK = FALSE;
                    
                    
            line_counter++;
       	 }/*data is handeled. integers ot string*/
       	 
        else if (type == ENTRY)
            line_counter++;
            
            
            
            
        else if (type == EXTERN)
        {
        	/* enter in 'outer_label' the current label */
            outer_label = legal_symbol(curr_position + h, end_of_line); 
            
            if (outer_label == NULL)
            {
                fprintf(stderr,"\nerror: in line %d invalid external label", line_counter);
                isOK = FALSE;
            }
            else
                if (enterInSymbolTable(outer_label, 0, -1, TRUE, line_counter) == FALSE)
                    isOK = FALSE;
            line_counter++;
        }
        
        else
        {
            if (flag_label == TRUE)
		
                enterInSymbolTable(curr_label, IC, CODE, FALSE, line_counter);
		
            if (lineOfCode(curr_position, end_of_line, line_counter) == FALSE)
                isOK = FALSE;
            line_counter++;
        }
        
      flag_label=FALSE;  
        
    } /* end of line of code */
    
    
    
    if (isOK == FALSE)
   	 return 1;
    
    ICF = IC;
    DCF = DC;
    
    
    /*number of symbols counter*/
    for (c = 0; c < numOfSymbols; c++)
    {
        if (symbol_table[c].isextern == FALSE)
            if (symbol_table[c].label_type == DATA)
                symbol_table[c].address += ICF;
    }
    
    
    return 0;
}
		
		



/*checks address of operands and return the number of command lines .
in use in the first move*/
int checkAddress(int action, char* start, int line)
{
    int add = 5;
    char wordw[MAX_LABEL + 2];
    int L = 0;
    int addressing_type = 0;
    char ch;
    int i = 0;
    
    
    while ((action_details[action].addressing[addressing_type][0] == 0) && (action_details[action].addressing[addressing_type][1] == 0) &&
        (action_details[action].addressing[addressing_type][2] == 0) && (action_details[action].addressing[addressing_type][3] == 0) && (addressing_type < 2))
        addressing_type++;
        
/*addressing_type = 0 means target addressing. addressing_type = 1 means source addressing*/     
   
    while (addressing_type < 2)
    {
        while ((*start == ' ') || (*start == '\t'))
            start++;
        ch = *start;
        i = 0;
        while ((*start != ' ') && (*start != '\t') && (*start != '\n') && (*start != ','))
        {
            wordw[i] = *start;
            i++;
            start++;
        }
        wordw[i] = '\0';
        
        if ((ch == 'r')  && (wordw[2] == '\0'))
        {
			if(((wordw[1] - '0') <= 7) && ((wordw[1] - '0') >= 0))
			{
				add = 3;
			}
			else
			{
				fprintf(stderr,"\n error: in line %d Invalid register \n", line);
                return -1;
			}
		}
        else if (ch == '#')
        {
            if (!((isdigit(wordw[1])) || (wordw[1] == '-')))
            {
               fprintf(stderr,"\n error: in line %d invalid character in immediate address \n", line);
                return -1;
            }
            for (i = 2; i < strlen(wordw); i++)
            {
                if (!(isdigit(wordw[i])))
                {
                    fprintf(stderr,"\n error: in line %d invalid character in immediate address \n", line);
                    return -1;
                }
            }
            add = 0;
        }
        else if (ch == '%')
            add = 2;
        else
            add = 1;
        if (action_details[action].addressing[addressing_type][add] == 0)
        {
            if (addressing_type == 0)
                fprintf(stderr,"\nerror: in line %d The address method of the source operand is invalid\n", line);
            else
                fprintf(stderr,"\nerror: in line %d The address method of the target operand is invalid\n", line);
            return -1;
        }
        
         L += 1;
            
        addressing_type++;
        
        while ((*start == ' ') || (*start == '\t'))
            start++;
        if (addressing_type == 1)
        {
            ch = *start;
            if (ch == ',')
                start++;
            else
            {
                fprintf(stderr,"\nerror: in line %d missing comma\n", line);
                return -1;
            }
        }
    } /* end of while */
    while ((*start == ' ') || (*start == '\t'))
        start++;
    if (*start != '\n')
    {
        fprintf(stderr,"\n error: in line %d Unnecessary operand \n", line);
        return -1;
    }
    return L + 1;
}




