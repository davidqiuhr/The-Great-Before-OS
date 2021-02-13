#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"textviewer.h" 
#include<stdio.h>
#include<string.h> 
void loadTextFile (char *fileName,struct Sheet *textViewerSheet,int* curPosX,int* curPosY,char curInput[21][75],int page)
{
	(*curPosX)=0;
	(*curPosY)=0;
	for (int i=0;i<21;i++)
		sprintf (curInput[i],"");
	if (strcmp(fileName,"\\Text\\Welcome")==0)
	{
		sprintf (curInput[(*curPosY)++],"                             Welcome to The Great Before OS");
		sprintf (curInput[(*curPosY)++],"    This is The Great Before OS which can run on bare computer.");
		sprintf (curInput[(*curPosY)++],"    The Great Before OS support visualization,multitasking and the management of memory,");
		sprintf (curInput[(*curPosY)++],"process and file.The detail of bottom layout includes start sector,system ");
		sprintf (curInput[(*curPosY)++],"boot,protected mode,graphics,pallet,interrupt,segmentation,PCB and file.Th");
		sprintf (curInput[(*curPosY)++],"e detail of upper layout includes buffer,multitask and communication,mouse");
		sprintf (curInput[(*curPosY)++]," and keyboard's driver,typeface,timer,sheet,window and focus,and API.The a");
		sprintf (curInput[(*curPosY)++],"pplication on this OS includes calculator,drawing,music player,note,task m");
		sprintf (curInput[(*curPosY)++],"anager,memory manager and file manager.");
		sprintf (curInput[(*curPosY)++],"    The management of memory uses free table and first fit algorithm,inclu");
		sprintf (curInput[(*curPosY)++],"ding memory test,memory alloc and auto free.The management of memory uses ");
		sprintf (curInput[(*curPosY)++],"round robin algorithm,including task alloc,shift,sleep and communication b");
		sprintf (curInput[(*curPosY)++],"etween tasks.The file system based on FAT,including text file,image file,m");
		sprintf (curInput[(*curPosY)++],"Music file and menu.");
	}else if (strcmp(fileName,"\\Music\\Mus1")==0)
	{
		sprintf (curInput[(*curPosY)++],"CCGGAAG.FFDDEEC.GGFFEED.GGFFEED.CCGGAAG.FFDDEEC... ");
	}else if (strcmp(fileName,"\\Source\\Play.c")==0 && page==0)
	{
		sprintf (curInput[(*curPosY)++],"#include\"system.h\"");
    	sprintf (curInput[(*curPosY)++],"#include\"io.h\"");
		sprintf (curInput[(*curPosY)++],"#include\"graphic.h\"");
		sprintf (curInput[(*curPosY)++],"#include\"window.h\"");
		sprintf (curInput[(*curPosY)++],"#include\"buffer.h\"");
		sprintf (curInput[(*curPosY)++],"#include\"memory.h\"");
		sprintf (curInput[(*curPosY)++],"#include\"file.h\"");
		sprintf (curInput[(*curPosY)++],"#include<stdio.h>");
		sprintf (curInput[(*curPosY)++],"int main(int argc,char **argv)");
		sprintf (curInput[(*curPosY)++],"{");
		sprintf (curInput[(*curPosY)++],"    if (argc!=1)"); 
		sprintf (curInput[(*curPosY)++],"        return 0;");
		sprintf (curInput[(*curPosY)++],"    char *fileName=argv[0];");
		sprintf (curInput[(*curPosY)++],"    //Open File");
		sprintf (curInput[(*curPosY)++],"    struct File *fp;");
		sprintf (curInput[(*curPosY)++],"    if ((fp=fileOpen(fileName))==0)");
		sprintf (curInput[(*curPosY)++],"        return 0;");
		sprintf (curInput[(*curPosY)++],"    int length=fileLength(fp);");
		sprintf (curInput[(*curPosY)++],"    //File Buffer");
		sprintf (curInput[(*curPosY)++],"    unsigned char *fileBufC = (unsigned char *)allocMem(length,\"File\");");
		sprintf (curInput[(*curPosY)++],"    struct Buffer fileBuffer;");
	}else if (strcmp(fileName,"\\Source\\Play.c")==0 && page==1)
	{ 
		sprintf (curInput[(*curPosY)++],"    initBuffer(&fileBuffer,length,fileBufC);"); 
		sprintf (curInput[(*curPosY)++],"    readFile(fp,0,length,fileBuffer); ");
		sprintf (curInput[(*curPosY)++],"");
		sprintf (curInput[(*curPosY)++],"    //Sheet Buffer");
		sprintf (curInput[(*curPosY)++],"    unsigned char *playerBufC=(unsigned char *)allocMem(70*28,\"Player\");");
		sprintf (curInput[(*curPosY)++],"    struct Buffer fileBuffer;");
		sprintf (curInput[(*curPosY)++],"    initBuffer(playerBufC,70*28,fileBuffer);");
		sprintf (curInput[(*curPosY)++],"    //Alloc Sheet");
		sprintf (curInput[(*curPosY)++],"    struct Sheet *playerSheet;");
		sprintf (curInput[(*curPosY)++],"    playerSheet=allocSheet();");
		sprintf (curInput[(*curPosY)++],"    setBufInSheet(playerSheet,playerBufC,70,28,-1);");
		sprintf (curInput[(*curPosY)++],"    //Init Sheet");
		sprintf (curInput[(*curPosY)++],"    makeWindow(playerSheet,70,28,\"Player\");");
		sprintf (curInput[(*curPosY)++],"    slideSheet(playerSheet,180,72);");
		sprintf (curInput[(*curPosY)++],"    updownSheet(playerSheet);");
		sprintf (curInput[(*curPosY)++],"    char s[8],data;");
		sprintf (curInput[(*curPosY)++],"    sprintf (s,\"Playing\");");
		sprintf (curInput[(*curPosY)++],"    putStrOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);");
		sprintf (curInput[(*curPosY)++],"");
		sprintf (curInput[(*curPosY)++],"    //Init Timer");
		sprintf (curInput[(*curPosY)++],"    struct Timer *timerPlayer;");
	}else if (strcmp(fileName,"\\Source\\Play.c")==0 && page==2)
	{
		sprintf (curInput[(*curPosY)++],"    char bufferTime;");
		sprintf (curInput[(*curPosY)++],"    timerPlayer=allocTimer();");
		sprintf (curInput[(*curPosY)++],"    initTimer(timerPlayer,&bufferTime,1);");
		sprintf (curInput[(*curPosY)++],"");
		sprintf (curInput[(*curPosY)++],"    char oneBeat;");
		sprintf (curInput[(*curPosY)++],"    while (1)");
		sprintf (curInput[(*curPosY)++],"    {");
		sprintf (curInput[(*curPosY)++],"        for (int i=0;i<length;i++)");
		sprintf (curInput[(*curPosY)++],"        {");
		sprintf (curInput[(*curPosY)++],"            //Close");
		sprintf (curInput[(*curPosY)++],"            if(isClosedWindow(playerSheet))");
		sprintf (curInput[(*curPosY)++],"            {");
		sprintf (curInput[(*curPosY)++],"                freeSheet(playerSheet);");
		sprintf (curInput[(*curPosY)++],"                freeMem(fileBufC,length);");
		sprintf (curInput[(*curPosY)++],"                freeMem(playerBufC,70*28);");
		sprintf (curInput[(*curPosY)++],"                PlayOneBeat(0);");
		sprintf (curInput[(*curPosY)++],"                return;");
		sprintf (curInput[(*curPosY)++],"            }");
		sprintf (curInput[(*curPosY)++],"            //Suspend");
		sprintf (curInput[(*curPosY)++],"            if (iskey(&data) && data==' ')");
		sprintf (curInput[(*curPosY)++],"            {");
	}else if (strcmp(fileName,"\\Source\\Play.c")==0 && page==3)
	{ 
		sprintf (curInput[(*curPosY)++],"                sprintf (s,\"Suspend\");");
		sprintf (curInput[(*curPosY)++],"                putStrOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);");
		sprintf (curInput[(*curPosY)++],"                while (!(iskey(&data) && data==' '));");
		sprintf (curInput[(*curPosY)++],"                //Continue");
		sprintf (curInput[(*curPosY)++],"                sprintf (s,\"Playing\");");
		sprintf (curInput[(*curPosY)++],"                putStrOnSht(musicPlayerSheet,70,28,BLACK,WHITE,str,8);");
		sprintf (curInput[(*curPosY)++],"            }");
		sprintf (curInput[(*curPosY)++],"            //OneBeat");
		sprintf (curInput[(*curPosY)++],"            getBuffer(fileBuffer,oneBeat);");
		sprintf (curInput[(*curPosY)++],"            playOneBeat(oneBeat);");
		sprintf (curInput[(*curPosY)++],"            //20ms");
		sprintf (curInput[(*curPosY)++],"            setTimer(timerPlayer,20);");
		sprintf (curInput[(*curPosY)++],"            while (1)");
		sprintf (curInput[(*curPosY)++],"                if (getBuffer(&bufferTime,&data))");
		sprintf (curInput[(*curPosY)++],"                    break;");
		sprintf (curInput[(*curPosY)++],"            //Stop");
		sprintf (curInput[(*curPosY)++],"            PlayOneBeat(0);");
		sprintf (curInput[(*curPosY)++],"            //15ms");
		sprintf (curInput[(*curPosY)++],"            setTimer(timerPlayer,15);");
		sprintf (curInput[(*curPosY)++],"            while (1)");
		sprintf (curInput[(*curPosY)++],"                if (getBuffer(&bufferTime,&data))");
	}else if (strcmp(fileName,"\\Source\\Play.c")==0 && page==4)
	{
		sprintf (curInput[(*curPosY)++],"                    break;");
		sprintf (curInput[(*curPosY)++],"        }");
		sprintf (curInput[(*curPosY)++],"    }");
		sprintf (curInput[(*curPosY)++],"    return 0");
		sprintf (curInput[(*curPosY)++],"}");
	}
	
	for (int i=0;i<21;i++)
		putStrAndBackOnSht(textViewerSheet,16,28+i*16,BLACK,WHITE,curInput[i],75);
}

