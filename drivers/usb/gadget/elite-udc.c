#include <asm/byteorder.h>
#include <usbdevice.h>
#include "ep0.h"
#include <usb_defs.h>
#include <asm/errno.h>
#include <common.h>

#if defined(CONFIG_ELITE1000_UDC) && defined(CONFIG_USB_DEVICE)

#include <usb/elite-udc.h>


//#define DEBUG

#ifdef DEBUG
#define dbg(format, arg...) printf("ELite UDC DEBUG: " format "\n", ## arg)
#else
#define dbg(format, arg...) do {} while(0)
#endif /* DEBUG */

static struct elite_udc_s  udc;

static struct UDC_REGISTER *pDevReg;
static struct UDC_DMA_REG *pUdcDmaReg;

static UCHAR *pSetupCommandBuf;
static UCHAR *SetupBuf;
static UCHAR *IntBuf;
static volatile unsigned char *pUSBMiscControlRegister5;

UCHAR ControlState; /* the state the control pipe*/
UCHAR TestMode;
UCHAR USBState;
static struct urb *ep0_urb = NULL;

static unsigned int gadget_connect = 0;

#ifdef OTGIP
static struct USB_GLOBAL_REG *pGlobalReg;
#endif

unsigned char UdcPdmaVirAddrLI[0x100]  __attribute__ ((aligned (4)));
unsigned char UdcPdmaVirAddrSI[0x100]  __attribute__ ((aligned (4)));
unsigned char UdcPdmaVirAddrLO[0x100]  __attribute__ ((aligned (4)));
unsigned char UdcPdmaVirAddrSO[0x100]  __attribute__ ((aligned (4)));
unsigned char bulk_out_dma1[512] __attribute__ ((aligned (4)));
unsigned char bulk_out_dma2[512] __attribute__ ((aligned (4)));
unsigned char bulk_in_dma[1024] __attribute__ ((aligned (4)));


#define IRQ_HANDLED 1
#define USB_BULK_RESET_REQUEST		0xff
#define USB_DEVICE_SELF_POWERED		0	/* (read only) */
//#define USB_DEVICE_REMOTE_WAKEUP	1	/* dev may initiate wakeup */
#define USB_DEVICE_TEST_MODE		2	/* (wired high speed only) */
#define USB_DEVICE_BATTERY		2	/* (wireless) */
#define USB_DEVICE_B_HNP_ENABLE		3	/* (otg) dev may initiate HNP */
#define USB_DEVICE_WUSB_DEVICE		3	/* (wireless)*/
#define USB_DEVICE_A_HNP_SUPPORT	4	/* (otg) RH port supports HNP */
#define USB_DEVICE_A_ALT_HNP_SUPPORT	5	/* (otg) other RH port does */
#define USB_DEVICE_DEBUG_MODE		6	/* (special devices only) */

static void udc_control_prepare_data_resp(void);

static void elite_udc_pdma_des_prepare(unsigned int size, unsigned int dma_phy
							, unsigned char des_type, unsigned char channel);
static void pullup_enable(void)
{
	dbg("gri pullup_enable()\n");
	/*Enable port control's FSM to enable 1.5k pull-up on D+.*/
	pDevReg->PhyMisc &= 0x0F;/*Hardware auto-mode*/

} 


static void pullup_disable(void)
{
	dbg("pullup_disable()\n");
	/*Hold port control's FSM from enter device mode.*/
	pDevReg->PhyMisc &= 0x0F;
	pDevReg->PhyMisc |= 0x10;

       	gadget_connect=0;
    
} /*static void pullup_disable()*/

static int elite_pullup( int is_on)
{
	udc.softconnect = (is_on != 0);
        
	if (udc.softconnect)        
		pullup_enable();
	else
		pullup_disable();
	return 0;
}
void bulk_out_prepare_dma(u8* buffer)
{

        	u32 dcmd = 0,dma_ccr = 0;
	       	u32	buf = 0;

    		dcmd = 512;
              	udc.ep[2].temp_buffer_address = (u32)buffer;//((req->req.dma + req->req.actual) & 0xFFFFFFFC); 
		elite_udc_pdma_des_prepare(dcmd, (u32) buffer, DESCRIPTOT_TYPE_LONG, TRANS_OUT);

		pDevReg->Bulk2DesStatus = 0x00;
		pDevReg->Bulk2DesTbytes2 |= (dcmd >> 16) & 0x3;
		pDevReg->Bulk2DesTbytes1 = (dcmd >> 8) & 0xFF;
		pDevReg->Bulk2DesTbytes0 = dcmd & 0xFF;


             	if (pDevReg->Bulk2EpControl & EP_STALL){
                     udc.ep[2].ep_stall_toggle_bit = udc.ep[2].toggle_bit;
                     udc.ep[2].temp_dma_ccr = dcmd;
		     udc.ep[2].temp_bulk_dma_addr = buf;
              	}
		/* set endpoint data toggle*/
		if (udc.ep[2].toggle_bit)
			pDevReg->Bulk2DesTbytes2 |= BULKXFER_DATA1;
		else
			pDevReg->Bulk2DesTbytes2 &= 0x3F;/*BULKXFER_DATA0;*/

		pDevReg->Bulk2DesStatus = BULKXFER_IOC;/*| BULKXFER_IN;*/
		dma_ccr = 0;
		dma_ccr = DMA_RUN;
		dma_ccr |= DMA_TRANS_OUT_DIR;

		pUdcDmaReg->DMA_Context_Control1 = dma_ccr;

		pDevReg->Bulk2DesStatus |= BULKXFER_ACTIVE;
}

static void elite_ep_enable(int ep)
{

	switch ((ep)) {
		case 0:/*Control In/Out*/
			pDevReg->ControlEpControl = EP_RUN + EP_ENABLEDMA;
			break;

		case 1:/*Bulk In*/
			pDevReg->Bulk1EpControl = EP_RUN + EP_ENABLEDMA;
	       		break;

		case 2:/*Bulk Out*/
              		bulk_out_prepare_dma(bulk_out_dma1);
			pDevReg->Bulk2EpControl = EP_RUN + EP_ENABLEDMA;
	       		break;

		case 3:/*Interrupt In*/
			pDevReg->InterruptEpControl = EP_RUN + EP_ENABLEDMA;
	       		break;
	}
} /*static int wmt_ep_disable()*/

static void elite_ep_disable(int ep)
{

	switch (ep) {
		case 0:/*Control In/Out*/
			pDevReg->ControlEpControl &= 0xFC;
			break;

		case 1:/*Bulk In*/
			pDevReg->Bulk1EpControl &= 0xFC;
			break;

		case 2:/*Bulk Out*/
			pDevReg->Bulk2EpControl &= 0xFC;
			break;

		case 3:/*Interrupt In*/
			pDevReg->InterruptEpControl &= 0xFC;
			break;
	}
} /*static int wmt_ep_disable()*/


static void
elite_udc_csr(void)
{
	/* abolish any previous hardware state*/
       	int i;
	dbg("elite_udc_csr()\n");

        int size = (udc.speed == USB_SPEED_FULL)? 64:512;
        for(i = 0; i< UDC_MAX_ENDPOINTS; i++) {
              if(udc.ep[i].bmAttributes == USB_ENDPOINT_XFER_BULK) {
                  udc.ep[i].maxpacket = size;
                   if((udc.ep[i].bEndpointAddress) & USB_DIR_IN)
                             pDevReg->Bulk1EpMaxLen = (size & 0xFF);
                   else
                             pDevReg->Bulk2EpMaxLen = (size & 0xFF);
                    
               }
        }
} /*wmt_udc_csr(void)*/

