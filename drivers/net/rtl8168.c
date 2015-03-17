#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <net.h>
#include <pci.h>
#include <errno.h>
#include <linux/mii.h>
#include <linux/compat.h>
#include <asm/io.h>

#include "rtl8168.h"


#define netdev_priv(dev) dev->priv

#define deviceid  PCI_BDF(1,0,0)  //pci_dev_t

static u16 rtl8168_intr_mask = SYSErr | LinkChg | RxDescUnavail | TxErr | TxOK | RxErr | RxOK;

static unsigned short speed = SPEED_1000;
static int duplex = DUPLEX_FULL;
static int autoneg = AUTONEG_ENABLE;
static int aspm = 1;
static int eee_enable = 0 ;

/* Define the TX Descriptor */
//static u8* tx_ring;

static u8 tx_ring[R8168_TX_RING_BYTES] __attribute__((aligned(256)));

/* Create a static buffer of size RX_BUF_SZ for each
TX Descriptor.  All descriptors point to a
part of this buffer */
//static unsigned char *txb;

static unsigned char txb[NUM_TX_DESC * RX_BUF_SIZE] __attribute__((aligned(256)));

/* Define the RX Descriptor */
//static u8* rx_ring;
static u8 rx_ring[R8168_RX_RING_BYTES]  __attribute__((aligned(256)));

/* Create a static buffer of size RX_BUF_SZ for each
RX Descriptor   All descriptors point to a
part of this buffer */

// static unsigned char *rxb;

static unsigned char rxb[NUM_RX_DESC * 2048] __attribute__((aligned(256)));

// #define BUFFER_TOTAL (R8168_TX_RING_BYTES + NUM_TX_DESC * RX_BUF_SIZE + R8168_RX_RING_BYTES + NUM_RX_DESC * 2048)

#define _R(NAME,MAC,RCR,MASK, JumFrameSz) \
    { .name = NAME, .mcfg = MAC, .RCR_Cfg = RCR, .RxConfigMask = MASK, .jumbo_frame_sz = JumFrameSz }

typedef struct MAC_FG_S {
	const char *name;
	u8 mcfg;
	u32 RCR_Cfg;
	u32 RxConfigMask;   /* Clears the bits supported by this chip */
	u32 jumbo_frame_sz;
} MAC_FG_T;



static MAC_FG_T rtl_chip_info[] = {
	_R("RTL8168G/8111G",
	CFG_METHOD_21,
	RxCfg_128_int_en | RxEarly_off | (RX_DMA_BURST << RxCfgDMAShift),
	0xff7e1880,
	Jumbo_Frame_9k),
};
#undef _R

static struct pci_device_id ehci_pci_ids[] = {
	/* Please add supported PCI EHCI controller ids here */
	{PCI_VENDOR_ID_REALTEK, 0x8168},
	{PCI_VENDOR_ID_DLINK, 0x4300},
	{0, 0}
};

#define CACHE_ALIGN_SIZE 32
static void net_flush_cache(unsigned long start, unsigned long size)
{
	flush_dcache_range((start & (~ (CACHE_ALIGN_SIZE -1))), ALIGN(start+size, CACHE_ALIGN_SIZE));
}
static void net_inval_cache(unsigned long start, unsigned long size)
{
	invalidate_dcache_range((start & (~ (CACHE_ALIGN_SIZE - 1))), ALIGN(start+size, CACHE_ALIGN_SIZE));

}

void
rtl8168_rar_set(struct rtl8168_private *tp,
                uint8_t *addr)
{
	void __iomem *ioaddr = tp->mmio_addr;
	uint32_t rar_low = 0;
	uint32_t rar_high = 0;

	rar_low = ((uint32_t) addr[0] |
	           ((uint32_t) addr[1] << 8) |
	           ((uint32_t) addr[2] << 16) |
	           ((uint32_t) addr[3] << 24));

	rar_high = ((uint32_t) addr[4] |
	            ((uint32_t) addr[5] << 8));

	RTL_W8(Cfg9346, Cfg9346_Unlock);
	RTL_W32(MAC0, rar_low);
	RTL_W32(MAC4, rar_high);

	if(tp->mcfg == CFG_METHOD_17) {
		rtl8168_eri_write(tp, 0xe0, 4, rar_low, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xe4, 4, rar_high, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xf0, 4, rar_low << 16, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xf4, 4, rar_low >> 16 | rar_high << 16, ERIAR_ExGMAC);
	}

	RTL_W8(Cfg9346, Cfg9346_Lock);
}

void setup_eth_mac(struct eth_device *dev, uint8_t *addr)
{
	if(strcmp(dev->name, "RTL8168") == 0)
		rtl8168_rar_set(netdev_priv(dev), addr);
}


static void rtl8168_get_mac_version(struct rtl8168_private *tp)
{
	u32 reg, val32;
	u32 ICVerID;

	val32 = RTL_R32(TxConfig)  ;
	reg = val32 & 0x7c800000;
	ICVerID = val32 & 0x00700000;

	switch(reg) {

	case 0x4C000000:
		if(ICVerID == 0x00000000)
			tp->mcfg = CFG_METHOD_21;
		else if(ICVerID == 0x00100000)
			tp->mcfg = CFG_METHOD_22;
		tp->efuse = EFUSE_SUPPORT;
		break;
	default:
		printf("unknown chip version (%x)\n", reg);
		tp->mcfg = CFG_METHOD_DEFAULT;
		tp->efuse = EFUSE_NOT_SUPPORT;
		break;
	}
}

static void
rtl8168_print_mac_version(struct rtl8168_private *tp)
{
	int i;
	for(i = ARRAY_SIZE(rtl_chip_info) - 1; i >= 0; i--) {
		if(tp->mcfg == rtl_chip_info[i].mcfg) {
			printf("MCFG:  %s (%04d)\n", rtl_chip_info[i].name,
			       rtl_chip_info[i].mcfg);
			return;
		}
	}

	printf("mac_version == Unknown\n");
}


static inline u16 map_phy_ocp_addr(u16 page, u16 reg)
{
	if(page != OCP_STD_PHY_BASE_PAGE) {
		reg -= 16;
	}

	page <<= 4;
	reg <<= 1;

	return (page + reg);
}

void mdio_write(struct rtl8168_private *tp,
                u32 RegAddr,
                u32 value)
{
	int i;

	if(tp->mcfg == CFG_METHOD_21 || tp->mcfg == CFG_METHOD_22 || tp->mcfg == CFG_METHOD_23 || tp->mcfg == CFG_METHOD_24) {
		u32 data32;
		u16 ocp_addr;

		if(RegAddr == 0x1F) {
			tp->cur_page = value ? value : OCP_STD_PHY_BASE_PAGE;
			return;
		}
		ocp_addr = map_phy_ocp_addr(tp->cur_page, RegAddr);
		data32 = ocp_addr / 2;
		data32 <<= OCPR_Addr_Reg_shift;
		data32 |= OCPR_Write | value;

		RTL_W32(PHYOCP, data32);
		for(i = 0; i < 10; i++) {
			udelay(100);

			if(!(RTL_R32(PHYOCP) & OCPR_Flag))
				break;
		}
	}
}

u32 mdio_read(struct rtl8168_private *tp,
              u32 RegAddr)
{
	int i, value = 0;

	if(tp->mcfg == CFG_METHOD_21 || tp->mcfg == CFG_METHOD_22 || tp->mcfg == CFG_METHOD_23 || tp->mcfg == CFG_METHOD_24) {
		u32 data32;
		u16 ocp_addr;

		ocp_addr = map_phy_ocp_addr(tp->cur_page, RegAddr);

		data32 = ocp_addr / 2;
		data32 <<= OCPR_Addr_Reg_shift;

		RTL_W32(PHYOCP, data32);
		for(i = 0; i < 10; i++) {
			udelay(100);

			if(RTL_R32(PHYOCP) & OCPR_Flag)
				break;
		}
		value = RTL_R32(PHYOCP) & OCPDR_Data_Mask;
	}

	return value;
}

void mac_ocp_write(struct rtl8168_private *tp, u16 reg_addr, u16 value)
{
	u32 data32;


	data32 = reg_addr / 2;
	data32 <<= OCPR_Addr_Reg_shift;
	data32 += value;
	data32 |= OCPR_Write;

	RTL_W32(MACOCP, data32);
}

u16 mac_ocp_read(struct rtl8168_private *tp, u16 reg_addr)
{
	u32 data32;
	u16 data16 = 0;

	data32 = reg_addr / 2;
	data32 <<= OCPR_Addr_Reg_shift;

	RTL_W32(MACOCP, data32);
	data16 = (u16) RTL_R32(MACOCP);

	return data16;
}

u32 OCP_read(struct rtl8168_private *tp, u8 mask, u16 Reg)
{
	int i;

	{
		RTL_W32(OCPAR, ((u32) mask & 0xF) << 12 | (Reg & 0xFFF));
		for(i = 0; i < 20; i++) {
			udelay(100);
			if(RTL_R32(OCPAR) & OCPAR_Flag)
				break;
		}
		return RTL_R32(OCPDR);
	}
}

void OCP_write(struct rtl8168_private *tp, u8 mask, u16 Reg, u32 data)
{
	int i;

	{
		RTL_W32(OCPDR, data);
		RTL_W32(OCPAR, OCPAR_Flag | ((u32) mask & 0xF) << 12 | (Reg & 0xFFF));
		for(i = 0; i < 20; i++) {
			udelay(100);
			if((RTL_R32(OCPAR) &OCPAR_Flag) == 0)
				break;
		}
	}
}

void OOB_mutex_lock(struct rtl8168_private *tp)
{
	{
		u32 reg;

		if(tp->mcfg == CFG_METHOD_13)
			reg = 0x04;
		else
			reg = 0x14;

		OCP_write(tp, 0x8, reg, 0x01000000);

		while(OCP_read(tp, 0xF, reg) & 0x00FF0000) {
			if(OCP_read(tp, 0xF, 0x09C) & 0x000000FF) {
				OCP_write(tp, 0x8, reg, 0x00000000);

				while(OCP_read(tp, 0xF, 0x09C) & 0x000000FF);

				OCP_write(tp, 0x8, reg, 0x01000000);
			}
		}
	}
}

void OOB_mutex_unlock(struct rtl8168_private *tp)
{
	{
		u32 reg;

		if(tp->mcfg == CFG_METHOD_13)
			reg = 0x04;
		else
			reg = 0x14;

		OCP_write(tp, 0x1, 0x9C, 0x00000001);
		OCP_write(tp, 0x8, reg, 0x00000000);
	}
}

void OOB_notify(struct rtl8168_private *tp, u8 cmd)
{
	int i;

	RTL_W8(ERIDR, cmd);
	RTL_W32(ERIAR, 0x800010E8);
	mdelay(2);
	for(i = 0; i < 5; i++) {
		udelay(100);
		if(!(RTL_R32(ERIAR) & ERIAR_Flag))
			break;
	}

	OCP_write(tp, 0x1, 0x30, 0x00000001);
}


void rtl8168_ephy_write(struct rtl8168_private *tp, int RegAddr, int value)
{
	int i;

	RTL_W32(EPHYAR,
	        EPHYAR_Write |
	        (RegAddr & EPHYAR_Reg_Mask) << EPHYAR_Reg_shift |
	        (value & EPHYAR_Data_Mask));

	for(i = 0; i < 10; i++) {
		udelay(100);

		/* Check if the RTL8168 has completed EPHY write */
		if(!(RTL_R32(EPHYAR) & EPHYAR_Flag))
			break;
	}

	udelay(20);
}

u16 rtl8168_ephy_read(struct rtl8168_private *tp, int RegAddr)
{
	int i;
	u16 value = 0xffff;

	RTL_W32(EPHYAR,
	        EPHYAR_Read | (RegAddr & EPHYAR_Reg_Mask) << EPHYAR_Reg_shift);

	for(i = 0; i < 10; i++) {
		udelay(100);

		/* Check if the RTL8168 has completed EPHY read */
		if(RTL_R32(EPHYAR) & EPHYAR_Flag) {
			value = (u16)(RTL_R32(EPHYAR) & EPHYAR_Data_Mask);
			break;
		}
	}

	udelay(20);

	return value;
}

static void
rtl8168_csi_write(struct rtl8168_private *tp,
                  u32 addr,
                  u32 value)
{
	u32 cmd;
	int i;

	RTL_W32(CSIDR, value);
	cmd = CSIAR_Write | CSIAR_ByteEn << CSIAR_ByteEn_shift | (addr & CSIAR_Addr_Mask);
	if(tp->mcfg == CFG_METHOD_20)
		cmd |= 0x00020000;
	RTL_W32(CSIAR, cmd);

	for(i = 0; i < 10; i++) {
		udelay(100);

		/* Check if the RTL8168 has completed CSI write */
		if(!(RTL_R32(CSIAR) & CSIAR_Flag))
			break;
	}

	udelay(20);
}

static u32
rtl8168_csi_read(struct rtl8168_private *tp,
                 u32 addr)
{
	u32 cmd;
	int i;
	u32 value = 0;

	cmd = CSIAR_Read | CSIAR_ByteEn << CSIAR_ByteEn_shift | (addr & CSIAR_Addr_Mask);

	if(tp->mcfg == CFG_METHOD_20)
		cmd |= 0x00020000;

	RTL_W32(CSIAR, cmd);

	for(i = 0; i < 10; i++) {
		udelay(100);

		/* Check if the RTL8168 has completed CSI read */
		if(RTL_R32(CSIAR) & CSIAR_Flag) {
			value = (u32) RTL_R32(CSIDR);
			break;
		}
	}

	udelay(20);

	return value;
}

