#include "elt_display.h"
#include "cbios_interface.h"
#include "fb_lib.h"

adapter_t *gadapter = NULL;

s3g_modeset_t dmtv1r12[] = {
	[0] = {
		.width = 640,
		.height = 350,
		.refresh = 85,
	},
	[1] = {
		.width = 640,
		.height = 400,
		.refresh = 85,
	},
	[2] = {
		.width = 720,
		.height = 400,
		.refresh = 85,
	},
	[3] = {
		.width = 640,
		.height = 480,
		.refresh = 60,
	},
	[4] = {
		.width = 640,
		.height = 480,
		.refresh = 72,
	},
	[5] = {
		.width = 640,
		.height = 480,
		.refresh = 75,
	},
	[6] = {
		.width = 640,
		.height = 480,
		.refresh = 85,
	},
	[7] = {
		.width = 800,
		.height = 600,
		.refresh = 56,
	},
	[8] = {
		.width = 800,
		.height = 600,
		.refresh = 60,
	},
	[9] = {
		.width = 800,
		.height = 600,
		.refresh = 72,
	},
	[0x0a] = {
		.width = 800,
		.height = 600,
		.refresh = 75,
	},
	[0x0b] = {
		.width = 800,
		.height = 600,
		.refresh = 85,
	},
	[0x0c] = {
		.width = 800,
		.height = 600,
		.refresh = 120,
	},
	[0x0d] = {
		.width = 848,
		.height = 480,
		.refresh = 60,
	},
	[0x0e] = {
		.width = 1024,
		.height = 768,
		.refresh = 43,
	},
	[0x0f] = {
		.width = 1024,
		.height = 768,
		.refresh = 60,
	},
	[0x10] = {
		.width = 1024,
		.height = 768,
		.refresh = 70,
	},
	[0x11] = {
		.width = 1024,
		.height = 768,
		.refresh = 75,
	},
	[0x12] = {
		.width = 1024,
		.height = 768,
		.refresh = 85,
	},
	[0x13] = {
		.width = 1024,
		.height = 768,
		.refresh = 120,
	},
	[0x14] = {
		.width = 1152,
		.height = 864,
		.refresh = 75,
	},
	[0x54] = {
		.width = 1280,
		.height = 720,
		.refresh = 60,
	},
	[0x15] = {
		.width = 1280,
		.height = 768,
		.refresh = 60, //RB
	},
	[0x16] = {
		.width = 1280,
		.height = 768,
		.refresh = 60, 
	},
	[0x17] = {
		.width = 1280,
		.height = 768,
		.refresh = 75, 
	},
	[0x18] = {
		.width = 1280,
		.height = 768,
		.refresh = 85, 
	},
	[0x19] = {
		.width = 1280,
		.height = 768,
		.refresh = 120, //RB 
	},
	[0x1a] = {
		.width = 1280,
		.height = 800,
		.refresh = 60, //RB 
	},
	[0x1b] = {
		.width = 1280,
		.height = 800,
		.refresh = 60,
	},
	[0x1c] = {
		.width = 1280,
		.height = 800,
		.refresh = 75,
	},
	[0x1d] = {
		.width = 1280,
		.height = 800,
		.refresh = 85,
	},
	[0x1e] = {
		.width = 1280,
		.height = 800,
		.refresh = 120, //RB
	},
	[0x1f] = {
		.width = 1280,
		.height = 960,
		.refresh = 60, 
	},
	[0x20] = {
		.width = 1280,
		.height = 960,
		.refresh = 85, 
	},
	[0x21] = {
		.width = 1280,
		.height = 960,
		.refresh = 120, //RB
	},
	[0x22] = {
		.width = 1280,
		.height = 1024,
		.refresh = 60, 
	},
	[0x23] = {
		.width = 1280,
		.height = 1024,
		.refresh = 75, 
	},
	[0x24] = {
		.width = 1280,
		.height = 1024,
		.refresh = 85, 
	},
	[0x25] = {
		.width = 1280,
		.height = 1024,
		.refresh = 120, //RB
	},
	[0x26] = {
		.width = 1360,
		.height = 768,
		.refresh = 60, 
	},
	[0x27] = {
		.width = 1360,
		.height = 768,
		.refresh = 120, //RB
	},
	[0x50] = {
		.width = 1366,
		.height = 768,
		.refresh = 60,
	},
	[0x55] = {
		.width = 1366,
		.height = 768,
		.refresh = 60, //RB
	},
	[0x28] = {
		.width = 1400,
		.height = 1050,
		.refresh = 60, //RB
	},
	[0x29] = {
		.width = 1400,
		.height = 1050,
		.refresh = 60, 
	},
	[0x2a] = {
		.width = 1400,
		.height = 1050,
		.refresh = 75, 
	},
	[0x2b] = {
		.width = 1400,
		.height = 1050,
		.refresh = 85, 
	},
	[0x2c] = {
		.width = 1400,
		.height = 1050,
		.refresh = 120, //RB
	},
	[0x2d] = {
		.width = 1440,
		.height = 900,
		.refresh = 60, //RB
	},
	[0x2e] = {
		.width = 1440,
		.height = 900,
		.refresh = 60, 
	},
	[0x2f] = {
		.width = 1440,
		.height = 900,
		.refresh = 75, 
	},
	[0x30] = {
		.width = 1440,
		.height = 900,
		.refresh = 85, 
	},
	[0x31] = {
		.width = 1440,
		.height = 900,
		.refresh = 120, //RB 
	},
	[0x52] = {
		.width = 1600,
		.height = 900,
		.refresh = 60, //RB 
	},
	[0x32] = {
		.width = 1600,
		.height = 1200,
		.refresh = 60, 
	},
	[0x33] = {
		.width = 1600,
		.height = 1200,
		.refresh = 65, 
	},
	[0x34] = {
		.width = 1600,
		.height = 1200,
		.refresh = 70, 
	},
	[0x35] = {
		.width = 1600,
		.height = 1200,
		.refresh = 75, 
	},
	[0x36] = {
		.width = 1600,
		.height = 1200,
		.refresh = 85, 
	},
	[0x37] = {
		.width = 1600,
		.height = 1200,
		.refresh = 120, //RB 
	},
	[0x38] = {
		.width = 1680,
		.height = 1050,
		.refresh = 60, //RB 
	},
	[0x39] = {
		.width = 1680,
		.height = 1050,
		.refresh = 60, 
	},
	[0x3a] = {
		.width = 1680,
		.height = 1050,
		.refresh = 75, 
	},
	[0x3b] = {
		.width = 1680,
		.height = 1050,
		.refresh = 85, 
	},
	[0x3c] = {
		.width = 1680,
		.height = 1050,
		.refresh = 120, //RB 
	},
	[0x3d] = {
		.width = 1792,
		.height = 1344,
		.refresh = 60, 
	},
	[0x3e] = {
		.width = 1792,
		.height = 1344,
		.refresh = 75, 
	},
	[0x3f] = {
		.width = 1792,
		.height = 1344,
		.refresh = 120, //RB
	},
	[0x40] = {
		.width = 1856,
		.height = 1392,
		.refresh = 60,
	},
	[0x41] = {
		.width = 1856,
		.height = 1392,
		.refresh = 75,
	},
	[0x42] = {
		.width = 1856,
		.height = 1392,
		.refresh = 120, //RB
	},
	[0x51] = {
		.width = 1920,
		.height = 1280,
		.refresh = 60,
	},
	[0x43] = {
		.width = 1920,
		.height = 1200,
		.refresh = 60, //RB
	},
	[0x44] = {
		.width = 1920,
		.height = 1200,
		.refresh = 60,
	},
	[0x45] = {
		.width = 1920,
		.height = 1200,
		.refresh = 75,
	},
	[0x46] = {
		.width = 1920,
		.height = 1200,
		.refresh = 85,
	},
	[0x47] = {
		.width = 1920,
		.height = 1200,
		.refresh = 120, //RB
	},
	[0x48] = {
		.width = 1920,
		.height = 1440,
		.refresh = 60,
	},
	[0x49] = {
		.width = 1920,
		.height = 1440,
		.refresh = 75,
	},
	[0x4a] = {
		.width = 1920,
		.height = 1440,
		.refresh = 120, //RB
	},
	[0x53] = {
		.width = 2048,
		.height = 1150,
		.refresh = 60, //RB
	},
	[0x4b] = {
		.width = 2560,
		.height = 1600,
		.refresh = 60, //RB
	},
	[0x4c] = {
		.width = 2560,
		.height = 1600,
		.refresh = 60, 
	},
	[0x4d] = {
		.width = 2560,
		.height = 1600,
		.refresh = 75, 
	},
	[0x4e] = {
		.width = 2560,
		.height = 1600,
		.refresh = 85, 
	},
	[0x4f] = {
		.width = 2560,
		.height = 1600,
		.refresh = 120, //RB
	},
};


