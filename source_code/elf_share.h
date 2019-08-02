#ifndef _elf_share 
#define _elf_share


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Legal values for e_machine (architecture).  */

#define EM_NONE		 0		/* No machine */
#define EM_M32		 1		/* AT&T WE 32100 */
#define EM_SPARC	 2		/* SUN SPARC */
#define EM_386		 3		/* Intel 80386 */
#define EM_68K		 4		/* Motorola m68k family */
#define EM_88K		 5		/* Motorola m88k family */
#define EM_860		 7		/* Intel 80860 */
#define EM_MIPS		 8		/* MIPS R3000 big-endian */
#define EM_S370		 9		/* IBM System/370 */
#define EM_MIPS_RS3_LE	10		/* MIPS R3000 little-endian */

#define EM_PARISC	15		/* HPPA */
#define EM_VPP500	17		/* Fujitsu VPP500 */
#define EM_SPARC32PLUS	18		/* Sun's "v8plus" */
#define EM_960		19		/* Intel 80960 */
#define EM_PPC		20		/* PowerPC */
#define EM_PPC64	21		/* PowerPC 64-bit */
#define EM_S390		22		/* IBM S390 */

#define EM_V800		36		/* NEC V800 series */
#define EM_FR20		37		/* Fujitsu FR20 */
#define EM_RH32		38		/* TRW RH-32 */
#define EM_RCE		39		/* Motorola RCE */
#define EM_ARM		40		/* ARM */
#define EM_FAKE_ALPHA	41		/* Digital Alpha */
#define EM_SH		42		/* Hitachi SH */
#define EM_SPARCV9	43		/* SPARC v9 64-bit */
#define EM_TRICORE	44		/* Siemens Tricore */
#define EM_ARC		45		/* Argonaut RISC Core */
#define EM_H8_300	46		/* Hitachi H8/300 */
#define EM_H8_300H	47		/* Hitachi H8/300H */
#define EM_H8S		48		/* Hitachi H8S */
#define EM_H8_500	49		/* Hitachi H8/500 */
#define EM_IA_64	50		/* Intel Merced */
#define EM_MIPS_X	51		/* Stanford MIPS-X */
#define EM_COLDFIRE	52		/* Motorola Coldfire */
#define EM_68HC12	53		/* Motorola M68HC12 */
#define EM_MMA		54		/* Fujitsu MMA Multimedia Accelerator*/
#define EM_PCP		55		/* Siemens PCP */
#define EM_NCPU		56		/* Sony nCPU embeeded RISC */
#define EM_NDR1		57		/* Denso NDR1 microprocessor */
#define EM_STARCORE	58		/* Motorola Start*Core processor */
#define EM_ME16		59		/* Toyota ME16 processor */
#define EM_ST100	60		/* STMicroelectronic ST100 processor */
#define EM_TINYJ	61		/* Advanced Logic Corp. Tinyj emb.fam*/
#define EM_X86_64	62		/* AMD x86-64 architecture */
#define EM_PDSP		63		/* Sony DSP Processor */

#define EM_FX66		66		/* Siemens FX66 microcontroller */
#define EM_ST9PLUS	67		/* STMicroelectronics ST9+ 8/16 mc */
#define EM_ST7		68		/* STmicroelectronics ST7 8 bit mc */
#define EM_68HC16	69		/* Motorola MC68HC16 microcontroller */
#define EM_68HC11	70		/* Motorola MC68HC11 microcontroller */
#define EM_68HC08	71		/* Motorola MC68HC08 microcontroller */
#define EM_68HC05	72		/* Motorola MC68HC05 microcontroller */
#define EM_SVX		73		/* Silicon Graphics SVx */
#define EM_ST19		74		/* STMicroelectronics ST19 8 bit mc */
#define EM_VAX		75		/* Digital VAX */
#define EM_CRIS		76		/* Axis Communications 32-bit embedded processor */
#define EM_JAVELIN	77		/* Infineon Technologies 32-bit embedded processor */
#define EM_FIREPATH	78		/* Element 14 64-bit DSP Processor */
#define EM_ZSP		79		/* LSI Logic 16-bit DSP Processor */
#define EM_MMIX		80		/* Donald Knuth's educational 64-bit processor */
#define EM_HUANY	81		/* Harvard University machine-independent object files */
#define EM_PRISM	82		/* SiTera Prism */
#define EM_AVR		83		/* Atmel AVR 8-bit microcontroller */
#define EM_FR30		84		/* Fujitsu FR30 */
#define EM_D10V		85		/* Mitsubishi D10V */
#define EM_D30V		86		/* Mitsubishi D30V */
#define EM_V850		87		/* NEC v850 */
#define EM_M32R		88		/* Mitsubishi M32R */
#define EM_MN10300	89		/* Matsushita MN10300 */
#define EM_MN10200	90		/* Matsushita MN10200 */
#define EM_PJ		91		/* picoJava */
#define EM_OPENRISC	92		/* OpenRISC 32-bit embedded processor */
#define EM_ARC_A5	93		/* ARC Cores Tangent-A5 */
#define EM_XTENSA	94		/* Tensilica Xtensa Architecture */
#define EM_ALTERA_NIOS2 113		/* Altera Nios II */
#define EM_AARCH64	183		/* ARM AARCH64 */
#define EM_TILEPRO	188		/* Tilera TILEPro */
#define EM_MICROBLAZE	189		/* Xilinx MicroBlaze */
#define EM_TILEGX	191		/* Tilera TILE-Gx */
#define EM_NUM		192


void elf_e_type_reader(short e_type){
	
	//printf("e_type = %d, ",e_type);
	switch (e_type){
		case 0:
			printf("This header do not have file type.\n");
			break;
		case 1:
			printf("This is a Relocatable file.\n");
			break;
		case 2:
			printf("This is a Executable file.\n");
			break;
		case 3:
			printf("This is a Shared object file.\n");
			break;
		case 4:
			printf("This is a Core file.\n");
			break;
		case 5:
			printf("five file types have been defined.\n");
			break;
		case 6:
			printf("This is a Processor-specific file.\n");
			break;
		case 7:
			printf("This is a Processor-specific file.\n");
			break;
		default:
			printf("This is a Undefine file type.\n");
			break;
	}
	return;
}

void elf_e_machine_reader(short e_machine){
	
	printf("e_machine = %hd, ",e_machine);
	switch (e_machine){
		case 0:
			printf("No machine architecture record.\n");
			break;
		case 1:
			printf("this is AT&T WE 32100 architecture.\n");
			break;
		case 2:
			printf("this is SPARC architecture.\n");
			break;
		case 3:
			printf("this is Intel 80386 architecture.\n");
			break;
		case 4:
			printf("this is Motorola 68000 architecture.\n");
			break;
		case 5:
			printf("this is Motorola 88000 architecture.\n");
			break;
		case 7:
			printf("this is Intel 80860 architecture.\n");
			break;
		case 8:
			printf("this is MIPS RS3000 architecture.\n");
			break;
		default:
			printf("this is a Undefine machine architecture.\n");
			break;
	}
	return;
}

void is_little_endian(char input){
	
	if(input == 0){
		printf("This is a little-endian file.\n");
	}
	else if(input == 1){
		printf("This is a big-endian file.\n");
	}
	else{
		printf("Error: e_idnet = %x, not little-endian or big-endian, something goes wrong.\n",input);
	}
	
	return;
}

#endif