u32 rtl8168_eri_read(struct rtl8168_private *tp, int addr, int len, int type)
{
	int i, val_shift, shift = 0;
	u32 value1 = 0, value2 = 0, mask;

	if(len > 4 || len <= 0)
		return -1;

	while(len > 0) {
		val_shift = addr % ERIAR_Addr_Align;
		addr = addr & ~0x3;

		RTL_W32(ERIAR,
		        ERIAR_Read |
		        type << ERIAR_Type_shift |
		        ERIAR_ByteEn << ERIAR_ByteEn_shift |
		        addr);

		for(i = 0; i < 10; i++) {
			udelay(100);

			/* Check if the RTL8168 has completed ERI read */
			if(RTL_R32(ERIAR) & ERIAR_Flag)
				break;
		}

		if(len == 1)       mask = (0xFF << (val_shift * 8)) & 0xFFFFFFFF;
		else if(len == 2)  mask = (0xFFFF << (val_shift * 8)) & 0xFFFFFFFF;
		else if(len == 3)  mask = (0xFFFFFF << (val_shift * 8)) & 0xFFFFFFFF;
		else            mask = (0xFFFFFFFF << (val_shift * 8)) & 0xFFFFFFFF;

		value1 = RTL_R32(ERIDR) & mask;
		value2 |= (value1 >> val_shift * 8) << shift * 8;

		if(len <= 4 - val_shift) {
			len = 0;
		} else {
			len -= (4 - val_shift);
			shift = 4 - val_shift;
			addr += 4;
		}
	}

	return value2;
}

int rtl8168_eri_write(struct rtl8168_private *tp, int addr, int len, u32 value, int type)
{

	int i, val_shift, shift = 0;
	u32 value1 = 0, mask;

	if(len > 4 || len <= 0)
		return -1;

	while(len > 0) {
		val_shift = addr % ERIAR_Addr_Align;
		addr = addr & ~0x3;

		if(len == 1)       mask = (0xFF << (val_shift * 8)) & 0xFFFFFFFF;
		else if(len == 2)  mask = (0xFFFF << (val_shift * 8)) & 0xFFFFFFFF;
		else if(len == 3)  mask = (0xFFFFFF << (val_shift * 8)) & 0xFFFFFFFF;
		else            mask = (0xFFFFFFFF << (val_shift * 8)) & 0xFFFFFFFF;

		value1 = rtl8168_eri_read(tp, addr, 4, type) & ~mask;
		value1 |= ((value << val_shift * 8) >> shift * 8);

		RTL_W32(ERIDR, value1);
		RTL_W32(ERIAR,
		        ERIAR_Write |
		        type << ERIAR_Type_shift |
		        ERIAR_ByteEn << ERIAR_ByteEn_shift |
		        addr);

		for(i = 0; i < 10; i++) {
			udelay(100);

			/* Check if the RTL8168 has completed ERI write */
			if(!(RTL_R32(ERIAR) & ERIAR_Flag))
				break;
		}

		if(len <= 4 - val_shift) {
			len = 0;
		} else {
			len -= (4 - val_shift);
			shift = 4 - val_shift;
			addr += 4;
		}
	}

	return 0;
}



static void
rtl8168_phy_power_up(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	mdio_write(tp, 0x1F, 0x0000);
	mdio_write(tp, MII_BMCR, BMCR_ANENABLE);
}


static int rtl8168_enable_EEE(struct rtl8168_private *tp)
{
	int ret;
	u16 data;

	ret = 0;
	switch(tp->mcfg) {

	case CFG_METHOD_21:
	case CFG_METHOD_22:
	case CFG_METHOD_24:
		data = rtl8168_eri_read(tp, 0x1B0, 4, ERIAR_ExGMAC);
		data |= BIT_1 | BIT_0;
		rtl8168_eri_write(tp, 0x1B0, 4, data, ERIAR_ExGMAC);
		mdio_write(tp, 0x1F, 0x0A43);
		data = mdio_read(tp, 0x11);
		mdio_write(tp, 0x11, data | BIT_4);
		mdio_write(tp, 0x1F, 0x0A5D);
		mdio_write(tp, 0x10, 0x0006);
		mdio_write(tp, 0x1F, 0x0000);
		break;

	default:
		printk(KERN_DEBUG "Not Support EEE\n");
		ret = -EOPNOTSUPP;
	}

	return ret;
}


static int rtl8168_disable_EEE(struct rtl8168_private *tp)
{
	int ret;
	u16 data;

	ret = 0;
	switch(tp->mcfg) {

	case CFG_METHOD_21:
	case CFG_METHOD_22:
	case CFG_METHOD_24:
		data = rtl8168_eri_read(tp, 0x1B0, 4, ERIAR_ExGMAC);
		data &= ~(BIT_1 | BIT_0);
		rtl8168_eri_write(tp, 0x1B0, 4, data, ERIAR_ExGMAC);
		mdio_write(tp, 0x1F, 0x0A43);
		data = mdio_read(tp, 0x11);
		mdio_write(tp, 0x11, data & ~BIT_4);
		mdio_write(tp, 0x1F, 0x0A5D);
		mdio_write(tp, 0x10, 0x0000);
		mdio_write(tp, 0x1F, 0x0000);
		break;

	default:
		printk(KERN_DEBUG "Not Support EEE\n");
		ret = -EOPNOTSUPP;
		break;
	}

	return ret;
}

