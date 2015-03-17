#ifndef FB_LIB_H
#define FB_LIB_H


#include <linux/types.h>
#include "adapter.h"


/* internal */
#define DEBUG

/* Error handling */
#ifndef OK
#define OK         0
#endif
#define PARAM_ERR -1


/* Rotations */
enum rotations
{
	FB_ROTATE_NONE,
	FB_ROTATE_90,
	FB_ROTATE_180,
	FB_ROTATE_270,
};

#ifndef BYTE
typedef unsigned char       BYTE;
#endif
typedef unsigned int s3g_pixel;

typedef struct {
	int pipe;
	int sx;
	int sy;
	int ex;
	int ey;
	s3g_pixel color;
} s3g_draw_t;

typedef struct {
	int fb_rotation;
	char *fb_visp;
	long int screensize;
	long int visiblesize;
	unsigned int line_length;
	int inc_x, inc_y;
}s3g_draw_info_t;



/*
 * Private functions
 */

int get_position(adapter_t *adapter, int pipe, int a, int b);
void set_pixel(adapter_t *adapter, int pipe, char *pos, s3g_pixel col);

/*
 * Exported interface 
*/

extern int s3g_initlib(adapter_t *adapter);  


extern long int get_screensize(adapter_t *adapter, int pipe);


extern s3g_pixel s3g_makecol(adapter_t *adapter, int pipe, u_char r,u_char g, u_char b, u_char t);


extern void s3g_clear_screen(adapter_t *adapter, int pipe, s3g_pixel color);


extern void s3g_putpixel(adapter_t *adapter, int pipe, int x, int y, s3g_pixel color);


extern s3g_pixel s3g_getpixel(adapter_t *adapter, int pipe, int x, int y);


extern void s3g_line(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color);


extern void s3g_vline(adapter_t *adapter, int pipe, int x, int sy, int ey, s3g_pixel color);


extern void s3g_hline(adapter_t *adapter, int pipe, int sx, int ex, int y, s3g_pixel color);


extern void s3g_rect(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color);


extern void s3g_rectfill(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color);


extern void s3g_rectfill_omber(adapter_t *adapter, int pipe, int sx, int sy, int ex, int ey, s3g_pixel color);


extern void s3g_triangle(adapter_t *adapter, int pipe, int x1,int y1,int x2,int y2,int x3,int y3,s3g_pixel col);


extern void s3g_getres(adapter_t *adapter, int pipe, int *x, int *y);


extern int s3g_getxres(adapter_t *adapter, int pipe);


extern int s3g_getyres(adapter_t *adapter, int pipe);


extern int s3g_getbpp(adapter_t *adapter, int pipe);


extern int s3g_setrotation(adapter_t *adapter, int pipe, int rotation);


extern int s3g_getrotation(adapter_t *adapter, int pipe);

#endif /* FBLIB_H */

