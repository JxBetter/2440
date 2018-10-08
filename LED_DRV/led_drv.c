#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#define LED_MAJOR 0
#define LED_MINOR 0

static dev_t dev;
static struct class *led_class;
static unsigned int *gpfcon = NULL;
static unsigned int *gpfdata = NULL;

struct led_dev {
	struct cdev led_cdev;
	int flag; // 1为亮，0为灭
};

static struct led_dev led;

static int led_drv_open(struct inode *indoe, struct file *filp)
{
	/* 初始化LED引脚 */
	*gpfcon &= ~( (3<<8) | (3<<10) | (3<<12) );
	*gpfcon |= ( (1<<8) | (1<<10) | (1<<12) );
	printk("led open\n\r");

	return 0;
}


static ssize_t led_drv_write(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	int err;
	/* 用copy_from_user，取得用户空间传来的值，根据值来点灯 */
	char choose;
	err = copy_from_user(&choose, buff, 1);
	if (err)
	{
		return -1;
	}
	if (1 == choose)
	{
		*gpfdata &= ~((1 << 4) | (1 << 5) | (1 << 6));
		led.flag = 1;
		printk("led on\n\r");
	}
	else
	{
		*gpfdata |= ((1 << 4) | (1 << 5) | (1 << 6));
		led.flag = 0;
		printk("led off\n\r");
	}

	return 0;
}


static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_drv_open,
	.write = led_drv_write,
};


int led_drv_init(void)
{
	int res;
	/* io虚拟地址映射 */
	gpfcon = ioremap(0x56000050, 16);
	gpfdata = ioremap(0x56000054, 8);

	/* 分配设备号 */
	if (LED_MAJOR)
	{
		dev = MKDEV(LED_MAJOR, LED_MINOR);
		res = register_chrdev_region(dev, 1, "led");
	}
	else
	{
		res = alloc_chrdev_region(&dev, LED_MINOR, 1, "led");
	}
	if (res<0)
	{
		printk("get dev failed\n");
		return -1;
	}

	/* 注册字符设备 */
	cdev_init(&led.led_cdev, &led_fops);
	led.led_cdev.owner = THIS_MODULE;

	/* 创建设备类 */
	led_class = class_create(THIS_MODULE, "led_class");

	/* 创建设备文件 */
	device_create(led_class, NULL, dev, NULL, "led_dev");
	printk("led_drv init success\n\r");
	return 0;
}


void led_drv_exit(void)
{
	/* 销毁设备文件 */
	device_destroy(led_class, dev);

	/* 销毁设备类 */
	class_destroy(led_class);

	/* 删除字符设备 */
	cdev_del(&led.led_cdev);

	/* 释放设备号 */
	unregister_chrdev_region(dev, 1);

	/* 释放虚拟地址 */
	iounmap(gpfcon);
	iounmap(gpfdata);

	printk("led_drv exit success\n\r");
}


module_init(led_drv_init);
module_exit(led_drv_exit);
