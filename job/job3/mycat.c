#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	mode_t mode = 0777;
	fd = open(argv[1],O_RDONLY,mode);
	char ch;
	int count = read(fd, &ch, sizeof(ch));
	while(count)
	{
		printf("%c", ch);
		count = read(fd, &ch, sizeof(ch));
	}
	close(fd);
	return 0;
}
