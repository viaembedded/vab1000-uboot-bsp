#include <common.h>
#include <linux/string.h>
#include <asm/io.h>
#include <asm/arch/pmc_clock.h>



static int elite_pll_clk_wait_for_event(unsigned int event_reg, unsigned int event_bit)
{

}

void clk_enable(const char *dev_name)
{

}


void clk_disable(const char *dev_name)
{

}

int clk_set_rate(const char *dev_name, unsigned long rate)
{

}


unsigned long clk_get_rate(const char *dev_name)
{

}

static void pllc_detect_freq(void)
{

}

static void pllb_detect_freq(void)
{

}

void init_pmc_clock(void)
{

}

/**
 * Set parent frequency
 */
int clk_set_parent_rate(const char *dev_name, unsigned long rate)
{

}

unsigned long clk_get_parent_rate(const char *dev_name)
{
}



