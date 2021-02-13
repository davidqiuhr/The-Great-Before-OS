#include"memory.h"
#include"nasmfunc.h" 
#include"sheet.h"
#include<stdio.h>
struct SheetControl *scl;
struct SheetControl* initSCL(struct BootInfo *binfo)
{
	int i;
	scl=(struct SheetControl *)allocMem(sizeof(struct SheetControl),"Sheet Control");
	scl->vram=binfo->vram;
	scl->xsize=binfo->scrnx;
	scl->ysize=binfo->scrny;
	scl->top=-1;//û��ͼ�� 
	for (i=0;i<MAXSHEET;i++)
		scl->sheet[i].flags=SHEETFREE;//���Ϊδʹ��
	return scl; 
}
struct Sheet *allocSheet()
{
	struct Sheet *sht;
	int i;
	//������е�ͼ�� 
	for (i=0;i<MAXSHEET;i++)
		if (scl->sheet[i].flags==SHEETFREE)
		{
			sht=&scl->sheet[i];
			sht->flags=SHEETUSE;
			sht->height=-1;//-1��ʾ����ʾ 
			return sht;
		}
	return 0;
}
//����ͼ�� 
void setBufInSheet(struct Sheet *sht,unsigned char *buf,int xsize,int ysize,int col_inv)
{
	sht->buffer=buf;
	sht->xsize=xsize; 
	sht->ysize=ysize;
	sht->col_inv=col_inv;
}

//����ͼ��߶�
void setHeightSheet(struct Sheet *sht,int height) 
{
	sht->height=height;
	if (scl->top<height)
		scl->top=height;
	if (height>=0)
		scl->sheetp[height]=sht;
	//refreshAllSheet();
	refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,0);
}

