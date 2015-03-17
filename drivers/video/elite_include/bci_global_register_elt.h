//    Spec Last Modified Time: 8/26/2011 2:47:39 PM
#ifndef _BCI_GLOBAL_REGISTER_ELT_H
#define _BCI_GLOBAL_REGISTER_ELT_H


#ifndef        BCI_GLOBAL_BLOCKBASE_INF
    #define    BCI_GLOBAL_BLOCKBASE_INF
    #define    BLOCK_BCI_GLOBAL_VERSION 1
    #define    BLOCK_BCI_GLOBAL_TIMESTAMP "8/26/2011 2:47:39 PM"
    #define    BCI_GLOBAL_BLOCK                                           0x0 // match with BlockID.h
    //  The following part is purposely set instead of auto generated. So pay attention to it.
    #define    BCI_GLOBAL_REG_START                                       0x1B // match with BlockID.h
    #define    BCI_GLOBAL_REG_END                                         0x1D // match with BlockID.h
    #define    BCI_GLOBAL_REG_LIMIT                                       0xF0 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Block_Busy_Bits_Offset                                 0x0
#define        Reg_Ring_Buf_Offset                                        0x1
#define        Reg_Time_Stamp_Low_Offset                                  0x11
#define        Reg_Time_Stamp_High_Offset                                 0x12
#define        Reg_Busy_Time_Stamp_Low_Offset                             0x13
#define        Reg_Busy_Time_Stamp_High_Offset                            0x14
#define        Reg_Cur_Dma_Cmd_Offset                                     0x15
#define        Reg_Cur_Rbuf_Cmd_Offset                                    0x16
#define        Reg_Cur_Hrbuf_Cmd_Offset                                   0x17
#define        Reg_Cur_Vrbuf_Cmd_Offset                                   0x18
#define        Reg_Cur_Irbuf_Cmd_Offset                                   0x19
#define        Reg_Mxu_Entries_Cfg_Offset                                 0x1A
#define        Reg_Ogl_Cut_Flag_32_Offset                                 0x1B
#define        Reg_Bci_Misc_Control_Offset                                0x1C
#define        Reg_Signature_Offset                                       0x1D
#define        Reg_Bci_Fence_Counter_Offset                               0x2F
#define        Reg_Ila_Counters_Offset                                    0x33
#define        Reg_Save_Restore_Control_Offset                            0x46
#define        Reg_Restore_Mask_Offset                                    0x47
#define        Reg_Pwr_Mgr_Cfg_En_Offset                                  0x48
#define        Reg_Pwr_Mgr_Clk_Surge_Cnt_Offset                           0x49
#define        Reg_Pwr_Mgr_Status_Offset                                  0x4A
#define        Reg_Pwr_Mgr_Pg_Pd_Misc0_Offset                             0x4B
#define        Reg_Pwr_Mgr_Pg_Pd_Misc1_Offset                             0x4C
#define        Reg_Pwr_Mgr_Pg_Pu_Misc_0_Offset                            0x4D
#define        Reg_Pwr_Mgr_Pg_Pu_Misc_1_Offset                            0x4E
#define        Reg_Pwr_Mgr_Cg_Wait_Cnt0_Offset                            0x4F
#define        Reg_Pwr_Mgr_Cg_Wait_Cnt1_Offset                            0x50
#define        Reg_Pwr_Mgr_Cg_Wait_Cnt2_Offset                            0x51
#define        Reg_Pwr_Mgr_Pg_Wait_Cnt0_Offset                            0x52
#define        Reg_Pwr_Mgr_Pg_Wait_Cnt1_Offset                            0x53
#define        Reg_Pwr_Mgr_Pg_Wait_Cnt2_Offset                            0x54
#define        Reg_Bci_Skip_Offset                                        0x55
#define        Reg_Eu_Rd_Group_Offset                                     0xC0
#define        Reg_Eu_Wr_Group_Offset                                     0xE0

