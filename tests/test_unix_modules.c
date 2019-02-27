#include "../modules/unix/x86_64-linux-gnu/unix_x86_64_linux_dir.h"

int main(int argc, char** argv)
{
	/* Test cases for unix directory utilities module*/
//	printf("%d\n", unix_x86_64_linux_mkdir(argv[1]));
//	printf("%d\n", unix_x86_64_linux_rmdir(argv[1]));
//	printf("%d\n", unix_x86_64_linux_count_file_in_dir(argv[1]));
//	printf("%s", unix_x86_64_linux_list_file_in_dir(argv[1]));
//	printf("%d\n", unix_x86_64_linux_count_dir_in_dir(argv[1]));
	printf("%s", unix_x86_64_linux_list_dir_in_dir(argv[1]));
	return 1;
}