#ifndef _REALTEK_PCIELAN_H_
#define _REALTEK_PCIELAN_H_

#define mdelay(n)       udelay((n)*1000)

#define RTL_W8(reg, val8)	writeb ((val8), (tp->mmio_addr) + (reg))
#define RTL_W16(reg, val16)	writew ((val16), (tp->mmio_addr) + (reg))
#define RTL_W32(reg, val32)	writel ((val32), (tp->mmio_addr) + (reg))
#define RTL_R8(reg)		readb ((tp->mmio_addr) + (reg))
#define RTL_R16(reg)		readw ((tp->mmio_addr) + (reg))
#define RTL_R32(reg)		((unsigned long) readl ((tp->mmio_addr) + (reg)))

#define Reserved2_data	7
#define RX_DMA_BURST	7	/* Maximum PCI burst, '6' is 1024 */
#define TX_DMA_BURST_unlimited	7
#define TX_DMA_BURST_1024	6
#define TX_DMA_BURST_512	5
#define TX_DMA_BURST_256	4
#define TX_DMA_BURST_128	3
#define TX_DMA_BURST_64		2
#define TX_DMA_BURST_32		1
#define TX_DMA_BURST_16		0
#define Reserved1_data 	0x3F
#define RxPacketMaxSize	0x3FE8	/* 16K - 1 - ETH_HLEN - VLAN - CRC... */
#define Jumbo_Frame_2k	(2 * 1024)
#define Jumbo_Frame_3k	(3 * 1024)
#define Jumbo_Frame_4k	(4 * 1024)
#define Jumbo_Frame_5k	(5 * 1024)
#define Jumbo_Frame_6k	(6 * 1024)
#define Jumbo_Frame_7k	(7 * 1024)
#define Jumbo_Frame_8k	(8 * 1024)
#define Jumbo_Frame_9k	(9 * 1024)
#define InterFrameGap	0x03	/* 3 means InterFrameGap = the shortest one */
#define RxEarly_off	(1 << 11)

#define OOB_CMD_RESET		0x00
#define OOB_CMD_DRIVER_START	0x05
#define OOB_CMD_DRIVER_STOP	0x06
#define OOB_CMD_SET_IPMAC	0x41

#define NODE_ADDRESS_SIZE 6
#define MAC_ADDR_LEN 6

#define RX_BUF_SIZE	0x05F3	/* 0x05F3 = 1522bye + 1 */
//#define R8168_TX_RING_BYTES	(NUM_TX_DESC * sizeof(struct TxDesc))
//#define R8168_RX_RING_BYTES	(NUM_RX_DESC * sizeof(struct RxDesc))

#define RTL8168_TX_TIMEOUT	(6 * HZ)
#define RTL8168_LINK_TIMEOUT	(1 * HZ)
#define RTL8168_ESD_TIMEOUT	(2 * HZ)


#define NUM_TX_DESC	1	/* Number of Tx descriptor registers */
#define NUM_RX_DESC	4	/* Number of Rx descriptor registers */
#define R8168_TX_RING_BYTES	(NUM_TX_DESC * sizeof(struct TxDesc))
#define R8168_RX_RING_BYTES	(NUM_RX_DESC * sizeof(struct RxDesc))


/* The forced speed, 10Mb, 100Mb, gigabit, 2.5Gb, 10GbE. */
#define SPEED_10		10
#define SPEED_100		100
#define SPEED_1000		1000
#define SPEED_2500		2500
#define SPEED_10000		10000
#define SPEED_UNKNOWN		-1

/* Duplex, half or full. */
#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01
#define DUPLEX_UNKNOWN		0xff

#define AUTONEG_DISABLE		0x00
#define AUTONEG_ENABLE		0x01

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_ZLEN	60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_FRAME_LEN	1514		/* Max. octets in frame sans FCS */
#define ETH_FCS_LEN	4		/* Octets in the FCS		 */



enum _DescStatusBit {
    DescOwn		= (1 << 31), /* Descriptor is owned by NIC */
    RingEnd		= (1 << 30), /* End of descriptor ring */
    FirstFrag	= (1 << 29), /* First segment of a packet */
    LastFrag	= (1 << 28), /* Final segment of a packet */

