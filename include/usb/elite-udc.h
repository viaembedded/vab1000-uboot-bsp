#ifndef _ELITE_UDC_H_
#define _ELITE_UDC_H_

#include <usbdevice.h>

#if 0
/* defined in linux/usb/ch9.h */
enum usb_device_speed {
        USB_SPEED_UNKNOWN = 0,                  /* enumerating */
        USB_SPEED_LOW, USB_SPEED_FULL,          /* usb 1.1 */
        USB_SPEED_HIGH,                         /* usb 2.0 */
        USB_SPEED_WIRELESS,                     /* wireless (usb 2.5) */
        USB_SPEED_SUPER,                        /* usb 3.0 */
};
#endif

#define UINT              volatile unsigned int
#define USHORT            volatile unsigned short
#define UCHAR             volatile unsigned char 
#define ULL		  volatile unsigned long long

#define OTGIP
#define UCE_MAX_DMA		((unsigned)64)
#define UIE_MAX_DMA		((unsigned)8)
#define UBE_MAX_DMA		((unsigned)0x20000) /* max length = 128K */
#define EP0_FIFO_SIZE		((unsigned)64)
#define BULK_FIFO_SIZE		((unsigned)512)
#define INT_FIFO_SIZE		((unsigned)8)


/*Define the USB register bit value*/
/* USB device commmand/status register  - 20*/
#define USBREG_RUNCONTROLLER     0x01           /* set the controller run*/
#define USBREG_RESETCONTROLLER   0x02           /* reset the device contrller*/
#define USBREG_COMPLETEINT       0x04           /* the completion interrupt*/
#define USBREG_BABBLEINT         0x08           /* the babble interrupt*/
#define USBREG_BUSACTINT         0x10           /* the bus activity interrupt*/
#define USBREG_TEST_J            0x20           /* set test J state*/
#define USBREG_TEST_K            0x40           /* set test k state*/
#define USBREG_SE0_NAK           0x60           /* set test SE0 and NAK*/
#define USBREG_TESTPACKET        0x80           /* set device sending test packet*/
#define USBREG_FORCEENABLE       0xA0           /* force port enable*/
#define USBREG_TESTEYE           0xC0           /* Send test eye pattern packet*/

/* The device address register          - 21*/
#define DEVADDR_ADDRCHANGE       0x80           /* the address change bit for device address register*/

/* Device port control register         - 22*/
#define PORTCTRL_CONNECTSTATUS   0x01           /* current connection status*/
#define PORTCTRL_CONNECTCHANGE   0x02           /* connection change bit*/
#define PORTCTRL_SELFPOWER       0x04           /* the device is in the self power configuration*/
#define PORTCTRL_FORCERESUME     0x08           /* force deivce to wake the host*/
#define PORTCTRL_HIGHSPEEDMODE   0x10           /* device is in the high speed mode*/
#define PORTCTRL_FULLSPEEDMODE   0x20           /* device is in the full speed mode*/

/* Device itnerrupt enable register     - 23*/
#define INTENABLE_DEVICERESET    0x01           /* this bit is set when the bus reset is completed*/
#define INTENABLE_SUSPENDDETECT  0x02           /* device controller detect the suspend signal*/
#define INTENABLE_RESUMEDETECT   0x04           /* device controller detect the resume signal*/
#define INTENABLE_COMPLETEINTEN  0x10           /* enable completion interrupt*/
#define INTENABLE_BABBLEINTEN    0x20           /* enable babble interrupt*/
#define INTENABLE_BUSACTINTEN    0x40           /* enable bus activity interrupt*/
#define INTENABLE_ALL            0x70           /* enable all interrupt */
#define INTENABLE_FULLSPEEDONLY  0x80           /* force device to support full speed only*/

