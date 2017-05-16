#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "chardev_log.h"

static int dev0_major = 0;
static __u8* buf = NULL;
static __u32 buf_size = 20;

int chardev_init(const char* device_name, const struct file_operations* f_ops) {
    const int device_major = register_chrdev(0, device_name, f_ops);
    if(device_major < 0){
        CHARDEV_LOG_CRIT("Error! The register_chrdev() failed. Error code = %d", device_major);
    }

    CHARDEV_LOG_INFO("dev major = %d", device_major);

    return device_major;
}

void chardev_cleanup(int device_major, const char* device_name)
{
    unregister_chrdev(device_major, device_name);
}

int chardev_open(struct inode* inode, struct file* file){
    try_module_get(THIS_MODULE);
    buf = kmalloc(buf_size, GFP_KERNEL);
    memset(buf, '@', buf_size - 1);
    buf[buf_size - 1] = '$';
    return 0;
}

ssize_t chardev_read(struct file* file, char __user * usr_buf, size_t size, loff_t* offset){
    int ret = -1;
    int count = 15;
    if(*offset >= buf_size){
        return 0;
    }

    if(*offset + count > buf_size){
        count = buf_size - *offset;
    }

    if((ret = copy_to_user(usr_buf, buf + *offset, count))){
        CHARDEV_LOG_CRIT("%s", "copy_to_user() error!\n");
        CHARDEV_LOG_CRIT("ret = %d\n", ret);
        return 0;
    }

    *offset += count;

    CHARDEV_LOG_INFO("size   = %ld\n", size);
    CHARDEV_LOG_INFO("offset = %lld\n", *offset);

    return count;
}

ssize_t chardev_write(struct file* file, const char __user * buf, size_t size, loff_t* offset){
   return size;
}

int chardev_release(struct inode* inode, struct file* file){
    module_put(THIS_MODULE);
    kfree(buf);
    return 0;
}

static struct file_operations f_ops =
{
    .owner = THIS_MODULE,
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write
};

static int __init load(void){
    printk("init_module()\n");
    dev0_major = chardev_init("dev_0", &f_ops);
    return 0;
}

static void __exit unload(void){
    chardev_cleanup(dev0_major, "dev_0");
    printk("cleanup_module()\n");
}

module_init(load);
module_exit(unload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vadim Stupakov <vadim.stupakov@gmail.com>");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Linux kernel module for testing.");

