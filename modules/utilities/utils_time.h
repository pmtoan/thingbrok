#ifndef __UTILITIES_TIME_H__
#define __UTILITIES_TIME_H__

#include "../core/types.h"
#include "utils_string.h"

/*----------------------------------------------------------------*/
char* time_get_timestamp();
/*----------------------------------------------------------------*/

char* time_get_timestamp()
{
    /*
     *	todo @time_get_timestamp get current date time and build up a ctime string format
     *	ctime string format: "Sat Jan  5 17:39:45 2019"
    */
	time_t ltime; /* calendar time */
    ltime = time(NULL); /* get current cal time */
    return string_replace(asctime(localtime(&ltime)), "\n", "");
}

#endif	// __UTILITIES_TIME_H__