    /* Tx private */
    /*------ offset 0 of tx descriptor ------*/
    LargeSend	= (1 << 27), /* TCP Large Send Offload (TSO) */
    MSSShift	= 16,        /* MSS value position */
    MSSMask		= 0x7ffU,    /* MSS value + LargeSend bit: 12 bits */
    TxIPCS		= (1 << 18), /* Calculate IP checksum */
    TxUDPCS		= (1 << 17), /* Calculate UDP/IP checksum */
    TxTCPCS		= (1 << 16), /* Calculate TCP/IP checksum */
    TxVlanTag	= (1 << 17), /* Add VLAN tag */

    /*@@@@@@ offset 4 of tx descriptor => bits for RTL8168C/CP only		begin @@@@@@*/
    TxUDPCS_C	= (1 << 31), /* Calculate UDP/IP checksum */
    TxTCPCS_C	= (1 << 30), /* Calculate TCP/IP checksum */
    TxIPCS_C	= (1 << 29), /* Calculate IP checksum */
    /*@@@@@@ offset 4 of tx descriptor => bits for RTL8168C/CP only		end @@@@@@*/


    /* Rx private */
    /*------ offset 0 of rx descriptor ------*/
    PID1		= (1 << 18), /* Protocol ID bit 1/2 */
    PID0		= (1 << 17), /* Protocol ID bit 2/2 */

#define RxProtoUDP	(PID1)
#define RxProtoTCP	(PID0)
#define RxProtoIP	(PID1 | PID0)
#define RxProtoMask	RxProtoIP

    RxIPF		= (1 << 16), /* IP checksum failed */
    RxUDPF		= (1 << 15), /* UDP/IP checksum failed */
    RxTCPF		= (1 << 14), /* TCP/IP checksum failed */
    RxVlanTag	= (1 << 16), /* VLAN tag available */

    /*@@@@@@ offset 0 of rx descriptor => bits for RTL8168C/CP only		begin @@@@@@*/
    RxUDPT		= (1 << 18),
    RxTCPT		= (1 << 17),
    /*@@@@@@ offset 0 of rx descriptor => bits for RTL8168C/CP only		end @@@@@@*/

    /*@@@@@@ offset 4 of rx descriptor => bits for RTL8168C/CP only		begin @@@@@@*/
    RxV6F		= (1 << 31),
    RxV4F		= (1 << 30),
    /*@@@@@@ offset 4 of rx descriptor => bits for RTL8168C/CP only		end @@@@@@*/
};


struct TxDesc {
	u32 opts1;
	u32 opts2;
	u64 addr;
};

struct RxDesc {
	u32 opts1;
	u32 opts2;
	u64 addr;
};

enum bits {
    BIT_0 = (1 << 0),
    BIT_1 = (1 << 1),
    BIT_2 = (1 << 2),
    BIT_3 = (1 << 3),
    BIT_4 = (1 << 4),
    BIT_5 = (1 << 5),
    BIT_6 = (1 << 6),
    BIT_7 = (1 << 7),
    BIT_8 = (1 << 8),
    BIT_9 = (1 << 9),
    BIT_10 = (1 << 10),
    BIT_11 = (1 << 11),
    BIT_12 = (1 << 12),
    BIT_13 = (1 << 13),
    BIT_14 = (1 << 14),
    BIT_15 = (1 << 15),
    BIT_16 = (1 << 16),
    BIT_17 = (1 << 17),
    BIT_18 = (1 << 18),
    BIT_19 = (1 << 19),
    BIT_20 = (1 << 20),
    BIT_21 = (1 << 21),
    BIT_22 = (1 << 22),
    BIT_23 = (1 << 23),
    BIT_24 = (1 << 24),
    BIT_25 = (1 << 25),
    BIT_26 = (1 << 26),
    BIT_27 = (1 << 27),
    BIT_28 = (1 << 28),
    BIT_29 = (1 << 29),
    BIT_30 = (1 << 30),
    BIT_31 = (1 << 31)
};

