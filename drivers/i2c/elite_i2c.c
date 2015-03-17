/*
 * S3 Graphics (elite1000) I2C driver.
 *
 * Copyright (C) 2012 S3 Graphics
 *
 * --------------------------------------------------------
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <i2c.h>

#include "elite_i2c.h"

#define STATUS_PASS 0
#define STATUS_FAIL 1
#define STATUS_HANG 2

#define __iomem	__attribute__((noderef, address_space(2)))


static int i2c_bus_num = 0;

/* every adapter generates one of this struct */
struct elite_i2c_priv {
    int                    dev_id;
    unsigned int           master;

    struct elite_isr_status    isr_stat;
    enum elite_i2c_mode        i2c_mode;

    unsigned int           irq;
    void __iomem           *regs;
    
    bool is_suspended;//prevents i2c controller accesses after suspend is called

};

struct elite_i2c_priv i2c0 = {
	.dev_id = 0,
	.master = 1,
	.i2c_mode=I2C_STANDARD_MODE,
	.regs=I2C0_BASE_ADDR
};

struct elite_i2c_priv i2c1 = {
	.dev_id = 0,
	.master = 1,
	.i2c_mode=I2C_STANDARD_MODE,
	.regs=I2C1_BASE_ADDR
};

static int elite_wait_i2c_ready(struct elite_i2c_priv *priv)
{
    int ret ;
    
    ret = 0 ;
    unsigned long long timeval = get_ticks();
    const unsigned long long timeout = 200000;
    while (1) {
        if ((readw(priv->regs + STAT_REG) & I2C_STATUS_MASK) == I2C_READY)
            break;

        if ((get_ticks() - timeval) > timeout) {
            ret = -1;
            printf("i2c_err : wait for bus ready time-out\n\r") ;
            break;
        }
    }

    return ret ;
}
static int elite_write_msg(struct elite_i2c_priv *priv, unsigned int slave_addr, char *buf, 
                                    unsigned int length, int restart, int last)
{
    unsigned short tcr_value;
    unsigned int xfer_length;
    int is_timeout;
    int ret = 0;
    int wait_event_result = 0;

    if (priv->master == 0)
        return -1;

    if (length < 0)
        return -1;
    xfer_length = 0;

    if (restart == 0)
        ret = elite_wait_i2c_ready(priv);
    if (ret < 0)
        return ret;

    priv->isr_stat.isr_nack        = 0;
    priv->isr_stat.isr_byte_end    = 0;
    priv->isr_stat.isr_timeout     = 0;
    priv->isr_stat.isr_int_pending = 0;

    if (length == 0)
        writew(0, priv->regs + DATAIO_REG);
    else
        writew((unsigned short)(buf[xfer_length] & I2C_CDR_DATA_WRITE_MASK),
                              priv->regs + DATAIO_REG);

    if (restart == 0) {
        writew(readw(priv->regs + CTRL_REG) & (~(I2C_CR_TX_END)), priv->regs + CTRL_REG);
        writew(readw(priv->regs + CTRL_REG) | I2C_CR_CPU_RDY, priv->regs + CTRL_REG);
    }

    tcr_value = 0;
    if (priv->i2c_mode == I2C_STANDARD_MODE) {
        tcr_value = (unsigned short)(I2C_TCR_STANDARD_MODE | I2C_TCR_MASTER_WRITE | \
                                     (slave_addr & I2C_TCR_SLAVE_ADDR_MASK));       
    } else if (priv->i2c_mode == I2C_FAST_MODE) {
        tcr_value = (unsigned short)(I2C_TCR_FAST_MODE | I2C_TCR_MASTER_WRITE | \
                                     (slave_addr & I2C_TCR_SLAVE_ADDR_MASK));
    }
    writew(tcr_value, priv->regs + XFER_REG);

    if (restart == 1)
        writew(readw(priv->regs + CTRL_REG) | I2C_CR_CPU_RDY, priv->regs + CTRL_REG);

    ret = 0;
    for (;;) {
#if 0    
        is_timeout = 0;
        wait_event_result = wait_event_interruptible_timeout(priv->wait, 
                                               priv->isr_stat.isr_int_pending,
                                                          (MAX_RX_TIMEOUT * HZ/1000));
        if (likely(wait_event_result > 0))
            ret = 0;
        else if (likely(priv->isr_stat.isr_int_pending == 0)) {
            is_timeout = 1;
            ret = -ETIMEDOUT;
        }
#endif	
        i2c_elite_get_transfer_status(priv);

        if (priv->isr_stat.isr_nack == 1) {
            ret = -EIO;
            break;
        }
       
        if (priv->isr_stat.isr_timeout == 1) {
            ret = -ETIMEDOUT;
            break;
        }

        if (is_timeout == 1) {
            ret = -ETIMEDOUT;
            break;
        }

        if (priv->isr_stat.isr_byte_end == 1)
            ++xfer_length;

        priv->isr_stat.isr_int_pending = 0;
        priv->isr_stat.isr_nack        = 0;
        priv->isr_stat.isr_byte_end    = 0;
        priv->isr_stat.isr_timeout     = 0;

        if ((readw(priv->regs + STAT_REG) & I2C_CSR_RCV_ACK_MASK) == I2C_CSR_RCV_NOT_ACK) {
            ret = -1;
            break;
        }

        if (length == 0) {
            writew((unsigned short)(I2C_CR_TX_END | I2C_CR_CPU_RDY | I2C_CR_ENABLE), priv->regs + CTRL_REG);
            break;
        }

        if (length > xfer_length) {
            writew((unsigned short)(buf[xfer_length] & I2C_CDR_DATA_WRITE_MASK), priv->regs + DATAIO_REG);
            writew((unsigned short)(I2C_CR_CPU_RDY | I2C_CR_ENABLE), priv->regs + CTRL_REG);
        } else if (length == xfer_length) {
            if (last == 1) {
                writew((I2C_CR_TX_END | I2C_CR_CPU_RDY | I2C_CR_ENABLE), priv->regs + CTRL_REG);
                break;
            } else {
                writew(I2C_CR_ENABLE, priv->regs + CTRL_REG);
                break;
            }
        } else {
            ret = -1;
            break;
        }
    }

    return ret;
}
static int elite_read_msg(struct elite_i2c_priv *priv, unsigned int slave_addr, char *buf,
                                  unsigned int length, int restart, int last)
{
    unsigned short tcr_value;
    unsigned int xfer_length;
    int is_timeout;
    int ret = 0;
    int wait_event_result = 0;

    if (priv->master == 0)
        return -ENXIO;

    if (length <= 0)
        return -1;

    xfer_length = 0;

    if (restart == 0)
        ret = elite_wait_i2c_ready(priv);
    if (ret < 0)
        return ret;

    priv->isr_stat.isr_nack        = 0;
    priv->isr_stat.isr_byte_end    = 0;
    priv->isr_stat.isr_timeout     = 0;
    priv->isr_stat.isr_int_pending = 0;

    writew(readw(priv->regs + CTRL_REG) & ~(I2C_CR_TX_END), priv->regs + CTRL_REG);
    writew(readw(priv->regs + CTRL_REG) & ~(I2C_CR_TX_NEXT_NO_ACK), priv->regs + CTRL_REG);
    if (restart == 0)
        writew(readw(priv->regs + CTRL_REG) | I2C_CR_CPU_RDY, priv->regs + CTRL_REG);
    
    tcr_value = 0;
    if (priv->i2c_mode == I2C_STANDARD_MODE) {
        tcr_value = (unsigned short)(I2C_TCR_STANDARD_MODE | I2C_TCR_MASTER_READ | \
                                     (slave_addr & I2C_TCR_SLAVE_ADDR_MASK));       
    } else if (priv->i2c_mode == I2C_FAST_MODE) {
        tcr_value = (unsigned short)(I2C_TCR_FAST_MODE | I2C_TCR_MASTER_READ | \
                                     (slave_addr & I2C_TCR_SLAVE_ADDR_MASK));
    }
    if (length == 1)
        writew(readw(priv->regs + CTRL_REG) | I2C_CR_TX_NEXT_NO_ACK, priv->regs + CTRL_REG);

    writew(tcr_value, priv->regs + XFER_REG);

    if (restart == 1)
        writew(readw(priv->regs + CTRL_REG) | I2C_CR_CPU_RDY, priv->regs + CTRL_REG);

    ret = 0;
    for (;;) {
#if 0     
        is_timeout = 0;
        wait_event_result = wait_event_interruptible_timeout(priv->wait, priv->isr_stat.isr_int_pending,
                                                          (MAX_RX_TIMEOUT * HZ/1000));
        if (likely(wait_event_result > 0))
            ret = 0;
        else if (likely(wait_event_result == 0))
        {
            is_timeout = 1;
            ret = -ETIMEDOUT;
        }
#endif
        i2c_elite_get_transfer_status(priv);

        if (priv->isr_stat.isr_nack == 1) {
            ret = -EIO;
            break;
        }
       
        if (priv->isr_stat.isr_timeout == 1) {
            ret = -ETIMEDOUT;
            break;
        }

        if (is_timeout == 1) {
            ret = -ETIMEDOUT;
            break;
        }

        if (priv->isr_stat.isr_byte_end == 1) {
            buf[xfer_length] = (readw(priv->regs + DATAIO_REG) >> 8);
            ++xfer_length;
        }

        priv->isr_stat.isr_int_pending = 0;
        priv->isr_stat.isr_nack        = 0;
        priv->isr_stat.isr_byte_end    = 0;
        priv->isr_stat.isr_timeout     = 0;

        if (length > xfer_length) {
            if ((length - 1) == xfer_length)
                writew(readw(priv->regs + CTRL_REG) | (I2C_CR_TX_NEXT_NO_ACK | I2C_CR_CPU_RDY),
                                priv->regs + CTRL_REG);
            else
                writew(readw(priv->regs + CTRL_REG) | I2C_CR_CPU_RDY,
                                priv->regs + CTRL_REG);
        } else if (length == xfer_length) {
            if (last == 1)
                break;
            else
                break;
        } else {
            ret = -EIO;
            break;
        }
    }

    return ret;
}

