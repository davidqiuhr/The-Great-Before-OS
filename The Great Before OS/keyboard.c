#include"keyboard.h"
#include"nasmfunc.h"
#include"buffer.h"
struct Keyboard keyboard;
//�ȴ����̿��Ƶ�·׼�����
void wait_KBC_sendready()
{
	//���̿��Ƶ�·׼�����ʱ PORT_KEYSTA��ַ�ĵ����ڶ�λӦ����0
	while (1)
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY)==0)
			break;
	return;
}
//��ʼ�����̿��Ƶ�·
void init_keyboard()
{
	//�ȴ����Է�����Ϣ
	wait_KBC_sendready();
	//����ģʽָ��
	io_out8(PORT_KEYCMD,KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	//����ָ����
	io_out8(PORT_KEYDAT,KBC_MODE);
}


