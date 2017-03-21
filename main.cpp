#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <memory.h>
#include <math.h>
#include "bmp.h"
/*--depeden on input graph size--*/
#define lsize 1024
#define wsize 1024
/*--error diffusion parameter--*/
#define threshold 128
#define value_0 0
#define value_1 255
#define right_ed 0.4375
#define left_down_ed 0.1875
#define down_ed 0.3125
#define right_down_ed 0.0625
/*--list_image_functin--*/
void List_image(void);
/*--Clean_image_functin--*/
void Clean_image(void);
/*--error_diffusion_function*/
void error_diffusion(void);
//-------------------------//
/*--Filter_function--*/
void Gaussian_Low_Pass_Filter(void);
void Sobel_High_Pass_Filter(void);
void Median_Pass_Filter(void);
//--------------------------//
/*--parallel--*/
void* mask_x(void* arg);
void* mask_y(void* arg);
void* Gaussian_parallel(void* arg);
struct para
{
	int x;
	int y;
};
struct para *PARA=NULL;
//------------//

/*--console--*/
int  console(void);
//------------//
char* filename=NULL;
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"
//int G[1024][1024];
//int B[1024][1024];
int r[MaxBMPSizeX][MaxBMPSizeY];
//int g[1024][1024];
//int b[1024][1024];
int xg[MaxBMPSizeX][MaxBMPSizeY];
int yg[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i,j;

	//open_bmp("lenna.bmp", R, G, B, width, height); for true color images
	//open_bmp("lena.bmp", R, R, R, width, height); // for gray images
	console();
	
    //----------------------------------------------------------------------------
	//save_bmp("lenna_new1.bmp", r, g, b); // for true color images
	// for gray images
	printf("Job Finished!\n");

	close_bmp();

	return 0;
}
/*--console--*/
int console(void)
	{
		int ch,temp;
		origin:;
		printf("Please enter any key to start\n");
		temp=getc(stdin);
		system("clear");
		start:
			printf("\n\n\n\n");
			printf("\t      +Please List bmp and input you want to chose image+\n");
			printf("\t\t         _____       _______      _____   \n");
			printf("\t\t        |  ___ \\    |__   __|    |  __ \\ \n");
			printf("\t\t        | |   \\ \\      | |       | |__||  \n");
			printf("\t\t        | |   | |      | |       | ____/  \n");
			printf("\t\t        | |__ / /    __| |__     | |      \n");
			printf("\t\t        | _____/    |_______|    |_|      \n");
			puts("\t\t      --------------------------------------");
			printf("\t\t      +List bmp image: \"L\" or \"l\"          +\n");
			printf("\t\t      +Error_diffusion: \"E\" or \"e\"         +\n");
			printf("\t\t      +Gaussian_Low_Pass_Filter: \"G\" or \"g\"+\n");
			printf("\t\t      +Sobel_High_Pass_Filter: \"S\" or \"s\"  +\n");
			//printf("\t\t      +Median_Pass_Filter:\"M\"or\"m\"      +\n");
			printf("\t\t      +Clean create bmp image: \"C\" or \"c\"  +\n");
			printf("\t\t      +Terminate program: \"T\" or \"t\"       +\n");
			puts("\t\t      --------------------------------------");
			printf("\t\t      chosen:");
			ch=getc(stdin);
			switch (ch)
			{
			case 'L':
    				List_image();
    				system("clear");
    				goto start;
    			case 'l':
    				List_image();
    				system("clear");
    				goto start;
			case 'C':
    				Clean_image();
    				system("clear");
    				goto start;
    			case 'c':
    				Clean_image();
    				system("clear");
    				goto start;
			case 'E':
    				error_diffusion();
    				system("clear");
    				goto start;
    			case 'e':
    				error_diffusion();
    				system("clear");
    				goto start;
    			case 'S':
    				Sobel_High_Pass_Filter();
    				system("clear");
    				goto start;
    			case 's':
    				Sobel_High_Pass_Filter();
    				system("clear");
    				goto start;
    			case 'G':
    				Gaussian_Low_Pass_Filter();
    				system("clear");
    				goto start;
    			case 'g':
    				Gaussian_Low_Pass_Filter();
    				system("clear");
    				goto start;
    			/*case 'M':
    				error_diffusion();
    				system("clear");
    				goto start;
    			case 'm':
    				error_diffusion();
    				system("clear");
    				goto start;*/
    			case 'T':
    				system("clear");
    				return 0;
    			case 't':
    				system("clear");
    				return 0;
    			default:
    				system("clear");
    				temp=getc(stdin);
    				printf("input function key error\n");
    				goto origin;
    				//break;
    		}
	}
	
//-------------------------------//

