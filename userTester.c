#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
 

static char *buffer;
    

int main()
{
	buffer = malloc(sizeof(char)*1024);
	int readerInt;
	int writerInt;
	char str[1024];
	size_t numBytes;
	int file;
	char c;
	int offset;
	int whence;
	int flag = 1;
	
	//FILE *ptr;
	file = open("/dev/part2Device", O_RDWR);

	while (flag == 1) {
		printf("\n");
		printf("Choose an option: \n");
		printf("Option 'r' - Read from file \n");
		printf("Option 'w' - Read from file \n");
		printf("Option 's' - Read from file \n");
		printf("Option 'e' - Read from file \n");
		scanf(" %c", &c);
	
		printf("You entered: %c \n", c);
	
		
		//printf("TEST: %c \n", c);
		if(c == 'r')
		{
			//open system call instead of fopen
			//ptr = fopen("/dev/part2Device", "r");
			//file = open("/dev/part2Device", O_RDONLY);

			if(file == -1)
			{
				printf("Error opening file");
				flag = 0;
				exit(1);
			}

			printf("Enter the number of bytes you want to read: \n");
			scanf("%zu", &numBytes);

			readerInt = read(file, buffer, numBytes);
			printf("You read: %zu bytes.\n", numBytes);
			printf("Data: %s \n", buffer);

		
			//read numBytes from the part2Device file using 'read' call 
			//file description = what open returns
			//buffer = maintain as golbal var / array of characters
			//read(file descriptor, buffer, numBytes);
		
			
		
		}
		else if(c == 'w')
		{
		
			//file = open("/dev/part2Device", O_WRONLY);

			if(file == -1)
			{
				printf("Error opening file");
				flag = 0;
				exit(1);
				
			}

			//ask for wtring to w
			printf("Enter data you want to write: \n");
			scanf("%s", buffer);
		
			// write that string to part2Device
			writerInt = write(file, buffer, strlen(buffer));

			
			printf("Number of bytes written: %d \n", writerInt);
			//
		}
		else if(c == 's')
		{
			//file = open("/dev/part2Device", O_RDWR);
			

			printf("Enter an offset value: \n");
			scanf("%d", &offset);

			printf("Enter a value for whence: \n"); 
			scanf("%d", &whence);
			
			if(whence == 0)
			{
				printf("Chose SEEK_SET \n");
				lseek(file, offset, SEEK_SET);
			}
			else if(whence == 1)
			{
				printf("Chose SEEK_CUR \n");
				lseek(file, offset, SEEK_CUR);
			}
			else if(whence == 2)
			{
				printf("Chose SEEK_END \n");
				lseek(file, offset, SEEK_END);
			}
			else
			{
				printf("SEEK choice not valid \n");
			}
			
		}
		else if(c == 'e')
		{
			printf("Quitting program!\n");
			flag = 0;
			exit(1);
			
		}
		else
		{
			printf("Error choice input\n");
		}

		//change to switch statement if have time
		//switch(c) {
		//	case "r":
		//	break;
		//}
		//close(file);
		
	}
	close(file);
	
	//printf("Complete! \n");
	return 0;
	
}