static void
rtl8168_hw_phy_config(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	unsigned int gphy_val, i;
	u32 csi_tmp;


	if(tp->mcfg == CFG_METHOD_21) {
		u16 rtl8111g_phy_value[] = {
			0xE008, 0xE01B, 0xE01D, 0xE01F, 0xE022,
			0xE025, 0xE031, 0xE04D, 0x49D2, 0xF10D,
			0x766C, 0x49E2, 0xF00A, 0x1EC0, 0x8EE1,
			0xC60A, 0x77C0, 0x4870, 0x9FC0, 0x1EA0,
			0xC707, 0x8EE1, 0x9D6C, 0xC603, 0xBE00,
			0xB416, 0x0076, 0xE86C, 0xC602, 0xBE00,
			0xA000, 0xC602, 0xBE00, 0x0000, 0x1B76,
			0xC202, 0xBA00, 0x059C, 0x1B76, 0xC602,
			0xBE00, 0x065A, 0x74E6, 0x1B78, 0x46DC,
			0x1300, 0xF005, 0x74F8, 0x48C3, 0x48C4,
			0x8CF8, 0x64E7, 0xC302, 0xBB00, 0x06A0,
			0x74E4, 0x49C5, 0xF106, 0x49C6, 0xF107,
			0x48C8, 0x48C9, 0xE011, 0x48C9, 0x4848,
			0xE00E, 0x4848, 0x49C7, 0xF00A, 0x48C9,
			0xC60D, 0x1D1F, 0x8DC2, 0x1D00, 0x8DC3,
			0x1D11, 0x8DC0, 0xE002, 0x4849, 0x94E5,
			0xC602, 0xBE00, 0x01F0, 0xE434, 0x49D9,
			0xF01B, 0xC31E, 0x7464, 0x49C4, 0xF114,
			0xC31B, 0x6460, 0x14FA, 0xFA02, 0xE00F,
			0xC317, 0x7460, 0x49C0, 0xF10B, 0xC311,
			0x7462, 0x48C1, 0x9C62, 0x4841, 0x9C62,
			0xC30A, 0x1C04, 0x8C60, 0xE004, 0x1C15,
			0xC305, 0x8C60, 0xC602, 0xBE00, 0x0384,
			0xE434, 0xE030, 0xE61C, 0xE906
		};
		mac_ocp_write(tp, 0xFC28, 0x0000);
		mac_ocp_write(tp, 0xFC2A, 0x0000);
		mac_ocp_write(tp, 0xFC2C, 0x0000);
		mac_ocp_write(tp, 0xFC2E, 0x0000);
		mac_ocp_write(tp, 0xFC30, 0x0000);
		mac_ocp_write(tp, 0xFC32, 0x0000);
		mac_ocp_write(tp, 0xFC34, 0x0000);
		mac_ocp_write(tp, 0xFC36, 0x0000);
		mdelay(3);
		mac_ocp_write(tp, 0xFC26, 0x0000);
		for(i = 0; i < ARRAY_SIZE(rtl8111g_phy_value); i++)
			mac_ocp_write(tp, 0xF800 + i * 2, rtl8111g_phy_value[i]);
		mac_ocp_write(tp, 0xFC26, 0x8000);
		mac_ocp_write(tp, 0xFC28, 0x0075);
		mac_ocp_write(tp, 0xFC2E, 0x059B);
		mac_ocp_write(tp, 0xFC30, 0x0659);
		mac_ocp_write(tp, 0xFC32, 0x069F);
		mac_ocp_write(tp, 0xFC34, 0x01CD);
		mac_ocp_write(tp, 0xFC36, 0x0303);

		RTL_W8(0xD0, RTL_R8(0xD0) | BIT_6 | BIT_7);
		RTL_W8(0xF2, RTL_R8(0xF2) | BIT_6);
		rtl8168_eri_write(tp, 0x5F0, 2, 0x4f87, ERIAR_ExGMAC);
		gphy_val = rtl8168_eri_read(tp, 0xD4, 2, ERIAR_ExGMAC);
		gphy_val |= (0x3F << 7);
		rtl8168_eri_write(tp, 0xD4, 2, gphy_val, ERIAR_ExGMAC);
		gphy_val = rtl8168_eri_read(tp, 0x1D0, 1, ERIAR_ExGMAC);
		gphy_val |= BIT_1;
		rtl8168_eri_write(tp, 0x1D0, 1, gphy_val, ERIAR_ExGMAC);

		mdio_write(tp, 0x1f, 0x0B82);
		gphy_val = mdio_read(tp, 0x10);
		gphy_val |= BIT_4;
		mdio_write(tp, 0x10, gphy_val);
		mdio_write(tp, 0x1f, 0x0B80);
		for(i = 0; i < 10; i++) {
			if(mdio_read(tp, 0x10) & 0x0040)
				break;
			mdelay(10);
		}
		mdio_write(tp, 0x1f, 0x0A43);
		mdio_write(tp, 0x13, 0x8146);
		mdio_write(tp, 0x14, 0x2300);
		mdio_write(tp, 0x13, 0xB820);
		mdio_write(tp, 0x14, 0x0210);
		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0xB820);
		mdio_write(tp, 0x14, 0x0290);
		mdio_write(tp, 0x13, 0xA012);
		mdio_write(tp, 0x14, 0x0000);
		mdio_write(tp, 0x13, 0xA014);
		mdio_write(tp, 0x14, 0x2c04);
		mdio_write(tp, 0x14, 0x2c1b);
		mdio_write(tp, 0x14, 0x2c65);
		mdio_write(tp, 0x14, 0x2d06);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x4092);
		mdio_write(tp, 0x14, 0xba04);
		mdio_write(tp, 0x14, 0x3084);
		mdio_write(tp, 0x14, 0x1cf6);
		mdio_write(tp, 0x14, 0x1ccf);
		mdio_write(tp, 0x14, 0x1cda);
		mdio_write(tp, 0x14, 0xaeff);
		mdio_write(tp, 0x14, 0xaf02);
		mdio_write(tp, 0x14, 0x8f02);
		mdio_write(tp, 0x14, 0x8eff);
		mdio_write(tp, 0x14, 0xce01);
		mdio_write(tp, 0x14, 0xe070);
		mdio_write(tp, 0x14, 0x0f00);
		mdio_write(tp, 0x14, 0xaf01);
		mdio_write(tp, 0x14, 0x8f01);
		mdio_write(tp, 0x14, 0xd712);
		mdio_write(tp, 0x14, 0x5fe8);
		mdio_write(tp, 0x14, 0xaf02);
		mdio_write(tp, 0x14, 0x8f02);
		mdio_write(tp, 0x14, 0x8e01);
		mdio_write(tp, 0x14, 0x1ce4);
		mdio_write(tp, 0x14, 0x27f2);
		mdio_write(tp, 0x14, 0xd05a);
		mdio_write(tp, 0x14, 0xd19a);
		mdio_write(tp, 0x14, 0xd709);
		mdio_write(tp, 0x14, 0x608f);
		mdio_write(tp, 0x14, 0xd06b);
		mdio_write(tp, 0x14, 0xd18a);
		mdio_write(tp, 0x14, 0x2c25);
		mdio_write(tp, 0x14, 0xd0be);
		mdio_write(tp, 0x14, 0xd188);
		mdio_write(tp, 0x14, 0x2c25);
		mdio_write(tp, 0x14, 0xd708);
		mdio_write(tp, 0x14, 0x4072);
		mdio_write(tp, 0x14, 0xc104);
		mdio_write(tp, 0x14, 0x2c37);
		mdio_write(tp, 0x14, 0x4076);
		mdio_write(tp, 0x14, 0xc110);
		mdio_write(tp, 0x14, 0x2c37);
		mdio_write(tp, 0x14, 0x4071);
		mdio_write(tp, 0x14, 0xc102);
		mdio_write(tp, 0x14, 0x2c37);
		mdio_write(tp, 0x14, 0x4070);
		mdio_write(tp, 0x14, 0xc101);
		mdio_write(tp, 0x14, 0x2c37);
		mdio_write(tp, 0x14, 0x175b);
		mdio_write(tp, 0x14, 0xd709);
		mdio_write(tp, 0x14, 0x3390);
		mdio_write(tp, 0x14, 0x5c32);
		mdio_write(tp, 0x14, 0x2c47);
		mdio_write(tp, 0x14, 0x175b);
		mdio_write(tp, 0x14, 0xd708);
		mdio_write(tp, 0x14, 0x6193);
		mdio_write(tp, 0x14, 0xd709);
		mdio_write(tp, 0x14, 0x5f9d);
		mdio_write(tp, 0x14, 0x408b);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x6042);
		mdio_write(tp, 0x14, 0xb401);
		mdio_write(tp, 0x14, 0x175b);
		mdio_write(tp, 0x14, 0xd708);
		mdio_write(tp, 0x14, 0x6073);
		mdio_write(tp, 0x14, 0x5fbc);
		mdio_write(tp, 0x14, 0x2c46);
		mdio_write(tp, 0x14, 0x26ed);
		mdio_write(tp, 0x14, 0xb280);
		mdio_write(tp, 0x14, 0xa841);
		mdio_write(tp, 0x14, 0x9420);
		mdio_write(tp, 0x14, 0x8710);
		mdio_write(tp, 0x14, 0xd709);
		mdio_write(tp, 0x14, 0x42ec);
		mdio_write(tp, 0x14, 0x606d);
		mdio_write(tp, 0x14, 0xd207);
		mdio_write(tp, 0x14, 0x2c50);
		mdio_write(tp, 0x14, 0xd203);
		mdio_write(tp, 0x14, 0x33ff);
		mdio_write(tp, 0x14, 0x563b);
		mdio_write(tp, 0x14, 0x3275);
		mdio_write(tp, 0x14, 0x7c57);
		mdio_write(tp, 0x14, 0xb240);
		mdio_write(tp, 0x14, 0xb402);
		mdio_write(tp, 0x14, 0x263b);
		mdio_write(tp, 0x14, 0x6096);
		mdio_write(tp, 0x14, 0xb240);
		mdio_write(tp, 0x14, 0xb406);
		mdio_write(tp, 0x14, 0x263b);
		mdio_write(tp, 0x14, 0x31d7);
		mdio_write(tp, 0x14, 0x7c60);
		mdio_write(tp, 0x14, 0xb240);
		mdio_write(tp, 0x14, 0xb40e);
		mdio_write(tp, 0x14, 0x263b);
		mdio_write(tp, 0x14, 0xb410);
		mdio_write(tp, 0x14, 0x8802);
		mdio_write(tp, 0x14, 0xb240);
		mdio_write(tp, 0x14, 0x940e);
		mdio_write(tp, 0x14, 0x263b);
		mdio_write(tp, 0x14, 0xba04);
		mdio_write(tp, 0x14, 0x1ccf);
		mdio_write(tp, 0x14, 0xa902);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x4045);
		mdio_write(tp, 0x14, 0xa980);
		mdio_write(tp, 0x14, 0x3003);
		mdio_write(tp, 0x14, 0x59b1);
		mdio_write(tp, 0x14, 0xa540);
		mdio_write(tp, 0x14, 0xa601);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4043);
		mdio_write(tp, 0x14, 0xa910);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x60a0);
		mdio_write(tp, 0x14, 0xca33);
		mdio_write(tp, 0x14, 0xcb33);
		mdio_write(tp, 0x14, 0xa941);
		mdio_write(tp, 0x14, 0x2c7b);
		mdio_write(tp, 0x14, 0xcaff);
		mdio_write(tp, 0x14, 0xcbff);
		mdio_write(tp, 0x14, 0xa921);
		mdio_write(tp, 0x14, 0xce02);
		mdio_write(tp, 0x14, 0xe070);
		mdio_write(tp, 0x14, 0x0f10);
		mdio_write(tp, 0x14, 0xaf01);
		mdio_write(tp, 0x14, 0x8f01);
		mdio_write(tp, 0x14, 0x1766);
		mdio_write(tp, 0x14, 0x8e02);
		mdio_write(tp, 0x14, 0x1787);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x609c);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fa4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0x1ce2);
		mdio_write(tp, 0x14, 0xce04);
		mdio_write(tp, 0x14, 0xe070);
		mdio_write(tp, 0x14, 0x0f20);
		mdio_write(tp, 0x14, 0xaf01);
		mdio_write(tp, 0x14, 0x8f01);
		mdio_write(tp, 0x14, 0x1766);
		mdio_write(tp, 0x14, 0x8e04);
		mdio_write(tp, 0x14, 0x6044);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0xa520);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4043);
		mdio_write(tp, 0x14, 0x2cba);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0501);
		mdio_write(tp, 0x14, 0x1ce8);
		mdio_write(tp, 0x14, 0xb801);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x4060);
		mdio_write(tp, 0x14, 0x7fc4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0x1cee);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0502);
		mdio_write(tp, 0x14, 0x1ce8);
		mdio_write(tp, 0x14, 0xb802);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x4061);
		mdio_write(tp, 0x14, 0x7fc4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0x1cee);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0504);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6099);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fa4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0xc17f);
		mdio_write(tp, 0x14, 0xc200);
		mdio_write(tp, 0x14, 0xc43f);
		mdio_write(tp, 0x14, 0xcc03);
		mdio_write(tp, 0x14, 0xa701);
		mdio_write(tp, 0x14, 0xa510);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4018);
		mdio_write(tp, 0x14, 0x9910);
		mdio_write(tp, 0x14, 0x8510);
		mdio_write(tp, 0x14, 0x2860);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0504);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6099);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fa4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0xa608);
		mdio_write(tp, 0x14, 0xc17d);
		mdio_write(tp, 0x14, 0xc200);
		mdio_write(tp, 0x14, 0xc43f);
		mdio_write(tp, 0x14, 0xcc03);
		mdio_write(tp, 0x14, 0xa701);
		mdio_write(tp, 0x14, 0xa510);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4018);
		mdio_write(tp, 0x14, 0x9910);
		mdio_write(tp, 0x14, 0x8510);
		mdio_write(tp, 0x14, 0x2926);
		mdio_write(tp, 0x14, 0x1792);
		mdio_write(tp, 0x14, 0x27db);
		mdio_write(tp, 0x14, 0xc000);
		mdio_write(tp, 0x14, 0xc100);
		mdio_write(tp, 0x14, 0xc200);
		mdio_write(tp, 0x14, 0xc300);
		mdio_write(tp, 0x14, 0xc400);
		mdio_write(tp, 0x14, 0xc500);
		mdio_write(tp, 0x14, 0xc600);
		mdio_write(tp, 0x14, 0xc7c1);
		mdio_write(tp, 0x14, 0xc800);
		mdio_write(tp, 0x14, 0xcc00);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xca0f);
		mdio_write(tp, 0x14, 0xcbff);
		mdio_write(tp, 0x14, 0xa901);
		mdio_write(tp, 0x14, 0x8902);
		mdio_write(tp, 0x14, 0xc900);
		mdio_write(tp, 0x14, 0xca00);
		mdio_write(tp, 0x14, 0xcb00);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xb804);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x6044);
		mdio_write(tp, 0x14, 0x9804);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6099);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fa4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xa510);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6098);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fa4);
		mdio_write(tp, 0x14, 0x2ccd);
		mdio_write(tp, 0x14, 0x8510);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x3003);
		mdio_write(tp, 0x14, 0x1cfa);
		mdio_write(tp, 0x14, 0x2d04);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x60be);
		mdio_write(tp, 0x14, 0xe060);
		mdio_write(tp, 0x14, 0x0920);
		mdio_write(tp, 0x14, 0x1ccf);
		mdio_write(tp, 0x14, 0x2c82);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x3063);
		mdio_write(tp, 0x14, 0x1948);
		mdio_write(tp, 0x14, 0x288a);
		mdio_write(tp, 0x14, 0x1ccf);
		mdio_write(tp, 0x14, 0x29bd);
		mdio_write(tp, 0x14, 0xa802);
		mdio_write(tp, 0x14, 0xa303);
		mdio_write(tp, 0x14, 0x843f);
		mdio_write(tp, 0x14, 0x81ff);
		mdio_write(tp, 0x14, 0x8208);
		mdio_write(tp, 0x14, 0xa201);
		mdio_write(tp, 0x14, 0xc001);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x30a0);
		mdio_write(tp, 0x14, 0x0d15);
		mdio_write(tp, 0x14, 0x30a0);
		mdio_write(tp, 0x14, 0x3d0c);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7f4c);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0xe003);
		mdio_write(tp, 0x14, 0x0202);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6090);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fac);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0xa20c);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6091);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fac);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0x820e);
		mdio_write(tp, 0x14, 0xa3e0);
		mdio_write(tp, 0x14, 0xa520);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x609d);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fac);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0x8520);
		mdio_write(tp, 0x14, 0x6703);
		mdio_write(tp, 0x14, 0x2d2d);
		mdio_write(tp, 0x14, 0xa13e);
		mdio_write(tp, 0x14, 0xc001);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x6046);
		mdio_write(tp, 0x14, 0x2d06);
		mdio_write(tp, 0x14, 0xa43f);
		mdio_write(tp, 0x14, 0xa101);
		mdio_write(tp, 0x14, 0xc020);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x3121);
		mdio_write(tp, 0x14, 0x0d3e);
		mdio_write(tp, 0x14, 0x30c0);
		mdio_write(tp, 0x14, 0x3d06);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7f4c);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0xa540);
		mdio_write(tp, 0x14, 0xc001);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4001);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0501);
		mdio_write(tp, 0x14, 0x1da5);
		mdio_write(tp, 0x14, 0xc1c4);
		mdio_write(tp, 0x14, 0xa268);
		mdio_write(tp, 0x14, 0xa303);
		mdio_write(tp, 0x14, 0x8420);
		mdio_write(tp, 0x14, 0xe00f);
		mdio_write(tp, 0x14, 0x0502);
		mdio_write(tp, 0x14, 0x1da5);
		mdio_write(tp, 0x14, 0xc002);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x8208);
		mdio_write(tp, 0x14, 0x8410);
		mdio_write(tp, 0x14, 0xa121);
		mdio_write(tp, 0x14, 0xc002);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x8120);
		mdio_write(tp, 0x14, 0x8180);
		mdio_write(tp, 0x14, 0x1d90);
		mdio_write(tp, 0x14, 0xa180);
		mdio_write(tp, 0x14, 0xa13a);
		mdio_write(tp, 0x14, 0x8240);
		mdio_write(tp, 0x14, 0xa430);
		mdio_write(tp, 0x14, 0xc010);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x30e1);
		mdio_write(tp, 0x14, 0x0abc);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7f8c);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0xa480);
		mdio_write(tp, 0x14, 0xa230);
		mdio_write(tp, 0x14, 0xa303);
		mdio_write(tp, 0x14, 0xc001);
		mdio_write(tp, 0x14, 0xd70c);
		mdio_write(tp, 0x14, 0x4124);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x6120);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x3128);
		mdio_write(tp, 0x14, 0x3d6f);
		mdio_write(tp, 0x14, 0x2d69);
		mdio_write(tp, 0x14, 0xa801);
		mdio_write(tp, 0x14, 0x2d65);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0xe018);
		mdio_write(tp, 0x14, 0x0208);
		mdio_write(tp, 0x14, 0xa1f8);
		mdio_write(tp, 0x14, 0x8480);
		mdio_write(tp, 0x14, 0xc004);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x6046);
		mdio_write(tp, 0x14, 0x2d06);
		mdio_write(tp, 0x14, 0xa43f);
		mdio_write(tp, 0x14, 0xa105);
		mdio_write(tp, 0x14, 0x8228);
		mdio_write(tp, 0x14, 0xc004);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x81bc);
		mdio_write(tp, 0x14, 0xa220);
		mdio_write(tp, 0x14, 0x1d90);
		mdio_write(tp, 0x14, 0x8220);
		mdio_write(tp, 0x14, 0xa1bc);
		mdio_write(tp, 0x14, 0xc040);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x30e1);
		mdio_write(tp, 0x14, 0x0abc);
		mdio_write(tp, 0x14, 0x30e1);
		mdio_write(tp, 0x14, 0x3d06);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7f4c);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0xa802);
		mdio_write(tp, 0x14, 0xd70c);
		mdio_write(tp, 0x14, 0x4244);
		mdio_write(tp, 0x14, 0xa301);
		mdio_write(tp, 0x14, 0xc004);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x3128);
		mdio_write(tp, 0x14, 0x3d9e);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x5f80);
		mdio_write(tp, 0x14, 0xd711);
		mdio_write(tp, 0x14, 0x3109);
		mdio_write(tp, 0x14, 0x3da0);
		mdio_write(tp, 0x14, 0x2da4);
		mdio_write(tp, 0x14, 0xa801);
		mdio_write(tp, 0x14, 0x2d93);
		mdio_write(tp, 0x14, 0xa802);
		mdio_write(tp, 0x14, 0xc004);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x4000);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x14, 0xa510);
		mdio_write(tp, 0x14, 0xd710);
		mdio_write(tp, 0x14, 0x609a);
		mdio_write(tp, 0x14, 0xd71e);
		mdio_write(tp, 0x14, 0x7fac);
		mdio_write(tp, 0x14, 0x2ab6);
		mdio_write(tp, 0x14, 0x8510);
		mdio_write(tp, 0x14, 0x0800);
		mdio_write(tp, 0x13, 0xA01A);
		mdio_write(tp, 0x14, 0x0000);
		mdio_write(tp, 0x13, 0xA006);
		mdio_write(tp, 0x14, 0x0ad6);
		mdio_write(tp, 0x13, 0xA004);
		mdio_write(tp, 0x14, 0x07f5);
		mdio_write(tp, 0x13, 0xA002);
		mdio_write(tp, 0x14, 0x06cc);
		mdio_write(tp, 0x13, 0xA000);
		mdio_write(tp, 0x14, 0xf7db);
		mdio_write(tp, 0x13, 0xB820);
		mdio_write(tp, 0x14, 0x0210);
		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0x83a0);
		mdio_write(tp, 0x14, 0xaf83);
		mdio_write(tp, 0x14, 0xacaf);
		mdio_write(tp, 0x14, 0x83b8);
		mdio_write(tp, 0x14, 0xaf83);
		mdio_write(tp, 0x14, 0xcdaf);
		mdio_write(tp, 0x14, 0x83d3);
		mdio_write(tp, 0x14, 0x0204);
		mdio_write(tp, 0x14, 0x9a02);
		mdio_write(tp, 0x14, 0x09a9);
		mdio_write(tp, 0x14, 0x0284);
		mdio_write(tp, 0x14, 0x61af);
		mdio_write(tp, 0x14, 0x02fc);
		mdio_write(tp, 0x14, 0xad20);
		mdio_write(tp, 0x14, 0x0302);
		mdio_write(tp, 0x14, 0x867c);
		mdio_write(tp, 0x14, 0xad21);
		mdio_write(tp, 0x14, 0x0302);
		mdio_write(tp, 0x14, 0x85c9);
		mdio_write(tp, 0x14, 0xad22);
		mdio_write(tp, 0x14, 0x0302);
		mdio_write(tp, 0x14, 0x1bc0);
		mdio_write(tp, 0x14, 0xaf17);
		mdio_write(tp, 0x14, 0xe302);
		mdio_write(tp, 0x14, 0x8703);
		mdio_write(tp, 0x14, 0xaf18);
		mdio_write(tp, 0x14, 0x6201);
		mdio_write(tp, 0x14, 0x06e0);
		mdio_write(tp, 0x14, 0x8148);
		mdio_write(tp, 0x14, 0xaf3c);
		mdio_write(tp, 0x14, 0x69f8);
		mdio_write(tp, 0x14, 0xf9fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0x10f7);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0x131f);
		mdio_write(tp, 0x14, 0xd104);
		mdio_write(tp, 0x14, 0xbf87);
		mdio_write(tp, 0x14, 0xf302);
		mdio_write(tp, 0x14, 0x4259);
		mdio_write(tp, 0x14, 0x0287);
		mdio_write(tp, 0x14, 0x88bf);
		mdio_write(tp, 0x14, 0x87cf);
		mdio_write(tp, 0x14, 0xd7b8);
		mdio_write(tp, 0x14, 0x22d0);
		mdio_write(tp, 0x14, 0x0c02);
		mdio_write(tp, 0x14, 0x4252);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xcda0);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xce8b);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xd1f5);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xd2a9);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xd30a);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xf010);
		mdio_write(tp, 0x14, 0xee80);
		mdio_write(tp, 0x14, 0xf38f);
		mdio_write(tp, 0x14, 0xee81);
		mdio_write(tp, 0x14, 0x011e);
		mdio_write(tp, 0x14, 0xee81);
		mdio_write(tp, 0x14, 0x0b4a);
		mdio_write(tp, 0x14, 0xee81);
		mdio_write(tp, 0x14, 0x0c7c);
		mdio_write(tp, 0x14, 0xee81);
		mdio_write(tp, 0x14, 0x127f);
		mdio_write(tp, 0x14, 0xd100);
		mdio_write(tp, 0x14, 0x0210);
		mdio_write(tp, 0x14, 0xb5ee);
		mdio_write(tp, 0x14, 0x8088);
		mdio_write(tp, 0x14, 0xa4ee);
		mdio_write(tp, 0x14, 0x8089);
		mdio_write(tp, 0x14, 0x44ee);
		mdio_write(tp, 0x14, 0x809a);
		mdio_write(tp, 0x14, 0xa4ee);
		mdio_write(tp, 0x14, 0x809b);
		mdio_write(tp, 0x14, 0x44ee);
		mdio_write(tp, 0x14, 0x809c);
		mdio_write(tp, 0x14, 0xa7ee);
		mdio_write(tp, 0x14, 0x80a5);
		mdio_write(tp, 0x14, 0xa7d2);
		mdio_write(tp, 0x14, 0x0002);
		mdio_write(tp, 0x14, 0x0e66);
		mdio_write(tp, 0x14, 0x0285);
		mdio_write(tp, 0x14, 0xc0ee);
		mdio_write(tp, 0x14, 0x87fc);
		mdio_write(tp, 0x14, 0x00e0);
		mdio_write(tp, 0x14, 0x8245);
		mdio_write(tp, 0x14, 0xf622);
		mdio_write(tp, 0x14, 0xe482);
		mdio_write(tp, 0x14, 0x45ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfdfc);
		mdio_write(tp, 0x14, 0x0402);
		mdio_write(tp, 0x14, 0x847a);
		mdio_write(tp, 0x14, 0x0284);
		mdio_write(tp, 0x14, 0xb302);
		mdio_write(tp, 0x14, 0x0cab);
		mdio_write(tp, 0x14, 0x020c);
		mdio_write(tp, 0x14, 0xc402);
		mdio_write(tp, 0x14, 0x0cef);
		mdio_write(tp, 0x14, 0x020d);
		mdio_write(tp, 0x14, 0x0802);
		mdio_write(tp, 0x14, 0x0d33);
		mdio_write(tp, 0x14, 0x020c);
		mdio_write(tp, 0x14, 0x3d04);
		mdio_write(tp, 0x14, 0xf8fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xe182);
		mdio_write(tp, 0x14, 0x2fac);
		mdio_write(tp, 0x14, 0x291a);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x24ac);
		mdio_write(tp, 0x14, 0x2102);
		mdio_write(tp, 0x14, 0xae22);
		mdio_write(tp, 0x14, 0x0210);
		mdio_write(tp, 0x14, 0x57f6);
		mdio_write(tp, 0x14, 0x21e4);
		mdio_write(tp, 0x14, 0x8224);
		mdio_write(tp, 0x14, 0xd101);
		mdio_write(tp, 0x14, 0xbf44);
		mdio_write(tp, 0x14, 0xd202);
		mdio_write(tp, 0x14, 0x4259);
		mdio_write(tp, 0x14, 0xae10);
		mdio_write(tp, 0x14, 0x0212);
		mdio_write(tp, 0x14, 0x4cf6);
		mdio_write(tp, 0x14, 0x29e5);
		mdio_write(tp, 0x14, 0x822f);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x24f6);
		mdio_write(tp, 0x14, 0x21e4);
		mdio_write(tp, 0x14, 0x8224);
		mdio_write(tp, 0x14, 0xef96);
		mdio_write(tp, 0x14, 0xfefc);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xe182);
		mdio_write(tp, 0x14, 0x2fac);
		mdio_write(tp, 0x14, 0x2a18);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x24ac);
		mdio_write(tp, 0x14, 0x2202);
		mdio_write(tp, 0x14, 0xae26);
		mdio_write(tp, 0x14, 0x0284);
		mdio_write(tp, 0x14, 0xf802);
		mdio_write(tp, 0x14, 0x8565);
		mdio_write(tp, 0x14, 0xd101);
		mdio_write(tp, 0x14, 0xbf44);
		mdio_write(tp, 0x14, 0xd502);
		mdio_write(tp, 0x14, 0x4259);
		mdio_write(tp, 0x14, 0xae0e);
		mdio_write(tp, 0x14, 0x0284);
		mdio_write(tp, 0x14, 0xea02);
		mdio_write(tp, 0x14, 0x85a9);
		mdio_write(tp, 0x14, 0xe182);
		mdio_write(tp, 0x14, 0x2ff6);
		mdio_write(tp, 0x14, 0x2ae5);
		mdio_write(tp, 0x14, 0x822f);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x24f6);
		mdio_write(tp, 0x14, 0x22e4);
		mdio_write(tp, 0x14, 0x8224);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf9e2);
		mdio_write(tp, 0x14, 0x8011);
		mdio_write(tp, 0x14, 0xad31);
		mdio_write(tp, 0x14, 0x05d2);
		mdio_write(tp, 0x14, 0x0002);
		mdio_write(tp, 0x14, 0x0e66);
		mdio_write(tp, 0x14, 0xfd04);
		mdio_write(tp, 0x14, 0xf8f9);
		mdio_write(tp, 0x14, 0xfaef);
		mdio_write(tp, 0x14, 0x69e0);
		mdio_write(tp, 0x14, 0x8011);
		mdio_write(tp, 0x14, 0xad21);
		mdio_write(tp, 0x14, 0x5cbf);
		mdio_write(tp, 0x14, 0x43be);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97ac);
		mdio_write(tp, 0x14, 0x281b);
		mdio_write(tp, 0x14, 0xbf43);
		mdio_write(tp, 0x14, 0xc102);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0xac28);
		mdio_write(tp, 0x14, 0x12bf);
		mdio_write(tp, 0x14, 0x43c7);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97ac);
		mdio_write(tp, 0x14, 0x2804);
		mdio_write(tp, 0x14, 0xd300);
		mdio_write(tp, 0x14, 0xae07);
		mdio_write(tp, 0x14, 0xd306);
		mdio_write(tp, 0x14, 0xaf85);
		mdio_write(tp, 0x14, 0x56d3);
		mdio_write(tp, 0x14, 0x03e0);
		mdio_write(tp, 0x14, 0x8011);
		mdio_write(tp, 0x14, 0xad26);
		mdio_write(tp, 0x14, 0x25bf);
		mdio_write(tp, 0x14, 0x4559);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97e2);
		mdio_write(tp, 0x14, 0x8073);
		mdio_write(tp, 0x14, 0x0d21);
		mdio_write(tp, 0x14, 0xf637);
		mdio_write(tp, 0x14, 0x0d11);
		mdio_write(tp, 0x14, 0xf62f);
		mdio_write(tp, 0x14, 0x1b21);
		mdio_write(tp, 0x14, 0xaa02);
		mdio_write(tp, 0x14, 0xae10);
		mdio_write(tp, 0x14, 0xe280);
		mdio_write(tp, 0x14, 0x740d);
		mdio_write(tp, 0x14, 0x21f6);
		mdio_write(tp, 0x14, 0x371b);
		mdio_write(tp, 0x14, 0x21aa);
		mdio_write(tp, 0x14, 0x0313);
		mdio_write(tp, 0x14, 0xae02);
		mdio_write(tp, 0x14, 0x2b02);
		mdio_write(tp, 0x14, 0x020e);
		mdio_write(tp, 0x14, 0x5102);
		mdio_write(tp, 0x14, 0x0e66);
		mdio_write(tp, 0x14, 0x020f);
		mdio_write(tp, 0x14, 0xa3ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfdfc);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xf9fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xe080);
		mdio_write(tp, 0x14, 0x12ad);
		mdio_write(tp, 0x14, 0x2733);
		mdio_write(tp, 0x14, 0xbf43);
		mdio_write(tp, 0x14, 0xbe02);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0xac28);
		mdio_write(tp, 0x14, 0x09bf);
		mdio_write(tp, 0x14, 0x43c1);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97ad);
		mdio_write(tp, 0x14, 0x2821);
		mdio_write(tp, 0x14, 0xbf45);
		mdio_write(tp, 0x14, 0x5902);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0xe387);
		mdio_write(tp, 0x14, 0xffd2);
		mdio_write(tp, 0x14, 0x001b);
		mdio_write(tp, 0x14, 0x45ac);
		mdio_write(tp, 0x14, 0x2711);
		mdio_write(tp, 0x14, 0xe187);
		mdio_write(tp, 0x14, 0xfebf);
		mdio_write(tp, 0x14, 0x87e4);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x590d);
		mdio_write(tp, 0x14, 0x11bf);
		mdio_write(tp, 0x14, 0x87e7);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfdfc);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xfaef);
		mdio_write(tp, 0x14, 0x69d1);
		mdio_write(tp, 0x14, 0x00bf);
		mdio_write(tp, 0x14, 0x87e4);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59bf);
		mdio_write(tp, 0x14, 0x87e7);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xee87);
		mdio_write(tp, 0x14, 0xff46);
		mdio_write(tp, 0x14, 0xee87);
		mdio_write(tp, 0x14, 0xfe01);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xfaef);
		mdio_write(tp, 0x14, 0x69e0);
		mdio_write(tp, 0x14, 0x8241);
		mdio_write(tp, 0x14, 0xa000);
		mdio_write(tp, 0x14, 0x0502);
		mdio_write(tp, 0x14, 0x85eb);
		mdio_write(tp, 0x14, 0xae0e);
		mdio_write(tp, 0x14, 0xa001);
		mdio_write(tp, 0x14, 0x0502);
		mdio_write(tp, 0x14, 0x1a5a);
		mdio_write(tp, 0x14, 0xae06);
		mdio_write(tp, 0x14, 0xa002);
		mdio_write(tp, 0x14, 0x0302);
		mdio_write(tp, 0x14, 0x1ae6);
		mdio_write(tp, 0x14, 0xef96);
		mdio_write(tp, 0x14, 0xfefc);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xf9fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x29f6);
		mdio_write(tp, 0x14, 0x21e4);
		mdio_write(tp, 0x14, 0x8229);
		mdio_write(tp, 0x14, 0xe080);
		mdio_write(tp, 0x14, 0x10ac);
		mdio_write(tp, 0x14, 0x2202);
		mdio_write(tp, 0x14, 0xae76);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x27f7);
		mdio_write(tp, 0x14, 0x21e4);
		mdio_write(tp, 0x14, 0x8227);
		mdio_write(tp, 0x14, 0xbf43);
		mdio_write(tp, 0x14, 0x1302);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0xef21);
		mdio_write(tp, 0x14, 0xbf43);
		mdio_write(tp, 0x14, 0x1602);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0x0c11);
		mdio_write(tp, 0x14, 0x1e21);
		mdio_write(tp, 0x14, 0xbf43);
		mdio_write(tp, 0x14, 0x1902);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0x0c12);
		mdio_write(tp, 0x14, 0x1e21);
		mdio_write(tp, 0x14, 0xe682);
		mdio_write(tp, 0x14, 0x43a2);
		mdio_write(tp, 0x14, 0x000a);
		mdio_write(tp, 0x14, 0xe182);
		mdio_write(tp, 0x14, 0x27f6);
		mdio_write(tp, 0x14, 0x29e5);
		mdio_write(tp, 0x14, 0x8227);
		mdio_write(tp, 0x14, 0xae42);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x44f7);
		mdio_write(tp, 0x14, 0x21e4);
		mdio_write(tp, 0x14, 0x8244);
		mdio_write(tp, 0x14, 0x0246);
		mdio_write(tp, 0x14, 0xaebf);
		mdio_write(tp, 0x14, 0x4325);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97ef);
		mdio_write(tp, 0x14, 0x21bf);
		mdio_write(tp, 0x14, 0x431c);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x970c);
		mdio_write(tp, 0x14, 0x121e);
		mdio_write(tp, 0x14, 0x21bf);
		mdio_write(tp, 0x14, 0x431f);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x970c);
		mdio_write(tp, 0x14, 0x131e);
		mdio_write(tp, 0x14, 0x21bf);
		mdio_write(tp, 0x14, 0x4328);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x970c);
		mdio_write(tp, 0x14, 0x141e);
		mdio_write(tp, 0x14, 0x21bf);
		mdio_write(tp, 0x14, 0x44b1);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x970c);
		mdio_write(tp, 0x14, 0x161e);
		mdio_write(tp, 0x14, 0x21e6);
		mdio_write(tp, 0x14, 0x8242);
		mdio_write(tp, 0x14, 0xee82);
		mdio_write(tp, 0x14, 0x4101);
		mdio_write(tp, 0x14, 0xef96);
		mdio_write(tp, 0x14, 0xfefd);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf8fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x46a0);
		mdio_write(tp, 0x14, 0x0005);
		mdio_write(tp, 0x14, 0x0286);
		mdio_write(tp, 0x14, 0x96ae);
		mdio_write(tp, 0x14, 0x06a0);
		mdio_write(tp, 0x14, 0x0103);
		mdio_write(tp, 0x14, 0x0219);
		mdio_write(tp, 0x14, 0x19ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf8fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x29f6);
		mdio_write(tp, 0x14, 0x20e4);
		mdio_write(tp, 0x14, 0x8229);
		mdio_write(tp, 0x14, 0xe080);
		mdio_write(tp, 0x14, 0x10ac);
		mdio_write(tp, 0x14, 0x2102);
		mdio_write(tp, 0x14, 0xae54);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x27f7);
		mdio_write(tp, 0x14, 0x20e4);
		mdio_write(tp, 0x14, 0x8227);
		mdio_write(tp, 0x14, 0xbf42);
		mdio_write(tp, 0x14, 0xe602);
		mdio_write(tp, 0x14, 0x4297);
		mdio_write(tp, 0x14, 0xac28);
		mdio_write(tp, 0x14, 0x22bf);
		mdio_write(tp, 0x14, 0x430d);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97e5);
		mdio_write(tp, 0x14, 0x8247);
		mdio_write(tp, 0x14, 0xac28);
		mdio_write(tp, 0x14, 0x20d1);
		mdio_write(tp, 0x14, 0x03bf);
		mdio_write(tp, 0x14, 0x4307);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59ee);
		mdio_write(tp, 0x14, 0x8246);
		mdio_write(tp, 0x14, 0x00e1);
		mdio_write(tp, 0x14, 0x8227);
		mdio_write(tp, 0x14, 0xf628);
		mdio_write(tp, 0x14, 0xe582);
		mdio_write(tp, 0x14, 0x27ae);
		mdio_write(tp, 0x14, 0x21d1);
		mdio_write(tp, 0x14, 0x04bf);
		mdio_write(tp, 0x14, 0x4307);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59ae);
		mdio_write(tp, 0x14, 0x08d1);
		mdio_write(tp, 0x14, 0x05bf);
		mdio_write(tp, 0x14, 0x4307);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59e0);
		mdio_write(tp, 0x14, 0x8244);
		mdio_write(tp, 0x14, 0xf720);
		mdio_write(tp, 0x14, 0xe482);
		mdio_write(tp, 0x14, 0x4402);
		mdio_write(tp, 0x14, 0x46ae);
		mdio_write(tp, 0x14, 0xee82);
		mdio_write(tp, 0x14, 0x4601);
		mdio_write(tp, 0x14, 0xef96);
		mdio_write(tp, 0x14, 0xfefc);
		mdio_write(tp, 0x14, 0x04f8);
		mdio_write(tp, 0x14, 0xfaef);
		mdio_write(tp, 0x14, 0x69e0);
		mdio_write(tp, 0x14, 0x8013);
		mdio_write(tp, 0x14, 0xad24);
		mdio_write(tp, 0x14, 0x1cbf);
		mdio_write(tp, 0x14, 0x87f0);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x97ad);
		mdio_write(tp, 0x14, 0x2813);
		mdio_write(tp, 0x14, 0xe087);
		mdio_write(tp, 0x14, 0xfca0);
		mdio_write(tp, 0x14, 0x0005);
		mdio_write(tp, 0x14, 0x0287);
		mdio_write(tp, 0x14, 0x36ae);
		mdio_write(tp, 0x14, 0x10a0);
		mdio_write(tp, 0x14, 0x0105);
		mdio_write(tp, 0x14, 0x0287);
		mdio_write(tp, 0x14, 0x48ae);
		mdio_write(tp, 0x14, 0x08e0);
		mdio_write(tp, 0x14, 0x8230);
		mdio_write(tp, 0x14, 0xf626);
		mdio_write(tp, 0x14, 0xe482);
		mdio_write(tp, 0x14, 0x30ef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf8e0);
		mdio_write(tp, 0x14, 0x8245);
		mdio_write(tp, 0x14, 0xf722);
		mdio_write(tp, 0x14, 0xe482);
		mdio_write(tp, 0x14, 0x4502);
		mdio_write(tp, 0x14, 0x46ae);
		mdio_write(tp, 0x14, 0xee87);
		mdio_write(tp, 0x14, 0xfc01);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf8fa);
		mdio_write(tp, 0x14, 0xef69);
		mdio_write(tp, 0x14, 0xfb02);
		mdio_write(tp, 0x14, 0x46d3);
		mdio_write(tp, 0x14, 0xad50);
		mdio_write(tp, 0x14, 0x2fbf);
		mdio_write(tp, 0x14, 0x87ed);
		mdio_write(tp, 0x14, 0xd101);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59bf);
		mdio_write(tp, 0x14, 0x87ed);
		mdio_write(tp, 0x14, 0xd100);
		mdio_write(tp, 0x14, 0x0242);
		mdio_write(tp, 0x14, 0x59e0);
		mdio_write(tp, 0x14, 0x8245);
		mdio_write(tp, 0x14, 0xf622);
		mdio_write(tp, 0x14, 0xe482);
		mdio_write(tp, 0x14, 0x4502);
		mdio_write(tp, 0x14, 0x46ae);
		mdio_write(tp, 0x14, 0xd100);
		mdio_write(tp, 0x14, 0xbf87);
		mdio_write(tp, 0x14, 0xf002);
		mdio_write(tp, 0x14, 0x4259);
		mdio_write(tp, 0x14, 0xee87);
		mdio_write(tp, 0x14, 0xfc00);
		mdio_write(tp, 0x14, 0xe082);
		mdio_write(tp, 0x14, 0x30f6);
		mdio_write(tp, 0x14, 0x26e4);
		mdio_write(tp, 0x14, 0x8230);
		mdio_write(tp, 0x14, 0xffef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xfc04);
		mdio_write(tp, 0x14, 0xf8f9);
		mdio_write(tp, 0x14, 0xface);
		mdio_write(tp, 0x14, 0xfaef);
		mdio_write(tp, 0x14, 0x69fb);
		mdio_write(tp, 0x14, 0xbf87);
		mdio_write(tp, 0x14, 0xb3d7);
		mdio_write(tp, 0x14, 0x001c);
		mdio_write(tp, 0x14, 0xd819);
		mdio_write(tp, 0x14, 0xd919);
		mdio_write(tp, 0x14, 0xda19);
		mdio_write(tp, 0x14, 0xdb19);
		mdio_write(tp, 0x14, 0x07ef);
		mdio_write(tp, 0x14, 0x9502);
		mdio_write(tp, 0x14, 0x4259);
		mdio_write(tp, 0x14, 0x073f);
		mdio_write(tp, 0x14, 0x0004);
		mdio_write(tp, 0x14, 0x9fec);
		mdio_write(tp, 0x14, 0xffef);
		mdio_write(tp, 0x14, 0x96fe);
		mdio_write(tp, 0x14, 0xc6fe);
		mdio_write(tp, 0x14, 0xfdfc);
		mdio_write(tp, 0x14, 0x0400);
		mdio_write(tp, 0x14, 0x0145);
		mdio_write(tp, 0x14, 0x7d00);
		mdio_write(tp, 0x14, 0x0345);
		mdio_write(tp, 0x14, 0x5c00);
		mdio_write(tp, 0x14, 0x0143);
		mdio_write(tp, 0x14, 0x4f00);
		mdio_write(tp, 0x14, 0x0387);
		mdio_write(tp, 0x14, 0xdb00);
		mdio_write(tp, 0x14, 0x0987);
		mdio_write(tp, 0x14, 0xde00);
		mdio_write(tp, 0x14, 0x0987);
		mdio_write(tp, 0x14, 0xe100);
		mdio_write(tp, 0x14, 0x0087);
		mdio_write(tp, 0x14, 0xeaa4);
		mdio_write(tp, 0x14, 0x00b8);
		mdio_write(tp, 0x14, 0x20c4);
		mdio_write(tp, 0x14, 0x1600);
		mdio_write(tp, 0x14, 0x000f);
		mdio_write(tp, 0x14, 0xf800);
		mdio_write(tp, 0x14, 0x7098);
		mdio_write(tp, 0x14, 0xa58a);
		mdio_write(tp, 0x14, 0xb6a8);
		mdio_write(tp, 0x14, 0x3e50);
		mdio_write(tp, 0x14, 0xa83e);
		mdio_write(tp, 0x14, 0x33bc);
		mdio_write(tp, 0x14, 0xc622);
		mdio_write(tp, 0x14, 0xbcc6);
		mdio_write(tp, 0x14, 0xaaa4);
		mdio_write(tp, 0x14, 0x42ff);
		mdio_write(tp, 0x14, 0xc408);
		mdio_write(tp, 0x14, 0x00c4);
		mdio_write(tp, 0x14, 0x16a8);
		mdio_write(tp, 0x14, 0xbcc0);
		mdio_write(tp, 0x13, 0xb818);
		mdio_write(tp, 0x14, 0x02f3);
		mdio_write(tp, 0x13, 0xb81a);
		mdio_write(tp, 0x14, 0x17d1);
		mdio_write(tp, 0x13, 0xb81c);
		mdio_write(tp, 0x14, 0x185a);
		mdio_write(tp, 0x13, 0xb81e);
		mdio_write(tp, 0x14, 0x3c66);
		mdio_write(tp, 0x13, 0xb820);
		mdio_write(tp, 0x14, 0x021f);
		mdio_write(tp, 0x13, 0xc416);
		mdio_write(tp, 0x14, 0x0500);
		mdio_write(tp, 0x13, 0xb82e);
		mdio_write(tp, 0x14, 0xfffc);
		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0x0000);
		mdio_write(tp, 0x14, 0x0000);
		mdio_write(tp, 0x1f, 0x0B82);
		gphy_val = mdio_read(tp, 0x10);
		gphy_val &= ~(BIT_9);
		mdio_write(tp, 0x10, gphy_val);
		mdio_write(tp, 0x1f, 0x0A43);
		mdio_write(tp, 0x13, 0x8146);
		mdio_write(tp, 0x14, 0x0000);
		mdio_write(tp, 0x1f, 0x0B82);
		gphy_val = mdio_read(tp, 0x10);
		gphy_val &= ~(BIT_4);
		mdio_write(tp, 0x10, gphy_val);

		mdio_write(tp, 0x1F, 0x0A46);
		gphy_val = mdio_read(tp, 0x10);
		mdio_write(tp, 0x1F, 0x0BCC);
		if(gphy_val & BIT_8)
			gphy_val = mdio_read(tp, 0x12) & ~BIT_15;
		else
			gphy_val = mdio_read(tp, 0x12) | BIT_15;
		mdio_write(tp, 0x1F, 0x0A46);
		gphy_val = mdio_read(tp, 0x13);
		mdio_write(tp, 0x1F, 0x0C41);
		if(gphy_val & BIT_8)
			gphy_val = mdio_read(tp, 0x15) | BIT_1;
		else
			gphy_val = mdio_read(tp, 0x15) & ~BIT_1;

		mdio_write(tp, 0x1F, 0x0A44);
		mdio_write(tp, 0x11, mdio_read(tp, 0x11) | BIT_2 | BIT_3);

		mdio_write(tp, 0x1F, 0x0BCC);
		mdio_write(tp, 0x14, mdio_read(tp, 0x14) & ~BIT_8);
		mdio_write(tp, 0x1F, 0x0A44);
		mdio_write(tp, 0x11, mdio_read(tp, 0x11) | BIT_7);
		mdio_write(tp, 0x11, mdio_read(tp, 0x11) | BIT_6);
		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0x8084);
		mdio_write(tp, 0x14, mdio_read(tp, 0x14) & ~(BIT_14 | BIT_13));
		mdio_write(tp, 0x10, mdio_read(tp, 0x10) | BIT_12);
		mdio_write(tp, 0x10, mdio_read(tp, 0x10) | BIT_1);
		mdio_write(tp, 0x10, mdio_read(tp, 0x10) | BIT_0);

		mdio_write(tp, 0x1F, 0x0A4B);
		mdio_write(tp, 0x11, mdio_read(tp, 0x11) | BIT_2);

		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0x8012);
		mdio_write(tp, 0x14, mdio_read(tp, 0x14) | BIT_15);

		mdio_write(tp, 0x1F, 0x0C42);
		gphy_val = mdio_read(tp, 0x11);
		gphy_val |= BIT_14;
		gphy_val &= ~BIT_13;
		mdio_write(tp, 0x11, gphy_val);

		mdio_write(tp, 0x1F, 0x0A43);
		mdio_write(tp, 0x13, 0x809A);
		mdio_write(tp, 0x14, 0x8022);
		mdio_write(tp, 0x13, 0x80A0);
		gphy_val = mdio_read(tp, 0x14) & 0x00FF;
		gphy_val |= 0x1000;
		mdio_write(tp, 0x14, gphy_val);
		mdio_write(tp, 0x13, 0x8088);
		mdio_write(tp, 0x14, 0x9222);

		mdio_write(tp, 0x1F, 0x0BCD);
		mdio_write(tp, 0x14, 0x5065);
		mdio_write(tp, 0x14, 0xD065);
		mdio_write(tp, 0x1F, 0x0BC8);
		mdio_write(tp, 0x11, 0x5655);
		mdio_write(tp, 0x1F, 0x0BCD);
		mdio_write(tp, 0x14, 0x1065);
		mdio_write(tp, 0x14, 0x9065);
		mdio_write(tp, 0x14, 0x1065);

		if(aspm) {
			mdio_write(tp, 0x1F, 0x0A43);
			mdio_write(tp, 0x10, mdio_read(tp, 0x10) | BIT_2);
		}

		mdio_write(tp, 0x1F, 0x0000);
	}

	mdio_write(tp, 0x1F, 0x0000);


	if(eee_enable == 1)
		rtl8168_enable_EEE(tp);
	else
		rtl8168_disable_EEE(tp);
}


