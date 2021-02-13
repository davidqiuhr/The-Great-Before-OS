#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include<stdio.h>
void boxfillOnSht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,unsigned char c)
{
	int i,j;
	for (j=y0;j<y0+pysize;j++)
		for (i=x0;i<x0+pxsize;i++)
			sht->buffer[j*sht->xsize+i]=c;
}

void putfont8Onsht(struct Sheet *sht,int x,int y,char color,char c)
{
	int i,j;
	char tmp[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	char *font=fontlib+c*16;
	for (i=0;i<16;i++)
		for (j=0;j<8;j++)
			if ((font[i]&tmp[j])!=0)
				sht->buffer[(y+i)*sht->xsize+x+j]=color;
}
void putStrOnSht(struct Sheet *sht,int x,int y,char color,char *str)
{
	char *t=str;
	while (*t!=0x00)
	{
		putfont8Onsht(sht,x,y,color,*t);
		x+=8;
		t++;
	}
}
void putStrAndBackOnSht(struct Sheet *sht,int x,int y,int color,int backcolor,char *str,int length)
{
	boxfillOnSht(sht,x,y,length*8,16,backcolor);
	putStrOnSht(sht,x,y,color,str);
	refreshSubInSheet(sht,x,y,length*8,16);
}
void initMouseCursor(struct Sheet *sht)
{
	//�������ͼ
	static char cursor[18][18] = {
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO",
		"OOOOOOOOOOOOOOOOOO"
	};
	//�˴��޷���⣬ǰ����Ϊ���޷���ʾ��������
	for(int i=0;i<16;i++)
		for (int j=0;j<16;j++)
			switch(cursor[i+2][j])
			{
				case '*':
					sht->buffer[i*16+j]=BLACK;
					break;
				case 'O':
					sht->buffer[i*16+j]=WHITE;
					break;
				default:
					sht->buffer[i*16+j]=sht->col_inv;
			}
}

void putBlockOnSht(struct Sheet *sht,int x0,int y0,int pxsize,int pysize,char *block)
{
	for (int y=0;y<pysize;y++)
		for (int x=0;x<pxsize;x++)
			if (block[y*pxsize+x]>=0)
				sht->buffer[(y0+y)*sht->xsize+x0+x]=block[y*pxsize+x];
}

void initPalette()
{	
	static unsigned char table_rgb[16][3] = 
	{
		{0x00, 0x00, 0x00},	//��
		{0xff, 0x00, 0x00},	//����
		{0x00, 0xff, 0x00},	//����
		{0xff, 0xff, 0x00},	//����
		{0x00, 0x00, 0xff},	//����
		{0xff, 0x00, 0xff},	//����
		{0x00, 0xff, 0xff},	//ǳ����
		{0xff, 0xff, 0xff},	//��
		{0xc6, 0xc6, 0xc6},	//����
		{0x84, 0x00, 0x00},	//����
		{0x00, 0x84, 0x00},	//����
		{0x84, 0x84, 0x00},	//����
		{0x00, 0x00, 0x84},	//����
		{0x84, 0x00, 0x84},	//����
		{0x00, 0x84, 0x84},	//ǳ����
		{0x84, 0x84, 0x84}	//����
	};
	setPalette(0, 15, table_rgb);
	return;
}

void setPalette(int start,int end,unsigned char rgb[16][3])
{
	int i,eflags;
	eflags=io_load_eflags();
	io_cli();
	io_out8(0x03c8,start);//��0�ŵ�ɫ�忪ʼ�趨
	
	for (i=start;i<=end;i++)
	{
		//��RGBд�룬��ɫ������Զ�����
		io_out8(0x03c9,rgb[i][0]/4);//��4��Ϊ�˸ı���ǳ
		io_out8(0x03c9,rgb[i][1]/4);
		io_out8(0x03c9,rgb[i][2]/4);
	}
	
	io_store_eflags(eflags);
}
void initScreenOnSht(struct Sheet *sht)
{
	//char *vram=sht->buffer;
	
	
	int x=sht->xsize; 
	int y=sht->ysize;
	
	//�������
	boxfillOnSht(sht,0,0,x,y-28,LIGHTBLUE);
	boxfillOnSht(sht,0,y-28,x,1,LIGHTGRAY);
	boxfillOnSht(sht,0,y-27,x,1,LIGHTGRAY);
	boxfillOnSht(sht,0,y-26,x,26,WHITE);

	boxfillOnSht(sht,3,y-24,57,1,WHITE);
	boxfillOnSht(sht,2,y-24,1,21,WHITE);
	boxfillOnSht(sht,3,y-4,57,1,DULLGRAY);
	boxfillOnSht(sht,59,y-23,1,19,DULLGRAY);
	boxfillOnSht(sht,2,y-3,58,1,BLACK);
	boxfillOnSht(sht,60,y-24,1,22,BLACK);

	boxfillOnSht(sht,x-47,y-24,44,1,DULLGRAY);
	boxfillOnSht(sht,x-47,y-23,1,20,DULLGRAY);
	boxfillOnSht(sht,x-47,y-3,44,1,WHITE);
	boxfillOnSht(sht,x-3,y-24,1,22,WHITE);
	return;
}

void putfont8OnSrn(struct BootInfo *binfo,int x,int y,char color,char c)
{
	int i,j;
	char tmp[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	char *font=fontlib+c*16;
	for (i=0;i<16;i++)
		for (j=0;j<8;j++)
			if ((font[i]&tmp[j])!=0)
				binfo->vram[(y+i)*binfo->scrnx+x+j]=color;
				
}
void putStrOnSrn(struct BootInfo *binfo,int x,int y,char color,char *str)
{
	char *t=str;
	while (*t!=0x00)
	{
		putfont8OnSrn(binfo,x,y,color,*t);
		x+=8;
		t++;
	}
}
void makeWindow(struct Sheet *sht,int xsize,int ysize,char *title)
{
	//boxfillOnSht(sht,0,0,xsize,ysize,LIGHTRED);
	static char closebtn[11][11] =
		{
		"OOOOOOOOOOOOO",
		"OOOO@OOOO@OOO",
		"OOOO@@OO@@OOO",
		"OOOOO@@@@OOOO",
		"OOOOOO@@OOOOO",
		"OOOOO@@@@OOOO",
		"OOOO@@OO@@OOO",
		"OOOO@OOOO@OOO",
		"OOOOOOOOOOOOO",
		};
	int x, y;
	char c;
	boxfillOnSht(sht, 0,         0,         xsize ,		1, 			LIGHTGRAY);
	boxfillOnSht(sht, 1,         1,         xsize - 2, 	1, 			WHITE);
	boxfillOnSht(sht, 0,         0,         1,     		ysize , 	LIGHTGRAY);
	boxfillOnSht(sht, 1,         1,         1,        	ysize - 2, 	WHITE);
	boxfillOnSht(sht, xsize - 2, 1,         1,		 	ysize - 2, 	DULLGRAY);
	boxfillOnSht(sht, xsize - 1, 0,         1, 			ysize, 		BLACK);
	//if (title[11]=='2'){struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"b21");putStrOnSrn(binfo,0,320,1,s);}
	boxfillOnSht(sht, 2,         2,         xsize - 4, 	ysize - 4, 	WHITE);//�� 
	//if (title[11]=='2'){struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"b22");putStrOnSrn(binfo,0,340,1,s);}
	boxfillOnSht(sht, 3,         3,         xsize - 6, 	18, 		DULLINDIGO);//�� 
	//if (title[11]=='2'){struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"b27");putStrOnSrn(binfo,0,360,1,s);}
	boxfillOnSht(sht, 1,         ysize - 2, xsize - 2, 	1, 			DULLGRAY);
	//if (title[11]=='2'){struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"b29");putStrOnSrn(binfo,0,380,1,s);}
	boxfillOnSht(sht, 0,         ysize - 1, xsize, 		1,	 		BLACK);
	//if (title[11]=='2'){struct BootInfo *binfo=(struct BootInfo *) 0x00000ff0;char s[128];sprintf (s,"b3");putStrOnSrn(binfo,0,400,1,s);}
	putStrOnSht(sht,16, 4, WHITE, title);
	for (y = 0; y < 14; y++) 
	{
		for (x = 0; x < 16; x++) 
		{
			c = closebtn[y][x];
			if (c == '@') 
			{
				c = BLACK;
			} else if (c == '$') 
			{
				c = DULLGRAY;
			} else if (c == 'Q') 
			{
				c = LIGHTGRAY;
			} else 
			{
				c = WHITE;
			}
			sht->buffer[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
	return;
}

void makeTextBox(struct Sheet *sht, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	boxfillOnSht(sht, x0 - 2, y0 - 3, sx + 4, 1		, DULLGRAY);
	boxfillOnSht(sht, x0 - 3, y0 - 3, 1		, sy + 5, DULLGRAY);
	boxfillOnSht(sht, x0 - 3, y1 + 2, sx + 5, 1		, WHITE);
	boxfillOnSht(sht, x1 + 2, y0 - 3, 1		, sy + 6, WHITE);
	boxfillOnSht(sht, x0 - 1, y0 - 2, sx + 2, 1		, BLACK);
	boxfillOnSht(sht, x0 - 2, y0 - 2, 1		, sy + 3, BLACK);
	boxfillOnSht(sht, x0 - 2, y1 + 1, sx + 3, 1		, LIGHTGRAY);
	boxfillOnSht(sht, x1 + 1, y0 - 2, 1		, sy + 4, LIGHTGRAY);
	boxfillOnSht(sht, x0 - 1, y0 - 1, sx + 2, sy + 2, c);
	return;
}
