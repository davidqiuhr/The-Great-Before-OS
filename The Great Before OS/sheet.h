#pragma once
#define MAXSHEET 256 
#define SHEETFREE 0
#define SHEETUSE 1

struct Sheet
{
	unsigned char *buffer;//����
	int x0,y0,xsize,ysize;//��ʼ���꣬����
	int col_inv,height,flags; //͸��ɫɫ�š�ͼ��߶ȡ�������Ϣ 
};
struct SheetControl
{
	unsigned char *vram;
	int xsize,ysize,top;//top��ʾ���ϲ�ͼ��ĸ߶� 
	struct Sheet *sheetp[MAXSHEET];//ͼ��ĵ�ַ������������� 
	struct Sheet sheet[MAXSHEET];//����ͼ�� 
};
extern struct SheetControl *scl;
/*extern struct BootInfo *binfo;
extern struct MemoryList *meml;*/
struct SheetControl* initSCL(struct BootInfo *binfo);
struct Sheet *allocSheet();
void setBufInSheet(struct Sheet *sht,unsigned char *buf,int xsize,int ysize,int col_inv);
void setHeightSheet(struct Sheet *sht,int height);
void updownSheet(struct Sheet *sht,int height);
void slideSheet(struct Sheet* sht,int vx0,int vy0);
void freeSheet(struct Sheet* sht);
void refreshSubSheet(int x0,int y0,int pxsize,int pysize,int h0);
void refreshAllSheet();
void refreshSheet(struct Sheet *sht);
void refreshSubInSheet(struct Sheet *sht,int x0,int y0,int xsize,int ysize);
