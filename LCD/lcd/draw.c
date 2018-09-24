#include "lcd.h"

static unsigned int fb_addr, xres, yres, bpp;
extern const unsigned char fontdata_10x18[];


int draw_get_params(void)
{
	get_lcd_params(&fb_addr, &xres, &yres, &bpp);
	return 0;
}


unsigned short convert_32_to_16rgb(unsigned int color)
{
	unsigned short rgb = 0;
	rgb = ((color >> 19) << 11) | (((color & (0x00ff00)) >> 10) << 5) | ((color & (0xff) >> 2) << 0);
	return rgb;
}


/* 画点函数 */
int draw_dot(unsigned int dotx, unsigned int doty, unsigned int color)
{
	unsigned char *dot_8_addr;
	unsigned short *dot_16_addr;
	unsigned int *dot_32_addr;
	unsigned int dot_addr;

	dot_addr = (fb_addr + (xres * bpp/8) * doty + dotx * bpp/8);
	if (bpp == 16)
	{
		dot_16_addr = (unsigned short *)dot_addr;
		*dot_16_addr = convert_32_to_16rgb(color);
	}
}


int draw_circle(int x, int y, int r, int color)  
 {	
	 int a, b, num;  
	 a = 0;  
	 b = r;  
	 while(22 * b * b >= r * r) 		 // 1/8圆即可  
	 {	
		 draw_dot(x + a, y - b,color); // 0~1  
		 draw_dot(x - a, y - b,color); // 0~7  
		 draw_dot(x - a, y + b,color); // 4~5  
		 draw_dot(x + a, y + b,color); // 4~3  
   
		 draw_dot(x + b, y + a,color); // 2~3  
		 draw_dot(x + b, y - a,color); // 2~1  
		 draw_dot(x - b, y - a,color); // 6~7  
		 draw_dot(x - b, y + a,color); // 6~5  
		   
		 a++;  
		 num = (a * a + b * b) - r*r;  
		 if(num > 0)  
		 {	
			 b--;  
			 a--;  
		 }	
	 }
	 return 0;	
 }	
   
 //-----------画线。参数：起始坐标，终点坐标，颜色--------	
 int draw_line(int x1,int y1,int x2,int y2,int color)	
 {	
	 int dx,dy,e;  
	 dx=x2-x1;	 
	 dy=y2-y1;	
	 if(dx>=0)	
	 {	
		 if(dy >= 0) // dy>=0  
		 {	
			 if(dx>=dy) // 1/8 octant  
			 {	
				 e=dy-dx/2;  
				 while(x1<=x2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){y1+=1;e-=dx;}	   
					 x1+=1;  
					 e+=dy;  
				 }	
			 }	
			 else		 // 2/8 octant	
			 {	
				 e=dx-dy/2;  
				 while(y1<=y2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){x1+=1;e-=dy;}	   
					 y1+=1;  
					 e+=dx;  
				 }	
			 }	
		 }	
		 else			// dy<0  
		 {	
			 dy=-dy;   // dy=abs(dy)  
			 if(dx>=dy) // 8/8 octant  
			 {	
				 e=dy-dx/2;  
				 while(x1<=x2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){y1-=1;e-=dx;}	   
					 x1+=1;  
					 e+=dy;  
				 }	
			 }	
			 else		 // 7/8 octant	
			 {	
				 e=dx-dy/2;  
				 while(y1>=y2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){x1+=1;e-=dy;}	   
					 y1-=1;  
					 e+=dx;  
				 }	
			 }	
		 }	   
	 }	
	 else //dx<0  
	 {	
		 dx=-dx;	 //dx=abs(dx)  
		 if(dy >= 0) // dy>=0  
		 {	
			 if(dx>=dy) // 4/8 octant  
			 {	
				 e=dy-dx/2;  
				 while(x1>=x2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){y1+=1;e-=dx;}	   
					 x1-=1;  
					 e+=dy;  
				 }	
			 }	
			 else		 // 3/8 octant	
			 {	
				 e=dx-dy/2;  
				 while(y1<=y2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){x1-=1;e-=dy;}	   
					 y1+=1;  
					 e+=dx;  
				 }	
			 }	
		 }	
		 else			// dy<0  
		 {	
			 dy=-dy;   // dy=abs(dy)  
			 if(dx>=dy) // 5/8 octant  
			 {	
				 e=dy-dx/2;  
				 while(x1>=x2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){y1-=1;e-=dx;}	   
					 x1-=1;  
					 e+=dy;  
				 }	
			 }	
			 else		 // 6/8 octant	
			 {	
				 e=dx-dy/2;  
				 while(y1>=y2)	
				 {	
					 draw_dot(x1,y1,color);  
					 if(e>0){x1-=1;e-=dy;}	   
					 y1-=1;  
					 e+=dx;  
				 }	
			 }	
		 }	   
	 }
	return 0;	
 }	

/* 写字 */
int draw_char(unsigned int dotx, unsigned int doty, char c, unsigned int color)
{
	int i, j;
	unsigned char *dot = &fontdata_10x18[c * 36];
	unsigned short data;

	for (j=doty; j<doty+18; j++)
	{
		data = *dot++;
		data <<= 2;
		data |= *dot++ >> 6;
		for (i=dotx; i<dotx+10; i++)
		{
			if (data & (1<<dotx+9-i))
			{
				draw_dot(i, j, color);
			}
		}
	}
	return 0;
}


int draw_string(unsigned int dotx, unsigned int doty, char *str, unsigned int color)
{
	int i = 0;

	while(str[i])
	{
		if (str[i] == '\n')
		{
			doty += 18;
		}
		else if (str[i] == '\r')
		{
			dotx = 0;
		}
		else
		{	
			if (dotx + 10 > xres)
			{
				doty += 18;
				dotx = 0;
			}
			draw_char(dotx, doty, str[i], color);
			dotx += 10;
			if (dotx > xres)
			{
				doty += 18;
				dotx = 0;
			}
		}
		i++;
	}
}