#include<windows.h>
#include <cstdlib> 
#include <ctime> 
#include <math.h>
#include"resource.h"

#define BULLET_MAG 10 //총알의 장탄수
#define BULLET_DAM 5 //총알 데미지
#define SQUID_HP 20 //오징어의 체력
#define ROBOT_HEALTH 10 //플레이어의 체력
#define SQUID_NUMBER 8 //오징어가 처음에 나오는 개체수
#define BOSS_HP 2000 //보스의 체력
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LPCTSTR frm_title=TEXT("c_form_view");
HINSTANCE hInst;
void robot_fire();
void upfire();
void upbullet();
void bullet();
void aim_up();
void crash();
void up_crash();
void jump();
void walk();
void blt(HDC,HDC,int,int);
void create_resource();
void squid();
void dmg_test();
void robot_damage();
void player_crash();
void squid_dead(int i);
void squid_attack(int i);
void squid_search();
void stone_crash();
void misson_failed_screen();
void misson_complete_screen();
void boss_idle();
void boss_appear();
void player_boss_crash();
void player_boss_attack_crash();
void boss_move();
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
		TEXT("GiantSquid"),
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
int x=100,y=600;
HWND hWnd1;
int fps=1000/50;;

static HBITMAP Bitmap_back1,Bitmap_back2,Bitmap_Misson_Complete,Bitmap_Misson_Failed;
HBITMAP Memory_Bitmap1,Memory_Bitmap2;
HFONT hFont,OldFont;
static HBITMAP Bitmap1;
HBITMAP Bitmap2;
BITMAP Bit1;
int Bw,Bh;
HDC DC1,BitmapDC,hdc;
HDC Memory_BitmapDC;

int attack_cnt=0;
int up_attack_cnt=0;
int cnt_stone=0;
int delay_tm1=0;
int delay_tm2=0;
int delay_tm3=0;
int delay_squid=0;
int key_sw=0;
int sw=0;
int mouse_x=0,mouse_y=0;
int stage_num=0;
HBITMAP Bitmap_Bullet,Bitmap_Robot_Idle[4],Bitmap_Robot_Walk[16],Bitmap_Robot_Jump[8],
	Bitmap_Robot_Fire[5],Bitmap_Robot_Aimup[6],Bitmap_Up_Bullet,Bitmap_Robt_Up_Fire[5],Bitmap_Robot_Damamge[2];

