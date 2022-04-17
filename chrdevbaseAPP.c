#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
static char usr_data[] = "usr data!";
/*
*argc:应用程序参数个数
*argv[]:具体的参数内容，字符串形式
*./chrdevbaseAPP <filename>
*/
int main(int argc, char *argv[])
{
    int ret = 0;
    int fd = 0;
    char *filename;
    char readbuf[100],writebuf[100];

    if(argc!=3){
        printf("Error Usage!!!\r\n");
        return -1;       
    }
    filename = argv[1];

    /*打开驱动文件*/
    fd = open(filename,O_RDWR);
    if(fd<0)
    {
        printf("Can't open file %s\r\n",filename);
        return -1;
    }
    if(atoi(argv[2])==1)    /*从驱动文件读取数据*/
    {
        ret = read(fd,readbuf,30);
        if(ret<0)
        {
            printf("read file %s failed.\r\n",filename);
        }
        else
        {
            printf("read file data:%s success!\r\n",readbuf);
        }
    }
    if(atoi(argv[2])==2)    /*向驱动文件写数据*/
    {
        memcpy(writebuf,usr_data,sizeof(usr_data));
        ret =  write(fd,writebuf,50);
        if(ret<0)
        {
            printf("write file %s failed.\r\n",filename);
        }
    }
    /*关闭设备*/
    ret = close(fd);
    if(ret<0)
    {
        printf("close file %s failed.\r\n",filename);
        return -1;
    }
    return 0;
}

