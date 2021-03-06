#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

/* Def GPIOs for LEDs */
static struct gpio leds[] = {
	{ 23, GPIOF_OUT_INIT_LOW, "LED 1" },
	{ 24, GPIOF_OUT_INIT_LOW, "LED 2" },
};

/* Def GPIOs for BUTTONS */
static struct gpio buttons[] = {
	{ 25, GPIOF_IN, "BUTTON 1" },
};

/* assigned IRQ numbers for the buttons */
static int button_irqs[] = { -1 };

static irqreturn_t button_isr(int irq, void *data)
{
	int value;

	value = gpio_get_value(leds[0].gpio);
	if (value != 0)
		value = 0;
	else
		value = 1;
	printk(KERN_ERR "Button pushed %d\n", value);
	gpio_set_value(leds[0].gpio, value);

	return IRQ_HANDLED;
}

static int __init gpiomode_init(void)
{
	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);

	// register LED gpios
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));
	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs for LEDs: %d\n", ret);
		return ret;
	}

	// tegister BUTTON gpios
	ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));
	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs for BUTTONs: %d\n", ret);
		gpio_free_array(leds, ARRAY_SIZE(leds));
		return ret;
	}

	printk(KERN_INFO "Current button1 value: %d\n", gpio_get_value(buttons[0].gpio));

	ret = gpio_to_irq(buttons[0].gpio);
	if(ret < 0) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		gpio_free_array(buttons, ARRAY_SIZE(leds));
		gpio_free_array(leds, ARRAY_SIZE(leds));
		return ret;
	}

	button_irqs[0] = ret;

	printk(KERN_INFO "Successfully requested BUTTON1 IRQ # %d\n", button_irqs[0]);

	ret = request_irq(button_irqs[0], button_isr, IRQF_TRIGGER_RISING, "gpiomod#button1", NULL);

	if(ret) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		gpio_free_array(buttons, ARRAY_SIZE(leds));
		gpio_free_array(leds, ARRAY_SIZE(leds));
		return ret;
	}

	return 0;
}

static void __exit gpiomode_exit(void)
{
	int i;
	printk(KERN_INFO "%s\n", __func__);

	// free irqs
	for (i = 0; i < ARRAY_SIZE(buttons); i++)
		free_irq(button_irqs[i], NULL);

	// turn all LEDs off
	for(i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_set_value(leds[i].gpio, 0);
	}

	// unregister
	gpio_free_array(leds, ARRAY_SIZE(leds));
	gpio_free_array(buttons, ARRAY_SIZE(buttons));
}

module_init(gpiomode_init);
module_exit(gpiomode_exit);

MODULE_LICENSE("GPL");

