#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int jriver-device;
	char buffer[10000];
	
	int num_written
	
	if (jriver-device = open("/dev/jriver-device") < 0) return 1;
	
	printf("Now writing args...\n");
	for (int i=1; i<argc; i++)
	{
		if(num_written= write(jriver-device, argv[i], 10000, 0) < 0)
		{
			printf("Failed to write argument number %d. Exiting...\n", i);
			close(jriver-device);
			return;
		}
		printf("Successfully wrote argument number %d. Continuing on...\n", i);
	}
	
	printf("Successfully wrote all arguments.\n");
	printf("Now reading...\n");
	
	if (read(jriver-device, buffer, 10000, 0) < 0)
	{
			printf("Failed to read. Exiting...\n");
			close(jriver-device);
			return;
	
	}
	printf("%s/n", buffer);
	close(jriver-device);
	
	return;
}