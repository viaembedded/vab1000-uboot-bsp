/*
 * PCI initialisation for Elite 1000 board.
 *
 * (C) Copyright 2011
 * S3 Graphics
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

#include <config.h>

#include <common.h>
#include <pci.h>
#include <asm/io.h>
#if defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#include <fdt_support.h>
#endif

#include "pcie_elite.h"

#define PCIBIOS_SUCCESSFUL 0

struct mem_bar_rec {
	u32 phys_addr;
	int size;
	int type;
#define NORM_MEM 0
#define PREF_MEM 1
};

static struct mem_bar_rec bar_rec[6];
static int bar_num = 0;
static mem_space_base = ELITE_PCIE_MEM_PHYS_BASE;
static total_mem_space = ELITE_PCIE_MEM_SIZE;

static norm_mem_space_base;
static norm_total_mem_space = 0;

static pref_mem_space_base;
static pref_total_mem_space = 0;

struct pcie_port {
	u8			index;
	u8			root_bus_nr;
	u32 		base;
	int 		header_index;
};

static struct pci_controller elite_hose[3];
static struct pcie_port pcie_port[3];
static int num_pcie_ports;

static u32 get_dev_cfg_base_addr(int bus, unsigned int devfn)
{
	int i = 0;
	int j = 0;
	u32 dev_id;
	//struct pcie_port *pp = bus_to_port(bus->number);
	u32 dev_id_offset; //= pp->base + BUS_DEV_FUNC_ID_SECTION_OFFSET;
#if 0
	dev_id = (devfn << 8)|(bus);

	for (i = num_pcie_ports - 1; i >= 0; i--) {
		dev_id_offset = pcie_port[i].base + BUS_DEV_FUNC_ID_SECTION_OFFSET;
		for(j = 0; j < 16; j++) {
			if(dev_id == readw(dev_id_offset + j * 2))
				break;
		}
		if(j < 16) break;
	}
	return pcie_port[i].base + j * DEVICE_CFG_SIZE;
#endif

	return pcie_port[i].base + bus * DEVICE_CFG_SIZE;
}


static int
elite_pcie_read_config(int bus, unsigned int devfn, int where, int size,
                       u32 * value)
{
	int i;
	bool found = false;
	u32 dev_id;
	u32 dev_cfg_base_addr;
	struct pcie_port *pp;

	//dev_id = (bus->number << 8)| devfn;
	dev_id = (devfn << 8)|(bus);
	if(dev_id !=(2<<11) && dev_id != 1 ) {
		*value == 0xffffffff;
		return PCIBIOS_SUCCESSFUL;
	}


	dev_cfg_base_addr = get_dev_cfg_base_addr(bus,devfn);
	switch (size) {
	case 1:
		*((u8*)(value)) = readb((dev_cfg_base_addr + where));
		break;
	case 2:
		*((u16*)(value)) = readw((dev_cfg_base_addr + (where & ~1)));
		break;
	case 4:
		*((u32*)(value)) = readl((dev_cfg_base_addr + (where & ~3)));
		break;
	}

	return PCIBIOS_SUCCESSFUL;
}

static int
elite_pcie_write_config(int bus, unsigned int devfn, int where, int size,
                        u32 value)
{
	u32 dev_cfg_base_addr;
	u32 dev_id;

	dev_id = (devfn << 8)|(bus);

	//if(dev_id =(2<<11) &&  where <= 0x34 && where >= 0x30) {
	//	return PCIBIOS_SUCCESSFUL;
	//}

	dev_id = (devfn << 8)|(bus);
	if(dev_id !=(2<<11) && dev_id != 1 ) {
		return PCIBIOS_SUCCESSFUL;
	}

	dev_cfg_base_addr = get_dev_cfg_base_addr(bus, devfn);

	switch (size) {
	case 1:
		writeb(value, dev_cfg_base_addr + where);
		break;
	case 2:
		writew(value, dev_cfg_base_addr + (where & ~1));
		break;
	case 4:
		writel(value, dev_cfg_base_addr + (where & ~3));
		break;
	}

	return PCIBIOS_SUCCESSFUL;
}


static int pci_elite_hose_read_config_dword (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned int *val)
{
	return elite_pcie_read_config(PCI_BUS(dev), 
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 4, val);
}

static int pci_elite_hose_read_config_word (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned short *val)
{
	return elite_pcie_read_config(PCI_BUS(dev),
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 2, val);
}

static int pci_elite_hose_read_config_byte (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned char *val)
{
	return elite_pcie_read_config(PCI_BUS(dev),
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 1, val);
}

static int pci_elite_hose_write_config_byte (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned char val)
{
	return elite_pcie_write_config(PCI_BUS(dev),
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 4, val);
}

static int pci_elite_hose_write_config_word (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned short val)
{
	return elite_pcie_write_config(PCI_BUS(dev),
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 2, val);
}

static int pci_elite_hose_write_config_dword (struct pci_controller *hose,
				pci_dev_t dev, int where, unsigned int val)
{
	return elite_pcie_write_config(PCI_BUS(dev),
			(PCI_DEV(dev) << 3) & PCI_FUNC(dev), where, 1, val);
}

static void elite_pcie_preinit(void)
{
	int i;

	for (i = 0; i < num_pcie_ports; i++) {
		struct pcie_port *pp = pcie_port + i;
		/* 0x300 is the special register in the root port that is designed by HW to solve the
		* problem of 0xC0000000~0xCFFFFFFF I/O space address range while elite-1K only support 64k
		* I/O address space. The value set in this register defines the upper 16bits of the I/O space
		* of this root port. Combied with the I/O address in the EP as the lower 16bits under
		* this port, 32bits I/O address can is formed.
		*/
		if (pp->index == 0) {

			writew((2<<11), pp->base + 0x260);//root port
			writew(0x1, pp->base + 0x262);//11st dev
			writew(0xcfff, pp->base + 0x300);

		} else {
			writew(0xcfff, pp->base + 0x300);
		}
	}
}


