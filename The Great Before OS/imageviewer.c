#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"imageviewer.h" 
#include<stdio.h> 
#include<string.h> 
void loadImageFile (char *fileName,struct Sheet *imageViewerSheet)
{
	if (strcmp(fileName,"\\Image\\Pic1")==0)
		for (int i=1;i<7;i++)
			for (int j=i*30;j<i*30+30;j++)
				for (int k=5;k<295;k++)
					imageViewerSheet->buffer[j*300+k]=i;
	refreshSheet(imageViewerSheet);
}
void imageViewerTask_Main(struct Task *task)
{ 
	//��ʼ�������� 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);
	
	//��ʾ����
	struct Sheet *imageViewerSheet;
	unsigned char *imageViewerBuffer;
	imageViewerSheet=allocSheet();
	slideSheet(imageViewerSheet,500,120);
	imageViewerBuffer=(unsigned char *)allocMem(300*240,"Image Viewer UI");//�����ڴ�ռ� 
	setBufInSheet(imageViewerSheet,imageViewerBuffer,300,240,-1);
	makeWindow(imageViewerSheet,300,240,"Image Viewer");
	setHeightSheet(imageViewerSheet,task->winID+1);
	
	//��ʾ��Ϣ 
	refreshSheet(imageViewerSheet);
	unsigned char data;
	int flag=0;
	loadImageFile(task->par[0],imageViewerSheet);
	while (1)
	{
		flag=0;
		io_cli();
		if (getBuffer(&task->bufAll.mouse,&data))
		{
			io_sti();
			flag=2;
			switch(data)
			{
				//����ƶ� 
				case 0:
					slideSheet(imageViewerSheet,mdec.x,mdec.y);
					break;
				//�Ҽ��ر�
				case 2:
					freeSheet(imageViewerSheet);
					freeMem((unsigned int)imageViewerBuffer,300*240);
					deleteWindow(task);
					deleteTask(task);
					break;
			}
		}
		if (flag==0)
			io_sti();
	}
} 
