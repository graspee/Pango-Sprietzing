// PangoSprietzing.cpp : Defines the entry point for the console application.
//26th March 2013




#include "stdafx.h"
#include "PangoTUIuser.h"
#include <array> 
#include <string>
#include <iomanip>
#include <sstream>
//#include <commdlg.h>
//#include <winnt.h>
#include <Windows.h>

using namespace PangoTUI;

//global vars
#if 1 

OPENFILENAME ofn;
HANDLE hf;  
wchar_t szFile[260]; 

std::string SpriteNames[40];

PangoEngine* pe;
Area* SprietGrid;
int lastcursorx=-1;
int lastcursory=-1;

//Pen and palette
Uint32 CurrentLeftPen;
int CurrentPenIndex=0;
Uint32 PaletteColours[256];
bool PaletteVirgin[256];
Area* PaletteGrid;

Slider* redslider,*greenslider,*blueslider;
Slider* redslider2,*greenslider2,*blueslider2;
Slider* hslider,*hslider2,*sslider,*sslider2,*bslider,*bslider2;

CustomWidget* spritebar;

Area* PenColor;
Button* ModeButtons[7];
Uint8 CurrentMode=0;

Area* RGBlabel,*HSBlabel;

Uint8 penred=255,pengreen=0,penblue=255;
IconButton* ScrollUp, *ScrollDown, *ScrollLeft, *ScrollRight;

SingleLineTextEntry* SpriteName,*ColourValueTextBox;

bool Virgin=true;

//TAG SPRITES STUFF
//int sprsys_numsprites; 
std::array<Sprite16x16*,40> sprsys_sprites;
int sprsys_currentsprite_n;
Sprite16x16* sprsys_currentsprite_p;

std::array<int,3> HSB_to_RGB(float hue, float saturation, float brightness);
std::array<float,3> RGB_to_HSB(int red,int green,int blue);
void ClearCurrentSpriet(void);
void DrawColourGrid(void);
void UpdatePen(void);
void UpdatePen_hsb(void);
void PaletteShowSelected(void);
void UpdateSliders(Uint32 n);
 void BresLine(int a,int b,int c,int d,bool isleftbuttonp,bool erasep,bool finalp=false);
void DrawRect(int x,int y,int x0,int y0,bool isframep,bool isleftbuttonp, bool erasep,bool finalp=false);
void DrawSpriteInGrid(Sprite16x16* x);
void DrawCircle(int x0, int y0, int radius,  bool isleftbuttonp,bool erasep,bool finalp=false);
void DrawFilledCircle(int x0, int y0, int radius,  bool isleftbuttonp,bool erasep,bool finalp=false);
inline void Draw2x2pixa_pen(Uint8 x,Uint8 y);
inline void Draw2x2pixa_pen_temp(Uint8 x,Uint8 y);
inline void Draw2x2pixa_givencolour(Uint8 x,Uint8 y,Uint32 c);
inline void Draw2x2pixa_erase_temp(Uint8 x,Uint8 y);

inline void Draw2x2pixa_alpha(Uint8 x,Uint8 y);
inline void Draw2x2pixa_alpha_temp(Uint8 x,Uint8 y);
inline void RedrawPixel(Uint8 x,Uint8 y);
inline int GetRadius(int x0,int y0,int x1,int y1);

inline void DrawSpriteBarNumber_normal(int x);
inline void DrawSpriteBarNumber_selected(int x);

inline void DrawColourBlocks(bool dotexttoo=true);

//	inline int rndz(int low,int high){
	//	return (std::rand()%((high-low)+1))+low;
  //}

void SetupLambdas(void);
std::function<void (int x,int y)> Lambda2x2PixelFn[6];

Uint8 WhichLambdap(bool isleftbuttonp, bool erasep, bool finalp);

bool FloodFill(int x, int y, Uint32 ColourToMatch);
bool FloodFillAlpha(int x,int y);

void ScrollGrid(int dx,int dy);
void UpdateHSBSliders(Uint8 r,Uint8 g,Uint8 b);
void UpdateColourText(Uint8 r,Uint8 g,Uint8 b);
void InitUI(void);
void InitUIEvents(void);
void UpdateSmallView(void);
//void RedrawExtraBits(void);