HBITMAP Bitmap_Squid_Idle[5],Bitmap_Squid_Move[10],Bitmap_Squid_Damage[12],Bitmap_Squid_Attack[2];
HBITMAP Bitmap_Boss_Idle[3],Bitmap_Boss_Appear[21];
int jump_po=0;
int hero_top=0;
int cnt=0;
int cnt_a=0;
int cnt_f=0;
bool bNowDraw=false;
int cnt_j=0;
int cnt_upf=0;
int cnt_w=0;
int hero_fire=0;
int Idle_cnt=0;
int Move_cnt=0;
int attact_cnt=0;
int boss_cnt=0;
int cnt_d=0;
int score=0;
int death_delay=0;
int cnt_boss_idle=0;
int robot_hp=ROBOT_HEALTH;
int squid_hp[SQUID_NUMBER]={0};
int boss_hp=BOSS_HP;
int boss_hp_per1=0;
int boss_hp_per2=0;
bool squid_state[SQUID_NUMBER]={false},aim=false,fire_sw=false,jump_sw=false,walk_sw=false,upfire_sw=false,boss_idle_sw=false;
bool bullet_sw[BULLET_MAG]={false,},stone_sw[SQUID_NUMBER]={false,};
bool up_bullet_sw[BULLET_MAG];
bool robot_idle_sw=true;
int bullet_x[BULLET_MAG]={0,},bullet_y[BULLET_MAG]={0,};
int up_bullet_x[BULLET_MAG],up_bullet_y[BULLET_MAG];
int squid_x[SQUID_NUMBER],squid_y[SQUID_NUMBER];
int squid_speed[SQUID_NUMBER];
int dead_squid_x[SQUID_NUMBER],dead_squid_y[SQUID_NUMBER];
int stone_x[SQUID_NUMBER],stone_y[SQUID_NUMBER];
int boss_x=900,boss_y=550;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT PS1;
	HDC txtdc1;
	PAINTSTRUCT txtdc2;
	TCHAR text[100];

	switch(iMsg){
	case WM_CREATE:
		SetTimer(hWnd,1,fps,NULL);
		create_resource();
		key_sw=0;
		srand(time(NULL));
		//초기화
		for(int i=0;i<SQUID_NUMBER;i++)
		{
			
			squid_state[i]=false;
			squid_y[i]=((int)(rand()%(600-100))+100);
			squid_x[i]=(int)(rand()%(1800-900)+1400);
			stone_y[i]=squid_y[i]+40;
			stone_x[i]=squid_x[i]+60;
			squid_speed[i]=(int)(rand()%(14-7)+3);
			squid_hp[i]=SQUID_HP;
		}
		for(int i=0;i<SQUID_NUMBER;i++){
			stone_sw[i]=true;
		}
		for(int i=0;i<BULLET_MAG;i++){
			bullet_sw[i]=false;
			up_bullet_sw[i]=true;
		}
		cnt_boss_idle=-1;
		if(boss_cnt==21){
			cnt_boss_idle=0;
		}
		boss_x=1030;
		boss_y=400;
		
		stage_num=0;
		 return 0;
	case WM_PAINT:
		hWnd1=hWnd;
		DC1=BeginPaint(hWnd,&PS1);
		Memory_BitmapDC=CreateCompatibleDC(DC1);
		Memory_Bitmap1=CreateCompatibleBitmap(DC1,1280,720);
		Memory_Bitmap2=(HBITMAP)SelectObject(Memory_BitmapDC,Memory_Bitmap1);
		BitmapDC=CreateCompatibleDC(DC1);
		if(stage_num==0){//GIANTSQUID stage
			SelectObject(BitmapDC,Bitmap_back2);
			BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCPAINT);
			BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCAND);
			SelectObject(BitmapDC,Bitmap_back1);
			BitBlt(Memory_BitmapDC,0,467,1024,213,BitmapDC,0,213,SRCPAINT);
			BitBlt(Memory_BitmapDC,0,467,1024,213,BitmapDC,0,0,SRCAND);
			SelectObject(BitmapDC,Bitmap_back1);
			BitBlt(Memory_BitmapDC,1024,467,1024,213,BitmapDC,0,213,SRCPAINT);
			BitBlt(Memory_BitmapDC,1024,467,1024,213,BitmapDC,0,0,SRCAND);
			txtdc1=BeginPaint(hWnd,&txtdc2);
			Bw=Bit1.bmWidth;
			Bh=Bit1.bmHeight;
			BitBlt(Memory_BitmapDC,0,0,Bw,Bh,BitmapDC,0,0,SRCCOPY);
			//카운트변수 딜레이
			delay_tm1++;
			if(delay_tm1==5)
			{
				key_sw=0;
				delay_squid++;
				Idle_cnt++;
				Move_cnt++;
				cnt_stone++;
				cnt_j++;
			
				delay_tm1=0;
			}
			//함수호출
			walk();	
			jump();
			robot_fire();
			upfire();
			aim_up();
			bullet();
			squid_search();
			upbullet();
			crash();
			up_crash();
			stone_crash();
			player_crash();
			//dmg_test();

			SetBkMode(Memory_BitmapDC, TRANSPARENT);
			SetTextColor(Memory_BitmapDC,RGB(0,255,0));
			wsprintfW(text,L"게임종료 : ESC	 이동 : ← →  점프 : SPACEBAR  공격 : Z  위쪽 공격 : ↑");
			TextOutW(Memory_BitmapDC,10,10,text,lstrlen(text));
			wsprintfW(text,L"HP : %d",robot_hp);
			TextOutW(Memory_BitmapDC,10,50,text,lstrlen(text));
			wsprintfW(text,L"SCORE : %d",score);
			TextOutW(Memory_BitmapDC,10,30,text,lstrlen(text));
		
			if(score>=250){//해당 스코어 달성시 보스스테이지로 바뀜 
				stage_num=2;
			}
		}
		else if(stage_num==1){// 미션실패
			misson_failed_screen();
		}
		else if(stage_num==2){// 보스 스테이지
			SelectObject(BitmapDC,Bitmap_back2);
			BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCPAINT);
			BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCAND);
			SelectObject(BitmapDC,Bitmap_back1);
			BitBlt(Memory_BitmapDC,0,467,1024,213,BitmapDC,0,213,SRCPAINT);
			BitBlt(Memory_BitmapDC,0,467,1024,213,BitmapDC,0,0,SRCAND);
			SelectObject(BitmapDC,Bitmap_back1);
			BitBlt(Memory_BitmapDC,1024,467,1024,213,BitmapDC,0,213,SRCPAINT);
			BitBlt(Memory_BitmapDC,1024,467,1024,213,BitmapDC,0,0,SRCAND);
			BitBlt(Memory_BitmapDC,0,0,Bw,Bh,BitmapDC,0,0,SRCCOPY);
			txtdc1=BeginPaint(hWnd,&txtdc2);
			delay_tm1++;
			if(delay_tm1==5)
			{
				key_sw=0;
				Idle_cnt++;
				Move_cnt++;
				cnt_j++;
				
				delay_tm1=0;
			}
			delay_tm3++;
			if(delay_tm3==2)
			{
				key_sw=0;
				boss_cnt++;
				delay_tm3=0;
			}
			
			for(int i=0;i<SQUID_NUMBER;i++){
				squid_dead(i);
				squid_x[i]=1500;
				squid_y[i]=1500;
			}
			
			walk();	
			jump();
			robot_fire();
			upfire();
			aim_up();
			bullet();
			upbullet();
			up_crash();
			player_boss_crash();
			boss_appear();
			player_boss_attack_crash();
		
			if(boss_idle_sw==true){
				delay_tm2++;
				if(delay_tm2==10)
				{
					key_sw=0;
					cnt_boss_idle++;
					delay_tm2=0;
				}
				boss_idle();
			}
			if(boss_hp==0){
				stage_num=3;
			}
			
			SetBkMode(Memory_BitmapDC, TRANSPARENT);
			SetTextColor(Memory_BitmapDC,RGB(0,255,0));
			wsprintfW(text,L"게임종료 : ESC	 이동 : ← →  점프 : SPACEBAR  공격 : Z  위쪽 공격 : ↑");
			TextOutW(Memory_BitmapDC,10,10,text,lstrlen(text));
			wsprintfW(text,L"HP : %d",robot_hp);
			TextOutW(Memory_BitmapDC,10,50,text,lstrlen(text));
			wsprintfW(text,L"SCORE : %d",score);
			TextOutW(Memory_BitmapDC,10,30,text,lstrlen(text));
			wsprintfW(text,L"Zoni Doloma : %d",boss_hp);
			TextOutW(Memory_BitmapDC,10,200,text,lstrlen(text));
		}
		
		else if(stage_num==3){
			misson_complete_screen();
		}
		BitBlt(DC1,0,0,1280,720,Memory_BitmapDC,0,0,SRCCOPY);
		SelectObject(BitmapDC,Memory_Bitmap2);
		DeleteObject(Memory_Bitmap1);
		DeleteDC(Memory_BitmapDC);
		DeleteDC(BitmapDC);
		EndPaint(hWnd,&PS1);
		return 0;
	case WM_TIMER:
		key_dwn(wParam);
		
		return 0;
	case WM_MOUSEMOVE:
		if(bNowDraw==true)
		{
			MoveToEx(hdc,mouse_x,mouse_y,NULL);
			mouse_x=LOWORD(lParam);
			mouse_y=HIWORD(lParam);

			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
		mouse_x=LOWORD(lParam);
		mouse_y=HIWORD(lParam);
		bNowDraw=true;
		InvalidateRect(hWnd,NULL,true);
		return 0;
	case WM_KEYUP:
		aim=false;
		walk_sw=false;
		
		jump_sw=false;
		fire_sw=false;
		upfire_sw=false;
		key_sw=0;
		return 0;
	case WM_DESTROY:
		DeleteObject(Bitmap_back1);
		DeleteObject(Bitmap_back2);
		DeleteObject(Bitmap_Misson_Failed);
		DeleteObject(Bitmap_Misson_Complete);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMsg,wParam,lParam));
}
void key_dwn(int dkey){//조작키 설정 함수
	if(GetKeyState(VK_LEFT) & 0x8000){
		x-=7;
		walk_sw=true;
		
	}
	if(GetKeyState(VK_LEFT)&&GetKeyState(0x5A) & 0x8000){
		
		walk_sw=false;
		
		
	}
	if(GetKeyState(VK_LEFT)&&GetKeyState(VK_SPACE) & 0x8000){
		
		walk_sw=false;
		
	}

	if(GetKeyState(VK_RIGHT) & 0x8000){
		x+=7;
		walk_sw=true;
		
	}
	if(GetKeyState(VK_RIGHT)&&GetKeyState(0x5A) & 0x8000){
		walk_sw=false;
		
	}
	if(GetKeyState(VK_LEFT)&&GetKeyState(VK_SPACE) & 0x8000){
		walk_sw=false;
		
		
	}
	
	if(GetKeyState(VK_UP)&&GetKeyState(VK_LEFT) & 0x8000){
		walk_sw=false;
	
		
	}
	if(GetKeyState(VK_UP)&&GetKeyState(VK_RIGHT) & 0x8000){
		walk_sw=false;
		
	}
	if(GetKeyState(VK_UP)&0x8000&&key_sw==0){
		fire_sw=false;
		up_attack_cnt++;
		upfire_sw=true;
		walk_sw=false;
		if(up_attack_cnt>BULLET_MAG)
		{
			up_attack_cnt=0;
		}
		if(up_bullet_sw[up_attack_cnt]==false)
		{
			upfire_sw=true;
			up_bullet_x[up_attack_cnt]=x+35;
			up_bullet_y[up_attack_cnt]=y;
			up_bullet_sw[up_attack_cnt]=true;
		}
		key_sw=1;
	}
	if(GetKeyState(0x5A)&0x8000&&key_sw==0)
	{
		upfire_sw=false;
		attack_cnt++;
		fire_sw=true;
	
		if(attack_cnt>BULLET_MAG)
		{
			attack_cnt=0;
		}
		if(bullet_sw[attack_cnt]==false)
		{
			fire_sw=true;
			bullet_x[attack_cnt]=x+70;
			bullet_y[attack_cnt]=y+28;
			bullet_sw[attack_cnt]=true;
		}
		key_sw=1;
	}
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
	if(GetKeyState(VK_SPACE)&0x8000){
		jump_sw=true;
		walk_sw=false;
		if(sw==0){
			sw=1;
			hero_top=y;
			jump_po=40;
		}
	}
	if(sw==1){
		y-=jump_po;
		jump_po-=3;
		if(y>=hero_top){
			sw=0;
			y=hero_top;
		}
	}
	if(GetKeyState('1') & 0x8000){
		DestroyWindow(hWnd1);
	}
	InvalidateRect(hWnd1,NULL,FALSE);
}

