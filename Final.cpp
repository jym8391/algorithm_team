// 미로 배열 수정, 뚫어야 하는 벽: 2, 뚫을 수 있는 횟수: 2

#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;
#define WHITE 15
#define MAZE_SIZE 20
#define X_AXIS 50
#define Y_AXIS 12

struct position {
    int x, y;
};

position stPlayer;
position prestPlayer;

bool HaveKey = false;
int CanBreak = 2;
int dx[4] = { 0,0,-1,1 };
int dy[4] = { 1,-1,0,0 };

//콘솔창 초기화
void Clear(void) {
    system("cls");
}

//커서 위치 이동
void GotoXY(short x, short y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//통로 확인
bool isValid(int Maze[MAZE_SIZE][MAZE_SIZE]) {
    if (Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 0 || Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 3 || Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 6) {
        return false;
    }
    if (Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 5 && HaveKey == false) {
        return false;
    }
    return true; //통로이면
}

//Hint isvalid
bool H_isValid(int Maze[MAZE_SIZE][MAZE_SIZE], position HstPlayer, bool& havekey) {
    if (Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 0 || Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 3) {
        return false;
    }
    if (Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 5 && havekey == false) {
        return false;
    }
    return true; //통로이면
}

//미로 탈출 여부
bool MazeEscape(int Maze[MAZE_SIZE][MAZE_SIZE]) {
    if (Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 5 && HaveKey) {
        return true; //열쇠를 가지고 탈출한 경우
    }
    return false;
}

//미로 생성
void drawMaze(int Maze[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        GotoXY(X_AXIS, Y_AXIS + i);
        for (int j = 0; j < MAZE_SIZE; j++) {
            switch (Maze[i][j]) {
            case 0: //벽
                printf("██");
                break;
            case 1: //통로
                printf("  ");
                break;
            case 2: //탈출 경로
                printf("● ");
                break;
            case 3: //부술 수 있는 벽
                printf("▒▒");
                break;
            case 4: //열쇠
                printf("K "); //🔑 key
                break;
            case 5: //탈출구
                printf("E ");//🔒 Escape
                break;
            case 6: //힌트
                printf("H ");
                break;
            case 10: //플레이어 현재 위치
                printf("P ");
                break;
            default:
                printf("");
                break;
            }
        }
    }
}

//플레이어 위치 설정
void Player(int Maze[MAZE_SIZE][MAZE_SIZE]) {
    Maze[prestPlayer.y - Y_AXIS][int(prestPlayer.x - X_AXIS) / 2] = 1;
    Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] = 10;
}

//벽 부수기
void BreakWall(int Maze[MAZE_SIZE][MAZE_SIZE]) {
    GotoXY(0, 4);
    bool flag = false; //벽을 부쉈는지 여부
    if (CanBreak > 0) {
        for (int i = 0; i < 4; i++) {
            int nx = stPlayer.x + 2 * dx[i];
            int ny = stPlayer.y + dy[i];
            if (Maze[ny - Y_AXIS][int(nx - X_AXIS) / 2] == 3 || Maze[ny - Y_AXIS][int(nx - X_AXIS) / 2] == 6) {
                Maze[ny - Y_AXIS][int(nx - X_AXIS) / 2] = 1;
                flag = true;
            }
        }
        if (flag) CanBreak--;
    }
    //벽이 아닌 곳을 부수라고 했을 경우 -> 
    if (CanBreak > 0 && flag == false) {
        printf("                      ");
        GotoXY(0, 5);
        printf("부술 수 없는 벽입니다!");
        Sleep(50);
        GotoXY(0, 5);
        printf("                      ");
    }
    return;
}

//힌트 경로 출력
void PrintSolve(int Solve[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        GotoXY(0, 7 + i);
        for (int j = 0; j < MAZE_SIZE; j++) {
            printf("%d ", Solve[i][j]);
        }
        printf("\n");
    }

}

//탈출 경로 초기화
void InitSolve(int Solve[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            Solve[i][j] = 0;
        }
    }
}