/* Control endpoint control             - 24*/
/* Bulk endpoint control                - 28*/
/* Interrupt endpoint control           - 2C*/
/* Bulk endpoint control                - 0x310*/
#define EP_RUN                   0x01           /* run the enpoint DMA*/
#define EP_ENABLEDMA             0x02           /* enable the DMA engine to run the schedule*/
#define EP_DMALIGHTRESET         0x04           /* light reset the DMA engine*/
#define EP_STALL                 0x08           /* set the endpoint to stall*/
#define EP_ACTIVE                0x10           /* the endpoint DMA is active*/
#define EP_COMPLETEINT           0x20           /* endpoint transfer complete interrupt*/
#define EP_BABBLE                0x40           /* endpoint babble interrupt*/

/* Control transfer descriptor status byte  - 30*/
#define CTRLXFER_XACTERR         0x01           /* trnasacion error of control transfer*/
#define CTRLXFER_BABBLE          0x02           /* babbe error of control transfer*/
#define CTRLXFER_SHORTPKT        0x04           /* shortpacket detect of control transfer*/
#define CTRLXFER_ACTIVE          0x08           /* transfer active of control transfer*/

#define CTRLXFER_IN              0x08           /* I/O control bit 0 for OUT , 1 for In*/
#define CTRLXFER_OUT             0x00           /**/
#define CTRLXFER_CMDVALID        0x10           /* setup command is valid*/
#define CTRLXFER_IOC             0x80           /* determine to gen interrupt for completion or not*/
#define CTRLXFER_DATA1           0x80           /* for data_toggle 1*/
#define CTRLXFER_DATA0           0x00           /* for data_toggle 0*/

/* Bulk endpoint transfer descriptor status            - 34*/
/* Bulk endpoint transfer descriptor status            - 0x314 */
#define BULKXFER_XACTERR         0x01           /* trnasacion error of bulk transfer*/
#define BULKXFER_BABBLE          0x02           /* babbe error of bulk transfer*/
#define BULKXFER_SHORTPKT        0X04           /* shortpacket detect of bulk transfer*/
#define BULKXFER_ACTIVE          0x08           /*  transfer active of bulk transfer*/
#define BULKXFER_IN              0x10           /* I/O control bit 0 for OUT , 1 for In*/
#define BULKXFER_OUT             0x00
/*HP05_Begin*/
#define BULKXFER_CBWBUF          0x20           /* enable CBW dedicated buffer*/
/*HP05_End*/
#define BULKXFER_IOC             0x80           /* determine to generate interrupt for completion or not*/
#define BULKXFER_DATA1           0x80           /* for data_toggle 1*/
#define BULKXFER_DATA0           0x00           /* for data_toggle 0*/

/* Interupt endpoint transfer descriptor - 33*/
#define INTXFER_XACTERR          0x01           /* trnasacion error for interrupt transfer*/
#define INTXFER_ACTIVE           0x02           /* transfer active*/
#define INTXFER_IOC              0x04           /* determine to generate interrupt for completion or not*/
#define INTXFER_DATA1            0x08           /* for data_toggle 1*/
#define INTXFER_DATA0            0x00           /* for data_toggle 0*/

#define CONTROLSTATE_SETUP	 0x00			/* the control endpoint is in the Setup state*/
#define CONTROLSTATE_DATA	 0x01			/* the control endpoint is in the Data state*/
#define CONTROLSTATE_STATUS	 0x02			/* the control endpoint is in the Status state*/



/*UDC 0x400 ~ 0x7FF*/
struct UDC_REGISTER{
	UCHAR           Reserved0[0x20];    /* 0x00~0x19 Reserved 0*/
	UCHAR           CommandStatus;      /* USB device commmand/status register  - 20*/
	UCHAR           DeviceAddr;         /* The device address register          - 21*/

	UCHAR           PortControl;        /* Device port control register         - 22*/
	UCHAR           IntEnable;          /* Device itnerrupt enable register     - 23*/

	UCHAR           ControlEpControl;   /* Control endpoint control             - 24*/
	UCHAR           ControlEpReserved;  /* Control endpoint reserved byte       - 25*/
	UCHAR           ControlEpMaxLen;    /* max length of control endpoint       - 26*/
	UCHAR           ControlEpEpNum;     /* The control endpoint number          - 27*/