void create_resource()//비트맵 할당
{
		Bitmap_back1=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND5));
		Bitmap_back2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND2));
		Bitmap_Misson_Complete=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_MISSON_COMPLETE));
		Bitmap_Misson_Failed=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_MISSON_FAILED));
		Bitmap_Robot_Idle[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_IDLE1));
		Bitmap_Robot_Idle[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_IDLE2));
		Bitmap_Robot_Idle[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_IDLE3));
		Bitmap_Robot_Idle[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_IDLE4));
		Bitmap_Robot_Walk[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK1));
		Bitmap_Robot_Walk[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK2));
		Bitmap_Robot_Walk[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK3));
		Bitmap_Robot_Walk[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK4));
		Bitmap_Robot_Walk[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK5));
		Bitmap_Robot_Walk[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK6));
		Bitmap_Robot_Walk[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK7));
		Bitmap_Robot_Walk[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK8));
		Bitmap_Robot_Walk[8]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK9));
		Bitmap_Robot_Walk[9]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK10));
		Bitmap_Robot_Walk[10]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK11));
		Bitmap_Robot_Walk[11]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK12));
		Bitmap_Robot_Walk[12]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK13));
		Bitmap_Robot_Walk[13]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK14));
		Bitmap_Robot_Walk[14]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK15));
		Bitmap_Robot_Walk[15]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_WALK16));
		Bitmap_Robot_Jump[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP1));
		Bitmap_Robot_Jump[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP2));
		Bitmap_Robot_Jump[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP3));
		Bitmap_Robot_Jump[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP4));
		Bitmap_Robot_Jump[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP5));
		Bitmap_Robot_Jump[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP6));
		Bitmap_Robot_Jump[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP7));
		Bitmap_Robot_Jump[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_JUMP8));
		Bitmap_Robot_Fire[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_FIRE_MODIFIED1));
		Bitmap_Robot_Fire[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_FIRE_MODIFIED2));
		Bitmap_Robot_Fire[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_FIRE_MODIFIED3));
		Bitmap_Robot_Fire[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_FIRE_MODIFIED4));
		Bitmap_Robot_Fire[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_FIRE_MODIFIED5));
		Bitmap_Robt_Up_Fire[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_UP_FIRE1));
		Bitmap_Robt_Up_Fire[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_UP_FIRE2));
		Bitmap_Robt_Up_Fire[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_UP_FIRE3));
		Bitmap_Robt_Up_Fire[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_UP_FIRE4));
		Bitmap_Robt_Up_Fire[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_UP_FIRE5));
		Bitmap_Robot_Aimup[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP1));
		Bitmap_Robot_Aimup[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP2));
		Bitmap_Robot_Aimup[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP3));
		Bitmap_Robot_Aimup[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP4));
		Bitmap_Robot_Aimup[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP5));
		Bitmap_Robot_Aimup[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_AIMUP6));
		Bitmap_Robot_Damamge[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_DAMAGED1));
		Bitmap_Robot_Damamge[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_ROBOT_DAMAGED2));
		Bitmap_Bullet=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BULLET_REVISE1));
		Bitmap_Up_Bullet=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_UP_BULLET));

		Bitmap_Squid_Move[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE1));
		Bitmap_Squid_Move[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE2));
		Bitmap_Squid_Move[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE3));
		Bitmap_Squid_Move[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE4));
		Bitmap_Squid_Move[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE5));
		Bitmap_Squid_Move[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE6));
		Bitmap_Squid_Move[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE7));
		Bitmap_Squid_Move[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE8));
		Bitmap_Squid_Move[8]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE9));
		Bitmap_Squid_Move[9]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_MOVE10));
		Bitmap_Squid_Damage[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE1));
		Bitmap_Squid_Damage[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE2));
		Bitmap_Squid_Damage[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE3));
		Bitmap_Squid_Damage[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE4));
		Bitmap_Squid_Damage[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE5));
		Bitmap_Squid_Damage[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE6));
		Bitmap_Squid_Damage[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE7));
		Bitmap_Squid_Damage[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE8));
		Bitmap_Squid_Damage[8]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE9));
		Bitmap_Squid_Damage[9]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE10));
		Bitmap_Squid_Damage[10]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE11));
		Bitmap_Squid_Damage[11]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_DAMAGE12));
		Bitmap_Squid_Attack[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_STONE1));
		Bitmap_Squid_Attack[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_SQUID_STONE2));
		
		Bitmap_Boss_Idle[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_IDLE1));
		Bitmap_Boss_Idle[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_IDLE2));
		Bitmap_Boss_Idle[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_IDLE3));

		Bitmap_Boss_Appear[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR1));
		Bitmap_Boss_Appear[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR2));
		Bitmap_Boss_Appear[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR3));
		Bitmap_Boss_Appear[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR4));
		Bitmap_Boss_Appear[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR5));
		Bitmap_Boss_Appear[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR6));
		Bitmap_Boss_Appear[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR7));
		Bitmap_Boss_Appear[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR8));
		Bitmap_Boss_Appear[8]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR9));
		Bitmap_Boss_Appear[9]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR10));
		Bitmap_Boss_Appear[10]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR11));
		Bitmap_Boss_Appear[11]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR12));
		Bitmap_Boss_Appear[12]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR13));
		Bitmap_Boss_Appear[13]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR14));
		Bitmap_Boss_Appear[14]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR15));
		Bitmap_Boss_Appear[15]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR16));
		Bitmap_Boss_Appear[16]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR17));
		Bitmap_Boss_Appear[17]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR18));
		Bitmap_Boss_Appear[18]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR19));
		Bitmap_Boss_Appear[19]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR20));
		Bitmap_Boss_Appear[20]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_BOSS_APPEAR21));
		
		
}
void jump(){//플레이어 점프 함수
	if(jump_sw==true)
	{
		if(cnt_j==0)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,61,BitmapDC,0,61,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,61,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j==1)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,59,BitmapDC,0,59,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,59,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j==2)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,56,BitmapDC,0,56,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,56,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j=3)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,77,BitmapDC,0,77,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,77,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j=4)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,72,BitmapDC,0,72,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,72,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j=5)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,73,56,BitmapDC,0,56,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,73,56,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j=6)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,73,59,BitmapDC,0,59,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,73,59,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_j=7)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Jump[cnt_j]);
			BitBlt(Memory_BitmapDC,x,y,74,61,BitmapDC,0,61,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,61,BitmapDC,0,0,SRCAND);
			cnt_j=0;
		}
	}
	
}
void walk()//플레이어 걷기 함수
{
	if(walk_sw==true)
	{
		if(cnt_w==0||cnt_w==8)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Walk[cnt_w]);
			BitBlt(Memory_BitmapDC,x,y,72,63,BitmapDC,0,63,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,72,63,BitmapDC,0,0,SRCAND);
		}
		else
		{
			SelectObject(BitmapDC,Bitmap_Robot_Walk[cnt_w]);
			BitBlt(Memory_BitmapDC,x,y,72,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,72,62,BitmapDC,0,0,SRCAND);
		}
		cnt_w++;
		if(cnt_w==15)
		{
			cnt_w=0;
			walk_sw=false;
		}
	}
	else if(walk_sw==false&&aim==false&&jump_sw==false&&fire_sw==false&&upfire_sw==false)
	{
		SelectObject(BitmapDC,Bitmap_Robot_Idle[cnt]);
		BitBlt(Memory_BitmapDC,x,y,72,62,BitmapDC,0,62,SRCPAINT);
		BitBlt(Memory_BitmapDC,x,y,72,62,BitmapDC,0,0,SRCAND);
		if(cnt==3)
		{
			cnt=0;
		}
		cnt++;
	
	}
}