//����ͼ�� ���޸ĸ߶ȣ� 
void updownSheet(struct Sheet *sht,int height)
{
	int old=sht->height,i;
	//����������ֵ 
	if (height>=scl->top+1)
		height=scl->top+1;
	else if (height<-1)
		height=-1;
	//�޸�ĳһͼ��߶���4����� 
	sht->height=height;
	if (old>height && height>=0)
	{
		//����ͼ������Ȼ��ʾ ,���м�ͼ�������ᣬ���޸�ͼ����и߶� 
		for (i=old;i>height;i--)
		{
			scl->sheetp[i]=scl->sheetp[i-1];
			scl->sheetp[i]->height=i;
		}
		scl->sheetp[height]=sht;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,height+1);
	}else if (old>height && height<0)
	{
		//����ͼ��,������ͼ��������,���޸�ͼ����и߶� 
		for (i=old;i<scl->top;i++)
		{ 
			scl->sheetp[i]=scl->sheetp[i+1];
			scl->sheetp[i]->height=i;
		}
		scl->top--;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,0);
	}else if (old <height && old>=0)
	{
		//���ͼ����ԭͼ������ʾ״̬ ,���м�ͼ�������������޸�ͼ����и߶� 
		for (i=old;i<height;i++)
		{
			scl->sheetp[i]=scl->sheetp[i+1];
			scl->sheetp[i]->height=i;
		}
		scl->sheetp[height]=sht;
	}else if (old<height && old<0)
	{
		//��ʾ���ص�ͼ�� ,������ͼ�������ᣬ���޸�ͼ����и߶� 
		for (i=scl->top+1;i>height;i--)
		{
			scl->sheetp[i]=scl->sheetp[i-1];
			scl->sheetp[i]->height=i;
			
		}
		scl->sheetp[height]=sht;
		scl->top++;
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,height);
	}
}
//�ƶ�ͼ�� 
void slideSheet(struct Sheet* sht,int vx0,int vy0)
{
	int old_x0=sht->x0,old_y0=sht->y0;
	sht->x0=vx0;
	sht->y0=vy0;
	if (sht->height>=0)//������ʾ 
	{
		//�������Ż�ֱ��ˢ��������Ļ
		//refreshAllSheet(scl);
		//�����Ż� ��ֻˢ���б䶯�������ƶ�ǰλ�����ƶ���λ�ã� 
		//ԭλ����Ҫȫ�����»��� 
		refreshSubSheet(old_x0,old_y0,sht->xsize,sht->ysize,0);
		//��λ��ֻ�ô�Ŀ��ͼ�㿪ʼ���� 
		refreshSubSheet(vx0,vy0,sht->xsize,sht->ysize,sht->height);
	}
	return;
}
//�ͷ�ͼ�� 
void freeSheet(struct Sheet* sht)
{
	//���ظ�ͼ�㲢��δʹ�ñ�־
	if (sht->height>=0) 
		updownSheet(sht,-1);
	//setHeightSheet(sht,-1);
	sht->flags=0;
	return;
}
//ˢ��һ������Ļ (x0,y0)�������Ļ���� 
void refreshSubSheet(int x0,int y0,int pxsize,int pysize,int h0)
{
	int h,vx,vy,sx,sy;//(vx,vy) ��ʾ���������е����� (sx,sy)��ʾͼ���е�����
	int vx0,vx1,vy0,vy1;
	unsigned char *buf,c,*vram=scl->vram;
	struct Sheet *sht;
	if (x0<0) 
		x0=0;
	if (x0+pxsize>scl->xsize)
		pxsize=scl->xsize-x0;
	if (y0<0) 
		y0=0;
	if (y0+pysize>scl->ysize)
		pysize=scl->ysize-y0;
	//�ӵ͵��߻���ͼ�� 
	for (h=h0;h<=scl->top;h++)
	{
		sht=scl->sheetp[h];
		buf=sht->buffer;
		/*
		vx0~vx1,vy0~vy1	��ͼ�����������Ľ���  
		*/
		vy0=y0>sht->y0?y0:sht->y0;
		vy1=y0+pysize<sht->y0+sht->ysize?y0+pysize:sht->y0+sht->ysize;
		vx0=x0>sht->x0?x0:sht->x0;
		vx1=x0+pxsize<sht->x0+sht->xsize?x0+pxsize:sht->x0+sht->xsize;
		for (vy=vy0;vy<vy1;vy++)
			for (vx=vx0;vx<vx1;vx++)
			{
				sy=vy-sht->y0;
				sx=vx-sht->x0;
				c=buf[sy*sht->xsize+sx];
				//�����ز���͸�� 
				if (c!=sht->col_inv)
					vram[vy*scl->xsize+vx]=c;
			}
	}
	
	//���ͼ�� 
	sht=&scl->sheet[1];
	buf=sht->buffer;
	vy0=y0>sht->y0?y0:sht->y0;
	vy1=y0+pysize<sht->y0+sht->ysize?y0+pysize:sht->y0+sht->ysize;
	vx0=x0>sht->x0?x0:sht->x0;
	vx1=x0+pxsize<sht->x0+sht->xsize?x0+pxsize:sht->x0+sht->xsize;
	for (vy=vy0;vy<vy1;vy++)
		for (vx=vx0;vx<vx1;vx++)
		{
			sy=vy-sht->y0;
			sx=vx-sht->x0;
			c=buf[sy*sht->xsize+sx];
			//�����ز���͸�� 
			if (c!=sht->col_inv)
				vram[vy*scl->xsize+vx]=c;
		}
}
//��ˢ��������Ļ 
void refreshAllSheet()
{
	int h,sx,sy,vx,vy;//(sx,sy)��ʾ����ͼ���е����� (vx,vy) ��ʾ���������е����� 
	unsigned char *buf,c,*vram=scl->vram;
	struct Sheet *sht;
	
	//�ӵ͵��߻���ͼ�� 
	for (h=0;h<=scl->top;h++)
	{
		sht=scl->sheetp[h];
		buf=sht->buffer;
		for (sy=0;sy<sht->ysize;sy++)
		{
			vy=sht->y0+sy;
			for (sx=0;sx<sht->xsize;sx++)
			{
				vx=sht->x0+sx;
				c=buf[sy*sht->xsize+sx];
				//�����ز���͸�� 
				if (c!=sht->col_inv)
					vram[vy*scl->xsize+vx]=c;
			}
		}
	}
	
	//���ͼ�� 
	sht=&scl->sheet[1];
	buf=sht->buffer;
	for (sy=0;sy<sht->ysize;sy++)
	{
		vy=sht->y0+sy;
		for (sx=0;sx<sht->xsize;sx++)
		{
			vx=sht->x0+sx;
			c=buf[sy*sht->xsize+sx];
			//�����ز���͸�� 
			if (c!=sht->col_inv)
				vram[vy*scl->xsize+vx]=c;
		}
	}
}
//ˢ��һ��ͼ��
void refreshSheet(struct Sheet *sht) 
{
	//���ͼ������ʾ����ˢ��ͼ���������� 
	if (sht->height>=0)
		refreshSubSheet(sht->x0,sht->y0,sht->xsize,sht->ysize,sht->height);
}
//ˢ��һ��ͼ���һ���֣����ͼ���λ�ã� 
void refreshSubInSheet(struct Sheet *sht,int x0,int y0,int xsize,int ysize) 
{
	//���ͼ������ʾ����ˢ��ͼ�����������һ���� 
	if (sht->height>=0)
		refreshSubSheet(sht->x0+x0,sht->y0+y0,xsize,ysize,sht->height);	
}