	UCHAR           Bulk1EpControl;      /* Bulk endpoint control                - 28*/
	UCHAR           Bulk1EpOutEpNum;     /* Bulk out endpoint number             - 29*/
	UCHAR           Bulk1EpMaxLen;       /* Bulk maximum length                  - 2A*/
	UCHAR           Bulk1EpInEpNum;      /* Bulk In endpoint number              - 2B*/

	UCHAR           InterruptEpControl; /* Interrupt endpoint control           - 2C*/
	UCHAR           InterruptReserved;  /* Interrupt endpoint reserved byte     - 2D*/
	UCHAR           InterruptEpMaxLen;  /* Interrupt maximum transfer length    - 2E*/
	UCHAR           InterruptEpEpNum;   /* interrupt endpoint number            - 2F*/

	UCHAR           ControlDesStatus;   /* Control transfer descriptor status byte  - 30*/
	UCHAR           ControlDesControl;  /* Control transfer descriptor control byte - 31*/
	UCHAR           ControlDesTbytes;   /* Control transfer descriptor total bytes  - 32*/

	UCHAR           InterruptDes;       /* Interupt endpoint transfer descriptor - 33*/

	UCHAR           Bulk1DesStatus;      /* Bulk endpoint transfer descriptor status            - 34*/
	UCHAR           Bulk1DesTbytes0;     /* Bulk endpoint transfer descriptor control           - 35*/
	UCHAR           Bulk1DesTbytes1;     /* Bulk endpoint transfer descriptor lower total bytes - 36*/
	UCHAR           Bulk1DesTbytes2;     /* Bulk endpoint transfer descriptor higer total bytes - 37*/

	UCHAR           MacToParameter;     /* USB2.0 MAC timeout parameter register - 38*/
	UCHAR           PhyMisc;            /* PHY misc control register             - 39*/
	UCHAR           MiscControl0;       /* Misc. control register 0              - 3A*/
	UCHAR           MiscControl1;       /* Misc. control register 1              - 3B*/
	UCHAR           MiscControl2;       /* Misc. control register 2              - 3C*/
	UCHAR           MiscControl3;       /* Misc. control register 3              - 3D*/
	UCHAR           MACDelay;           /* MAC receiver enable delay parameter   - 3E*/
	UCHAR           FunctionPatchEn;    /* Function Patch Enable bits            - 3F*/

	UCHAR           InterruptInBuf[32]; /* Interrupt In Endpoint Data Buffer     - 40 ~ 5F (32 bytes) for CBW(31 bytes)*/

	UCHAR           Reserved1[0x2A0];   /* 0x060~0x2FF ; 0x2FF - 0x60 + 1 = 0x2A0  for Bulk Endpoint 01 02 DMA Source / Destination*/
	UCHAR           SetupCommand[8];    /* 0x300~0x307 */
	UCHAR           PHYControlBit;      /* 0x308 Phy Control Bit */
	UCHAR           Reserved2[2];       /* 0x309 ~ 0x30A*/
	UCHAR           NewUSBPhyTestControl;/* 0x30B New USB 1.1 and USB 2.0 PHY test control bit*/
	UCHAR           NewUSBPhyTestStatus; /* 0x30C New USB 1.1 and USB 2.0 PHY test status bit*/
	UCHAR           SelfPowerConnect;           /* 0x30D Debug pins for USBC side control bit*/

	UCHAR           Reserved3[2];           /* 0x30E ~ 0x30F*/

	UCHAR           Bulk2EpControl;      /* Bulk endpoint control                - 0x310*/
	UCHAR           Bulk2EpOutEpNum;     /* Bulk out endpoint number             - 0x311*/
	UCHAR           Bulk2EpMaxLen;       /* Bulk maximum length                  - 0x312*/
	UCHAR           Bulk2EpInEpNum;      /* Bulk In endpoint number              - 0x313*/

	UCHAR           Bulk2DesStatus;      /* Bulk endpoint transfer descriptor status            - 0x314*/
	UCHAR           Bulk2DesTbytes0;     /* Bulk endpoint transfer descriptor control           - 0x315*/
	UCHAR           Bulk2DesTbytes1;     /* Bulk endpoint transfer descriptor lower total bytes - 0x316*/
	UCHAR           Bulk2DesTbytes2;     /* Bulk endpoint transfer descriptor higer total bytes - 0x317*/

