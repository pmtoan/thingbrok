#ifndef __UNIX_X86_64_LINUX_COMMAND_UTILS_H__
#define __UNIX_X86_64_LINUX_COMMAND_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../utilities/utils.h"

/*----------------------------------------------------------------*/
void unix_x86_64_linux_cmd_silent(const char* cmd);
char* unix_x86_64_linux_cmd(const char* cmd);
/*----------------------------------------------------------------*/

void unix_x86_64_linux_cmd_silent(const char* cmd)
{
    /*
     *  todo @unix_x86_64_linux_cmd_silent execute a command line and not get output
    */
    char* do_cmd = (char*)malloc(__SIZE_LARGE__);

    /* Execute command */
    sprintf(do_cmd, "%s > /dev/null", cmd);
    system(do_cmd);

    free(do_cmd);

    return;
}

char* unix_x86_64_linux_cmd(const char* cmd)
{
    /*
     *  todo @unix_x86_64_linux_cmd execute a command and return output
    */
    FILE* fp;
    fp = popen(cmd, "r");

    if (!fp)
        return "Command error!";

    char* rt = (char*)malloc(__SIZE_EXTRA__);
    fread(rt, __SIZE_EXTRA__, 1, fp);
    fclose(fp);
    return rt;
}



#endif // __UNIX_X86_64_LINUX_COMMAND_UTILS_H__