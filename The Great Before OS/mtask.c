#include"timer.h"
#include"nasmfunc.h" 
#include"dsctbl.h"
#include"memory.h"
#include"buffer.h"
#include"mtask.h"
#include"stdio.h"
struct TaskCTL *taskctl;
struct Window window;
void initTaskCTL()
{
	int i=0;
	struct Segment_Descriptor *gdt=(struct Segment_Descriptor *)ADR_GDT;
	taskctl=(struct TaskCTL *)allocMem_4k(sizeof (struct TaskCTL),"Task List");
	for (i=0;i<MAX_TASKS;i++) 
	{
		taskctl->tasks0[i].flags=TASK_UNALLOCED;
		taskctl->tasks0[i].sel=(TASK_GDT0+i)*8;
		set_segmdesc(gdt+TASK_GDT0+i,103,(int)&taskctl->tasks0[i].tss,AR_TSS32);	//�趨�� 
	}
	return;
}
struct Task *getMainTask()
{
	//��ʼ�������� 
	struct Task *task;
	task=allocTask();
	task->flags=TASK_USING;
	taskctl->runningCount=1;
	taskctl->tasks[0]=task;
	taskctl->now=0;
	load_tr(task->sel);
	taskTimer=allocTimer();
	setTimer(taskTimer,2);
	//����
	return task;
}
//�������� 
struct Task *allocTask()
{
	int i;
	struct Task *task;
	for (i=0;i<MAX_TASKS;i++)
	{
		if (taskctl->tasks0[i].flags==TASK_UNALLOCED)//�ҵ���һ�����Է�������� 
		{
			//�趨��������� 
			task=&taskctl->tasks0[i];
			task->flags=TASK_ALLOCED;
			task->tss.eflags=0x00000202; //IF=1
			task->tss.eax=0;
			task->tss.ecx=0;
			task->tss.edx=0;
			task->tss.ebx=0;
			task->tss.ebp=0;
			task->tss.esi=0;
			task->tss.edi=0;
			task->tss.es=0;
			task->tss.ds=0;
			task->tss.fs=0;
			task->tss.gs=0;
			task->tss.ldtr=0;
			task->tss.iomap=0x40000000;
			return task;
		}
	}
	return 0;
}
void initTask(struct Task *task,int eip,char *s,int f)//,unsigned char *keyb,unsigned char *mouseb)
{
	char str[30];
	sprintf (str,"%s PCB",s);
	task->tss.esp=allocMem_4k(64*1024,str)+64*1024-8;//ջ��ָ��Ϊ�ڴ��ĩβ��Ϊ���õ�ַ��������Χ��sht_backΪesp+4��esp+8 
	task->tss.eip=eip;
	task->tss.es=1*8;
	task->tss.cs=2*8;
	task->tss.ss=1*8;
	task->tss.ds=1*8;
	task->tss.fs=1*8;
	task->tss.gs=1*8;
	*((int *)(task->tss.esp+4)) = (int)task;//ѹ��ջ�����������Ĳ��� 
	task->parCount=0;
	//�趨ID 
	task->winID=window.winCount;
	
	//������껺����
	if (f>=10)
	{
		sprintf (str,"%s Communication Buffer",s);
		unsigned char *keyb=(unsigned char *)allocMem(32,str);
		initBuffer(&task->bufAll.key,32,keyb);
	}
	if (f%10==1)	
	{
		sprintf (str,"%s Operation Buffer",s);
		unsigned char *mouseb=(unsigned char *)allocMem(1024,str);
		initBuffer(&task->bufAll.mouse,1024,mouseb);  
	}

}
void runTask(struct Task *task)
{
	task->flags=TASK_USING;
	taskctl->tasks[taskctl->runningCount]=task;//��task��ӵ����ж����У����� 
	taskctl->runningCount++;
}

void switchTask()
{
	setTimer(taskTimer,2);
	if (taskctl->runningCount>=2)
	{
		taskctl->now++;
		if (taskctl->runningCount==taskctl->now)
			taskctl->now=0;
		farjmp(0,taskctl->tasks[taskctl->now]->sel);
	}
}
void deleteTask(struct Task *task)
{
	int i;
	char ts=0;
	if (task->flags==2)
	{
		//�����Ҫɾ���������е����� ������Ҫ���������л� 
		if (task==taskctl->tasks[taskctl->now])
			ts=1;
			
		//Ѱ����Ҫɾ�������� 
		for(i=0;i<taskctl->runningCount;i++) 
			if (task==taskctl->tasks[i])
				break;
				
		//�޸��������еı��
		if (taskctl->now>i) 
			taskctl->now--;
		//ɾ������ 
		taskctl->runningCount--;
		for (;i<taskctl->runningCount;i++)
			taskctl->tasks[i]=taskctl->tasks[i+1];
		task->flags=TASK_UNALLOCED;
		//taskctl->tasks[taskctl->runningCount]->flags=TASK_UNALLOCED;
		
		
		//�����л� 
		if (ts!=0)
		{  
			if (taskctl->now>=taskctl->runningCount)
				taskctl->now=0;
			farjmp(0,taskctl->tasks[taskctl->now]->sel);
		}
		
	}
}

void initWindow()
{
	window.focus=0;
	window.winCount=0;
	window.isChanged=1; 
}
void createWindow(struct Task *task,char *name)
{
	io_cli();
	sprintf (window.winName[window.winCount],"%s",name);	
	window.focus=window.winCount;
	window.winCount++;
	window.isChanged=1;
	io_sti();
}
void deleteWindow(struct Task *task)
{
	int i;
	io_cli();
	//�رմ��ڣ��л�����
	window.winCount--;
	for (i=task->winID;i<window.winCount;i++) 
		sprintf(window.winName[i],"%s",window.winName[i+1]);
	for (i=0;i<taskctl->runningCount;i++) 
		if (taskctl->tasks[i]->winID>task->winID)
			taskctl->tasks[i]->winID--;
	window.focus=0;
	window.isChanged=1;
	io_sti();
}