// Block constant definition
typedef enum
{
    RUN_LIST_CTX_ADDR_TRANSLATE_TRANSLATE    = 0,   // 0: Translate the address
    RUN_LIST_CTX_ADDR_TRANSLATE_DISABLE      = 1,   // 1: Do not translate the address. Disables the address to be
                                                    // translated by the MXU.
} RUN_LIST_CTX_ADDR_TRANSLATE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            BCI_GLOBAL Block (Block ID = 0) Register Definitions                      ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef union
{
    struct
    {
        unsigned int Blocks_Busy_Status        : 32; 
    } reg;
    unsigned int uint;
} Reg_Block_Busy_Bits;
 
typedef union
{
    struct
    {
        unsigned int Translate                 : 1;   // Translation Disabled
        unsigned int Kickoff                   : 1;   // whether the RB is kick off
        unsigned int Reserved                  : 10;  // Reserved (must be 0)
        unsigned int Addr                      : 20;  // Run List Context Base Address (4KB aligned)
    } reg;
    unsigned int uint;
} Reg_Run_List_Ctx_Addr;
 
typedef union
{
    struct
    {
        unsigned int Rb_Size                   : 32;  // RB size
    } reg;
    unsigned int uint;
} Reg_Ring_Buf_Size;
 
typedef union
{
    struct
    {
        unsigned int Rb_Head                   : 32;  // RB Current Head Pointer
    } reg;
    unsigned int uint;
} Reg_Ring_Buf_Head;
 
typedef union
{
    struct
    {
        unsigned int Rb_Tail                   : 32;  // RB Current Tail Pointer
    } reg;
    unsigned int uint;
} Reg_Ring_Buf_Tail;
 
typedef struct _Group_Ring_Buf
{
    Reg_Run_List_Ctx_Addr            reg_Run_List_Ctx_Addr;
    Reg_Ring_Buf_Size                reg_Ring_Buf_Size;
    Reg_Ring_Buf_Head                reg_Ring_Buf_Head;
    Reg_Ring_Buf_Tail                reg_Ring_Buf_Tail;
} Reg_Ring_Buf_Group;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Time_Stamp_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Time_Stamp_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Busy_Time_Stamp_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Busy_Time_Stamp_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Cur_Dma_Cmd;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Cur_Rbuf_Cmd;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Cur_Hrbuf_Cmd;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Cur_Vrbuf_Cmd;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Cur_Irbuf_Cmd;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Mxu_Entries_Cfg;
 
typedef union
{
    struct
    {
        unsigned int Ogl_Cut_Flag_32           : 32;  // Cut Flag for IB(only for triangle list/strip,others will
                                                      // be patched by sw)
    } reg;
    unsigned int uint;
} Reg_Ogl_Cut_Flag_32;
 
typedef union
{
    struct
    {
        unsigned int Triangle_Cut_Index_Enable : 1;   // Whether enable the cut index for triangle strip/list or
                                                      // not.
        unsigned int Reserved0                 : 3; 
        unsigned int Ila_Mode                  : 4;   // ILA mode selector
        unsigned int Ila_Mask                  : 19;  // tell which ILA need to be restore when do restore cmd
        unsigned int Reserved                  : 3; 
        unsigned int Slice1_Valid              : 1; 
        unsigned int Dump_3d_Signature_Zero    : 1;   // When it is 1, dump SG/D/Z signature with 0. Else, dump as
                                                      // their correct value. This is used for FPGA validation, to
                                                      // avoid change other environment part.
    } reg;
    unsigned int uint;
} Reg_Bci_Misc_Control;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Sg_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Sg_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_D_Slice0_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_D_Slice0_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_D_Slice1_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_D_Slice1_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Z_Slice0_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Z_Slice0_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Z_Slice1_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Z_Slice1_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Ispio_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Ispio_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Ispoo_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Ispoo_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_S3vd_Dec_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_S3vd_Dec_Signature_High;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_S3vd_Enc_Signature_Low;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_S3vd_Enc_Signature_High;
 