enum mcfg {
    CFG_METHOD_1=0,
    CFG_METHOD_2,
    CFG_METHOD_3,
    CFG_METHOD_4,
    CFG_METHOD_5,
    CFG_METHOD_6,
    CFG_METHOD_7,
    CFG_METHOD_8,
    CFG_METHOD_9 ,
    CFG_METHOD_10,
    CFG_METHOD_11,
    CFG_METHOD_12,
    CFG_METHOD_13,
    CFG_METHOD_14,
    CFG_METHOD_15,
    CFG_METHOD_16,
    CFG_METHOD_17,
    CFG_METHOD_18,
    CFG_METHOD_19,
    CFG_METHOD_20,
    CFG_METHOD_21,
    CFG_METHOD_22,
    CFG_METHOD_23,
    CFG_METHOD_24,
    CFG_METHOD_MAX,
    CFG_METHOD_DEFAULT = 0xFF
};

enum RTL8168_registers {
    MAC0			= 0x00,		/* Ethernet hardware address. */
    MAC4			= 0x04,
    MAR0			= 0x08,		/* Multicast filter. */
    CounterAddrLow		= 0x10,
    CounterAddrHigh		= 0x14,
    CustomLED		= 0x18,
    TxDescStartAddrLow	= 0x20,
    TxDescStartAddrHigh	= 0x24,
    TxHDescStartAddrLow	= 0x28,
    TxHDescStartAddrHigh	= 0x2c,
    FLASH			= 0x30,
    ERSR			= 0x36,
    ChipCmd			= 0x37,
    TxPoll			= 0x38,
    IntrMask		= 0x3C,
    IntrStatus		= 0x3E,
    TxConfig		= 0x40,
    RxConfig		= 0x44,
    TCTR			= 0x48,
    Cfg9346			= 0x50,
    Config0			= 0x51,
    Config1			= 0x52,
    Config2			= 0x53,
    Config3			= 0x54,
    Config4			= 0x55,
    Config5			= 0x56,
    TDFNR			= 0x57,
    TimeIntr		= 0x58,
    PHYAR			= 0x60,
    CSIDR			= 0x64,
    CSIAR			= 0x68,
    PHYstatus		= 0x6C,
    MACDBG			= 0x6D,
    GPIO			= 0x6E,
    PMCH			= 0x6F,
    ERIDR			= 0x70,
    ERIAR			= 0x74,
    EPHY_RXER_NUM		= 0x7C,
    EPHYAR			= 0x80,
    OCPDR			= 0xB0,
    MACOCP			= 0xB0,
    OCPAR			= 0xB4,
    PHYOCP			= 0xB8,
    DBG_reg			= 0xD1,
    MCUCmd_reg		= 0xD3,
    RxMaxSize		= 0xDA,
    EFUSEAR			= 0xDC,
    CPlusCmd		= 0xE0,
    IntrMitigate		= 0xE2,
    RxDescAddrLow		= 0xE4,
    RxDescAddrHigh		= 0xE8,
    MTPS			= 0xEC,
    FuncEvent		= 0xF0,
    FuncEventMask		= 0xF4,
    FuncPresetState		= 0xF8,
    FuncForceEvent		= 0xFC,
};

enum effuse {
    EFUSE_SUPPORT = 1,
    EFUSE_NOT_SUPPORT = 0,
};

enum eetype {
    EEPROM_TYPE_NONE=0,
    EEPROM_TYPE_93C46,
    EEPROM_TYPE_93C56,
    EEPROM_TWSI
};

enum RTL8168_register_content {
    /* InterruptStatusBits */
    SYSErr		= 0x8000,
    PCSTimeout	= 0x4000,
    SWInt		= 0x0100,
    TxDescUnavail	= 0x0080,
    RxFIFOOver	= 0x0040,
    LinkChg		= 0x0020,
    RxDescUnavail	= 0x0010,
    TxErr		= 0x0008,
    TxOK		= 0x0004,
    RxErr		= 0x0002,
    RxOK		= 0x0001,

