#pragma once
//easyOS.nsm
struct BootInfo
{
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
};

//nasmfunc.nsm
void io_hlt();
void io_stihlt();
void write_mem8(int addr, int data);
void io_cli(); 
void io_sti();
int io_in8(int port);
void io_out8(int port,int data);
int io_load_eflags();
void io_store_eflags(int eflags);
void load_gdtr(int limit,int addr);
void load_idtr(int limit,int addr);
void nsm_inthandler20(void);
void nsm_inthandler21(void);
void nsm_inthandler27(void);
void nsm_inthandler2c(void);
void load_tr(int);
int load_cr0(void);
void store_cr0(int cr0);
void farjmp(int eip,int cs);
void shutdown();