typedef struct _Group_Signature
{
    Reg_Sg_Signature_Low             reg_Sg_Signature_Low;
    Reg_Sg_Signature_High            reg_Sg_Signature_High;
    Reg_D_Slice0_Signature_Low       reg_D_Slice0_Signature_Low;
    Reg_D_Slice0_Signature_High      reg_D_Slice0_Signature_High;
    Reg_D_Slice1_Signature_Low       reg_D_Slice1_Signature_Low;
    Reg_D_Slice1_Signature_High      reg_D_Slice1_Signature_High;
    Reg_Z_Slice0_Signature_Low       reg_Z_Slice0_Signature_Low;
    Reg_Z_Slice0_Signature_High      reg_Z_Slice0_Signature_High;
    Reg_Z_Slice1_Signature_Low       reg_Z_Slice1_Signature_Low;
    Reg_Z_Slice1_Signature_High      reg_Z_Slice1_Signature_High;
    Reg_Ispio_Signature_Low          reg_Ispio_Signature_Low;
    Reg_Ispio_Signature_High         reg_Ispio_Signature_High;
    Reg_Ispoo_Signature_Low          reg_Ispoo_Signature_Low;
    Reg_Ispoo_Signature_High         reg_Ispoo_Signature_High;
    Reg_S3vd_Dec_Signature_Low       reg_S3vd_Dec_Signature_Low;
    Reg_S3vd_Dec_Signature_High      reg_S3vd_Dec_Signature_High;
    Reg_S3vd_Enc_Signature_Low       reg_S3vd_Enc_Signature_Low;
    Reg_S3vd_Enc_Signature_High      reg_S3vd_Enc_Signature_High;
} Reg_Signature_Group;
 
typedef union
{
    struct
    {
        unsigned int Counter0                  : 8; 
        unsigned int Counter1                  : 8; 
        unsigned int Counter2                  : 8; 
        unsigned int Counter3                  : 8; 
    } reg;
    unsigned int uint;
} Reg_Fence_Counter;
 
typedef struct _Group_Bci_Fence_Counter
{
    Reg_Fence_Counter                reg_Fence_Counter;
} Reg_Bci_Fence_Counter_Group;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Ila_Counters;
 
typedef union
{
    struct
    {
        unsigned int Auto_Sr_Enable            : 1;   // When enable, hw will auto sent save/restore cmd while Lw
                                                      // queue switching to Hi. When disable, driver will in charge
                                                      // of sending save/restore cmd.
        unsigned int Reserved                  : 13; 
        unsigned int Sr_Addr                   : 18;  // when AUTO_SR_ENABLE=1, driver will programming addr
                                                      // which hw will use while auto sent save/restore cmd.
    } reg;
    unsigned int uint;
} Reg_Save_Restore_Control;
 
typedef union
{
    struct
    {
        unsigned int Fence_Mask                : 16;  // tell which fence need to be restore when do restore cmd
        unsigned int Reserved                  : 16; 
    } reg;
    unsigned int uint;
} Reg_Restore_Mask;
 