	UCHAR           Reserved4[0x28];     /*0x318 ~ 0x33F ; 0x33F - 0x318 + 1 = 0x28*/

	UCHAR           ControlDataBuf[64];  /*0x340~0x37F Control Endpoint Data Buffer */
};


/*DMA_Context_Control*/
/*1:IF0-->peripheral 0:peripher-->IF0*/
#define DMA_TRANS_OUT_DIR		0x00400000
/*1:Run 0:Stop Scheduled*/
#define DMA_RUN				0x00000080

/*DMA Interrupt Enable*/
/*enable interfac 0 interrupt*/
#define DMA_INTERRUPT_ENABLE0		0x00000001
/*enable interfac 1 interrupt*/
#define DMA_INTERRUPT_ENABLE1		0x00000002

/*enable interfac 0 status*/
#define DMA_INTERRUPT_STATUS0		0x00000001
/*enable interfac 1 status*/
#define DMA_INTERRUPT_STATUS1		0x00000002

struct UDC_DMA_REG {
		union { /*Rx00 - DMA Global Register (0x100~0x103)*/
			UINT DMA_Global;
			struct {
				UINT DMAConrollerEnable:1;
				UINT Reserved0:7;
				UINT SoftwareReset:1;
				UINT SoftwareReset0:1;
				UINT SoftwareReset1:1;
				UINT Reserved1:21;
			} DMA_Global_Bits;
		};

		union { /*Rx04 - DMA Interrupt Enable (0x104~x107)*/
			UINT DMA_IER;
			struct {
				UINT DMAInterruptEnable0:1;
				UINT DMAInterruptEnable1:1;
				UINT Reserved:30;
			} DMA_IER_Bits;
		};

		union { /*Rx08 - DMA Interrupt Status Register (0x108~0x10b)*/
			UINT DMA_ISR;
			struct {
				UINT InterruptStatus0:1;
				UINT InterruptStatus1:1;
				UINT Reserved:30;
			} DMA_ISR_Bits;
		};

		/*Interface 0*/
		union {/*Rx10c - DMA Memory-Descriptor Point Register dor Interdace 0 (0x10c~0x10f)*/
			UINT DMA_Descriptor_Point0;
			struct {
				UINT Reserved:2;
				UINT DesBaseAddr:30;
			} DMA_Descriptor_Point0_Bits;
		};

		union { /*Rx110 - DMA Residual Bytes Register for Interface 0 (Rx110~Rx103)*/
			UINT DMA_Residual_Bytes0;
			struct {
				UINT ResidualBytes:16;
				UINT IntEn:1;
				UINT Reseved:13;
				UINT Format:1;
				UINT End:1;
			} DMA_Residual_Bytes0_Bits;
		};

		/*Rx114 - DMA Data Address Register for Interface 0 (0x114~0x117)*/
		UINT DMA_Data_Addr0;

		union {/*Rx118 DMA Branch Address Register for INterface 0(0x118~0x11b)*/
			UINT DMA_Branch_Addr0;
			struct {
				UINT Reserved:4;
				UINT BranchDesxriptorAddress:28;
			} DMA_Branch_Addr0_Bits;
		};

		union {/*Rx11c DMA command Pointer Register for Interrface 0(0x11c~0x11f)*/
			UINT Descriptor_Addr0;
			struct {
				UINT Reserved:4;
				UINT DescriptorAddr:28;
			} Descriptor_Addr0_Bits;
		};

		union {/*Rx120 DMA Context Control Register for Interfaxe 0(0x120~0x123)*/
			UINT DMA_Context_Control0;
			struct {
				UINT EventCode:4;
				UINT Reserved0:3;
				UINT Run:1;
				UINT Active:1;
				UINT P0Cmpl:1;
				UINT Reserved1:2;
				UINT Prot:4;
				UINT Reserved2:6;
				UINT TransDir:1;
				UINT Reserved3:9;
			} DMA_Context_Control0_Bis;
		};