//백트래킹 힌트 사용
int Hint1(int Maze[MAZE_SIZE][MAZE_SIZE], int Solve[MAZE_SIZE][MAZE_SIZE], position HstPlayer, bool& havekey) {
    GotoXY(0, 6);
    printf("havekey : %d", havekey);

    //PrintSolve(Solve); //백트래킹 과정 확인하는 함수
    //Sleep(10);

    //if (!havekey) { // havekey 없을때 hint 사용 불가
    //    return 0;
    //}

    if (Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 4) {
        havekey = true;
        return 1;
    }

    if (H_isValid(Maze, HstPlayer, havekey)) {
        if (Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 2) {
            return 0;
        }

        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 2;

        for (int i = 0; i < 4; i++) {
            int nx = HstPlayer.x + 2 * dx[i];
            int ny = HstPlayer.y + dy[i];
            if (Hint1(Maze, Solve, { nx, ny }, havekey) == 1) {
                return 1;
            }
        }

        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 1; // if there is no move, backtracking
    }

    return 0;
}

int Hint2(int Maze[MAZE_SIZE][MAZE_SIZE], int Solve[MAZE_SIZE][MAZE_SIZE], int canbreak, position HstPlayer, bool& havekey) { // havekey 존재 여부 추가
    GotoXY(0, 6);
    printf("havekey : %d", havekey);

    //PrintSolve(Solve); //백트래킹 과정 확인하는 함수
    //Sleep(10);

    if (!havekey) { // havekey 없을때 hint 사용 불가
        return 0;
    }

    if (Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 5) {
        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 2;
        return 1;
    }

    if (canbreak && Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 3) { //벽을 부술 수 있다면, 벽을 부수고 움직여라
        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 2;
        Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 1; // 벽 부수기

        for (int i = 0; i < 4; i++) {
            int nx = HstPlayer.x + 2 * dx[i];
            int ny = HstPlayer.y + dy[i];
            if (Hint2(Maze, Solve, canbreak - 2, { nx,ny }, havekey) == 1) {
                Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 6; //H 힌트 표시
                return 1;
            }
        }
        // reset the wall for backtracking
        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 1;
        Maze[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 3;
    }

    if (H_isValid(Maze, HstPlayer, havekey)) {
        if (Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] == 2) {
            return 0;
        }

        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 2;

        for (int i = 0; i < 4; i++) {
            int nx = HstPlayer.x + 2 * dx[i];
            int ny = HstPlayer.y + dy[i];
            if (Hint2(Maze, Solve, canbreak, { nx,ny }, havekey) == 1) {
                return 1;
            }
        }

        Solve[HstPlayer.y - Y_AXIS][int(HstPlayer.x - X_AXIS) / 2] = 1; // if there is no move, backtracking
    }

    return 0;
}

int Hint(int Maze[MAZE_SIZE][MAZE_SIZE], int Solve[MAZE_SIZE][MAZE_SIZE], int canbreak, position HstPlayer) {
    bool havekey = HaveKey;
    if (!havekey) {
        Hint1(Maze, Solve, HstPlayer, havekey); //열쇠가 없으면 탐색
    }

    if (havekey) { // Hint 1을 실행하여 열쇠를 찾았거나 이미 열쇠가 있는 경우
        InitSolve(Solve);
        int Hint2Result = Hint2(Maze, Solve, canbreak, HstPlayer, havekey);
        return Hint2Result; // Hint 2 실행가능
    }
    return 0;
}

