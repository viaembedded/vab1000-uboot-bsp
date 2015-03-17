
#include "fb_lib.h"
#include "s3g_display.h"
#include <common.h>


int get_position(adapter_t *adapter, int pipe, int a, int b)
{
    int pos = 0;
    surface_t *surface = &adapter->surface[pipe];
	
    switch(s3g_getrotation(adapter, pipe))
    {
        case FB_ROTATE_NONE:
            pos = a*surface->bit_count/8 + (b*surface->pitch);
            break;
        case FB_ROTATE_90:
            pos = 0;
            break;
        case FB_ROTATE_180:
            pos = 0;
            break;
        case FB_ROTATE_270:
            pos = 0;
            break;
        default:

            return PARAM_ERR;
       }

    return pos;
}

void set_pixel(adapter_t *adapter, int pipe, char *pos, s3g_pixel col)
{
    unsigned char *abs_pos;

    surface_t *surface = &adapter->surface[pipe];
	
    abs_pos = surface->base + (long int)pos;

    if(s3g_getbpp(adapter, pipe) == 16)
    {
        *abs_pos = (u_char)col;

        *(abs_pos + 1) = (u_char)(col>>8);

    }else if (s3g_getbpp(adapter, pipe) == 32)
    {
        *abs_pos = (u_char)col;
        *(abs_pos + 1) = (col>>8);
        *(abs_pos + 2) = (u_char)(col>>16);
        *(abs_pos + 3) = (u_char)(col>>24);
    }

    return;
}

void s3g_putpixel(adapter_t *adapter, int pipe, int x, int y, s3g_pixel color)
{
    char *pos;
	
    if(x < 0 || x > s3g_getxres(adapter, pipe))
        return;
	
    if(y < 0 || y > s3g_getyres(adapter, pipe))
        return;
	
    pos = (char*)(get_position(adapter, pipe, x, y));
    
    set_pixel(adapter, pipe, pos, color);
}

s3g_pixel s3g_getpixel(adapter_t *adapter, int pipe, int x, int y)
{
    long int location;
	
    s3g_pixel tmp;

    surface_t *surface = &adapter->surface[pipe];
	
    location=x * (s3g_getbpp(adapter, pipe)/8) + y * (surface->pitch);

    tmp = *(surface->base + location) | (*(surface->base + location + 1)<<8);

    if(s3g_getbpp(adapter, pipe) == 32)
        tmp |= (*(surface->base + location + 2)<<16) + (*(surface->base + location + 3)<<24);

    return tmp;
}


void s3g_vline(adapter_t *adapter, int pipe, int x, int sy, int ey, s3g_pixel color)
{
    int i;

    /* Parameter sanity checks */
    if(x < 0 || x > s3g_getxres(adapter, pipe))
        return;
	
    if(sy > ey)
    {
        i=sy;
        sy=ey;
        ey=i;
    }

    if(sy < 0)
        sy = 0;

    if(ey > s3g_getyres(adapter, pipe))
        ey = s3g_getyres(adapter, pipe);
	
    /* end of checks */

    for(i = sy; i <= ey; i++)
    {
        s3g_putpixel(adapter, pipe, x, i, color);
    }
}


void s3g_hline(adapter_t *adapter, int pipe, int sx, int ex, int y, s3g_pixel color)
{
    int i;
	
    if(y < 0 || y > s3g_getyres(adapter, pipe))
        return;
	
    if(sx > ex)
    {
        i=sx;
        sx=ex;
        ex=i;
    }

    if(sx < 0)
        sx = 0;
    
    if(ex > s3g_getxres(adapter, pipe))
        ex = s3g_getxres(adapter, pipe);
	
    for(i = sx; i <= ex; i++)
    {
        s3g_putpixel(adapter, pipe, i,y,color);
    }
}


/* This is Bresenham's line algorithm */
void s3g_line(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color)
{
    int dx,dy;
    int x,y,incx,incy;
    int balance;
	
    if(sx == ex)
        s3g_vline(adapter, pipe, sx,sy,ey,color);
	
    if(sy == ey)
        s3g_hline(adapter, pipe, sx,ex,sy,color);
	
    if (ex >= sx)
    {
        dx = ex - sx;
        incx = 1;
    }else
    {
        dx = sx - ex;
        incx = -1;
    }

    if (ey >= sy)
    {
        dy = ey - sy;
        incy = 1;
    }else
    {
        dy = sy - ey;
        incy = -1;
    }

    x = sx;
    y = sy;

    if (dx >= dy)
    {
        dy <<= 1;
        balance = dy - dx;
        dx <<= 1;

        while (x != ex)
        {
            s3g_putpixel(adapter, pipe, x, y, color);
            if (balance >= 0)
            {
                y += incy;
                balance -= dx;
            }
            balance += dy;
            x += incx;
        } 
        s3g_putpixel(adapter, pipe, x, y, color);
    }
    else
    {
        dx <<= 1;
        balance = dx - dy;
        dy <<= 1;

        while (y != ey)
        {
            s3g_putpixel(adapter, pipe, x, y, color);
            if (balance >= 0)
            {
                x += incx;
                balance -= dy;
            }
            balance += dx;
            y += incy;
        } 
        s3g_putpixel(adapter, pipe, x, y, color);
    }
}


