#ifndef __UNIX_X86_64_LINUX_DIC_UTILS_H__
#define __UNIX_X86_64_LINUX_DIC_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

/**************************************************/
int unix_x86_64_linux_mkdir(const char* path);
int unix_x86_64_linux_rmdir(const char *path);
int unix_x86_64_linux_count_file_in_dir(const char* path);
int unix_x86_64_linux_count_dir_in_dir(const char* path);
char* unix_x86_64_linux_list_file_in_dir(const char* path);
char* unix_x86_64_linux_list_dir_in_dir(const char* path);
/**************************************************/


int unix_x86_64_linux_mkdir(const char* path)
{
	/*
	 *	@todo: create a relative directory
	 *	return -1 if occur an error
	 */
	return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int unix_x86_64_linux_rmdir(const char *path)
{
	size_t path_len;
	char *full_path;
	DIR *dir;
	struct stat stat_path, stat_entry;
	struct dirent *entry;

	stat(path, &stat_path);
	if ((dir = opendir(path)) == NULL)
		return -1;

	path_len = strlen(path);

	while ((entry = readdir(dir)) != NULL) {

		// skip entries "." and ".."
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		// determinate a full path of an entry
		full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
		strcpy(full_path, path);
		strcat(full_path, "/");
		strcat(full_path, entry->d_name);

		// stat for the entry
		stat(full_path, &stat_entry);

		// recursively remove a nested directory
		if (S_ISDIR(stat_entry.st_mode) != 0) {
			unix_x86_64_linux_rmdir(full_path);
			continue;
		}
		unlink(full_path);
	}

	int status = rmdir(path);
	closedir(dir);
	return status;
}

int unix_x86_64_linux_count_file_in_dir(const char* path)
{
	/*
	 *	@todo: count how many regular file in directory
	 */
	int file_count = 0;
	DIR * dirp;
	struct dirent* entry;

	dirp = opendir(path); /* There should be error handling after this */
	if (dirp == 0) return file_count;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (entry->d_type == DT_REG)
			file_count++;
	}
	closedir(dirp);
	return file_count;
}

int unix_x86_64_linux_count_dir_in_dir(const char* path)
{
	/*
	 *	@todo: count how many sub directory in directory
	 */
	int dir_count = 0;
	DIR * dirp;
	struct dirent* entry;

	dirp = opendir(path); /* There should be error handling after this */
	if (dirp == 0) return dir_count;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (entry->d_type == DT_DIR)
			dir_count++;
	}
	closedir(dirp);
	return dir_count - 2;
}

char* unix_x86_64_linux_list_file_in_dir(const char* path)
{
	/*
	 *	@todo: list all regular file in directory separate by '\n'
	 */
	char* buffer = (char*)malloc(1024*1024);

	DIR * dirp;
	struct dirent* entry;

	dirp = opendir(path);
	if (dirp == NULL) return NULL;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (entry->d_type == DT_REG)
			sprintf(buffer, "%s%s\n", buffer, entry->d_name);
	}
	closedir(dirp);

	return buffer;
}
char* unix_x86_64_linux_list_dir_in_dir(const char* path)
{
	/*
	 *	@todo: list all directory in directory separate by '\n'
	 */
	char* buffer = (char*)malloc(1024*1024);

	DIR * dirp;
	struct dirent* entry;

	dirp = opendir(path);
	if (dirp == NULL) return NULL;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (entry->d_type == DT_DIR) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, ".."))
				sprintf(buffer, "%s%s\n", buffer, entry->d_name);
		}
	}
	closedir(dirp);

	return buffer;
}

#endif // __UNIX_X86_64_LINUX_DIC_UTILS_H__