/*--List_image--*/
void List_image(void)
{
	char ch;
	filename=(char *)malloc(sizeof(char)*10);
	memset(filename,'\0',10);
	printf("\t\t      The file present has this image:\n");
	system("ls *.bmp ");
	printf("\t\t      input you want to chose image name and its suffix:\n\t\t      ");
	scanf("%s",filename);
	//clear stdin avoid getc() read old data frome stdin
	while((ch=getchar())!='\n'&&ch!=EOF);
	//------------------------------------------------//
}

/*--clean_image function--*/
void Clean_image(void)
{
	int ch,temp;
	system("rm -rf Gussian_LPT.bmp Error_diffusion.bmp Sobel_HPT.bmp");
	while((ch=getchar())!='\n'&&ch!=EOF);
	puts("\t\t      Please press any key to continue");  
	temp=getc(stdin);
}
/*---processing error_diffusion-*/
void error_diffusion(void)
{
	int width, height;
	open_bmp(filename, R, R, R, width, height);
	int error=0;
	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<wsize;j++)
		{
			if(j==0)//left edge condition
			{
				if(i==lsize-1) break;
				if(R[i][j]<=threshold)
				{
					r[i][j]=value_0;
					error=R[i][j]-0;
					R[i][j+1]+=(int)(error*right_ed);
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j+1]+=(int)(error*right_down_ed);
				}
				else if(R[i][j]>threshold)
				{
					r[i][j]=value_1;
					error=R[i][j]-255;
					R[i][j+1]+=(int)(error*right_ed);
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j+1]+=(int)(error*right_down_ed);
				}
			}
			else if(j==wsize-1)//right edge condition
			{
				if(i==lsize-1) break;
				if(R[i][j]<=threshold)
				{
					r[i][j]=value_0;
					error=R[i][j]-0;
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j-1]+=(int)(error*left_down_ed);
				}
				else if(R[i][j]>threshold)
				{
					r[i][j]=value_1;
					error=R[i][j]-255;
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j-1]+=(int)(error*left_down_ed);
				}	
			}
			else if(i==lsize-1)//down edge condition
			{
				if(j==wsize-1) 
				{
					if(R[i][j]<=threshold)
						r[i][j]=value_0;
					
					else if(R[i][j]>threshold)
						r[i][j]=value_1;
					goto exit;
					//break;
				}
				if(R[i][j]<=threshold)
				{
					r[i][j]=value_0;
					error=R[i][j]-0;
					R[i][j+1]+=(int)(error*right_ed);
				}
				else if(R[i][j]>threshold)
				{
					r[i][j]=value_1;
					error=R[i][j]-255;
					R[i][j+1]+=(int)(error*right_ed);
				}	
			}
			else //inside process
			{
				if(R[i][j]<=threshold)
				{
					r[i][j]=value_0;
					error=R[i][j]-0;
					R[i][j+1]+=(int)(error*right_ed);
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j+1]+=(int)(error*right_down_ed);
					R[i+1][j-1]+=(int)(error*left_down_ed);
				}
				else if(R[i][j]>threshold)
				{
					r[i][j]=value_1;
					error=R[i][j]-255;
					R[i][j+1]+=(int)(error*right_ed);
					R[i+1][j]+=(int)(error*down_ed);
					R[i+1][j+1]+=(int)(error*right_down_ed);
					R[i+1][j-1]+=(int)(error*left_down_ed);
				}
			}
		}
	}
exit:;
save_bmp("Error_diffusion.bmp", r, r, r);
printf("\t\t      Job Finished!\n");
//clear stdin avoid getc() read old data frome stdin
char ch;
while((ch=getchar())!='\n'&&ch!=EOF);
//------------------------------------------------//
puts("\t\t      Please press any key to continue"); 
int temp; 
temp=getc(stdin);
}
/*==============================================*/

/*--High_Pass_Filter function;--*/
void Sobel_High_Pass_Filter(void)
{
	int width, height;
	open_bmp(filename, R, R, R, width, height);
	int* x=NULL;
	int* y=NULL;
	int sobel;
	pthread_t t1,t2;
	pthread_create(&t1,NULL,mask_x,x);
	pthread_create(&t2,NULL,mask_y,y);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<wsize;j++)
		{
			sobel=abs(xg[i][j])+abs(yg[i][j]);
			if(sobel<185)
				r[i][j]=0;
			else if(sobel>=185)
				r[i][j]=255;	
		}
	}
	save_bmp("Sobel_HPT.bmp", r, r, r);
	free(x);
	free(y); 
	printf("\t\t      Job Finished!\n");
	//clear stdin avoid getc() read old data frome stdin
	char ch;
	while((ch=getchar())!='\n'&&ch!=EOF);
	//------------------------------------------------//
	puts("\t\t      Please press any key to continue"); 
	int temp; 
	temp=getc(stdin);

}

