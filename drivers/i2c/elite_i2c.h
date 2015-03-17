#ifndef _ELITE_I2C_H_
#define _ELITE_I2C_H_
#define ELITE_I2C_REG(x)   (x)

#define CTRL_REG       ELITE_I2C_REG(0x00)
#define XFER_REG       ELITE_I2C_REG(0x02)
#define STAT_REG       ELITE_I2C_REG(0x04)
#define INT_STAT_REG   ELITE_I2C_REG(0x06)
#define INT_MARK_REG   ELITE_I2C_REG(0x08)
#define DATAIO_REG     ELITE_I2C_REG(0x0A)
#define TIME_REG       ELITE_I2C_REG(0x0C)
#define CLK_DIV_REG    ELITE_I2C_REG(0x0E)
/* slave I2C registers */
#define S_CTRL_REG     ELITE_I2C_REG(0x10)
#define S_STAT_REG     ELITE_I2C_REG(0x12)
#define S_INT_STAT_REG ELITE_I2C_REG(0x14)
#define S_INT_MARK_REG ELITE_I2C_REG(0x16)
#define S_DATAIO_REG   ELITE_I2C_REG(0x18)
#define S_TIME_REG     ELITE_I2C_REG(0x1A)

/*
 * I2C Module Status Info
 */
#define I2C_READY                        0x0002
#define I2C_BUSY                         0x0000
#define I2C_STATUS_MASK                  0x0002
#define I2C_CSR_RCV_ACK                  0x0000
#define I2C_CSR_RCV_NOT_ACK              0x0001
#define I2C_CSR_RCV_ACK_MASK             0x0001

/*
 * I2C Controller Control Info
 */
#define I2C_CR_CPU_RDY                   0x0008
#define I2C_CR_TX_END                    0x0004
#define I2C_CR_TX_NEXT_NO_ACK            0x0002
#define I2C_CR_TX_NEXT_ACK               0x0000
#define I2C_CR_ENABLE                    0x0001
#define I2C_SLAV_MODE_SEL                0x8000
#define I2C_MST_RST                      0x80


/*
 * I2C Transfer Control Info
 */
#define I2C_TCR_HS_MODE                 0x2000
#define I2C_TCR_STANDARD_MODE           0x0000
#define I2C_TCR_FAST_MODE               0x8000
#define I2C_TCR_MASTER_WRITE            0x0000
#define I2C_TCR_MASTER_READ             0x4000
#define I2C_TCR_SLAVE_ADDR_MASK         0x007F

/*
 * I2C Interrupt Status Info
 */
#define I2C_ISR_SCL_TIME_OUT             0x0004
#define I2C_ISR_SCL_TIME_OUT_WRITE_CLEAR 0x0004
#define I2C_ISR_BYTE_END                 0x0002
#define I2C_ISR_BYTE_END_WRITE_CLEAR     0x0002
#define I2C_ISR_NACK_ADDR                0x0001
#define I2C_ISR_NACK_ADDR_WRITE_CLEAR    0x0001
#define I2C_ISR_ALL_WRITE_CLEAR          0x0007

#define I2C_IMR_SCL_TIME_OUT_MASK        0x0004
#define I2C_IMR_BYTE_END_MASK            0x0002
#define I2C_IMR_NACK_ADDR_MASK           0x0001
#define I2C_IMR_ALL_ENABLE               0x0007

/*
 * I2C Data IO Info
 */
#define I2C_CDR_DATA_READ_MASK           0xFF00
#define I2C_CDR_DATA_WRITE_MASK          0x00FF

/*
 * I2C Timer Info
 */
#define I2C_TR_SCL_TIME_OUT_MASK         0xFF00
#define I2C_TR_FSTP_MASK                 0x00FF
#define I2C_TR_STD_VALUE                 0x8064
#define I2C_TR_FAST_VALUE                0x8019

/*
 * I2C Clock Divider Info
 */
#define APB_96M_I2C_DIV                  7
#define APB_166M_I2C_DIV                 12

#define MAX_RX_TIMEOUT                   500
#define MAX_MESSAGES                     65536

struct elite_isr_status {
    int      isr_nack;
    int      isr_byte_end;
    int      isr_timeout;
    int      isr_int_pending;
}isr_stat;

enum elite_i2c_mode {
    I2C_STANDARD_MODE = 0,
    I2C_FAST_MODE     = 1,
    I2C_HS_MODE       = 2,
};


#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
struct i2c_msg {
	u16 addr;	/* slave address			*/
	u16 flags;
	u16 len;		/* msg length				*/
	u8 *buf;		/* pointer to msg data			*/
};



#define MAX_MESSAGES                    65536

