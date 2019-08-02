#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "elf_share.h"

typedef struct tem_elf_64_header{

	unsigned char e_idnet[16];//magic number
	unsigned short e_type;//Object file type 
	unsigned short e_machine;//Architecture
	unsigned int e_version;// Object file version 
	unsigned long long int e_entry;// Entry point virtual address 
	unsigned long long int e_phoff;// Program header table file offset 
	unsigned long long int e_shoff;// Section header table file offset 
	unsigned int e_flags;// Processor-specific flags 
	unsigned short e_ehsize;// ELF header size in bytes 
	unsigned short e_phentsize;// Program header table entry size 
	unsigned short e_phnum;// Program header table entry count 
	unsigned short e_shentsize;// Section header table entry size 
	unsigned short e_shnum;// Section header table entry count 
	unsigned short e_shstrndx;// Section header string table index 
	
}elf_64_header;


typedef struct {

	unsigned int sh_name;
	unsigned int sh_type;
	unsigned long long int sh_flags;
	unsigned long long int sh_addr;
	unsigned long long int sh_offset;
	unsigned long long int sh_size;
	unsigned int sh_link;
	unsigned int sh_info;
	unsigned long long int sh_addraling; 
	unsigned long long int sh_entsize;
	
}elf_64_session_header;



elf_64_header elf_64_header_reader(FILE *input,elf_64_header header){
	
	FILE *file = input;
	
	fseek(file,0,SEEK_SET);
	
	int i;
	
	for(i=0;i<16;i++){
		fread(&header.e_idnet[i],1,1,file);
	}
	fread(&header.e_type,2,1,file);
	fread(&header.e_machine,2,1,file);
	fread(&header.e_version,4,1,file);
	fread(&header.e_entry,8,1,file);
	fread(&header.e_phoff,8,1,file);
	fread(&header.e_shoff,8,1,file);
	fread(&header.e_flags,4,1,file);
	fread(&header.e_ehsize,2,1,file);
	fread(&header.e_phentsize,2,1,file);
	fread(&header.e_phnum,2,1,file);
	fread(&header.e_shentsize,2,1,file);
	fread(&header.e_shnum,2,1,file);
	fread(&header.e_shstrndx,2,1,file);
	
	return header;
}



elf_64_session_header elf_64_session_header_reader(FILE *input,elf_64_session_header header,int offset){
	
	FILE *file = input;
	
	fseek(file,offset,SEEK_SET);
	
	fread(&header.sh_name,4,1,file);
	fread(&header.sh_type,4,1,file);
	fread(&header.sh_flags,8,1,file);
	fread(&header.sh_addr,8,1,file);
	fread(&header.sh_offset,8,1,file);
	fread(&header.sh_size,8,1,file);
	fread(&header.sh_link,4,1,file);
	fread(&header.sh_info,4,1,file);
	fread(&header.sh_addraling,8,1,file);
	fread(&header.sh_entsize,8,1,file);
	
	return header;
}





short elf_64_e_type_adapter(elf_64_header header){
	return header.e_type;
}

void elf_64_e_type_reader(elf_64_header header){
	elf_e_type_reader(elf_64_e_type_adapter(header));
}






short elf_64_e_machine_adapter(elf_64_header header){
	return header.e_machine;
}

void elf_64_e_machine_reader(elf_64_header header){
	elf_e_machine_reader(elf_64_e_machine_adapter(header));
}




