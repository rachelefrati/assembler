#include "project.h"
#include "global.h"

/* the method writes the three output files */
void outputter(char name[])
{
    FILE * ob_file_pointer, * ext_file_pointer, * ent_file_pointer;
    
    int line = MEMORY_START_ADDRESS;
    int i;
    int bin_machine_code;
    
    char ob_file_name[MAX_FILE_NAME];
    char ext_file_name[MAX_FILE_NAME];
    char ent_file_name[MAX_FILE_NAME];
    
    
    
    
    
    strcpy(ob_file_name, name);
    strcat(ob_file_name, ".ob");
    strcpy(ext_file_name, name);
    strcat(ext_file_name, ".ext");
    strcpy(ent_file_name, name);
    strcat(ent_file_name, ".ent");
    
    
    ob_file_pointer = fopen(ob_file_name, "w");
    
    
    /*in case .ext file is needed as output file */
    if (numOfExtern != 0)
        ext_file_pointer = fopen(ext_file_name, "w");
        
        
    /*in case .ent file is needed as output file */
    if (numOfEntry != 0)
        ent_file_pointer = fopen(ent_file_name, "w");
        


/* print the file .ext */   
	for (i = 0; i < numOfExtern; i++) 
		{
			
			fprintf(ext_file_pointer, "\n%s ", extPrint[i].name);
			fprintf(ext_file_pointer, "%04d", extPrint[i].line);
		}
		
		
		
/* print the file .ent */		
for (i = 0; i < numOfSymbols; i++) 
    {
    		/*the file .ent contains only entry labels*/
        if (symbol_table[i].entry == TRUE)
        {
            fprintf(ent_file_pointer, "\n%s ", symbol_table[i].label_name);
            fprintf(ent_file_pointer, "%04d", symbol_table[i].address);
        }
    }
    
    
/* print the finals: IC, DC */   
    fprintf(ob_file_pointer, "%d %d", ICF - MEMORY_START_ADDRESS, DCF); 
    
    
    /*print .ob file with the needed data:
     addresses , machine code words and A-R-E features
     according to input file
     the printing is divided by 2 'for' loops-
     for printing code info and data info separately*/
     
     
     
	for(i = 0; i < numOfCode; i++){
	
		fprintf(ob_file_pointer, "\n%04d ",(MEMORY_START_ADDRESS + i));
		bin_machine_code = code_tabel[i].ww.mem1.machine_code_bits;
		fprintf(ob_file_pointer,"%03X ", bin_machine_code);

		/*some ARE are not valid, thats why the characters are unknown and the file output seems to be Chinese*/
		if(code_tabel[i].select.ARE=='A' ||code_tabel[i].select.ARE=='R'||code_tabel[i].select.ARE=='E')
			fprintf(ob_file_pointer,"%c",code_tabel[i].select.ARE);
		line++;
		
	}
		
		
		
	
/* print the data info*/	
	for (i = 0; i < numOfData; i++){ 

		fprintf(ob_file_pointer, "\n%04d ", line); 
		bin_machine_code = data_lines[i].machine_code_bits;
		fprintf(ob_file_pointer,"%03X ",bin_machine_code);
		fprintf(ob_file_pointer,"%c",'A');
				
		line++;
	}
	
	
	fclose(ob_file_pointer);
	
	
	/*in case .ext file is opened for use as output file*/
    if (numOfExtern != 0)
        fclose(ext_file_pointer);
        
        
	/*in case .ent file is opened for use as output file*/
    if (numOfEntry != 0)
        fclose(ent_file_pointer);
}   
    
   
   
    
    
  
    
    
    
    
    
  
