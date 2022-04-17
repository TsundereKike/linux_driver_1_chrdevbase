#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#define CHRDEVBASE_MAJOR    200     /*主设备号*/
#define CHRDEVBASE_NAME     "chrdevbase"/*名字*/

static char readbuf[100];/*读缓冲区*/
static char writebuf[100];/*写缓冲区*/
static char kerneldata[] = {"kernel data!!!"};
/*
*@brief:    打开设备
*@param:    inode:传递给驱动的inode
*           filp: 设备文件，file结构体有个叫做private_data的成员变量，
*                 一般再open的时候简爱嗯private_data指向设备结构体
*@return    0:成功;     其他：失败 
*/
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    printk("chrdevbase_open\r\n");
    return 0;
}
/*
*@brief:    关闭/释放设备
*@param:    filp:要关闭的设备文件(文件描述符)
*@return:   0:成功;     其他:失败
*/
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    //printk("chrdevbase_release\r\n");
    return 0;
}
/*
*@brief:    从设备读数据
*@param：   filp:要打开的文件(文件描述符)
*           buf:返回用户空间的数据缓冲区
*           count:要读取的数据长度
*           ppos:相对于文件首地址的偏移
*@return:   读取的字节数，如果为负值，比哦啊是读取失败
*/
static ssize_t chrdevbase_read(struct file *filp,char __user *buf,
                                size_t count, loff_t *ppos)
{
    int retval = 0;
    /*向用户空间发送数据*/
    memcpy(readbuf,kerneldata,sizeof(kerneldata));
    retval = copy_to_user(buf,readbuf,count);
    if(retval==0)
    {
        printk("kernel senddata ok!\r\n");
    }
    else
    {
        printk("kernel senddata failed!\r\n");
    }
    //printk("chrdevbase_read\r\n");
    return 0;
}
/*
*@brief:    向设备写数据
*@param:    filp:设备文件(表示打开的文件描述符)
*           buf:要给设备写入的数据
*           count:要写入的数据长度
*           ppos:相对于文件首地址的偏移
*@return：  写入的字节数，如果为负值，表示写入失败
*/
static ssize_t chrdevbase_write(struct file *filp,const char __user *buf,
                                    size_t count, loff_t *ppos)
{
    int retval = 0;
    /*接收用户空间传递给内核的数据并且打印出来*/
    retval = copy_from_user(writebuf,buf,count);
    if(retval==0)
    {
        printk("kernel receive data:%s\r\n",writebuf);
    }
    else
    {
        printk("kernel receive data failed!\r\n");
    }
    
    //printk("chrdevbase_write\r\n");
    return 0;
}
/*
* 字符设备 操作集合结构体
*/
static struct file_operations chrdevbase_fops={
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .release = chrdevbase_release,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
};
/*
*@brief:    驱动入口函数
*@param：   NULL
*@return:   0:成功;     其他失败
*/
static int __init chrdevbase_init(void)
{
    int ret = 0;
    /*注册字母设备*/
    ret = register_chrdev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME,
                        &chrdevbase_fops);
    if(ret<0)
    {
        printk("chrdevbase init failed!!!\r\n");
    }
    printk("init chrdevbase\r\n");
    return 0;
}
/*
*@brief:    驱动出口函数
*@param:    NULL
*@return:   NULL
*/
static void __exit chrdevbase_exit(void)
{
    /*注销字符设备*/
    unregister_chrdev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME);
    printk("exit chrdevbase\r\n");
}
/*模块入口与出口指定*/
module_init(chrdevbase_init);/*入口*/
module_exit(chrdevbase_exit);/*出口*/
/*作者及License信息*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tanminghang");