    /* RxStatusDesc */
    RxRWT = (1 << 22),
    RxRES = (1 << 21),
    RxRUNT = (1 << 20),
    RxCRC = (1 << 19),

    /* ChipCmdBits */
    StopReq  = 0x80,
    CmdReset = 0x10,
    CmdRxEnb = 0x08,
    CmdTxEnb = 0x04,
    RxBufEmpty = 0x01,

    /* Cfg9346Bits */
    Cfg9346_Lock = 0x00,
    Cfg9346_Unlock = 0xC0,
    Cfg9346_EEDO = (1 << 0),
    Cfg9346_EEDI = (1 << 1),
    Cfg9346_EESK = (1 << 2),
    Cfg9346_EECS = (1 << 3),
    Cfg9346_EEM0 = (1 << 6),
    Cfg9346_EEM1 = (1 << 7),

    /* rx_mode_bits */
    AcceptPROMISC = 0x40,
    AcceptErr = 0x20,
    AcceptRunt = 0x10,
    AcceptBroadcast = 0x08,
    AcceptMulticast = 0x04,
    AcceptMyPhys = 0x02,
    AcceptAllPhys = 0x01,

    /* Transmit Priority Polling*/
    HPQ = 0x80,
    NPQ = 0x40,
    FSWInt = 0x01,

    /* RxConfigBits */
    Reserved2_shift = 13,
    RxCfgDMAShift = 8,
    RxCfg_128_int_en = (1 << 15),
    RxCfg_fet_multi_en = (1 << 14),
    RxCfg_half_refetch = (1 << 13),
    RxCfg_9356SEL = (1 << 6),

    /* TxConfigBits */
    TxInterFrameGapShift = 24,
    TxDMAShift = 8,	/* DMA burst value (0-7) is shift this many bits */
    TxMACLoopBack = (1 << 17),	/* MAC loopback */

    /* Config1 register p.24 */
    LEDS1		= (1 << 7),
    LEDS0		= (1 << 6),
    Speed_down	= (1 << 4),
    MEMMAP		= (1 << 3),
    IOMAP		= (1 << 2),
    VPD		= (1 << 1),
    PMEnable	= (1 << 0),	/* Power Management Enable */

    /* Config3 register */
    MagicPacket	= (1 << 5),	/* Wake up when receives a Magic Packet */
    LinkUp		= (1 << 4),	/* This bit is reserved in RTL8168B.*/
    /* Wake up when the cable connection is re-established */
    ECRCEN		= (1 << 3),	/* This bit is reserved in RTL8168B*/
    Jumbo_En0	= (1 << 2),	/* This bit is reserved in RTL8168B*/
    RDY_TO_L23	= (1 << 1),	/* This bit is reserved in RTL8168B*/
    Beacon_en	= (1 << 0),	/* This bit is reserved in RTL8168B*/

    /* Config4 register */
    Jumbo_En1	= (1 << 1),	/* This bit is reserved in RTL8168B*/

    /* Config5 register */
    BWF		= (1 << 6),	/* Accept Broadcast wakeup frame */
    MWF		= (1 << 5),	/* Accept Multicast wakeup frame */
    UWF		= (1 << 4),	/* Accept Unicast wakeup frame */
    LanWake		= (1 << 1),	/* LanWake enable/disable */
    PMEStatus	= (1 << 0),	/* PME status can be reset by PCI RST# */

    /* CPlusCmd */
    EnableBist	= (1 << 15),
    Macdbgo_oe	= (1 << 14),
    Normal_mode	= (1 << 13),
    Force_halfdup	= (1 << 12),
    Force_rxflow_en	= (1 << 11),
    Force_txflow_en	= (1 << 10),
    Cxpl_dbg_sel	= (1 << 9),//This bit is reserved in RTL8168B
    ASF		= (1 << 8),//This bit is reserved in RTL8168C
    PktCntrDisable	= (1 << 7),
    RxVlan		= (1 << 6),
    RxChkSum	= (1 << 5),
    Macdbgo_sel	= 0x001C,
    INTT_0		= 0x0000,
    INTT_1		= 0x0001,
    INTT_2		= 0x0002,
    INTT_3		= 0x0003,