		/*Interface 1*/
		union {/*Rx124 - DMA Memory-Descriptor Point Register dor Interdace 1 (0x124~0x127)*/
			UINT DMA_Descriptor_Point1;
			struct {
				UINT Reserved:2;
				UINT DesBaseAddr:30;
			} DMA_Descriptor_Point1_Bits;
		};

		union { /*Rx128 - DMA Residual Bytes Register for Interface 1 (Rx128~Rx12b)*/
			UINT DMA_Residual_Bytes1;
			struct {
				UINT ResidualBytes:16;
				UINT IntEn:1;
				UINT Reseved:13;
				UINT Format:1;
				UINT End:1;
			} DMA_Residual_Bytes1_Bits;
		};

		/*Rx12c - DMA Data Address Register for Interface 1 (0x12c~0x12f)*/
		UINT DMA_Data_Addr1;

		union {/*Rx130 DMA Branch Address Register for INterface 10(0x130~0x133)*/
			UINT DMA_Branch_Addr1;
			struct {
				UINT Reserved:4;
				UINT BranchDesxriptorAddress:28;
			} DMA_Branch_Addr1_Bits;
		};

		union {/*Rx134 DMA command Pointer Register for Interrface 1(0x134~0x137)*/
			UINT Descriptor_Addr1;
			struct {
				UINT Reserved:4;
				UINT DescriptorAddr:28;
			} Descriptor_Addr1_Bits;
		};

		union {/*Rx138DMA Context Control Register for Interfaxe 1(0x138~0x13b)*/
			UINT DMA_Context_Control1;
			struct {
				UINT EventCode:4;
				UINT Reserved0:3;
				UINT Run:1;
				UINT Active:1;
				UINT P0Cmpl:1;
				UINT Reserved1:2;
				UINT Prot:4;
				UINT Reserved2:6;
				UINT TransDir:1;
				UINT Reserved3:9;
			} DMA_Context_Control1_Bis;
		};

}__attribute__((packed));

#define UDC_TEST_J_STATE           1
#define UDC_TEST_K_STATE           2
#define UDC_TEST_SE0_NAK           3
#define UDC_TEST_PACKET            4
#define UDC_TEST_FRORCE_ENABLE     5
#define UDC_TEST_EYE_PATTERN       6
#define UDC_TEST_MODE_NOT_ENABLED  0

#define UHDC_INT_UDC            0x0700
#define UHDC_INT_UDC_DMA2 	0x0400
#define UHDC_INT_UDC_DMA1 	0x0200
#define UHDC_INT_UDC_CORE  	0x0100

#define DESCRIPTOT_TYPE_SHORT	0
#define DESCRIPTOT_TYPE_LONG	1
#define DESCRIPTOT_TYPE_MIX	2

#define TRANS_OUT	1
#define TRANS_IN	0

#define DES_L_SIZE	0x10
#define DES_S_SIZE	0x4


/*
 *	PDMA Descriptor short
 */
struct _UDC_PDMA_DESC_S {
	union {
		UINT D_Word0;
		struct {
			UINT ReqCount:16;	/* bit  0 -15	-Request count*/
			UINT i:1;					/* bit	16		-interrupt*/
			UINT Reserved:13;
			UINT Format:1;		/* bit	30		-The descriptor format*/
			UINT End:1;				/* bit	31-End flag of descriptor list*/
		} D_Word0_Bits;
	};

	UINT Data_Addr;				/* bit	0-31		-Data Buffer address*/
}__attribute__((packed));

/*
 *	PDMA Descriptor long
 */

struct _UDC_PDMA_DESC_L {
	union {
		UINT D_Word0;
		struct {
			UINT ReqCount:16;	/* bit  0 -15-Request count*/
			UINT i:1;					/* bit	16-interrupt*/
			UINT Reserved:13;
			UINT Format:1;		/* bit	30-The descriptor format*/
			UINT End:1;				/* bit	31-End flag of descriptor list*/
		} D_Word0_Bits;
	};

	UINT Data_Addr;				/* bit	0-31		-Data Buffer address*/