static int elite_i2c_valid_messages(struct i2c_msg msgs[], int num)
{
    int i;

    if (num < 1 || num > MAX_MESSAGES)
        return -EINVAL;

    for (i = 0; i < num; i++) {
        if (&msgs[i] == NULL)
            return -EINVAL;
        else {
            if (msgs[i].buf == NULL)
                return -EINVAL;
        }
    }

    return 0;
}

static int elite_i2c_doxfer(struct elite_i2c_priv *priv, 
                                struct i2c_msg *msg, int num)
{
    int i, last, restart, ret = 0;
    struct i2c_msg *pmsg;

    for (i = 0; i < 10; ++i)
        ;

    for (i = 0; ret >= 0 && i < num; i++) {
        last = ((i + 1) == num);
        restart = (i != 0);
        
        pmsg = &msg[i];

       if (pmsg->flags & I2C_M_NOSTART)
           restart = 1;

       if (pmsg->flags & I2C_M_RD)
           ret = elite_read_msg(priv, pmsg->addr, pmsg->buf, pmsg->len, restart, last);
       else
           ret = elite_write_msg(priv, pmsg->addr, pmsg->buf, pmsg->len, restart, last);
    }

    if (ret < 0)
        return ret;
    else 
        return i;
}

int elite_i2c_xfer( struct elite_i2c_priv *priv,struct i2c_msg *msgs, int num)
{
    int retry;
    int ret;

    ret = elite_i2c_valid_messages(msgs, num);
    if (ret)
        return ret;


    ret = elite_i2c_doxfer(priv, msgs, num);
    if (ret != -EAGAIN)
        return ret;


    return -EREMOTEIO;    
}


