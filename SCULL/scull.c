#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>

#define SCULL_MAJOR 0
#define SCULL_MINOR 0

static dev_t dev;
static struct class *scull_class;

static int scull_open(struct inode *inode, struct file *filp);
static ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t scull_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);



struct scull_dev{
	struct cdev *cdev;
	int data;
};

static struct scull_dev a_scull_dev = {
	.data = 0,
};

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = scull_open,
	.read = scull_read,
	.write = scull_write,
};


static int scull_open(struct inode *inode, struct file *filp)
{
	printk("scull open\n\r");
	(int *)(filp -> private_data) = &(a_scull_dev.data);
	return 0;
}


static ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	printk("scull read\n\r");
	copy_to_user(buff, filp -> private_data, 4);
	return 0;
}


static ssize_t scull_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	printk("scull write\n\r");
	copy_from_user(filp -> private_data, buff, 4);
	return 0;
}


int scull_init(void)
{
	int res, err;
	/* 分配设备号 */
	if (SCULL_MAJOR)
	{
		dev = MKDEV(SCULL_MAJOR, SCULL_MINOR);
		res = register_chrdev_region(dev, 1, "scull"); //设备编号，设备数量，proc/devices/scull
	}
	else
	{
		res = alloc_chrdev_region(&dev, SCULL_MINOR, 1, "scull");
	}
	if (res<0)
	{
		printk("get major failed\n\r");
		return -1;
	}

	/* 注册字符设备驱动 */
	cdev_init(a_scull_dev.cdev, &fops);
	a_scull_dev.cdev -> owner = THIS_MODULE;
	err = cdev_add(a_scull_dev.cdev, dev, 1);
	if (err)
	{
		return err;
	}


	/* 生成设备类 */
	scull_class = class_create(THIS_MODULE, "scull_class");

	/* 生成设备文件 */
	device_create(scull_class, NULL, dev, NULL, "scull_dev_file");
	return 0;
}


void scull_exit(void)
{
	/* 销毁设备文件 */
	device_destroy(scull_class, dev);

	/* 销毁设备类 */
	class_destroy(scull_class);

	/* 销毁设备驱动 */
	cdev_del(a_scull_dev.cdev);

	/* 释放主设备号 */
	unregister_chrdev_region(dev, 1);
}


module_init(scull_init);
module_exit(scull_exit);