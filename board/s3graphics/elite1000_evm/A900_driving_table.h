



static struct drive_str driving_map[] = {

//68[7:0]=00------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x00,},
{.rw_reg = "write", .mask = 0xF8, .offset = 0x65, .w_reg = 0xA8,},
{.rw_reg = "write", .mask = 0xF8, .offset = 0x66, .w_reg = 0xD8,},
//68[7:0]=04------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x04,},
{.rw_reg = "write", .mask = 0x07, .offset = 0x66, .w_reg = 0x04,},
{.rw_reg = "write", .mask = 0x07, .offset = 0x67, .w_reg = 0x07,},
//68[7:0]=3C------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x3C,},
{.rw_reg = "write", .mask = 0xFF, .offset = 0x66, .w_reg = 0xFC,},
//68[7:0]=40------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x40,},
{.rw_reg = "write", .mask = 0xFF, .offset = 0x66, .w_reg = 0xFC,},
//68[7:0]=28------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x28,},
{.rw_reg = "write", .mask = 0xF0, .offset = 0x67, .w_reg = 0x05,},
//68[7:0]=08------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x08,},
{.rw_reg = "write", .mask = 0x9F, .offset = 0x65, .w_reg = 0x20,},
{.rw_reg = "write", .mask = 0xE0, .offset = 0x66, .w_reg = 0xA0,},
//68[7:0]=14------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x14,},
{.rw_reg = "write", .mask = 0xFF, .offset = 0x66, .w_reg = 0x94,},
//68[7:0]=18------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x18,},
{.rw_reg = "write", .mask = 0x3F, .offset = 0x64, .w_reg = 0x21,},
{.rw_reg = "write", .mask = 0xC0, .offset = 0x65, .w_reg = 0x00,},
//68[7:0]=08------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x08,},
{.rw_reg = "write", .mask = 0x1E, .offset = 0x65, .w_reg = 0x02,},
{.rw_reg = "write", .mask = 0xFF, .offset = 0x67, .w_reg = 0x10,},
//68[7:0]=0C------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x0C,},
{.rw_reg = "write", .mask = 0x1E, .offset = 0x64, .w_reg = 0x02,},
{.rw_reg = "write", .mask = 0xEB, .offset = 0x65, .w_reg = 0x89,},
{.rw_reg = "write", .mask = 0xFC, .offset = 0x66, .w_reg = 0x7C,},
{.rw_reg = "write", .mask = 0xFC, .offset = 0x67, .w_reg = 0x7C,},
//68[7:0]=10------------------------------------------------------
{.rw_reg = "write", .mask = 0xFF, .offset = 0x68, .w_reg = 0x10,},
{.rw_reg = "write", .mask = 0xBC, .offset = 0x64, .w_reg = 0x00,},
{.rw_reg = "write", .mask = 0xBC, .offset = 0x65, .w_reg = 0x00,},
{.rw_reg = "write", .mask = 0xBC, .offset = 0x66, .w_reg = 0x00,},
//Down to GEN1----------------------------------------------------
{.rw_reg = "write", .mask = 0x10, .offset = 0x5D, .w_reg = 0x10,},
//Finish tag------------------------------------------------------
{.rw_reg = "end"  , .mask = 0x00, .offset = 0x00, .w_reg = 0x00,},

};








