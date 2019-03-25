#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/init.h"
#include "linux/types.h"
#include "linux/errno.h"
#include "linux/uaccess.h"
#include "linux/kdev_t.h"

MODULE_LICENSE("leo BSD/GPL");
static char buffer[1024]; //数据缓冲区
static int device_num = 0;

static int mydriver_open(struct inode *inode,struct file *file )
{
  printk(KERN_NOTICE"3Driver_Major : %d\n", MAJOR(inode->i_rdev));
	printk(KERN_NOTICE"3Driver_Minor : %d\n", MINOR(inode->i_rdev));
	try_module_get(THIS_MODULE);
	return 0;
}

//读函数
static ssize_t mydriver_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	if (copy_to_user(buf, buffer, sizeof(buffer))) //读缓冲
	{
		return -1;
	}
	return sizeof(buffer);
}

//写函数,将用户的输入字符串写入
static ssize_t mydriver_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	if (copy_from_user(buffer, buf, sizeof(buffer))) //写缓冲
	{
		return -1;
	}
	return sizeof(buffer);
}

//释放设备函数
static int mydriver_release(struct inode *inode, struct file* filp)
{
	module_put(THIS_MODULE);
	return 0;
}

static struct file_operations mydriver_fops = {
    .read = mydriver_read,
    .write = mydriver_write,
    .open = mydriver_open,
    .release = mydriver_release,
};

//注册设备函数
int init_module()
{
	int result;
	printk("Begin to init Char Device!"); //注册设备
	//向系统的字符登记表登记一个字符设备
	result = register_chrdev(100, "mydriver", &mydriver_fops);
	if (result < 0) {
		printk("mydriver: register failure\n");
		return -1;
	}
	else
	{
		printk("mydriver: register success!\n");
		device_num = result;
		return 0;
	}
}

//注销设备函数
void cleanup_module()
{
	printk("Unloading...\n");
	unregister_chrdev(device_num, "mydriver"); //注销设备
	printk("unregister success!\n");
}


//模块宏定义
MODULE_LICENSE("GPL");
