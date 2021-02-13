#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h"
#include"calculator.h" 
#include<stdio.h>
void analysize(char *str,struct Exp *exp)
{
	int l=0,i,isnum=0,r=0;
	int num=0;
	while (str[l]!='\0')
	{
		//�������� 
		if (str[l]>='0' && str[l]<='9')
		{
			num*=10;
			num=num+str[l]-'0';
			isnum=1;
		}
		else
		{
			if (isnum)//��ȡ���� 
			{
				exp->data[r]=num;
				exp->type[r]=0;
				r++;
				isnum=0;
			}
			num=0;
			exp->type[r]=1;
			switch(str[l])//���� 
			{
				case '-'://�������⴦�� 
					if (l==0)
					{
						exp->type[0]=0;
						exp->data[0]=0;
						r++;
						break;
					}
				case '+':
				case '*':
				case '/':
				case '(':
				case ')':
					exp->data[r]=0;
					exp->type[r]=str[l];
					break;
				default://���� 
					exp->type[0]=-1;
					return;
			}
			r++;
		}
		l++;
	}
	if (isnum)//��ȡ���� 
	{
		exp->data[r]=num;
		exp->type[r]=0;
		r++;
	}
	exp->length=r;
} 
int calSub(struct Exp *exp,int h,int r)
{
	int i,deep=0,j;
	int pl=0,pr=0;
	//�������� 
	for (i=h;i<=r;i++)
	{
		if (exp->type[i]=='(')
		{
			if (deep==0)
				pl=i;
			deep++;
		}else if (exp->type[i]==')')
		{
			deep--;
			if (deep<0)
			{
				exp->type[0]=-1;
				return 0;
			}else if (deep==0)
			{
				pr=i;
				int d=pr-pl-1; 
				int  res=calSub(exp,pl+1,pr-1);//���������ڵ� 
				if (exp->type[0]==-1 || exp->type[pl+2]!=')')
				{
					exp->type[0]=-1;
					return 0;
				}else //ɾ������ 
				{
					exp->type[pl]=0;
					exp->data[pl]=exp->data[pl+1];
					for (j=pl+1;j<=exp->length-2;j++)
					{
						exp->data[j]=exp->data[j+2];
						exp->type[j]=exp->type[j+2];
					}
					exp->length-=2;
					i-=(d+1);
					r-=(d+1); 
				}
			}
		}
	}
	//����˳� 
	for (i=h+1;i<=r-1;i++) 
	{
		if (exp->type[i]=='*' || exp->type[i]=='/')
		{
			if (exp->type[i-1]==0 && exp->type[i+1]==0)
			{
				if (exp->type[i]=='/')
				{
					if (exp->data[i+1]==0)
					{
						//������Ϊ0 
						exp->type[0]=-1;
						return 0;
					}else 
					{
						exp->data[i-1]=exp->data[i-1]/exp->data[i+1];//���� 
					}
				}else 
				{
					exp->data[i-1]=exp->data[i-1]*exp->data[i+1];//�˷� 
				}
				//ɾ�������� 
				for (j=i;j<exp->length-2;j++)
				{
					exp->data[j]=exp->data[j+2];
					exp->type[j]=exp->type[j+2];
				}
				exp->length-=2;
				i-=2;
				r-=2; 
			}else
			{
				exp->type[0]=-1;
				return 0;
			}
		}
	}
	//����Ӽ�
	for (i=h+1;i<=r-1;i++) 
	{
		if (exp->type[i]=='+' || exp->type[i]=='-')
		{
			if (exp->type[i-1]==0 && exp->type[i+1]==0)
			{
				if (exp->type[i]=='-')
				{
					exp->data[i-1]=exp->data[i-1]-exp->data[i+1];//���� 
				}else 
				{
					exp->data[i-1]=exp->data[i-1]+exp->data[i+1];//�ӷ� 
				}
				//ɾ�������� 
				for (j=i;j<exp->length-2;j++)
				{
					exp->data[j]=exp->data[j+2];
					exp->type[j]=exp->type[j+2];
				}
				exp->length-=2;
				i-=2;
				r-=2; 
			}else
			{
				exp->type[0]=-1;
				return 0;
			}
		}
	}
	return exp->data[h];
}
int calculator (char *str)
{
	int i;
	int res; 
	struct Exp exp;
	analysize(str,&exp);//�������ʽ 
	if (exp.type[0]==-1)
	{
		str[0]='\0';
		return 0;
	}
	else
	{
		res=calSub(&exp,0,exp.length-1);
		if (exp.type[0]==-1 || exp.length!=1)
		{
			str[0]='\0';
			return 0;
		}else return res;
	} 
}
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
	
	//��ʼ��������ͼ��
	struct Sheet *calculatorSheet;
	unsigned char *calculatorBuffer;	
	calculatorBuffer=(unsigned char *)allocMem(312*210,"Calculator UI");
	calculatorSheet=allocSheet();
	setBufInSheet(calculatorSheet,calculatorBuffer,312,210,-1);//û��͸��ɫ
	slideSheet(calculatorSheet,202,8);
	makeWindow(calculatorSheet,312,210,"Calculator");
	makeTextBox(calculatorSheet,8,27,296,176,BLACK);
	updownSheet(calculatorSheet,task->winID+1);
	
	
	unsigned char data;
	int curPosX=0,curPosY=0,length=0;
	char *curInput=(unsigned char *)allocMem(20,"Calculator Input");
	curInput[0]='>';
	curInput[1]=' ';
	curInput[2]='\0';
	curPosX=2;
	char str[40];
	int flag=0,f1=0;
	
	sprintf (str,"%s",curInput);
	putStrAndBackOnSht(calculatorSheet,8,28+16*curPosY,WHITE,BLACK,str,20);
	while (1)
	{
		flag=0;
		if (window.focus!=task->winID)//���㲻�ڣ�ȡ����� 
		{
			if (f1==0)
			{
				boxfillOnSht(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15,BLACK);
				refreshSubInSheet(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15); 
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
		if (getBuffer(&task->bufAll.key,&data))
		{
			io_sti();
			switch (data)
			{
				case 0x0e://�˸�� 
					if (curPosX>2)
					{
						curPosX--;
						length--; 
						for (int j=curPosX;j<=length;j++)
							curInput[j]=curInput[j+1];
						curInput[length] = '\0';
					} 
					break; 
				case 0x4b:
					if (curPosX>0)
						curPosX--;
					break;
				case 0x4d:
					if (curPosX<length)
						curPosX++;
					break;
				case 0x1c://�س��� 
					putStrAndBackOnSht(calculatorSheet,8,28+16*curPosY,WHITE,BLACK,str,37);
					boxfillOnSht(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15,BLACK);
					curPosY++;//��һ�� 
					//ɾ�� > 
					for (int i=2;i<curPosX;i++)
						curInput[i-2]=curInput[i]; 
					curInput[curPosX-2]='\0';
					int res=calculator(curInput); 
					if (curInput[0]=='\0')//���ʽ��ȷ�� 
						sprintf (str,"Please input the correct expression.");
					else sprintf (str,"Result is %d.",res);	//������ 
					putStrAndBackOnSht(calculatorSheet,8,28+16*curPosY,WHITE,BLACK,str,37);
					//����һ�� 
					curInput[0]='>';
					curInput[1]=' ';
					curInput[2]='\0';
					curPosX=2;
					length=2;
					curPosY++; 
					break;
				default:
					if (data<0x80 && keyboard.keyTable[data]>0 && curPosX<=20)//��ĸ������ 
					{
						curInput[curPosX++] = keyboard.keyTable[data+keyboard.isShift*0x80];
						if (curPosX>length)
						{
							curInput[curPosX] = '\0';
							length=curPosX;
						}
					}
					break;
			}
			sprintf (str,"%s",curInput);
			putStrAndBackOnSht(calculatorSheet,8,28+16*curPosY,WHITE,BLACK,str,37);
		}
		if (getBuffer(&task->bufAll.mouse,&data))
		{
			io_sti();
			flag=2;
			switch(data)
			{
				//����ƶ� 
				case 0:
					slideSheet(calculatorSheet,mdec.x,mdec.y);
					break;
				//�Ҽ��ر�
				case 2:
					freeSheet(calculatorSheet);
					freeMem((unsigned int)curInput,20);
					freeMem((unsigned int)calculatorBuffer,312*210);
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
				boxfillOnSht(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15,WHITE);
				refreshSubInSheet(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15);   
				setTimer(timerCur,50);
				break;
			case 1:
				initTimer(timerCur,&bufferTime,0);
				boxfillOnSht(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15,BLACK);
				refreshSubInSheet(calculatorSheet,8+8*curPosX,28+16*curPosY,8,15);
				setTimer(timerCur,50);
				break; 
			}
		} 
		if (flag==0)
		{
			io_sti();
		} 
	}
}
