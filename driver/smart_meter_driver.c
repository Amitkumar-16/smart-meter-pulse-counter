#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "smart_meter"
#define CLASS_NAME "smart_meter_class"

static dev_t device_number;
static struct cdev smart_meter_cdev;
static struct class *smart_meter_class;

static int pulse_count = 0;
static DEFINE_MUTEX(pulse_lock);

static int smart_meter_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Smart Meter: device opened\n");
    return 0;
}

static int smart_meter_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Smart Meter: device closed\n");
    return 0;
}

static ssize_t smart_meter_read(struct file *file,
                                char __user *buffer,
                                size_t length,
                                loff_t *offset)
{
    char message[64];
    int message_length;

    mutex_lock(&pulse_lock);

    message_length = snprintf(message, sizeof(message),
                              "%d\n", pulse_count);

    mutex_unlock(&pulse_lock);

    if (*offset >= message_length)
        return 0;

    if (length < message_length)
        return -EINVAL;

    if (copy_to_user(buffer, message, message_length))
        return -EFAULT;

    *offset += message_length;

    return message_length;
}

static ssize_t smart_meter_write(struct file *file,
                                 const char __user *buffer,
                                 size_t length,
                                 loff_t *offset)
{
    char input;

    if (length < 1)
        return -EINVAL;

    if (copy_from_user(&input, buffer, 1))
        return -EFAULT;

    if (input != '1')
    {
        printk(KERN_INFO
               "Smart Meter: invalid pulse input\n");

        return -EINVAL;
    }

    mutex_lock(&pulse_lock);

    pulse_count++;

    mutex_unlock(&pulse_lock);

    printk(KERN_INFO
           "Smart Meter: pulse received, count = %d\n",
           pulse_count);

    return length;
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = smart_meter_open,
    .read = smart_meter_read,
    .write = smart_meter_write,
    .release = smart_meter_release
};

static int __init smart_meter_init(void)
{
    int result;

    result = alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME);

    if (result < 0)
        return result;

    cdev_init(&smart_meter_cdev, &fops);

    result = cdev_add(&smart_meter_cdev, device_number, 1);

    if (result < 0)
    {
        unregister_chrdev_region(device_number, 1);
        return result;
    }

    smart_meter_class = class_create(CLASS_NAME);

    if (IS_ERR(smart_meter_class))
    {
        cdev_del(&smart_meter_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(smart_meter_class);
    }

    if (IS_ERR(device_create(smart_meter_class,
                             NULL,
                             device_number,
                             NULL,
                             DEVICE_NAME)))
    {
        class_destroy(smart_meter_class);
        cdev_del(&smart_meter_cdev);
        unregister_chrdev_region(device_number, 1);
        return -1;
    }

    printk(KERN_INFO "Smart Meter: driver loaded\n");

    return 0;
}

static void __exit smart_meter_exit(void)
{
    device_destroy(smart_meter_class, device_number);
    class_destroy(smart_meter_class);
    cdev_del(&smart_meter_cdev);
    unregister_chrdev_region(device_number, 1);

    printk(KERN_INFO "Smart Meter: driver unloaded\n");
}

module_init(smart_meter_init);
module_exit(smart_meter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smart Meter Project");
MODULE_DESCRIPTION("Smart Meter Pulse Counter Character Device Driver");
