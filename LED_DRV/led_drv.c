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
static volatile unsigned int *gpfcon = NULL;
static volatile unsigned int *gpfdata = NULL;

struct led_dev {
	struct cdev led_cdev;
	int flag;
}

static struct led_dev led;

static int led_drv_open(struct inode *indoe, struct file *filp)
{
	/* 初始化LED引脚 */

	return 0;
}


static ssize_t led_drv_write(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	/* 用copy_from_user，取得用户空间传来的值，根据值来点灯 */

	return 0;
}


static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_drv_open,
	.write = led_drv_write,
}


int led_drv_init(void)
{
	/* io虚拟地址映射 */

	/* 分配设备号 */

	/* 注册字符设备 */

	/* 创建设备类 */

	/* 创建设备文件 */
	return 0;
}


void led_drv_exit(void)
{
	/* 销毁设备文件 */

	/* 销毁设备类 */

	/* 删除字符设备 */

	/* 释放设备号 */

	/* 释放虚拟地址 */
}


module_init(led_drv_init);
module_exit(led_drv_exit);