Button* SaveButton,*LoadButton,*CLSButton;
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	pe=new PangoEngine(1280,720,2,2,false); //this is the real one
	//pe=new PangoEngine(640,360,1,1,false);
	//Font* f=new Font((unsigned char*)Font::Hobbit,6,8);//in real one this is remmed

	//Font* f=new Font((unsigned char*)Font::Halfwidth,4,8);
	
	//pe=new PangoEngine(1280,720,f,1,1,false);//remmed in real one
	pe->SetAppTitle("Pango Spriet Editzor");
	//pe=new PangoEngine(640,360,1,1,false);
	//pe->DrawString_halfwidth("Available now on Desura and Steam",5,5,pe->colour_window_border,pe->colour_bg);	
	/*
	pe->desktop->DrawString("You are in a comfortable tunnel like hall",pe->colour_window_border,pe->colour_bg,0,0);
	pe->desktop->DrawString("To the east there is the round green door",pe->colour_window_border,pe->colour_bg,0,1);
	pe->desktop->DrawString("You see :",pe->colour_window_border,pe->colour_bg,0,2);
	pe->desktop->DrawString("    the wooden chest.",pe->colour_window_border,pe->colour_bg,0,3);
	pe->desktop->DrawString("    Gandalf. Gandalf is carrying",pe->colour_window_border,pe->colour_bg,0,4);
	pe->desktop->DrawString("      a curious map.",pe->colour_window_border,pe->colour_bg,0,5);
	pe->desktop->DrawString("    Thorin.",pe->colour_window_border,pe->colour_bg,0,6);
	pe->desktop->DrawString("Gandalf gives the curious map to you.",pe->colour_window_border,pe->colour_bg,0,7);
	*/
	
	//char* cstring=new char[256];

	//for(unsigned char c=32;c<128;c++)cstring[c-32]=c;
	//cstring[128-32]=0;
	//std::string s=std::string(cstring);

	//pe->desktop->DrawString(s,pe->colour_window_border,pe->colour_bg,0,0);

	//pe->WaitForMausClik();//in real one- this line is remmed


	/*
	const int numpoints=10;
	int voronoi_point_x[numpoints];
	int voronoi_point_y[numpoints];
	for(int n=0;n<numpoints;n++){
		voronoi_point_x[n]=rndz(0,640);
		voronoi_point_y[n]=rndz(0,360);
	}

	for (int y = 0; y < 360; y++)
	{
		for (int x = 0; x < 640; x++)
		{
			float shortest=-1; int which=-1;

			for (int n = 0; n < numpoints; n++)
			{
				float dx = voronoi_point_x[n] - x;
				float dy = voronoi_point_y[n] - y;
				
				float dx0 = voronoi_point_x[0] - x;
				float dy0 = voronoi_point_y[0] - y;
				float dist0=  sqrt( (dx0*dx0) + (dy0*dy0) );
				if(dx<shortest || dy<shortest || n==0){
					float dist = sqrt( (dx*dx) + (dy*dy) );//euclidean distance
					//float dist=abs(pow(dx,4))+abs(pow(dy,4));//weird distance
					//float dist= sqrt( (dx*dx) + (dy*dy) ) - (dist0/2);
					if( dist < shortest || n==0){
						shortest=dist;
						which=n;
					}
				}
			}

			pe->DrawScaledPixel(x,y,pe->palette->u32vals[which%130]);
			//pe->DrawChar(x,y,219,pe->palette->u32vals[which],pe->colour_bg,true);
		}
	}


	*/




	//Sprite16x16* beeta=new Sprite16x16(pe->screen,"lovely15.dat");
	//pe->DrawSprite16x16(beeta,640-17,360-17);

	/*
#include <random>
#define randx(low,high) (std::rand()%((high-low)+1))+low
	pe->WaitForMausClik();
	Uint32 i=0;
	while(i++<1000000){
		Uint32 fg=pe->MapRGB(randx(0,255),randx(0,255),randx(0,255));
		pe->DrawChar(randx(0,79),randx(0,44),randx(0,255),fg,0);
	}


	*/
	
	
	
	
	//pe->WaitForMausClik();
	//pe=new PangoEngine(1280,720,font,2,2,false);

	SetupLambdas();
	
	//set up palette and palette virginity 
	for (int i = 0; i < 256; i++)
	{
		PaletteColours[i]=pe->colour_bg;
		PaletteVirgin[i]=true;
	}

	InitUI();
	InitUIEvents();

	int sprsys_numsprites=1;
	sprsys_currentsprite_n=0;
	sprsys_currentsprite_p = sprsys_sprites[0] = new Sprite16x16;
	
	


	pe->DoEvents();
	
	//winter wrap up, winter wrap up!
	for (int i = 0; i < 40; i++)
	{
		if(sprsys_sprites[i]!=nullptr)delete sprsys_sprites[i];
	}

	return 0;
}

void ClearCurrentSpriet(void){
	if(sprsys_currentsprite_p!=nullptr){
		for(int y=0;y<16;y++){
			for(int x=0;x<16;x++){
				sprsys_currentsprite_p->transparency[x][y]=true;
				sprsys_currentsprite_p->colours[x][y]=0;
			}
		}
	}
	
	for(int y=0;y<32;y+=2){
		for(int x=0;x<32;x+=2){
			SprietGrid->DrawChar(201,pe->palette->u32vals[75],pe->colour_bg,x,y);
			SprietGrid->DrawChar(187,pe->palette->u32vals[75],pe->colour_bg,x+1,y);
			SprietGrid->DrawChar(200,pe->palette->u32vals[75],pe->colour_bg,x,y+1);
			SprietGrid->DrawChar(188,pe->palette->u32vals[75],pe->colour_bg,x+1,y+1);

		}

	}
}

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
	
}

void UpdatePen(void){
	//updates pencolour based on rgb sliders and draws colour square
	Uint8 r,g,b;
	r=redslider->value*16+redslider2->value;
	g=greenslider->value*16+greenslider2->value;
	b=blueslider->value*16+blueslider2->value;

	CurrentLeftPen=pe->MapRGB(r,g,b);
	DrawColourBlocks();
	UpdateHSBSliders(r,g,b);

	//UpdateColourText(r,g,b);
}

void UpdatePen_hsb(void){
	//called when you manually move hsb sliders.
	//need to set pen colour and rgb sliders
	float hh,ss,bb;
	Uint8 hhh,sss,bbb;
	hhh=(hslider->value*16)+hslider2->value;
	sss=(sslider->value*16)+sslider2->value;
	bbb=(bslider->value*16)+bslider2->value;

	ss=(float)sss/255.0f;
	bb=(float)bbb/255.0f;
	hh=((float)hhh/255.0f)*360.0f;

	std::array<int,3> rgb=HSB_to_RGB(hh,ss,bb);
	Uint32 rgb2=pe->MapRGB(rgb[0],rgb[1],rgb[2]);

	redslider->SetValue(rgb[0]/16);
	redslider2->SetValue(rgb[0]%16);
	greenslider->SetValue(rgb[1]/16);
	greenslider2->SetValue(rgb[1]%16);
	blueslider->SetValue(rgb[2]/16);
	blueslider2->SetValue(rgb[2]%16);

	CurrentLeftPen=rgb2;
	DrawColourBlocks();
	//UpdateColourText(rgb[0],rgb[1],rgb[2]);
}

void UpdateSliders(Uint32 n){
	//sets rgb sliders based on pen colour
	CurrentLeftPen=n;
	Uint8 r,g,b;
	pe->GetRGB(n,&r,&g,&b);
	redslider->SetValue(r/16);
	redslider2->SetValue(r%16);
	greenslider->SetValue(g/16);
	greenslider2->SetValue(g%16);
	blueslider->SetValue(b/16);
	blueslider2->SetValue(b%16);
	UpdateHSBSliders(r,g,b);
	UpdateColourText(r,g,b);
}

void BresLine(int x0,int y0,int x1,int y1,bool isleftbuttonp,bool erasep,bool finalp){
	std::function<void (int x,int y)>& WhatToDoForEachSquare=Lambda2x2PixelFn[WhichLambdap(isleftbuttonp,erasep,finalp)];

	int dx = abs(x1-x0);
	int dy = abs(y1-y0); 
	int sx=(x0 < x1)?1:-1;
	int sy=(y0 < y1)?1:-1;
	int err= dx-dy;
 
	while(1){
		WhatToDoForEachSquare(x0,y0);
		if(x0 == x1 && y0 == y1)break;
		int e2 = 2*err;
		if (e2 > -dy){
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx){
			err += dx;
			y0 += sy;
		}
	}
}

