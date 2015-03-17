#ifndef _REGISTER_DEF_ELT_H
#define _REGISTER_DEF_ELT_H

#include      "bci_global_register_elt.h"
#include      "tas_reg_elt.h"
#include      "eu_vs_reg_elt.h"
#include      "iu_reg_elt.h"
#include      "eu_ps_reg_elt.h"
#include      "ff_registers_elt.h"
#include      "tu_reg_elt.h"
#include      "mxu_registers_elt.h"
#include      "miu_reg_elt.h"
#include      "reg_busy_engine_status_elt.h"

 //  Define the top level register structure
typedef struct _EliteRegs
{
	Bci_Global_regs					 Regs_BCI;
    Eu_Vs_regs                       Regs_EU_VS;
    Eu_Ps_regs                       Regs_EU_PS;
    Tas_regs                         Regs_TAS;
    Iu_regs                          Regs_IU;
    Tu_regs                          Regs_TU;
	Ff_regs                          Regs_FF;
	Mxu_regs                         Regs_MXU;
} EliteRegs;

#define MMIO_FLAG 0x000FF000
#define MMIO_MIU_START_ADDRESS                  0x00008000
#define MMIO_MIU_DYNAMIC_FB_START_ADDRESS       0x00008B00
#define MMIO_BCI_START_ADDRESS                  0x00000000
#define MMIO_MXU_START_ADDRESS                  0x0009000
#define MMIO_EU_RID_START_ADDRESS               (MMIO_BCI_START_ADDRESS|(Reg_Eu_Rd_Group_Offset<<2))
#define MMIO_EU_WID_START_ADDRESS               (MMIO_BCI_START_ADDRESS|(Reg_Eu_Wr_Group_Offset<<2))
#define MMIO_EU_END_ADDRESS                     (MMIO_BCI_START_ADDRESS|(BCI_GLOBAL_REG_LIMIT<<2))

#define MMIO_MXU_RANGETBL_MASK  0xC00
#define MMIO_MXU_RANGETBL_VALUE 0x400

#define MIU_DYNAMIC_FB_ENTRY_SIZE 0x400000

// Complement to FF_registers.h
#define FF_ZCLIPMIN_OFFSET 0
#define FF_ZCLIPMAX_OFFSET 1

#define FF_SFRONTFACE_OFFSET 0
#define FF_SBACKFACE_OFFSET  1

#define FF_RT_FMT_OFFSET  0
#define FF_RT_DESC_OFFSET 1
#define FF_RT_MISC_OFFSET 2

#define FF_RT_REG_SIZE 3
#define FF_RT_DST 0
#define FF_RT_SRC 1

#define FF_16FL_COLOR_OFFSET     0 //16 Float low dword
#define FF_16FH_COLOR_OFFSET     1 //16 Float high dword
#define FF_8888_COLOR_OFFSET     2 //8 8 8 8 unorm
#define FF_2101010L_COLOR_OFFSET 3 //2 10 10 10 unorm low dword store B10 G10 R10
#define FF_2101010H_COLOR_OFFSET 4 //2 10 10 10 unorm high dword extend A2 to A10

#define FF_FG_COLOR_OFFSET 0 //Front ground color offset
#define FF_BG_COLOR_OFFSET 1 //Back ground color offset

#endif