static void
rtl8168dp_10mbps_gphy_para(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	u8 status = RTL_R8(PHYstatus);

	if((status & LinkStatus) && (status & _10bps)) {
		mdio_write(tp, 0x1f, 0x0000);
		mdio_write(tp, 0x10, 0x04EE);
	} else {
		mdio_write(tp, 0x1f, 0x0000);
		mdio_write(tp, 0x10, 0x01EE);
	}
}
#define MII_CTRL1000        0x09    /* 1000BASE-T control          */
/* 1000BASE-T Control register */
#define ADVERTISE_1000FULL  0x0200  /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF  0x0100  /* Advertise 1000BASE-T half duplex */
#define CTL1000_AS_MASTER   0x0800
#define CTL1000_ENABLE_MASTER   0x1000
#define ADVERTISE_PAUSE_CAP 0x0400  /* Try for pause               */
#define ADVERTISE_PAUSE_ASYM    0x0800  /* Try for asymetric pause     */

static int
rtl8168_set_speed_xmii(struct eth_device *dev,
                       u8 autoneg,
                       u16 speed,
                       u8 duplex)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int auto_nego = 0;
	int giga_ctrl = 0;
	int bmcr_true_force = 0;

	if((speed != SPEED_1000) &&
	   (speed != SPEED_100) &&
	   (speed != SPEED_10)) {
		speed = SPEED_1000;
		duplex = DUPLEX_FULL;
	}

	if((autoneg == AUTONEG_ENABLE) || (speed == SPEED_1000)) {
		/*n-way force*/
		if((speed == SPEED_10) && (duplex == DUPLEX_HALF)) {
			auto_nego |= ADVERTISE_10HALF;
		} else if((speed == SPEED_10) && (duplex == DUPLEX_FULL)) {
			auto_nego |= ADVERTISE_10HALF |
			             ADVERTISE_10FULL;
		} else if((speed == SPEED_100) && (duplex == DUPLEX_HALF)) {
			auto_nego |= ADVERTISE_100HALF |
			             ADVERTISE_10HALF |
			             ADVERTISE_10FULL;
		} else if((speed == SPEED_100) && (duplex == DUPLEX_FULL)) {
			auto_nego |= ADVERTISE_100HALF |
			             ADVERTISE_100FULL |
			             ADVERTISE_10HALF |
			             ADVERTISE_10FULL;
		} else if(speed == SPEED_1000) {
			giga_ctrl |= ADVERTISE_1000HALF |
			             ADVERTISE_1000FULL;

			auto_nego |= ADVERTISE_100HALF |
			             ADVERTISE_100FULL |
			             ADVERTISE_10HALF |
			             ADVERTISE_10FULL;
		}

		//flow contorol
		auto_nego |= ADVERTISE_PAUSE_CAP | ADVERTISE_PAUSE_ASYM;

		tp->phy_auto_nego_reg = auto_nego;
		tp->phy_1000_ctrl_reg = giga_ctrl;

		mdio_write(tp, 0x1f, 0x0000);
		mdio_write(tp, MII_ADVERTISE, auto_nego);
		mdio_write(tp, MII_CTRL1000, giga_ctrl);
		mdio_write(tp, MII_BMCR, BMCR_RESET | BMCR_ANENABLE | BMCR_ANRESTART);
		mdelay(20);
	} else {
		/*true force*/
#ifndef BMCR_SPEED100
#define BMCR_SPEED100   0x0040
#endif

#ifndef BMCR_SPEED10
#define BMCR_SPEED10    0x0000
#endif
		if((speed == SPEED_10) && (duplex == DUPLEX_HALF)) {
			bmcr_true_force = BMCR_SPEED10;
		} else if((speed == SPEED_10) && (duplex == DUPLEX_FULL)) {
			bmcr_true_force = BMCR_SPEED10 | BMCR_FULLDPLX;
		} else if((speed == SPEED_100) && (duplex == DUPLEX_HALF)) {
			bmcr_true_force = BMCR_SPEED100;
		} else if((speed == SPEED_100) && (duplex == DUPLEX_FULL)) {
			bmcr_true_force = BMCR_SPEED100 | BMCR_FULLDPLX;
		}

		mdio_write(tp, 0x1f, 0x0000);
		mdio_write(tp, MII_BMCR, bmcr_true_force);
	}

	tp->autoneg = autoneg;
	tp->speed = speed;
	tp->duplex = duplex;

	if(tp->mcfg == CFG_METHOD_11)
		rtl8168dp_10mbps_gphy_para(dev);

	return 0;
}


