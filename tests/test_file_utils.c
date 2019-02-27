#include "../modules/utilities/utils_file.h"

int main(int agrc, char** argv)
{
	file_append_line(argv[1], argv[2]);
//	printf("%s", file_read_first_line("thingler.c"));
	return 0;
}