#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kobject.h>
#include <asm/uaccess.h>

static int num1;
static int num2;
static int result;
static char operation;

static struct proc_dir_entry *proc_file_1;
static struct proc_dir_entry *proc_file_2;
static struct proc_dir_entry *proc_result_file;
static struct proc_dir_entry *proc_operation_file;

int proc_file1_write (struct file *file, const char __user *buffer, unsigned long count, void *data)
{
    char *page;
    page = (char *) vmalloc(count);
    if (!page)
       return -ENOMEM;

    if (copy_from_user(page, buffer, count)) {
       vfree(page);
       return -EFAULT;
    }
    sscanf(page, "%d", &num1);
    return count;
}

int proc_file2_write (struct file *file, const char __user *buffer, unsigned long count, void *data)
{
    char *page;
    page = (char *) vmalloc(count);
    if (!page)
       return -ENOMEM;   

    if (copy_from_user(page, buffer, count)) {
       vfree(page);
       return -EFAULT;
    }
    sscanf(page, "%d", &num2);
    return count;
}

int proc_file_operation_write (struct file *file, const char __user *buffer, unsigned long count, void *data)
{
    sscanf(buffer, "%c", &operation);
    return count;
}

int proc_file_result_read (char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int len;
    if (operation == '+')
	len = sprintf(buffer, "%d\n", num1 + num2);
    if (operation == '-')
	len = sprintf(buffer, "%d\n", num1 - num2);
    if (operation == '*')
	len = sprintf(buffer, "%d\n", num1 * num2);
    if (operation == '/')
	len = sprintf(buffer, "%d\n", num1 / num2);
    return len;
}

static ssize_t sysfs_file1_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &num1);
    return count;
}

static ssize_t sysfs_file2_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &num2);
    return count;
}

static ssize_t sysfs_operation_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%c", &operation);
    return count;
}

static ssize_t sysfs_result_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    int len;
    if (operation == '+')
	len = sprintf(buffer, "%d\n", num1 + num2);
    if (operation == '-')
	len = sprintf(buffer, "%d\n", num1 - num2);
    if (operation == '*')
	len = sprintf(buffer, "%d\n", num1 * num2);
    if (operation == '/')
	len = sprintf(buffer, "%d\n", num1 / num2);
    return len;
}

static struct kobj_attribute num1_attribute = __ATTR(num1, 0666, NULL, sysfs_file1_store);
static struct kobj_attribute num2_attribute = __ATTR(num2, 0666, NULL, sysfs_file2_store);
static struct kobj_attribute operation_attribute = __ATTR(operation, 0666, NULL, sysfs_operation_store);
static struct kobj_attribute result_attribute = __ATTR(num1, 0666, sysfs_result_show, NULL);

static struct attribute *attrs[] = {
	&num1_attribute,
	&num2_attribute,
	&operation_attribute,
	&result_attribute,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

int init_module()
{
    proc_file_1 = create_proc_entry("proc_num1", 0666, NULL);
    proc_file_2 = create_proc_entry("proc_num2", 0666, NULL);
    proc_result_file = create_proc_entry("proc_result", 0666, NULL);
    proc_operation_file = create_proc_entry("proc_operation", 0666, NULL);
    if (proc_file_1 == NULL || proc_file_2 == NULL || proc_result_file == NULL || proc_operation_file == NULL)
	return -ENOMEM;
    proc_file_1->write_proc = proc_file1_write;
    proc_file_2->write_proc = proc_file2_write;
    proc_result_file->read_proc = proc_file_result_read;
    proc_operation_file->write_proc = proc_file_operation_write;
    //return 0;

    int retval;
    example_kobj = kobject_create_and_add("calc", kernel_kobj);
    if (!example_kobj)
	return -ENOMEM;
    retval = sysfs_create_group(example_kobj, &attr_group);
    if (retval)
	kobject_put(example_kobj);
    return retval;
}

void cleanup_module()
{
    remove_proc_entry("proc_num1", NULL);
    remove_proc_entry("proc_num2", NULL);
    remove_proc_entry("proc_result", NULL);
    remove_proc_entry("proc_operation", NULL);

    kobject_put(example_kobj);
}

