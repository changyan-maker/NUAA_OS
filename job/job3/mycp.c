#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	mode_t mode = 0777;
	int fd1 = open(argv[1], O_RDONLY, mode);
	int fd2 = creat(argv[2],mode);
	char ch;
	int count = read(fd1, &ch, sizeof(ch));
	int count2;
	while(count)
	{
		count2 = write(fd2, &ch, sizeof(ch));
		count = read(fd1, &ch, sizeof(ch));
	}
	close(fd1);
	close(fd2);
	return 0;
}
