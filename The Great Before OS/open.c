#include"nasmfunc.h"
#include"memory.h"
#include"sheet.h"
#include"graphic.h"
#include"buffer.h"
#include"timer.h" 
#include"mtask.h"
#include"keyboard.h"
#include"mouse.h" 
#include"console.h"
#include"textviewer.h"
#include"imageviewer.h"
#include"player.h"
#include"open.h"
#include<stdio.h>
#include<string.h>
void openFile(char *fileName)
{
	if (strcmp(fileName,"\\Text\\Welcome")==0)
	{
		struct Task *textViewerTask; 
		textViewerTask=allocTask();		
		initTask(textViewerTask,(int)&textViewerTask_Main,"TextViewer",11);
		sprintf(textViewerTask->par[0],"\\Text\\Welcome");
		textViewerTask->parCount++;
		createWindow(textViewerTask,"TextViewer");
		runTask(textViewerTask);
	}else if (strcmp(fileName,"\\Source\\Play.c")==0)
	{
		struct Task *textViewerTask; 
		textViewerTask=allocTask();		
		initTask(textViewerTask,(int)&textViewerTask_Main,"TextViewer",11);
		sprintf(textViewerTask->par[0],"\\Source\\Play.c");
		textViewerTask->parCount++;
		createWindow(textViewerTask,"Play.c");
		runTask(textViewerTask);
	}else if (strcmp(fileName,"\\Image\\Pic1")==0)
	{
		struct Task *imageViewerTask; 
		imageViewerTask=allocTask();		
		initTask(imageViewerTask,(int)&imageViewerTask_Main,"ImageViewer",1);
		sprintf(imageViewerTask->par[0],"\\Image\\Pic1");
		imageViewerTask->parCount++;
		createWindow(imageViewerTask,"ImageViewer");
		runTask(imageViewerTask);
	}else if (strcmp(fileName,"\\Music\\Mus1")==0)
	{
		struct Task *textViewerTask; 
		textViewerTask=allocTask();		
		initTask(textViewerTask,(int)&textViewerTask_Main,"TextViewer",11);
		sprintf(textViewerTask->par[0],"\\Music\\Mus1");
		textViewerTask->parCount++;
		createWindow(textViewerTask,"TextViewer");
		runTask(textViewerTask);
	}

}