/* Draws a rectangle (only borders) */
void s3g_rect(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color)
{
    s3g_hline(adapter, pipe, sx,ex,sy,color);
    s3g_hline(adapter, pipe, sx,ex,ey,color);
    s3g_vline(adapter, pipe, sx,sy,ey,color);
    s3g_vline(adapter, pipe, ex,sy,ey,color);
}

/* Draws a solid rectangle filled with color */
void s3g_rectfill(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color)
{
    int i,j;
	
    if(sx > ex)
    {
        i=sx;
        sx=ex;
        ex=i;
    }
	
    if(sy > ey)
    {
        i=sy;
        sy=ey;
        ey=i;
    }
	
    if(sx < 0)
        sx = 0;
	
    if(sy < 0)
        sy = 0;
	
    if(ex > s3g_getxres(adapter, pipe))
        ex = s3g_getxres(adapter, pipe);
	
    if(ey > s3g_getyres(adapter, pipe))
        ey = s3g_getyres(adapter, pipe);
	
    for(i = sy; i <= ey; i++)
    {
        for(j = sx; j <= ex; j++ )
        {
            s3g_putpixel(adapter, pipe, j, i, color);
        }
    }
}


/* Draws a solid rectangle filled with gradiant color */
void s3g_rectfill_omber(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color)
{
    int i,j;
    u_char col, r, g, b, t=0;
    s3g_pixel newcol;
    surface_t *surface = &adapter->surface[pipe];

    if(surface->bit_count == 16) {
        b = (u_char)color;
        g = (u_char)(color>>5);
        r = (u_char)(color>>11);
    }else if(surface->bit_count == 32)
    {
        b = (u_char)color;
        g = (u_char)(color>>8);
        r = (u_char)(color>>16);
        t = (u_char)(color>>24);
    }else
	    return;
	
    if(sx > ex)
    {
        i=sx;
        sx=ex;
        ex=i;
    }

    if(sy > ey)
    {
        i=sy;
        sy=ey;
        ey=i;
    }

    if(sx < 0)
        sx = 0;

    if(sy < 0)
        sy = 0;

    if(ex > s3g_getxres(adapter, pipe))
        ex = s3g_getxres(adapter, pipe);
	
    if(ey > s3g_getyres(adapter, pipe))
        ey = s3g_getyres(adapter, pipe);
	
    for(i = sy; i <= ey; i++)
    {
        col = 0;

        for(j = sx; j <= ex; j++ )
        {  
            newcol = s3g_makecol(adapter, pipe, r*(j-sx)/(ex-sx), g*(j-sx)/(ex-sx), b*(j-sx)/(ex-sx), t);
            s3g_putpixel(adapter, pipe, j, i, newcol);
        }
    }
}

/* Draws a triangle (only borders) */
void s3g_triangle(adapter_t *adapter, int pipe, int x1,int y1,int x2,int y2,int x3,int y3,s3g_pixel col)
{
    s3g_line(adapter, pipe, x1, y1, x2, y2, col);
    s3g_line(adapter, pipe, x3, y3, x2, y2, col);
    s3g_line(adapter, pipe, x1, y1, x3, y3, col);
}


/* clear screen with color */
void s3g_clear_screen(adapter_t *adapter, int pipe, s3g_pixel color)
{
    unsigned int i, size;
    unsigned short *abs_pos1;
    unsigned int *abs_pos2;

    surface_t *surface = &adapter->surface[pipe];

    size = get_screensize(adapter, pipe);

    
    if(surface->bit_count == 16)
    {
        abs_pos1 = surface->base;
        for(i=0; i<size/2; i++)
            *abs_pos1++ = (unsigned short)color;
    }
    else if(surface->bit_count == 32)
    {
        abs_pos2 = surface->base;
        for(i=0; i<size/4; i++)
            *abs_pos2++ = color;
    }
}
