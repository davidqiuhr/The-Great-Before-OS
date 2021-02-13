#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"dsctbl.h"  
#include"buffer.h"
#include"timer.h" 
#include"keyboard.h"
#include"mouse.h"
#include"mtask.h"
#include"int.h"
#include"console.h" 
#include"calculator.h" 
#include"main.h"
#include<stdio.h>
void HariMain()
{	
	//�жϳ�ʼ�� 
	init_gdtidt();
	init_pic();
	io_sti();
	io_out8(PIC0_IMR, 0xf8);//����PIC�ж�������ж�
	io_out8(PIC1_IMR, 0xef);//��������ж�
	
	//��ʼ����ꡢ���̡���ʱ������������������󻺳���������ջ�У������񻺳嶨���ڶ��У� 
	unsigned char data;
	unsigned char keyb[32],mouseb[1024],timeb[8];
	struct Buffer timeBuffer;
	initBuffer(&allBuf.key,32,keyb);
	initBuffer(&allBuf.mouse,1024,mouseb);
	initBuffer(&timeBuffer,8,timeb);
	
	//��ʱ����ʼ��
	initPit();
	//�趨��ʱ�� 
	struct Timer *timer1;//timer1Ϊ�����˸��ʱ��,timer2Ϊ����ʱ��ʱ�� 
	timer1=allocTimer();
	initTimer(timer1,&timeBuffer,10);
	setTimer(timer1,50);
	
	//��ʼ����Ļ 
	struct BootInfo *binfo=(struct BootInfo *) BOOTADDR;//��Ļ����ͼ�񻺳�����ʼλ��
	initPalette();//�趨��ɫ�� 
	putStrOnSrn(binfo,0,20,LIGHTRED,"Welcome To my OS");
	putStrOnSrn(binfo,0,50,LIGHTRED,"Check the memory now...");
	
	//���->�����Ƶ�·->CPU
	//�ȳ�ʼ�������Ƶ�·�������ڼ��̿��Ƶ�·�
	//�ټ������
	mdec.x=100;
	mdec.y=100;
	init_keyboard();//�ȳ�ʼ�����̿��Ƶ�·
	enable_mouse(&mdec);//�ټ������
	


	
	//��ʼ���ڴ�
	//struct MemoryList *
	meml=(struct MemoryList *)MEMORYLISTADDR;
	int size;
	initMem();
	meml->maxsize=0x7fffffff; 
	size=memtest(0x00400000,0xbfffffff);//�ڴ���� 
	freeMem(0x00001000,0x0009e000);
	freeMem(0x00400000,size-0x00400000);
	meml->used[0].addr=0x9e000;
	meml->used[0].size=0x00400000-0x9e000;
	meml->used[0].flag=1;
	sprintf (meml->used[0].status,"System");
	
	//��ʼ����������
	char keyTable[0x160]=
	{
		 0 , 0 ,'1','2','3','4','5','6','7','8','9','0','-','=', 0 , 0 ,
		'q','w','e','r','t','y','u','i','o','p','[',']', 0 , 0 ,'a','s',
		'd','f','g','h','j','k','l',';','\'','`', 0 ,'\\','z','x','c','v',
		'b','n','m',',','.','/', 0 ,'*', 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'7','8','9','-','4','5','6','+','1',
		'2','3','0','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 ,'_', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'\\', 0 , 0 ,
 		 0 , 0 ,'!','@','#','$','%','^','&','*','(',')','_','+', 0 , 0 ,
		'Q','W','E','R','T','Y','U','I','O','P','[',']', 0 , 0 ,'A','S',
		'D','F','G','H','J','K','L',';','|','~', 0 ,'\\','Z','X','C','V',
		'B','N','M',',','.','/', 0 ,'*', 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'7','8','9','_','4','5','6','+','1',
		'2','3','0','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
 		 0 , 0 ,'_', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'\\', 0 , 0
	};
	for (int i=0x00;i<0x160;i++)
		keyboard.keyTable[i]=keyTable[i];
	char curInput[128];
	int curPos = 0;

	//��ʼ��ͼ���
	scl=initSCL(binfo);
	sprintf (meml->used[1].status,"Sheet Control");

	//��ʼ������ͼ��
	struct Sheet *sht_back;
	unsigned char *buf_back;
	sht_back=allocSheet();//����ͼ�� 
	buf_back=(unsigned char *)allocMem_4k(binfo->scrnx*binfo->scrny,"Background UI");//�����ڴ�ռ� 
	setBufInSheet(sht_back,buf_back,binfo->scrnx,binfo->scrny,-1);//��ʼ��ͼ�㣬������͸��ɫ 
	initScreenOnSht(sht_back);//��ͼ��
	slideSheet(sht_back,0,0);//�ƶ�ͼ��λ��
	updownSheet(sht_back,0);
	
	//��ʼ�����ͼ��
	struct Sheet *sht_mouse;
	char mousebuf[256];
	sht_mouse=allocSheet();
	setBufInSheet(sht_mouse,mousebuf,16,16,99);
	initMouseCursor(sht_mouse);
	slideSheet(sht_mouse, mdec.x, mdec.y);
	updownSheet(sht_mouse,1); 
	
	//��ʾ������Ϣ 
	char str[128]; 
	//��ʾ�ڴ��С
	unsigned int u=meml->maxsize/1024/1024;
	sprintf (str,"Memory: %dM",u);
	putStrAndBackOnSht(sht_back,0,0,LIGHTRED,LIGHTGRAY,str,-1);
	//��ʾFocus����
	sprintf (str,"Fouse: Console");
	putStrAndBackOnSht(sht_back,0,1*16,LIGHTRED,LIGHTBLUE,str,40);
	refreshAllSheet();//����ͼ��׼�� 
	
	//��ʼ��������
	initTaskCTL();
	
	//�趨������
	struct Task *mainTask;
	mainTask=getMainTask();
	
	//��ʼ�����㣬��ʼ����ΪConsole���� 
	initWindow();
	
	//����Console����
	struct Task *consoleTask;
	consoleTask=allocTask();
	initTask(consoleTask,(int)&consoleTask_Main,"Console",11);
	createWindow(consoleTask,"Console");
	runTask(consoleTask);
	
	//����calculator����
	/*struct Task *calculatorTask;
	calculatorTask=allocTask();
	window.winCount++;
	initTask(calculatorTask,(int)&calculatorTask_Main,window.winCount);
	sprintf (window.winName[window.winCount],"Calculator");
	runTask(calculatorTask);*/ 
	
	int flag;
	keyboard.isShift=0;
	//�����ѭ�� 
	while(1)
	{
		flag=0;
		if (window.isChanged)
		{
			sprintf (str,"Focus: %s, winCount: %d, runningCount: %d",window.winName[window.focus],window.winCount,taskctl->runningCount);
			putStrAndBackOnSht(sht_back,0,1*16,LIGHTRED,LIGHTBLUE,str,60);
			window.isChanged=0;
		}
		//�������ж� 
		io_cli();
		if (getBuffer(&allBuf.key,&data))
		{
			//���� 
			io_sti();
			flag=1;
			sprintf (str,"Key: %x",data);
			putStrAndBackOnSht(sht_back,0,2*16,LIGHTRED,LIGHTBLUE,str,26);//��ʾ������Ϣ 
			//ִ�м��̶��� 
			switch (data)
			{
				case 0x0f://Tab��
					window.focus++; 
					window.focus%=window.winCount;
					window.isChanged=1;
					break;
				case 0x2a:
				case 0x36://Shift��ס 
					keyboard.isShift=1;
					break;
				case 0xaa:
					case 0xb6://Shift�ɿ� 
					keyboard.isShift=0;
					break;
				default://ʣ����̶��� ���������� 
					putBuffer(&taskctl->tasks[1+window.focus]->bufAll.key,data);//������û�д��ڣ��ʴ��ں�+1=����� 
					break;
			}
			/*if (window.focus==0)
			{
				if (data<0x80 && keyTable[data]>0 && curPos<=15)//��ĸ������ 
				{
					curInput[curPos++] = keyTable[data];
					curInput[curPos] = '\0';
				}else switch (data)
				{
					case 0x0e://�˸�� 
						if (curPos>0)
							curInput[--curPos] = '\0';
						break; 
					case 0x0f://Tab��
						window.focus++; 
						window.focus%=window.winCount;
						break;
				}
				sprintf (str,"%s",curInput);
				putStrAndBackOnSht(sht_window,16,44,BLACK,WHITE,str,16);
			}*/
			
		}
		if (getBuffer(&allBuf.mouse,&data))
		{
			//��� 
			io_sti();
			flag=2;
			if (mouse_decode(&mdec,data))
			{
				mdec.x+=mdec.dx;
				if (mdec.x<0)
					mdec.x=0;
				if (mdec.x>binfo->scrnx-1)
					mdec.x=binfo->scrnx-1;
				mdec.y+=mdec.dy;
				if (mdec.y<0)
					mdec.y=0;
				if (mdec.y>binfo->scrny-1)
					mdec.y=binfo->scrny-1;
				
				
				sprintf (str,"Mouse: ");
				if (mdec.lbtn)
					sprintf (str,"%sL",str);
				if (mdec.mbtn)  
					sprintf (str,"%sM",str);				
				if (mdec.rbtn)
					sprintf (str,"%sR",str);
				sprintf (str,"%s (%d,%d)",str,mdec.x,mdec.y);
				putStrAndBackOnSht(sht_back,0,3*16,LIGHTRED,LIGHTBLUE,str,25);//��ʾ�����Ϣ 
				slideSheet(sht_mouse,mdec.x,mdec.y);
				
				//ʣ����궯������������ 
				//������û�д��ڣ���ͼ���+1=���ں�+1=����� 
				if (mdec.lbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,0);
				else if (mdec.mbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,1);
				else if (mdec.rbtn)
					putBuffer(&taskctl->tasks[window.focus+1]->bufAll.mouse,2);
				
			}
		}
		/*if (getBuffer(&timeBuffer,&data))
		{
			//��ʱ�� 
			io_sti();
			flag=3;
			switch (data)//Ϊ�����ֲ�ͬ�Ķ�ʱ�� 
			{
			case 10://timer1��ʾ��� 
				initTimer(timer1,&timeBuffer,11);
				boxfillOnSht(sht_window,16+8*curPos,44,8,15,BLACK);
				refreshSubInSheet(sht_window,16+8*curPos,44,8,15);   
				setTimer(timer1,50);
				break;
			case 11://timer1���ع�� 
				initTimer(timer1,&timeBuffer,10);
				boxfillOnSht(sht_window,16+8*curPos,44,8,15,WHITE);
				refreshSubInSheet(sht_window,16+8*curPos,44,8,15);
				setTimer(timer1,50);
				break; 
			}
		}*/
		if (flag==0)
		{
			//û���¼������� 
			//sleepTask(mainTask);
			io_sti();
		}
	}
}



