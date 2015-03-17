/*
 * (C) Copyright 2005
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

#include <common.h>
#include <command.h>
#include <led-display.h>
#include "../drivers/video/fb_lib.h" //elite1k-320118d-YSW-08

//elite1k-320118d-YSW-08 #undef DEBUG_DISP
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08 int do_display (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
//elite1k-320118d-YSW-08 {
//elite1k-320118d-YSW-08	int i;
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08	/* Clear display */
//elite1k-320118d-YSW-08	display_set(DISPLAY_CLEAR | DISPLAY_HOME);
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08	if (argc < 2)
//elite1k-320118d-YSW-08		return (0);
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08	for (i = 1; i < argc; i++) {
//elite1k-320118d-YSW-08		char *p = argv[i];
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08		if (i > 1) { /* Insert a space between strings */
//elite1k-320118d-YSW-08			display_putc(' ');
//elite1k-320118d-YSW-08		}
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08		while ((*p)) {
//elite1k-320118d-YSW-08 #ifdef DEBUG_DISP
//elite1k-320118d-YSW-08			putc(*p);
//elite1k-320118d-YSW-08 #endif
//elite1k-320118d-YSW-08			display_putc(*p++);
//elite1k-320118d-YSW-08		}
//elite1k-320118d-YSW-08	}
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08 #ifdef DEBUG_DISP
//elite1k-320118d-YSW-08 	putc('\n');
//elite1k-320118d-YSW-08 #endif
//elite1k-320118d-YSW-08
//elite1k-320118d-YSW-08	return (0);
//elite1k-320118d-YSW-08 }

//elite1k-320118d-YSW-08 start
extern adapter_t *gadapter;
extern int mv_stdout;
extern unsigned int cursor_row;
extern unsigned int cursor_col;
extern void s3g_init_adapter(void);

int do_display (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	switch (argc) {
		case 0:
		case 1:	
			printf ("Usage:\n%s\n", cmdtp->usage);
		return 0;
		default:
			if (strncmp(argv[1], "init", 4) == 0) {
				s3g_init_adapter();
			        s3g_set_mode16_to_iga(gadapter, 0);
			        s3g_set_mode16_to_iga(gadapter, 1);
				video_init();
				cursor_row = 0;
				cursor_col = 0;
				s3g_clear_screen(gadapter,0, 0);
				return 0;
			}

			if (strncmp(argv[1], "on", 2) == 0) {
				mv_stdout = 1;
				return 0;
			}
			if (strncmp(argv[1], "off", 2) == 0) {
				mv_stdout = 0;
				return 0;
			}
			if (strncmp(argv[1], "clear", 2) == 0) {
				cursor_row = 0;
				cursor_col = 0;
				s3g_clear_screen(gadapter,0, 0);
				return 0;
			}
			if (strncmp(argv[1], "goto", 2) == 0) {
                            if( argc > 3 )
                            {
				cursor_row = simple_strtoul(argv[3], NULL, 10);
				cursor_col = simple_strtoul(argv[2], NULL, 10);
                            }
				return 0;
			}


	}
	return 0;

}
//elite1k-320118d-YSW-08 end
/***************************************************/

//elite1k-320118d-YSW-08 U_BOOT_CMD(
//elite1k-320118d-YSW-08 	display,	CONFIG_SYS_MAXARGS,	1,	do_display,
//elite1k-320118d-YSW-08 	"display string on dot matrix display",
//elite1k-320118d-YSW-08 	"[<string>]\n"
//elite1k-320118d-YSW-08 	"    - with <string> argument: display <string> on dot matrix display\n"
//elite1k-320118d-YSW-08 	"    - without arguments: clear dot matrix display"
//elite1k-320118d-YSW-08 );

//elite1k-320118d-YSW-08 - start
U_BOOT_CMD(
	display,	CONFIG_SYS_MAXARGS,	1,	do_display,
	"display string on dot matrix display",
	"init | off | on\n"
	"clear - clear screen\n"
	"goto  col row\n"
);
//elite1k-320118d-YSW-08 - end
