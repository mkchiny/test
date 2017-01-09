#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *pcTest = "asdfasdfasdfasdfasdf";
	char acTemp[64];

	snprintf(acTemp, sizeof(acTemp), "echo %s > 1.txt", pcTest);

	system(acTemp);

	return 0;
}