	union {
		UINT Branch_addr;
		struct {
			UINT Reserved:2;
			UINT BranchAddr:30;	/* bit	31-2	-Descriptor Branch address*/
		} Branch_addr_Bits;
	};

	UINT Reserve0;
}__attribute__((packed));

#ifdef OTGIP
/*UHDC Global Register Space : 0x7F0~0x7F7*/
struct USB_GLOBAL_REG {
	union { /*Rx00 - UHDC Interrupt Status Register*/
		USHORT UHDC_Interrupt_Status;
		struct {
			USHORT EHCIInt:1; 	  /* [0] EHCI Interrupt bit*/
			USHORT UHCIInt:1;	  /* [1] UHCI Interrupt bit*/
			USHORT Reserved0:6;  /* reserved for future use*/
			USHORT UDCCoreInt:1; /* [8]  UDC Core Interrupt bit(IOC/Babble/Bus Activity)*/
			USHORT UDCDMA1Int:1; /* [9]  DMA1 Interrupt bit (Bulk Transfer)*/
			USHORT UDCDMA2Int:1; /* [10] DMA2 Interrupt bit (Bulk Transfer)*/
			USHORT Reserved1:5;  /* reserved for future use*/
		} UHDC_Interrupt_Status_Bits;
	};

	union { /*Rx01 - UHDC ID Status Register*/
		UCHAR  UHDC_ID_Status;
		struct {
			UCHAR DualRoleID:1; /* Dual-Role USB Controller ID Value*/
			UCHAR Reserved:7;   /* reserved for future use*/
		} UHDC_ID_Status_Bits;
	};

	union { /*Rx02 - UHDC Offchip Power Control Register*/
		UCHAR  UHDC_OffChip_PowerControl;
		struct {
			UCHAR  PowerSupplyMode:1; /* Power Supply Mode Select (1)SW mode (0)HW mode*/
			UCHAR  PowerSupplySWEn:1; /* Power Supply SW Mode Enable bit */
			UCHAR Reserved:6;         /* reserved for future use    */
		} UHDC_OffChip_PowerControl_Bits;
	};

	union { /*Rx03 - UHDC Clock Reset Control Register*/
		USHORT UHDC_Clock_Reset_Control;
		struct {
			USHORT AHBClkGatingEn:1;          /* [0] AHB clock Gating Enable bit*/
			USHORT HCSuspendClkGatingEn:1;    /* [1] Host Controller Suspend Clock Gating Enable bit*/
			USHORT DCSuspendClkGatingEn:1;    /* [2] Device Controller Suspend Clock Gating Enable bit*/
			USHORT DataReceivingClkGatingEn:1;/* [3] Controller Data-Receiving Clock Gating Enable bit*/
			USHORT Phy120MClkGatingEn:1;      /* [4] Phy 120M Clock Gating Enable bit*/
			USHORT Phy60MClkGatingEn:1;       /* [5] Phy  60M Clock Gating Enable bit*/
			USHORT Phy48MClkGatingEn:1;       /* [6] Phy  48M Clock Gating Enable bit */
			USHORT Reserved0:1;               /* [7] reserved for future use*/
			USHORT AHBResetGatingEn:1;        /* [8] AHB Bus Reset Gating Enable bit*/
			USHORT PowerOnResetGatingEn:1;    /* [9] Power-on Reset Gating Enable bit  */                                     
			USHORT Reserved1:6;               /* [15:10] reserved for future use*/
		} UHDC_Clock_Reset_Control_Bits;
	};

	union { /*Rx04 - UHDC Pullup/Pulldown Control Register*/
		USHORT UHDC_Pull_Up_Down_Control;
		struct {
			USHORT PullUpDownSWControlEn:1;   /* [0] Pullup/Pulldown SW Control Enable bit*/
			USHORT DPLinePullUpEn:1;          /* [1] DP Line Pullup Enable bit*/
			USHORT DPLinePullDownEn:1;        /* [2] DP Line Pulldown Enable bit*/
			USHORT DMLinePullDownEn:1;        /* [3] DM Line Pulldown Enable bit*/
			USHORT HSTerminationRegisterEn:1; /* [4] High Speed Termination Register On bit*/
			USHORT Reserved1:11;              /* [15:5] reserved for future use*/
		} UHDC_Pull_Up_Down_Control_Bits;
	};
}__attribute__((packed));
#endif
#define UDC_MAX_ENDPOINTS  4

