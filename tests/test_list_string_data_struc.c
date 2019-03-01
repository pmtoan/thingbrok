#include "../modules/data_structure/list_string.h"

int main(int argc, char const *argv[])
{
	LIST_STRING l = list_string_parse(argv[1], "&");
	list_string_print(l);
	list_string_free(&l);
	return 0;
}