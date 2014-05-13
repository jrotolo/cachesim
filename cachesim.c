/* By: Jarrod Rotolo and Caleb Leger */

#include <stdio.h>
#include <string.h>

int addr_space = 24;
int block_size;
int num_sets;
int assoc;
char write_policy;
int cache_size;
int num_blocks;
int b_tag, b_index, b_offset;
int tag = 0;
int index_ = 0; /* had to name index index_ because of error */
int offset = 0;

/* Will process and output all flags */
void GetFlags(int argc, char *argv[]) 
{
	if (argc == 1)
	{
		block_size = 32;
		num_sets = 64;
		assoc = 1;
		write_policy = 't';
	}

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'b':
				block_size = atoi(&argv[1][3]);
				break;
			case 's':
				num_sets = atoi(&argv[1][3]);
				break;
			case 'a':
				assoc = atoi(&argv[1][3]);
				break;
			case 'W':
				write_policy = argv[1][3];
				break;
			default:
				printf("Wrong Argument: %s\n", argv[1]);
		}
		argv += 2;
		argc -= 2;
	}
		
}

/* Prints header information for cache */
void PrintHeader()
{
   b_offset= block_size;
	b_index = num_sets;
	int counter = 0;

	/* Log base 2 of block_size to find offset */
	while (b_offset >>= 1)
	{
		counter++;
	}
	b_offset = counter;
	counter = 0;

	/* Log base 2 of num_sets to find index */	
	while (b_index >>= 1)
	{
		counter++;
	}
	b_index = counter;
	
	b_tag = addr_space - b_offset - b_index;

	cache_size = (num_sets * block_size * assoc) / 1024;
	num_blocks = num_sets * assoc;
   
	int extra_space = (num_blocks * b_tag) / 8;
	double percentage = ((double)extra_space / (cache_size * 1024)) * 100;

	printf("%dKB %d-way associative cache:\n", cache_size, assoc);
	printf("%2s Block size = %d bytes\n", "",  block_size);
	printf("%2s Number of [sets,blocks] = [%d,%d]\n", "", num_sets, num_blocks);
	printf("%2s Extra space for tag storage = %d bytes (%1.4g%)\n", "", extra_space, percentage);
	printf("%2s Bits for [tag,index,offset] = [%d, %d, %d] = %d\n", "", b_tag, b_index, b_offset , b_tag+b_index+b_offset);
	
	if (write_policy == 'b')
		printf("%2s Write policy = %s\n", "", "Write-back");
	else
		printf("%2s Write policy = %s\n", "", "Write-through");

	printf("\n");
	printf("%-7s %-36s %-5s %s %19s\n", "Hex", "Binary Address", "Set", "Blk", 
						 "Memory");
   printf("%-7s %-31s %s %s %4s %4s %-4s %-4s %-4s\n", "Address", 
						 "(tag/index/offset)", "Tag", "Index", "off", "Way", "UWay", 
						 "Read", "Writ");
   printf("%-7s %s %s %s %4s %4s %-4s %-4s %-4s\n", "=======", 
						 "==========================", "========", "=====", "===", 
						 "====", "====", "====", "====");
	
}

void HexToBin(char hex[], int hexc, char *bin[])
{
	int i = 0;
	while (i != hexc)
	{
		switch (hex[i])
		{
			case '0':
				bin[i] = "0000";
				break;
			case '1':
				bin[i] = "0001";
				break;
			case '2':
				bin[i] = "0010";
				break;
			case '3':
				bin[i] = "0011";
				break;
			case '4':
				bin[i] = "0100";
				break;
			case '5':
				bin[i] = "0101";
				break;
			case '6':
				bin[i] = "0110";
				break;
			case '7':
				bin[i] = "0111";
				break;
			case '8':
				bin[i] = "1000";
				break;
			case '9':
				bin[i] = "1001";
				break;
			case 'a':
				bin[i] = "1010";
				break;
			case 'b':
				bin[i] = "1011";
				break;
			case 'c':
				bin[i] = "1100";
				break;
			case 'd':
				bin[i] = "1101";
				break;
			case 'e':
				bin[i] = "1110";
				break;
			case 'f':
				bin[i] = "1111";
				break;
			default:
				break;
		}
		//printf("%s\n", bin[i]);
		i++;
	}
	bin[6] = "\0";
}

void SetProperties(char bin[])
{
	char tag_bits[b_tag];
	char index_bits[b_index];
	char offset_bits[b_offset];
	int i = 0;
	int k = 0;
	int l = 0;
	
	for (i; i < addr_space; i++) {
		if (i < b_tag) {
			tag_bits[i] = bin[i];
		}
		else if ((b_tag-1 < i) && (i < (b_tag+b_index))) {
			index_bits[k] = bin[i];
			k++;
		}
		else {
			offset_bits[l] = bin[i];
			l++;
		}
	}
	
	int j = b_tag-1;
	tag_bits[b_tag] = '0';	
	for (i=0; i < b_tag; i++, j--) { 
		if (tag_bits[i] == '1') {
			tag += 2 << j-1;
			if (i == b_tag-1)
				tag += 1;
		}
	}

	j = b_index-1;
	index_bits[b_index] = '0';
	for (i=0; i < b_index; i++, j--) {
		if (index_bits[i] == '1') {
			index_ += 2 << j-1;
			if (i == b_index-1)
				index_ += 1;
		}
	}
	
	j = b_offset-1;
	offset_bits[b_offset] = '0';
	for (i=0; i < b_offset; i++, j--) {
		if (offset_bits[i] == '1') {
			offset += 2 << j-1;
			if (i == b_offset-1)
				offset += 1;
		}
	}
	
}


int main(int argc, char *argv[]) 
{
	GetFlags(argc, argv);	
	PrintHeader();
	
	char hex[6];
	char *bin[6];

	hex[0] = '0';
	hex[1] = '0';
	hex[2] = '0';
	hex[3] = '3';
	hex[4] = '7';
	hex[5] = '6';
	hex[6] = '\0';
	
	int hexc = sizeof(hex);

	printf("%-8s", hex);

	FILE *infile;
	infile = fopen("trace.in", "r");

	if (!infile)
		printf("No file stupid\n");
	
	char operation;
	unsigned	int *newhex;
	fscanf(infile, "%c %x", operation, newhex); 
	printf("%c %x", operation, newhex);
	/* Start of one line */
	HexToBin(hex, hexc, bin);


	int i = 1;

	char *str = bin[0];
	char *newstr;
	newstr = (char *)malloc(sizeof(char *)*addr_space);
	strcpy(newstr, str);
	for (i=1; i < hexc; i++)
		strcat(newstr, bin[i]);
	newstr[addr_space] = '\0';

	for (i = 0; i < addr_space; i++) {
		printf("%c", newstr[i]);
		//printf("i=%d\n", i);
		if (i == 12 || i == 18 )
				  printf(" ");
	}
	SetProperties(newstr);
	printf("%9d %5d %4d", tag, index_, offset);	
	printf("\n");
	/* End of line */
	
	
	free(newstr);
	return 0;
}