void elf_64_session_editer(FILE *input, elf_64_session_header session_header,char *sh_name,
						   elf_64_header header,char *file_name){

	while(1){
		
		printf("section name:%s\n",sh_name);
		printf("section addr:0x%llx\n",session_header.sh_addr);
		printf("section offset:0x%llx\n",session_header.sh_offset);
		printf("section size:%lld\n\n",session_header.sh_size);
					
		printf("What do you want to do?\n");
		printf("Print the whole %s:P\n",sh_name);
		printf("Find byte with value:F <Hex value>\n");
		printf("Find two byte with value(in big-endian):L <Hex value>\n");
		printf("Edit two byte with value and address(in big-endian):E <Hex value> <Hex address>\n");
		printf("Delete X bytes:D <Hex address> <X>\n");
		printf("Exit:Q\n\n");

		unsigned short count = 0;
		unsigned short ins;
		char mode, EOF_cleaner;
		scanf("%c",&mode);
		scanf("%c",&EOF_cleaner);//clear EOL

		if(mode == 'P'){

			fseek(input,session_header.sh_offset,SEEK_SET);
			count = 0;
			while(count<session_header.sh_size){

				if(count % 16==0){
					printf("\n%08X\t",count);
				}

				fread(&ins,1,1,input);
				printf("%02hhx",ins);
				count++;
				
				if(count % 2==0){
					printf(" ");
				}
				
				//if(strcmp(sh_name,".rodata")==0 && count % 16==0){
				if(count % 16==0){
					
					fseek(input,-16,SEEK_CUR);
					int i;
					char tem;
					printf("\t");
					for(i=0;i<16;i++){
						fread(&tem,1,1,input);
						if(tem>=32&&tem<=126){
							printf("%c",tem);
						}
						else{
							printf(".");
						}
						
					}
				}

			}
			
			
			//if(strcmp(sh_name,".rodata")==0 && count % 16!=0){
			if(count % 16!=0){
				
				int i;
				for(i=0;i<(16-(count % 16))*2;i++){
					if(i%2==0){
						printf(" ");
					}
					printf(" ");
				}
				
				fseek(input,-(count % 16),SEEK_CUR);
				char tem;
				printf("\t");
				for(i=0;i<count % 16;i++){
					fread(&tem,1,1,input);
					if(tem>=32&&tem<=126){
						printf("%c",tem);
					}
					else{
						printf(".");
					}

				}
			}
			printf("\n");

		}
		else if(mode == 'F'){
			
			short flag = 0;
			unsigned char target;
			scanf("%hhx",&target);
			scanf("%c",&EOF_cleaner);//clear EOL
			fseek(input,session_header.sh_offset,SEEK_SET);

			count = 0;

			while(count<session_header.sh_size){
				fread(&ins,1,1,input);
				if(ins == target){
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
			fseek(input,session_header.sh_offset,SEEK_SET);

			count = 0;

			while(count<session_header.sh_size){
			
				fread(&ins,2,1,input);
				fseek(input,-1,SEEK_CUR);	
				if(ins == target){
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
			
			fseek(input,session_header.sh_offset+target_address,SEEK_SET);
			
			printf("%x %x\n",new_ins,target_address);
			fwrite(&new_ins,2,1,input);
			//fread(&new_ins,2,1,input);
			//printf("%x\n",new_ins);

			printf("Done\n");


		}
		else if(mode == 'D'){
			

			unsigned short target_address,times;
			scanf("%hx",&target_address);
			scanf("%hd",&times);
			scanf("%c",&EOF_cleaner);//clear EOL
			
			while(times--){
				int target_offset = session_header.sh_offset;

				elf_64_session_header session_header_tem;

				int i;

				for(i=0;i<header.e_shnum;i++){

					session_header_tem = elf_64_session_header_reader(input,session_header_tem,
								 header.e_shoff+header.e_shentsize*i);

					if(session_header_tem.sh_offset==target_offset){
						session_header.sh_size -= 1;
						fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8+8+8,SEEK_SET);
						fwrite(&session_header.sh_size,8,1,input);
						//printf("in");
					}

					if(session_header_tem.sh_offset>target_offset){
						session_header_tem.sh_addr--;
						fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8,SEEK_SET);
						fwrite(&session_header_tem.sh_addr,8,1,input);

						session_header_tem.sh_offset--;
						fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8+8,SEEK_SET);
						fwrite(&session_header_tem.sh_offset,8,1,input);
					}



				}

				header.e_shoff--;
				fseek(input,16+2+2+4+8+8,SEEK_SET);
				fwrite(&header.e_shoff,8,1,input);

				fseek(input,target_offset+target_address+1,SEEK_SET);

				unsigned char buf;

				while(fread(&buf,1,1,input)==1){
					fseek(input,-2,SEEK_CUR);
					fwrite(&buf,1,1,input);
					fseek(input,1,SEEK_CUR);
				}
			}

			printf("Done\n");

		}
		else if(mode == 'A'){

			unsigned short new_ins,target_address;
			scanf("%hx",&new_ins);
			scanf("%hx",&target_address);
			scanf("%c",&EOF_cleaner);//clear EOL
			
			
			int target_offset = session_header.sh_offset;

			elf_64_session_header session_header_tem;

			int i;

			for(i=0;i<header.e_shnum;i++){

				session_header_tem = elf_64_session_header_reader(input,session_header_tem,
							 header.e_shoff+header.e_shentsize*i);

				if(session_header_tem.sh_offset==target_offset){
					session_header.sh_size += 2;
					fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8+8+8,SEEK_SET);
					fwrite(&session_header.sh_size,8,1,input);
					printf("in");
				}

				if(session_header_tem.sh_offset>target_offset){
					session_header_tem.sh_addr+=2;
					fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8,SEEK_SET);
					fwrite(&session_header_tem.sh_addr,8,1,input);

					session_header_tem.sh_offset+=2;
					fseek(input,header.e_shoff+header.e_shentsize*i+4+4+8+8,SEEK_SET);
					fwrite(&session_header_tem.sh_offset,8,1,input);
				}



			}

			header.e_shoff+=2;
			fseek(input,16+2+2+4+8+8,SEEK_SET);
			fwrite(&header.e_shoff,8,1,input);

			fseek(input,target_offset+target_address,SEEK_SET);

			unsigned short buf;

			while(1){
				int flag;
				flag = fread(&buf,1,2,input);
				
				if(flag == 2){
					fseek(input,-2,SEEK_CUR);
					fwrite(&new_ins,2,1,input);
					new_ins = buf;
				}
				else if(flag == 1){
					fseek(input,-1,SEEK_CUR);
					fwrite(&new_ins,1,1,input);
					break;
				}
				else{
					break;
				}
			}
			

			printf("Done\n");

		}
		else if(mode == 'Q'){
			printf("Exit\n");
			break;
		}
		else{
			printf("Undefine mode %c\n",mode);	
		}

		printf("\n");
	}

}












