#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"mouse.h"
#include"tasklist.h"
#include<stdio.h>
void showTask(struct Sheet *taskListSheet)
{
	int curPosY=0;
	makeWindow(taskListSheet,270,340,"Task List");
	char str[70]=" ID       Name       Status";
	putStrAndBackOnSht(taskListSheet,18,32+curPosY*30,BLACK,WHITE,str,28);
	curPosY=1;
	//���������⻯���� 
	sprintf (str,"  1      System      Running");
	putStrAndBackOnSht(taskListSheet,18,32+curPosY*30,BLACK,WHITE,str,28);
	curPosY++; 
	for (int i=1;i<taskctl->runningCount;i++)
	{
		sprintf (str,"  %d",taskctl->tasks[i]->winID+2);
		int j=0,k=0; 
		while (window.winName[taskctl->tasks[i]->winID][j]!='\0')
			j++;
		for (k=0;k<9-j/2;k++)
			sprintf (str,"%s ",str);
		sprintf (str,"%s%s",str,window.winName[taskctl->tasks[i]->winID]);//winID�������������ҳ�ʼΪ0 
		for (k=0;k<9-(j+1)/2;k++)
			sprintf (str,"%s ",str);
		sprintf (str,"%sRunning",str);//winID�������������ҳ�ʼΪ0 
		putStrAndBackOnSht(taskListSheet,18,32+curPosY*30,BLACK,WHITE,str,28);
		curPosY++;
	}
}

void taskListTask_Main(struct Task *task)
{
	//��ʼ�������� 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);
	//��ʼ����ʱ�� 
	struct Timer *timerCur;
	timerCur=allocTimer();
	initTimer(timerCur,&bufferTime,1);
	setTimer(timerCur,100);
	
	
	//��ʾ����
	struct Sheet *taskListSheet;
	unsigned char *taskListBuffer;
	taskListSheet=allocSheet();
	slideSheet(taskListSheet,200,140);
	taskListBuffer=(unsigned char *)allocMem(270*340,"Task List UI");//�����ڴ�ռ� 
	setBufInSheet(taskListSheet,taskListBuffer,270,340,-1);
	makeWindow(taskListSheet,270,310,"Task List");
	setHeightSheet(taskListSheet,task->winID+1);
	
	//��ʾ��Ϣ 
	showTask(taskListSheet);
	refreshSheet(taskListSheet);
	unsigned char data;
	int flag=0;
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
					slideSheet(taskListSheet,mdec.x,mdec.y);
					break;
				//�Ҽ��ر�
				case 2:
					freeSheet(taskListSheet);
					freeMem((unsigned int)taskListBuffer,270*340);
					deleteWindow(task);
					deleteTask(task);
					break;
			}
		}
		//��ʱˢ�� 
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 1:
				initTimer(timerCur,&bufferTime,1);
				//��ʾ��Ϣ 
				showTask(taskListSheet);
				refreshSheet(taskListSheet);
				setTimer(timerCur,100);
				break;
			}
		} 
		if (flag==0)
			io_sti();
	}
} 
