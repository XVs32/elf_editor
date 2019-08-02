#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

short file_format_checker(unsigned char *input){

	if(input[0]==0x4d && input[1]==0x5a){
		return 1;//this is a DOS file
	}
	else if(input[0]==0x7f && input[1]==0x45 && input[2]==0x4c && input[3]==0x46){
		return 2;//this is a elf file
	}

}

int main(int argc, char *argv[]){
	
	
	int i;
	
	if(argc != 2){
		printf("ERROR: Usage: ./file_editor.o YOUR_FILE_NAME\n");//incorrect input
		return 1;
	}
	
	FILE *input;
	input = fopen(argv[1],"rb+"); //read + write in binary
	
	fseek(input, 0, SEEK_END);	
	int file_size = ftell(input); //get file size
	unsigned char *file;
	file=malloc(file_size*sizeof(char)); //file buffer memory allocation

	if(input == NULL){//file name wrong
		printf("Error: Can not find the file.\n");
		return 1;
	}
	
	fseek(input, 0, SEEK_SET);	
	fread(file,file_size*sizeof(char),1,input); //read the whole file to buffer
	
	fclose(input);
	
	short file_format = file_format_checker(file); //check file format
	
	if(file_format==1){
		printf("This is a PE format file.\n");
		return 0;
	}
	
	if(file_format==2){
		
		printf("This is a ELF format file.\n");
		
		short file_32_64 = elf_is_32_64(file); //check ELF type(32-bit or 64-bit)
		elf_32_header header_32;
		
		switch (file_32_64){
			case 0:
				printf("Invalid class, not 32 bit or 64 bit.\n");
				break;
			case 1: //32-bit elf
				
				header_32 = elf_32_header_reader(file);//read elf header
				
				printf("This is 32 bit object.\n");
				elf_32_e_type_reader(header_32); //read and analyze e_type value
				is_little_endian(header_32.e_idnet[5]); //read and analyze endian type
				
				printf("\n"); //print basic information
				printf("e_version:%x\n",header_32.e_version);
				printf("entry_point:%x\n",header_32.e_entry);
				printf("e_shstrndx:%x\n",header_32.e_shstrndx);
				printf("section offset:%x\n",header_32.e_shoff);
				printf("section size:%d\n",header_32.e_shentsize);
				printf("\n");
				
				char *string_table_32;
				
				while(1){
				
					
					header_32 = elf_32_header_reader(file);//read elf header
					
					//read string table
					string_table_32 = elf_32_section_header_string_table_reader(header_32,file);
					
					printf("Print the whole header:-555\n\n");
			
					printf("Which section you want to read?\n");
					
					
					elf_32_session_header session_header;
					char sh_name[50];
					for(i=0;i<header_32.e_shnum;i++){
						memset(sh_name,0,50*sizeof(char));//initialize
						//read section header
						session_header = elf_32_session_header_reader(file,header_32.e_shoff+header_32.e_shentsize*i);
						//read section name
						sscanf(&string_table_32[session_header.sh_name], "%s", sh_name);

						if(strlen(sh_name)==0){continue;}
						
						printf("%s:",sh_name); //print section name
						int j,len;
						
						//leave space to keep second column align
						len = 20-strlen(sh_name);
						for(j=0;j<len;j++){
							printf(" ");
						}
						
						//print section index
						printf("%x\n",session_header.sh_name);
					}
					printf("Exit:               -1\n\n");
					
					

					char EOF_cleaner;
					int mode = -2;
					scanf("%x",&mode);//read operation mode or section index
					scanf("%c",&EOF_cleaner);//clear EOL
					
					char target_name[50];
					
					if(mode == -0x1){
						break;
					}
					else if(mode == -0x555){
						
						//print section header
						printf("\ne_ident:");
						for(i=0;i<16;i++){
							printf("%x ",header_32.e_idnet[i]);
						}
						printf("\n");
						
						printf("e_type: %hd\n" , header_32.e_type);
						printf("e_machine: %hd\n" , header_32.e_machine);
						printf("e_version: %d\n" , header_32.e_version);
						printf("e_entry: %d\n" , header_32.e_entry);
						printf("e_phoff: 0x%x\n" , header_32.e_phoff);
						printf("e_shoff: 0x%x\n" , header_32.e_shoff);
						printf("e_flags: %d\n" , header_32.e_flags);
						printf("e_ehsize: %hd\n" , header_32.e_ehsize);
						printf("e_phentsize: %hd\n" , header_32.e_phentsize);
						printf("e_phnum: %hd\n" , header_32.e_phnum);
						printf("e_shentsize: %hd\n" , header_32.e_shentsize);
						printf("e_shnum: %hd\n" , header_32.e_shnum);
						printf("e_shstrndx: %hd\n" , header_32.e_shstrndx);
						
						printf("\n");
						
						continue;
					}

					//read section header and match with index which input by user
					for(i=0;i<header_32.e_shnum;i++){
						
						session_header = elf_32_session_header_reader(file,header_32.e_shoff+header_32.e_shentsize*i);

						if(session_header.sh_name==mode){break;}
						
					}	
					
					//error if no result match
					if(i==header_32.e_shnum){
						printf("Error: Session not found.\n");
					}
					else{
						
						//start analyze and edit the corresponding section
						sscanf(&string_table_32[session_header.sh_name], "%s", sh_name);
						elf_32_session_editer(file,session_header,sh_name,header_32,file_size,argv[1]);
						
						//read file to buffer again, for the next round of task
						input = fopen(argv[1],"rb+"); //read + write in binary
						fseek(input, 0, SEEK_END);	
						file_size = ftell(input);
						file=malloc(file_size*sizeof(char));

						if(input == NULL){
							printf("Error: Can not find the file.\n");
							return 1;
						}

						fseek(input, 0, SEEK_SET);	
						fread(file,file_size*sizeof(char),1,input);

						fclose(input);
					}
					
				}

				break;
			case 2:
				//64-bit elf
				printf("This is 64 bit object.\n");
				
				break;
			default:
				//error: elf header show a invalid value
				printf("Invalid class, not 32 bit or 64 bit.\n");
				
				header_32 = elf_32_header_reader(file);
				break;
		}

	}

	free(file);

	return 0;
}