void aim_up()//윗방향키 누르면 위쪽으로 총구를 올리는 모션 함수
{
	if(aim==true)
	{
		if(cnt_a==0)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_a==1)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y,70,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,70,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_a==2)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y,64,68,BitmapDC,0,68,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,64,68,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_a==3)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,68,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_a==4)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,68,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_a==5)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Aimup[cnt_a]);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,68,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,0,SRCAND);
			
		}
		cnt_a++;
		if(cnt_a>=5)
		{
			cnt_a=5;
		}
	}
	else if(aim==false)
	{
		cnt_a=0;
	}
}

void squid(int i)//오징어 이동 모션 및 위치 초기화
{
	
	if(Move_cnt==1)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[0]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],117,33,BitmapDC,0,33,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],117,33,BitmapDC,0,0,SRCAND);
	}
	else if(Move_cnt==2)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[1]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],115,30,BitmapDC,0,30,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],115,30,BitmapDC,0,0,SRCAND);
	}
	else if(Move_cnt==3)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[2]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],113,33,BitmapDC,0,33,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],113,33,BitmapDC,0,0,SRCAND);
	}
	else if(Move_cnt==4)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[3]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],107,37,BitmapDC,0,37,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],107,37,BitmapDC,0,0,SRCAND);
		squid_speed[i]=3;
	}
	else if(Move_cnt==5)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[4]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],99,37,BitmapDC,0,37,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],99,37,BitmapDC,0,0,SRCAND);
		squid_speed[i]=2;
		
	}
	else if(Move_cnt==6)
	{
		
		SelectObject(BitmapDC,Bitmap_Squid_Move[5]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],91,41,BitmapDC,0,41,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],91,41,BitmapDC,0,0,SRCAND);
	}
	else if(Move_cnt==7)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[6]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],86,48,BitmapDC,0,48,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],86,48,BitmapDC,0,0,SRCAND);
	}
	else if(Move_cnt==8)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[7]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],95,40,BitmapDC,0,40,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],95,40,BitmapDC,0,0,SRCAND);
		squid_speed[i]=(int)(rand()%(14-7)+3);
	}
	else if(Move_cnt==9)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[8]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],107,34,BitmapDC,0,34,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],107,34,BitmapDC,0,0,SRCAND);
		
	}
	else if(Move_cnt==10)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Move[9]);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],115,28,BitmapDC,0,28,SRCPAINT);
		BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],115,28,BitmapDC,0,0,SRCAND);
		Move_cnt=1;
	}

	if(squid_x[i]<0)
	{
		squid_y[i]=((int)(rand()%(600-100))+100);
		squid_x[i]=(int)(rand()%(1800-900)+1400);
		score-=10;
		if(score<0){
			score=0;
		}
	}

}
void squid_search(){

	for(int i=0;i<SQUID_NUMBER;i++){
		stone_x[i]=squid_x[i]+60;
		stone_y[i]+=8;
		if(cnt_stone==1)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Attack[0]);
			BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,22,SRCPAINT);
			BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,0,SRCAND);
		
		}
		else if(cnt_stone==2)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Attack[1]);
			BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,22,SRCPAINT);
			BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,0,SRCAND);
			cnt_stone=1;
		}
		if(stone_y[i]>1440)//y가720까지지만 딜레이를 주기위해 1440까지내려가면 다시 돌 생성
		{
			stone_y[i]=squid_y[i]+40;
		}	
	}
}
void squid_attack(int i)//돌 스폰
{
	if(cnt_stone==1)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Attack[0]);
		BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,22,SRCPAINT);
		BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,0,SRCAND);
	}
	else if(cnt_stone==2)
	{
		SelectObject(BitmapDC,Bitmap_Squid_Attack[1]);
		BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,22,SRCPAINT);
		BitBlt(Memory_BitmapDC,stone_x[i],stone_y[i],23,22,BitmapDC,0,0,SRCAND);	
	}
	if(stone_y[i]>1440)//y가720까지지만 딜레이를 주기위해 1440까지내려가면 다시 돌 생성
	{
		stone_y[i]=squid_y[i]+40;	
	}
}
void stone_crash()//오징어 공격 충돌검사
{
	for(int i=0;i<SQUID_NUMBER;i++)
	{
		if((stone_x[i]+23)>x&&(stone_x[i])<(x+74)&&(stone_y[i]+22)>
			y&&(stone_y[i])<(y+62)&&stone_sw[i]==true)
		{
			robot_damage();
			stone_y[i]=squid_y[i]+40;
			stone_x[i]=squid_x[i]+60;			
		}
	}
}
void squid_dead(int i){//오징어 사망 모션
	for(cnt_d=0;cnt_d<12;cnt_d++)
	{
		if(cnt_d==0)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],98,39,BitmapDC,0,39,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],98,39,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==1)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,46,BitmapDC,0,46,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,46,BitmapDC,0,0,SRCAND);
				
		}
		else if(cnt_d==2)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],66,45,BitmapDC,0,45,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],66,45,BitmapDC,0,0,SRCAND);
				
		}
		else if(cnt_d==3)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],64,43,BitmapDC,0,43,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],64,43,BitmapDC,0,0,SRCAND);
				
		}
		else if(cnt_d==4)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],73,46,BitmapDC,0,46,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],73,46,BitmapDC,0,0,SRCAND);
				
		}
		else if(cnt_d==5)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],73,47,BitmapDC,0,47,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],73,47,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==6)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,58,BitmapDC,0,58,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,58,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==7)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,55,BitmapDC,0,55,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,55,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==8)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,56,BitmapDC,0,58,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,56,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==9)
		{
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,56,BitmapDC,0,56,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],82,56,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_d==10){
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],49,53,BitmapDC,0,53,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],49,53,BitmapDC,0,0,SRCAND);
		}else if(cnt_d==11){
			SelectObject(BitmapDC,Bitmap_Squid_Damage[cnt_d]);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],48,54,BitmapDC,0,54,SRCPAINT);
			BitBlt(Memory_BitmapDC,squid_x[i],squid_y[i],48,54,BitmapDC,0,0,SRCAND);
		}

	}
	
}