static int elite_i2c_irq_handler(void *dev_info)
{
    struct elite_i2c_priv *priv = (struct elite_i2c_priv *)dev_info;

    int  wakeup;
    unsigned short isr_status;
    unsigned long flags;
    unsigned short tmp;

    wakeup = 0;

    isr_status = readw(priv->regs + INT_STAT_REG);
    
    if (isr_status & I2C_ISR_NACK_ADDR) {
        writew(I2C_ISR_NACK_ADDR_WRITE_CLEAR, priv->regs + INT_STAT_REG);
        tmp = readw(priv->regs + STAT_REG);
        priv->isr_stat.isr_nack = 1;   
        wakeup = 1;    
    }

    if (isr_status & I2C_ISR_BYTE_END) {
        writew(I2C_ISR_BYTE_END_WRITE_CLEAR, priv->regs + INT_STAT_REG);
        priv->isr_stat.isr_byte_end = 1;
        wakeup = 1;
    }

    if (isr_status & I2C_ISR_SCL_TIME_OUT) {
        writew(I2C_ISR_SCL_TIME_OUT_WRITE_CLEAR, priv->regs + INT_STAT_REG); 
        priv->isr_stat.isr_timeout = 1;
        wakeup = 1;
    } 

    return 1;
}
static void elite_i2c_init(struct elite_i2c_priv *priv)
{
    unsigned short tmp;
    /*
     * hard code I2C controller to operate in standard mode
     */
    priv->i2c_mode = I2C_STANDARD_MODE;

    priv->isr_stat.isr_nack        = 0;
    priv->isr_stat.isr_byte_end    = 0;
    priv->isr_stat.isr_timeout     = 0;
    priv->isr_stat.isr_int_pending = 0;

    writew(0, priv->regs + CTRL_REG);
    writew(0, priv->regs + XFER_REG);

    writew(I2C_ISR_ALL_WRITE_CLEAR, priv->regs + INT_STAT_REG);
    writew(I2C_IMR_ALL_ENABLE, priv->regs + INT_MARK_REG);
    writew(I2C_CR_ENABLE, priv->regs + CTRL_REG);

    /* read clear */
    tmp = readw(priv->regs + STAT_REG);

    writew(I2C_ISR_ALL_WRITE_CLEAR, priv->regs + INT_STAT_REG);
    
    /* 
     * only standard mode supported
     * It can also be set as fast mode.
     */
    writew(I2C_TR_STD_VALUE, priv->regs + TIME_REG);
}
int i2c_elite_get_transfer_status(struct elite_i2c_priv *priv)
{
    int ret;
    unsigned short isr_status;
    unsigned short tmp ;
    unsigned long long timeval = get_ticks();
    const unsigned long long timeout = 100000;

    while(1) {
        isr_status = readw(priv->regs + INT_STAT_REG);
        ret = 0;

        if (isr_status & I2C_ISR_NACK_ADDR) {
            ret = 1;
            writew(I2C_ISR_NACK_ADDR_WRITE_CLEAR,priv->regs + INT_STAT_REG);
            tmp = readw(priv->regs + STAT_REG);/* read clear*/
            priv->isr_stat.isr_nack =1;	    
            tmp = tmp; /* remove compile warning */ 
            break;
        }
        else if (isr_status & I2C_ISR_SCL_TIME_OUT) {
            ret = 2;
            writew(I2C_ISR_SCL_TIME_OUT_WRITE_CLEAR, priv->regs + INT_STAT_REG);
            writew(readw(priv->regs + CTRL_REG) | I2C_MST_RST, priv->regs + CTRL_REG);
            priv->isr_stat.isr_timeout     = 1;
	    break;
        }
        else if (isr_status & I2C_ISR_BYTE_END) {
            ret = 0;
	    priv->isr_stat.isr_byte_end    = 1;
            writew(I2C_ISR_BYTE_END_WRITE_CLEAR, priv->regs + INT_STAT_REG);
            break;
        }

        if ((get_ticks() - timeval) > timeout) {
            ret = -1;
            break;
        }
    }

    return ret;
}

