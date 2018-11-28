/*
 * @file: clockgettimeday.c
 * @create time: 2018-09-25 11:23:09
 * @last modified: 2018-09-25 11:23:09
 * @description:
 */
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
int main()
{	
	time_t c_start, t_start, c_end, t_end; 	
	c_start = clock();    
	//!< 单位为ms	
	t_start = time(NULL);  //!< 单位为s	
    sleep(2);
	c_end   = clock();	
	t_end	= time(NULL); 	//!<difftime（time_t, time_t）返回两个time_t变量间的时间间隔，即时间差	
	printf("The pause used %f ms by clock()\n",difftime(c_end,c_start)); 	
	printf("The pause used %f s by time()\n",difftime(t_end,t_start));	
	return 0;
}