/* 
//����calculator 
void calculatorTask_Main(struct Task *task)
{
	//��ʼ�������� 
	char bufferArray[128];
	struct Buffer bufferTime;
	initBuffer(&bufferTime,128,bufferArray);

	//��ʼ����ʱ�� 
	struct Timer *timerCur;
	timerCur=allocTimer();
	initTimer(timerCur,&bufferTime,1);
	setTimer(timerCur,50);
	
	//��ʼ������̨ͼ��
	struct Sheet *consoleSheet;
	unsigned char *consoleBuffer;	
	consoleBuffer=(unsigned char *)allocMem_4k(512*310);
	consoleSheet=allocSheet();
	setBufInSheet(consoleSheet,consoleBuffer,512,310,-1);//û��͸��ɫ
	slideSheet(consoleSheet,202,8);
	makeWindow(consoleSheet,512,310,"Calculator");
	makeTextBox(consoleSheet,8,27,496,276,BLACK);
	setHeightSheet(consoleSheet,1);
	
	unsigned char data;
	int curPos=0;
	int flag=0;
	while (1)
	{
		flag=0;
		if (window.focus!=task->winID)//���㲻�ڣ�ȡ����� 
		{
			boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,BLACK);
			refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15); 
			continue;
		}else if (timerCur->flag==TIMER_ALLOCED)//���»�ý��㣬������� 
		{
			initTimer(timerCur,&bufferTime,1);
			setTimer(timerCur,50);
		}
		io_cli();
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 0:
				initTimer(timerCur,&bufferTime,1);
				boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,WHITE);
				refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15);   
				setTimer(timerCur,50);
				break;
			case 1:
				initTimer(timerCur,&bufferTime,0);
				boxfillOnSht(consoleSheet,8+8*curPos,28,8,15,BLACK);
				refreshSubInSheet(consoleSheet,8+8*curPos,28,8,15);
				setTimer(timerCur,50);
				break; 
			}
		} 
		if (flag==0)
		{
			io_sti();
		} 
	}
}*/ 