    /* rtl8168_PHYstatus */
    PowerSaveStatus = 0x80,
    TxFlowCtrl = 0x40,
    RxFlowCtrl = 0x20,
    _1000bpsF = 0x10,
    _100bps = 0x08,
    _10bps = 0x04,
    LinkStatus = 0x02,
    FullDup = 0x01,

    /* DBG_reg */
    Fix_Nak_1 = (1 << 4),
    Fix_Nak_2 = (1 << 3),
    DBGPIN_E2 = (1 << 0),

    /* DumpCounterCommand */
    CounterDump = 0x8,

    /* PHY access */
    PHYAR_Flag = 0x80000000,
    PHYAR_Write = 0x80000000,
    PHYAR_Read = 0x00000000,
    PHYAR_Reg_Mask = 0x1f,
    PHYAR_Reg_shift = 16,
    PHYAR_Data_Mask = 0xffff,

    /* EPHY access */
    EPHYAR_Flag = 0x80000000,
    EPHYAR_Write = 0x80000000,
    EPHYAR_Read = 0x00000000,
    EPHYAR_Reg_Mask = 0x1f,
    EPHYAR_Reg_shift = 16,
    EPHYAR_Data_Mask = 0xffff,

    /* CSI access */
    CSIAR_Flag = 0x80000000,
    CSIAR_Write = 0x80000000,
    CSIAR_Read = 0x00000000,
    CSIAR_ByteEn = 0x0f,
    CSIAR_ByteEn_shift = 12,
    CSIAR_Addr_Mask = 0x0fff,

    /* ERI access */
    ERIAR_Flag = 0x80000000,
    ERIAR_Write = 0x80000000,
    ERIAR_Read = 0x00000000,
    ERIAR_Addr_Align = 4, /* ERI access register address must be 4 byte alignment */
    ERIAR_ExGMAC = 0,
    ERIAR_MSIX = 1,
    ERIAR_ASF = 2,
    ERIAR_OOB = 2,
    ERIAR_Type_shift = 16,
    ERIAR_ByteEn = 0x0f,
    ERIAR_ByteEn_shift = 12,

    /* OCP GPHY access */
    OCPDR_Write = 0x80000000,
    OCPDR_Read = 0x00000000,
    OCPDR_Reg_Mask = 0xFF,
    OCPDR_Data_Mask = 0xFFFF,
    OCPDR_GPHY_Reg_shift = 16,
    OCPAR_Flag = 0x80000000,
    OCPAR_GPHY_Write = 0x8000F060,
    OCPAR_GPHY_Read = 0x0000F060,
    OCPR_Write = 0x80000000,
    OCPR_Read = 0x00000000,
    OCPR_Addr_Reg_shift = 16,
    OCPR_Flag = 0x80000000,
    OCP_STD_PHY_BASE_PAGE = 0x0A40,

    /* MCU Command */
    Now_is_oob = (1 << 7),
    Txfifo_empty = (1 << 5),
    Rxfifo_empty = (1 << 4),

    /* E-FUSE access */
    EFUSE_WRITE	= 0x80000000,
    EFUSE_WRITE_OK	= 0x00000000,
    EFUSE_READ	= 0x00000000,
    EFUSE_READ_OK	= 0x80000000,
    EFUSE_Reg_Mask	= 0x03FF,
    EFUSE_Reg_Shift	= 8,
    EFUSE_Check_Cnt	= 300,
    EFUSE_READ_FAIL	= 0xFF,
    EFUSE_Data_Mask	= 0x000000FF,

    /* GPIO */
    GPIO_en = (1 << 0),

};
#define DMA_BIT_MASK(value)				((1ULL << value) - 1)

#define __iomem
#define __chk_io_ptr(x) (void)0
#define __bitwise
#define PCI_DEVICE_ID_NVIDIA_GEFORCE_GO_6200    0x0164
enum wol_capability {
    WOL_DISABLED = 0,
    WOL_ENABLED = 1
};

#define TX_BUFFS_AVAIL(tp) \
	(tp->dirty_tx + NUM_TX_DESC - tp->cur_tx - 1)

