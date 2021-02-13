#include"nasmfunc.h"
#include"buffer.h"
#include"timer.h" 
#include"int.h"
#include"mtask.h"
#include<stdio.h>
/*
PIC�ǿɱ���жϿ����� ������PIC�������ӣ�ÿ��PIC��8·IRQ���ж����� 
IMR �ж���������8λ��Ӧ8��IRQ�ź� ��PIC�ļĴ��� 
io_out8���Ӧ�˿�д���� 
*/
void init_pic(void)
{
	//��ֹ�����ж� 
	io_out8(PIC0_IMR,0xff);	
	io_out8(PIC1_IMR,0xff);
	
	//������PIC 
	//ICW��4��2�ֽ� ICW1��ICW4 Ϊ�������ԣ��̶�ֵ
	//ICW2����IRQ����һ���ж�֪ͨCPU 
	//ICW3������PIC�����趨 0x00000100  ��PIC���ӵ���PIC��IRQ2�� �̶�ֵ 
	io_out8(PIC0_ICW1,0x11); //���ش���ģʽ
	io_out8(PIC0_ICW2,0x20); //IRQ0-7��INT20-27����
	io_out8(PIC0_ICW3,1<<2); //��PIC��IRQ2���� 
	io_out8(PIC0_ICW4,0x01); //�޻�����ģʽ
	
	//���ô�PIC 
	io_out8(PIC1_ICW1,0x11); //���ش���ģʽ
	io_out8(PIC1_ICW2,0x28); //IRQ8-15��INT28-2f����
	io_out8(PIC1_ICW3,2); 	 //��PIC��IRQ2���� 
	io_out8(PIC1_ICW4,0x01); //�޻�����ģʽ
	
	io_out8(PIC0_IMR,0xfb);	//11111011 PIC1���� ȫ����ֹ 
	io_out8(PIC1_IMR,0xff);	//11111111 ��ֹ�����ж�
	return; 	 
}

//��ʱ���жϴ������ 
void inthandler20(int *esp)
{
	int i;			
	char ts=0;//�����л��ı�־ 
	//֪ͨPIC�ж��Ѿ����� 
	//��ʱ���ж���IRQ0
	io_out8(PIC0_OCW2,0x60);
	timerctl.count++;
	//δ����һ�γ�ʱ��ʱ�� 
	if (timerctl.next>timerctl.count)
		return;
	timerctl.next=TIME_MAX;
	//���˳�ʱʱ�䣬���г�ʱ��ʱ��д���棬δ��ʱ��ʣ��ʱ����̵�Ϊ��һ�γ�ʱʱ�� 
	for (i=0;i<TIMER_MAX;i++) 
	{
		if (timerctl.timer[i].flag==TIMER_USING)
		{
			if (timerctl.count>=timerctl.timer[i].timeout) 
			{
				timerctl.timer[i].flag=TIMER_ALLOCED;
				//�����л��ĳ��� 
				if (taskTimer==&timerctl.timer[i])//�����л���ʱ�� 
					ts=1; 
				else putBuffer(timerctl.timer[i].timeoutBuffer,timerctl.timer[i].timeoutData);//������ʱ�� 
			}else if (timerctl.timer[i].timeout<timerctl.next)
					timerctl.next=timerctl.timer[i].timeout;
		}
		if (ts!=0)
			switchTask();
	}
	return;
} 
//�����жϴ������ 
void inthandler21(int *esp)
{
	//֪ͨPIC�ж��Ѿ����� 
	//�����ж���IRQ1����PIC0����0x(1+60)����
	io_out8(PIC0_OCW2,0x61);
	unsigned char data=io_in8(PORT_KEYDAT);
	putBuffer(&allBuf.key,data);
	return;
}
//�������Ǹ�ɶ��
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}

//����жϴ������ 
void inthandler2c(int *esp)
{
	//����ж���IRQ12����IRQ0����2��ҲҪ��IRQ1����4����PIC�ĵ�5���жϣ�
	io_out8(PIC1_OCW2,0x64);
	io_out8(PIC0_OCW2,0x62);
	unsigned char data=io_in8(PORT_KEYDAT);
	putBuffer(&allBuf.mouse,data);
	return;
} 
