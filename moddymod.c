// Operating Systems (COP4600)
// Programming Assignment #2: Character Device Kernal Module
// Submission Date: 3/23/18
// Submission By: Brandon Cuevas, Jacquelyn Law, Lorraine Yerger
// File: moddymod.c
// Reference Used: http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024
#define DEVICE_NAME "moddymod"
#define CLASS_NAME "mod"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Jacquelyn Lorraine");
MODULE_DESCRIPTION("Programming Assignment #2: Character Device Kernal Module");
MODULE_VERSION("0.1");


static int majorNumber;
static char mainBuffer[BUFFER_SIZE]= {0};
static struct class *modClass = NULL;
static struct device *modDevice = NULL;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
 
static struct file_operations fops = {
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
	//.owner = THIS_MODULE
};

int init_module(void) {
	printk(KERN_INFO "moddymod: installing moddymod.\n");
	
	// Try to dynamically allocate a major number for the device
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0) {
		printk(KERN_ALERT "moddymod failed to register a major number \n");
		return majorNumber;
	}
	printk(KERN_INFO "moddymod: registered correctly with major number %d\n", majorNumber);

	// Register the device class
   	modClass = class_create(THIS_MODULE, CLASS_NAME);
   	if (IS_ERR(modClass))
	{                
		// Check for error and clean up if there is
      		unregister_chrdev(majorNumber, DEVICE_NAME);
      		printk(KERN_ALERT "Failed to register device class\n");
      		return PTR_ERR(modClass);          // Return pointer error
   	}
  	printk(KERN_INFO "moddymod: device class registered correctly\n");
 
   	// Register the device driver
   	modDevice = device_create(modClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(modDevice))
	{
		// Clean up if there is an error
		class_destroy(modClass);           
		// Repeated code but the alternative is goto statements
      		unregister_chrdev(majorNumber, DEVICE_NAME);
      		printk(KERN_ALERT "Failed to create the device\n");

		return PTR_ERR(modDevice);
	}

   	printk(KERN_INFO "moddymod: device class created correctly\n");

	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "moddymod: removing moddymod.\n");

	device_destroy(modClass, MKDEV(majorNumber, 0));     // remove the device
	class_unregister(modClass);                          // unregister the device class
	class_destroy(modClass);                             // remove the device class
   	unregister_chrdev(majorNumber, DEVICE_NAME);         // unregister the major number
   	printk(KERN_INFO "moddymod: goodbye from the LKM!\n");
}

// IMPLEMENTATION TBA
static int dev_open(struct inode *inodep, struct file *filep){
   //numberOpens++;
   //printk(KERN_INFO "moddymod: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

// IMPLEMENTATION TBA
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   return 0;
}

// IMPLEMENTATION TBA
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   return 0;
}

// IMPLEMENTATION TBA
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "moddymod: Device successfully closed\n");
   return 0;
}
