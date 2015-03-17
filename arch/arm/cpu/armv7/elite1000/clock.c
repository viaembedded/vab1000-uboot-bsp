#include <common.h>
#include <linux/string.h>
#include <asm/io.h>
#include <asm/arch/pmc_clock.h>

#define ELT_PMC_BASE			0xD8130000

/* registers' offset */
#define REG_CLK_EN0			0x250
#define REG_CLK_EN1			0x254
#define REG_PLLB_MULT			0x208
#define REG_PLLB_MISC			0x20c
#define REG_PLLC_MULT			0x210
#define REG_PLLC_MISC			0x214
#define REG_PERIPH_UPDATE_STATUS	0x000
#define REG_PLL_UPDATE_STATUS		0X004
#define REG_WMTPLL			0x220

#define REG_PCIE_PWR_CTRL	0x400
#define REG_PCIE_PWR_STAT	0x401

#define NO_REG		((unsigned int)(~0))
#define NO_BIT		((unsigned int)(~0))
#define NO_MAX_DIV	((unsigned int)(~0))

#ifndef mdelay
# define mdelay(n)	udelay((n)*1000)
#endif
#ifndef ARRAY_SIZE
# define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

#define pmc_writel(v, a) \
        __raw_writel(v, (a + ELT_PMC_BASE))
#define pmc_readl(a) \
        __raw_readl((a + ELT_PMC_BASE))
#define pmc_writew(v, a) \
        __raw_writew(v, (a + ELT_PMC_BASE))
#define pmc_readw(a) \
        __raw_readw((a + ELT_PMC_BASE))
#define pmc_writeb(v, a) \
        __raw_writel(v, (a + ELT_PMC_BASE))
#define pmc_readb(a) \
        __raw_readb((a + ELT_PMC_BASE))

#define PERIPH_CLK(_name, _en_reg, _en_bit, _reg_div, _max_rate, _max_div, _stat_bit_div) \
	{					\
		.dev_name = _name,		\
		.en_reg = _en_reg,		\
		.en_bit = _en_bit,		\
		.reg_div = _reg_div,		\
		.max_rate = _max_rate,		\
		.stat_bit_div = _stat_bit_div,	\
		.max_div = _max_div,		\
		.parent_rate = 1200000000UL,	\
	}

//#define DEBUG
#ifdef DEBUG
#define pr_debug(fmt, arg...) printf(fmt, ##arg)
#else
#define pr_debug(fmt, arg...) do {}while(0)
#endif

struct clk {
	const char *dev_name;
	unsigned int en_reg;
	unsigned int en_bit;
	unsigned int reg_div;
	unsigned int max_div;
	unsigned int stat_bit_div;
	unsigned long max_rate;
	unsigned long parent_rate;
};

/* PLL Multiplier and Range Values Register */
union pll_reg_mult {
        struct {
                u32 r:3;
                u32 resv1:5;
                u32 int1:7;
                u32 resv2:1;
                u32 fn:10;
                u32 resv3:6;
        } field;
        u32 mult;
};

/* PLL misc signal control Register */
union pll_reg_misc {
        struct {
                u32 sscg_flag1:1;
                u32 sscg_sel:1;
                u32 sscg_flag:1;
                u32 sscg_tune:1;
                u32 lfp:2;
                u32 cp:3;
                u32 t:3;
                u32 tst:8;
                u32 order:1;
                u32 testb:1;
                u32 pd:1;
                u32 resv:10;
        } field;
        u32 misc;
};

struct pmc_pll {
        unsigned long freq;
        union pll_reg_mult u_mult;
        union pll_reg_misc u_misc;
};


#define PMC_PLL(_freq, _int, _fn, _r) \
	{                                 \
		.freq = _freq,          \
		.u_mult.field = {      \
			.r = _r,                \
			.int1 = _int,           \
			.fn = _fn,            \
		},                       \
		.u_misc.field = {   \
			.sscg_flag1 = 0,     \
			.sscg_sel = 0,        \
			.sscg_tune = 0,      \
			.lfp = 0,                 \
			.cp = 2,                 \
			.t = 0,                   \
			.tst = 0,                \
			.order = 1,             \
			.testb = 0,             \
		},                     \
	}

/* periphera pll */
static struct pmc_pll pll_b[] = {
	PMC_PLL(1200000000UL, 42, 456, 0),
	PMC_PLL(0, 0, 0, 0),
};