static int
rtl8168_set_speed(struct eth_device *dev,
                  u8 autoneg,
                  u16 speed,
                  u8 duplex)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int ret;

	ret = tp->set_speed(dev, autoneg, speed, duplex);

	return ret;
}


static void
rtl8168_link_option(
    u8 *aut,
    u16 *spd,
    u8 *dup)
{

	if((*spd != SPEED_1000) && (*spd != SPEED_100) && (*spd != SPEED_10))
		*spd = SPEED_1000;

	if((*dup != DUPLEX_FULL) && (*dup != DUPLEX_HALF))
		*dup = DUPLEX_FULL;

	if((*aut != AUTONEG_ENABLE) && (*aut != AUTONEG_DISABLE))
		*aut = AUTONEG_ENABLE;
}

static void
rtl8168_init_sequence(struct rtl8168_private *tp)
{
	u16 data16;
	int i;

	switch(tp->mcfg) {
	case CFG_METHOD_21:
	case CFG_METHOD_22:
	case CFG_METHOD_23:
	case CFG_METHOD_24:
		RTL_W32(RxConfig, 0xcf00);
		RTL_W8(0xF2, RTL_R8(0xF2) | BIT_3);
		for(i = 0; i < 10; i++) {
			udelay(100);
			if(RTL_R32(TxConfig) & BIT_11) {
				if((RTL_R8(MCUCmd_reg) & Txfifo_empty) &&
				   (RTL_R8(MCUCmd_reg) & Rxfifo_empty))
					break;
			}
		}
		RTL_W8(ChipCmd, RTL_R8(ChipCmd) & ~(CmdRxEnb | CmdTxEnb));
		mdelay(1);

		RTL_W8(MCUCmd_reg, RTL_R8(MCUCmd_reg) & ~Now_is_oob);

		data16 = mac_ocp_read(tp, 0xE8DE) & ~BIT_14;
		mac_ocp_write(tp, 0xE8DE, data16);
		for(i = 0; i < 10; i++) {
			udelay(100);
			if(RTL_R16(0xD2) & BIT_9)
				break;
		}

		data16 = mac_ocp_read(tp, 0xE8DE) | BIT_15;
		mac_ocp_write(tp, 0xE8DE, data16);
		for(i = 0; i < 10; i++) {
			udelay(100);
			if(RTL_R16(0xD2) & BIT_9)
				break;
		}

		RTL_W8(ChipCmd, CmdReset);
		break;
	}
}


