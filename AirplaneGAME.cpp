#include<windows.h>
#include <cstdlib> 
#include <ctime> 
#include"resource.h"
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LPCTSTR frm_title=TEXT("c_form_view");
HINSTANCE hInst;
void aa(HBITMAP,HBITMAP,HBITMAP,HBITMAP,int,HDC,HDC);
void blt(HDC,HDC,int,int);
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR IpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Msg_frm;
	WNDCLASS WndClass;
	hInst=hInstance;

	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc=WndProc;
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hInstance=hInstance;
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(NULL));
	WndClass.hCursor=LoadCursor(hInstance,MAKEINTRESOURCE(NULL));
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName=NULL;
	WndClass.lpszClassName=frm_title;

	RegisterClass(&WndClass);

	hWnd=CreateWindow(
		frm_title,
		TEXT("5-29"),
		WS_OVERLAPPEDWINDOW,
		200,
		0,
		1280,
		720,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd,nCmdShow);
	
	while(GetMessage(&Msg_frm,NULL,0,0)){
		TranslateMessage(&Msg_frm);
		DispatchMessage(&Msg_frm);
	}
	return (int)Msg_frm.wParam;
}
void key_dwn(int dkey);
int x=100,y=410;
HWND hWnd1;
int fps=1000/50;;
int cnt=1;
static HBITMAP Bitmap_air,Bitmap_back1,Bitmap_air2,Bitmap_air3,Bitmap_buble,Bitmap_pon;
HBITMAP Bitmap_dragon[5];
HBITMAP Memory_Bitmap1,Memory_Bitmap2;
static HBITMAP Bitmap1;
HBITMAP Bitmap2;
BITMAP Bit1;
int Bw,Bh;
HDC DC1,BitmapDC;
HDC Memory_BitmapDC;

bool buble_gun[5]={false};
int buble_gun_x[5]={0};
int buble_gun_y[5]={0};
int attack_cnt=0;

int dragon_x[5]={900,1000,1100,1150,800};
int dragon_y[5]={-300,-200,-100,-150,-500};

int delay_tm1=0;