void* mask_x(void* arg)
{
	int* x=(int *)arg;
	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<wsize;j++)
		{
			if(i==0 || j==0 || i==lsize-1 || j==wsize-1)
			{
				xg[i][j]=0;
				//break;
			}
			else
			{
				xg[i][j]=(R[i-1][j-1]*-1)+(R[i-1][j]*0)+(R[i-1][j+1]*1)+
					     (R[i][j-1]*-2)+(R[i][j]*0)+(R[i][j+1]*2)+
					     (R[i+1][j-1]*-1)+(R[i+1][j]*0)+(R[i+1][j+1]*1);
			}
		}
	}
	return NULL;
}
void* mask_y(void* arg)
{
	int* y=(int *)arg;
	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<wsize;j++)
		{
			if(i==0 || j==0 || i==lsize-1 || j==wsize-1)
			{
				yg[i][j]=0;
				//break;
			}
			else
			{
				yg[i][j]=(R[i-1][j-1]*-1)+(R[i-1][j]*-2)+(R[i-1][j+1]*-1)+
					     (R[i][j-1]*0)+(R[i][j]*0)+(R[i][j+1]*0)+
					     (R[i+1][j-1]*1)+(R[i+1][j]*2)+(R[i+1][j+1]*1);
			}
		}
	}
	return NULL;
}
//=====================================================================//

/*--Low_Pass_Filter function--*/
void Gaussian_Low_Pass_Filter(void)
{
	int width, height;
	open_bmp(filename, R, R, R, width, height);
	PARA=(struct para *)malloc(sizeof(struct para)*4);
	pthread_t tid[4];
	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<wsize;j=j+4)
		{	
			for(int thr=0;thr<4;thr++)
			{	
				PARA[thr].x=j+thr;
				PARA[thr].y=i;
				pthread_create(&tid[thr],NULL,Gaussian_parallel,(void *)&PARA[thr]);
			}
			for(int thrend=0;thrend<4;thrend++)
				pthread_join(tid[thrend],NULL);

		}

	}
	save_bmp("Gussian_LPT.bmp", r, r, r);
	free(PARA); 
	printf("\t\t      Job Finished!\n");
	//clear stdin avoid getc() read old data frome stdin
	char ch;
	while((ch=getchar())!='\n'&&ch!=EOF);
	//------------------------------------------------//
	puts("\t\t      Please press any key to continue"); 
	int temp; 
	temp=getc(stdin);
}

void* Gaussian_parallel(void* arg)
{
	struct para *P=(struct para *)arg;
	//printf("j=%d i=%d\n",P->x,P->y);
	if( (P->x==0) || (P->x==1) || (P->x==wsize-1) || (P->x==wsize-2) ) //edge condition
	{
		r[P->y][P->x]=R[P->y][P->x];
	}
	else if( (P->y==0) || (P->y==1) || (P->y==wsize-1) || (P->y==wsize-2) ) //edge condition
	{
		r[P->y][P->x]=R[P->y][P->x];
	}
	else
	{
		r[P->y][P->x]=(int)(R[(P->y)-2][(P->x)-2]*2 + R[(P->y)-2][(P->x)-1]*4 + R[(P->y)-2][(P->x)]*5 + R[(P->y)-2][(P->x)+1]*4 + R[(P->y)-2][(P->x)+2]*2+
					  R[(P->y)-1][(P->x)-2]*4 + R[(P->y)-1][(P->x)-1]*9 + R[(P->y)-1][(P->x)]*12 + R[(P->y)-1][(P->x)+1]*9 + R[(P->y)-1][(P->x)+2]*4+
					  R[(P->y)][(P->x)-2]*15 + R[(P->y)][(P->x)-1]*12 + R[(P->y)][(P->x)]*15 + R[(P->y)][(P->x)+1]*12 + R[(P->y)][(P->x)+2]*5+	
					  R[(P->y)+1][(P->x)-2]*4 + R[(P->y)+1][(P->x)-1]*9 + R[(P->y)+1][(P->x)]*12 + R[(P->y)+1][(P->x)+1]*9 + R[(P->y)+1][(P->x)+2]*4+
					  R[(P->y)+2][(P->x)-2]*2 + R[(P->y)+2][(P->x)-1]*4 + R[(P->y)+2][(P->x)]*5 + R[(P->y)+2][(P->x)+1]*4 + R[(P->y)+2][(P->x)+2]*2)/159;
	}
	return NULL;
}
//=====================================================================//

/*--Median_Pass_Filter function--*/
/*void Median_Pass_Filter(void)
{


}*/