static void
rtl8168_set_rxbufsize(struct rtl8168_private *tp,
                      struct eth_device *dev)
{
	unsigned int mtu = ETH_DATA_LEN;

	tp->rx_buf_sz = (mtu > ETH_DATA_LEN) ? mtu + ETH_HLEN + 8 + 1 : RX_BUF_SIZE;

	RTL_W16(RxMaxSize, tp->rx_buf_sz);
}

static inline void
rtl8168_mark_as_last_descriptor(struct RxDesc *desc)
{
	desc->opts1 |= cpu_to_le32(RingEnd);
}
void rtl8168_init_ring_indexes(struct rtl8168_private *tp)
{
	tp->dirty_tx = 0;
	tp->dirty_rx = 0;
	tp->cur_tx = 0;
	tp->cur_rx = 0;
}

static void
rtl8168_tx_desc_init(struct rtl8168_private *tp)
{
	int i = 0;

	memset(tp->TxDescArray, 0x0, NUM_TX_DESC * sizeof(struct TxDesc));

	for(i = 0; i < NUM_TX_DESC; i++) {
		if(i == (NUM_TX_DESC - 1))
			tp->TxDescArray[i].opts1 = cpu_to_le32(RingEnd);
	}
}

static void
rtl8168_rx_desc_init(struct rtl8168_private *tp)
{
	memset(tp->RxDescArray, 0x0, NUM_RX_DESC * sizeof(struct RxDesc));
}

static inline void
rtl8168_mark_to_asic(struct RxDesc *desc,
                     u32 rx_buf_sz)
{
	u32 eor = le32_to_cpu(desc->opts1) & RingEnd;

	desc->opts1 = cpu_to_le32(DescOwn | eor | rx_buf_sz);
}


static u32
rtl8168_rx_fill(struct rtl8168_private *tp,
                struct eth_device *dev,
                u32 start,
                u32 end)
{
	u32 cur;

	for(cur = start; end - cur > 0; cur++) {
		int  i = cur % NUM_RX_DESC;

		if(tp->Rx_skbuff[i])
			continue;
		tp->Rx_skbuff[i] =  &rxb[i * 2048];
		tp->RxDescArray[i].addr = (u32)(tp->Rx_skbuff[i]);
		rtl8168_mark_to_asic(tp->RxDescArray + i, tp->rx_buf_sz);
		net_inval_cache((unsigned long)tp->Rx_skbuff[i], 2048);
	}
	return cur - start;
}

static int
rtl8168_init_ring(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int i;
	rtl8168_init_ring_indexes(tp);

	memset(tp->tx_skb, 0x0, NUM_TX_DESC * sizeof(struct ring_info));
	memset(tp->Rx_skbuff, 0x0, NUM_RX_DESC * sizeof(unsigned char *));

	rtl8168_tx_desc_init(tp);
	rtl8168_rx_desc_init(tp);

	for(i = 0; i < NUM_TX_DESC; i++) {
		tp->tx_skb[i].skb = &txb[i * RX_BUF_SIZE];
	}

	rtl8168_rx_fill(tp, dev, 0, NUM_RX_DESC);

	rtl8168_mark_as_last_descriptor(tp->RxDescArray + NUM_RX_DESC - 1);

	return 0;
}

static void
rtl8168_set_rx_mode(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	u32 mc_filter[2];   /* Multicast hash filter */
	int rx_mode;
	u32 tmp = 0;
	rx_mode = AcceptBroadcast | AcceptMulticast | AcceptMyPhys;
	mc_filter[1] = mc_filter[0] = 0xffffffff;

	if(rx_mode & AcceptPROMISC) {
		rx_mode =
		    AcceptBroadcast | AcceptMulticast | AcceptMyPhys |
		    AcceptAllPhys;
		mc_filter[1] = mc_filter[0] = 0xffffffff;
	}


	tmp = mc_filter[0];
	mc_filter[0] = swab32(mc_filter[1]);
	mc_filter[1] = swab32(tmp);

	tp->rtl8168_rx_config = rtl_chip_info[tp->chipset].RCR_Cfg;
	tmp = tp->rtl8168_rx_config | rx_mode | (RTL_R32(RxConfig) & rtl_chip_info[tp->chipset].RxConfigMask);

	RTL_W32(RxConfig, tmp);
	RTL_W32(MAR0 + 0, mc_filter[0]);
	RTL_W32(MAR0 + 4, mc_filter[1]);

}

static void rtl8168_powerup_pll(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	switch(tp->mcfg) {
	case CFG_METHOD_21:
	case CFG_METHOD_22:
	case CFG_METHOD_24:
		RTL_W8(PMCH, RTL_R8(PMCH) | BIT_6);
		break;
	}

	rtl8168_phy_power_up(dev);
	rtl8168_set_speed(dev, tp->autoneg, tp->speed, tp->duplex);
}

static void
rtl8168_nic_reset(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int i;

	RTL_W32(RxConfig, RTL_R32(RxConfig) &
	        ~(AcceptErr | AcceptRunt | AcceptBroadcast | AcceptMulticast |
	          AcceptMyPhys |  AcceptAllPhys));

	switch(tp->mcfg) {
	case CFG_METHOD_1:
	case CFG_METHOD_2:
	case CFG_METHOD_3:
		break;
	case CFG_METHOD_4:
	case CFG_METHOD_5:
	case CFG_METHOD_6:
	case CFG_METHOD_7:
	case CFG_METHOD_8:
	case CFG_METHOD_9:
	case CFG_METHOD_10:
	case CFG_METHOD_14:
	case CFG_METHOD_15:
		RTL_W8(ChipCmd, StopReq | CmdRxEnb | CmdTxEnb);
		udelay(100);
		break;
	case CFG_METHOD_11:
	case CFG_METHOD_12:
	case CFG_METHOD_13:
		while(RTL_R8(TxPoll) & NPQ)
			udelay(20);
		break;
	case CFG_METHOD_23:
		RTL_W8(0xF2, RTL_R8(0xF2) | BIT_3);
		mdelay(2);
		for(i = 0; i < 10; i++) {
			if((RTL_R8(MCUCmd_reg) & (Txfifo_empty | Rxfifo_empty)) == (Txfifo_empty | Rxfifo_empty))
				break;
			udelay(100);
		}
		mdelay(2);
		break;
	default:
		mdelay(10);
		RTL_W8(ChipCmd, StopReq | CmdRxEnb | CmdTxEnb);
		while(!(RTL_R32(TxConfig) & BIT_11)) udelay(100);
		break;
	}

	/* Soft reset the chip. */
	RTL_W8(ChipCmd, CmdReset);

	/* Check that the chip has finished the reset. */
	for(i = 100; i > 0; i--) {
		udelay(100);
		if((RTL_R8(ChipCmd) & CmdReset) == 0)
			break;
	}

}

static void
rtl8168_rx_desc_offset0_init(struct rtl8168_private *tp, int own)
{
	int i = 0;
	int ownbit = 0;

	if(own)
		ownbit = DescOwn;

	for(i = 0; i < NUM_RX_DESC; i++) {
		if(i == (NUM_RX_DESC - 1))
			tp->RxDescArray[i].opts1 = cpu_to_le32((ownbit | RingEnd) | (unsigned long) tp->rx_buf_sz);
		else
			tp->RxDescArray[i].opts1 = cpu_to_le32(ownbit | (unsigned long) tp->rx_buf_sz);
	}
	net_flush_cache((unsigned long)tp->RxDescArray, R8168_RX_RING_BYTES);

}

static void
set_offset70F(struct rtl8168_private *tp, u8 setting)
{

	u32 csi_tmp;
	u32 temp = (u32) setting;
	temp = temp << 24;
	/* set PCI configuration space offset 0x70F to setting
	 * When the register offset of PCI configuration space 
	 * larger than 0xff, use CSI to access it.
	 */

	csi_tmp = rtl8168_csi_read(tp, 0x70c) & 0x00ffffff;
	rtl8168_csi_write(tp, 0x70c, csi_tmp | temp);
}

