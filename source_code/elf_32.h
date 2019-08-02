#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ld32.c"
#include "elf_share.h"

typedef struct{

	unsigned char e_idnet[4];//magic number
	unsigned short e_type;//Object file type 
	unsigned short e_machine;//Architecture
	unsigned int e_version;// Object file version 
	unsigned int e_entry;// Entry point virtual address 
	unsigned int e_phoff;// Program header table file offset 
	unsigned int e_shoff;// Section header table file offset 
	unsigned int e_flags;// Processor-specific flags 
	unsigned short e_ehsize;// ELF header size in bytes 
	unsigned short e_phentsize;// Program header table entry size 
	unsigned short e_phnum;// Program header table entry count 
	unsigned short e_shentsize;// Section header table entry size 
	unsigned short e_shnum;// Section header table entry count 
	unsigned short e_shstrndx;// Section header string table index 

}elf_32_header;

typedef struct {

	unsigned int sh_name;
	unsigned int sh_type;
	unsigned int sh_flags;
	unsigned int sh_addr;
	unsigned int sh_offset;
	unsigned int sh_size;
	unsigned int sh_link;
	unsigned int sh_info;
	unsigned int sh_addraling; 
	unsigned int sh_entsize;
	
}elf_32_session_header;

elf_32_header elf_32_header_reader(unsigned char *input){
	
	elf_32_header header;
	
	int i;
	
	for(i=0;i<16;i++){
		header.e_idnet[i] = *(unsigned char*)&input[i];
	}
	header.e_type = *(unsigned short*)&input[16];
	header.e_machine = *(unsigned short*)&input[18];
	header.e_version = *(unsigned int*)&input[20];
	header.e_entry = *(unsigned int*)&input[24];
	header.e_phoff = *(unsigned int*)&input[28];
	header.e_shoff = *(unsigned int*)&input[32];
	header.e_flags = *(unsigned int*)&input[36];
	header.e_ehsize = *(unsigned short*)&input[40];
	header.e_phentsize = *(unsigned short*)&input[42];
	header.e_phnum = *(unsigned short*)&input[44];
	header.e_shentsize = *(unsigned short*)&input[46];
	header.e_shnum = *(unsigned short*)&input[48];
	header.e_shstrndx = *(unsigned short*)&input[50];
	
	return header;
}

elf_32_session_header elf_32_session_header_reader(char* input,int offset){
	
	elf_32_session_header header;
	
	header.sh_name = *(unsigned int*)&input[offset+0];
	header.sh_type = *(unsigned int*)&input[offset+4];
	header.sh_flags = *(unsigned int*)&input[offset+8];
	header.sh_addr = *(unsigned int*)&input[offset+12];
	header.sh_offset = *(unsigned int*)&input[offset+16];
	header.sh_size = *(unsigned int*)&input[offset+20];
	header.sh_link = *(unsigned int*)&input[offset+24];
	header.sh_info = *(unsigned int*)&input[offset+28];
	header.sh_addraling = *(unsigned int*)&input[offset+32];
	header.sh_entsize = *(unsigned int*)&input[offset+36];
	
	return header;
}

short elf_32_e_type_adapter(elf_32_header header){
	return header.e_type;
}

void elf_32_e_type_reader(elf_32_header header){
	elf_e_type_reader(elf_32_e_type_adapter(header));
}

short elf_32_e_machine_adapter(elf_32_header header){
	return header.e_machine;
}

void elf_32_e_machine_reader(elf_32_header header){
	elf_e_machine_reader(elf_32_e_machine_adapter(header));
}

char* elf_32_section_header_string_table_reader(elf_32_header header,char *file){
	char *output;
	elf_32_session_header session_header;	
				
	session_header = elf_32_session_header_reader(file,header.e_shoff+header.e_shentsize*header.e_shstrndx);
				
	output = &file[session_header.sh_offset];
	return output;
}