typedef union
{
    struct
    {
        unsigned int En_Clkgat_Ffu             : 1;   // 0: Disable
                                                      //  1: Enable
                                                      //  Clock gating is disabled after power-on reset.
        unsigned int En_Clkgat_Eutu            : 1; 
        unsigned int En_Clkgat_S3vd            : 1; 
        unsigned int En_Clkgat_Isp             : 1; 
        unsigned int En_Pwrgat_Ffu             : 1;   // 0: Disable
                                                      //  1: Enable
                                                      //  Clock gating is disabled after power-on reset.
        unsigned int En_Pwrgat_Eutu            : 1; 
        unsigned int En_Pwrgat_S3vd            : 1; 
        unsigned int En_Pwrgat_Isp             : 1; 
        unsigned int Pwr_Rdy_Enabled           : 1;   // Power-On or Power-Down complete status will rely on the
                                                      // Chain feedback signal plus a counter or rely on counter
                                                      // only.
        unsigned int En_Pwr_Sw_Ctl_3d          : 1;   // 0: Automatically(Power-on default)
                                                      //  1: Manually
                                                      //  C3D power zone¡¯s Power-Management sequence will be
                                                      // directly controlled by internal state machine or by SW
                                                      // register programming.
        unsigned int En_Pwr_Sw_Ctl_S3vd        : 1;   // S3VD power zone manual control enable
        unsigned int En_Pwr_Sw_Ctl_Isp         : 1; 
        unsigned int Pwr_Sw_Ctl_Ffu_Clken      : 1; 
        unsigned int Pwr_Sw_Ctl_Eutu_Clken     : 1; 
        unsigned int Pwr_Sw_Ctl_S3vd_Clken     : 1; 
        unsigned int Pwr_Sw_Ctl_Isp_Clken      : 1; 
        unsigned int Pwr_Sw_Ctl_Ffu_Pwron      : 1; 
        unsigned int Pwr_Sw_Ctl_Eutu_Pwron     : 1; 
        unsigned int Pwr_Sw_Ctl_S3vd_Pwron     : 1; 
        unsigned int Pwr_Sw_Ctl_Isp_Pwron      : 1; 
        unsigned int Pwr_Sw_Ctl_Ffu_Rston      : 1; 
        unsigned int Pwr_Sw_Ctl_Eutu_Rston     : 1; 
        unsigned int Pwr_Sw_Ctl_S3vd_Rston     : 1; 
        unsigned int Pwr_Sw_Ctl_Isp_Rston      : 1; 
        unsigned int Pwr_Sw_Ctl_Ffu_Isoon      : 1; 
        unsigned int Pwr_Sw_Ctl_Eutu_Isoon     : 1; 
        unsigned int Pwr_Sw_Ctl_S3vd_Isoon     : 1; 
        unsigned int Pwr_Sw_Ctl_Isp_Isoon      : 1; 
        unsigned int Reserved                  : 4; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Cfg_En;
 
typedef union
{
    struct
    {
        unsigned int Surge_Cnt_Ffu             : 4;   // Wait up to 16 cycles for Clock ON and OFF
        unsigned int Surge_Cnt_Eutu            : 4; 
        unsigned int Surge_Cnt_S3vd            : 4; 
        unsigned int Surge_Cnt_Isp             : 4; 
        unsigned int Reserved                  : 16; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Clk_Surge_Cnt;
 
typedef union
{
    struct
    {
        unsigned int Pwr_Sta_Ffu_Clken         : 1;   // FFU clk status
        unsigned int Pwr_Sta_Eutu_Clken        : 1;   // EUTU clk status
        unsigned int Pwr_Sta_S3vd_Clken        : 1;   // S3VD clk status
        unsigned int Pwr_Sta_Isp_Clken         : 1;   // ISP clk status
        unsigned int Pwr_Sta_Ffu_Pwron         : 1;   // FFU power status
        unsigned int Pwr_Sta_Eutu_Pwron        : 1;   // EUTU power status
        unsigned int Pwr_Sta_S3vd_Pwron        : 1;   // S3VD power status
        unsigned int Pwr_Sta_Isp_Pwron         : 1;   // ISP power status
        unsigned int Pwr_Sta_Ffu_Rston         : 1;   // FFU reset status
        unsigned int Pwr_Sta_Eutu_Rston        : 1;   // EUTU reset status
        unsigned int Pwr_Sta_S3vd_Rston        : 1;   // S3VD reset status
        unsigned int Pwr_Sta_Isp_Rston         : 1;   // ISP reset status
        unsigned int Pwr_Sta_Ffu_Isoon         : 1;   // FFU isolation status
        unsigned int Pwr_Sta_Eutu_Isoon        : 1;   // EUTU isolation status
        unsigned int Pwr_Sta_S3vd_Isoon        : 1;   // S3VD isolation status
        unsigned int Pwr_Sta_Isp_Isoon         : 1;   // ISP isolation status
        unsigned int Pwr_Sta_Ffu_Pwron_Be      : 1;   // FFU power status from chain back
        unsigned int Pwr_Sta_Eutu_Pwron_Be     : 1;   // EUTU power status from chain back
        unsigned int Pwr_Sta_S3vd_Pwron_Be     : 1;   // S3VD power status from chain back
        unsigned int Pwr_Sta_Isp_Pwron_Be      : 1;   // ISP power status from chain back
        unsigned int Reserved                  : 12; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Status;
 
typedef union
{
    struct
    {
        unsigned int Cnt_Pd_Isoon_Pwroff_3d    : 10;  // Power-down sequence, isolation on to power off
        unsigned int Cnt_Pd_Isoon_Pwroff_S3vd : 10;   // Power-down sequence, isolation on to power off
        unsigned int Cnt_Pd_Isoon_Pwroff_Isp   : 10;  // Power-down sequence, isolation on to power off
        unsigned int Reserved                  : 2; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Pd_Misc0;
 
typedef union
{
    struct
    {
        unsigned int Cnt_Pd_Complete_3d        : 10;  // Power-Down Complete status. It will start to count from
                                                      // either
                                                      //  - All the chain got Power-Down.(PWR_RDY_ENABLED =0)
                                                      //  - Or the Power-down feedback signals from the Chain.
                                                      // (PWR_RDY_ENABLED =1)
                                                      //  Once the counter reaches the value, the state machine will
                                                      // treat it as Power-Down Complete. This counter will
                                                      // increase every 256 cycles. The maximum time for this
                                                      // counter is 1024*256*3.2ns = 0.838ms. 
        unsigned int Cnt_Pd_Complete_S3vd      : 10;  // The maximum time for this counter is 1024*256*3.2ns =
                                                      // 0.838ms. 
        unsigned int Cnt_Pd_Complete_Isp       : 10;  // The maximum time for this counter is 1024*256*3.2ns =
                                                      // 0.838ms.
        unsigned int Reserved                  : 2; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Pd_Misc1;
 
typedef union
{
    struct
    {
        unsigned int Cnt_Wakeup_Rston_3d       : 10;  // Power-Up Complete status. It will start to count from
                                                      // either
                                                      //  - All the chain got Power-Up.
                                                      //  - Or the Power-Up feedback signals from the Mother/
                                                      // Daughter Chain.
                                                      //  Once the counter reaches the value, the state machine will
                                                      // treat it as Power-Up Complete and start asserting RESET
                                                      // signals. This counter will increase every 256 cycles. The
                                                      // maximum time for this counter is 1024*256*3.2ns =
                                                      // 0.838ms.
        unsigned int Cnt_Wakeup_Rston_S3vd     : 10;  // The maximum time for this counter is 1024*256*3.2ns =
                                                      // 0.838ms.
        unsigned int Cnt_Wakeup_Rston_Isp      : 10;  // The maximum time for this counter is 1024*256*3.2ns =
                                                      // 0.838ms.
        unsigned int Reserved                  : 2; 
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Pu_Misc_0;
 
typedef union
{
    struct
    {
        unsigned int Cnt_Rston_Clkon           : 8;   // Reset_on to clk_on,up to 256 cycles
        unsigned int Cnt_Clkon_Rstoff          : 8;   // Clk_on to reset_off, up to 256 cycles. Purpose for this is
                                                      // to do synchronous reset.
        unsigned int Cnt_Rstoff_Isooff         : 8;   // Reset_off to Isolation off, up to 256 cycles
        unsigned int Cnt_Isooff_Ready          : 8;   // Isolation off to ready, up to 256 cycles
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Pu_Misc_1;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the 3D Clock-Gating after wait for # of
                                                      // cycles without any 3D activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Cg_Wait_Cnt0;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the S3VD Clock-Gating after wait for # of
                                                      // cycles without any S3VD  activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Cg_Wait_Cnt1;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the ISP Clock-Gating after wait for # of
                                                      // cycles without any ISP activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Cg_Wait_Cnt2;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the 3D Power-Gating after wait for # of
                                                      // cycles without any 3D activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Wait_Cnt0;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the S3VD Power-Gating after wait for # of
                                                      // cycles without any S3VD activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Wait_Cnt1;
 
typedef union
{
    struct
    {
        unsigned int Wait_Cnt                  : 32;  // It will enable the ISP Power-Gating after wait for # of
                                                      // cycles without any ISP activities.
    } reg;
    unsigned int uint;
} Reg_Pwr_Mgr_Pg_Wait_Cnt2;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Bci_Skip;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Eu_Rd_Group;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Eu_Wr_Group;
 
typedef struct _Bci_Global_regs
{
    Reg_Block_Busy_Bits              reg_Block_Busy_Bits;
    Reg_Ring_Buf_Group               reg_Ring_Buf[4];
    Reg_Time_Stamp_Low               reg_Time_Stamp_Low;
    Reg_Time_Stamp_High              reg_Time_Stamp_High;
    Reg_Busy_Time_Stamp_Low          reg_Busy_Time_Stamp_Low;
    Reg_Busy_Time_Stamp_High         reg_Busy_Time_Stamp_High;
    Reg_Cur_Dma_Cmd                  reg_Cur_Dma_Cmd;
    Reg_Cur_Rbuf_Cmd                 reg_Cur_Rbuf_Cmd;
    Reg_Cur_Hrbuf_Cmd                reg_Cur_Hrbuf_Cmd;
    Reg_Cur_Vrbuf_Cmd                reg_Cur_Vrbuf_Cmd;
    Reg_Cur_Irbuf_Cmd                reg_Cur_Irbuf_Cmd;
    Reg_Mxu_Entries_Cfg              reg_Mxu_Entries_Cfg;
    Reg_Ogl_Cut_Flag_32              reg_Ogl_Cut_Flag_32;
    Reg_Bci_Misc_Control             reg_Bci_Misc_Control;
    Reg_Signature_Group              reg_Signature;
    Reg_Bci_Fence_Counter_Group      reg_Bci_Fence_Counter[4];
    Reg_Ila_Counters                 reg_Ila_Counters[19];
    Reg_Save_Restore_Control         reg_Save_Restore_Control;
    Reg_Restore_Mask                 reg_Restore_Mask;
    Reg_Pwr_Mgr_Cfg_En               reg_Pwr_Mgr_Cfg_En;
    Reg_Pwr_Mgr_Clk_Surge_Cnt        reg_Pwr_Mgr_Clk_Surge_Cnt;
    Reg_Pwr_Mgr_Status               reg_Pwr_Mgr_Status;
    Reg_Pwr_Mgr_Pg_Pd_Misc0          reg_Pwr_Mgr_Pg_Pd_Misc0;
    Reg_Pwr_Mgr_Pg_Pd_Misc1          reg_Pwr_Mgr_Pg_Pd_Misc1;
    Reg_Pwr_Mgr_Pg_Pu_Misc_0         reg_Pwr_Mgr_Pg_Pu_Misc_0;
    Reg_Pwr_Mgr_Pg_Pu_Misc_1         reg_Pwr_Mgr_Pg_Pu_Misc_1;
    Reg_Pwr_Mgr_Cg_Wait_Cnt0         reg_Pwr_Mgr_Cg_Wait_Cnt0;
    Reg_Pwr_Mgr_Cg_Wait_Cnt1         reg_Pwr_Mgr_Cg_Wait_Cnt1;
    Reg_Pwr_Mgr_Cg_Wait_Cnt2         reg_Pwr_Mgr_Cg_Wait_Cnt2;
    Reg_Pwr_Mgr_Pg_Wait_Cnt0         reg_Pwr_Mgr_Pg_Wait_Cnt0;
    Reg_Pwr_Mgr_Pg_Wait_Cnt1         reg_Pwr_Mgr_Pg_Wait_Cnt1;
    Reg_Pwr_Mgr_Pg_Wait_Cnt2         reg_Pwr_Mgr_Pg_Wait_Cnt2;
    Reg_Bci_Skip                     reg_Bci_Skip[107];
    Reg_Eu_Rd_Group                  reg_Eu_Rd_Group[32];
    Reg_Eu_Wr_Group                  reg_Eu_Wr_Group[16];
} Bci_Global_regs;

#endif