enum RTL8168_DSM_STATE {
    DSM_MAC_INIT = 1,
    DSM_NIC_GOTO_D3 = 2,
    DSM_IF_DOWN = 3,
    DSM_NIC_RESUME_D3 = 4,
    DSM_IF_UP = 5,
};

#define RsvdMask	0x3fffc000

struct ring_info {
	unsigned char *skb;
	u32		len;
	u8		__pad[sizeof(void *) - sizeof(u32)];
};

enum features {
//	RTL_FEATURE_WOL	= (1 << 0),
    RTL_FEATURE_MSI	= (1 << 1),
    NETIF_F_IP_CSUM     = (1<<2),
    NETIF_F_RXCSUM       = (1<<3),
};

struct pci_resource {
	u8	cmd;
	u8	cls;
	u16	io_base_h;
	u16	io_base_l;
	u16	mem_base_h;
	u16	mem_base_l;
	u8	ilr;
	u16	resv_0x1c_h;
	u16	resv_0x1c_l;
	u16	resv_0x20_h;
	u16	resv_0x20_l;
	u16	resv_0x24_h;
	u16	resv_0x24_l;
	u16	resv_0x2c_h;
	u16	resv_0x2c_l;
	u32	pci_nvidia_geforce_6200;
	u32 pci_nvidia_geforce__6250_1;
};



struct rtl8168_private {
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;

	void __iomem *mmio_addr;	/* memory map physical address */
//	struct pci_dev *pci_dev;	/* Index of PCI device */
//	struct net_device *dev;
//	struct net_device_stats stats;	/* statistics of net device */
//	spinlock_t lock;		/* spin lock flag */
//	spinlock_t phy_lock;		/* spin lock flag for GPHY */
	u32 msg_enable;
	u32 tx_tcp_csum_cmd;
	u32 tx_udp_csum_cmd;
	u32 tx_ip_csum_cmd;
	int max_jumbo_frame_size;
	int chipset;
	u32 mcfg;
	u32 cur_rx; /* Index into the Rx descriptor buffer of next Rx pkt. */
	u32 cur_tx; /* Index into the Tx descriptor buffer of next Rx pkt. */
	u32 dirty_rx;
	u32 dirty_tx;
	struct TxDesc *TxDescArray;	/* 256-aligned Tx descriptor ring */
	struct RxDesc *RxDescArray;	/* 256-aligned Rx descriptor ring */
	dma_addr_t TxPhyAddr;
	dma_addr_t RxPhyAddr;
	unsigned char *Rx_skbuff[NUM_RX_DESC];	/* Rx data buffers */
	struct ring_info tx_skb[NUM_TX_DESC];	/* Tx data buffers */
	unsigned rx_buf_sz;
	int rx_fifo_overflow;
//	struct timer_list esd_timer;
//	struct timer_list link_timer;
	int old_link_status;
	struct pci_resource pci_cfg_space;
	unsigned int esd_flag;
	unsigned int pci_cfg_is_read;
	unsigned int rtl8168_rx_config;
	u16 cp_cmd;
	u16 intr_mask;
	int phy_auto_nego_reg;
	int phy_1000_ctrl_reg;
	u8 org_mac_addr[NODE_ADDRESS_SIZE];
//#ifdef CONFIG_R8168_VLAN
//	struct vlan_group *vlgrp;
//#endif
	u8	wol_enabled;
	u8	efuse;
	u8	eeprom_type;
	u8	autoneg;
	u8	duplex;
	u16	speed;
	u16	eeprom_len;
	u16	cur_page;
	int (*set_speed)(struct eth_device *, u8 autoneg, u16 speed, u8 duplex);
	unsigned int (*link_ok)(struct eth_device *);

#if 0

	void (*get_settings)(struct net_device *, struct ethtool_cmd *);
	void (*phy_reset_enable)(struct net_device *);
	unsigned int (*phy_reset_pending)(struct net_device *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	struct work_struct task;
#else
	struct delayed_work task;
#endif
#endif
	unsigned features;
};

extern void setup_eth_mac(struct eth_device *dev,uint8_t *addr);

#endif

