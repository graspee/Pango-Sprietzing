#include "stdafx.h"
#include <iostream>
#include <array>
#include <algorithm>

std::array<float,3> RGB_to_HSB(int red,int green,int blue){
     float r = red / 255.0f;
     float g = green / 255.0f;
     float b = blue / 255.0f;
     float max = std::max(std::max(r, g), b);
     float min = std::min(std::min(r, g), b);
     float delta = max - min;
     float hue = 0;
     float brightness = max;
     float saturation = max == 0 ? 0 : (max - min) / max;
     if (delta != 0) {
         if (r == max) {
             hue = (g - b) / delta;
         } else {
             if (g == max) {
                 hue = 2 + (b - r) / delta; 
             } else {
                 hue = 4 + (r - g) / delta;
             }
         }
         hue *= 60;
         if (hue < 0) hue += 360;
     }
	std::array<float,3> retval={hue, saturation, brightness};
	return retval;
} 
 



std::array<int,3> HSB_to_RGB(float hue, float saturation, float brightness) {
     if (hue < 0 || hue > 360 || saturation < 0 || saturation > 1 || 
         brightness < 0 || brightness > 1) {
       std::cerr << "HSB_to_RGB: error with inputs" << std::endl;
	   exit(0);
     }
     float r, g, b;
     if (saturation == 0) {
         r = g = b = brightness; 
     } else {
         if (hue == 360) hue = 0;
         hue /= 60; 
         int i = (int)hue;
         float f = hue - i;
         float p = brightness * (1 - saturation);
         float q = brightness * (1 - saturation * f);
         float t = brightness * (1 - saturation * (1 - f));
         switch(i) {
             case 0:
                 r = brightness;
                 g = t;
                 b = p;
                 break;
             case 1:
                 r = q;
                 g = brightness;
                 b = p;
                 break;
             case 2:
                 r = p;
                 g = brightness;
                 b = t;
                 break;
             case 3:
                 r = p;
                 g = q;
                 b = brightness;
                 break;
             case 4:
                 r = t;
                 g = p;
                 b = brightness;
                 break;
             case 5:
             default:
                 r = brightness;
                 g = p;
                 b = q;
                 break;
         }
     }
     int red = (int)(r * 255 + 0.5);
     int green = (int)(g * 255 + 0.5);
     int blue = (int)(b * 255 + 0.5); 

	 std::array<int,3> retval={red,green,blue};
	 return retval;

}

/*
void DrawColourGrid(void){
const float XW=80.0;
	const float YH=45.0;
	for (float y=0; y<YH; y++) {
		float s = y < (YH/2) ? 1       : (YH- y) / (YH/2);
		float b = y < (YH/2) ? y / (YH/2) : 1;
		for (float x=0; x<XW; x++) {
			float h = x / XW;
			std::array<int,3> rgb=HSB_to_RGB(360*h,s,b);
			Uint32 col=pe->MapRGB(rgb[0],rgb[1],rgb[2]);        
			pe->desktop->DrawChar(' ',0,col,x,44-y);
		}
	}
}*/