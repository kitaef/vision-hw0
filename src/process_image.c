#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"


float get_pixel(image im, int x, int y, int c)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > im.w - 1) x = im.w - 1;
    if (y > im.h - 1) y = im.h - 1;
    float pixel = im.data[im.w * im.h * c + im.w * y + x];
    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > im.w - 1) x = im.w - 1;
    if (y > im.h - 1) y = im.h - 1;
    im.data[im.w * im.h * c + im.w * y + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for (int i = 0; i < im.w * im.h * im.c; i++)
        {
            copy.data[i] = im.data[i];
        }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
		for (int c = 0; c < im.c; c++)
			{
				for (int pixel = 0; pixel < im.w * im.h; pixel++)
					{
						if (c==0) gray.data[pixel]  = 0.299 * im.data[pixel];
						if (c==1) gray.data[pixel] += 0.587 * im.data[pixel + im.w * im.h];
						if (c==2) gray.data[pixel] += 0.114 * im.data[pixel + 2*(im.w * im.h)];	
					}
			}
    return gray;
}

void shift_image(image im, int c, float v, int change_hue)
{
	for (int i = c * im.w * im.h;  i < (c+1) * im.h * im.w; i++)
        {
            im.data[i] += v;
            if (change_hue)
                {
                    float H = im.data[i] * 6;
                    while (H < 0 || H >= 6)
                        {
                            if (H<0) H += 6;
                            else     H -= 6;
                        }
                    if (H >= 0 && H < 0.4 || H >= 5.6)
                        H +=2;
                    H /= 6;
                    while (H < 0 || H >= 1)
                        {
                            if (H<0)
                                H += 1;
                            else
                                H -= 1;
                        }
                    im.data[i] = H;
                }
        }
}

void scale_image(image im, int c, float v)
{
    for (int i = c*im.w*im.h;  i < (c+1)*im.h * im.w; i++)
    {
        im.data[i] *= v;				
	}
		
}

void clamp_image(image im)
{
    for (int i = 0; i< im.c*im.w*im.h; i++)
			{
                if (im.data[i] < 0) im.data[i] = 0;
				if (im.data[i] > 1) im.data[i] = 1;
			}
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int y=0; y<im.h; y++)
        {
            for (int x=0; x<im.w; x++)
                {
                    float r, g, b, v, m, C, H, S, V;
                    r = get_pixel(im, x, y, 0);
                    g = get_pixel(im, x, y, 1);
                    b = get_pixel(im, x, y, 2);
                    V = three_way_max(r, g, b);
                    m = three_way_min(r, g, b);
                    C = V - m;
                    if (V == 0)
                        S = 0;
                    else 
                        S = C / V;
                    if (C == 0) H = 0;
                    else if (V == r) H = fmod((g - b)/C, 6);
                    else if (V == g) H = ((b - r)/C + 2);
                    else if (V == b) H = ((r - g)/C + 4);
                    H /= 6;
                    while (H < 0)
                        {
                            H += 1;
                        }
                    set_pixel(im, x, y, 0, H);
                    set_pixel(im, x, y, 1, S);
                    set_pixel(im, x, y, 2, V);
                }
        }
}

void hsv_to_rgb(image im)
{
    for (int y=0; y<im.h; y++)
        {
            for (int x=0; x<im.w; x++)
                {
                    float H, S, V, r, g, b, C, X, m;
                    H = get_pixel(im, x, y, 0) * 6;
                    S = get_pixel(im, x, y, 1);
                    V = get_pixel(im, x, y, 2);
                                    C = V * S;
                                    X = C * (1 - fabs(fmod(H, 2) - 1));
                                    if      (0 <= H && H < 1) {r = C; g = X; b = 0;}
                                    else if (1 <= H && H < 2) {r = X; g = C; b = 0;}
                                    else if (2 <= H && H < 3) {r = 0; g = C; b = X;}
                                    else if (3 <= H && H < 4) {r = 0; g = X; b = C;}
                                    else if (4 <= H && H < 5) {r = X; g = 0; b = C;}
                                    else if (5 <= H && H < 6) {r = C; g = 0; b = X;}
                                    m = V - C;
                                    r += m; g += m; b += m;
                                    set_pixel(im, x, y, 0, r);
                    set_pixel(im, x, y, 1, g);
                    set_pixel(im, x, y, 2, b);
                }
        }
}
