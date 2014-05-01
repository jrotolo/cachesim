#include <stdio.h>
#include <string.h>

int addr_space = 24;
int block_size;
int num_sets;
int assoc;
char write_policy;
int cache_size;
int num_blocks;

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
	int offset = block_size;
	int index = num_sets;
	int counter = 0;

	/* Log base 2 of block_size to find offset */
	while (offset >>= 1)
	{
		counter++;
	}
	offset = counter;
	counter = 0;

	/* Log base 2 of num_sets to find index */	
	while (index >>= 1)
	{
		counter++;
	}
	index = counter;
	
	int tag = addr_space - offset - index;

	cache_size = (num_sets * block_size * assoc) / 1024;
	num_blocks = num_sets * assoc;
   
	int extra_space = (num_blocks * tag) / 8;
	double percentage = ((double)extra_space / (cache_size * 1024)) * 100;

	printf("%dKB %d-way associative cache:\n", cache_size, assoc);
	printf("%2s Block size = %d bytes\n", "",  block_size);
	printf("%2s Number of [sets,blocks] = [%d,%d]\n", "", num_sets, num_blocks);
	printf("%2s Extra space for tag storage = %d bytes (%1.4g%)\n", "", extra_space, percentage);
	printf("%2s Bits for [tag,index,offset] = [%d, %d, %d] = %d\n", "", tag, index, offset, tag+index+offset);
	
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
	bin[5] = "\0";
}

int main(int argc, char *argv[]) 
{
	GetFlags(argc, argv);	
	PrintHeader();
	
	char hex[6];
	char *bin[6];

	hex[0] = '0';
	hex[1] = 'c';
	hex[2] = '0';
	hex[3] = 'e';
	hex[4] = '7';
	hex[5] = '\0';
	
	int hexc = sizeof(hex);

	printf("%s", hex);

	HexToBin(hex, hexc, bin);

	int i = 1;
   printf("%7s", bin[0]);
	//for (i; i < hexc; i++)
	//	printf("%s", bin[i]);	
	//printf("\n");

	char *str = bin[0];
	char *newstr;
	newstr = malloc(strlen(str)+1+4);
	strcpy(newstr, str);
	for (i=1; i < hexc; i++)
		strcat(newstr, bin[i]);

	for (i = 0; i < addr_space; i++) {
		printf("%c", newstr[i]);
		//printf("i=%d\n", i);
		if (i == 8 || i == 14 )
				  printf(" ");
	}
	printf("\n");
	return 0;
}