void udc_setup_ep (struct usb_device_instance *device,
		   unsigned int ep, struct usb_endpoint_instance *endpoint)
{

	struct vt8500_ep	*elite_ep;

       	int ep_num, ep_addr,  ep_type, ep_size;
       	int attributes;
        
	dbg("setting up endpoint id %d", ep);

	if (!endpoint) {
		usberr("endpoint void!");
		return;
	}

	ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	if (ep_num >= UDC_MAX_ENDPOINTS) {
		usberr("unable to setup ep %d!", ep_num);
		return;
	}

	ep_addr = endpoint->endpoint_address;
	if ((ep_addr & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) {
			/* IN endpoint */
		ep_size = endpoint->tx_packetSize;
		attributes = endpoint->tx_attributes;
	} else {
			/* OUT endpoint */
		ep_size = endpoint->rcv_packetSize;
		attributes = endpoint->rcv_attributes;
	}
       	ep_type = attributes & USB_ENDPOINT_XFERTYPE_MASK;


	if (ep_type == USB_ENDPOINT_XFER_CONTROL) {
		/*pDevReg->ControlEpControl;*/
		pDevReg->ControlEpReserved = 0;
		pDevReg->ControlEpMaxLen = (ep_size & 0xFF);
		pDevReg->ControlEpEpNum = 0;
              elite_ep_enable(0);
	} else if (ep_type == USB_ENDPOINT_XFER_BULK) {
		if (ep_addr & USB_DIR_IN) {
			/*pDevReg->Bulk1EpControl;*/
			pDevReg->Bulk1EpOutEpNum = (ep_addr & 0x7f) << 4;
			pDevReg->Bulk1EpMaxLen = (ep_size & 0xFF);
			pDevReg->Bulk1EpInEpNum = (((ep_addr & 0x7f) << 4) | ((ep_type & 0x700) >> 8));
                     	elite_ep_enable(1);
		} else {
			/*pDevReg->Bulk2EpControl;*/
			pDevReg->Bulk2EpOutEpNum = (ep_addr & 0x7f) << 4;
			pDevReg->Bulk2EpMaxLen = (ep_size & 0xFF);
			pDevReg->Bulk2EpInEpNum = (((ep_addr & 0x7f) << 4) | ((ep_type & 0x700) >> 8));
                     	elite_ep_enable(2);
		}
	} else if (ep_type == USB_ENDPOINT_XFER_INT) {
		/*pDevReg->InterruptEpControl; // Interrupt endpoint control           - 2C*/
		pDevReg->InterruptReserved = (ep_addr & 0x7f) << 4;/*Interrupt endpoint reserved byte  - 2D*/
		pDevReg->InterruptEpMaxLen = (ep_size & 0xFF); /* Interrupt maximum transfer length    - 2E*/
		pDevReg->InterruptEpEpNum = (((ep_addr & 0x7f) << 4) | ((ep_size & 0x700) >> 8));
              	elite_ep_enable(3);
	}

    
	elite_ep = &udc.ep[ep_addr & 0x7f];
       	elite_ep->bEndpointAddress = ep_addr;
	elite_ep->bmAttributes = ep_type;
	elite_ep->double_buf = 0;/*dbuf;*/
       	elite_ep->rndis = 0;
	elite_ep->udc = &udc;
	elite_ep->toggle_bit = 0;
	elite_ep->maxpacket = ep_size;

}

static void elite_pdma0_reset(void)
{

	if (!pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable)
		pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/

	pUdcDmaReg->DMA_Global_Bits.SoftwareReset0 = 1;
	while (pUdcDmaReg->DMA_Global_Bits.SoftwareReset0)/*wait reset complete*/
		;

	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable0 = 1;

	pUdcDmaReg->DMA_Context_Control0_Bis.TransDir = 0;
	
	/*descriptor initial*/

} 


static void elite_pdma1_reset(void)
{

	if (!pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable)
		pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/

	pUdcDmaReg->DMA_Global_Bits.SoftwareReset1 = 1;
	while (pUdcDmaReg->DMA_Global_Bits.SoftwareReset1)/*wait reset complete*/
		;

	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable1 = 1;

	pUdcDmaReg->DMA_Context_Control1_Bis.TransDir = 1;
	
	/*descriptor initial*/

} 

static void elite_pdma_init(void)
{

        memset(UdcPdmaVirAddrLI, 0, 0x100);
        memset(UdcPdmaVirAddrSI, 0, 0x100);
        memset(UdcPdmaVirAddrLO, 0, 0x100);
        memset(UdcPdmaVirAddrSO, 0, 0x100);
        memset(bulk_in_dma, 0,  1024);
        memset(bulk_out_dma1, 0, 512);
        memset(bulk_out_dma2, 0, 512);

	pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/
	pUdcDmaReg->DMA_Global_Bits.SoftwareReset = 1;
	while (pUdcDmaReg->DMA_Global_Bits.SoftwareReset)/*wait reset complete*/
		;
	pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/
	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable0 = 1;
	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable1 = 1;

	pUdcDmaReg->DMA_Context_Control0_Bis.TransDir = 0;
	pUdcDmaReg->DMA_Context_Control1_Bis.TransDir = 1;

} 



/*static int __init wmt_udc_probe(struct device *dev)*/
int  udc_init(void)
{
/*	struct platform_device	*odev = to_platform_device(dev);*/

	dbg("elite_udc_init\n");
	
	pDevReg    = (struct UDC_REGISTER *)USB_UDC_REG_BASE;
	pUdcDmaReg = (struct UDC_DMA_REG *)USB_UDC_DMA_REG_BASE;
	pSetupCommandBuf = (unsigned char *)(USB_UDC_REG_BASE + 0x300);
	SetupBuf = (UCHAR *)(USB_UDC_REG_BASE + 0x340);
	IntBuf = (UCHAR *)(USB_UDC_REG_BASE + 0x40);
	pUSBMiscControlRegister5 = (unsigned char *)(USB_UDC_REG_BASE + 0x1A0);

#ifdef OTGIP
	/*UHDC Global Register Space 0x7F0~0x7F7*/
	pGlobalReg = (struct USB_GLOBAL_REG *) USB_GLOBAL_REG_BASE;
#endif

	*pUSBMiscControlRegister5 = 0x01;
	pDevReg->CommandStatus &= 0x1F;
	pDevReg->CommandStatus |= USBREG_RESETCONTROLLER;

	while (pDevReg->CommandStatus & USBREG_RESETCONTROLLER)
		;

	elite_pdma_init();

	pDevReg->Bulk1EpControl = 0; /* stop the bulk DMA*/
	while (pDevReg->Bulk1EpControl & EP_ACTIVE) /* wait the DMA stopped*/
		;
	pDevReg->Bulk2EpControl = 0; /* stop the bulk DMA*/
	while (pDevReg->Bulk2EpControl & EP_ACTIVE) /* wait the DMA stopped*/
		;
	pDevReg->Bulk1DesStatus = 0x00;
	pDevReg->Bulk2DesStatus = 0x00;

	pDevReg->Bulk1DesTbytes0 = 0;
	pDevReg->Bulk1DesTbytes1 = 0;
	pDevReg->Bulk1DesTbytes2 = 0;

	pDevReg->Bulk2DesTbytes0 = 0;
	pDevReg->Bulk2DesTbytes1 = 0;
	pDevReg->Bulk2DesTbytes2 = 0;

	/* enable DMA and run the control endpoint*/
	pDevReg->ControlEpControl = EP_RUN + EP_ENABLEDMA;
	/* enable DMA and run the bulk endpoint*/
	pDevReg->Bulk1EpControl = EP_RUN + EP_ENABLEDMA;
	pDevReg->Bulk2EpControl = EP_RUN + EP_ENABLEDMA;
	pDevReg->InterruptEpControl = EP_RUN + EP_ENABLEDMA;
	/* enable DMA and run the interrupt endpoint*/
	/* UsbControlRegister.InterruptEpControl = EP_RUN+EP_ENABLEDMA;*/
	/* run the USB controller*/
	pDevReg->MiscControl3 = 0x36;

	/* HW attach process evaluation enable bit For WM3426 and after project*/
	/*pDevReg->FunctionPatchEn |= 0x20;*/
#ifdef HW_BUG_HIGH_SPEED_PHY
	pDevReg->MiscControl0 &= ~0x80;
#endif
	pDevReg->MiscControl0 |= 0x02;

	ControlState = CONTROLSTATE_SETUP;
	TestMode = 0;

	/*udc->chip_version = tmp8;*/
	/* "udc" is now valid*/
	//pullup_disable(udc);
       	pullup_enable();
	udc.ep0_status_0_byte = 0;
	udc.usb_connect = 0;
	pDevReg->SelfPowerConnect |= 0x10;//Neil
       	elite_ep_disable(0);
       	elite_ep_disable(1);
       	elite_ep_disable(2);
       	elite_ep_disable(3);
	return 0;

} /*wmt_udc_probe()*/

static void ep0_status(void)
{
  	struct vt8500_ep	*ep;

  	ep = &udc.ep[0];

	if (udc.ep0_in) {	 /*OUT STATUS*/
		if (udc.ep0_status_0_byte == 1) {
			udc.ep0_status_0_byte = 0;

			/* the data phase is OUT*/

			if (ep->toggle_bit)
				pDevReg->ControlDesTbytes = 0x40|CTRLXFER_DATA1;
			else
				pDevReg->ControlDesTbytes = 0x40|CTRLXFER_DATA0;

			pDevReg->ControlDesControl = CTRLXFER_OUT+CTRLXFER_IOC;
			pDevReg->ControlDesStatus = CTRLXFER_ACTIVE;
			ControlState = CONTROLSTATE_STATUS;
		} /*if(udc->ep0_status_0_byte == 1)*/
	} else {  /*IN STATUS*/
		if (udc.ep0_status_0_byte == 1) {
			udc.ep0_status_0_byte = 0;

			if (ep->toggle_bit)
				pDevReg->ControlDesTbytes = 0x00 | CTRLXFER_DATA1;
			else
				pDevReg->ControlDesTbytes = 0x00 | CTRLXFER_DATA0;

			pDevReg->ControlDesTbytes = 0x00 | CTRLXFER_DATA1; /* zero length*/
			pDevReg->ControlDesControl = CTRLXFER_IN + CTRLXFER_IOC;
			pDevReg->ControlDesStatus = CTRLXFER_ACTIVE;
			ControlState = CONTROLSTATE_STATUS;
		} /*if(udc->ep0_status_0_byte == 1)*/
	}

} /*static void ep0_status(struct vt8500_udc *udc)*/


static void wmt_pdma_reset(void)
{

	if (!pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable)
		pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/

	pUdcDmaReg->DMA_Global_Bits.SoftwareReset = 1;
	while (pUdcDmaReg->DMA_Global_Bits.SoftwareReset)/*wait reset complete*/
		;

	pUdcDmaReg->DMA_Global_Bits.DMAConrollerEnable = 1;/*enable DMA*/
	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable0 = 1;
	pUdcDmaReg->DMA_IER_Bits.DMAInterruptEnable1 = 1;

	pUdcDmaReg->DMA_Context_Control0_Bis.TransDir = 0;
	pUdcDmaReg->DMA_Context_Control1_Bis.TransDir = 1;

	/*descriptor initial*/

} /*wmt_pdma_init*/

static struct usb_endpoint_instance *elite_find_ep (int ep)
{
	int i;

	for (i = 0; i < udc.udc_device->bus->max_endpoints; i++) {
		dbg("udc_device->bus->endpoint_array[%d].endpoint_address = %8.8x\n",i,udc.udc_device->bus->endpoint_array[i].endpoint_address);
		if (udc.udc_device->bus->endpoint_array[i].endpoint_address == ep)
			return &(udc.udc_device->bus->endpoint_array[i]);
	}
	return NULL;
}

static int
elite_udc_dma_irq(void)//, struct pt_regs *r)
{
	int	status = 0;
	u32	bulk_dma_csr;

	bulk_dma_csr = pUdcDmaReg->DMA_ISR;
	/*printk("[udc_dma_isr]dma int_sts = 0x%8.8x\n",pUdcDmaReg->DMA_ISR);*/

	if (bulk_dma_csr & DMA_INTERRUPT_STATUS0) {
		/*	printk("[udc_dma_isr]channel0 event = 0x%8.8x\n",pUdcDmaReg->DMA_Context_Control0);*/
		pUdcDmaReg->DMA_ISR |= DMA_INTERRUPT_STATUS0;
		status = 0;
	}

	if (bulk_dma_csr & DMA_INTERRUPT_STATUS1) {
		/*	printk("[udc_dma_isr]channel1 event = 0x%8.8x\n",pUdcDmaReg->DMA_Context_Control1);*/
		pUdcDmaReg->DMA_ISR |= DMA_INTERRUPT_STATUS1;
		status = 0;
	}

	return status;
}



/* Each USB transfer request using DMA maps to one or more DMA transfers.
 * When DMA completion isn't request completion, the UDC continues with
 * the next DMA transfer for that USB transfer.
 */

static void elite_udc_pdma_des_prepare(unsigned int size, unsigned int dma_phy
							, unsigned char des_type, unsigned char channel)
{
	unsigned int res_size = size;
	volatile unsigned int trans_dma_phy = dma_phy, des_phy_addr = 0, des_vir_addr = 0;
	unsigned int cut_size = 0x40;

	switch (des_type) {
	case DESCRIPTOT_TYPE_SHORT:
	{
		struct _UDC_PDMA_DESC_S *pDMADescS;

		if (channel == TRANS_OUT) {
			des_phy_addr = (unsigned int) UdcPdmaVirAddrSO;
			des_vir_addr = (unsigned int) UdcPdmaVirAddrSO;
			pUdcDmaReg->DMA_Descriptor_Point1 = (unsigned int)(des_phy_addr);
			cut_size = pDevReg->Bulk2EpMaxLen & 0x3ff;
		}	else if (channel == TRANS_IN) {
			des_phy_addr = (unsigned int) UdcPdmaVirAddrSI;
			des_vir_addr = (unsigned int) UdcPdmaVirAddrSI;
			pUdcDmaReg->DMA_Descriptor_Point0 = (unsigned int)(des_phy_addr);
			cut_size = pDevReg->Bulk1EpMaxLen & 0x3ff;
		}	else
			dbg("!! wrong channel %d\n", channel);

		while (res_size) {
			pDMADescS = (struct _UDC_PDMA_DESC_S *) des_vir_addr;

			pDMADescS->Data_Addr = trans_dma_phy;
			pDMADescS->D_Word0_Bits.Format = 0;

//			if (res_size <= 65535) {
			if (res_size <= 32767) {                
				pDMADescS->D_Word0_Bits.i = 1;
				pDMADescS->D_Word0_Bits.End = 1;
				pDMADescS->D_Word0_Bits.ReqCount = res_size;

				res_size -= res_size;
			} else {
				pDMADescS->D_Word0_Bits.i = 0;
				pDMADescS->D_Word0_Bits.End = 0;
				pDMADescS->D_Word0_Bits.ReqCount = 0x8000 - cut_size;

				res_size -= 0x8000;
				des_vir_addr += (unsigned int)DES_S_SIZE;
				trans_dma_phy += (unsigned int)0x8000;
			}
		}
		break;
	}

	case DESCRIPTOT_TYPE_LONG:
	{
		struct _UDC_PDMA_DESC_L *pDMADescL;

		if (channel == TRANS_OUT) {
			des_phy_addr = (unsigned int) UdcPdmaVirAddrLO;
			des_vir_addr = (unsigned int) UdcPdmaVirAddrLO;
			pUdcDmaReg->DMA_Descriptor_Point1 = (unsigned int)(des_phy_addr);
			cut_size = pDevReg->Bulk2EpMaxLen & 0x3ff;
		}	else if (channel == TRANS_IN) {
			des_phy_addr = (unsigned int) UdcPdmaVirAddrLI;
			des_vir_addr = (unsigned int) UdcPdmaVirAddrLI;
			pUdcDmaReg->DMA_Descriptor_Point0 = (unsigned int)(des_phy_addr);
			cut_size = pDevReg->Bulk1EpMaxLen & 0x3ff;
		}	else
			dbg("!! wrong channel %d\n", channel);
		{
			unsigned int * paddr;
			paddr = (unsigned int *)des_vir_addr;
        		memset(paddr, 0, 0x100);
      		}

		while (res_size) {
			pDMADescL = (struct _UDC_PDMA_DESC_L *) des_vir_addr;

			pDMADescL->Data_Addr = trans_dma_phy;
			pDMADescL->D_Word0_Bits.Format = 1;

//			if (res_size <= 65535) {
			if (res_size <= 32767) {                
				pDMADescL->D_Word0_Bits.i = 1;
				pDMADescL->D_Word0_Bits.End = 1;
				pDMADescL->Branch_addr = 0;
				pDMADescL->D_Word0_Bits.ReqCount = res_size;

				res_size -= res_size;
			} else {
				pDMADescL->D_Word0_Bits.i = 0;
				pDMADescL->D_Word0_Bits.End = 0;
				pDMADescL->Branch_addr = des_vir_addr + (unsigned int)DES_L_SIZE;
				pDMADescL->D_Word0_Bits.ReqCount = 0x8000 - cut_size;

				res_size -= 0x8000;
				des_vir_addr += (unsigned int)DES_L_SIZE;
				trans_dma_phy += (unsigned int)0x8000;
			}
		}

		break;
	}

	case DESCRIPTOT_TYPE_MIX:
	break;
	default:
	break;
	}
} 

static u32 dma_src_len(struct usb_endpoint_instance *endpoint, int len)
{
	unsigned  end = 0;
	u32 temp32 = 0;
    
	if ((endpoint->endpoint_address & 0x7F) ==  0) {/*Control In*/
		end = (pDevReg->ControlDesTbytes & 0x7F);
		temp32 = (udc.ep[endpoint->endpoint_address & 0x7F].ep_fifo_length - end);
		return temp32;
	} else if ((endpoint->endpoint_address & 0x7F) ==  1) {/*Bulk In*/
		end =  (pDevReg->Bulk1DesTbytes2 & 0x03) << 16;
		end |=  pDevReg->Bulk1DesTbytes1 << 8;
		end |=  pDevReg->Bulk1DesTbytes0;
	} else if ((endpoint->endpoint_address & 0x7F) ==  3)/*Interrupt In*/
		end = (pDevReg->InterruptDes >> 4);

	temp32 = (len - end);
	return temp32;
} /*static u32 dma_src_len()*/

static u32 dma_dest_len(struct usb_endpoint_instance *endpoint, int len)
{
	/*dma_addr_t	end;*/
	unsigned  end = 0;
	u32 temp32 = 0;


	if ((endpoint->endpoint_address & 0x7F) ==  0) {/*Control out*/
		end = (pDevReg->ControlDesTbytes & 0x7F);
		temp32 = (udc.ep[endpoint->endpoint_address & 0x7F].ep_fifo_length - end);
		return temp32;
	} else if ((endpoint->endpoint_address & 0x7F) ==  2) {/*Bulk Out*/
		end =  (pDevReg->Bulk2DesTbytes2 & 0x03) << 16;
		end |=  pDevReg->Bulk2DesTbytes1 << 8;
		end |=  pDevReg->Bulk2DesTbytes0;
	}

	temp32 = (len - end);
	return temp32;
} /*static u32 dma_dest_len()*/

/* Each USB transfer request using DMA maps to one or more DMA transfers.
 * When DMA completion isn't request completion, the UDC continues with
 * the next DMA transfer for that USB transfer.
 */


static void next_in_dma(struct usb_endpoint_instance *endpoint)
{
	u32 temp32;
	u32 dma_ccr = 0;
	unsigned int length;
	u32	dcmd;
        u32	buf;
        int	is_in, i;
        u8	*pctrlbuf, *pintbuf;
        struct urb *urb = endpoint->tx_urb;
        struct vt8500_ep *ep = &(udc.ep[endpoint->endpoint_address & 0x07f]);
        
        dcmd = length = MIN (urb->actual_length - endpoint->sent,endpoint->tx_packetSize);
        buf = (u32)urb->buffer + endpoint->sent;

        is_in = 1;/*ep->bEndpointAddress & USB_DIR_IN;*/

    

	if (((ep->bEndpointAddress & 0x7F) == 1) &&  (ep->rndis == 0)) {
 		unsigned int dma_length = 1024;

		ep->rndis_buffer_address = (unsigned int)bulk_in_dma;
		ep->rndis_dma_phy_address = (u32)bulk_in_dma;
		ep->rndis_buffer_length = dma_length;
		ep->rndis = 1;

	}
	if (((ep->bEndpointAddress & 0x7F) == 1) && (ep->rndis == 1) && (length > ep->rndis_buffer_length)) {

                dbg( "rndis ooooobb %d\n",length);
	}	

	if ((ep->bEndpointAddress & 0x7F) == 0) {/*Control*/
		ep->ep_fifo_length = 0;

		if (length >= 64)
			length = 64;

		ep->ep_fifo_length = length;
		pctrlbuf = (u8 *)(buf);

		for (i = 0; i < length; i++)
			SetupBuf[i] = pctrlbuf[i];

		if (ep->toggle_bit)
			pDevReg->ControlDesTbytes = length | CTRLXFER_DATA1;
		else
			pDevReg->ControlDesTbytes = length | CTRLXFER_DATA0;

		pDevReg->ControlDesControl = CTRLXFER_IN + CTRLXFER_IOC;
		pDevReg->ControlDesStatus = CTRLXFER_ACTIVE;
		ControlState = CONTROLSTATE_DATA;

		if (ep->toggle_bit == 0)
			ep->toggle_bit = 1;
		else
			ep->toggle_bit = 0;
	} else if (((ep->bEndpointAddress & 0x7F) ==  1)) {/*Bulk In*/
		ep->stall_more_processing  = 0;
		

		if (dcmd > UBE_MAX_DMA)
			dcmd = UBE_MAX_DMA;

		if (pDevReg->Bulk1EpControl & EP_STALL) {
			ep->stall_more_processing = 1;
			ep->temp_dcmd = dcmd;
		}

		if (ep->rndis == 1) {
			memcpy((void *)((u32)ep->rndis_buffer_address), (void *)(buf ), length);
			elite_udc_pdma_des_prepare(dcmd,
			((ep->rndis_dma_phy_address + 0) & 0xFFFFFFFC),
			DESCRIPTOT_TYPE_LONG, TRANS_IN);
		} else
			elite_udc_pdma_des_prepare(dcmd, buf, DESCRIPTOT_TYPE_LONG, TRANS_IN);

		if (pDevReg->Bulk1EpControl & EP_STALL)
			ep->temp_bulk_dma_addr = buf;

		if (pDevReg->Bulk1EpControl & EP_STALL)
			ep->temp_dma_ccr = dma_ccr;

		pDevReg->Bulk1DesStatus = 0x00;
		pDevReg->Bulk1DesTbytes2 |= (dcmd >> 16) & 0x3;
		pDevReg->Bulk1DesTbytes1 = (dcmd >> 8) & 0xFF;
		pDevReg->Bulk1DesTbytes0 = dcmd & 0xFF;

		/* set endpoint data toggle*/
		if (is_in) {
			if (ep->toggle_bit)
				pDevReg->Bulk1DesTbytes2 |= 0x40;/* BULKXFER_DATA1;*/
			else
				pDevReg->Bulk1DesTbytes2 &= 0xBF;/*(~BULKXFER_DATA1);*/

			pDevReg->Bulk1DesStatus = (BULKXFER_IOC | BULKXFER_IN);
		} /*if(is_in)*/

		if (pDevReg->Bulk1EpControl & EP_STALL)
			ep->ep_stall_toggle_bit = ep->toggle_bit;


		/*if((ep->bEndpointAddress & 0x7F) != 0)//!Control*/
		if (length > ep->maxpacket) {
			/*ex : 512 /64 = 8  8 % 2 = 0*/
			temp32 = (length + ep->maxpacket - 1) / ep->maxpacket;
			ep->toggle_bit = ((temp32 + ep->toggle_bit) % 2);
		} else {
			if (ep->toggle_bit == 0)
				ep->toggle_bit = 1;
			else
				ep->toggle_bit = 0;
		}

		/* DMA Channel Control Reg*/
		/* Software Request + Channel Enable*/
		dma_ccr = 0;
		dma_ccr = DMA_RUN;
		if (!is_in)
			dma_ccr |= DMA_TRANS_OUT_DIR;

		if (dcmd) /* PDMA can not support 0 byte transfer*/
			pUdcDmaReg->DMA_Context_Control0 = dma_ccr;

		pDevReg->Bulk1DesStatus |= BULKXFER_ACTIVE;
		
	
		
	} else if ((ep->bEndpointAddress & 0x7F) == 3) {/*Interrupt In*/
		if (dcmd > INT_FIFO_SIZE)
			dcmd = INT_FIFO_SIZE;

		pintbuf = (u8 *)(buf);/* + req->req.actual);*/

		for (i = 0; i <length; i++)
			IntBuf[i] = pintbuf[i];

		pDevReg->InterruptDes = 0x00;
		pDevReg->InterruptDes = (dcmd << 4);

		if (ep->toggle_bit)
			pDevReg->InterruptDes |= INTXFER_DATA1;
		else
			pDevReg->InterruptDes &= 0xF7;

		if (ep->toggle_bit == 0)
			ep->toggle_bit = 1;
		else
			ep->toggle_bit = 0;

		pDevReg->InterruptDes |= INTXFER_IOC;
		pDevReg->InterruptDes |= INTXFER_ACTIVE;
	}
} /*static void next_in_dma()*/


static void elite_udc_epn_tx (int ep)
{
	struct usb_endpoint_instance *endpoint = elite_find_ep (ep);
		
        if(ep == 0x83) /*interupt*/
		   next_in_dma (endpoint);
	else { 
    		if (endpoint->tx_urb && (endpoint->last == endpoint->tx_packetSize)
    		    && (endpoint->tx_urb->actual_length - endpoint->sent - endpoint->last == 0)) {
    		    /* Prepare to transmit a zero-length packet. */
    		    endpoint->sent += endpoint->last;
    		    /* write 0 bytes of data to FIFO */
    		    next_in_dma (endpoint);
    
    	        } else if (endpoint->tx_urb && endpoint->tx_urb->actual_length) {
    		    /* retire the data that was just sent */
    		    usbd_tx_complete (endpoint);
    		    /* Check to see if we have more data ready to transmit
    		     * now.
    		     */
    		    if (endpoint->tx_urb && endpoint->tx_urb->actual_length) {
    		        /* write data to FIFO */
    		        next_in_dma (endpoint);
    		    }
    	        }else{
    		    dbg("[wmt_udc_epn_tx]Neil_test\n");
    		}
	}
}


static int next_out_dma(struct usb_endpoint_instance *endpoint)
{
       u32	dcmd;
       u32	buf;
       int	is_in;
       int   len = 0;
       struct urb *urb = endpoint->tx_urb;
       struct vt8500_ep *ep = &(udc.ep[endpoint->endpoint_address & 0x07f]);


	is_in = 0;/*ep->bEndpointAddress & USB_DIR_IN;*/

        dcmd =  MIN (urb->actual_length - endpoint->sent,endpoint->tx_packetSize);
        buf = (u32)urb->buffer + endpoint->sent;


	if ((ep->bEndpointAddress == 2) && (ep->rndis == 0)) {

		unsigned int dma_length = 1024;

		ep->rndis_buffer_address = (unsigned int)bulk_out_dma1;
		ep->rndis_dma_phy_address = (u32)bulk_out_dma1;
		ep->rndis_buffer_length = dma_length;
		ep->rndis = 1;

	}
	if ((ep->bEndpointAddress == 2) && (ep->rndis == 1) && (dcmd > ep->rndis_buffer_length)) {

          dbg( "rndis ooooobb %d\n",dcmd);
	}

	if (ep->bEndpointAddress == 0) {/*Control*/
		ep->ep_fifo_length = 0;

		if (dcmd >= 64)
			dcmd = 64;

		ep->ep_fifo_length = dcmd;

		if (ep->toggle_bit)
			pDevReg->ControlDesTbytes = dcmd | CTRLXFER_DATA1;
		else
			pDevReg->ControlDesTbytes = dcmd | CTRLXFER_DATA0;

		pDevReg->ControlDesControl = CTRLXFER_OUT+CTRLXFER_IOC;
		pDevReg->ControlDesStatus = CTRLXFER_ACTIVE;
		ControlState = CONTROLSTATE_DATA;

		if (ep->toggle_bit == 0)
			ep->toggle_bit = 1;
		else
			ep->toggle_bit = 0;

	} else if (ep->bEndpointAddress  == 2) {/*Bulk Out*/
		ep->stall_more_processing  = 0;


		/*if(req->dma_bytes == 64)*/
		ep->udc->cbw_virtual_address =  (buf & 0xFFFFFFFC);

		if (dcmd > UBE_MAX_DMA)
			dcmd = UBE_MAX_DMA;

		if (pDevReg->Bulk2EpControl & EP_STALL) {
			ep->stall_more_processing = 1;
			ep->temp_dcmd = dcmd;
		} 
              len = dma_dest_len(endpoint,512);
              {
                     unsigned char *cp = urb->buffer + urb->actual_length;
                     int i = len;
                     for(i = 0; i < len; i++) {
                        cp[i] = ((u8*)udc.ep[2].temp_buffer_address)[i];
                     }
             }

              if(udc.ep[2].temp_buffer_address == (u32)bulk_out_dma1)
                    bulk_out_prepare_dma(bulk_out_dma2);
              else
                    bulk_out_prepare_dma(bulk_out_dma1);

       }
       return len; 
} /*static void next_out_dma()*/


static void elite_udc_epn_rx (int ep)
{
	/* Check endpoint status */

	int nbytes;
	struct usb_endpoint_instance *endpoint = elite_find_ep (ep);

	nbytes = next_out_dma (endpoint);
	usbd_rcv_complete (endpoint, nbytes, 0);
}


static void dma_irq(int addr)
{
	if ((addr & 0x7F) == 1) {/*Bulk In*/
  	 elite_udc_epn_tx(0x81);
	} else if ((addr & 0x7F) == 2) {/*Bulk Out*/
  	 elite_udc_epn_rx(0x2);
	} else if ((addr & 0x7F) == 3) {/*Interrupt In*/
	 elite_udc_epn_tx(0x83);	
	} /*EP3 : Interrupt In*/
}//static void dma_irq()


static void devstate_irq(u32 port0_status)
{
	udc.usb_connect = 0;

	if (pDevReg->IntEnable & INTENABLE_DEVICERESET) {
		/*vt3357 hw issue : clear all device register.*/
		/*payload & address needs re-set again...*/
		pDevReg->IntEnable |= INTENABLE_SUSPENDDETECT;
		dbg("devstate_irq() - Global Reset...\n");

		udc.ep0_set_config = 0;
		pDevReg->Bulk1DesStatus = 0;

		udc.ep[0].toggle_bit = 0;
		udc.ep[1].toggle_bit = 0;
		udc.ep[2].toggle_bit = 0;
		udc.ep[3].toggle_bit = 0;


		udc.ep[0].rndis = 0;
		udc.ep[1].rndis = 0;
		udc.ep[2].rndis = 0;
		udc.ep[3].rndis = 0;
		udc.file_storage_set_halt = 0;
		udc.bulk_out_dma_write_error = 0;

		udc.speed = USB_SPEED_LOW;
		wmt_pdma_reset();
              	usbd_device_event_irq (udc.udc_device, DEVICE_RESET, 0);
		/*vt8500_usb_device_reg_dump();*/

	} /*if (pDevReg->IntEnable & INTENABLE_DEVICERESET)*/

	if (pDevReg->IntEnable & INTENABLE_SUSPENDDETECT) {
		dbg("devstate_irq() - Global Suspend...\n");
		pDevReg->IntEnable |= INTENABLE_SUSPENDDETECT;
              usbd_device_event_irq (udc.udc_device, DEVICE_BUS_INACTIVE, 0);

	} /*if (pDevReg->IntEnable & INTENABLE_SUSPENDDETECT)*/

	if (pDevReg->IntEnable & INTENABLE_RESUMEDETECT) {
		pDevReg->IntEnable |= INTENABLE_RESUMEDETECT;
              usbd_device_event_irq (udc.udc_device, DEVICE_BUS_ACTIVITY, 0);
		dbg("devstate_irq() - Global Resume...\n");
	} /*if (pDevReg->IntEnable & INTENABLE_RESUMEDETECT)*/

/*#ifdef UDC_A1_SELF_POWER_ENABLE*/
	/* USB Bus Connection Change*/
	/* clear connection change event*/
	if (pDevReg->PortControl & PORTCTRL_SELFPOWER)/* Device port control register         - 22)*/
		if (pDevReg->PortControl & PORTCTRL_CONNECTCHANGE)/* Device port control register   - 22)*/
			pDevReg->PortControl |= PORTCTRL_CONNECTCHANGE;/*   0x02 // connection change bit*/

/*#endif*/

	if (pDevReg->PortControl & PORTCTRL_FULLSPEEDMODE) {
		udc.speed = USB_SPEED_FULL;
		udc.usb_connect = 1;
		/*2007-8.27 GigaHsu : enable float, reset, suspend and resume IE*/
		/*after host controller connected.*/
		dbg("devstate_irq() - full speed host connect...\n");
	} /*if(pDevReg->PortControl & PORTCTRL_FULLSPEEDMODE)*/

	if (pDevReg->PortControl & PORTCTRL_HIGHSPEEDMODE) {
		udc.speed = USB_SPEED_HIGH;
		udc.usb_connect = 1;
		/*2007-8.27 GigaHsu : enable float, reset, suspend and resume IE*/
		/*after host controller connected.*/
		dbg("devstate_irq() - high speed host connect...\n");
	} /*if(pDevReg->PortControl & PORTCTRL_HIGHSPEEDMODE)*/

} /*static void devstate_irq()*/

void USB_ControlXferComplete(void)
{
	struct vt8500_ep	*ep;
        struct usb_endpoint_instance *endpoint ;
	ep = &udc.ep[0];
	/* when ever a setup received, the Control state will reset*/
	/* check for the valid bit of the contol descriptor*/
	/*DBG("USB_ControlXferComplete()\n");*/
	if (pDevReg->ControlDesControl & CTRLXFER_CMDVALID) {
		ep->toggle_bit = 1;
		if (udc.usb_connect == 0) {
			if (pDevReg->PortControl & PORTCTRL_FULLSPEEDMODE) {
				udc.speed = USB_SPEED_FULL;
				udc.usb_connect = 1;
			//	udc->is_dualspeed = 1;
				/*2007-8.27 GigaHsu : enable float, reset, suspend and resume IE*/
				/*after host controller connected.*/
				elite_udc_csr();
				dbg("devstate_irq() - full speed host connect...\n");
                            USBState = USBSTATE_DEFAULT;
			} /*if(pDevReg->PortControl & PORTCTRL_FULLSPEEDMODE)*/

			if (pDevReg->PortControl & PORTCTRL_HIGHSPEEDMODE) {
				udc.speed = USB_SPEED_HIGH;
				udc.usb_connect = 1;
				// udc->is_dualspeed = 1;
				/*2007-8.27 GigaHsu : enable float, reset, suspend and resume IE*/
				/*after host controller connected.*/
				elite_udc_csr();
				dbg("devstate_irq() - high speed host connect...\n");
                            USBState = USBSTATE_DEFAULT;
			} /*if(pDevReg->PortControl & PORTCTRL_HIGHSPEEDMODE)*/
		}
		/* HP11_Begin*/
		/* clear the command valid bit*/
		/* pDevReg->ControlDesControl |= CTRLXFER_CMDVALID;*/
		/* always clear control stall when SETUP received*/
		pDevReg->ControlEpControl &= 0x17; /* clear the stall*/
		ControlState = CONTROLSTATE_DATA;
		udc_control_prepare_data_resp();  /*processing SETUP command ...*/
		pDevReg->ControlDesControl &= 0xEF;/*(~CTRLXFER_CMDVALID);*/
		/*HP11_End*/
		return;
	}
        endpoint = elite_find_ep (0);
        endpoint->tx_urb = ep0_urb;
	endpoint->sent = 0;
        if (udc.ep0_in) {
		/* IN dma: tx to host*/
	        ep->udc->ep0_status_0_byte = 1;

		 while (pDevReg->ControlEpControl & EP_COMPLETEINT) /* clear the event*/
			;
		next_in_dma(endpoint);
        } else {/*ep0 out*/
		/* OUT dma: rx from host*/
                ep->udc->ep0_status_0_byte = 1;
                
		  while (pDevReg->ControlEpControl & EP_COMPLETEINT) /* clear the event*/
			;
		
 		  next_out_dma(endpoint);
	}
        

} /*void USB_ControlXferComplete(void)*/




/*UDC_IS_SETUP_INT*/
static void udc_control_prepare_data_resp(void)/*(struct vt8500_udc *udc, u32 udc_irq_src)*/
{
	struct vt8500_ep	*ep0 = &udc.ep[0];
	/*UCHAR Result = RESPFAIL;*/
	/*unsigned char CurXferLength = 0;*/
	/*unsigned char Control_Length = 0;*/
	int i;
	u8 test_mode_enable = 0;


	/*ep0->irqs++;*/
	dbg("ep0_irq()\n");
	ep0->toggle_bit = 1;


	/* SETUP starts all control transfers*/
	{
		union u {
			u8			bytes[8];
			struct usb_device_request	r;
		} u;
		int	status = -EINVAL;
              	unsigned char *datap =
			(unsigned char *) &ep0_urb->device_request;
               
		struct vt8500_ep *ep = &udc.ep[0];
		int    ep0_status_phase_0_byte = 0;

		// nuke(ep0, 0);
		/* read the (latest) SETUP message*/
		for (i = 0; i <= 7; i++)
			datap[i] = u.bytes[i] = pSetupCommandBuf[i];

		le32_to_cpus(&u.r.wValue);
		le32_to_cpus(&u.r.wIndex);
		le32_to_cpus(&u.r.wLength);

		/* Delegate almost all control requests to the gadget driver,*/
		/* except for a handful of ch9 status/feature requests that*/
		/* hardware doesn't autodecode _and_ the gadget API hides.*/
		/**/
		udc.ep0_in = (u.r.bmRequestType & USB_DIR_IN) != 0;
		udc.ep0_set_config = 0;
		udc.ep0_pending = 1;
		udc.ep0_in_status = 0;

		ep0->stopped = 0;
		ep0->ackwait = 0;

		if ((u.r.bmRequestType & USB_RECIP_OTHER) == USB_RECIP_OTHER) {/*USB_RECIP_OTHER(0x03)*/
			status = 0;

			/*INFO("ep0_irq() setup command[0]=(0x%08X)\n",u.dword[0]);*/
			/*INFO("ep0_irq() setup command[1]=(0x%08X)\n",u.dword[1]);*/
			/*INFO("ep0_irq() address(%08X)", udc_reg->AddressControl);*/
			goto delegate;
		}

		switch (u.r.bRequest) {
		case USB_REQ_SET_CONFIGURATION:
			/* udc needs to know when ep != 0 is valid*/
			/*SETUP 00.09 v0001 i0000 l0000*/
			if (u.r.bmRequestType != USB_RECIP_DEVICE)/*USB_RECIP_DEVICE(0x00)*/
				goto delegate;
			if (u.r.wLength != 0)
				goto do_stall;
			udc.ep0_set_config = 1;
			udc.ep0_reset_config = (u.r.wValue == 0);
			dbg("set config %d\n", u.r.wValue);

			if (u.r.wValue == 0) {
                              usbd_device_event_irq (udc.udc_device, DEVICE_ADDRESS_ASSIGNED, 0);
                              USBState = USBSTATE_ADDRESS;
			}else {
			         usbd_device_event_irq (udc.udc_device, DEVICE_CONFIGURED, 0);
                              USBState = USBSTATE_CONFIGED;
                     }
			/* update udc NOW since gadget driver may start*/
			/* queueing requests immediately; clear config*/
			/* later if it fails the request.*/
			/**/

			udc.ep[0].toggle_bit = 0;
			udc.ep[1].toggle_bit = 0;
			udc.ep[2].toggle_bit = 0;
			udc.ep[3].toggle_bit = 0;


			status = 0;
			goto delegate;

		/* Giga Hsu : 2007.6.6 This would cause set interface status 0
			return to fast and cause bulk endpoint in out error*/
		case USB_REQ_SET_INTERFACE:

			dbg("set interface %d\n", u.r.wValue);
			status = 0;

			udc.ep[0].toggle_bit = 0;
			udc.ep[1].toggle_bit = 0;
			udc.ep[2].toggle_bit = 0;
			udc.ep[3].toggle_bit = 0;

			goto delegate;
		    /*break;*/

		case USB_REQ_CLEAR_FEATURE:
			/* clear endpoint halt*/

			if (u.r.bmRequestType != USB_RECIP_ENDPOINT)
				goto delegate;

			if (u.r.wValue != USB_ENDPOINT_HALT
			|| u.r.wLength != 0)
				goto do_stall;

			ep = &udc.ep[u.r.wIndex & 0xf];

			if (ep != ep0) {
				if (ep->bmAttributes == USB_ENDPOINT_XFER_ISOC || !ep->desc)
					goto do_stall;

				if (udc.file_storage_set_halt == 0) {
					switch ((ep->bEndpointAddress & 0x7F)) {
					case 0:/*Control In/Out*/
					   pDevReg->ControlEpControl &= 0xF7;
					break;

					case 1:/*Bulk In*/

						pDevReg->Bulk1EpControl &= 0xF7;
						if (ep->stall_more_processing == 1) {
							u32 dma_ccr = 0;

							ep->stall_more_processing = 0;
							elite_pdma0_reset();
							elite_udc_pdma_des_prepare(ep->temp_dcmd,
								ep->temp_bulk_dma_addr,
								DESCRIPTOT_TYPE_LONG, TRANS_IN);

							pDevReg->Bulk1DesStatus = 0x00;
							pDevReg->Bulk1DesTbytes2 |=
								(ep->temp_dcmd >> 16) & 0x3;
							pDevReg->Bulk1DesTbytes1 =
								(ep->temp_dcmd >> 8) & 0xFF;
							pDevReg->Bulk1DesTbytes0 =
								ep->temp_dcmd & 0xFF;

							/* set endpoint data toggle*/
							if (ep->ep_stall_toggle_bit) {
								/* BULKXFER_DATA1;*/
								pDevReg->Bulk1DesTbytes2 |= 0x40;
							} else {
								/*(~BULKXFER_DATA1);*/
								pDevReg->Bulk1DesTbytes2 &= 0xBF;
							}

							pDevReg->Bulk1DesStatus =
								(BULKXFER_IOC | BULKXFER_IN);

							/* DMA Channel Control Reg*/
							/* Software Request + Channel Enable*/
							dma_ccr = 0;
							dma_ccr = DMA_RUN;
							pUdcDmaReg->DMA_Context_Control0 = dma_ccr; //neil

							pDevReg->Bulk1DesStatus |= BULKXFER_ACTIVE;
						}
					break;

					case 2:/*Bulk Out*/
						pDevReg->Bulk2EpControl &= 0xF7;

						if (ep->stall_more_processing == 1) {
							u32 dma_ccr = 0;
							ep->stall_more_processing  = 0;
							elite_pdma1_reset();
							elite_udc_pdma_des_prepare(ep->temp_dcmd,
								ep->temp_bulk_dma_addr,
							DESCRIPTOT_TYPE_LONG, TRANS_OUT);
							/* DMA Global Controller Reg*/
							/* DMA Controller Enable +*/
							/* DMA Global Interrupt Enable(if any TC, error,
								or abort status in any channels occurs)*/

							pDevReg->Bulk2DesStatus = 0x00;
							pDevReg->Bulk2DesTbytes2 |=
								(ep->temp_dcmd >> 16) & 0x3;
							pDevReg->Bulk2DesTbytes1 =
								(ep->temp_dcmd >> 8) & 0xFF;
							pDevReg->Bulk2DesTbytes0 =
								ep->temp_dcmd & 0xFF;

							if (ep->ep_stall_toggle_bit)
								pDevReg->Bulk2DesTbytes2 |= BULKXFER_DATA1;
							else
								pDevReg->Bulk2DesTbytes2 &= 0x3F;

							pDevReg->Bulk2DesStatus = BULKXFER_IOC;

							/* DMA Channel Control Reg*/
							/* Software Request + Channel Enable*/

							/*udc_bulk_dma_dump_register();*/
							dma_ccr = 0;
							dma_ccr = DMA_RUN;
							dma_ccr |= DMA_TRANS_OUT_DIR;
							pUdcDmaReg->DMA_Context_Control1 = dma_ccr;
							pDevReg->Bulk2DesStatus |= BULKXFER_ACTIVE;
						}
					break;

					case 3:/*Interrupt In*/
						pDevReg->InterruptEpControl &= 0xF7;
					break;
					}

					ep->stall = 0;
					ep->stopped = 0;
					/**ep->reg_irp_descriptor = ep->temp_irp_descriptor;*/
				}
			} /*if (ep != ep0)*/

			ep0_status_phase_0_byte = 1;
			dbg("%s halt cleared by host\n", ep->name);
			/*goto ep0out_status_stage;*/
			status = 0;
			udc.ep0_pending = 0;
		/*break;*/
			goto delegate;

		case USB_REQ_SET_FEATURE:
			/* set endpoint halt*/

			if ((u.r.wValue == USB_DEVICE_TEST_MODE)) {/*(USBState == USBSTATE_DEFAULT) &&*/
				TestMode = (UCHAR)(u.r.wIndex >> 8);
				test_mode_enable = 1;
				dbg("USB_REQ_SET_FEATURE - TestMode (0x%02X)\n", TestMode);

				/*ControlState = CONTROLSTATE_STATUS;*/
				ep0_status_phase_0_byte = 1;
				status = 0;
				break;
			}

			if (u.r.bmRequestType != USB_RECIP_ENDPOINT)
				goto delegate;

			if (u.r.wValue != USB_ENDPOINT_HALT || u.r.wLength != 0)
				goto do_stall;

			ep = &udc.ep[u.r.wIndex & 0xf];

			if (ep->bmAttributes == USB_ENDPOINT_XFER_ISOC || ep == ep0 || !ep->desc)
				goto do_stall;

			switch ((ep->bEndpointAddress & 0x7F)) {
			case 0:/*Control In/Out*/
				pDevReg->ControlEpControl |= EP_STALL;
			break;

			case 1:/*Bulk In*/
				pDevReg->Bulk1EpControl |= EP_STALL;
			break;

			case 2:/*Bulk Out*/
				pDevReg->Bulk2EpControl |= EP_STALL;
			break;

			case 3:/*Interrupt In*/
				pDevReg->InterruptEpControl |= EP_STALL;
			break;
			}
			ep->stall = 1;
			ep->stopped = 1;

			ep0_status_phase_0_byte = 1;
			/*use_ep(ep, 0);*/
			/* can't halt if fifo isn't empty...*/
			/*UDC_CTRL_REG = UDC_CLR_EP;*/
			/*UDC_CTRL_REG = UDC_SET_HALT;*/
			dbg("%s halted by host\n", ep->name);
			/*ep0out_status_stage:*/
			status = 0;

			udc.ep0_pending = 0;
			/*break;*/
			goto delegate;

		case USB_REQ_GET_STATUS:
			/* return interface status.  if we were pedantic,*/
			/* we'd detect non-existent interfaces, and stall.*/
			/**/

			if (u.r.bmRequestType == (USB_DIR_IN|USB_RECIP_ENDPOINT)) {
				ep = &udc.ep[u.r.wIndex & 0xf];

				if (ep->stall == 1) {
					udc.ep0_in_status = 0x01;

					/*GgiaHsu-B 2007.08.10 : patch HW Bug :
						MSC Compliance Test : Error Recovery Items.*/
					ep = &udc.ep[3];
					if ((udc.file_storage_set_halt == 1) && (ep->stall == 1))
						ep->stall = 1;
					/*GgiaHsu-E 2007.08.10 :
						--------------------------------------------*/

					ep = &udc.ep[0];
					ep->stall = 0;
				} else
					udc.ep0_in_status = 0x00;

				dbg("GET_STATUS, interface wIndex(0x%02X) ep0_in_status(0x%04X)\n"
					, u.r.wIndex, udc.ep0_in_status);
			} else
				udc.ep0_in_status = 0x00;

			/* return two zero bytes*/
			status = 0;

			/* next, status stage*/
			goto delegate;
			break;

		case USB_REQ_SET_ADDRESS:
			if (u.r.bmRequestType == USB_RECIP_DEVICE) { /*USB_RECIP_DEVICE(0x00)*/
				if (USBState == USBSTATE_DEFAULT) {
					if (u.r.wValue != 0){
						usbd_device_event_irq (udc.udc_device, DEVICE_ADDRESS_ASSIGNED, 0);
                                          USBState = USBSTATE_ADDRESS;
                                  }
				} else if (USBState == USBSTATE_ADDRESS) {
					if (u.r.wValue == 0){
                                           usbd_device_event_irq (udc.udc_device, DEVICE_RESET, 0);
						USBState = USBSTATE_DEFAULT;
                                   }
				}

				pDevReg->DeviceAddr |= (DEVADDR_ADDRCHANGE | u.r.wValue);
				dbg("USB_REQ_SET_ADDRESS 0x%03d\n", u.r.wValue);
				ControlState = CONTROLSTATE_STATUS;
				ep0_status_phase_0_byte = 1;
				status = 0;
			}
		break;

		case USB_BULK_RESET_REQUEST:

			dbg("USB_BULK_RESET_REQUEST\n");
			udc.file_storage_set_halt = 0;

			udc.ep[0].toggle_bit = 0;
			udc.ep[1].toggle_bit = 0;
			udc.ep[2].toggle_bit = 0;
			udc.ep[3].toggle_bit = 0;

			status = 0;

			goto delegate;
			/*break;*/

		default:
delegate:
			dbg("SETUP %02x.%02x v%04x i%04x l%04x\n",
			u.r.bmRequestType, u.r.bRequest,
			u.r.wValue, u.r.wIndex, u.r.wLength);
			  /*
			// The gadget driver may return an error here,
			// causing an immediate protocol stall.
			//
			// Else it must issue a response, either queueing a
			// response buffer for the DATA stage, or halting ep0
			// (causing a protocol stall, not a real halt).  A
			// zero length buffer means no DATA stage.
			//
			// It's fine to issue that response after the setup()
			// call returns, and this IRQ was handled.
			//
			*/
			udc.ep0_setup = 1;
			/*usb gadget driver prepare setup data phase(control in)*/
		    //	status = udc->driver->setup(&udc->gadget, &u.r); /*usb_gadget_driver->setup()*/

                    	if ((status = ep0_recv_setup (ep0_urb)) < 0) {
                		/* Not a setup packet, stall next EP0 transaction */
                		dbg ("can't parse setup packet, still waiting for setup");
                	}
			udc.ep0_setup = 0;
		} /*switch (u.r.bRequest)*/
		if (ep0_status_phase_0_byte == 1) {/* && (udc->ep[0].stall==0))*/
			udc.ep0_status_0_byte = 1;
			if (test_mode_enable == 1)
				ep0_status();
		}

		if (status < 0) {
do_stall:
			/* ep = &udc->ep[0];*/
			/**ep->reg_control_status |= UDC_EP0_STALL;*/
			/* fail in the command parsing*/
			pDevReg->ControlEpControl |= EP_STALL;  /* stall the pipe*/
			ControlState = CONTROLSTATE_SETUP;      /* go back to setup state*/

			ep->stall = 1;
			dbg("Setup Command STALL : req %02x.%02x protocol STALL; stat %d\n",
			u.r.bmRequestType, u.r.bRequest, status);

			udc.ep0_pending = 0;
		} /*if (status < 0)*/

	} /*if (udc_irq_src & UDC_IS_SETUP_INT)*/
} /*udc_control_prepare_data_resp()*/

void udc_irq(void)
{
       	u32 udc_irq_src;
	int	status = 0;
#ifdef OTGIP
	u32 global_irq_src;
#endif

        dbg("elite_udc_irq()\n");

#ifdef OTGIP
	/*Global Interrupt Status*/
	global_irq_src = pGlobalReg->UHDC_Interrupt_Status;

	if (global_irq_src & UHDC_INT_UDC) {/*UDC Core + UDC DMA1 + UDC DMA2 */
		if (global_irq_src & UHDC_INT_UDC_CORE) {/*UDC Core*/
#endif
			/*connection interrupt*/
		if (pDevReg->SelfPowerConnect & 0x02) {			
			if (pDevReg->SelfPowerConnect & 0x01) {         
				wmt_pdma_reset();               
                     }else {
    			    dbg("disconnect 1\n"); //gri
                         gadget_connect=0;                                 
        		}
			status = 0;
			pDevReg->SelfPowerConnect |= 0x02;
		}

		/* Device Global Interrupt Pending Status*/
		udc_irq_src = pDevReg->CommandStatus;

		/* Device state change (usb ch9 stuff)*/
		if (udc_irq_src & USBREG_BUSACTINT) {/* the bus activity interrupt occured*/
			/*caused by Port 0 Statsu Change*/
			devstate_irq(udc_irq_src);
			status = 0;
			pDevReg->CommandStatus |= USBREG_BUSACTINT;
		}

        	if (udc_irq_src & USBREG_BABBLEINT) {/* the Babble interrupt ocuured*/
				/* check for Control endpoint for BABBLE error*/
			if (pDevReg->ControlEpControl & EP_BABBLE) {
		 			pDevReg->ControlEpControl |= (EP_BABBLE + EP_STALL);
					dbg("elite_udc_irq - EP0 Babble Detect!\n");
					udc.ep[0].stopped = 1;
					udc.ep[0].stall = 1;
				}

				if (pDevReg->Bulk1EpControl & EP_BABBLE) {
					/* the Bulk endpoint is encounted the babble error*/
					/* stall the endpoint and clear the babble condition*/
					pDevReg->Bulk1EpControl |= (EP_BABBLE + EP_STALL);
					dbg("elite_udc_irq - EP1 Babble Detect!\n");
					udc.ep[1].stopped = 1;
					udc.ep[1].stall = 1;
				}

				if (pDevReg->Bulk2EpControl & EP_BABBLE) {
					/* the Bulk endpoint is encounted the babble error*/
					/* stall the endpoint and clear the babble condition*/
					pDevReg->Bulk2EpControl |= (EP_BABBLE + EP_STALL);
					dbg("elite_udc_irq - EP2 Babble Detect!\n");
					udc.ep[2].stopped = 1;
					udc.ep[2].stall = 1;
				}
				status = IRQ_HANDLED;
			}

			if (udc_irq_src & USBREG_COMPLETEINT) {/* the complete inerrupt occured*/
				if (pDevReg->ControlEpControl & EP_COMPLETEINT) {
					/* the control transfer complete event*/
					pDevReg->ControlEpControl |= EP_COMPLETEINT; /* clear the event*/
					USB_ControlXferComplete();

				}

				if (pDevReg->Bulk1EpControl & EP_COMPLETEINT) {
					/* the bulk transfer complete event*/
					pDevReg->Bulk1EpControl |= EP_COMPLETEINT;
					/*DBG("USB_Bulk 1 DMA()\n");*/
					dma_irq(0x81);
				}

				if (pDevReg->Bulk2EpControl & EP_COMPLETEINT) {
					/* the bulk transfer complete event*/
					pDevReg->Bulk2EpControl |= EP_COMPLETEINT;
					/*DBG("USB_Bulk 2 DMA()\n");*/
					dma_irq(2);
				}

				if (pDevReg->InterruptEpControl & EP_COMPLETEINT) {
					/* the bulk transfer complete event*/
					pDevReg->InterruptEpControl |= EP_COMPLETEINT;
					/*DBG("USB_INT 3 DMA()\n");*/
					dma_irq(0x83);
				}
				status = 0;
			}

			if	(udc.ep0_status_0_byte == 1)/* && (udc_ep[0].stall==0))*/
				ep0_status();

			if (ControlState == CONTROLSTATE_STATUS) {
				/* checking for test mode*/
				if (TestMode != 0x00) {
					pDevReg->CommandStatus &= 0x1F;
					pDevReg->CommandStatus |= USBREG_RESETCONTROLLER;
					while (pDevReg->CommandStatus & USBREG_RESETCONTROLLER)
						;
					/* HW attach process evaluation enable bit*/
					pDevReg->FunctionPatchEn |= 0x20;
					/* Device port control register - 22*/
					pDevReg->PortControl |= PORTCTRL_SELFPOWER;

					/*GigaHsu 2008.1.26 : Don't set RUN bit while TestMode enable*/
					/* setting the test mode*/
					switch (TestMode) {
					case UDC_TEST_MODE_NOT_ENABLED:/*0*/
						dbg("UDC_TEST_MODE_NOT_ENABLED (0x%02X)\n", TestMode);
					       break;

					case UDC_TEST_J_STATE:/*1*/
						pDevReg->CommandStatus = USBREG_TEST_J;
						dbg("UDC_TEST_J_STATE wIndex(0x%02X)\n", TestMode);
					        break;

					case UDC_TEST_K_STATE:/*2*/
						pDevReg->CommandStatus = USBREG_TEST_K;
						dbg("UDC_TEST_K_STATE wIndex(0x%02X)\n", TestMode);
					       break;

					case UDC_TEST_SE0_NAK:/*3*/
						pDevReg->CommandStatus = USBREG_SE0_NAK;
						dbg("UDC_TEST_SE0_NAK wIndex(0x%02X)\n", TestMode);
					       break;

					case UDC_TEST_PACKET:/*4*/
						pDevReg->CommandStatus = USBREG_TESTPACKET;
						dbg("UDC_TEST_PACKET wIndex(0x%02X)\n", TestMode);
					       break;

					case UDC_TEST_FRORCE_ENABLE:/*5*/
						pDevReg->CommandStatus = USBREG_FORCEENABLE;
						dbg("UDC_TEST_FRORCE_ENABLE wIndex(0x%02X)\n", TestMode);
					       break;

					case UDC_TEST_EYE_PATTERN:/*6*/
						pDevReg->CommandStatus = USBREG_TESTEYE;
						dbg("UDC_TEST_EYE_PATTERN wIndex(0x%02X)\n", TestMode);
					       break;
					} /*switch(TestMode)*/

					dbg("UDC - CommandStatus(0x%02X)\n", pDevReg->CommandStatus);
					/* stop the 8051*/
					/*ChipTopRegister.Config |= CT_STOP8051;*/
				}
				ControlState = CONTROLSTATE_SETUP; /* go back SETUP state*/
			}
#ifdef OTGIP
		}
		if (global_irq_src & UHDC_INT_UDC_DMA1)/*UDC Bulk DMA 1*/
			elite_udc_dma_irq();//, r);

		if (global_irq_src & UHDC_INT_UDC_DMA2)/*UDC Bulk DMA 1*/
			elite_udc_dma_irq();//, r);

	}
#endif

	//return status;
}

/* udc_enable
 *
 * Grab an EP0 URB, register interest in a subset of USB events
 */
void udc_enable (struct usb_device_instance *device)
{

        udc.udc_device = device;
	if (!ep0_urb) {
		ep0_urb = usbd_alloc_urb (device, device->bus->endpoint_array);
	}

	pDevReg->CommandStatus |= USBREG_RUNCONTROLLER;

	return;
}

/* udc_disable
 *
 * disable the currently hooked device
 */
void udc_disable (void)
{
	udc.udc_device =  NULL;
        
	if (ep0_urb) {
		usbd_dealloc_urb (ep0_urb);
		ep0_urb= 0;
	}

	pDevReg->CommandStatus &= ~USBREG_RUNCONTROLLER;
}

/* udc_startup_events
 *
 * Enable the specified device
 */
void udc_startup_events (struct usb_device_instance *device)
{
	/* The DEVICE_INIT event puts the USB device in the state STATE_INIT. */
	usbd_device_event_irq(device, DEVICE_INIT, 0);

	/*
	 * The DEVICE_CREATE event puts the USB device in the state
	 * STATE_ATTACHED.
	 */
	usbd_device_event_irq(device, DEVICE_CREATE, 0);

	/*
	 * Some USB controller driver implementations signal
	 * DEVICE_HUB_CONFIGURED and DEVICE_RESET events here.
	 * DEVICE_HUB_CONFIGURED causes a transition to the state STATE_POWERED,
	 * and DEVICE_RESET causes a transition to the state STATE_DEFAULT.
	 * The DW USB client controller has the capability to detect when the
	 * USB cable is connected to a powered USB bus, so we will defer the
	 * DEVICE_HUB_CONFIGURED and DEVICE_RESET events until later.
	 */

	udc_enable(device);
}



/* udc_connect
 *
 * Move state, switch on the USB
 */
void udc_connect (void)
{
	/* Enable pull-up resistor on D+
	 * TODO: fit a pull-up resistor to drive SE0 for > 2.5us
	 */

	pullup_enable();
}

/* udc_disconnect
 *
 * Disconnect is not used but, is included for completeness
 */
void udc_disconnect (void)
{
	/* Disable pull-up resistor on D-
	 * TODO: fix a pullup resistor to control this
	 */
        pullup_disable();
}

int udc_endpoint_write (struct usb_endpoint_instance *endpoint)
{
	if (endpoint->tx_urb) {
		next_in_dma(endpoint);
	}
       	return 0;
}

#endif