//키보드 입력
void InputProcess(int Maze[MAZE_SIZE][MAZE_SIZE], int Solve[MAZE_SIZE][MAZE_SIZE]) {
    prestPlayer.x = stPlayer.x;
    prestPlayer.y = stPlayer.y;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000) { //힌트 사용
        position HstPlayer;
        HstPlayer.x = stPlayer.x;
        HstPlayer.y = stPlayer.y;
        Hint(Maze, Solve, 1, HstPlayer);
        //PrintSolve(Solve);

    }
    if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) { //벽 부수기
        BreakWall(Maze);
    }
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { //디버깅용 열쇠 획득키
        HaveKey = true;
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //왼쪽 키 입력
        stPlayer.x -= 2;
        if (!isValid(Maze)) stPlayer.x += 2;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //오른쪽 키 입력
        stPlayer.x += 2;
        if (!isValid(Maze)) stPlayer.x -= 2;

    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) { //위쪽 키 입력
        stPlayer.y -= 1;
        if (!isValid(Maze)) stPlayer.y += 1;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //아래쪽 키 입력
        stPlayer.y += 1;
        if (!isValid(Maze)) stPlayer.y -= 1;
    }

    GotoXY(0, 0);
    printf("현재 위치  : %d, %d", int(stPlayer.x - X_AXIS) / 2, stPlayer.y - Y_AXIS);
    GotoXY(0, 1);
    printf("벽을 부술 수 있는 횟수 : %d", CanBreak);

    if (Maze[stPlayer.y - Y_AXIS][int(stPlayer.x - X_AXIS) / 2] == 4) {
        HaveKey = true; //열쇠 획득한 경우
        GotoXY(0, 2);
        if (HaveKey) printf("현재 열쇠를 보유중입니다.\n");
    }
    if (CanBreak <= 0) {
        GotoXY(0, 4);
        printf("벽을 더 이상 부술 수 없습니다!");

    }
    GotoXY(stPlayer.x, stPlayer.y);
    if (MazeEscape(Maze)) return;
    if (isValid(Maze)) Player(Maze);
    Sleep(50);
}

//글자 색깔 변경
void SetColor(unsigned char _BgColor, unsigned char _TextColor)
{
    unsigned short ColorNum = (_BgColor << 4) | _TextColor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNum);
}

//콘솔 크기 조정
void SetConsoleSize(int _col, int _lines)
{
    char setText[100];
    sprintf(setText, "mode con cols=%d lines=%d", _col, _lines);
    system(setText);
}

//커서 활성화 여부
void SetCursorView(BOOL _bShow)
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = _bShow;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//초기 설정
void Init(void) {
    Clear();
    SetColor(0, WHITE); //글자 흰색으로 설정
    SetConsoleSize(80, 35); // StartMenu 크기 맞춰서 설정
    SetCursorView(false); //커서 비활성화
    SetConsoleTitle(TEXT("Algorithm termproject : Solving Maze")); // 콘솔 이름
}

//시작 메뉴
void StartMenu(void) {
    Clear();
    while (_kbhit()) _getch(); //키보드 입력을 받음
    while (1) {
        Clear();
        GotoXY(25, 10);
        printf("Algorithm termproject : Solving Maze");
        GotoXY(22, 11);
        printf("미로 찾기 게임입니다.");
        GotoXY(22, Y_AXIS);
        printf("열쇠를 찾아 미로를 탈출해보세요!");
        GotoXY(38, 14);
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) break; //엔터키를 누른다면 종료
        Sleep(50);
    }

}

//플레이어 위치 초기화
void InitPlayerState() {
    stPlayer.x = 52;
    stPlayer.y = 13;
}

//인게임
void GameMain(void) {
    int Maze[MAZE_SIZE][MAZE_SIZE] = { // 0: wall, 1: path, 2: point, 3: breakwall
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
        {0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 3, 1, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 4, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 3, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    int Solve[MAZE_SIZE][MAZE_SIZE] = {};

    SetConsoleSize(140, X_AXIS);
    Clear();
    drawMaze(Maze);
    InitSolve(Solve);
    InitPlayerState();
    CanBreak = 2;
    HaveKey = false;
    while (1) {
        drawMaze(Maze);
        InputProcess(Maze, Solve);
        if (MazeEscape(Maze)) break;
    }
}

bool ResultMenu(void) {
    Clear();
    SetConsoleSize(80, 35);
    while (1) {
        Clear();
        GotoXY(25, 10);
        printf("Algorithm termproject : Solving Maze");
        GotoXY(22, 11);
        printf("탈출에 성공하셨습니다!.");
        GotoXY(22, Y_AXIS);
        printf("다시하려면 space를, 종료하시려면 enter를 눌러주세요.");
        GotoXY(38, 14);
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) return false; //스페이스를 누른다면 종료
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) return true; //엔터키를 누른다면 종료
        Sleep(50);
    }

}

int main() {
    Init();
    while (1) {
        StartMenu();
        GameMain();
        if (ResultMenu()) break;
    }
    return 0;
}