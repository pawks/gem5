/*
     Copyright (c) 2024 University of Toronto All Rights Reserved. See license filefor more details.
     Created On:  Mon Feb 12, 2024 10:34:37 
     Author(s): 
     - Pawan Kumar Sanjaya<pawan.sanjaya@mail.utoronto.ca> gitlab: @pawks github: @pawks
 */
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<asm/uaccess.h>
#define AUTHOR "Pawan"
#define DEVICE_NAME "PPUv1"
MOUDLE_LICENCE("GPL");
MODULE_AUTHOR(AUTOHOR);

int majornum;
static int drv_open(struct inone *inode, struct file *fp);
static ssize_t drv_read(struct file *fp, uint64_t buf, loff_t *off_set);
static ssize_t drv_write(struct file *fp, const uint64_t buf, loff_t *off_set);
static int drv_rel(struct inode *inode, struct file *fp);

static struct file_operations fileops = {
    .read=drv_read,
    .write=drv_write,
    .open=drv_open,
    .release=drv_rel,
};

static struct platform_device my_platform_device = {
    .name=DEVICE_NAME,
    .id = -1,
}

statis int __init drv_init(void)
{
    majornum = platform_device_register(&my_platform_device);
    if (ret < 0) {
        pr_err("my_subsystem: Platform device registration failed\n");
        return ret;
    }
    pr_info("my_subsystem: Platform device registered successfully\n");
    return 0;
}
static void __exit drv_exit(void){
    platform_device_unregister(&my_platform_device);
}

static int drv_open(struct inone *inode, struct file *fp){
    printk(KERN_INFO "PPU Device Open.")
        return 0;
}
static ssize_t drv_read(struct file *fp, uint64_t buf, loff_t *off_set);
static ssize_t drv_write(struct file *fp, const uint64_t buf, loff_t *off_set);
static int drv_rel(struct inode *inode, struct file *fp);

module_init(drv_init);
module_exit(drv_exit);