void DrawRect(int x,int y,int x2,int y2,bool isframep, bool isleftbuttonp,bool erasep,bool finalp){
	int swap;
	if(x2<x){swap=x;x=x2;x2=swap;}
	if(y2<y){swap=y;y=y2;y2=swap;}

	std::function<void (int x,int y)>& WhatToDoForEachSquare=Lambda2x2PixelFn[WhichLambdap(isleftbuttonp,erasep,finalp)];

	if(isframep){		
		for(int yy=y+1;yy<y2;yy++){
			WhatToDoForEachSquare(x,yy);
			WhatToDoForEachSquare(x2,yy);
		}
		for(int xx=x;xx<x2+1;xx++){
			WhatToDoForEachSquare(xx,y);
			WhatToDoForEachSquare(xx,y2);
		}
	} else {
		for(int yy=y;yy<y2+1;yy++){
			for(int xx=x;xx<x2+1;xx++){
				WhatToDoForEachSquare(xx,yy);
			}
		}
	}
}
void DrawFilledCircle(int x0, int y0, int radius,  bool isleftbuttonp,bool erasep,bool finalp)
{

	std::function<void (int x,int y)>& WhatToDoForEachSquare=Lambda2x2PixelFn[WhichLambdap(isleftbuttonp,erasep,finalp)];


	//TODO it's ridiculous to use bresline for these straight lines really. sort it out
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  WhatToDoForEachSquare(x0, y0 + radius);
  WhatToDoForEachSquare(x0, y0 - radius);
  BresLine(x0 + radius, y0,x0 - radius, y0,isleftbuttonp,erasep,finalp);
 
  while(x < y)
  {

    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    BresLine(x0 + x, y0 + y,x0 - x, y0 + y,isleftbuttonp,erasep,finalp);
    BresLine(x0 + x, y0 - y,x0 - x, y0 - y,isleftbuttonp,erasep,finalp);
    BresLine(x0 + y, y0 + x,x0 - y, y0 + x,isleftbuttonp,erasep,finalp);
    BresLine(x0 + y, y0 - x,x0 - y, y0 - x,isleftbuttonp,erasep,finalp);
  }
}

void DrawCircle(int x0, int y0, int radius,  bool isleftbuttonp,bool erasep,bool finalp)
{

  std::function<void (int x,int y)>& WhatToDoForEachSquare=Lambda2x2PixelFn[WhichLambdap(isleftbuttonp,erasep,finalp)];

  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  WhatToDoForEachSquare(x0, y0 + radius);
  WhatToDoForEachSquare(x0, y0 - radius);
  WhatToDoForEachSquare(x0 + radius, y0);
  WhatToDoForEachSquare(x0 - radius, y0);
 
  while(x < y)
  {

    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    WhatToDoForEachSquare(x0 + x, y0 + y);
    WhatToDoForEachSquare(x0 - x, y0 + y);
    WhatToDoForEachSquare(x0 + x, y0 - y);
    WhatToDoForEachSquare(x0 - x, y0 - y);
    WhatToDoForEachSquare(x0 + y, y0 + x);
    WhatToDoForEachSquare(x0 - y, y0 + x);
    WhatToDoForEachSquare(x0 + y, y0 - x);
    WhatToDoForEachSquare(x0 - y, y0 - x);
  }
}

inline void RedrawPixel(Uint8 x,Uint8 y){
	if(sprsys_currentsprite_p->transparency[x][y]){
		Draw2x2pixa_alpha(x,y);
	} else {
		Uint32 c=sprsys_currentsprite_p->colours[x][y];
		SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2,y*2);
		SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2+1,y*2);
		SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2,y*2+1);
		SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2+1,y*2+1);
	}


}

inline void Draw2x2pixa_pen(Uint8 x,Uint8 y){
	SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2,y*2);
	SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2+1,y*2);
	SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2,y*2+1);
	SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2+1,y*2+1);
}
inline void Draw2x2pixa_givencolour(Uint8 x,Uint8 y,Uint32 c){
	SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2,y*2);
	SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2+1,y*2);
	SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2,y*2+1);
	SprietGrid->DrawChar(' ',pe->colour_bg,c,x*2+1,y*2+1);
}
inline void Draw2x2pixa_erase_temp(Uint8 x,Uint8 y){
	SprietGrid->DrawCharEx(' ',pe->colour_bg,CurrentLeftPen,x*2,	y*2,false,true,true,true,true,true);
	SprietGrid->DrawCharEx(' ',pe->colour_bg,CurrentLeftPen,x*2+1,	y*2,false,true,true,true,true,true);
	SprietGrid->DrawCharEx(' ',pe->colour_bg,CurrentLeftPen,x*2,	y*2+1,false,true,true,true,true,true);
	SprietGrid->DrawCharEx(' ',pe->colour_bg,CurrentLeftPen,x*2+1,	y*2+1,false,true,true,true,true,true);

	
}
inline void Draw2x2pixa_pen_temp(Uint8 x,Uint8 y){
		SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2,y*2,false);
		SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2+1,y*2,false);
		SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2,y*2+1,false);
		SprietGrid->DrawChar(' ',pe->colour_bg,CurrentLeftPen,x*2+1,y*2+1,false);
}

inline void Draw2x2pixa_alpha(Uint8 x,Uint8 y){
	SprietGrid->DrawChar(201,pe->palette->u32vals[75],pe->colour_bg,x*2,y*2);
	SprietGrid->DrawChar(187,pe->palette->u32vals[75],pe->colour_bg,x*2+1,y*2);
	SprietGrid->DrawChar(200,pe->palette->u32vals[75],pe->colour_bg,x*2,y*2+1);
	SprietGrid->DrawChar(188,pe->palette->u32vals[75],pe->colour_bg,x*2+1,y*2+1);
}

