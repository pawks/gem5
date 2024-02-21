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

module_init(drv_init);
module_exit(drv_exit);