#define F1 3/4
#define F2 1/4
#define F3 3/40
static void vba1(adapter_t *adapter, unsigned int iga_index)
{
    int i;
    s3g_pixel color = 0;

    for(i=0; i<8; i++) 
    {
        switch(i) {
        case 0://white
	        color = s3g_makecol(adapter, iga_index, 255, 255, 255, 0xff);
            break;
        case 1://yellow
            color = s3g_makecol(adapter, iga_index, 255, 255, 0, 0xff);
            break;
        case 2://light blue
            color = s3g_makecol(adapter, iga_index, 0, 255, 255, 0xff);
            break;
        case 3://green
            color = s3g_makecol(adapter, iga_index, 0, 255, 0, 0xff);
            break;
        case 4://pink
            color = s3g_makecol(adapter, iga_index, 255, 0, 255, 0xff);
            break;
        case 5://red
            color = s3g_makecol(adapter, iga_index, 255, 0, 0, 0xff);
            break;
        case 6://blue
            color = s3g_makecol(adapter, iga_index, 0, 0, 255, 0xff);
            break;
        case 7://black
            color = s3g_makecol(adapter, iga_index, 0, 0, 0, 0xff);
            break;
        default:
            break;
        }

        s3g_rectfill(adapter, iga_index, 
                 i*s3g_getxres(adapter, iga_index)/8,
                 0, 
                 (i+1)*s3g_getxres(adapter, iga_index)/8,
                 s3g_getyres(adapter, iga_index), color);
    }
}

static void vba2(adapter_t *adapter, unsigned int iga_index)
{
    int i;
    s3g_pixel color = 0;

    for(i=0; i<8; i++) 
    {
        switch(i) {
        case 0://white
	        color = s3g_makecol(adapter, iga_index, 255, 255, 255, 0xff);
            break;
        case 1://yellow
            color = s3g_makecol(adapter, iga_index, 255*F1, 255*F1, 0, 0xff);
            break;
        case 2://light blue
            color = s3g_makecol(adapter, iga_index, 0, 255*F1, 255*F1, 0xff);
            break;
        case 3://green
            color = s3g_makecol(adapter, iga_index, 0, 255*F1, 0, 0xff);
            break;
        case 4://pink
            color = s3g_makecol(adapter, iga_index, 255*F1, 0, 255*F1, 0xff);
            break;
        case 5://red
            color = s3g_makecol(adapter, iga_index, 255*F1, 0, 0, 0xff);
            break;
        case 6://blue
            color = s3g_makecol(adapter, iga_index, 0, 0, 255*F1, 0xff);
            break;
        case 7://black
            color = s3g_makecol(adapter, iga_index, 0, 0, 0, 0xff);
            break;
        default:
            break;
        }

        s3g_rectfill(adapter, iga_index, 
                 i*s3g_getxres(adapter, iga_index)/8,
                 0, 
                 (i+1)*s3g_getxres(adapter, iga_index)/8,
                 s3g_getyres(adapter, iga_index), color);
    }
}
static void vba3(adapter_t *adapter, unsigned int iga_index)
{
    int i;
    s3g_pixel color = 0;

    for(i=0; i<8; i++) 
    {
        switch(i) {
        case 0://white
	        color = s3g_makecol(adapter, iga_index, 255, 255, 255, 0xff);
            break;
        case 1://yellow
            color = s3g_makecol(adapter, iga_index, 255, 255,255*F2, 0xff);
            break;
        case 2://light blue
            color = s3g_makecol(adapter, iga_index, 255*F2, 255, 255, 0xff);
            break;
        case 3://green
            color = s3g_makecol(adapter, iga_index, 255*F2, 255, 255*F2, 0xff);
            break;
        case 4://pink
            color = s3g_makecol(adapter, iga_index, 255, 255*F2, 255, 0xff);
            break;
        case 5://red
            color = s3g_makecol(adapter, iga_index, 255, 255*F2, 255*F2, 0xff);
            break;
        case 6://blue
            color = s3g_makecol(adapter, iga_index, 255*F2, 255*F2, 255, 0xff);
            break;
        case 7://black
            color = s3g_makecol(adapter, iga_index, 0, 0, 0, 0xff);
            break;
        default:
            break;
        }

        s3g_rectfill(adapter, iga_index, 
                 i*s3g_getxres(adapter, iga_index)/8,
                 0, 
                 (i+1)*s3g_getxres(adapter, iga_index)/8,
                 s3g_getyres(adapter, iga_index), color);
    }
}
static void vba4(adapter_t *adapter, unsigned int iga_index)
{
    int i;
    s3g_pixel color = 0;

    for(i=0; i<8; i++) 
    {
        switch(i) {
        case 0://white
	        color = s3g_makecol(adapter, iga_index, 255*F1, 255*F1, 255*F1, 0xff);
            break;
        case 1://yellow
            color = s3g_makecol(adapter, iga_index, 255*F1, 255*F1, 255*F3, 0xff);
            break;
        case 2://light blue
            color = s3g_makecol(adapter, iga_index, 255*F3, 255*F1, 255*F1, 0xff);
            break;
        case 3://green
            color = s3g_makecol(adapter, iga_index, 255*F3, 255*F1, 255*F3, 0xff);
            break;
        case 4://pink
            color = s3g_makecol(adapter, iga_index, 255*F1, 255*F3, 255*F1, 0xff);
            break;
        case 5://red
            color = s3g_makecol(adapter, iga_index, 255*F1, 255*F3, 255*F3, 0xff);
            break;
        case 6://blue
            color = s3g_makecol(adapter, iga_index, 255*F3, 255*F3, 255*F1, 0xff);
            break;
        case 7://black
            color = s3g_makecol(adapter, iga_index, 255*F3, 255*F3, 255*F3, 0xff);
            break;
        default:
            break;
        }

        s3g_rectfill(adapter, iga_index, 
                 i*s3g_getxres(adapter, iga_index)/8,
                 0, 
                 (i+1)*s3g_getxres(adapter, iga_index)/8,
                 s3g_getyres(adapter, iga_index), color);
    }
}

