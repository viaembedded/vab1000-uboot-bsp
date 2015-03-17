#ifndef _REG_BUSYSTATUS_ELT_H_
#define _REG_BUSYSTATUS_ELT_H_
typedef union
{
    struct
    {
        unsigned int BCI_busy_0        : 1;
        unsigned int RU_busy_1         : 1;
        unsigned int ZU_busy_2         : 1;
        unsigned int WBU_busy_3        : 1;
        unsigned int FALU_busy_4       : 1;
        unsigned int FFC_busy_5        : 1;
        unsigned int IU_busy_6         : 1;
        unsigned int EU_VS_busy_7      : 1;
        unsigned int EU_PS_busy_8      : 1;
        unsigned int TU_busy_9         : 1;
        unsigned int Reserved          : 22;
    };
    unsigned int uint;
} Reg_BusyStatus_elt;

#endif