static void elite_pcie_bar_rec(u32 addr)
{
	int i;
	u32 tmp, bar_addr,sz;


	for (i = 0; i < 6; ) {
		bar_addr = addr + PCI_BASE0 + i*4;
		tmp = readl(bar_addr);

		if((tmp & 0x9) == 0x0) { //norm mem bar
			writel(0xffffffff, bar_addr);
			sz = readl(bar_addr);
			writel(tmp, bar_addr);

			sz &= ~ 0xf ;
			sz = ((~sz) & (sz - 1)) + 1;
			if(!sz ) goto next;

			bar_rec[bar_num].phys_addr = bar_addr;
			bar_rec[bar_num].size= sz;
			bar_rec[bar_num].type = NORM_MEM;
			bar_num++;
		} else if ((tmp & 0x9) == 0x8) { //pref mem bar
			writel(0xffffffff, bar_addr);
			sz = readl(bar_addr);
			writel(tmp, bar_addr);

			sz &= ~ 0xf ;
			sz = ((~sz) & (sz - 1)) + 1;
			if(!sz ) goto next;

			bar_rec[bar_num].phys_addr = bar_addr;
			bar_rec[bar_num].size= sz;
			bar_rec[bar_num].type = PREF_MEM;
			bar_num++;
		}
	next:
		if(((tmp>>1)&0x3) == 0x2)//64 bits bar, ignore it
			i += 2;
		else
			i++;
	}
}


static void elite_pcie_bar_assigning(struct pci_controller *hose, u32 root_base)
{
	int i;
	u32 base;
	pci_addr_t bus_start = CONFIG_SYS_PCI_MEMORY_BUS;

	norm_mem_space_base = mem_space_base;
	base = mem_space_base;

	for(i = 0; i < bar_num; i ++) {
		if(bar_rec[i].type == NORM_MEM) { //norm mem
			pci_set_region(hose->regions + 1, bus_start, base, bar_rec[i].size,
				PCI_REGION_MEM | PCI_REGION_SYS_MEMORY);
			bus_start = bar_rec[i].size;

			if(base &  (bar_rec[i].size -1)){
				base = ALIGN(base , bar_rec[i].size);
			}
			printf("Set %d norm base address %p base %p, size %p\n", i, bar_rec[i].phys_addr, base, bar_rec[i].size);
			writel(base, bar_rec[i].phys_addr);
			norm_total_mem_space += bar_rec[i].size;
			base += bar_rec[i].size;
		}
	}

	pref_mem_space_base = base;

	if(((norm_mem_space_base) >> 16) == ((pref_mem_space_base) >> 16)) {
		pref_mem_space_base &= ~0xffff;
		pref_mem_space_base += 0x100000;
		base = pref_mem_space_base;
	}

	for(i = 0; i < bar_num; i ++) {
		if(bar_rec[i].type == PREF_MEM) { //pref mem
			pci_set_region(hose->regions + 2, bus_start, base, bar_rec[i].size,
				PCI_REGION_MEM | PCI_REGION_PREFETCH);
			bus_start = bar_rec[i].size;

			if(base &  (bar_rec[i].size -1)){
				base = ALIGN(base , bar_rec[i].size);
			}
			printf("Set %d perf base address %p base %p, size %p\n", i, bar_rec[i].phys_addr, base, bar_rec[i].size);
			writel(base, bar_rec[i].phys_addr);
			pref_total_mem_space += bar_rec[i].size;
			base += bar_rec[i].size;
		}
	}

	writew(norm_mem_space_base >> 16, root_base + 0x20);
	writew((norm_mem_space_base + norm_total_mem_space) >> 16 , root_base + 0x22);

	writew(pref_mem_space_base >> 16, root_base + 0x24);
	writew((pref_mem_space_base + pref_total_mem_space) >> 16, root_base + 0x26);
	/* command */
	writew(0x147, root_base + 0x4);
	writew(0x8, root_base + 0xc);
	writel(0x00010129, root_base + 0x3c);
	writel(0x00010100, root_base + 0x18);
	writel(0x00001010, root_base + 0x1c);
	writew(0x1, root_base + 0x262);
}