int key_sw=0;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT PS1;

	switch(iMsg){
	case WM_CREATE:
		SetTimer(hWnd,1,fps,NULL);
		Bitmap_air=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
		Bitmap_air2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2));
		Bitmap_air3=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP3));
		Bitmap_back1=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP4));
		Bitmap_buble=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP5));
		Bitmap_pon=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP6));
		Bitmap_dragon[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP7));
		Bitmap_dragon[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP8));
		Bitmap_dragon[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP9));
		Bitmap_dragon[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP10));
		Bitmap_dragon[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP11));
		for(int j=0;j<5;j++)
		{
			buble_gun[j]=false;
		}
		key_sw=0;
		for(int i=0;i<5;i++)
		{
			dragon_y[i]=((int)(rand()%(350-10))+10)*-1;
			dragon_x[i]=(int)(rand()%(1200-500))+500;
		}
		return 0;
	case WM_PAINT:
		hWnd1=hWnd;
		DC1=BeginPaint(hWnd,&PS1);
		Memory_BitmapDC=CreateCompatibleDC(DC1);
		Memory_Bitmap1=CreateCompatibleBitmap(DC1,1280,720);
		Memory_Bitmap2=(HBITMAP)SelectObject(Memory_BitmapDC,Memory_Bitmap1);
		BitmapDC=CreateCompatibleDC(DC1);
		SelectObject(BitmapDC,Bitmap_back1);
		GetObject(Bitmap_back1,sizeof(BITMAP),&Bit1);
		Bw=Bit1.bmWidth;
		Bh=Bit1.bmHeight;
		BitBlt(Memory_BitmapDC,0,0,Bw,Bh,BitmapDC,0,0,SRCCOPY);
		delay_tm1++;
		if(delay_tm1==5)
		{
			cnt++;
			delay_tm1=0;
		}
		
		if(cnt==1){
			SelectObject(BitmapDC,Bitmap_air);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
		}else if(cnt==2){
			SelectObject(BitmapDC,Bitmap_air2);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
		}else if(cnt==3){
			SelectObject(BitmapDC,Bitmap_air3);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
			cnt=1;
		}
		for(int j=0;j<5;j++)
		{
			if(buble_gun[j]==true)
			{
				SelectObject(BitmapDC,Bitmap_buble);
				buble_gun_x[j]+=10;
				BitBlt(Memory_BitmapDC,buble_gun_x[j],buble_gun_y[j],30,30,BitmapDC,0,30,SRCPAINT);
				BitBlt(Memory_BitmapDC,buble_gun_x[j],buble_gun_y[j],30,30,BitmapDC,0,0,SRCAND);
				if(buble_gun_x[j]>1280)
				{
					buble_gun[j]=false;
				}
			}
		}
		/*if(buble_gun==1)
			{
				SelectObject(BitmapDC,Bitmap_buble);
				buble_gun_x+=10;
				BitBlt(Memory_BitmapDC,buble_gun_x,buble_gun_y,30,30,BitmapDC,0,30,SRCPAINT);
				BitBlt(Memory_BitmapDC,buble_gun_x,buble_gun_y,30,30,BitmapDC,0,0,SRCAND);
				if(buble_gun_x>1280)
				{
					buble_gun=0;
				}
			}*/
		for(int i=0;i<5;i++)
		{
			dragon_y[i]+=5;
			for(int j=0;j<5;j++)
			{
				if((buble_gun_x[j]+30)>dragon_x[i]&&(buble_gun_x[j])<(dragon_x[i]+78)&&(buble_gun_y[j]+30)>dragon_y[i]&&(buble_gun_y[j])<(dragon_y[i]+78)&&buble_gun[j]==true)
				{
					SelectObject(BitmapDC,Bitmap_pon);
					BitBlt(Memory_BitmapDC,dragon_x[i]+10,dragon_y[i]+5,61,57,BitmapDC,0,57,SRCPAINT);
					BitBlt(Memory_BitmapDC,dragon_x[i]+10,dragon_y[i]+5,61,57,BitmapDC,0,0,SRCAND);


					dragon_y[i]=((int)(rand()%(350-10))+10)*-1;
					dragon_x[i]=(int)(rand()%(1200-500))+500;
					buble_gun[j]=false;
				}
				else
				{
					SelectObject(BitmapDC,Bitmap_dragon[i]);
					BitBlt(Memory_BitmapDC,dragon_x[i],dragon_y[i],78,78,BitmapDC,0,78,SRCPAINT);
					BitBlt(Memory_BitmapDC,dragon_x[i],dragon_y[i],78,78,BitmapDC,0,0,SRCAND);
					if(dragon_y[i]>720)
					{
						dragon_y[i]=((rand()%(350-10))+10)*-1;
						dragon_x[i]=(rand()%(1200-500))+500;
					}
				}
			}
		}
		/*if((buble_gun_x+30)>dragon_x[i]&&(buble_gun_x)<(dragon_x[i]+78)&&(buble_gun_y+30)>dragon_y[i]&&
				(buble_gun_y)<(dragon_y[i]+78)&&buble_gun==true)
			{
				SelectObject(BitmapDC,Bitmap_pon);
				BitBlt(Memory_BitmapDC,dragon_x[i]+10,dragon_y[i]+5,61,57,BitmapDC,0,57,SRCPAINT);
				BitBlt(Memory_BitmapDC,dragon_x[i]+10,dragon_y[i]+5,61,57,BitmapDC,0,0,SRCAND);
				dragon_y[i]=-100;
				buble_gun=0;
			}
			else
			{
				SelectObject(BitmapDC,Bitmap_dragon);
				BitBlt(Memory_BitmapDC,dragon_x[i],dragon_y[i],78,78,BitmapDC,0,78,SRCPAINT);
				BitBlt(Memory_BitmapDC,dragon_x[i],dragon_y[i],78,78,BitmapDC,0,0,SRCAND);
				if(dragon_y[i]>720){
					dragon_y[i]=-100;
				}
			}*/
		BitBlt(DC1,0,0,1280,720,Memory_BitmapDC,0,0,SRCCOPY);
		SelectObject(BitmapDC,Memory_Bitmap2);
		DeleteObject(Memory_Bitmap1);
		DeleteDC(Memory_BitmapDC);
		DeleteDC(BitmapDC);
		EndPaint(hWnd,&PS1);
		return 0;
	case WM_KEYUP:
		
		key_sw=0;
		return 0;
	case WM_TIMER:
		key_dwn(wParam);
	
		SetTimer(hWnd,1,fps,NULL);
		return 0;
	case WM_DESTROY:
		DeleteObject(Bitmap_back1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMsg,wParam,lParam));
}
void key_dwn(int dkey){
	if(GetKeyState(VK_LEFT) & 0x8000){
		x-=5;
	}
	if(GetKeyState(VK_RIGHT) & 0x8000){
		x+=5;
	}
	if(GetKeyState(VK_UP)&0x8000){
		y-=5;
	}
	if(GetKeyState(VK_DOWN)&0x8000){
		y+=5;
	}
	if((GetKeyState(VK_SPACE)&0x8000)&&key_sw==0)
	{
		attack_cnt++;
		if(attack_cnt>5)
		{
			attack_cnt=1;
		}
		if(buble_gun[attack_cnt]==false)
		{
			buble_gun_x[attack_cnt]=x+70;
			buble_gun_y[attack_cnt]=y+15;
			buble_gun[attack_cnt]=true;
		}
		key_sw=1;
	}
	
	/*if(GetKeyState(VK_SPACE)&0x8000){

		if(buble_gun==0)
		{
			buble_gun_x=x+70;
			buble_gun_y=y+15;
			buble_gun=1;
		}
	}*/
	if(x<0){
		x=0;
	}
	if(x>1180){
		x=1180;
	}
	if(y<0){
		y=0;
	}
	if(y>630){
		y=630;
	}
	if(GetKeyState('1') & 0x8000){
		DestroyWindow(hWnd1);
	}
	InvalidateRect(hWnd1,NULL,FALSE);
}
void aa(HBITMAP Bitmap_air,HBITMAP Bitmap_back,HBITMAP Bitmap_air2,HBITMAP Bitmap_air3,int cnt,HDC BitmapDC,HDC DC1,HDC Memory_BitmapDC){
	cnt++;
	if(cnt==1){
		SelectObject(BitmapDC,Bitmap_air);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
	}else if(cnt==2){	
		SelectObject(BitmapDC,Bitmap_air2);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
	}else if(cnt==3){
		SelectObject(BitmapDC,Bitmap_air3);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,48,SRCPAINT);
		BitBlt(Memory_BitmapDC,x,y,79,48,BitmapDC,0,0,SRCAND);
		cnt=0;
	}
	BitBlt(DC1,0,0,800,600,Memory_BitmapDC,0,0,SRCCOPY);
}
void blt(HDC Memory_BitmapDC,HDC BitmapDC,int Bw,int Bh){
	BitBlt(Memory_BitmapDC,0,0,Bw,Bh,BitmapDC,0,0,SRCCOPY);
}