int elite_i2c_probe(struct elite_i2c_priv *priv,uchar chip)
{
    int ret = 0;

    unsigned short tcr_val;
    unsigned short ctrl_val;

    ret = elite_wait_i2c_ready(priv);
    
    if (ret < 0)
        return ret;

    writew(0,priv->regs + DATAIO_REG);

    tcr_val = I2C_TCR_MASTER_WRITE | I2C_TCR_STANDARD_MODE | chip;
    writew(tcr_val,priv->regs + XFER_REG);

    ret = i2c_elite_get_transfer_status(priv);

    ctrl_val = 0;
    ctrl_val |= I2C_CR_TX_END | I2C_CR_CPU_RDY | I2C_CR_ENABLE;
    writew(ctrl_val, priv->regs + CTRL_REG );

    return ret;
}

int i2c_probe(uchar chip)
{
    int ret = 0;	
    if(i2c_bus_num==0){
        ret=elite_i2c_probe(&i2c0,chip);		
    }else{
        ret=elite_i2c_probe(&i2c1,chip);	
    }

    return ret;
}

void i2c_init(int speed, int slaveaddr)
{		
    elite_i2c_init(&i2c0);
    elite_i2c_init(&i2c1);
}

unsigned int i2c_get_bus_num(void)
{
    return i2c_bus_num;
}


