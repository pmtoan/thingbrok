#include "../modules/utilities/utils_file.h"

int main(int agrc, char** argv)
{
	// file_append_line(argv[1], argv[2]);
	// printf("%s", file_read_first_line("thingler.c"));
	// printf("%ld\n", file_count_line(argv[1]));
	list_string_print(file_read_text_last(argv[1], atoi(argv[2])));
	return 0;
}