static void elite_pcie_sizing(void)
{
	int i;

	for (i = 0; i < num_pcie_ports; i++) {
		struct pcie_port *pp = pcie_port + i;
		struct pci_controller *hose = elite_hose + i;

		if (pp->index == 0) {
			//	writew((2<<11), pp->base + 0x260);//root port
			//	writew(0x1, pp->base + 0x262);//11st dev
			writew(0xcfff, pp->base + 0x300);

			/* PCI I/O space */
			pci_set_region(hose->regions,
		       			CONFIG_SYS_PCI_IO_BUS, 
						ELITE_PCIE_IO_PHYS_BASE(i), 
						ELITE_PCIE_IO_PHYS_SIZE , PCI_REGION_IO);

			elite_pcie_bar_rec(pp->base + DEVICE_CFG_SIZE);// device cfg space
			elite_pcie_bar_assigning(hose, pp->base);// device cfg space

		} else {
			writew(0xcfff, pp->base + 0x300);
		}
	}
}

static void add_pcie_port(int index, unsigned long base)
{
	u32 value;

	struct pcie_port *pp = &pcie_port[num_pcie_ports++];
	printf("Elite PCIe port %d: ", index);
	pp->index = index;
	pp->header_index = 0;
	pp->root_bus_nr = -1;
	pp->base = base;
#if 1	
	/* reset */
	value = readl(pp->base + 0x254);
	value &= 0x7fffffff;
	writel(value, pp->base + 0x254);
	
	udelay(15000);
	 
	value = readl(pp->base + 0x254);
	value |= 0x80000000;
	writel(value, pp->base + 0x254);
#endif

	printf("pp->base %d    0x%x\n", index, pp->base);

	value = readl(pp->base + 0x250);
	printf("RP before 0x250 0x%x\n", value);
	//value |= (1<< 3);
	value &= ~(1 << 2);
	printf("RP middle 0x250 0x%x\n", value);
	writel(value, pp->base + 0x250);
	udelay(15000);
	value = readl(pp->base + 0x250);
	printf("RP after 0x250 0x%x\n", value);
}


static void elite_pcie_ports_init(int init_port0, int init_port1, int init_port2)
{
	if (init_port0)
		add_pcie_port(0, ELITE_PCIE0_BASE_ADDR);

	if (init_port1)
		add_pcie_port(1, ELITE_PCIE1_BASE_ADDR);

	if (init_port2)
		add_pcie_port(2, ELITE_PCIE2_BASE_ADDR);
}

void pci_init_board (void)
{
	int i;

	elite_pcie_ports_init(1, 1, 0);
	//elite_pcie_preinit();
	elite_pcie_sizing();

	
	for (i = 0; i < num_pcie_ports; i++) {
		struct pci_controller *hose = elite_hose + i;

		hose->first_busno = 0;
		hose->last_busno = 0;

		hose->region_count = 3;

		pci_set_ops(hose,
		    	pci_elite_hose_read_config_byte,
		    	pci_elite_hose_read_config_word,
		    	pci_elite_hose_read_config_dword,
		    	pci_elite_hose_write_config_byte,
		    	pci_elite_hose_write_config_word,
		    	pci_elite_hose_write_config_dword);

		pci_register_hose(hose);
		//hose->last_busno = pci_hose_scan(hose);
		hose->last_busno = 2;
	}

	debug ("Done PCI initialization\n");
	return;
}
