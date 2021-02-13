#pragma once

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

//ȫ���������� 
struct Segment_Descriptor
{
	/*
	base �λ�ַ�����Ǽ����ԣ��ֳ������Σ� 
	limit ��������20λ��limit_low��limit_high��4λ����limit��λ��ҳ��1ҳΪ4KB�����ܿɱ�ʾ4KB*1M= 4GB
	������12λ��access_right��limit_high��4λ�� 
	*/
	short limit_low,base_low;
	char base_mid,access_right;
	char limit_high,base_high;
};
//�жϼ�¼�� 
struct Gate_Descriptor
{
	/*
	offset ƫ��
	selector ѡ���
	�����жϺ� �����������λ�ã�ѡ���ȷ���Σ�ƫ��ȷ������λ��
	*/
	 
	short offset_low,selector;
	char dw_count,access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct Segment_Descriptor *sd,unsigned int limit,int base,int ar);
void set_gatedesc(struct Gate_Descriptor *gd,int offset,int selector,int ar);
