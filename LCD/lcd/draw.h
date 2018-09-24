#ifndef _DRAW_
#define _DRAW_

int draw_get_params(void);
int draw_dot(unsigned int dotx, unsigned int doty, unsigned int color);
int draw_circle(int x, int y, int r, int color);
int draw_line(int x1,int y1,int x2,int y2,int color);
int draw_char(unsigned int x, unsigned int y, char c, unsigned int color);
int draw_string(unsigned int dotx, unsigned int doty, char *str, unsigned int color);


#endif