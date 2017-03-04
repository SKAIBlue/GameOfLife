#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

#define	TRUE		1
#define	FALSE		0
#define SLEEP_TIME	100


typedef struct WORLD
{
	char* world;
	int width;
	int height;
}World;



//////////////////////////////////////////////////////////////////////////
// 셀들이 활동할 월드를 생성합니다.
// width		너비 min = 1 max = 60
// height		높이 min = 1 max = 29
// return		생성된 월드
World* CreateWorld(int width, int height);


//////////////////////////////////////////////////////////////////////////
// 월드를 복사합니다.
// world		셀들이 활동하는 세계
// return		복사된 world
World* CopyWorld(World* world);




//////////////////////////////////////////////////////////////////////////
// 화면을 출력합니다.
// world		셀들이 활동하는 월드
// width		너비
// height		높이
// print		출력 함수포인터
void PrintWorld(World* world, void(*print)(int value));




//////////////////////////////////////////////////////////////////////////
// 커서를 이동합니다.
// x			이동할 커서 위치 x
// y			이동할 커서 위치 y
void MoveCursor(int x, int y);




//////////////////////////////////////////////////////////////////////////
// 입력을 처리합니다.
// input		입력
// cx			커서 위치 x
// cy			커서 위치 y
void InputProcess(char input, World* world, int * cx, int * cy);



//////////////////////////////////////////////////////////////////////////
// 다음 상태
// world		월드
void NextState(World* world);



//////////////////////////////////////////////////////////////////////////
// 전이 함수
// now		현재 셀의 값
// left		왼쪽 셀의 값
// top		위쪽 셀의 값
// right	오른쪽 셀의 값
// bottom	아래쪽 셀의 값
// return	다음 값
char TransferFunction(char now, char left, char top, char right, char bottom);



//////////////////////////////////////////////////////////////////////////
// 값의 출력 형태 지정
// value	값
void PrintValue(int value);



//////////////////////////////////////////////////////////////////////////
// 스레드로 작동하는 함수
// world	셀이 활동하는 월드
int Run(World* world);


//////////////////////////////////////////////////////////////////////////
// 월드의 특정 위치의 값을 설정합니다.
// world	셀이 활동하는 월드
// x		값을 변경할 x 좌표
// y		값을 변경할 y 좌표
// value	변경할 값
void SetValue(World* world, int x, int y, int value);



//////////////////////////////////////////////////////////////////////////
// 월드의 특정 위치의 값을 가져옵니다.
// world	셀이 활동하는 월드
// x		값을 변경할 x 좌표
// y		값을 변경할 y 좌표
// return	월드(x, y) 좌표의 값
int GetValue(World* world, int x, int y);



int isThreadWorking = FALSE;

char ch = 'e';




void main()
{
	World* world = CreateWorld(100, 100);

	int cx = 0, cy = 0;

	PrintWorld(world, PrintValue);
	MoveCursor(cx, cy);
	do 
	{
		ch = getch();
		InputProcess(ch, world, &cx, &cy);
		MoveCursor(cx, cy);
	} while (ch != 'E' && ch != 'e');
}



World * CreateWorld(int width, int height)
{
	if (width >= 60)
	{
		width = 60;
	}
	else if (width < 1)
	{
		width = 1;
	}
	if (height >= 29)
	{
		height = 29;
	}
	else if (height < 1)
	{
		height = 1;
	}

	World* world = (World*)malloc(sizeof(World));
	world->width = width;
	world->height = height;

	int size = width * height;

	world->world = (char*)malloc(sizeof(char) * size);

	for (int i = 0; i < size; i += 1)
	{
		*(world->world + i) = '0';
	}

	return world;
}



World * CopyWorld(World * world)
{
	int size = world->width * world->height;
	World* cpy = (World*)malloc(sizeof(World));
	cpy->world = (char*)malloc(sizeof(char) * size);
	cpy->width = world->width;
	cpy->height = world->height;
	for (int i = 0; i < size; i += 1)
	{
		*(cpy->world + i) = *(world->world + i);
	}
	return cpy;
}





void PrintWorld(World* world, void(*print)(int value))
{
	system("cls");
	for (int i = 0; i < world->height; i += 1)
	{
		for (int j = 0; j < world->width; j += 1)
		{
			print(GetValue(world, j, i));
		}
		printf("\n");
	}
	for (int i = 0; i < 29 - world->height; i += 1)
	{
		printf("\n");
	}
	printf("커서이동 W: 위 S: 아래 A: 왼쪽 D: 오른쪽   N: 다음 상태 전이  1~9: 값 변경   E: 종료  T: 스레드 시작  Y: 스레드 중지");
}



void MoveCursor(int x, int y)
{
	COORD Pos;
	Pos.X = x * 2;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}




void InputProcess(char input, World* world, int * cx, int * cy)
{
	switch (input)
	{
	case 'W':
	case 'w':
		*cy = (*cy - 1 + world->height) % world->height;
		return;
	case 'A':
	case 'a':
		*cx = (*cx - 1 + world->width) % world->width;
		return;
	case 'S':
	case 's':
		*cy = (*cy + 1 ) % world->height;
		return;
	case 'D':
	case 'd':
		*cx = (*cx + 1 ) % world->width;
		return;
	case 'N':
	case 'n':
		if (!isThreadWorking)
		{
			NextState(world);
			PrintWorld(world, PrintValue);
		}
		return;
	case 'T':
	case 't':
		if (!isThreadWorking)
		{
			_beginthread(Run, 0, world);
			isThreadWorking = TRUE;
		}
		
		return;
	case 'Y':
	case 'y':
		return;
	case 'E':
	case 'e':
		return;
	}
	if (input >='0' && input <= '9')
	{
		SetValue(world, *cx, *cy, input);
		PrintWorld(world, PrintValue);
	}
}



void NextState(World * world)
{
	World* temp = CopyWorld(world);
	for (int i = 0; i < world->height; i += 1)
	{
		for (int j = 0; j < world->width; j += 1)
		{
			int leftx = (j - 1 + world->width) % world->width;
			int topy = (i - 1 + world->height) % world->height;
			char now = GetValue(temp, j, i);
			char left = GetValue(temp, leftx, i);
			char top = GetValue(temp, j, topy);
			char right = GetValue(temp, (j + 1) % world->width, i);
			char bottom = GetValue(temp, j, (i + 1) % world->height);
			SetValue(world, j, i, TransferFunction(now, left, top, right, bottom));
		}
	}
	free(temp);
}



char TransferFunction(char now, char left, char top, char right, char bottom)
{
	int count = 0;

	if (left != '0')
	{
		count += 1;
	}
	if (top != '0')
	{
		count += 1;
	}
	if (right != '0')
	{
		count += 1;
	}
	if (bottom != '0')
	{
		count += 1;
	}
	if (count == 2 || count == 3)
	{
		return '1';
	}
	else
	{
		return '0';
	}
}

void PrintValue(int value)
{
	switch (value)
	{
	case '0':
		printf("□");
		return;
	default:
		printf("■");
		return;
	}
}

int Run(void* p)
{
	World* world = (World*)p;
	while (ch!='Y' && ch!= 'y' && ch !='E'&& ch != 'e')
	{
		NextState(world);
		PrintWorld(world, PrintValue);
		Sleep(SLEEP_TIME);
	}
	isThreadWorking = FALSE;
	return 0;
}

void SetValue(World * world, int x, int y, int value)
{
	*(world->world + (world->width * y + x)) = value;
}

int GetValue(World * world, int x, int y)
{
	return *(world->world + (world->width * y + x));
}

