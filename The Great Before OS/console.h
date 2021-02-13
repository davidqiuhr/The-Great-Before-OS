#pragma once
#define MAXCOMMANDCOUNT 	6 
#define MAXCOMMANDLENGTH	20
enum CommandType
{
	Calculator,	//������ 
	Open,		//���ļ� 
	Play,		//�������� 
	TaskList,	//��������� 
	MemoryList,	//�ڴ�� 
	Shutdown	//�ػ� 
};
struct Command
{
	int commandType;
	int parCount;
	char par[MAXCOMMANDCOUNT][MAXCOMMANDLENGTH];
};
extern struct Keyboard keyboard;
extern struct Window window;
void analyseCommand(char *commandString,struct Command *command);
void consoleTask_Main(struct Task *task);