/* fb_var_screeninfo: s3g_modeset_t */
static void s3g_draw_color_bars(adapter_t *adapter, unsigned int iga_index, unsigned char cmd)
{
	
	switch(cmd)
	{
	case 1:
		vba1(adapter, iga_index);
		break;
	case 2:
		vba2(adapter, iga_index);
		break;
	case 3:
		vba3(adapter, iga_index);
		break;
	case 4:
		vba4(adapter, iga_index);
		break;
	default:
		break;
	}	

}

static void s3g_fill_sc_color(adapter_t *adapter, unsigned int iga_index, unsigned int color)
{    
    s3g_clear_screen(adapter, iga_index, color);
}

static void s3g_draw_box(adapter_t *adapter, unsigned int iga_index)
{
    s3g_pixel red;
    s3g_pixel green;
    s3g_pixel blue;
    s3g_pixel white;
    s3g_pixel black;
    unsigned int high = s3g_getyres(adapter, iga_index)/8; /* high of gradiant color bars */
    unsigned int dist = (s3g_getyres(adapter, iga_index) - high)/10; /* distance between gradiant color bars */
    unsigned int border1 = dist;
    unsigned int border2 = 5;

	
    red  = s3g_makecol(adapter, iga_index, 255, 0, 0, 0xff);
    green = s3g_makecol(adapter, iga_index, 0, 255, 0, 0xff);
    blue = s3g_makecol(adapter, iga_index, 0, 0, 255, 0xff);
    white = s3g_makecol(adapter, iga_index, 255, 255, 255, 0xff);
    black = s3g_makecol(adapter, iga_index, 0, 0, 0, 0xff);

    s3g_clear_screen(adapter, iga_index, black);

    /* Draws gradiant color bars */
    s3g_rectfill_omber(adapter, iga_index, border1, s3g_getyres(adapter, iga_index)/2 - 2*high - dist*3/2,
            s3g_getxres(adapter, iga_index) - border1,
            s3g_getyres(adapter, iga_index)/2 - high - dist*3/2, red);
    s3g_rectfill_omber(adapter, iga_index, border1, s3g_getyres(adapter, iga_index)/2 - high - dist/2,
            s3g_getxres(adapter, iga_index) - border1,
            s3g_getyres(adapter, iga_index)/2 - dist/2, green);
    s3g_rectfill_omber(adapter, iga_index, border1, s3g_getyres(adapter, iga_index)/2 + dist/2,
            s3g_getxres(adapter, iga_index) - border1,
            s3g_getyres(adapter, iga_index)/2 + high + dist/2, blue);
    s3g_rectfill_omber(adapter, iga_index, border1, s3g_getyres(adapter, iga_index)/2 + high + dist*3/2,
            s3g_getxres(adapter, iga_index) - border1,
            s3g_getyres(adapter, iga_index)/2 + 2*high + dist*3/2, white);
	
    /* Draws a rectangle (only borders) */
    s3g_rect(adapter, iga_index, border2, border2, 
            s3g_getxres(adapter, iga_index) - border2, 
            s3g_getyres(adapter, iga_index) - border2, white);
	
    /* Draws 'X'-shaped cross lines */
    s3g_line(adapter, iga_index, border2, border2,
            s3g_getxres(adapter, iga_index) - border2,
	    s3g_getyres(adapter, iga_index) - border2, white);
            s3g_line(adapter, iga_index, s3g_getxres(adapter, iga_index) - border2, border2,
            border2, s3g_getyres(adapter, iga_index) - border2, white);
}