void robot_damage()
{
	
	for(int i=0;i<3;i++)
	{
		if(i==0)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Damamge[0]);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,0,SRCAND);
		}
		else if(i==2)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Damamge[1]);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,0,SRCAND);
				
		}
	}
	robot_hp--;
	score-=300;

	
	if(robot_hp==0){
		stage_num=1;
	}
	
	if(score<0){
		score=0;
	}
}
void robot_fire()//플레이어 공격 모션
{
	if(fire_sw==true)
	{
		if(cnt_f==0)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Fire[cnt_f]);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,74,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_f==1)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Fire[cnt_f]);
			BitBlt(Memory_BitmapDC,x,y,96,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,96,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_f==2)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Fire[cnt_f]);
			BitBlt(Memory_BitmapDC,x,y,107,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,107,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_f==3)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Fire[cnt_f]);
			BitBlt(Memory_BitmapDC,x,y,106,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,106,62,BitmapDC,0,0,SRCAND);
		}
		else if(cnt_f==4)
		{
			SelectObject(BitmapDC,Bitmap_Robot_Fire[cnt_f]);
			BitBlt(Memory_BitmapDC,x,y,105,62,BitmapDC,0,62,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y,105,62,BitmapDC,0,0,SRCAND);
		}
		cnt_f++;
		if(cnt_f>=4){
			cnt_f=0;
		}
	}else if(fire_sw==false){
		cnt_f=0;			
	}
}
void bullet(){//총알 비트맵
	
	for(int i=0;i<BULLET_MAG;i++)
	{

		if(bullet_sw[i]==true)
		{
			SelectObject(BitmapDC,Bitmap_Bullet);
			bullet_x[i]+=10;
			BitBlt(Memory_BitmapDC,bullet_x[i],bullet_y[i],30,5,BitmapDC,0,5,SRCPAINT);
			BitBlt(Memory_BitmapDC,bullet_x[i],bullet_y[i],30,5,BitmapDC,0,0,SRCAND);
			if(bullet_x[i]>1280)
			{
				bullet_sw[i]=false;
			}
		}
	}
		
			
}

