#include <stdio.h>

int addr_space = 24;
int block_size;
int num_sets;
int assoc;
int write_policy;
int cache_size;
int num_blocks;

/* Will process and output all flags */
void GetFlags(int argc, char *argv[]) 
{
	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'b':
				block_size = atoi(&argv[1][3]);
				printf("Block size = %d\n", block_size);
				break;
			case 's':
				num_sets = atoi(&argv[1][3]);
				printf("Number of sets = %d\n", num_sets);
				break;
			case 'a':
				assoc = atoi(&argv[1][3]);
				printf("Associativity = %d\n", assoc);
				break;
			case 'W':
				write_policy = atoi(&argv[1][3]);
				printf("Write policty = %d\n", write_policy);
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
	int offset = num_sets;
	int index = num_sets;
	int counter = 1;

	while (offset != 0)
	{
		++counter;
		offset = offset >> 2;
	}
	offset = counter++;
	counter = 1;
	
	while (index != 0)
	{
		++counter;
		index = index >> 2;
	}
	index = ++counter;

	int tag = addr_space - offset - index;

	cache_size = (num_sets * block_size * assoc) / 1024;
	num_blocks = num_sets * assoc;

	printf("%dKB %d-way associative cache:\n", cache_size, assoc);
	printf("\tBlock size = %d bytes\n", block_size);
	printf("\tNumber of [sets,blocks] = [%d,%d]\n", num_sets, num_blocks);
	printf("\tExtra space for tag storage = ?? bytes (1.95%)\n");
	printf("\tBits for [tag,index,offset] = [%d,%d,%d]\n", tag, index, offset);
}

int main(int argc, char *argv[]) 
{
	GetFlags(argc, argv);	
	PrintHeader();
	return 0;
}
