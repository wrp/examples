/* Get the size of a device.  Pass the device as first arg.  eg '/dev/sda1' */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>

int
main(int argc, char **argv)
{
	int fd;
	unsigned long numblocks=0;

	(void) argc;
	fd = open(argv[1], O_RDONLY);
	if( fd == -1 ) {
		perror(argv[1]);
		exit(1);
	}
	if( ioctl(fd, BLKGETSIZE, &numblocks)) {
		perror(argv[1]);
		exit(1);
	}
	close(fd);
	printf("%lu blocks\t", numblocks);
	printf("%.3f GiB\n", (double)numblocks * 512.0 / (1024 * 1024 * 1024));
}