void textViewerTask_Main(struct Task *task)
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

	//��ʾ����
	struct Sheet *textViewerSheet;
	unsigned char *textViewerBuffer;
	textViewerSheet=allocSheet();
	textViewerBuffer=(unsigned char *)allocMem_4k(650*400,"Text Viewer UI");//�����ڴ�ռ� 
	setBufInSheet(textViewerSheet,textViewerBuffer,650,400,-1);
	char str[128];
	sprintf (str,"Text Viewer: %s",task->par[0]);
	makeWindow(textViewerSheet,650,400,str);
	slideSheet(textViewerSheet,180,72);
	updownSheet(textViewerSheet,task->winID+1);
	
	//�������Ϣ 
	char curInput[21][75];
	int curPosX=0,curPosY=0,lengthX=0,lengthY=0;//��� 
	unsigned char data;
	int page=0;
	//struct Command command;//���� 
	int flag=0,f1=0;
	
	for (int i=0;i<21;i++)
		curInput[i][0]='\0';
	loadTextFile(task->par[0],textViewerSheet,&curPosX,&curPosY,curInput,page);
	
	while (1)
	{
		flag=0; 
		if (window.focus!=task->winID)//���㲻�ڣ�ȡ����� 
		{
			if (f1==0)
			{
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15); 
				f1=1;
			}
			continue;
		}else if (timerCur->flag==TIMER_ALLOCED)//���»�ý��㣬������� 
		{
			initTimer(timerCur,&bufferTime,1);
			setTimer(timerCur,50);
		}
		f1=0;
		io_cli();
		//����������¼� 
		if (getBuffer(&task->bufAll.key,&data))
		{
			//���� 
			io_sti();
			flag=1;
			switch (data)
			{
				case 0x0e://�˸�� 
					if (curPosX>2)
					{
						curPosX--;
						lengthX--; 
						for (int j=curPosX;j<=lengthX;j++)
							curInput[curPosY][j]=curInput[curPosY][j+1];
						curInput[curPosY][lengthX] = '\0';
					} 
					break; 
				case 0x4b:
					if (curPosX>0)
						curPosX--;
					break;
				case 0x4d:
					if (curPosX<lengthX)
						curPosX++;
					break;
				case 0x48:
					if (curPosY>0)
					{
						curPosY--;
						lengthX=0;
						while (curInput[curPosY][lengthX] != '\0')
							lengthX++;
					}
					break;
				case 0x50:
					if (curPosY<lengthY)
					{
						curPosY++;
						lengthX=0;
						while (curInput[curPosY][lengthX] != '\0')
							lengthX++;
					}
					break;
				case 0x1c://�س��� 
					boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
					refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15); 
					curPosY++;
					if (curPosY>lengthY)
						lengthY=curPosY;
					curInput[curPosY][0] = '\0';
					curPosX=0;
					lengthX=0;
					break;
				case 0x38:
					page++;
					loadTextFile(task->par[0],textViewerSheet,&curPosX,&curPosY,curInput,page);
					break;
				case 0x1d:
					page--;
					loadTextFile(task->par[0],textViewerSheet,&curPosX,&curPosY,curInput,page);
					break;
				default:
					if (data<0x80 && keyboard.keyTable[data]>0 && curPosX<50)//��ĸ������ 
					{
						curInput[curPosY][curPosX++] = keyboard.keyTable[data+keyboard.isShift*0x80];
						if (curPosX>lengthX)
						{
							curInput[curPosY][curPosX] = '\0';
							lengthX=curPosX;
						}
					}
					break;
			}
			if (curPosY<21)
				sprintf (str,"%s",curInput[curPosY]);
			else sprintf (str,"");
			putStrAndBackOnSht(textViewerSheet,16,28+curPosY*16,BLACK,WHITE,str,75);
		}
		if (getBuffer(&task->bufAll.mouse,&data))
		{
			io_sti();
			flag=2;
			switch(data)
			{
				case 0:
					slideSheet(textViewerSheet,mdec.x,mdec.y);
					break;
				case 2:
					freeSheet(textViewerSheet);
					//freeMem((unsigned int)curInput,20);
					freeMem((unsigned int)textViewerBuffer,312*210);
					deleteWindow(task);
					deleteTask(task);
			}
		}
		if (getBuffer(&bufferTime,&data))
		{
			io_sti();
			flag=3;
			switch (data)
			{
			case 0:
				initTimer(timerCur,&bufferTime,1);
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,WHITE);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15);   
				setTimer(timerCur,50);
				break;
			case 1:
				initTimer(timerCur,&bufferTime,0);
				boxfillOnSht(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15,BLACK);
				refreshSubInSheet(textViewerSheet,16+8*curPosX,28+curPosY*16,8,15);
				setTimer(timerCur,50);
				break; 
			}
		} 
		if (flag==0)
			io_sti();
	}
}