static void
set_offset79(struct rtl8168_private *tp, u8 setting)
{
	//Set PCI configuration space offset 0x79 to setting

	u8 device_control;

	pci_read_config_byte(deviceid, 0x79, &device_control);
	device_control &= ~0x70;
	device_control |= setting;
	pci_write_config_byte(deviceid, 0x79, device_control);

}

static void
rtl8168_desc_addr_fill(struct rtl8168_private *tp)
{

	if(!tp->TxPhyAddr || !tp->RxPhyAddr)
		return;

	RTL_W32(TxDescStartAddrLow, ((u64) tp->TxPhyAddr & DMA_BIT_MASK(32)));
	RTL_W32(TxDescStartAddrHigh, ((u64) tp->TxPhyAddr >> 32));
	RTL_W32(RxDescAddrLow, ((u64) tp->RxPhyAddr & DMA_BIT_MASK(32)));
	RTL_W32(RxDescAddrHigh, ((u64) tp->RxPhyAddr >> 32));
}

static void
rtl8168_dsm(struct eth_device *dev, int dev_state)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	switch(dev_state) {
	case DSM_MAC_INIT:
		if((tp->mcfg == CFG_METHOD_5) || (tp->mcfg == CFG_METHOD_6)) {
			if(RTL_R8(MACDBG) & 0x80)
				RTL_W8(GPIO, RTL_R8(GPIO) | GPIO_en);
			else
				RTL_W8(GPIO, RTL_R8(GPIO) & ~GPIO_en);
		}

		break;
	case DSM_NIC_GOTO_D3:
	case DSM_IF_DOWN:
		if((tp->mcfg == CFG_METHOD_5) || (tp->mcfg == CFG_METHOD_6)) {
			if(RTL_R8(MACDBG) & 0x80)
				RTL_W8(GPIO, RTL_R8(GPIO) & ~GPIO_en);
		}
		break;

	case DSM_NIC_RESUME_D3:
	case DSM_IF_UP:
		if((tp->mcfg == CFG_METHOD_5) || (tp->mcfg == CFG_METHOD_6)) {
			if(RTL_R8(MACDBG) & 0x80)
				RTL_W8(GPIO, RTL_R8(GPIO) | GPIO_en);
		}

		break;
	}

}

static void
rtl8168_hw_start(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	void __iomem *ioaddr = tp->mmio_addr;
	u8 device_control, options1, options2;
	u16 ephy_data;
	u32 csi_tmp;

	switch(tp->mcfg) {
	case CFG_METHOD_1:
	case CFG_METHOD_2:
	case CFG_METHOD_3:
	case CFG_METHOD_4:
	case CFG_METHOD_5:
	case CFG_METHOD_6:
	case CFG_METHOD_7:
	case CFG_METHOD_8:
	case CFG_METHOD_9:
	case CFG_METHOD_10:
	case CFG_METHOD_11:
	case CFG_METHOD_12:
	case CFG_METHOD_13:
	case CFG_METHOD_14:
	case CFG_METHOD_15:
		break;
	default:
		RTL_W32(RxConfig, RxCfg_128_int_en | (RX_DMA_BURST << RxCfgDMAShift));
		break;
	}

	rtl8168_nic_reset(dev);

	rtl8168_rx_desc_offset0_init(tp, 1);

	RTL_W8(Cfg9346, Cfg9346_Unlock);

	RTL_W8(MTPS, Reserved1_data);

	tp->cp_cmd |= PktCntrDisable | INTT_1;
	RTL_W16(CPlusCmd, tp->cp_cmd);

	RTL_W16(IntrMitigate, 0x5f51);
	RTL_W8(Config5, RTL_R8(Config5) & ~BIT_7);

	// Work around for RxFIFO overflow
	if(tp->mcfg == CFG_METHOD_1) {
		rtl8168_intr_mask |= RxFIFOOver | PCSTimeout;
		rtl8168_intr_mask &= ~RxDescUnavail;
	}

	rtl8168_desc_addr_fill(tp);

	/* Set DMA burst size and Interframe Gap Time */
	if(tp->mcfg == CFG_METHOD_1)
		RTL_W32(TxConfig, (TX_DMA_BURST_512 << TxDMAShift) |
		        (InterFrameGap << TxInterFrameGapShift));
	else
		RTL_W32(TxConfig, (TX_DMA_BURST_unlimited << TxDMAShift) |
		        (InterFrameGap << TxInterFrameGapShift));

	/* Clear the interrupt status register. */
	RTL_W16(IntrStatus, 0xFFFF);

	if(tp->mcfg == CFG_METHOD_21 || tp->mcfg == CFG_METHOD_22 || tp->mcfg == CFG_METHOD_24) {
		rtl8168_eri_write(tp, 0xC8, 1, 0x02, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xCA, 1, 0x08, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xCC, 1, 0x38, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xD0, 1, 0x48, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xE8, 4, 0x00100006, ERIAR_ExGMAC);
		set_offset70F(tp, 0x17);
		set_offset79(tp, 0x50);
		csi_tmp = rtl8168_eri_read(tp, 0xDC, 1, ERIAR_ExGMAC);
		csi_tmp &= ~BIT_0;
		rtl8168_eri_write(tp, 0xDC, 1, csi_tmp, ERIAR_ExGMAC);
		csi_tmp |= BIT_0;
		rtl8168_eri_write(tp, 0xDC, 1, csi_tmp, ERIAR_ExGMAC);

		RTL_W8(Config3, RTL_R8(Config3) & ~Beacon_en);

		tp->cp_cmd = RTL_R16(CPlusCmd) &
		             ~(EnableBist | Macdbgo_oe | Force_halfdup |
		               Force_rxflow_en | Force_txflow_en |
		               Cxpl_dbg_sel | ASF | PktCntrDisable |
		               Macdbgo_sel);

		rtl8168_eri_write(tp, 0xC0, 2, 0x00000000, ERIAR_ExGMAC);
		rtl8168_eri_write(tp, 0xB8, 2, 0x00000000, ERIAR_ExGMAC);
		RTL_W8(0x1B, RTL_R8(0x1B) & ~0x07);

		if(aspm) {
			RTL_W8(Config5, RTL_R8(Config5) | BIT_0);
			RTL_W8(Config2, RTL_R8(Config2) | BIT_7);
		} else {
			RTL_W8(Config5, RTL_R8(Config5) & ~BIT_0);
			RTL_W8(Config2, RTL_R8(Config2) & ~BIT_7);
		}

		RTL_W8(0xF1, RTL_R8(0xF1) | BIT_7);

		csi_tmp = rtl8168_eri_read(tp, 0x2FC, 1, ERIAR_ExGMAC);
		csi_tmp &= ~(BIT_0 | BIT_1 | BIT_2);
		csi_tmp |= BIT_0;
		rtl8168_eri_write(tp, 0x2FC, 1, csi_tmp, ERIAR_ExGMAC);

		RTL_W8(MTPS, 0x0C);

		/* tx checksum offload enable */
		tp->features |= NETIF_F_IP_CSUM;

		rtl8168_set_rxbufsize(tp, dev);
	}

	if((tp->mcfg == CFG_METHOD_1) || (tp->mcfg == CFG_METHOD_2) || (tp->mcfg == CFG_METHOD_3)) {
		/* csum offload command for RTL8168B/8111B */
		tp->tx_tcp_csum_cmd = TxIPCS | TxTCPCS;
		tp->tx_udp_csum_cmd = TxIPCS | TxUDPCS;
		tp->tx_ip_csum_cmd = TxIPCS;
	} else {
		/* csum offload command for RTL8168C/8111C and RTL8168CP/8111CP */
		tp->tx_tcp_csum_cmd = TxIPCS_C | TxTCPCS_C;
		tp->tx_udp_csum_cmd = TxIPCS_C | TxUDPCS_C;
		tp->tx_ip_csum_cmd = TxIPCS_C;
	}

	RTL_W16(CPlusCmd, tp->cp_cmd);

	RTL_W8(ChipCmd, CmdTxEnb | CmdRxEnb);

	if(tp->mcfg == CFG_METHOD_21 || tp->mcfg == CFG_METHOD_22 ||
	   tp->mcfg == CFG_METHOD_23 || tp->mcfg == CFG_METHOD_24)
		RTL_W8(0xF2, RTL_R8(0xF2) & ~BIT_3);

	//if (tp->mcfg == CFG_METHOD_11 || tp->mcfg == CFG_METHOD_12)
	//  rtl8168_mac_loopback_test(tp);

	/* Set Rx Config register */
	rtl8168_set_rx_mode(dev);

	if(tp->rx_fifo_overflow == 0) {
		/* Enable all known interrupts by setting the interrupt mask. */
		RTL_W16(IntrMask, rtl8168_intr_mask);
	}

	RTL_W8(Cfg9346, Cfg9346_Lock);

	if(!tp->pci_cfg_is_read) {

		pci_read_config_byte(deviceid, PCI_COMMAND, &tp->pci_cfg_space.cmd);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_0, &tp->pci_cfg_space.io_base_l);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_0 + 2, &tp->pci_cfg_space.io_base_h);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_2, &tp->pci_cfg_space.mem_base_l);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_2 + 2, &tp->pci_cfg_space.mem_base_h);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_3, &tp->pci_cfg_space.resv_0x1c_l);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_3 + 2, &tp->pci_cfg_space.resv_0x1c_h);
		pci_read_config_byte(deviceid, PCI_INTERRUPT_LINE, &tp->pci_cfg_space.ilr);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_4, &tp->pci_cfg_space.resv_0x20_l);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_4 + 2, &tp->pci_cfg_space.resv_0x20_h);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_5, &tp->pci_cfg_space.resv_0x24_l);
		pci_read_config_word(deviceid, PCI_BASE_ADDRESS_5 + 2, &tp->pci_cfg_space.resv_0x24_h);
		pci_read_config_word(deviceid, PCI_SUBSYSTEM_VENDOR_ID, &tp->pci_cfg_space.resv_0x2c_l);
		pci_read_config_word(deviceid, PCI_SUBSYSTEM_VENDOR_ID + 2, &tp->pci_cfg_space.resv_0x2c_h);
		tp->pci_cfg_space.pci_nvidia_geforce_6200 = rtl8168_csi_read(tp, PCI_DEVICE_ID_NVIDIA_GEFORCE_GO_6200);

		tp->pci_cfg_is_read = 1;
	}

	rtl8168_dsm(dev, DSM_MAC_INIT);

	options1 = RTL_R8(Config3);
	options2 = RTL_R8(Config5);
	csi_tmp = rtl8168_eri_read(tp, 0xDE, 4, ERIAR_ExGMAC);

	switch(tp->mcfg) {
	case CFG_METHOD_16:
	case CFG_METHOD_17:
	case CFG_METHOD_18:
	case CFG_METHOD_19:
	case CFG_METHOD_20:
	case CFG_METHOD_21:
	case CFG_METHOD_22:
	case CFG_METHOD_23:
	case CFG_METHOD_24:
		if((options1 & LinkUp) || (csi_tmp & BIT_0) || (options2 & UWF) || (options2 & BWF) || (options2 & MWF))
			tp->wol_enabled = WOL_ENABLED;
		else
			tp->wol_enabled = WOL_DISABLED;
		break;
	case CFG_METHOD_DEFAULT:
		tp->wol_enabled = WOL_DISABLED;
		break;
	default:
		if((options1 & LinkUp) || (options1 & MagicPacket) || (options2 & UWF) || (options2 & BWF) || (options2 & MWF))
			tp->wol_enabled = WOL_ENABLED;
		else
			tp->wol_enabled = WOL_DISABLED;
		break;
	}

	udelay(10);
}


static void
rtl8168_irq_mask_and_ack(struct rtl8168_private *tp)
{
	RTL_W16(IntrMask, 0x0000);
}

static void
rtl8168_hw_reset(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	/* Disable interrupts */
	rtl8168_irq_mask_and_ack(tp);

	rtl8168_nic_reset(dev);
}

static void
rtl8168_phy_power_down(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	mdio_write(tp, 0x1F, 0x0000);
	mdio_write(tp, MII_BMCR, BMCR_PDOWN);
}

static void
rtl8168_powerdown_pll(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int auto_nego = 0;
	int giga_ctrl = 0;
	u32 csi_tmp;
	u16 val = 0;




	if(tp->wol_enabled == WOL_ENABLED) {
		mdio_write(tp, 0x1F, 0x0000);
		auto_nego = mdio_read(tp, MII_ADVERTISE);
		auto_nego &= ~(ADVERTISE_10HALF | ADVERTISE_10FULL
		               | ADVERTISE_100HALF | ADVERTISE_100FULL);

		val = mdio_read(tp, MII_LPA);

#ifdef CONFIG_DOWN_SPEED_100
		auto_nego |= (ADVERTISE_100FULL | ADVERTISE_100HALF | ADVERTISE_10HALF | ADVERTISE_10FULL);
#else
		if(val & (LPA_10HALF | LPA_10FULL))
			auto_nego |= (ADVERTISE_10HALF | ADVERTISE_10FULL);
		else
			auto_nego |= (ADVERTISE_100FULL | ADVERTISE_100HALF | ADVERTISE_10HALF | ADVERTISE_10FULL);
#endif

		giga_ctrl = mdio_read(tp, MII_CTRL1000) & ~(ADVERTISE_1000HALF | ADVERTISE_1000FULL);
		mdio_write(tp, MII_ADVERTISE, auto_nego);
		mdio_write(tp, MII_CTRL1000, giga_ctrl);
		mdio_write(tp, MII_BMCR, BMCR_RESET | BMCR_ANENABLE | BMCR_ANRESTART);

		RTL_W32(RxConfig, RTL_R32(RxConfig) | AcceptBroadcast | AcceptMulticast | AcceptMyPhys);

		return;
	}

	rtl8168_phy_power_down(dev);

	switch(tp->mcfg) {
	case CFG_METHOD_21:
	case CFG_METHOD_22:
		RTL_W8(PMCH, RTL_R8(PMCH) & ~BIT_6);
		break;
	}
}

static void rtl8168_sleep_rx_enable(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	return ;
}