inline void Draw2x2pixa_alpha_temp(Uint8 x,Uint8 y){
	SprietGrid->DrawChar(201,pe->palette->u32vals[75],pe->colour_bg,x*2,y*2,false);
	SprietGrid->DrawChar(187,pe->palette->u32vals[75],pe->colour_bg,x*2+1,y*2,false);
	SprietGrid->DrawChar(200,pe->palette->u32vals[75],pe->colour_bg,x*2,y*2+1,false);
	SprietGrid->DrawChar(188,pe->palette->u32vals[75],pe->colour_bg,x*2+1,y*2+1,false);
}
void SetupLambdas(void){
#define AAA if(x<0 || x>15 || y<0 || y>15) return;
			Lambda2x2PixelFn[0]=[](int x,int y) -> void {AAA sprsys_currentsprite_p->colours[x][y]=CurrentLeftPen;sprsys_currentsprite_p->transparency[x][y]=false;Draw2x2pixa_pen(x,y);};
			Lambda2x2PixelFn[1]=[](int x,int y) -> void {AAA sprsys_currentsprite_p->transparency[x][y]=true;Draw2x2pixa_alpha(x,y);};
			Lambda2x2PixelFn[2]=[](int x,int y) -> void {AAA Draw2x2pixa_erase_temp(x,y);};
			Lambda2x2PixelFn[3]=[](int x,int y) -> void {AAA Draw2x2pixa_pen_temp(x,y);};
			Lambda2x2PixelFn[4]=[](int x,int y) -> void {AAA Draw2x2pixa_erase_temp(x,y);};
			Lambda2x2PixelFn[5]=[](int x,int y) -> void {AAA Draw2x2pixa_alpha_temp(x,y);};
#undef AAA
}
Uint8 WhichLambdap(bool isleftbuttonp, bool erasep, bool finalp){
	//ugly workaround for the supposed problem of returning a lambda
	if(finalp){
		if(isleftbuttonp){
			return 0;
		} else {
			return 1;
		}
	} else if(isleftbuttonp){
		if(erasep){
			return 2;
		} else {
			return 3;
		}
	} else {
		if(erasep){
			return 4;
		} else {
			return 5;
		}
	}
}
inline int GetRadius(int x0,int y0,int x1,int y1){
	int delta1=abs(x0-x1),delta2=abs(y0-y1);
	return (delta1>delta2)?delta1:delta2;
}

bool FloodFill(int x, int y, Uint32 ColourToMatch){
	if(x<0||y<0||x>15||y>15)return false; 
	if(sprsys_currentsprite_p->colours[x][y]!=ColourToMatch)return false;
	Lambda2x2PixelFn[0](x,y);
	FloodFill(x-1,y,ColourToMatch);
	FloodFill(x+1,y,ColourToMatch);
	FloodFill(x,y-1,ColourToMatch);
	FloodFill(x,y+1,ColourToMatch);
}

bool FloodFillAlpha(int x, int y){
	if(x<0||y<0||x>15||y>15)return false; 
	if(!sprsys_currentsprite_p->transparency[x][y])return false;
	Lambda2x2PixelFn[0](x,y);
	FloodFillAlpha(x-1,y);
	FloodFillAlpha(x+1,y);
	FloodFillAlpha(x,y-1);
	FloodFillAlpha(x,y+1);
}

void ScrollGrid(int dx,int dy){
	static Uint32 othersprite[16][16];
	static bool othersprite_alpha[16][16];
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int newx=x+dx; int newy=y+dy;
			if(newx==-1)newx=15; else if(newx==16)newx=0;
			if(newy==-1)newy=15; else if(newy==16)newy=0;
			othersprite[newx][newy]=sprsys_currentsprite_p->colours[x][y];
			othersprite_alpha[newx][newy]=sprsys_currentsprite_p->transparency[x][y];
		}
	}

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			sprsys_currentsprite_p->colours[x][y]=othersprite[x][y];
			sprsys_currentsprite_p->transparency[x][y]=othersprite_alpha[x][y];
			RedrawPixel(x,y);
		}
	}

	UpdateSmallView();

}

void UpdateHSBSliders(Uint8 r,Uint8 g,Uint8 b){
	float hh,ss,bb;
	std::array<float,3> ret;
	ret=RGB_to_HSB(r,g,b);
	hh=(ret[0]/360.0f)*255.0f;
	ss=ret[1]*255.0f;
	bb=ret[2]*255.0f;

	//std::cout << hh << " " << ss << " " << bb << std::endl;

	Uint8 hhh=(Uint8)hh;
	Uint8 bbb=(Uint8)bb;
	Uint8 sss=(Uint8)ss;

	hslider->SetValue(hhh/16);
	hslider2->SetValue(hhh%16);
	sslider->SetValue(sss/16);
	sslider2->SetValue(sss%16);
	bslider->SetValue(bbb/16);
	bslider2->SetValue(bbb%16);

}