static bool parse_edid(unsigned char *data) {
	unsigned char i,j,sum=0,temp;
	unsigned char version, reversion,ceaversion;
	unsigned int cascii;


	printf("---------------------------------------------------\n");
	for (j=0; j<0x10; j++) {
		printf("%02x ",j<<4);
		for(i=0; i<0x10; i++) {
			temp = data[j*0x10 + i];
			printf(" %02X", temp);
			if (j <= 7)
				sum += temp;	// do 128 bytes check sum
		}
		printf("\n");
	}
	printf("Check Sum = %02X\n", sum);
	if(sum!=0) {
		printf("Check Sum Error!\n");
		return(false);
	}

	if(data[0]==0) {	// EDID 1.x
		if((data[1]!=0xFF)||(data[2]!=0xFF)||(data[3]!=0xFF)||(data[4]!=0xFF)||(data[5]!=0xFF)||(data[6]!=0xFF)||(data[7]!=0)) {
			printf("EDID 1.x Header Error!\n");
			return(false);
		}

		version = data[0x12];
		reversion = data[0x13];
		printf("\nEDID Version %d Reversion %d.\n",version,reversion);

		cascii = (data[0x08]<<8)+data[0x09];
		printf("Manufacturer Name: %c%c%c\n", ((cascii>>10)&0x1F)+0x40, ((cascii>>5)&0x1F)+0x40, (cascii&0x1F)+0x40);
		printf("Product Code: %04X\n", *((unsigned short *)(data+0x0a)));
		printf("Serial Number: %08X\n", *((unsigned int *)(data+0x0c)));
		printf("Manufacture Year/Week: %04d/%02d\n", data[0x11]+1990, data[0x10]);

		printf("Video Input Definition:\n");
		if((data[0x14]&0x80)==0)
			printf("\tAnalog\nSignal Level: ");
		else
			printf("\tDigital\nSignal Level: ");
		
		switch((data[0x14]>>5)&0x03){
			case 0:
				printf("0.700, 0.300 (1.000 V p-p)\n");	
				break;
			case 1:
				printf("0.714, 0.286 (1.000 V p-p)\n");	
				break;
			case 2:
				printf("1.000, 0.400 (1.400 V p-p)\n");	
				break;
			case 3:
				printf("0.700, 0.000 (0.700 V p-p)\n");	
				break;
		}
		
		printf("\tBlank-to-black setup or pedestal : ");
		if((data[0x14]&0x10)==0)
			printf("NOT ");
		printf("required\n");
		printf("\tSeparate syncs : ");
		if((data[0x14]&0x8)==0)
			printf("NOT ");
		printf("supported\n");
		printf("\tComposite sync(on Hsync line) : ");
		if((data[0x14]&0x4)==0)
			printf("NOT ");
		printf("supported\n");
		printf("\tSync. on green video : ");
		if((data[0x14]&0x2)==0)
			printf("NOT ");
		printf("supported\n");
		printf("\tSerration of the Vsync. Pulse : ");
		if((data[0x14]&0x1)==0)
			printf("NOT ");
		printf("required when composite sync. or sync-on-green video is used\n");
		
		printf("Max Image Size (Hor x Ver): %d x %d cm\n",data[0x15],data[0x16]);
		printf("Display Transfer Characteristic (Gamma): %0.2f\n", (data[0x17]+100)/100.0);

		printf("Feature Support:\n");
		printf("\tStand-by: ");
		if((data[0x15]&0x80)==0)
			printf("NOT ");
		printf("supported\n");
		printf("\tSuspend: ");
		if((data[0x15]&0x40)==0)
			printf("NOT ");
		printf("supported\n");
		printf("\tActive Off: ");
		if((data[0x15]&0x20)==0)
			printf("NOT ");
		printf("supported\n");
		switch((data[0x15]>>3)&0x03){
			case 0:
				printf("\tMonochrome / grayscale display\n");	
				break;
			case 1:
				printf("\tR/G/B color display\n");	
				break;
			case 2:
				printf("\tNon R/G/B multicolor display\n");	
				break;
			default:
				printf("\tUndefined\n");	
		}
		printf("Color Characteristics:\n");
		printf("\tRed_x: %0.3f\n",((data[0x1b]>>2)+((data[0x19]>>6)&0x3))/1024.0);
		printf("\tRed_y: %0.3f\n",((data[0x1c]>>2)+((data[0x19]>>4)&0x3))/1024.0);
		printf("\tGreen_x: %0.3f\n",((data[0x1d]>>2)+((data[0x19]>>2)&0x3))/1024.0);
		printf("\tGreen_y: %0.3f\n",((data[0x1e]>>2)+((data[0x19]>>0)&0x3))/1024.0);
		printf("\tBlue_x: %0.3f\n",((data[0x1f]>>2)+((data[0x1a]>>6)&0x3))/1024.0);
		printf("\tBlue_y: %0.3f\n",((data[0x20]>>2)+((data[0x1a]>>4)&0x3))/1024.0);
		printf("\tWhite_x: %0.3f\n",((data[0x21]>>2)+((data[0x1a]>>2)&0x3))/1024.0);
		printf("\tWhite_y: %0.3f\n",((data[0x22]>>2)+((data[0x1a]>>0)&0x3))/1024.0);
		

		printf("Established Timings:\n");
		if((data[0x23]&0x80)!=0)	printf("\t720 x 400 @ 70Hz IBM, VGA\n");
		if((data[0x23]&0x40)!=0)	printf("\t720 x 400 @ 88Hz IBM, XGA2\n");
		if((data[0x23]&0x20)!=0)	printf("\t640 x 480 @ 60Hz IBM, VGA\n");
		if((data[0x23]&0x10)!=0)	printf("\t640 x 480 @ 67Hz Apple, Mac II\n");
		if((data[0x23]&0x08)!=0)	printf("\t640 x 480 @ 72Hz VESA\n");
		if((data[0x23]&0x04)!=0)	printf("\t640 x 480 @ 75Hz VESA\n");
		if((data[0x23]&0x02)!=0)	printf("\t800 x 600 @ 56Hz VESA\n");
		if((data[0x23]&0x01)!=0)	printf("\t800 x 600 @ 60Hz\n");
		if((data[0x24]&0x80)!=0)	printf("\t800 x 600 @ 72Hz VESA\n");
		if((data[0x24]&0x40)!=0)	printf("\t800 x 600 @ 75Hz VESA\n");
		if((data[0x24]&0x20)!=0)	printf("\t832 x 624 @ 75Hz Apple, Mac II\n");
		if((data[0x24]&0x10)!=0)	printf("\t1024 x 768 @ 87Hz(I) IBM\n");
		if((data[0x24]&0x08)!=0)	printf("\t1024 x 768 @ 60Hz VESA\n");
		if((data[0x24]&0x04)!=0)	printf("\t1024 x 768 @ 70Hz VESA\n");
		if((data[0x24]&0x02)!=0)	printf("\t1024 x 768 @ 75Hz VESA\n");
		if((data[0x24]&0x01)!=0)	printf("\t1280 x 1024 @ 75Hz VESA\n");
		if((data[0x25]&0x80)!=0)	printf("\t1152 x 870 @ 75Hz Apple, Mac II\n");

		printf("Standard Timing Identification:\n");
		for(i=0x26;i<0x36;i+=2) {
			if(data[i]!=0x01) {
				int v;
				switch(data[i+1]>>6) {
					case 0:
						v = (data[i]+31)*8;
						break;
					case 1:
						v = (data[i]+31)*8*3/4;
						break;
					case 2:
						v = (data[i]+31)*8*4/5;
						break;
					case 3:
						v = (data[i]+31)*8*9/16;
						break;
					default:
						v = 0;
						printf("parse edid failed\n");
						break;
				}
				printf("\t%d x %d @ %dHz\n",(data[i]+31)*8, v, (data[i+1]&0x3F)+60);
			}
		}
		
		for(i=0x36;i<0x7E;i+=18) {
			if(*((unsigned short *)(data+i))!=0) {
				printf("Detailed Timing Descriptions #%d:\n", (i-0x36)/18+1);
				printf("\tPixel Clock: %0.2fMHz\n", (*((unsigned short *)(data+i)))/100.0);
				printf("\tHorizontal Active: %d\n", data[i+2]+((data[i+4]&0xF0)<<4));
				printf("\tHorizontal Blanking: %d\n", data[i+3]+((data[i+4]&0xF)<<8));
				printf("\tVertical Active: %d\n", data[i+5]+((data[i+7]&0xF0)<<4));
				printf("\tVertical Blanking: %d\n", data[i+6]+((data[i+7]&0xF)<<8));
				printf("\tHorizontal Sync Offset: %d\n", data[i+8]+((data[i+11]&0xC0)<<2));
				printf("\tHorizontal Sync Pulse Width: %d\n", data[i+9]+((data[i+11]&0x30)<<4));
				printf("\tVertical Sync Offset: %d\n", (data[i+10]>>4)+((data[i+11]&0xc)<<2));
				printf("\tVertical Sync Pulse Width: %d\n", (data[i+10]&0xF)+((data[i+11]&0x3)<<4));
				printf("\tHorizontal Image Size: %d mm\n", (data[i+12]>>4)+((data[i+14]&0xF0)<<4));
				printf("\tVertical Image Size: %d mm\n", (data[i+13]&0xF)+((data[i+14]&0xF)<<8));
				printf("\tHorizontal Border: %d\n", data[15]);
				printf("\tVertical Border: %d\n", data[16]);
				printf("\tFlags:\n");
				if((data[i+17]&0x80)!=0)
					printf("\t\tInterlaced\n");
				else
					printf("\t\tNon-interlaced\n");
				if((data[i+17]&0x60)==0)
					printf("\t\tNormal display, no stereo\n");
				else {
					//switch(((data[i+17]&0x60)>>4)+data[i+17]&0x1) {
					switch(((data[i+17]&0x60)>>4)+(data[i+17]&0x1)) {
						case 2:
							printf("\t\tField sequential stereo, right image when stereo sync = 1\n");
							break;
						case 4:
							printf("\t\tField sequential stereo, left image when stereo sync = 1\n");
							break;
						case 3:
							printf("\t\t2-way interleaved stereo, right image on even lines\n");
							break;
						case 5:
							printf("\t\t2-way interleaved stereo, left image on even lines\n");
							break;
						case 6:
							printf("\t\t4-way interleaved stereo\n");
							break;
						case 7:
							printf("\t\tSide-by-side interleaved stereo\n");
							break;
					}
				}
				switch((data[i+17]&0x18)>>3) {
					case 0:
						printf("\t\tAnalog composite\n");
						if((data[i+17]&0x4)!=0)
							printf("\t\tSerration (Hsync during Vsync) supported.\n");
						else
							printf("\t\tSerration (Hsync during Vsync) not supported.\n");

						if((data[i+17]&0x2)!=0)
							printf("\t\tSync pulses should appear on all 3 video signal lines.");
						else
							printf("\t\tSync on green video line only.");
						break;
					case 1:
						printf("\t\tBipolar analog composite\n");
						if((data[i+17]&0x4)!=0)
							printf("\t\tSerration (Hsync during Vsync) supported.\n");
						else
							printf("\t\tSerration (Hsync during Vsync) not supported.\n");

						if((data[i+17]&0x2)!=0)
							printf("\t\tSync pulses should appear on all 3 video signal lines.");
						else
							printf("\t\tSync on green video line only.");
						break;
					case 2:
						if((data[i+17]&0x4)!=0)
							printf("\t\tSerration (Hsync during Vsync) supported.\n");
						else
							printf("\t\tSerration (Hsync during Vsync) not supported.\n");

						if((data[i+17]&0x2)!=0)
							printf("\t\tComposite Sync Polarity is positive.\n");
						else
							printf("\t\tComposite Sync Polarity is negative.\n");
						printf("\t\tDigital composite\n");
						break;
					case 3:
						printf("\t\tDigital separate\n");
						if((data[i+17]&0x4)!=0)
							printf("\t\tVertical Sync Polarity is positive.\n");
						else
							printf("\t\tVertical Sync Polarity is negative.\n");

						if((data[i+17]&0x2)!=0)
							printf("\t\tHorizontal Sync Polarity is positive.\n");
						else
							printf("\t\tHorizontal Sync Polarity is negative.\n");
						break;
				}
			}
			else {
				switch(data[i+3]) {
					case 0xff:
						printf("Monitor S/N: ");
						for(j=5;j<18;j++) {
							if(data[i+j]!=0x0a)
								printf("%c",data[i+j]);
							else
								break;
						}
						printf("\n");
						break;
					case 0xfe:
						printf("ASCII Data String: ");
						for(j=5;j<18;j++) {
							if(data[i+j]!=0x0a)
								printf("%c",data[i+j]);
							else
								break;
						}
						printf("\n");
						break;
					case 0xfc:
						printf("Monitor Name: ");
						for(j=5;j<18;j++) {
							if(data[i+j]!=0x0a)
								printf("%c",data[i+j]);
							else
								break;
						}
						printf("\n");
						break;
					case 0xfd:
						printf("Monitor Range Limits:\n");
						printf("\tMin Vertical rate: %d Hz\n", data[i+5]);
						printf("\tMax Vertical rate: %d Hz\n", data[i+6]);
						printf("\tMin Horizontal rate: %d KHz\n", data[i+7]);
						printf("\tMax Horizontal rate: %d KHz\n", data[i+8]);
						if(data[i+9]!=0xff)
							printf("\tMax Supported Pixel Clock: %d MHz\n", data[i+9]*10);
						if((data[i+10]==0x00)&(data[1+11]==0x0a))
							printf("GTF Supported Data.\n");
						// GTF ////////////////////////////////
						break;
					case 0xfb:
						printf("Color Point: \n");
						//////////////////////////////////////
						break;
					case 0xfa:
						printf("Additional Standard Timing Identification:\n");
						for(j=i+5;i<i+17;i+=2) {
							if(data[j]!=0x01) {
								int v;
								switch(data[j+1]>>6) {
									case 0:
										v = (data[j]+31)*8;
										break;
									case 1:
										v = (data[j]+31)*8*3/4;
										break;
									case 2:
										v = (data[j]+31)*8*4/5;
										break;
									case 3:
										v = (data[j]+31)*8*9/16;
										break;
									default:
										v = 0;
										printf("Parse edid failed\n");
										break;
								}
								printf("\t%d x %d @ %dHz\n",(data[j]+31)*8, v, (data[j+1]&0x3F)+60);
							}
						}
						break;
				}
			}
		}
		if(data[0x7e]==1) {
			switch(data[0x80]) {
				case 0x01:
					printf("LCD Timings: \n");
					break;
				case 0x02:
					printf("Additional timing data type 2: \n");
					ceaversion = data[0x81];
					if(data[0x82]!=0) {
					}
					break;
				case 0x20:
					printf("EDID 2.0 Extension: \n");
					break;
				case 0x30:
					printf("Color information type 0: \n");
					break;
				case 0x40:
					printf("DVI feature data: \n");
					break;
				case 0x50:
					printf("Touch screen data: \n");
					break;
				case 0xF0:
					printf("Block Map: \n");
					break;
			}
		}
		else if(data[0x7e]>1) {
		}
	}
	else {
		version = data[0]>>4;
		reversion = data[0]&0xF;
		printf("\nEDID Version %d Reversion %d.\n",version,reversion);

		cascii = (data[0x01]<<8)+data[0x02];
		printf("Manufacturer Name: %c%c%c\n", ((cascii>>10)&0x1F)+0x40, ((cascii>>5)&0x1F)+0x40, (cascii&0x1F)+0x40);
		printf("Product Code: %04X\n", *((unsigned short *)(data+0x03)));
		printf("Manufacture Year/Week: %02d/%04d\n", *((unsigned short *)(data+0x06)), data[0x05]);

		printf("Manufacturer/product ID: ");
		for(i=0x08;i<0x28;i++) {
			if(data[i]!=0x0a)
				printf("%c",data[i+j]);
			else
				break;
		}
		printf("\n");

		printf("Serial number string: ");
		for(i=0x28;i<0x38;i++) {
			if(data[i]!=0x0a)
				printf("%c",data[i+j]);
			else
				break;
		}
		printf("\n");
	}

	return(true);
}

