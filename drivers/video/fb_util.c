#include "fb_lib.h"
#include "s3g_display.h"

s3g_pixel s3g_makecol(adapter_t *adapter, int pipe, u_char r, u_char g, u_char b, u_char t)
{
    s3g_pixel color = 0;
    surface_t *surface = &adapter->surface[pipe];

    if(surface->bit_count == 16)
        color =  r<<11|g<<5|b;
    else if(surface->bit_count == 32)
        color = t<<24|r<<16|g<<8|b;
	
    return color;
}

long int get_screensize(adapter_t *adapter, int pipe)
{
    surface_t *surface = &adapter->surface[pipe];
	
    return surface->pitch*surface->h;
}


void s3g_getres(adapter_t *adapter, int pipe, int *x, int *y)
{
    surface_t *surface = &adapter->surface[pipe];
	
    if(x!=NULL && y!=NULL)
    {
        *x = surface->w;
	*y = surface->h;
    }
}

int s3g_getxres(adapter_t *adapter, int pipe)
{
    surface_t *surface = &adapter->surface[pipe];

    return surface->pitch*8/surface->bit_count;
}

int s3g_getyres(adapter_t *adapter, int pipe)
{
    surface_t *surface = &adapter->surface[pipe];

    return surface->h;
}


int s3g_getbpp(adapter_t *adapter, int pipe)
{
    surface_t *surface = &adapter->surface[pipe];

    return surface->bit_count;
}


int s3g_setrotation(adapter_t *adapter, int pipe, int rotation)
{
    return 0;
}

int s3g_getrotation(adapter_t *adapter, int pipe)
{
    return 0;
}

