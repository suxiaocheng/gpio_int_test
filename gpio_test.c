/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/ipc_logging.h>
#include <asm/irq.h>
#include <linux/kthread.h>


MODULE_LICENSE("Dual BSD/GPL");

#define TEST_IRQ

int test_irqres;
struct timer_list mytimer;

void timer_tick(unsigned long data)
{
	static int val = 0;
	if(val != gpio_get_value(0x9)) {
		val = !val;//gpio_get_value(0x9);
		printk(KERN_EMERG "[suxch]timer tick %d\n", val);
	}
	mytimer.expires = jiffies+1*HZ;
	add_timer(&mytimer);
}

#ifdef TEST_IRQ
static irqreturn_t cyttsp4_irq(int irq, void *handle)
{
	printk(KERN_EMERG "[suxch]Receive interrupt\n");
	return IRQ_HANDLED;
}
#endif

static int int_test_probe(struct platform_device *pdev)
{
	int rc = 0;
	int test_irqres;
	
        printk(KERN_ERR"[suxch] %s start.\n", __func__);
	test_irqres = platform_get_irq_byname(pdev, "test_irq");
        if (test_irqres < 0) {
                test_irqres = -1;
                printk(KERN_ERR"[suxch]test irq not specified.\n");
        }	
#ifdef TEST_IRQ
	{
		unsigned long irq_flags;
		irq_flags = IRQ_TYPE_EDGE_RISING | IRQF_ONESHOT;
		printk(KERN_EMERG"[suxch]irq num is: 0x%x, flag: 0x%lx", test_irqres, irq_flags);
		rc = request_irq(test_irqres, cyttsp4_irq, irq_flags,
			"test_irq_9", &test_irqres);
		if (rc < 0) {
			test_irqres = -1;
			printk(KERN_EMERG "[suxch]Request irq error, rc %d\n", rc);
			//return -1;
		}
        }
#endif
	init_timer(&mytimer);
	mytimer.function=timer_tick;
	mytimer.data = (unsigned long)NULL;
	mytimer.expires = jiffies+1*HZ;
	add_timer(&mytimer);

	printk(KERN_EMERG "[suxch]exit\n");
	return 0;
}

int int_test_remove(struct platform_device *pdev)
{
	if (test_irqres != -1) {
		free_irq(test_irqres, &test_irqres);
		test_irqres = -1;
	}
	del_timer(&mytimer);
	return 0;
}

static struct of_device_id gpio_int_match_table[] = {
        { .compatible = "qcom,gpio_int_test"},
        {}
};

static struct platform_driver gpio_int_test_driver = {
        .probe          = int_test_probe,
        .remove         = int_test_remove,
        .driver         = {
                .name           = "int_test",
		.of_match_table = gpio_int_match_table,
        },
};

module_platform_driver(gpio_int_test_driver);