int s3g_cmd_reg(int argc, char * const argv[])
{
    unsigned char index;
    unsigned char val;
    unsigned char mask;
    unsigned char *mmio = (unsigned char*)S3G_MMIO_BASE;

    //if(!gadapter)
    //{
    //    printf("Need to run \"s3g init\" before execute this command.\n");
    //    return 0;
    //}
	
    if (argc == 2)
    {
        if(!strcmp(argv[0], "AR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, AR, index);
        }
        else if(!strcmp(argv[0], "GR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, GR, index);
        }
        else if(!strcmp(argv[0], "SR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_get_sr(mmio, index);
        }
        else if(!strcmp(argv[0], "CR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_get_cr(mmio, index);
        }
        else if(!strcmp(argv[0], "cr"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR, index);
        }
        else if(!strcmp(argv[0], "crb"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR_B, index);
        }
        else if(!strcmp(argv[0], "crc"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR_C, index);
        }
        else if(!strcmp(argv[0], "crd"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR_D, index);
        }
        else if(!strcmp(argv[0], "crd0"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR_D_0, index);
        }
        else if(!strcmp(argv[0], "crd1"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, CR_D_1, index);
        }
        else if(!strcmp(argv[0], "sr"))
		{
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = s3g_read_reg(mmio, SR, index);
		}
		else if(!strcmp(argv[0], "srb"))
        {
			index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
			val = s3g_read_reg(mmio, SR_BB, index);
		}
		else
			return -1;
        printf("index: 0x%02x, value(GET): 0x%02x\n", index, val);
    }
    else if (argc == 3)
    {
        if(!strcmp(argv[0], "AR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, AR, index, val);
        }
        else if(!strcmp(argv[0], "GR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, GR, index, val);
        }
        else if(!strcmp(argv[0], "SR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_set_sr(mmio, index, val);
        }
        else if(!strcmp(argv[0], "CR"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_set_cr(mmio, index, val);
        }

        else if(!strcmp(argv[0], "cr"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR, index, val);
        }
        else if(!strcmp(argv[0], "crb"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR_B, index, val);
        }
        else if(!strcmp(argv[0], "crc"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR_C, index, val);
        }
        else if(!strcmp(argv[0], "crd"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR_D, index, val);
        }
        else if(!strcmp(argv[0], "crd0"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR_D_0, index, val);
        }
        else if(!strcmp(argv[0], "crd1"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, CR_D_1, index, val);
        }
        else if(!strcmp(argv[0], "sr"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, SR, index, val);
        }
        else if(!strcmp(argv[0], "srb"))
        {
            index = (unsigned char)simple_strtoul(argv[1], NULL, 16);
            val = (unsigned char)simple_strtoul(argv[2], NULL, 16);
            s3g_write_reg_no_mask(mmio, SR_BB, index, val);
        }
		else
	    	return -1;
		printf("index: 0x%02x, value(SET): 0x%02x\n", index, val);
    }
   // else if (argc == 5)
   // {
   //     if(!strcmp(argv[1], "AR"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, AR, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "GR"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, GR, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "CR_A"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, CR, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "CR_B"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, CR_B, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "CR_C"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, CR_C, index, val, mask);
   //     }   
   //     	
   //     else if(!strcmp(argv[1], "CR_D"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, CR_D, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "SR_A"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, SR, index, val, mask);
   //     }
   //     else if(!strcmp(argv[1], "SR_B"))
   //     {
   //         index = (unsigned char)simple_strtoul(argv[2], NULL, 16);
   //         val = (unsigned char)simple_strtoul(argv[3], NULL, 16);
   //         mask = (unsigned char)simple_strtoul(argv[4], NULL, 16);
   //         s3g_write_reg(mmio, SR_B, index, val, mask);
   //     }
   //     else
   //         return -1;
   //     	printf("GROUP: %s, index: 0x%02x, value(SET): 0x%02x, with mask: 0x02x\n", argv[0], index, val, mask);
   // }
    else
        return -1;

    return 0;
}

int s3g_cmd_i2c(int argc, char * const argv[])
{
//done
    char *cmd = argv[0];
    u8 port;
    u8 addr;
    u8 subaddr;
    u8 data;
    u8 i;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }
		
    if (argc == 2)
    {
        if (!strcmp(cmd, "gi2c0") == 0)
        {
            port = 0;
        }
        else if (!strcmp(cmd, "gi2c1") == 0)
        {
            port = 1;
        }
        else if (!strcmp(cmd, "gi2c2") == 0)
        {
            port = 2;
        }
        else if (!strcmp(cmd, "gi2c3") == 0)
        {
            port = 3;
        }
        else
        {
           // port = simple_strtoul(argv[2], NULL, 16);
           //if(port > 3)
                return -1;
        }
        addr = simple_strtoul(argv[1], NULL, 16);
		printf("PORT: %d, ADDR: 0x%x\n", port, addr);
        for(i=0; i< 256; i++)
        {
            s3g_i2c_data_read(gadapter, port, addr, i, &data);
            printf("SUBADDR: 0x%x, DATA: 0x%x \n", i, data);
        }
    }
    else if (argc == 3)
    {
        if (!strcmp(cmd, "gi2c0") == 0)
        {
            port = 0;
        }
        else if (!strcmp(cmd, "gi2c1") == 0)
        {
            port = 1;
        }
        else if (!strcmp(cmd, "gi2c2") == 0)
        {
            port = 2;
        }
        else if (!strcmp(cmd, "gi2c3") == 0)
        {
            port = 3;
        }
        else
        {
           // port = simple_strtoul(argv[2], NULL, 16);
           //if(port > 3)
                return -1;
        }
        addr = simple_strtoul(argv[1], NULL, 16);
        subaddr = simple_strtoul(argv[2], NULL, 16);
        s3g_i2c_data_read(gadapter, port, addr, subaddr, &data);
        printf("PORT: %s, ADDR: 0x%x, SUBADDR: 0x%x, DATA: 0x%x\n", cmd, addr, subaddr, data);
    }
    else if (argc == 4) 
    {
        if (!strcmp(cmd, "gi2c0") == 0)
        {
            port = 0;
        }
        else if (!strcmp(cmd, "gi2c1") == 0)
        {
            port = 1;
        }
        else if (!strcmp(cmd, "gi2c2") == 0)
        {
            port = 2;
        }
        else if (!strcmp(cmd, "gi2c3") == 0)
        {
            port = 3;
        }
        else
        {
           // port = simple_strtoul(argv[2], NULL, 16);
           //if(port > 3)
                return -1;
        }
        addr = simple_strtoul(argv[1], NULL, 16);
        subaddr = simple_strtoul(argv[2], NULL, 16);
        data = simple_strtoul(argv[3], NULL, 16);
        s3g_i2c_data_write(gadapter, port, addr, subaddr, data);
        printf("PORT: %s, ADDR: 0x%x, SUBADDR: 0x%x, DATA: 0x%x\n", cmd, addr, subaddr, data);
    }
    else
        return -1;

    return 0;
}


int s3g_cmd_cc(int argc, char * const argv[])
{
    unsigned int clock;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }
		
    if (argc == 1)
    {
        s3g_get_clocks(gadapter, S3_SELECT_MCLK, &clock);
        printf("MCLK: %u MHz\n", clock);
        s3g_get_clocks(gadapter, S3_SELECT_DCLK1, &clock);
        printf("DCLK1: %u MHz\n", clock);
        s3g_get_clocks(gadapter, S3_SELECT_DCLK2, &clock);
        printf("DCLK2: %u MHz\n", clock);
        s3g_get_clocks(gadapter, S3_SELECT_TVCLK, &clock);
        printf("TVCLK: %u MHz\n", clock);
        s3g_get_clocks(gadapter, S3_SELECT_ECLK, &clock);
        printf("ECLK: %u MHz\n", clock);
        s3g_get_clocks(gadapter, S3_SELECT_ICLK, &clock);
        printf("ICLK: %u MHz\n", clock);
    } 
    else if (argc == 2)
    {
        if(!strcmp(argv[1], "m"))
        {
            s3g_get_clocks(gadapter, S3_SELECT_MCLK, &clock);
            printf("MCLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "e"))
        {
            s3g_get_clocks(gadapter, S3_SELECT_ECLK, &clock);
            printf("ECLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "i"))
        {
            s3g_get_clocks(gadapter, S3_SELECT_ICLK, &clock);
            printf("ICLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "d1"))
        {
            s3g_get_clocks(gadapter, S3_SELECT_DCLK1, &clock);
            printf("DCLK1: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "d2"))
        {
            s3g_get_clocks(gadapter, S3_SELECT_DCLK2, &clock);
            printf("DCLK2: %u MHz\n", clock);
        } else
	    	return -1;
    } else if (argc == 3)
    {
        clock  = simple_strtoul(argv[2], NULL, 10);
        if(!strcmp(argv[1], "m"))
        {
            s3g_set_clocks(gadapter, S3_SELECT_MCLK, clock);
			printf("MCLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "e"))
        {
            s3g_set_clocks(gadapter, S3_SELECT_ECLK, clock);
			printf("ECLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "i"))
        {
            s3g_set_clocks(gadapter, S3_SELECT_ICLK, clock);
			printf("ICLK: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "d1"))
        {
            s3g_set_clocks(gadapter, S3_SELECT_DCLK1, clock);
			printf("DCLK1: %u MHz\n", clock);
        } else if(!strcmp(argv[1], "d2"))
        {
            s3g_set_clocks(gadapter, S3_SELECT_DCLK2, clock);
			printf("DCLK2: %u MHz\n", clock);
        } else
	    	return -1;
    } else
		return -1;

    return 0;
}


int s3g_cmd_mmr(int argc, char * const argv[])
{
//done
    unsigned int reg;
    unsigned int value;
    unsigned int mask;
    unsigned int temp;
    unsigned char *mmio = (unsigned char*)S3G_MMIO_BASE;

    //if(!gadapter)
    //{
    //    printf("Need to run \"s3g init\" before execute this command.\n");
    //    return 0;
    //}
		
    if (argc == 2)
    {
        reg  = simple_strtoul(argv[1], NULL, 16);
        value = s3g_read_ulong(mmio, reg);
        printf("mmio register value 0x%x\n", value);
    } else if (argc == 3)
    {
        reg  = simple_strtoul(argv[1], NULL, 16);
        value = simple_strtoul(argv[2], NULL, 16);
        s3g_write_ulong(mmio, reg, value);
		printf("mmio register: 0x%x, value 0x%x\n", reg, value);
    }
//	 else if (argc == 4)
//    {
//        reg  = simple_strtoul(argv[1], NULL, 16);
//        value = simple_strtoul(argv[2], NULL, 16);
//        mask = simple_strtoul(argv[3], NULL, 16);
//        if((value & mask) != 0)
//        {
//            printf("register mask wrong, value: 0x%x, mask: 0x%x\n", value, mask);
//            return -1;
//        }
//        temp = s3g_read_ulong(gadapter->mmio, reg);
//        temp = (temp & mask) | (value);
//        s3g_write_ulong(gadapter->mmio, reg, temp);
//		printf("mmio register: 0x%x, value: 0x%x, mask: 0x%x\n", reg, value, mask);
//    }
    else
		return -1;

    return 0;
}

int s3g_cmd_sc(int argc, char * const argv[])
{
    unsigned int color;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }

    if (argc == 2)
    {
        color = simple_strtoul(argv[1], NULL, 16);
        if(gadapter->iga_active[0])
            s3g_fill_sc_color(gadapter, 0, color);
        else
            printf("IGA0 not active\n");
        if(gadapter->iga_active[1])
            s3g_fill_sc_color(gadapter, 1, color);
        else
            printf("IGA1 not active\n");
    } else
        return -1;
    return 0;
}


int s3g_cmd_edid(int argc, char * const argv[])
{
    int port;
    unsigned char *pedid;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }
	
    if (argc == 2)
    {	
        port = (int)simple_strtoul(argv[1], NULL, 16);
        pedid = s3g_read_edid(gadapter, port);

        if(!pedid)
            return -1;

        if(!parse_edid(pedid))
        {
            s3g_free(pedid);
            return -1;
        }

        s3g_free(pedid);
    } else
        return -1;

    return 0;
}


int s3g_cmd_dmt(int argc, char * const argv[])
{
    int i;
    unsigned int id;
    s3g_modeset_t mode_set;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }
		
    
    if (argc == 2)
    {
        for(i = 0; i < sizeof(dmtv1r12)/sizeof(dmtv1r12[0]); i++)
            printf("index: %d, width=%d, height=%d, refresh=%d\n",
                i+1, dmtv1r12[i].width, dmtv1r12[i].height, dmtv1r12[i].refresh);
    }		
    else if (argc == 3)
    {
        id = (unsigned int)simple_strtoul(argv[2], NULL, 10);
        if((id > sizeof(dmtv1r12)/sizeof(dmtv1r12[0])) || id == 0)
            return -1;

        memset((void *)&mode_set, 0, sizeof(mode_set));

        mode_set.bpp = 32;
        //mode_set.output = UT_OUTPUT_CRT1;
        mode_set.width = dmtv1r12[id-1].width;
        mode_set.height = dmtv1r12[id-1].height;
        mode_set.refresh = dmtv1r12[id-1].refresh;
        mode_set.dstWidth = mode_set.width;
        mode_set.dstHeight = mode_set.height;
        mode_set.dstRefresh = mode_set.refresh;

			
        mode_set.pipe = 0;
        mode_set.output = gadapter->iga_active[0];
        if(gadapter->iga_active[0])
        {
            s3g_set_mode(gadapter, &mode_set);
            s3g_draw_box(gadapter, mode_set.pipe);
        }
        else
            printf("IGA0 not active\n");

        mode_set.pipe = 1;
        mode_set.output = gadapter->iga_active[1];
        if(gadapter->iga_active[1])
        {
            s3g_set_mode(gadapter, &mode_set);
            s3g_draw_box(gadapter, mode_set.pipe);
        }
        else
            printf("IGA1 not active\n");
			
        
    } else if (argc == 4)
    {
        id = (unsigned int)simple_strtoul(argv[2], NULL, 10);
        if((id > sizeof(dmtv1r12)/sizeof(dmtv1r12[0])) || id == 0)
            return -1;

        memset((void *)&mode_set, 0, sizeof(mode_set));		
	
        mode_set.bpp = 32;
        //mode_set.output = UT_OUTPUT_CRT1;
        mode_set.width = dmtv1r12[id-1].width;
        mode_set.height = dmtv1r12[id-1].height;
        mode_set.refresh = dmtv1r12[id-1].refresh;
        mode_set.dstWidth = mode_set.width;
        mode_set.dstHeight = mode_set.height;
        mode_set.dstRefresh = mode_set.refresh;
		
        mode_set.pipe = 0;
        mode_set.output = gadapter->iga_active[0];
        if(gadapter->iga_active[0])
            s3g_set_mode(gadapter, &mode_set);
        else
            printf("IGA0 not active\n");
        mode_set.pipe = 1;
        mode_set.output = gadapter->iga_active[1];
        if(gadapter->iga_active[1])
            s3g_set_mode(gadapter, &mode_set);
        else
            printf("IGA1 not active\n");
    } else
        return -1;

    return 0;
}

int s3g_cmd_dv(int argc, char * const argv[])
{
    unsigned int device;
	
    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }
	
    if (argc == 1)
    {
        s3g_query_vbeinfo(gadapter);
        s3g_get_connected_output(gadapter);
    } else if (argc == 2)
    {
        device = (int)simple_strtoul(argv[1], NULL, 16);
        s3g_set_output_active(gadapter, device, device);
    } else
        return -1;

    return 0;
}

int s3g_cmd_vba(int argc, char * const argv[])
{
    unsigned char cmd;

    if(!gadapter)
    {
        printf("Need to run \"s3g init\" before execute this command.\n");
        return 0;
    }

    if(argc > 1)
	    cmd = (unsigned char)simple_strtoul(argv[1], NULL, 16);
    else 
	    cmd = 1;
    if(gadapter->iga_active[0])
        s3g_draw_color_bars(gadapter, 0, cmd);
    else
        printf("IGA0 not active\n");
    if(gadapter->iga_active[1])
        s3g_draw_color_bars(gadapter, 1, cmd);
    else
        printf("IGA1 not active\n");

    return 0;
}



int s3g_cmd_init(int argc, char * const argv[])
{
    printf("initializing S3G video driver...\n");
    s3g_init_adapter();
    printf("Done\n");

    return 0;
}