char opcode_finder(unsigned char *input){
	
	int i;	
	for(i=0;;i++){
		
		if(input[i]==0xF0||input[i]==0xF2||input[i]==0xF3){continue;}
		if(input[i]==0x2E||input[i]==0x36||input[i]==0x3E||input[i]==0x26||input[i]==0x64||input[i]==0x65){continue;}
		if(input[i]==0x66){continue;}
		if(input[i]==0x67){continue;}
		
		break;
		
	}
	return input[i];
}

char two_bytes_opcode_finder(unsigned char *input){
	
	int i;	
	for(i=0;;i++){
		
		if(input[i]==0xF0||input[i]==0xF2||input[i]==0xF3){continue;}
		if(input[i]==0x2E||input[i]==0x36||input[i]==0x3E||input[i]==0x26||input[i]==0x64||input[i]==0x65){continue;}
		if(input[i]==0x66){continue;}
		if(input[i]==0x67){continue;}
		if(input[i]==0x0f){continue;}
		
		break;
		
	}
	return input[i];
}



void opcode_position(FILE *input,unsigned char list[][200]){
	
	char buf[200];
	char *ptr;
	int count = 0;
	
	fseek(input,0,SEEK_SET);
	while(fgets(buf,200, input) != NULL){
		
		ptr = strtok(buf," ");
		
		ptr = strtok(NULL," ");
		if(ptr==NULL){
			strcpy(list[count],"NULL");
			count++;
			continue;
		}
		else{
			strcpy(list[count],ptr);
			if(list[count][strlen(list[count])-1]=='\r'||list[count][strlen(list[count])-1]=='\n'){
				list[count][strlen(list[count])-1] = '\0';
			}
		}
		
		ptr = strtok(NULL," ");
		if(ptr!=NULL && strlen(ptr)>1){
			strcat(list[count],"\t");
			strcat(list[count],ptr);
			if(list[count][strlen(list[count])-1]=='\r'||list[count][strlen(list[count])-1]=='\n'){
				list[count][strlen(list[count])-1] = '\0';
			}
		}

		count++;
		
	}
	return;
}