void InitUI(void){
	
	spritebar=new CustomWidget(pe->desktop,"sprite bar",PangoTUI::Rectangle(0,42,80,3,true));

	PaletteGrid=new Area(PangoTUI::Rectangle(32,9,18,18,true));
	pe->desktop->AddSub(PaletteGrid);
	PaletteGrid->DrawFrame(pe->colour_window_border,pe->colour_bg,0,0,18,18);
	for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				PaletteGrid->DrawChar(250,pe->palette->u32vals[75],pe->colour_bg,i+1,j+1);
			}
		}
	PaletteShowSelected();
	SprietGrid=new Area(PangoTUI::Rectangle(0,0,32,32,true));
	pe->desktop->AddSub(SprietGrid);
	ClearCurrentSpriet();
	

	redslider=new Slider(pe->desktop,"red slider",32,0,16);
	redslider2=new Slider(pe->desktop,"red slider2",32,1,16);
	greenslider=new Slider(pe->desktop,"green slider",32,2,16);
	greenslider2=new Slider(pe->desktop,"green slider2",32,3,16);
	blueslider=new Slider(pe->desktop,"blue slider",32,4,16);
	blueslider2=new Slider(pe->desktop,"blue slider2",32,5,16);

	hslider=new Slider(pe->desktop,"h slider",49,0,16);
	hslider2=new Slider(pe->desktop,"h slider2",49,1,16);
	sslider=new Slider(pe->desktop,"s slider",49,2,16);
	sslider2=new Slider(pe->desktop,"s slider2",49,3,16);
	bslider=new Slider(pe->desktop,"b slider",49,4,16);
	bslider2=new Slider(pe->desktop,"b slider2",49,5,16);

	ModeButtons[0]=new Button(pe->desktop,"pen ",pe->palette->u32vals[Yellow],0,33,false,true,(Area*)0);
	ModeButtons[1]=new Button(pe->desktop,"line",pe->colour_window_border,6,33,false,true,(Area*)1);
	ModeButtons[2]=new Button(pe->desktop,"rect",pe->colour_window_border,12,33,false,true,(Area*)2);
	ModeButtons[3]=new Button(pe->desktop,"circ",pe->colour_window_border,18,33,false,true,(Area*)3);
	ModeButtons[4]=new Button(pe->desktop,"fill",pe->colour_window_border,24,33,false,true,(Area*)4);
	ModeButtons[5]=new Button(pe->desktop,"Frec",pe->colour_window_border,12,37,false,true,(Area*)5);
	ModeButtons[6]=new Button(pe->desktop,"Fcir",pe->colour_window_border,18,37,false,true,(Area*)6);

	ScrollUp=new IconButton(pe->desktop,"scroll up",31,33,24);
	ScrollDown=new IconButton(pe->desktop,"scroll down",31,35,25);
	ScrollLeft=new IconButton(pe->desktop,"scroll left",30,34,27);
	ScrollRight=new IconButton(pe->desktop,"scroll right",32,34,26);

	ScrollUp->OnClick=[](Area* a,Event& e){ ScrollGrid(0,-1); };
	ScrollDown->OnClick=[](Area* a,Event& e){ ScrollGrid(0,1); };
	ScrollLeft->OnClick=[](Area* a,Event& e){ ScrollGrid(-1,0); };
	ScrollRight->OnClick=[](Area* a,Event& e){ ScrollGrid(1,0); };

	RGBlabel=new Area(PangoTUI::Rectangle(48,0,1,6,true));
	pe->desktop->AddSub(RGBlabel);

	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Red],0,0);
	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Red],0,1);
	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Green],0,2);
	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Green],0,3);
	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Blue],0,4);
	RGBlabel->DrawChar(' ',0,pe->palette->u32vals[Blue],0,5);
	
	HSBlabel=new Area(PangoTUI::Rectangle(65,0,1,6,true));
	pe->desktop->AddSub(HSBlabel);

	HSBlabel->DrawChar('H',pe->colour_window_border,0,0,0);
	HSBlabel->DrawChar('h',pe->colour_window_border,0,0,1);
	HSBlabel->DrawChar('S',pe->colour_window_border,0,0,2);
	HSBlabel->DrawChar('s',pe->colour_window_border,0,0,3);
	HSBlabel->DrawChar('B',pe->colour_window_border,0,0,4);
	HSBlabel->DrawChar('b',pe->colour_window_border,0,0,5);
	
	ColourValueTextBox=new SingleLineTextEntry(pe->desktop,"colour value text",35,6,6,6);
	
	UpdateSliders(pe->MapRGB(255,0,255));

	PenColor=new Area(PangoTUI::Rectangle(32,6,3,3,true));
	pe->desktop->AddSub(PenColor);
	DrawColourBlocks();

	SaveButton=new Button(pe->desktop,"save",pe->colour_window_border,70,38);
	LoadButton=new Button(pe->desktop,"load",pe->colour_window_border,70,34);
	CLSButton=new Button(pe->desktop,"CLS",pe->colour_window_border,24,37);

	SpriteName=new SingleLineTextEntry(pe->desktop,"sprite name textenter",40,38,16,16);
	//SpriteName->SetValue("sprite");
	//SpriteName->FullReDraw();
	//RedrawExtraBits();	
}

