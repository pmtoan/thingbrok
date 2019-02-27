#ifndef __UNIX_X86_64_LINUX_FILE_UTILS_H__
#define __UNIX_X86_64_LINUX_FILE_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include "../../utilities/utils.h"

/*----------------------------------------------------------------*/
char* unix_x86_64_linux_get_stat_mode(const char* path);
int unix_x86_64_linux_get_stat_type(const char* path);
char* unix_x86_64_linux_get_stat_user(const char* path);
char* unix_x86_64_linux_get_stat_group(const char* path);
int unix_x86_64_linux_change_mode(const char* path, const char* new_mode);
int unix_x86_64_linux_change_owner(const char* path, const char* new_usr, const char* new_grp);
/*----------------------------------------------------------------*/



char* unix_x86_64_linux_get_stat_mode(const char* path)
{
    /*
     *  todo @unix_x86_64_linux_get_stat_mode get a @path mode/ permission
     *  return permission string format if success, return "" on failure
     *  permission string format: "-rwxr-----"
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    char* permission = (char*)malloc(11);

    /* For file permission */
    sprintf(permission, "%s", (S_ISDIR(sb.st_mode) ? "d" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IRUSR) ? "r" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IWUSR) ? "w" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IXUSR) ? "x" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IRGRP) ? "r" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IWGRP) ? "w" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IXGRP) ? "x" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IROTH) ? "r" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IWOTH) ? "w" : "-"));
    sprintf(permission, "%s%s", permission, ((sb.st_mode & S_IXOTH) ? "x" : "-"));
    return permission;
}

int unix_x86_64_linux_get_stat_type(const char* path)
{
    /*
     *  todo @unix_x86_64_linux_get_stat_type get type of @path
     *  return -1 on failure
     *  return 0 if @path is a directory, otherwise, @path is a file
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return -1;
    /* For file type */
    switch (sb.st_mode & S_IFMT) {
        case S_IFDIR:
           return 0;
        default:
            return 1;
    }
}

char* unix_x86_64_linux_get_stat_user(const char* path)
{
    /*
     *  todo @unix_x86_64_linux_get_stat_user get username of @path owner
     *  return "" on failure
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    /* For user and group */
    struct passwd *pws;
    pws = getpwuid(sb.st_uid);
    return pws->pw_name;
}

char* unix_x86_64_linux_get_stat_group(const char* path)
{
    /*
     *  todo @unix_x86_64_linux_get_stat_user get group name of @path owner
     *  return "" on failure
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    /* For user and group */
    struct group *g;
    g = getgrgid(sb.st_gid);
    return g->gr_name;
}

char* UNIX_X86_64_LINUX_stat_lmt(const char* path)
{
    /*
     *  todo @UNIX_X86_64_LINUX_stat_lmt get last modification timestamp of @path
     *  return "" on failure
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    return string_replace(ctime(&sb.st_ctime), "\n", "");
}

char* UNIX_X86_64_LINUX_stat_lat(const char* path)
{
    /*
     *  todo @UNIX_X86_64_LINUX_stat_lat get last access timestamp of @path
     *  return "" on failure
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    return string_replace(ctime(&sb.st_atime), "\n", "");
}

char* UNIX_X86_64_LINUX_stat_lst(const char* path)
{
    /*
     *  todo @UNIX_X86_64_LINUX_stat_lst get last status change timestamp of @path
     *  return "" on failure
    */
    struct stat sb;
    if ((stat(path, &sb) == -1))
        return "";
    return string_replace(ctime(&sb.st_ctime), "\n", "");
}

int unix_x86_64_linux_change_mode(const char* path, const char* new_mode)
{
    /*
    *  todo @unix_x86_64_linux_change_mode change file permission
    *  @new_mode format as: -rwxr-----
    */
    mode_t mode = S_IRUSR;
    if (new_mode[2] == 'w') mode = mode | S_IWUSR;
    if (new_mode[3] == 'x') mode = mode | S_IXUSR;
    if (new_mode[4] == 'r') mode = mode | S_IRGRP;
    if (new_mode[5] == 'w') mode = mode | S_IWGRP;
    if (new_mode[6] == 'x') mode = mode | S_IXGRP;
    if (new_mode[7] == 'r') mode = mode | S_IROTH;
    if (new_mode[8] == 'w') mode = mode | S_IWOTH;
    if (new_mode[9] == 'x') mode = mode | S_IXOTH;
    return chmod(path, mode);
}

int unix_x86_64_linux_change_owner(const char* path, const char* new_usr, const char* new_grp)
{
    /*
    * todo @unix_x86_64_linux_change_owner change owner and group of a file
    */
    struct group* grp = getgrnam(new_grp);
    struct passwd* pwd = getpwnam(new_usr);
    return chown(path, pwd->pw_uid, grp->gr_gid);
}

#endif  // __UNIX_X86_64_LINUX_FILE_UTILS_H__