void elf_32_session_editer(unsigned char *file, elf_32_session_header session_header,char *sh_name,
							elf_32_header header,int size,char* input_name){
						   
	while(1){
					
		printf("section name:%s\n",sh_name);
		printf("section addr:0x%x\n",session_header.sh_addr);
		printf("section offset:0x%x\n",session_header.sh_offset);
		printf("section size:%d\n\n",session_header.sh_size);
					
		printf("What do you want to do?\n");
		printf("\n");
		printf("Print the whole section header:I\n");
		printf("Print the whole %s:P\n",sh_name);
		printf("Print the whole %s in disassembly mode:Z \n",sh_name);
		printf("\n");
		printf("Find byte with value:F <Hex value>\n");
		printf("Find two byte with value(in big-endian):L <Hex value>\n");
		printf("\n");
		printf("Edit two byte with value and address(in big-endian):E <Hex value> <Hex address>\n");
		printf("Delete X bytes:D <Hex address> <X>\n");
		printf("Add byte:A <Hex value> <Hex address>\n");
		printf("\n");
		printf("Save and Exit:Q\n\n");
		printf("Exit without Save: Any Char else\n\n");

		unsigned short count = 0;
		unsigned short ins;
		char mode, EOF_cleaner;
		scanf("%c",&mode);
		scanf("%c",&EOF_cleaner);//clear EOL

		if(mode == 'P'){

			count = 0;
			while(count<session_header.sh_size){

				if(count % 16==0){
					printf("\n%08X\t",count);
				}

				printf("%02hhx",file[session_header.sh_offset+count]);
				
				count++;
				
				if(count % 2==0){
					printf(" ");
				}
				
				if(count % 16==0){
					
					int i;
					char *tem;
					printf("\t");
					for(i=0;i<16;i++){
						tem = &file[session_header.sh_offset+count-16+i];
						if(*tem>=32&&*tem<=126){
							printf("%c",*tem);
						}
						else{
							printf(".");
						}
					}
				}
				
			}
			
			if(count % 16!=0){

				int i;
				for(i=0;i<(16-(count % 16))*2;i++){
					if(i%2==0){
						printf(" ");
					}
					printf(" ");
				}

				
				char *tem;
				printf("\t");
				for(i=0;i<count % 16;i++){
					tem = &file[session_header.sh_offset+count-(count % 16)+i];
					if(*tem>=32&&*tem<=126){
						printf("%c",*tem);
					}
					else{
						printf(".");
					}

				}
			}
			
			printf("\n");

		}
		else if(mode == 'I'){

			printf("sh_name: %x\n",session_header.sh_name);
			printf("sh_type: %x\n",session_header.sh_type);
			printf("sh_flags: %x\n",session_header.sh_flags);
			printf("sh_addr: %x\n",session_header.sh_addr);
			printf("sh_offset: %x\n",session_header.sh_offset);
			printf("sh_size: %x\n",session_header.sh_size);
			printf("sh_link: %x\n",session_header.sh_link);
			printf("sh_info: %x\n",session_header.sh_info);
			printf("sh_addraling: %x\n",session_header.sh_addraling);
			printf("sh_entsize: %x\n",session_header.sh_entsize);
			printf("\n");

		}
		else if(mode == 'F'){
			
			short flag = 0;
			unsigned char target;
			scanf("%hhx",&target);
			scanf("%c",&EOF_cleaner);//clear EOL

			count = 0;

			while(count<session_header.sh_size){
				
				if(file[session_header.sh_offset+count] == target){
					printf("location at %X\n",count);
					flag = 1;
				}
				count ++;
			}

			if(flag == 0){
				printf("no match found\n");
			}

		}
		else if(mode == 'L'){
			
			short flag = 0;
			unsigned short target;
			scanf("%hx",&target);
			scanf("%c",&EOF_cleaner);//clear EOL

			count = 0;

			while(count<session_header.sh_size){
				
				if(*(unsigned short*)&file[session_header.sh_offset+count] == target){
					printf("location at %X\n",count);
					flag = 1;
				}
				
				count ++;
			}

			if(flag == 0){
				printf("no match found\n");
			}

		}
		else if(mode == 'E'){

			unsigned short new_ins, target_address;
			scanf("%hx",&new_ins);
			scanf("%hx",&target_address);
			scanf("%c",&EOF_cleaner);//clear EOL
			
			char *tem = (char*)&new_ins;
			
			file[session_header.sh_offset+target_address] = tem[0];
			file[session_header.sh_offset+target_address+1] = tem[1];
			
			printf("%x %x\n",new_ins,target_address);

			printf("Done\n");

		}
		else if(mode == 'D'){
			

			unsigned short target_address,times;
			scanf("%hx",&target_address);
			scanf("%hd",&times);
			scanf("%c",&EOF_cleaner);//clear EOL
			
			while(times--){
				int target_offset = session_header.sh_offset;

				elf_32_session_header session_header_tem;

				int i;

				for(i=0;i<header.e_shnum;i++){

					session_header_tem = elf_32_session_header_reader(file,header.e_shoff+header.e_shentsize*i);

					if(session_header_tem.sh_offset==target_offset){
						session_header.sh_size --;
						memcpy(&file[header.e_shoff+header.e_shentsize*i+20],&session_header.sh_size,sizeof(int));
					}
					else if(session_header_tem.sh_offset>target_offset){
						session_header_tem.sh_addr--;
						memcpy(&file[header.e_shoff+header.e_shentsize*i+12],&session_header_tem.sh_addr,sizeof(int));
						session_header_tem.sh_offset--;
						memcpy(&file[header.e_shoff+header.e_shentsize*i+16],&session_header_tem.sh_offset,sizeof(int));
					}

				}
				
				header.e_shoff--;

				memcpy(&file[32],&header.e_shoff,sizeof(int));

				char *buf;
				buf = malloc((size-target_offset-target_address-1)*sizeof(char));
				
				memcpy(buf,&file[target_offset+target_address+1],(size-target_offset-target_address-1)*sizeof(char));
				memcpy(&file[target_offset+target_address],buf,(size-target_offset-target_address-1)*sizeof(char));
				free(buf);
				size--;
			}
			
			printf("Done\n");

		}
		else if(mode == 'A'){

			unsigned short new_ins,target_address;
			scanf("%hx",&new_ins);
			scanf("%hx",&target_address);
			scanf("%c",&EOF_cleaner);//clear EOL
			
			int target_offset = session_header.sh_offset;

			elf_32_session_header session_header_tem;

			int i;

			for(i=0;i<header.e_shnum;i++){

				session_header_tem = elf_32_session_header_reader(file,header.e_shoff+header.e_shentsize*i);

				if(session_header_tem.sh_offset==target_offset){
					session_header.sh_size += 2;
					memcpy(&file[header.e_shoff+header.e_shentsize*i+20],&session_header.sh_size,sizeof(int));
				}

				if(session_header_tem.sh_offset>target_offset){
					session_header_tem.sh_addr+=2;
					session_header_tem.sh_offset+=2;
					memcpy(&file[header.e_shoff+header.e_shentsize*i+12],&session_header_tem.sh_addr,sizeof(int));
					memcpy(&file[header.e_shoff+header.e_shentsize*i+16],&session_header_tem.sh_offset,sizeof(int));
				}

			}

			header.e_shoff+=2;
			
			memcpy(&file[32],&header.e_shoff,sizeof(int));		

			char *buf;
			buf = malloc(size*sizeof(char));

			memcpy(buf,file,size*sizeof(char));			
			free(file);
			file = malloc((size+2)*sizeof(char));
			
			memcpy(file,buf,(target_offset+target_address)*sizeof(char));		
			memcpy(&file[target_offset+target_address],&new_ins,2*sizeof(char));			
			memcpy(&file[target_offset+target_address+2],&buf[target_offset+target_address],
				   (size-target_offset-target_address)*sizeof(char));
			
			size+=2;
			printf("Done\n");

		}
		else if(mode == 'Z'){
			
			FILE *one_byte_file,*two_bytes_file;
			
			one_byte_file = fopen("one-byte_opcodes.txt","r");
			two_bytes_file = fopen("two-byte_opcodes.txt","r");
			
			unsigned char one_byte_list[256][200];
			unsigned char two_bytes_list[256][200];
			
			opcode_position(one_byte_file,one_byte_list);
			opcode_position(two_bytes_file,two_bytes_list);
			
			fclose(one_byte_file);
			fclose(two_bytes_file);
			
			
			void *opcode;
			unsigned int length;
			
			unsigned char *ins_buf;
			ins_buf = malloc((session_header.sh_size+5)*sizeof(char));
			
			memcpy(ins_buf,&file[session_header.sh_offset],session_header.sh_size*sizeof(char));
			
			count = 0;
			while(count<session_header.sh_size){
				
				opcode = (void*)ins_buf;
				length = length_disasm(opcode);

				printf("\n%08X\t",count);

				int v;
				for(v=0;v<length;v++){
					printf("%02hhx ",ins_buf[v]);
				}
				
				for(v=0;v<15-length;v++){			
					printf("   ");
				}

				unsigned char op_tem = opcode_finder(ins_buf);

				if(op_tem!=0x0F){
					printf("%s",one_byte_list[op_tem]);
				}
				else{
					printf("%s",two_bytes_list[two_bytes_opcode_finder(ins_buf)]);
				}
				
				count += length;
				ins_buf += length;
				
			}
			
			printf("\n");
			
		}
		else if(mode == 'Q'){
			FILE *input;
			input = fopen(input_name,"rb+"); //read + write in binary
			fwrite(file,size*sizeof(char),1,input);
			fclose(input);
			
			printf("Exit\n");
			break;
		}
		else{
			printf("Undefine mode %c\n",mode);	
			break;
		}

		printf("\n");
	}
	
	return;

}




