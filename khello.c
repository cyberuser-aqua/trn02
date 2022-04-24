#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static int khello_init(void) {
    printk("%s\n", "Loaded khello");
    return 0;
}

static void khello_exit(void) {
    printk("%s\n", "Unloaded khello");
    return;
}

module_init(khello_init);
module_exit(khello_exit);
MODULE_LICENSE("asdf");