void InitUIEvents(void){
	SpriteName->OnSubmit=NONEVENTLAMBDA {
		SpriteNames[sprsys_currentsprite_n]=SpriteName->value;
	};


	PaletteGrid->OnLeftClick=EVENTLAMBDA {
		int x=a->SCREENXTOAREAX(e.CurrentScreenX);
		int y=a->SCREENYTOAREAY(e.CurrentScreenY);
		CurrentPenIndex=((y-1)*16)+(x-1);
		CurrentLeftPen=PaletteColours[CurrentPenIndex];
		DrawColourBlocks(false);
		UpdateSliders(CurrentLeftPen);
		PaletteShowSelected();
	};

	
	PaletteGrid->OnMiddleClick=EVENTLAMBDA {
		int x=a->SCREENXTOAREAX(e.CurrentScreenX);
		int y=a->SCREENYTOAREAY(e.CurrentScreenY);
		PaletteColours[CurrentPenIndex]=PaletteColours[((y-1)*16)+(x-1)];
		CurrentLeftPen=PaletteColours[CurrentPenIndex];
		DrawColourBlocks(false);
		UpdateSliders(CurrentLeftPen);
		//PaletteShowSelected();
	};

	ColourValueTextBox->OnSubmit=NONEVENTLAMBDA {
		Uint32 u =std::stoul(SINGLELINETEXTENTRY(a)->value, 0, 16);
		Uint32 u2= pe->MapRGB((u&(255<<16))>>16,(u&(255<<8))>>8,u&255);
		UpdateSliders(u2);
		DrawColourBlocks();
	};


	spritebar->OnFullReDraw=[](Area* a)->void {
		a->Clear(0,0);
		for (int i = 0; i < 40; i++){
			pe->DrawString_halfwidth(std::to_string(i),i*4+2,42,pe->colour_icon_hi,pe->colour_bg);
			if(sprsys_sprites[i]!=nullptr)
				pe->DrawSprite16x16(sprsys_sprites[i],16*i,43*8);
		}
		DrawSpriteBarNumber_selected(sprsys_currentsprite_n);
	};

	spritebar->OnLeftClick=[](Area* a, Event& e)->void {
		int sp=e.CurrentScreenX/2;
		if(sp!=sprsys_currentsprite_n){
			DrawSpriteBarNumber_normal(sprsys_currentsprite_n);
			sprsys_currentsprite_n=sp;
			SpriteName->SetValue(SpriteNames[sp]);
			if(sprsys_sprites[sp]==nullptr)sprsys_sprites[sp]=new Sprite16x16;
			sprsys_currentsprite_p=sprsys_sprites[sp];
			DrawSpriteInGrid(sprsys_currentsprite_p);
			UpdateSmallView();
			DrawSpriteBarNumber_selected(sprsys_currentsprite_n);
		}

	};

	spritebar->OnMiddleClick=[](Area* a, Event& e)->void {
		int sp=e.CurrentScreenX/2;
		if(sp!=sprsys_currentsprite_n){
			if(sprsys_sprites[sp]!=nullptr){
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						sprsys_currentsprite_p->colours[x][y]=sprsys_sprites[sp]->colours[x][y];
						sprsys_currentsprite_p->transparency[x][y]=sprsys_sprites[sp]->transparency[x][y];
					}
				}
			
				DrawSpriteInGrid(sprsys_currentsprite_p);
				UpdateSmallView();
			}			
		}

	};

	spritebar->FullReDraw();

	for(int i=0;i<7;i++){
		ModeButtons[i]->OnClick=[](Area* a,Event& e)->void{
			for(int i=0;i<7;i++){
				ModeButtons[i]->TextColour=pe->colour_window_border; 
				ModeButtons[i]->FullReDraw();
			}
			BUTTON(a)->TextColour=pe->palette->u32vals[Yellow];
			a->FullReDraw();
			CurrentMode=(int)a->AreaOfInterest;

		};
	}
	
	PenColor->OnLeftClick=[](Area* a, Event& e)->void {
		DrawColourGrid();
		CharCell* cc=pe->WaitForMausClik_RetCharCell();
		int tempbg=cc->bg;
		pe->CLS_allcells();
		pe->desktop->FullReDraw();
		UpdateSliders(tempbg);
		DrawColourBlocks();
	};

		redslider->OnValueChangedManually=
		redslider2->OnValueChangedManually=
		greenslider->OnValueChangedManually=
		greenslider2->OnValueChangedManually=
		blueslider->OnValueChangedManually=
		blueslider2->OnValueChangedManually= [](Area* a)->void {
			UpdatePen();
	};

		hslider->OnValueChangedManually=
		hslider2->OnValueChangedManually=
		sslider->OnValueChangedManually=
		sslider2->OnValueChangedManually=
		bslider->OnValueChangedManually=
		bslider2->OnValueChangedManually=[](Area*)->void {
			UpdatePen_hsb();
		};

		

	SaveButton->OnLeftClick=[](Area* a, Event& e)->void {
		a->visible=false;
		a->FullReDraw();

		ZeroMemory(&ofn, sizeof(ofn));
		szFile[0] = '\0'; 
		ofn.lStructSize = sizeof(OPENFILENAME); 
		ofn.hwndOwner = NULL; //winhandle; 
		ofn.lpstrFilter =  L"Pango Sprite File\0*.PSF\0All\0*.*\0";
		ofn.lpstrFile= szFile; 
		ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
		ofn.lpstrFileTitle = NULL; //the title of the file
		ofn.nMaxFileTitle = NULL; //length of above
		ofn.lpstrInitialDir = NULL; 
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT; 
		ofn.lpstrTitle = NULL;  //will be "save as" if null
 
		if (GetSaveFileName(&ofn)==TRUE){
	
		
			std::ofstream ofile;
			ofile.open(ofn.lpstrFile,std::ios::out | std::ios::binary);
			char buffer[4];
			//save the sprites
			for(int s=0;s<40;s++){
				if(sprsys_sprites[s]==nullptr)sprsys_sprites[s]=new Sprite16x16;
				Uint8 r,g,b;
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{	
						pe->GetRGB(sprsys_sprites[s]->colours[x][y],&r,&g,&b);
						buffer[0]=r;buffer[1]=g;buffer[2]=b;
						buffer[3]=sprsys_sprites[s]->transparency[x][y]?1:0 ;
						ofile.write(buffer,4);
						//ofile << r << g << b << sprsys_sprites[s]->transparency[x][y]?(char)1:(char)0 ;
			
					}
				}
			}
			//save sprite names
			for (int i = 0; i < 40; i++)
			{
				char c=(char)SpriteNames[i].length();
				ofile.write(&c,1);
				ofile.write(SpriteNames[i].c_str(),SpriteNames[i].length());
			}
			//save palette, colours and virginity
			for (int i = 0; i < 256; i++)
			{
				Uint8 r,g,b;
				char c;
				pe->GetRGB(	PaletteColours[i],&r,&g,&b);
				c=(unsigned char)r;ofile.write(&c,1);
				c=(unsigned char)g;ofile.write(&c,1);
				c=(unsigned char)b;ofile.write(&c,1);
				c=(unsigned char)PaletteVirgin[i]?1:0;ofile.write(&c,1);
			}
			ofile.close();
		}//end if filename thing true
		a->visible=true;
		a->FullReDraw();
		};

	LoadButton->OnLeftClick=[](Area* a, Event& e)->void {
		a->visible=false;
		a->FullReDraw();
	
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"Pango Sprite File\0*.PSF\0All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					// Display the Open dialog box. 

		if (GetOpenFileName(&ofn)==TRUE) {

			std::ifstream ifile;
			ifile.open(ofn.lpstrFile,std::ios::in | std::ios::binary);
			char buffer[4];
			for(int s=0;s<40;s++){
				if(sprsys_sprites[s]==nullptr)sprsys_sprites[s]=new Sprite16x16;
				Uint8 r,g,b;
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{						
						ifile.read(buffer,4);
						sprsys_sprites[s]->colours[x][y] = pe->MapRGB(buffer[0],buffer[1],buffer[2]);
						sprsys_sprites[s]->transparency[x][y] = (buffer[3] == 1)?true:false ;			
					}
				}
			}
			//load names
			for (int i = 0; i < 40; i++)
			{
				char buffer[16];
				char c;
				ifile.read(&c,1);
				ifile.read(buffer,c);
				if(c<16)buffer[c]=0;
				SpriteNames[i]=buffer;
			}
			//load palette, colours and virginity
			for (int i = 0; i < 256; i++)
			{
				char r,g,b;
				ifile.read(&r,1); ifile.read(&g,1); ifile.read(&b,1);
				char c;
				ifile.read(&c,1);

				PaletteVirgin[i]=(c==1)?true:false;
				PaletteColours[i]=pe->MapRGB(r,g,b);
				if(PaletteVirgin[i]) PaletteGrid->DrawChar(250,pe->palette->u32vals[75],pe->colour_bg,(i%16)+1,(i/16)+1);
				else PaletteGrid->DrawChar(219,PaletteColours[i],pe->colour_bg,(i%16)+1,(i/16)+1);
			} 
			ifile.close();
		
			spritebar->FullReDraw();
			
			if(sprsys_currentsprite_n!=0){
				DrawSpriteBarNumber_normal(sprsys_currentsprite_n);
				sprsys_currentsprite_n=0;
				DrawSpriteBarNumber_selected(0);
			}

			sprsys_currentsprite_p=sprsys_sprites[0];
			DrawSpriteInGrid(sprsys_currentsprite_p);
			UpdateSmallView();
		}//end if filename
		a->visible=true;
		a->FullReDraw();
		};

	CLSButton->OnLeftClick=[](Area* a,Event& e)->void{
		ClearCurrentSpriet();
		UpdateSmallView();
	};

	SprietGrid->OnMausMoob=[](Area* a, Event& e)->void {
		//std::cout << " SprietGrid->OnMausMoob " << std::endl;
		int x=a->SCREENXTOAREAX(e.CurrentScreenX);
		int y=a->SCREENYTOAREAY(e.CurrentScreenY);
		x= x / 2; y= y /2;

		if(x>=0 && x<16 && y>=0 && y<16){
			if(lastcursorx>-1){
				if(sprsys_currentsprite_p->transparency[lastcursorx][lastcursory]){
					SprietGrid->DrawChar(201,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2,lastcursory*2);
					SprietGrid->DrawChar(187,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2+1,lastcursory*2);
					SprietGrid->DrawChar(200,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2,lastcursory*2+1);
					SprietGrid->DrawChar(188,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2+1,lastcursory*2+1);
				} else {
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2,lastcursory*2);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2+1,lastcursory*2);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2,lastcursory*2+1);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2+1,lastcursory*2+1);
				}
			}

			SprietGrid->DrawCharEx(182,pe->colour_window_border,pe->colour_bg,x*2,y*2,true,true,true,false,true,false);
			SprietGrid->DrawCharEx(181,pe->colour_window_border,pe->colour_bg,x*2+1,y*2,true,true,true,false,true,false);
			SprietGrid->DrawCharEx(181,pe->colour_window_border,pe->colour_bg,x*2,y*2+1,true,true,true,false,true,false);
			SprietGrid->DrawCharEx(182,pe->colour_window_border,pe->colour_bg,x*2+1,y*2+1,true,true,true,false,true,false);
			lastcursorx=x;lastcursory=y;
		}
	};
	SprietGrid->OnMausLeeb=[](Area* a, Event& e)->void {
		//std::cout << " SprietGrid->OnMausLeeb " << std::endl;
		if(lastcursorx>-1){
			if(sprsys_currentsprite_p->transparency[lastcursorx][lastcursory]){
				SprietGrid->DrawChar(201,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2,lastcursory*2);
					SprietGrid->DrawChar(187,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2+1,lastcursory*2);
					SprietGrid->DrawChar(200,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2,lastcursory*2+1);
					SprietGrid->DrawChar(188,pe->palette->u32vals[75],pe->colour_bg,lastcursorx*2+1,lastcursory*2+1);
				} else {
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2,lastcursory*2);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2+1,lastcursory*2);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2,lastcursory*2+1);
					SprietGrid->DrawChar(' ',pe->colour_bg,sprsys_currentsprite_p->colours[lastcursorx][lastcursory],lastcursorx*2+1,lastcursory*2+1);
				}
			lastcursorx=-1;
			//int x0,y0,x2,y2;
			if(!Virgin){
					int x0=a->SCREENXTOAREAX(e.DragBeginScreenX);
					int y0=a->SCREENYTOAREAY(e.DragBeginScreenY);
					x0= x0 / 2; y0= y0 /2;
					int x2=a->SCREENXTOAREAX(e.LastScreenX);
					int y2=a->SCREENYTOAREAY(e.LastScreenY);
					x2= x2 / 2; y2= y2 /2;
				switch(CurrentMode){
				
					case(1)://line
						if(e.mausbutton==1)BresLine(x0,y0,x2,y2,true,true,false); else if(e.mausbutton==4)BresLine(x0,y0,x2,y2,false,true,false);
					break;
					case(2)://frame rect
					 if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,true,true,true); else if(e.mausbutton==4)DrawRect(x0,y0,x2,y2,true,false,true);
						break;
					case(5)://filled rect
					if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,false,true,true); else if(e.mausbutton==4)DrawRect(x0,y0,x2,y2,false,false,true);
						break;
					case(3)://frame circle
					 if(e.mausbutton==1)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,true); else if(e.mausbutton==4)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,true);
						break;
					case(6)://filled circle
					if(e.mausbutton==1)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,true); else if(e.mausbutton==4)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,true);
						break;
				}
				Virgin=true;
			}

		}

	};
	SprietGrid->OnMausDown=[](Area* a, Event& e)->void {//was click
		//std::cout << " SprietGrid->OnClick " << std::endl;
		int x=a->SCREENXTOAREAX(e.CurrentScreenX);
		int y=a->SCREENYTOAREAY(e.CurrentScreenY);
		x= x / 2; y= y /2;
		switch(e.mausbutton){
		case 1:
			if(CurrentMode==4){
				if(sprsys_currentsprite_p->transparency[x][y])FloodFillAlpha(x,y); else if(sprsys_currentsprite_p->colours[x][y]!=CurrentLeftPen)FloodFill(x,y,sprsys_currentsprite_p->colours[x][y]);		
			} else if(CurrentMode==0){
				Lambda2x2PixelFn[0](x,y);
				//sprsys_currentsprite_p->colours[x][y]=CurrentLeftPen;
				//sprsys_currentsprite_p->transparency[x][y]=false;
				//Draw2x2pixa_pen(x,y);
			}
			break;
		case 2:
			UpdateSliders(sprsys_currentsprite_p->colours[x][y]);
			DrawColourBlocks();
			break;
		case 3:
			sprsys_currentsprite_p->transparency[x][y]=true;
			sprsys_currentsprite_p->colours[x][y]=0;
			Draw2x2pixa_alpha(x,y);
			break;
		}
		UpdateSmallView();
	};	
	SprietGrid->OnDragEnd=[](Area*a,Event& e)->void{
		//std::cout << " SprietGrid->OndragEnd " << std::endl;
		int x0=a->SCREENXTOAREAX(e.DragBeginScreenX);
		int y0=a->SCREENYTOAREAY(e.DragBeginScreenY);
		x0= x0 / 2; y0= y0 /2;
		int x2=a->SCREENXTOAREAX(e.CurrentScreenX);
		int y2=a->SCREENYTOAREAY(e.CurrentScreenY);
		x2= x2 / 2; y2= y2 /2;
		if(!e.DragOutOfBoundsP){
			switch(CurrentMode){
				case 1://line
					if(e.mausbutton==1)BresLine(x0,y0,x2,y2,true,false,true); else if(e.mausbutton==3)BresLine(x0,y0,x2,y2,false,false,true);
					break;
				break;
				case 2: //frame rect
					if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,true,true,false,true); else if(e.mausbutton==3)DrawRect(x0,y0,x2,y2,true,false,false,true);
					break;
				case 5://filled rect
					if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,false,true,false,true); else if(e.mausbutton==3)DrawRect(x0,y0,x2,y2,false,false,false,true);
					break;
				case 3://frame circle
					if(e.mausbutton==1)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,false,true); else if(e.mausbutton==3)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,false,true);
					break;
				case 6://filled circle
					if(e.mausbutton==1)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,false,true); else if(e.mausbutton==3)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,false,true);
					break;
			}
			UpdateSmallView();
		}
		Virgin=true;
	};
	SprietGrid->OnDrag=[](Area* a, Event& e)->void {
		//std::cout << " SprietGrid->Ondrag " << std::endl;
		if(!e.DragOutOfBoundsP){
			int x=a->SCREENXTOAREAX(e.CurrentScreenX);
			int y=a->SCREENYTOAREAY(e.CurrentScreenY);
			x= x / 2; y= y /2;
			int x0,y0;
			
			if(CurrentMode==0){
				if(e.mausbutton==1)Lambda2x2PixelFn[0](x,y); else if(e.mausbutton==4)Lambda2x2PixelFn[1](x,y);
			} else {
				x0=a->SCREENXTOAREAX(e.DragBeginScreenX);
				y0=a->SCREENYTOAREAY(e.DragBeginScreenY);
				x0= x0 / 2; y0= y0 /2;
				int x2=a->SCREENXTOAREAX(e.LastScreenX);
				int y2=a->SCREENYTOAREAY(e.LastScreenY);
				x2= x2 / 2; y2= y2 /2;
				switch(CurrentMode){
					case(1)://line
						if(!Virgin) if(e.mausbutton==1)BresLine(x0,y0,x2,y2,true,true,false); else if(e.mausbutton==4)BresLine(x0,y0,x2,y2,false,true,false);
						if(e.mausbutton==1)BresLine(x0,y0,x,y,true,false,false); else if(e.mausbutton==4)BresLine(x0,y0,x,y,false,false,false);
					break;
					case(2)://frame rect
						if(!Virgin) if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,true,true,true); else if(e.mausbutton==4)DrawRect(x0,y0,x2,y2,true,false,true);
						if(e.mausbutton==1)DrawRect(x0,y0,x,y,true,true,false); else if(e.mausbutton==4)DrawRect(x0,y0,x,y,true,false,false);
						break;
					case(5)://filled rect
						if(!Virgin) if(e.mausbutton==1)DrawRect(x0,y0,x2,y2,false,true,true); else if(e.mausbutton==4)DrawRect(x0,y0,x2,y2,false,false,true);
						if(e.mausbutton==1)DrawRect(x0,y0,x,y,false,true,false); else if(e.mausbutton==4)DrawRect(x0,y0,x,y,false,false,false);
						break;
					case(3)://frame circle
						if(!Virgin) if(e.mausbutton==1)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,true); else if(e.mausbutton==4)DrawCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,true);
						if(e.mausbutton==1)DrawCircle(x0,y0,GetRadius(x0,y0,x,y),true,false); else if(e.mausbutton==4)DrawCircle(x0,y0,GetRadius(x0,y0,x,y),false,false);
						break;
					case(6)://filled circle
						if(!Virgin)	if(e.mausbutton==1)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),true,true); else if(e.mausbutton==4)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x2,y2),false,true);
						if(e.mausbutton==1)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x,y),true,false); else if(e.mausbutton==4)DrawFilledCircle(x0,y0,GetRadius(x0,y0,x,y),false,false);
						break;
				}
				Virgin=false;
			}
		}
	};
	
	
}
void UpdateSmallView(void){
	//this is the current sprite preview:
	pe->DrawSprite16x16(sprsys_currentsprite_p,30*8,37*8);
	//and this is the sprite "in place" in the sprite bank
	pe->DrawSprite16x16(sprsys_currentsprite_p,sprsys_currentsprite_n*16,43*8);
}