int i2c_set_bus_num(unsigned int bus)
{
    if (bus+1 > 2)
        return -1;

    i2c_bus_num = bus;
    return 0;
}
/*
 * Read/Write interface:
 *   chip:    I2C chip address, range 0..127
 *   addr:    Memory (register) address within the chip
 *   alen:    Number of bytes to use for addr (typically 1, 2 for larger
 *              memories, 0 for register type devices with only one
 *              register)
 *   buffer:  Where to read/write the data
 *   len:     How many bytes to read/write
 *
 *   Returns: 0 on success, not 0 on failure
 */
int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
    struct i2c_msg  msgs[2];
    int ret = STATUS_FAIL;
    u8 rev_buf[128]={0};
    u8 send_buf[128]={0};
    struct elite_i2c_priv *i2c;		
    printf("chip %x . addr %x . alen %x .len %x\n",chip,addr,alen,len);
    if (len > 128 && alen >2) {
        printf("No support for using %d bytes to length buffer in chip %d alen =%d\n", len, chip,alen);
        return -1;
    }

    msgs[0].len=alen; 
    msgs[0].addr=chip; 
    msgs[0].flags=0;//write
    msgs[0].buf=send_buf;
    msgs[0].buf[0]=(u8)(addr>>8);
    msgs[0].buf[1]=(u8)(addr);
    
    msgs[1].len=len;//read lenth
    msgs[1].addr=chip;
    msgs[1].flags=I2C_M_RD;//read
    msgs[1].buf=&rev_buf[0];//return buffer
    
    if(i2c_bus_num==0){
        i2c=&i2c0;		
    } else {
        i2c=&i2c1;	
    }	
	
    i2c->i2c_mode = I2C_FAST_MODE;
    
    if (elite_i2c_probe(i2c,msgs[0].addr) == 0){
            printf("probe i2c1 found 0x%x\n",msgs[0].addr);            
    }	

    i2c->i2c_mode = I2C_FAST_MODE;
    writew(I2C_TR_FAST_VALUE, i2c->regs + TIME_REG);
    ret=elite_i2c_xfer(i2c,&msgs[0],2);
    memcpy(buffer,&rev_buf[0],len);
	
    if(ret >0) 
        ret =STATUS_PASS;
	
    return ret;
}
/**
 * i2c_write - write data to an i2c device
 *	@chip: i2c chip addr
 *	@addr: memory (register) address in the chip
 *	@alen: byte size of address
 *	@buffer: buffer holding data to write to chip
 *	@len: how many bytes to write
 *	@return: 0 on success, non-0 on failure
 */
