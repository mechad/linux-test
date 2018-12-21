/*
 * @file: omp.c
 * @author: Lin, Chao <chaochaox.lin@intel.com>
 * @create time: 2018-12-11 13:58:36
 * @last modified: 2018-12-11 13:58:36
 * @description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
 
//一个简单的耗时任务
double Sum0(double* data,long data_count);
 
int main()
{
    long data_count=1920*1080*4;
    double* data=new double[data_count];
    long i;
 
    //初始化测试数据 
    for (i=0;i<data_count;++i)
        data[i]=(double)(rand()*(1.0/RAND_MAX));
 
    const long test_count=20;//为了能够测量出代码执行的时间，让函数执行多次
    double sumresult=0;
    for( i=0; i<test_count; ++i ) 
    {
        sumresult+=Sum0(data,data_count);
    }
    printf ("  最后结果     = %10.4f  ",sumresult);
 
    delete [] data;
    return 0;
}
 
 
double Sum0(double* data,long data_count)
{
    double result=0;
    #pragma omp parallel for schedule(static) reduction(+: result)
    for (long i=0;i<data_count;++i)
    {
        data[i]=(double)sin(cos(data[i]));
        result+=data[i];
    }
    return  result;
}