/* i2s pll */
static struct pmc_pll pll_c[] = {
#if 0
	PMC_PLL(16384000UL, 19, 865, 5),
	PMC_PLL(22579200UL, 28, 108, 5),
	PMC_PLL(33868800UL, 43, 162, 5),
	PMC_PLL(36864000UL, 47, 155, 5),
#endif
	PMC_PLL(27324000UL, 30, 786, 5),
	PMC_PLL(0, 0, 0, 0),
};

static struct clk periph_clk[] = {
	PERIPH_CLK("ahb", NO_REG, NO_BIT, 0x304, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("uhdc", REG_CLK_EN1, 7, 0x308, 48000000, 8, 31),
	PERIPH_CLK("dsp", REG_CLK_EN1, 20, 0x31c, 600000000, 32, 12),
	PERIPH_CLK("pcm", REG_CLK_EN0, 20, 0x320, 120000000, 32, 21),
	PERIPH_CLK("sd0", REG_CLK_EN1, 18, 0x328, 200000000, 2048, 22),
	PERIPH_CLK("nand", REG_CLK_EN1, 16, 0x330, 100000000, 32, 25),
	PERIPH_CLK("sd1", REG_CLK_EN0, 16, 0x338, 50000000, 2048, 20),
	PERIPH_CLK("spi0", REG_CLK_EN0, 12, 0x33c, 100000000, 64, 26),
	PERIPH_CLK("spi1", REG_CLK_EN0, 13, 0x340, 100000000, 64, 27),
	PERIPH_CLK("sd2", REG_CLK_EN0, 25, 0x344, 50000000, 2048, 18),
	PERIPH_CLK("pwm", REG_CLK_EN0, 10, 0x348, 50000000, 32, 14),
	PERIPH_CLK("se", REG_CLK_EN1, 24, 0x35c, 300000000, 32, 9),
	PERIPH_CLK("paxi", NO_REG, NO_BIT, 0x368, 600000000, 368, 28),
	PERIPH_CLK("i2c0", REG_CLK_EN0, 5, 0x36c, 40000000, 64, 5),
	PERIPH_CLK("i2c1", REG_CLK_EN0, 0, 0x370, 240000000, 64, 6),	
	PERIPH_CLK("gpio", REG_CLK_EN0, 11, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("kpad", REG_CLK_EN0, 9, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("cir", REG_CLK_EN0, 17, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("tzpc", REG_CLK_EN0, 8, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("rtc", REG_CLK_EN0, 7, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("uart0", REG_CLK_EN0, 1, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("uart1", REG_CLK_EN0, 2, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("uart2", REG_CLK_EN0, 3, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("uart3", REG_CLK_EN0, 4, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("pcie0", REG_CLK_EN0, 28, NO_REG, 24000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("dma", REG_CLK_EN1, 5, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("pcie1", REG_CLK_EN1, 8, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("pdma", REG_CLK_EN1, 9, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("sata", REG_CLK_EN1, 19, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("pcie2", REG_CLK_EN1, 22, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
	PERIPH_CLK("sf", REG_CLK_EN1, 23, NO_REG, 240000000, NO_MAX_DIV, NO_BIT),
};

static struct clk i2s_clk = {
	.dev_name = "i2s",
	.en_reg = REG_CLK_EN1,
	.en_bit = 15,
	.reg_div = 0x324,
	.max_rate = 27324000UL,//PLLC bug: 36863800,
	.max_div = 64,
	.stat_bit_div = 24,
	.parent_rate = 27324000UL,
};


static int elite_pll_clk_wait_for_event(unsigned int event_reg, unsigned int event_bit)
{
	int i;

	for (i = 0; i < 100; i++) {
		if (pmc_readl(event_reg) & (1 << event_bit)) {
			udelay(1000);
			return 0;
		}
		udelay(2);              /* timeout = 2 * lock time */
	}

	return -1;
}

void clk_enable(const char *dev_name)
{
	int i;
	
	for (i=0; i< ARRAY_SIZE(periph_clk); i++) {
		if (strcmp(dev_name, periph_clk[i].dev_name))
			continue;
		if (NO_REG == periph_clk[i].en_reg)
			return;
		pmc_writel(pmc_readl(periph_clk[i].en_reg) | (1 << periph_clk[i].en_bit),
				periph_clk[i].en_reg);
		pr_debug("%s: %s\n", __func__, periph_clk[i].dev_name);
	}

	if (!strcmp(dev_name, i2s_clk.dev_name)) {
		pmc_writel(pmc_readl(i2s_clk.en_reg) | (1 << i2s_clk.en_bit),
				i2s_clk.en_reg);
		pr_debug("%s: %s\n", __func__, i2s_clk.dev_name);
	} 
}


void clk_disable(const char *dev_name)
{
	int i;
	
	for (i=0; i< ARRAY_SIZE(periph_clk); i++) {
		if (strcmp(dev_name, periph_clk[i].dev_name))
			continue;
		if (NO_REG == periph_clk[i].en_reg) {
			printf("Can't enable clock for %s\n", dev_name);
			return;
		}
		pmc_writel(pmc_readl(periph_clk[i].en_reg) & (~(1 << periph_clk[i].en_bit)),
				periph_clk[i].en_reg);
		pr_debug("%s: %s\n", __func__, periph_clk[i].dev_name);
	}

	if (!strcmp(dev_name, i2s_clk.dev_name)) {
		pmc_writel(pmc_readl(i2s_clk.en_reg) & (~(1 << i2s_clk.en_bit)),
				i2s_clk.en_reg);
		pr_debug("%s: %s\n", __func__, i2s_clk.dev_name);
	} 
}

int clk_set_rate(const char *dev_name, unsigned long rate)
{
	unsigned long parent_rate;
	unsigned long div;
	int i;
	
	for (i = 0; i < ARRAY_SIZE(periph_clk); i++) {
		if (strcmp(dev_name, periph_clk[i].dev_name))
			continue;
		if (rate > periph_clk[i].max_rate) {
			printf("%s: Exceeding the maximum frequency1\n", dev_name);
			return -1;
		}
		if (NO_REG == periph_clk[i].reg_div) {
			printf("%s: Setting frequency is unsurpported\n", dev_name);
			return -2;
		}
		parent_rate = periph_clk[i].parent_rate;
		if (!strncmp(dev_name, "sd", 2))
			parent_rate /= 3;
		div = parent_rate/rate;
		if (div > periph_clk[i].max_div) {
			printf("%s: Exceeding the maximum frequency2 (BUG)\n", dev_name);
			return -3;
		}
		if (periph_clk[i].max_div < 255) {
			if (div == periph_clk[i].max_div)
				div = 0;
			pmc_writeb(div, periph_clk[i].reg_div);
		} else {
			if (div == periph_clk[i].max_div)
				div = 0;
			pmc_writew(div, periph_clk[i].reg_div);
		}
	
		if (elite_pll_clk_wait_for_event(REG_PERIPH_UPDATE_STATUS, periph_clk[i].stat_bit_div)) {
			pr_debug("time out waiting for periphera: %s\n", periph_clk[i].dev_name);
			return -5;
		}
		return 0;
	}

	if (strcmp(dev_name, i2s_clk.dev_name) && (i == ARRAY_SIZE(periph_clk))) {
		printf("unknown device\n");
		return -4;
	}

	if (rate > i2s_clk.max_rate) {
		printf("%s: Exceeding the maximum frequency3\n", dev_name);
		return -1;
	}
	if (NO_REG == i2s_clk.reg_div) {
		printf("%s: Setting frequency is unsurpported\n", dev_name);
		return -2;
	}
	div = i2s_clk.parent_rate/rate;
	if (div > i2s_clk.max_div) {
		printf("%s: Exceeding the maximum frequency4\n", dev_name);
		return -3;
	}
	if (div == i2s_clk.max_div)
		div = 0;
	pmc_writeb(div, i2s_clk.reg_div);
		
	if (elite_pll_clk_wait_for_event(REG_PERIPH_UPDATE_STATUS, i2s_clk.stat_bit_div)) {
		pr_debug("time out for periphera i2s\n");
		return -5;
	}

	return 0;
}


unsigned long clk_get_rate(const char *dev_name)
{
	unsigned long div;
	int i;

	for (i = 0; i < ARRAY_SIZE(periph_clk); i++) {
		if (strcmp(dev_name, periph_clk[i].dev_name))
			continue;

		if (!strncmp(dev_name, "uart", 4))
			return clk_get_rate("uhdc")/2;

		if (periph_clk[i].max_div < 255)
			div = pmc_readb(periph_clk[i].reg_div);
		else
			div = pmc_readw(periph_clk[i].reg_div);

		if (!div)
			div = periph_clk[i].max_div;
		if (!strncmp(dev_name, "sd", 2))
			div *= 3;
		return periph_clk[i].parent_rate/div;
	}

	if (strcmp(dev_name, i2s_clk.dev_name) && (i == ARRAY_SIZE(periph_clk))) {
		printf("unknown device\n");
		return 0;
	}
	div = pmc_readb(i2s_clk.reg_div);
	if (!div)
		div = i2s_clk.max_div;

	return i2s_clk.parent_rate/div;
}

static void pllc_detect_freq(void)
{
	struct pmc_pll *pll = pll_c;
	union pll_reg_mult mult; 
	union pll_reg_misc misc;
	unsigned long rate;

	misc.misc = pmc_readl(REG_PLLC_MISC);
	mult.mult = pmc_readl(REG_PLLC_MULT);
	for (; pll->freq != 0; pll++) {
		if (mult.mult == pll->u_mult.mult) {
			rate = pll->freq;
			break;
		}
	}

	if (!pll->freq) {
		printf ("\nPMC: Detected PLL_C freqency doesn't match the table # r: %d, int: %d, fn: %d\n",
			mult.field.r, mult.field.int1, mult.field.fn);
		return;
	}

	pr_debug ("\nPMC: Detected PLL_C frequency: %lu | r: %d, int: %d fn: %d\n", rate,
		mult.field.r, mult.field.int1, mult.field.fn);
	i2s_clk.parent_rate = rate;
}

static void pllb_detect_freq(void)
{
	struct pmc_pll *pll = pll_b;
	union pll_reg_mult mult; 
	union pll_reg_misc misc;
	unsigned long rate;
	int i;

	misc.misc = pmc_readl(REG_PLLB_MISC);
	mult.mult = pmc_readl(REG_PLLB_MULT);
	for (; pll->freq != 0; pll++) {
		if (mult.mult == pll->u_mult.mult) {
			rate = pll->freq;
			break;
		}
	}

	if (!pll->freq) {
		printf ("\nPMC: Detected PLL_B freqency doesn't match the table # r: %d, int: %d, fn: %d\n",
			mult.field.r, mult.field.int1, mult.field.fn);
		return;
	}

	pr_debug ("\nPMC: Detected PLL_B frequency: %lu | r: %d, int: %d fn: %d\n", rate,
		mult.field.r, mult.field.int1, mult.field.fn);
	for (i = 0; i < ARRAY_SIZE(periph_clk); i++) {
		periph_clk[i].parent_rate = rate;
	}
}

void init_pmc_clock(void)
{
	pllb_detect_freq();
	pllc_detect_freq();

	clk_enable("pcie0");
}

/**
 * Set parent frequency
 */
int clk_set_parent_rate(const char *dev_name, unsigned long rate)
{
	struct pmc_pll *pll = pll_c;
	union pll_reg_mult mult; 
	union pll_reg_misc misc;

	if(strcmp(dev_name, "i2s"))
		return -1;

	for (; pll->freq != 0; pll++) {
		if (rate == pll->freq) {
			break;
		}
	}

	if (!pll->freq) {
		printf("invalid freq\n");
		return -2;
	}

	misc.misc = pmc_readl(REG_PLLC_MISC);
	mult.mult = pmc_readl(REG_PLLC_MULT);

	/* disable PLL */
	misc.field.pd = 1;
	pmc_writel(misc.misc, REG_PLLC_MISC);

	/* set the expected value of INT/FN/R */
	pmc_writel(mult.mult, REG_PLLC_MULT);
	/* wait for 1ms as per the spec */
	udelay(1000);

	/* enable PLL */
	misc.field.pd = 0;
	pmc_writel(misc.misc, REG_PLLC_MISC);

	pmc_writel(pll->u_mult.mult, REG_PLLC_MULT);
	/* wait for update completed */
	if(elite_pll_clk_wait_for_event(REG_PLL_UPDATE_STATUS, 2))
		return -3;
	if(elite_pll_clk_wait_for_event(REG_WMTPLL, 18))
		return -3;

	/* wait for 60ms to make sure that PLL has already been steady */
	mdelay(60);
	return 0;
}

unsigned long clk_get_parent_rate(const char *dev_name)
{
	if(strcmp(dev_name, "i2s"))
		return periph_clk[0].parent_rate;
	else
		return i2s_clk.parent_rate;
}

/**
 * @ c = 1: power down PCIE
 * @ c = 0: power up PCIE
 */
void pcie_set_pwr_ctrl(int c)
{
	if (c) //power down PCIE host controller
		pmc_writeb(0x1, REG_PCIE_PWR_CTRL);
	else   //power up PCIE host controller
		pmc_writeb(0x0, REG_PCIE_PWR_CTRL);
}

/**
 * @ p = 0: get PCIE0 status
 * @ p = 1: get PCIE1 status
 * ret: 1: power down; 0: power up
 */
int pcie_get_pwr_stat(unsigned int p)
{
	if (p > 1)
		return -1;
	if (!p)
		return pmc_readb(REG_PCIE_PWR_STAT) & 0x1;
	else
		return (pmc_readb(REG_PCIE_PWR_STAT) & 0x2) >> 1;
}


