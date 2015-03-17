
#include <common.h>
#include <sdhci.h>


//#define DEBUG
#ifdef  DEBUG
#define DEBUG(format, arg...) printf(" Elite_MMC : " format "\n", ## arg)
#else
#define DEBUG(format, arg...) do {} while(0)
#endif /* DEBUG */

#define MIU_SEC_RANGE_ADDR	0xd813d390
void *msec_base = (void *)MIU_SEC_RANGE_ADDR;
#define sec_writel(v, a) \
	writel(v, (msec_base + (a)))

void set_secure_range(void)
{
	int i;
	static int flag = 0;

	if(flag) return;

	for (i = 0; i <= 0x3c; i += 0x04)
		sec_writel(0, i);
	sec_writel(1, 0x60);

	flag = 1;
}

int elite_sdhci_init(int index, void * base)
{
	struct sdhci_host *host = NULL;

	host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
	if (!host) {
		printf("sdhci host malloc fail!\n");
		return 1;
	}

	set_secure_range();

	memset(host, 0, sizeof(struct sdhci_host));

	host->name = "sdhci";
	host->ioaddr = base;
	host->index = index;
	DEBUG("SD bass address : %08x\n", host->ioaddr );
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);
	host->voltages = MMC_VDD_32_33 | MMC_VDD_33_34;// | MMC_VDD_165_195;
	DEBUG("SD host version register : %04x\n", host->version );
	return add_sdhci(host, 52000000, 200000);
}

