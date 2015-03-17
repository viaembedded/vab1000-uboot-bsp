#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libcbios.o

CBIOSDIR = ../../../../cbios


#COBJS-$(CONFIG_VIDEO_S3G) += 	$(CBIOSDIR)/CBios.o \
	$(CBIOSDIR)/CBiosShare.o \
	$(CBIOSDIR)/ChipShare/CBiosInterface.o \
	$(CBIOSDIR)/ChipShare/CBiosData.o      \
	$(CBIOSDIR)/ChipShare/CBiosUtil.o      \
	$(CBIOSDIR)/ChipShare/CBiosMode.o      \
	$(CBIOSDIR)/ChipShare/CBiosDevice.o    \
	$(CBIOSDIR)/ChipShare/CBiosCRT.o       \
	$(CBIOSDIR)/ChipShare/CBiosLCD.o       \
	$(CBIOSDIR)/ChipShare/CBiosLCDMonitor.o\
	$(CBIOSDIR)/ChipShare/CBiosDVI.o       \
	$(CBIOSDIR)/ChipShare/CBiosCH7301.o    \
	$(CBIOSDIR)/ChipShare/CBiosCH7305.o    \
	$(CBIOSDIR)/ChipShare/CBiosADV7511.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1636.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1625.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1632.o    \
	$(CBIOSDIR)/ChipShare/CBiosAD9389.o    \
	$(CBIOSDIR)/ChipShare/CBiosTV.o        \
	$(CBIOSDIR)/ChipShare/CBiosScaler.o    \
	$(CBIOSDIR)/ChipShare/CBiosPost.o      \
	$(CBIOSDIR)/ChipShare/CBiosI2C.o       \
	$(CBIOSDIR)/ChipShare/CBiosDP.o        \
	$(CBIOSDIR)/ChipShare/CBiosHDMIMonitor.o   \
	$(CBIOSDIR)/ChipShare/CBiosDPMonitor.o     \
	$(CBIOSDIR)/ChipShare/CBiosChipShare.o \
	$(CBIOSDIR)/ChipShare/CBiosEDID.o      \
	$(CBIOSDIR)/ChipShare/CBiosSecure.o      \
	$(CBIOSDIR)/ChipShare/CBiosDSI.o       \
	$(CBIOSDIR)/ChipShare/DSIPanel/CBiosHX8369A.o      \
	$(CBIOSDIR)/ChipShare/DSIPanel/CBiosHX8392A.o      \
	$(CBIOSDIR)/Elite/CBios_elt.o          \
	$(CBIOSDIR)/Elite/CBiosVCP_elt.o       \
	$(CBIOSDIR)/vga/s3g_vgaHW.o	$(CBIOSDIR)/CBios.o \
	$(CBIOSDIR)/CBiosShare.o \
	$(CBIOSDIR)/ChipShare/CBiosInterface.o \
	$(CBIOSDIR)/ChipShare/CBiosData.o      \
	$(CBIOSDIR)/ChipShare/CBiosUtil.o      \
	$(CBIOSDIR)/ChipShare/CBiosMode.o      \
	$(CBIOSDIR)/ChipShare/CBiosDevice.o    \
	$(CBIOSDIR)/ChipShare/CBiosCRT.o       \
	$(CBIOSDIR)/ChipShare/CBiosLCD.o       \
	$(CBIOSDIR)/ChipShare/CBiosLCDMonitor.o\
	$(CBIOSDIR)/ChipShare/CBiosDVI.o       \
	$(CBIOSDIR)/ChipShare/CBiosCH7301.o    \
	$(CBIOSDIR)/ChipShare/CBiosCH7305.o    \
	$(CBIOSDIR)/ChipShare/CBiosADV7511.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1636.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1625.o    \
	$(CBIOSDIR)/ChipShare/CBiosVT1632.o    \
	$(CBIOSDIR)/ChipShare/CBiosAD9389.o    \
	$(CBIOSDIR)/ChipShare/CBiosTV.o        \
	$(CBIOSDIR)/ChipShare/CBiosScaler.o    \
	$(CBIOSDIR)/ChipShare/CBiosPost.o      \
	$(CBIOSDIR)/ChipShare/CBiosI2C.o       \
	$(CBIOSDIR)/ChipShare/CBiosDP.o        \
	$(CBIOSDIR)/ChipShare/CBiosHDMIMonitor.o   \
	$(CBIOSDIR)/ChipShare/CBiosDPMonitor.o     \
	$(CBIOSDIR)/ChipShare/CBiosChipShare.o \
	$(CBIOSDIR)/ChipShare/CBiosEDID.o      \
	$(CBIOSDIR)/ChipShare/CBiosSecure.o      \
	$(CBIOSDIR)/ChipShare/CBiosDSI.o       \
	$(CBIOSDIR)/ChipShare/DSIPanel/CBiosHX8369A.o      \
	$(CBIOSDIR)/ChipShare/DSIPanel/CBiosHX8392A.o      \
	$(CBIOSDIR)/Elite/CBios_elt.o          \
	$(CBIOSDIR)/Elite/CBiosVCP_elt.o       \
	$(CBIOSDIR)/vga/s3g_vgaHW.o



COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

EXTRA_CFLAGS += -D__LINUX__  -D__uboot__  -I./libfb -I. -I./elite_include \
                -I$(CBIOSDIR)/global -I$(CBIOSDIR)/Elite -I$(CBIOSDIR)/ChipShare -I$(CBIOSDIR) -mno-unaligned-access

CFLAGS += $(EXTRA_CFLAGS)
HOSTCFLAGS += $(EXTRA_CFLAGS)
CPPFLAGS += $(EXTRA_CFLAGS)

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	#$(call cmd_link_o_target, $(OBJS))
	cp -f cbios_prebuild libcbios.o
#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