static void
rtl8168_tx_clear(struct rtl8168_private *tp)
{
	unsigned int i;

	for(i = tp->dirty_tx; i < tp->dirty_tx + NUM_TX_DESC; i++) {
		unsigned int entry = i % NUM_TX_DESC;
		struct ring_info *tx_skb = tp->tx_skb + entry;
		tx_skb->skb = NULL;
	}
	tp->cur_tx = tp->dirty_tx = 0;
}
static inline void
rtl8168_make_unusable_by_asic(struct RxDesc *desc)
{
	desc->addr = 0x0badbadbadbadbadull;
	desc->opts1 &= ~ cpu_to_le32(DescOwn | RsvdMask);
}

static void
rtl8168_rx_clear(struct rtl8168_private *tp)
{
	int i;

	for(i = 0; i < NUM_RX_DESC; i++) {
		if(tp->Rx_skbuff[i]) {
			tp->Rx_skbuff[i] = NULL;
			rtl8168_make_unusable_by_asic(tp->RxDescArray + i);

		}
	}
	net_flush_cache((unsigned long)tp->RxDescArray, R8168_RX_RING_BYTES);
}

static void rtl8168_down(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int count;

	rtl8168_dsm(dev, DSM_IF_DOWN);



	for(count = 0; count < 100; count++) {

		rtl8168_hw_reset(dev);

		rtl8168_sleep_rx_enable(dev);


		/*
		* And now for the 50k$ question: are IRQ disabled or not ?
		*
		* Two paths lead here:
		* 1) dev->close
		*    -> netif_running() is available to sync the current code and the
		*       IRQ handler. See rtl8168_interrupt for details.
		* 2) dev->change_mtu
		*    -> rtl8168_poll can not be issued again and re-enable the
		*       interruptions. Let's simply issue the IRQ down sequence again.
		*/
		if(RTL_R16(IntrMask) == 0)
			break;
	}

	rtl8168_tx_clear(tp);

	rtl8168_rx_clear(tp);

	rtl8168_powerdown_pll(dev);
}

static void rtl_close(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);

	if(tp->TxDescArray != NULL && tp->RxDescArray != NULL) {
		rtl8168_down(dev);

		tp->TxDescArray = NULL;
		tp->RxDescArray = NULL;
	}

}


static int rtl_open(struct eth_device *dev, bd_t *bis)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	int retval;

	rtl8168_set_rxbufsize(tp, dev);

	retval = -ENOMEM;

	/*
	 * Rx and Tx desscriptors needs 256 bytes alignment.
	 * pci_alloc_consistent provides more.
	 */
	tp->TxDescArray = (struct TxDesc *) tx_ring ;
	tp->TxPhyAddr = (dma_addr_t) tx_ring;

	if(!tp->TxDescArray)
		goto out;

	tp->RxDescArray = (struct RxDesc *) rx_ring;
	tp->RxPhyAddr = (dma_addr_t) rx_ring;

	memset(tp->TxDescArray, 0, R8168_TX_RING_BYTES);
	memset(tp->RxDescArray, 0, R8168_RX_RING_BYTES);

	retval = rtl8168_init_ring(dev);

	net_flush_cache((unsigned long)tp->TxDescArray, R8168_TX_RING_BYTES);
	net_flush_cache((unsigned long)tp->RxDescArray, R8168_RX_RING_BYTES);

	
	rtl8168_powerup_pll(dev);
	//rtl8168_rar_set(tp, dev->enetaddr);
	rtl8168_hw_start(dev);

	//memcpy(tp->org_mac_addr, dev->enetaddr, NODE_ADDRESS_SIZE);
	rtl8168_dsm(dev, DSM_IF_UP);

	//rtl8168_check_link_status(dev, tp, tp->mmio_addr);

	//retval = request_irq(dev->irq, rtl8168_interrupt, (tp->features & RTL_FEATURE_MSI) ? 0 : SA_SHIRQ, dev->name, dev);

out:
	return retval;

}

static unsigned int
rtl8168_xmii_link_ok(struct eth_device *dev)
{
	struct rtl8168_private *tp = netdev_priv(dev);
	unsigned int retval;

	retval = RTL_R8(PHYstatus) & LinkStatus;

	return retval;
}


#define TX_TIMEOUT  (6* CONFIG_SYS_HZ)

static int rtl_send(struct eth_device *dev, volatile void *packet, int length)
{
	/* send the packet to destination */

	u32 to;
	u8 *ptxb;

	struct rtl8168_private *tp = netdev_priv(dev);
	int entry = tp->cur_tx % NUM_TX_DESC;
	struct TxDesc *txd = tp->TxDescArray + entry;
	u32 len = length;
	u32 opts1 = 0;
	u32 opts2 = 0;
	u32 status1, status2;
	int timeout = 0;

	/* point to the current txb incase multiple tx_rings are used */
	ptxb = tp->tx_skb[entry].skb;
	memcpy(ptxb, (char *) packet, (int) length);
	net_flush_cache((unsigned long)ptxb, length);

	if(unlikely(le32_to_cpu(txd->opts1) & DescOwn))
		return 0;

	opts1 = DescOwn;
#if 0
	if(tp->features & NETIF_F_IP_CSUM) {
		if((tp->mcfg == CFG_METHOD_1) || (tp->mcfg == CFG_METHOD_2) || (tp->mcfg == CFG_METHOD_3))
			opts1 |= rtl8168_tx_csum(ptxb, dev);
		else
			opts2 = rtl8168_tx_csum(ptxb, dev);
	}
#endif

	opts1 |= FirstFrag | LastFrag;
	tp->tx_skb[entry].len = len;
	txd->addr = (u32) ptxb;
	txd->opts2 = 0 ; //cpu_to_le32(rtl8168_tx_vlan_tag(tp, skb));

	/* anti gcc 2.95.3 bugware (sic) */
	status1 = opts1 | len | (RingEnd * !((entry + 1) % NUM_TX_DESC));
	status2 = opts2;
	txd->opts1 = cpu_to_le32(status1);
	txd->opts2 |= cpu_to_le32(status2);

	tp->cur_tx +=  1;
	RTL_W8(TxPoll, NPQ);    /* set polling bit */
	to = get_timer(0) + TX_TIMEOUT;
	net_flush_cache((unsigned long)&tp->TxDescArray[entry], sizeof(struct TxDesc));
	
	while(cpu_to_le32(tp->TxDescArray[entry].opts1) & DescOwn) {
		if(get_timer(0)  >= to) {
			timeout = 1;
			break;
		}
		net_inval_cache((unsigned long)&tp->TxDescArray[entry], sizeof(struct TxDesc));
	}

	if(timeout) {
		printf("tx timeout/error\n");
		return 0;
	} else {
		//  printf("tx done\n");
		return length;
	}

}


static inline int
rtl8168_fragmented_frame(u32 status)
{
	return (status & (FirstFrag | LastFrag)) != (FirstFrag | LastFrag);
}


static int rtl_recv(struct eth_device *dev)
{
	/* return true if there's an ethernet packet ready to read */
	/* nic->packet should contain data on return */
	/* nic->packetlen should contain length of data */
	int num_p = 0;
	struct rtl8168_private *tp = netdev_priv(dev);
	int cur_rx = tp->cur_rx;
	unsigned int entry = cur_rx % NUM_RX_DESC;
	struct RxDesc *desc = tp->RxDescArray + entry;
	u32 status ;

	net_inval_cache((unsigned long)&tp->RxDescArray[cur_rx], sizeof(struct RxDesc));
	status = le32_to_cpu(tp->RxDescArray[cur_rx].opts1);
	if((status & DescOwn) == 0) {
		if(!(status & RxRES)) {
			unsigned char rxdata[RX_BUF_SIZE];
			int pkt_size = (status & 0x00003FFF) - 4;

			if(unlikely(rtl8168_fragmented_frame(status))) {
				rtl8168_mark_to_asic(desc, tp->rx_buf_sz);
				net_flush_cache((unsigned long)&tp->RxDescArray[cur_rx],
					sizeof(struct RxDesc));
				cur_rx = (cur_rx + 1) % NUM_RX_DESC;
				goto exit;
			}
			net_inval_cache((unsigned long)tp->Rx_skbuff[cur_rx], 2048);
			memcpy(rxdata, tp->Rx_skbuff[cur_rx], pkt_size);
			NetReceive(rxdata, pkt_size);    


			rtl8168_mark_to_asic(tp->RxDescArray + cur_rx, tp->rx_buf_sz);
			tp->RxDescArray[cur_rx].addr = (u32)(tp->Rx_skbuff[cur_rx]);
			net_flush_cache((unsigned long)&tp->RxDescArray[cur_rx],  sizeof(struct RxDesc));
			net_inval_cache((unsigned long)tp->Rx_skbuff[cur_rx],  2048);
			
		} else {
			rtl8168_mark_to_asic(desc, tp->rx_buf_sz);
			puts("Error Rx");
		}

		cur_rx = (cur_rx + 1) % NUM_RX_DESC;
		tp->cur_rx = cur_rx;

		return 1;

	}
exit:
	tp->cur_rx = cur_rx;
	return (!!num_p);       /* initially as this is called to flush the input */
}

//static unsigned char* dev_buffer;
static unsigned char dev_buffer [sizeof(struct eth_device) + sizeof(struct rtl8168_private)];

int rtl8168_initialize(bd_t *bis)
{
	int i;
	u16 vendor, device, status;
	struct eth_device *dev;
	struct rtl8168_private *tp;
	u8 mac_addr[6];
	
	pci_read_config_word(deviceid, PCI_VENDOR_ID, &vendor);
	pci_read_config_word(deviceid, PCI_DEVICE_ID, &device);

	for(i = 0; i < ARRAY_SIZE(ehci_pci_ids); i++) {
		if((ehci_pci_ids[i].vendor == vendor) && (ehci_pci_ids[i].device  == device))
			break;
	}

	if(i ==  ARRAY_SIZE(ehci_pci_ids)) {
		printf("rtl8168 chip doesn't exist!\n");
		return -1;
	}
#if 0
	u8* pbuffer;
	int buffer_size;

	buffer_size = (BUFFER_TOTAL + 4 * 256 + sizeof(struct eth_device) + sizeof(struct rtl8168_private));
	pbuffer = malloc(buffer_size);
	if(pbuffer == NULL) {
		printf("RTK LAN:Fail to alloc memory(size = %d)!\n", buffer_size);
		return -1;
	}

	tx_ring = ROUND((u32) pbuffer,  0x100);
	txb = ROUND((u32) tx_ring  + R8168_TX_RING_BYTES, 0x100) ;
	rx_ring = ROUND((u32) txb + NUM_TX_DESC * RX_BUF_SIZE, 0x100);
	rxb = ROUND((u32) rx_ring + R8168_RX_RING_BYTES, 0x100);
	dev_buffer = (u32) rxb + NUM_RX_DESC * 2048 ;
#endif


	/*rtl8168_init_board*/
	dev = (struct eth_device *) dev_buffer;   //malloc(sizeof *dev + sizeof *tp);
	memset(dev, 0, sizeof * dev);
	tp = (struct rtl8168_private *)(dev + 1);
	memset(tp, 0, sizeof * tp);
	sprintf(dev->name, "RTL8168");
	dev->priv = tp;
	tp->vendor = vendor;
	tp->device = device;
	pci_read_config_word(deviceid, PCI_SUBSYSTEM_VENDOR_ID, &tp->subsystem_vendor);
	pci_read_config_word(deviceid, PCI_SUBSYSTEM_ID, &tp->subsystem_device);

	pci_read_config_dword(deviceid, PCI_BASE_ADDRESS_2, (unsigned int*)(& (tp->mmio_addr)));
	tp->mmio_addr = (void*)((u32)(tp->mmio_addr) & (~0xf));
	printf("REALTEK RTL8168 @ 0x%x\n", (u32) tp->mmio_addr);

	pci_read_config_word(deviceid, PCI_COMMAND, &status);
	status |= PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER;
	pci_write_config_word(deviceid, PCI_COMMAND, status);

	pci_write_config_byte(deviceid, PCI_LATENCY_TIMER, 0x40);

	/* Identify chip attached to board */
	rtl8168_get_mac_version(tp);

	rtl8168_print_mac_version(tp);

	for(i = ARRAY_SIZE(rtl_chip_info) - 1; i >= 0; i--) {
		if(tp->mcfg == rtl_chip_info[i].mcfg)
			break;
	}
	if(i < 0) {
		printf("Realtek unknown chip version, assuming %s\n", rtl_chip_info[0].name);
		i++;
	}

	tp->chipset = i;

	tp->set_speed = rtl8168_set_speed_xmii;
	tp->link_ok = rtl8168_xmii_link_ok;

	/*
	tp->get_settings = rtl8168_gset_xmii;
	tp->phy_reset_enable = rtl8168_xmii_reset_enable;
	tp->phy_reset_pending = rtl8168_xmii_reset_pending;

	tp->features |= rtl8168_try_msi(pdev, ioaddr);
	*/
	if((tp->mcfg == CFG_METHOD_9) || (tp->mcfg == CFG_METHOD_10))
		RTL_W8(DBG_reg, RTL_R8(DBG_reg) | BIT_1 | BIT_7);

	
	if (board_get_enetaddr(mac_addr)) {
		rtl8168_rar_set(tp, mac_addr);
	} else 	if(eth_getenv_enetaddr("ethaddr", mac_addr)) {
		rtl8168_rar_set(tp, mac_addr);
	}

	for(i = 0; i < MAC_ADDR_LEN; i++) {
		dev->enetaddr[i] = RTL_R8(MAC0 + i);
		tp->org_mac_addr[i] = dev->enetaddr[i]; /* keep the original MAC address */
	}

	tp->intr_mask = rtl8168_intr_mask;

	tp->max_jumbo_frame_size = rtl_chip_info[tp->chipset].jumbo_frame_sz;

	rtl8168_phy_power_up(dev);
	rtl8168_hw_phy_config(dev);

	rtl8168_link_option((u8*) &autoneg, (u16*) &speed, (u8*) &duplex);
	rtl8168_set_speed(dev, autoneg, speed, duplex);


	dev->init = rtl_open;
	dev->halt = rtl_close;
	dev->send = rtl_send;
	dev->recv = rtl_recv;

	eth_register(dev);

	rtl8168_init_sequence(tp);

	udelay(10 * 1000);
	return 0;
}