struct vt8500_ep {
	unsigned long		irqs;
	const struct usb_endpoint_descriptor *desc;

	char			name[14];
	u16			maxpacket;
	u8			bEndpointAddress;
	u8			bmAttributes;
	unsigned		double_buf:1;
	unsigned		stopped:1;
	unsigned		fnf:1;
	unsigned		has_dma:1;
	unsigned            	toggle_bit:1;
        unsigned            	stall:1;
        unsigned            	rndis:1;
	u8			ackwait;
	u32			dma_counter;
	struct elite_udc_s	*udc;
	volatile u32        	temp_buffer_address;
	volatile u32        	temp_dma_phy_address;
	volatile u32        	temp_buffer_address2;
	volatile u32        	temp_dma_phy_address2;

	volatile u32        	rndis_buffer_address;	
	volatile u32        	rndis_dma_phy_address;	
	unsigned int        	rndis_buffer_length;	

	/*processing stall and clear stall.*/
	volatile u32        	stall_more_processing;
	volatile u32        	temp_dcmd;	
	volatile u32        	temp_dma_ccr;
	volatile u32        	temp_bulk_dma_addr;
	volatile u32        	ep_stall_toggle_bit;
	volatile u32        	ep_fifo_length;

}__attribute__((packed));

struct elite_udc_s {

    	struct vt8500_ep		ep[UDC_MAX_ENDPOINTS];//gri
       struct usb_device_instance *	udc_device;
	u16	               		devstat;
	u16                    		ep0_in_status;
    	volatile u32           		cbw_virtual_address;
	unsigned			softconnect:1;
	unsigned			vbus_active:1;
	unsigned			ep0_pending:1;
	unsigned			ep0_in:1;
	unsigned			ep0_set_config:1;
	unsigned			ep0_reset_config:1;
	unsigned			ep0_setup:1;
	unsigned               		ep0_status_0_byte:1;    
	unsigned               		usb_connect:1;
	unsigned               		file_storage_set_halt:1;
	unsigned               		bulk_out_dma_write_error:1;
	unsigned  int          		chip_version;
	int				speed;
	u8                         	setup_command[8];
};

#define USB_IP_BASE			0xD8007800
#define USB_UDC_REG_BASE	        (USB_IP_BASE + 0x400)
#define USB_UDC_DMA_REG_BASE	        (USB_IP_BASE + 0x500)

#define USBSTATE_ATTACHED       	0x00            /* device attached to host*/
#define USBSTATE_POWERED        	0x01            /* device obtain power*/
#define USBSTATE_DEFAULT        	0x02            /* default working state*/
#define USBSTATE_ADDRESS        	0x04            /* device has get the address*/
#define USBSTATE_CONFIGED       	0x10            /* deivce has set the configuration*/
#define USBSTATE_SUSPEND        	0x20            /* No bus activity device goes to suspend */

#ifdef OTGIP
/*UHDC Global Register Space 0x7F0~0x7F7*/
#define USB_GLOBAL_REG_BASE 	(USB_IP_BASE + 0x7f0)
#endif


void udc_irq (void);
/* Higher level functions for abstracting away from specific device */
int udc_endpoint_write(struct usb_endpoint_instance *endpoint);
void udc_setup_ep(struct usb_device_instance *device, unsigned int ep, struct usb_endpoint_instance *endpoint);
void udc_connect(void);
void udc_disconnect(void);
void udc_startup_events(struct usb_device_instance *device);
void udc_enable(struct usb_device_instance *device);
void udc_disable(void);
int  udc_init (void);

#ifdef	CONFIG_CMD_FASTBOOT
#define	EP0_MAX_PACKET_SIZE	64
#endif /* CONFIG_FASTBOOT */

#endif


