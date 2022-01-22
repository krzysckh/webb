#include <stdio.h>
#include <unistd.h>

int main (void) {
	int ret = execl("/usr/bin/convert", "convert", "/tmp/_im_webb_tmp","/tmp/_im_webb_ppm.ppm", (char*) NULL);
	printf("Ret = %d\n", ret);

	return ret;
}