//void RedrawExtraBits(void){
//	for (int i = 0; i < 40; i++)
//	{
//		pe->DrawString_halfwidth(std::to_string(i),i*4+2,42,pe->colour_icon_hi,pe->colour_bg);
//	}
//}

void DrawSpriteInGrid(Sprite16x16* s){
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if(s->transparency[x][y])Draw2x2pixa_alpha(x,y);
			else Draw2x2pixa_givencolour(x,y,s->colours[x][y]);
		}
	}
}

inline void DrawSpriteBarNumber_normal(int _x){
	pe->DrawString_halfwidth(std::to_string(_x),_x*4+2,42,pe->colour_icon_hi,pe->colour_bg);
}

inline void DrawSpriteBarNumber_selected(int _x){
	pe->DrawString_halfwidth(std::to_string(_x),_x*4+2,42,pe->colour_bg,pe->colour_icon_hi);
}

void UpdateColourText(Uint8 a,Uint8 b,Uint8 c){
	std::stringstream stream;
	stream << std::setfill ('0') << std::setw(2) << std::hex << (int)a << std::setw(2) << (int)b << std::setw(2) << (int)c;
	std::string x=stream.str();
	ColourValueTextBox->SetValue(x);
}

inline void DrawColourBlocks(bool dotexttoo){
	PenColor->DrawFilledRect(' ',0,CurrentLeftPen,0,0,2,2);

	PaletteColours[CurrentPenIndex]=CurrentLeftPen;
	PaletteGrid->DrawChar(219,CurrentLeftPen,pe->colour_bg,(CurrentPenIndex%16)+1,(CurrentPenIndex/16)+1);

	PaletteVirgin[CurrentPenIndex]=false;

	if(dotexttoo){
		UpdateColourText((CurrentLeftPen&(255<<16))>>16,(CurrentLeftPen&(255<<8))>>8,CurrentLeftPen&255);
	}


}

void PaletteShowSelected(void){
	static Uint8 posx=93,posy=93;
	if(posx!=93){
		PaletteGrid->DrawChar(179,pe->colour_window_border,pe->colour_bg,0,posy);
		PaletteGrid->DrawChar(179,pe->colour_window_border,pe->colour_bg,17,posy);
		PaletteGrid->DrawChar(196,pe->colour_window_border,pe->colour_bg,posx,0);
		PaletteGrid->DrawChar(196,pe->colour_window_border,pe->colour_bg,posx,17);
	}
	
	Uint8 x,y;
	y=(CurrentPenIndex/16)+1;
	x=(CurrentPenIndex%16)+1;

	PaletteGrid->DrawChar(133,pe->colour_window_border,pe->colour_bg,0,y);
	PaletteGrid->DrawChar(135,pe->colour_window_border,pe->colour_bg,17,y);
	PaletteGrid->DrawChar(134,pe->colour_window_border,pe->colour_bg,x,0);
	PaletteGrid->DrawChar(132,pe->colour_window_border,pe->colour_bg,x,17);

	posx=x,posy=y;




}