void crash()//충돌검사
{

	for(int i=0;i<SQUID_NUMBER;i++)
	{
		squid_x[i]-=squid_speed[i];
		for(int j=0;j<BULLET_MAG;j++)
		{
			
			if((bullet_x[j]+30)>squid_x[i]&&(bullet_x[j])<(squid_x[i]+117)&&(bullet_y[j]+5)>
				squid_y[i]&&(bullet_y[j])<(squid_y[i]+80)&&bullet_sw[j]==true)
			{
				squid_hp[i]-=BULLET_DAM;
				bullet_sw[j]=false;	
				if(squid_hp[i]==0){
					squid_dead(i);
					squid_y[i]=((int)(rand()%(600-100))+100);
					squid_x[i]=(int)(rand()%(1800-900)+1400);
					squid_hp[i]=SQUID_HP;
					score+=100;

						
				}
				
			}
			else
			{
				squid(i);

			}
		}
	}

	
}
//상단으로 발사하는 총알과의 충돌검사 및 적 사망모션
void up_crash(){
	for(int i=0;i<SQUID_NUMBER;i++)
	{
		for(int j=0;j<BULLET_MAG;j++)
		{
			if((up_bullet_x[j]+5)>squid_x[i]&&(up_bullet_x[j])<(squid_x[i]+117)&&(up_bullet_y[j]+30)>squid_y[i]&&(up_bullet_y[j])<(squid_y[i]+96)&&up_bullet_sw[j]==true)
			{
				squid_hp[i]-=BULLET_DAM;
				up_bullet_sw[j]=false;
				if(squid_hp[i]==0){

					squid_dead(i);
					squid_y[i]=((int)(rand()%(600-100))+100);
					squid_x[i]=(int)(rand()%(1800-900)+1400);
					squid_hp[i]=SQUID_HP;
					score+=100;
					
				}
				
			}
			else
			{
				squid(i);

			}
		}
	}
}
void player_crash()//플레이어 피격 충돌검사
{

	for(int i=0;i<SQUID_NUMBER;i++)
	{
		
		if((squid_x[i]+107)>x&&(squid_x[i])<(x+50)&&(squid_y[i]+86)>
			y&&(squid_y[i])<(y+40)==true)
		{
			squid_dead(i);
			robot_damage();
			squid_y[i]=((int)(rand()%(600-100))+100);
			squid_x[i]=(int)(rand()%(1800-900)+1400);
		}
	}
}
void player_boss_crash(){
	if((boss_x-270+329)>x&&(boss_x-270)<(x+70)&&(boss_y+155+115)>y&&(boss_y+155)<(y+60)==true)
	{
		robot_damage();
		
	}
}
void player_boss_attack_crash(){
	for(int j=0;j<BULLET_MAG;j++){
		if((bullet_x[j]+30)>boss_x-270&&(bullet_x[j])<(boss_x-270+329)&&(bullet_y[j]+5)>boss_y+155&&(bullet_y[j])<(boss_y+155+115)&&bullet_sw[j]==true)
		{
			bullet_sw[j]=false;
			boss_hp-=BULLET_DAM;
		}
	}
}
//위로 총알이 나가는 함수
void upbullet(){
	for(int i=0;i<BULLET_MAG;i++)
	{

		if(up_bullet_sw[i]==true)
		{
			SelectObject(BitmapDC,Bitmap_Up_Bullet);
			up_bullet_y[i]-=10;
			BitBlt(Memory_BitmapDC,up_bullet_x[i]+10,up_bullet_y[i]-10,5,30,BitmapDC,0,30,SRCPAINT);
			BitBlt(Memory_BitmapDC,up_bullet_x[i]+10,up_bullet_y[i]-10,5,30,BitmapDC,0,0,SRCAND);
		
			if(up_bullet_y[i]<0)
			{
				up_bullet_sw[i]=false;
			}
		}
		
	}
}
//위로 공격 함수
void upfire(){
	if(upfire_sw==true)
	{
		if(cnt_upf==0){
			SelectObject(BitmapDC,Bitmap_Robt_Up_Fire[cnt_upf]);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,68,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-6,64,68,BitmapDC,0,0,SRCAND);
		}else if(cnt_upf==1){
			SelectObject(BitmapDC,Bitmap_Robt_Up_Fire[cnt_upf]);
			BitBlt(Memory_BitmapDC,x,y-31,64,93,BitmapDC,0,93,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-31,64,93,BitmapDC,0,0,SRCAND);
		}else if(cnt_upf==2){
			SelectObject(BitmapDC,Bitmap_Robt_Up_Fire[cnt_upf]);
			BitBlt(Memory_BitmapDC,x,y-43,64,105,BitmapDC,0,105,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-43,64,105,BitmapDC,0,0,SRCAND);
		}else if(cnt_upf==3){
			SelectObject(BitmapDC,Bitmap_Robt_Up_Fire[cnt_upf]);
			BitBlt(Memory_BitmapDC,x,y-42,66,104,BitmapDC,0,104,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-42,66,104,BitmapDC,0,0,SRCAND);
		}else if(cnt_upf==4){
			SelectObject(BitmapDC,Bitmap_Robt_Up_Fire[cnt_upf]);
			BitBlt(Memory_BitmapDC,x,y-38,64,100,BitmapDC,0,100,SRCPAINT);
			BitBlt(Memory_BitmapDC,x,y-38,64,100,BitmapDC,0,0,SRCAND);
		}
		cnt_upf++;
		if(cnt_upf>4){
			cnt_upf=0;
		}
	}else if(upfire_sw==false){
		cnt_upf=0;
	}
}
void misson_failed_screen(){
	SelectObject(BitmapDC,Bitmap_Misson_Failed);
	BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCPAINT);
	BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCAND);
}
void misson_complete_screen(){
	SelectObject(BitmapDC,Bitmap_Misson_Complete);
	BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCPAINT);
	BitBlt(Memory_BitmapDC,0,0,1280,720,BitmapDC,0,0,SRCAND);
}
void boss_idle(){
	if(cnt_boss_idle==0){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,329,115,BitmapDC,0,115,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,329,115,BitmapDC,0,0,SRCAND);
	}
	if(cnt_boss_idle==1){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,327,118,BitmapDC,0,118,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,327,118,BitmapDC,0,0,SRCAND);
		boss_x-=0.1;
	}
	if(cnt_boss_idle==2){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,326,120,BitmapDC,0,120,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,326,120,BitmapDC,0,0,SRCAND);
		cnt_boss_idle=0;
	}
	
	
}
void boss_appear(){
	if(boss_cnt==0){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x,boss_y+200,85,42,BitmapDC,0,42,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x,boss_y+200,85,42,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==1){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x,boss_y+185,143,87,BitmapDC,0,87,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x,boss_y+185,143,87,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==2){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-50,boss_y+150,172,132,BitmapDC,0,132,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-50,boss_y+150,172,132,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==3){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-80,boss_y+100,184,158,BitmapDC,0,158,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-80,boss_y+100,184,158,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==4){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-90,boss_y+80,203,182,BitmapDC,0,182,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-90,boss_y+80,203,182,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==5){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-100,boss_y,235,205,BitmapDC,0,205,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-100,boss_y,235,205,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==6){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-150,boss_y,248,223,BitmapDC,0,223,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-150,boss_y,248,223,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==7){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-150,boss_y,267,234,BitmapDC,0,234,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-150,boss_y,267,234,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==8){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-170,boss_y,334,188,BitmapDC,0,188,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-170,boss_y,334,188,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==9){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-180,boss_y+80,342,149,BitmapDC,0,149,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-180,boss_y+80,342,149,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==10){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-190,boss_y+55,318,236,BitmapDC,0,236,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-190,boss_y+55,318,236,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==11){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y+35,240,266,BitmapDC,0,266,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y+35,240,266,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==12){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y,187,262,BitmapDC,0,262,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y,187,262,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==13){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y+35,217,238,BitmapDC,0,238,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-210,boss_y+35,217,238,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==14){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-230,boss_y+55,258,185,BitmapDC,0,185,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-230,boss_y+55,258,185,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==15){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-240,boss_y+80,290,136,BitmapDC,0,136,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-240,boss_y+80,290,136,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==16){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-250,boss_y+95,316,110,BitmapDC,0,110,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-250,boss_y+95,316,110,BitmapDC,0,0,SRCAND);
	}else if(boss_cnt==17){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-260,boss_y+110,320,107,BitmapDC,0,107,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-260,boss_y+110,320,107,BitmapDC,0,0,SRCAND);
		boss_idle_sw=true;
	}else if(boss_cnt==18){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-265,boss_y+125,327,108,BitmapDC,0,108,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-265,boss_y+125,327,108,BitmapDC,0,0,SRCAND);
		
	}else if(boss_cnt==19){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+140,329,107,BitmapDC,0,107,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+140,329,107,BitmapDC,0,0,SRCAND);
		
	}else if(boss_cnt==20){
		SelectObject(BitmapDC,Bitmap_Boss_Appear[boss_cnt]);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,329,113,BitmapDC,0,113,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x-270,boss_y+155,329,113,BitmapDC,0,0,SRCAND);
		boss_cnt=21;
		
	}
	
	
	
}


// 테스트 함수
void dmg_test(){
	cnt_boss_idle=0;
	if(cnt_boss_idle==0){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,329,115,BitmapDC,0,115,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,329,115,BitmapDC,0,0,SRCAND);
	}
	if(cnt_boss_idle==1){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,327,118,BitmapDC,0,118,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,327,118,BitmapDC,0,0,SRCAND);
	}
	if(cnt_boss_idle==2){
		SelectObject(BitmapDC,Bitmap_Boss_Idle[cnt_boss_idle]);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,326,120,BitmapDC,0,120,SRCPAINT);
		BitBlt(Memory_BitmapDC,boss_x,boss_y,326,120,BitmapDC,0,0,SRCAND);
		cnt_boss_idle=0;
	}
}
//Chowmein-Conga Giant Hunter Walker Zoni Doloma 보스몹킵