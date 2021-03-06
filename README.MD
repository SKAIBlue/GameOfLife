# Game of life (생명 게임)
## Cellular Automata

셀룰러 오토마타를 이용해 만들어진 생명 게임입니다. </br>
It is Game of life using by cellular automata.

## Struct
<pre>
typedef struct WORLD
{
	char* world;        // Two-dimensional array for save cell satus. 
	int width;          // world width
	int height;         // world height
}World;
</pre>

## Functions
<pre>
//////////////////////////////////////////////////////////////////////////
// 셀들이 활동할 월드를 생성합니다.
// width		너비 min = 1 max = 60
// height		높이 min = 1 max = 29
// return		생성된 월드
World* CreateWorld(int width, int height);
</pre>

<pre>
//////////////////////////////////////////////////////////////////////////
// 월드를 복사합니다.
// world		셀들이 활동하는 세계
// return		복사된 world
World* CopyWorld(World* world);
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 설명과 함께 화면을 출력합니다.
// world		셀들이 활동하는 월드
// width		너비
// height		높이
// print		출력 함수포인터
void PrintWorld(World* world, void(*print)(int value));
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 커서를 이동합니다.
// x			이동할 커서 위치 x
// y			이동할 커서 위치 y
void MoveCursor(int x, int y);
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 입력을 처리합니다.
// input		입력
// cx			커서 위치 x
// cy			커서 위치 y
void InputProcess(char input, World* world, int * cx, int * cy);
</pre>

<pre>
//////////////////////////////////////////////////////////////////////////
// 다음 상태
// world		월드
void NextState(World* world);
</pre>

<pre>
//////////////////////////////////////////////////////////////////////////
// 전이 함수
// now		현재 셀의 값
// left		왼쪽 셀의 값
// top		위쪽 셀의 값
// right	오른쪽 셀의 값
// bottom	아래쪽 셀의 값
// return	다음 값
char TransferFunction(char now, char left, char top, char right, char bottom);
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 값의 출력 형태 지정
// value	값
void PrintValue(int value);
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 스레드로 작동하는 함수
// world	셀이 활동하는 월드
int Run(World* world);
</pre>


<pre>
//////////////////////////////////////////////////////////////////////////
// 월드의 특정 위치의 값을 설정합니다.
// world	셀이 활동하는 월드
// x		값을 변경할 x 좌표
// y		값을 변경할 y 좌표
// value	변경할 값
void SetValue(World* world, int x, int y, int value);
</pre>

<pre>
//////////////////////////////////////////////////////////////////////////
// 월드의 특정 위치의 값을 가져옵니다.
// world	셀이 활동하는 월드
// x		값을 변경할 x 좌표
// y		값을 변경할 y 좌표
// return	월드(x, y) 좌표의 값
int GetValue(World* world, int x, int y);

</pre>