int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
    struct i2c_msg  msgs[2];
    int ret = STATUS_FAIL;
    u8 send_buf[128]={0};
    struct elite_i2c_priv *i2c;		

    printf("chip %x . addr %x . alen %x .len %x\n",chip,addr,alen,len);	
    if (len > 128 && alen >2)
    {
        printf("No support for using %d bytes to length buffer in chip %d alen =%d\n", len, chip,alen);
        return -1;
    }

    msgs[0].len=alen+len; 
    msgs[0].addr=chip; 
    msgs[0].flags=0;//write
    msgs[0].buf=send_buf;
    if(alen==1){
        send_buf[0]=(u8)(addr);
	 memcpy(&send_buf[1],buffer,len);	
    }else{
    	 send_buf[0]=(u8)(addr>>8);
        send_buf[1]=(u8)(addr);
	 memcpy(&send_buf[2],buffer,len);	
    }
    
    if(i2c_bus_num==0){
        i2c = &i2c0;		
    } else {
        i2c = &i2c1;	
    }
    
    if (elite_i2c_probe(i2c,msgs[0].addr) == STATUS_PASS){
            printf("probe i2c0 found 0x%x\n",msgs[0].addr);            
    }
    
    printf("i2c%d write addrss 0x%x length %d \n",chip,msgs[0].addr,msgs[0].len);


    i2c->i2c_mode = I2C_STANDARD_MODE;
    writew(I2C_TR_STD_VALUE, i2c->regs + TIME_REG);
    ret=elite_i2c_xfer(i2c,&msgs[0],1);
    
    if(ret >0) 
        ret =STATUS_PASS;

    return ret;

}

 int i2c_probe_test(void)
{
    int j;
    int ret = -1; 

    for (j = 0; j < 128; j++) {
        if (elite_i2c_probe(&i2c0,j) == 0){
            printf("probe i2c0 found 0x%x\n",j);
            ret = 0;
        }
    }

     for (j = 0; j < 128; j++) {
        if (elite_i2c_probe(&i2c1,j) == 0){
            printf("probe i2c1 found 0x%x\n",j);
            ret = 0;
        }
    }
     
    return ret;
}
 static int i2c1_read_id_test(void )
{

    struct i2c_msg  msgs[2];
    int ret = STATUS_FAIL;
    char rev_buf[10];
    char send_buf[10];

    msgs[0].len=2; 
    msgs[0].addr=0x34; 
    msgs[0].flags=0;//write
    msgs[0].buf=send_buf;
    msgs[0].buf[0]=0x40;
    msgs[0].buf[1]=0x00;
    
    msgs[1].len=2;//read lenth
    msgs[1].addr=0x34;
    msgs[1].flags=I2C_M_RD;//read
    msgs[1].buf=&rev_buf[0];//return buffer
    msgs[1].buf[0]=0;
    msgs[1].buf[1]=0;
    
    if (elite_i2c_probe(&i2c1,msgs[0].addr) == 0){
            printf("probe i2c1 found 0x%x\n",msgs[0].addr);            
    }	

    i2c1.i2c_mode = I2C_STANDARD_MODE;
    writew(I2C_TR_STD_VALUE, i2c1.regs + TIME_REG);
    ret=elite_i2c_xfer(&i2c1,&msgs[0],2);
    
    if(ret >0 && msgs[1].buf[0]==0x62 && msgs[1].buf[1]==0x46) 
        ret =STATUS_PASS;
    
    printf("i2c1 id is 0x%x%x\n",ret,msgs[1].buf[0],msgs[1].buf[1]);

    return ret;
}

int i2c1_stress_test(void)
{
    struct i2c_msg  msgs[2];
    int ret = STATUS_PASS;
    char rev_buf[10]={0};
    char send_buf[10]={0};
    unsigned int i,j,low ,high,count;
    	
    i2c1.i2c_mode = I2C_STANDARD_MODE;
    writew(I2C_TR_STD_VALUE, i2c1.regs + TIME_REG);

    if (elite_i2c_probe(&i2c1,0x34) == 0) {
        printf("probe i2c1 found 0x%x\n",msgs[0].addr);            
    }
    count=0;

    for(j=0;j<=0x1;j++) {
        for(i=0;i<=0xff;i++) {	
    	
            low=i%16;	
            high=i&0xf;
    	
            msgs[0].len=5; 
            msgs[0].addr=0x34; 
            msgs[0].flags=0;//write
            msgs[0].buf=send_buf;
            msgs[0].buf[0]=0x40;
            msgs[0].buf[1]=0x30;
            msgs[0].buf[2]=high;
            msgs[0].buf[3]=low;     
    
            ret=elite_i2c_xfer(&i2c1,&msgs[0],1);    

            if(ret <= 0) {
                printf("i2c stress test failed , %d\n",i);
                ret =STATUS_FAIL;
            }
    
            // mdelay(1);
    
            msgs[0].len=2; 
            msgs[0].addr=0x34; 
            msgs[0].flags=0;//write
            msgs[0].buf=send_buf;
            msgs[0].buf[0]=0x40;
            msgs[0].buf[1]=0x30;
    
            msgs[1].len=2;//read lenth
            msgs[1].addr=0x34;
            msgs[1].flags=I2C_M_RD;//read
            msgs[1].buf=&rev_buf[0];//return buffer
            msgs[1].buf[0]=0;
            msgs[1].buf[1]=0;  
    
            ret=elite_i2c_xfer(&i2c1,&msgs[0],2);
    
            if(ret <=0 || msgs[1].buf[1] !=low ) {
                printf("i2c stress test failed , %d ,j=0x%x,i=0x%x\n",count,j,i);
                ret =STATUS_FAIL;
            }   
            count++;
        } 
        printf(".");
    }

    ret = STATUS_PASS;
    printf("i2c1 id is 0x%x%x\n",ret,msgs[1].buf[0],msgs[1].buf[1]);

    return ret;
}