#define I2C0_BASE_ADDR                  0xd8280000
#define I2C1_BASE_ADDR                  0xd8320000


#define CONTROLLER_CONTROL              0x00
#define CONTROLLER_TRANSFER_CONTROL     0x02
#define CONTROLLER_STATUS               0x04
#define CONTROLLER_INTERRUPT_STATUS     0x06
#define CONTROLLER_INTERRUPT_MASK       0x08
#define CONTROLLER_DATA_IO_BUFFER       0x0a
#define CONTROLLER_TIME_PARAMETERS      0x0c
#define MASTER_CODE_IN_HIGH_SPEED_MODE	0x0e


#define GPIO_BASE_ADDR                  0xd8110000
#define GPIO_ENABLE_I2C0                (GPIO_BASE_ADDR+0x51)
#define GPIO_PULL_ENABLE_I2C0           (GPIO_BASE_ADDR+0x492)
#define GPIO_PULL_CTRL0_I2C0            (GPIO_BASE_ADDR+0x4d2)

#define GPIO_ENABLE_I2C1_REG            (GPIO_BASE_ADDR+0x4c)
#define	GPIO_ENABLE_I2C1                (GPIO_BASE_ADDR+0x55)
#define	GPIO_PULL_ENABLE_I2C1           (GPIO_BASE_ADDR+0x495)
#define	GPIO_PULL_CTRL0_I2C1            (GPIO_BASE_ADDR+0x4d5)


#define PMC_BASE_ADDR                   0xd8130000
#define	PMCEL_VAL                       (PMC_BASE_ADDR+0x250)

#define I2C_CDR_DATA_READ_MASK          0xFF00
#define I2C_CDR_DATA_WRITE_MASK         0x00FF

#define I2C_TR_STD_VALUE                0x8064
#define I2C_TR_FAST_VALUE               0x8019

/*
 *  I2C_DIV_REG
 *  I2C DIV
 *
 */
//#define APB_96M_I2C_DIV                 (0x07 <<8 )  /*Dean revised 2007/9/11 */
//#define APB_166M_I2C_DIV                (12 << 8) /*Dean revised 2008/5/9 */


/*
 *  I2C_TCR_REG
 *  I2C Transfer Control
 *
 */
#define I2C_TCR_HS_MODE                 0x2000		/* [13:13] */
#define I2C_TCR_STANDARD_MODE           0x0000          /* [15:15] */
#define I2C_TCR_FAST_MODE               0x8000
#define I2C_TCR_MASTER_WRITE            0x0000          /* [14:14] */
#define I2C_TCR_MASTER_READ             0x4000
/* Reserved [13:07] */
#define I2C_TCR_SLAVE_ADDR_MASK         0x007F          /* [06:00] */

/*
 *  I2C_CSR_REG
 *  I2C Status
 *
 */
/* Reserved [15:02] */
#define I2C_READY                       0x0002          /* [01:01] R */
#define I2C_BUSY                        0x0000
#define I2C_STATUS_MASK                 0x0002
#define I2C_CSR_RCV_ACK                 0x0000          /* [00:00] R */
#define I2C_CSR_RCV_NOT_ACK             0x0001
#define I2C_CSR_RCV_ACK_MASK            0x0001

/*
 *  I2C_ISR_REG
 *  I2C Interrupt Status
 *
 */
/* Reserved [15:03] */
#define I2C_ISR_SCL_TIME_OUT              0x0004          /* [02:02] R */
#define I2C_ISR_SCL_TIME_OUT_WRITE_CLEAR  0x0004
#define I2C_ISR_BYTE_END                  0x0002          /* [01:01] R */
#define I2C_ISR_BYTE_END_WRITE_CLEAR      0x0002
#define I2C_ISR_NACK_ADDR                 0x0001          /* [00:00] R */
#define I2C_ISR_NACK_ADDR_WRITE_CLEAR     0x0001
#define I2C_ISR_ALL_WRITE_CLEAR           0x0007
#define I2C_IMR_ALL_ENABLE                0x0007

#define WITE_BIT 0
#define READ_BIT 1

#define SLAVE_CONTROL                     0x10
#define SLAVE_STATUS                      0x12
#define SLAVE_INTERRUPT_STATUS            0x14
#define SLAVE_INTERRUPT_MASK              0x16
#define SLAVE_DATA_IO_BUFFER              0x18
#define SLAVE_TIME_PARAMETERS             0x1a


#define	EINVAL		22	/* Invalid argument */
#define	EIO		 5	/* I/O error */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ENXIO		 6	/* No such device or address */
#define	EAGAIN		11	/* Try again */
#define	EREMOTEIO	121	/* Remote I/O error */

#endif /*_ELITE_I2C_H_*/







