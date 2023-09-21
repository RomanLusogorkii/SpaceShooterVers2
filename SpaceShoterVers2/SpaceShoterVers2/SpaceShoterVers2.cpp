#include<iostream>
#include<conio.h>
#include<dos.h> 
#include<windows.h>

#include<time.h>
#pragma warning(disable : 4996)

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
// описание игрока
int enY[3];
int enX[3];
int enemyFlag[3];
char bird[3][5] = { '!',' ','±',' ','!',
					'±','^','±','^','±',
					'±','±','±','±','±' };
int birdPos = WIN_WIDTH / 2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
// Курсор
void setcursor(bool visible, DWORD size) {
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
//Рисунок границ игрового уровня
void drawBorder() {

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		gotoxy(i, SCREEN_HEIGHT); cout << "__";
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i+1); cout << "|";
		gotoxy(SCREEN_WIDTH+1, i+1); cout << "|";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(WIN_WIDTH, i+1); cout << "|";
	}
}
//Генерация врагов
void genEnemy(int ind) {
	enX[ind] = 3 + rand() % (WIN_WIDTH - 10);
}
// Изображение Врага
void drawEnemy(int ind) {
	if (enemyFlag[ind] == true) {
		gotoxy(enX[ind], enY[ind]);     cout << "!vv!";
		gotoxy(enX[ind], enY[ind] + 1); cout << "|±±|";
		gotoxy(enX[ind], enY[ind] + 2); cout << "|±±|";
		gotoxy(enX[ind], enY[ind] + 3); cout << "|±±|";

	}
}
// Уничтожение врага
void eraseEnemy(int ind) {
	if (enemyFlag[ind] == true) {
		gotoxy(enX[ind], enY[ind]);     cout << "    ";
		gotoxy(enX[ind], enY[ind] + 1); cout << "    ";
		gotoxy(enX[ind], enY[ind] + 2); cout << "    ";
		gotoxy(enX[ind], enY[ind] + 3); cout << "    ";
	}
}
// Ресет врагов
void resetEnemy(int ind) {
	eraseEnemy(ind);
	enY[ind] = 4;
	genEnemy(ind);
}
//выстрел
void genBullet() {
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = birdPos;
	bullets[bIndex][2] = 22;
	bullets[bIndex][3] = birdPos + 4;
	bIndex++;
	if (bIndex == 20)
		bIndex = 0;
}
//Движение выстрела
void moveBullet() {
	for (int i = 0; i < 20; i++) {
		if (bullets[i][0] > 2)
			bullets[i][0]--;
		else
			bullets[i][0] = 0;

		if (bullets[i][2] > 2)
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	}
}
// Отображение выстрела
void drawBullets() {
	for (int i = 0; i < 20; i++) {
		if (bullets[i][0] > 1) {
			gotoxy(bullets[i][1], bullets[i][0]); cout << "\x1B[31m.\033[0m";
			gotoxy(bullets[i][3], bullets[i][2]); cout << "\x1B[31m.\033[0m";
		}
	}
}
// Стирание в движении выстрела
void eraseBullets() {
	for (int i = 0; i < 20; i++) {
		if (bullets[i][0] >= 1) {
			gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
			gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
		}
	}
}
// Стирание после поподания
void eraseBullet(int i) {
	gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
	gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
}
//Отображеник игрока в движении 
void drawBird() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			gotoxy(j + birdPos, i + 22); cout << bird[i][j];
		}
	}
}
//Стирание игорока после движения
void eraseBird() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			gotoxy(j + birdPos, i + 22); cout << " ";
		}
	}
}
// кализия
int collision() {
	if (enY[0] + 4 >= 23) {
		if (enX[0] + 4 - birdPos >= 0 && enX[0] + 4 - birdPos < 8) {
			return 1;
		}
	}
	return 0;
}
// Попадание выстрелом
int bulletHit() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 4; j += 2) {
			if (bullets[i][j] != 0) {
				if (bullets[i][j] >= enY[0] && bullets[i][j] <= enY[0] + 4) {
					if (bullets[i][j + 1] >= enX[0] && bullets[i][j + 1] <= enX[0] + 4) {
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}
				if (bullets[i][j] >= enY[1] && bullets[i][j] <= enY[1] + 4) {
					if (bullets[i][j + 1] >= enX[1] && bullets[i][j + 1] <= enX[1] + 4) {
						eraseBullet(i);
						resetEnemy(1);
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
// Конец игры
void gameover() {
	system("cls");
	gotoxy(35, 10); cout << endl;
	gotoxy(35, 11); cout << "\t\t-----------------------------" << endl;
	gotoxy(35, 12); cout << "\t\t----------\x1B[31mКонец Игры\033[0m---------" << endl;
	gotoxy(35, 13); cout << "\t\t-----------------------------" << endl;
	gotoxy(35, 14); cout << "\t\t\x1B[33m            Счет:\033[0m" << score << endl;
	gotoxy(35, 15); cout << endl;
	gotoxy(35, 16); cout << "\t\t\x1B[32m Нажми пробел чтобы вернутся\033[0m"  << endl << endl;
	getch();
}
void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5); cout << "\x1B[33mСчет: \033[0m" << score << endl;
}

//Виджет Информации
void instructions() {

	system("cls");
	gotoxy(15, 5); cout << "\t\t\x1B[31m                                Управление\033[0m" << endl;
	gotoxy(15, 6); cout << "\t\t---------------------------------------------------------------------------" << endl;
	gotoxy(15, 7); cout << "\t\tНажми на ПРОБЕЛ для выстрела" << endl;
	gotoxy(15, 8); cout << "\t\tДля движения в стороны используются A и D" << endl;
	gotoxy(15, 9); cout << "\t\t---------------------------------------------------------------------------" << endl;
	gotoxy(15, 10); cout << "\t\t " << endl;
	gotoxy(15, 11); cout << "\t\t\x1B[31m                                Механика игры\033[0m" << endl;
	gotoxy(15, 12); cout << "\t\t---------------------------------------------------------------------------" << endl;
	gotoxy(15, 13); cout << "\t\tПопадая по противникам зарабатываются очки" << endl;
	gotoxy(15, 14); cout << "\t\tЕсли противник приблизится слишком близко игрок проигрывает" << endl;
	gotoxy(15, 15); cout << "\t\t---------------------------------------------------------------------------" << endl;
	gotoxy(15, 16); cout << "\t\t " << endl;
	gotoxy(15, 17); cout << "\t\t                           !vv!           ! ± !" << endl;
	gotoxy(15, 18); cout << "\t\t                           |±±|           ±^±^±" << endl;
	gotoxy(15, 19); cout << "\t\t                           |±±|           ±±±±±" << endl;
	gotoxy(15, 20); cout << "\t\t                           |±±|          " << endl;
	gotoxy(15, 21); cout << "\t\t\x1B[31m                           Враги          Игрок\033[0m" << endl;
	gotoxy(15, 22); cout << "\t\t " << endl;
	gotoxy(15, 23); cout << "\t\t\x1B[32m                    Нажми на пробел для возврата в меню\033[0m" << endl << endl;
	getch();
}
// Игра
void play() {

	birdPos = -1 + WIN_WIDTH / 2;
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 1;
	enY[0] = enY[1] = 4;

	for (int i = 0; i < 20; i++) {
		bullets[i][0] = bullets[i][1] = 0;
	}
	// Отображение на экране в игре
	system("cls");
	drawBorder();
	genEnemy(0);
	genEnemy(1);
	updateScore();

	gotoxy(WIN_WIDTH + 6, 3); cout << "\x1B[31mОдна жизнь\033[0m";
	gotoxy(WIN_WIDTH + 1, 4); cout << "--------------------";
	gotoxy(WIN_WIDTH + 1, 6); cout << "--------------------";
	gotoxy(WIN_WIDTH + 6, 12); cout << "\x1B[32mУправление \033[0m";
	gotoxy(WIN_WIDTH + 1, 13); cout << "--------------------";
	gotoxy(WIN_WIDTH + 1, 14); cout << "клавиша A - В Влево";
	gotoxy(WIN_WIDTH + 1, 15); cout << "клавиша D - В Вправо";
	gotoxy(WIN_WIDTH + 3, 16); cout << "Пробел - Выстрел";
	gotoxy(WIN_WIDTH + 1, 17); cout << "--------------------";

	gotoxy(22, 10); cout << "\x1B[32mНажми на пробел чтобы начать\033[0m";
	getch();
	gotoxy(22, 10); cout << "                            ";

	//Управление
	while (1) {
		if (kbhit()) {
			char ch = getch();
			if (ch == 'a' || ch == 'A') {
				if (birdPos > 2)
					birdPos -= 2;
			}
			if (ch == 'd' || ch == 'D') {
				if (birdPos < WIN_WIDTH - 7)
					birdPos += 2;
			}
			if (ch == 32) {
				genBullet();
			}
			if (ch == 27) {
				break;
			}
		}
		// Просчет попадания в врага и конец
		drawBird();
		drawEnemy(0);
		drawEnemy(1);
		drawBullets();
		if (collision() == 1) {
			gameover();
			return;
		}
		if (bulletHit() == 1) {
			score++;
			updateScore();
		}
		Sleep(200);
		eraseBird();
		eraseEnemy(0);
		eraseEnemy(1);
		eraseBullets();
		moveBullet();

		if (enemyFlag[0] == 1)
			enY[0] += 1;

		if (enemyFlag[1] == 1)
			enY[1] += 1;

		if (enY[0] > SCREEN_HEIGHT - 5) {
			resetEnemy(0);
		}
		if (enY[1] > SCREEN_HEIGHT - 5) {
			resetEnemy(1);
		}
	}
}

// Меню
int main()
{
	setcursor(0, 0);
	srand((unsigned)time(NULL));

	do {
		setlocale(LC_ALL, "");
		system("cls");
		gotoxy(50, 9); cout << "";
		gotoxy(50, 10); cout << "\x1B[33m|-------------------------| \033[0m";
		gotoxy(50, 11); cout << "\x1B[33m| Космический Шутер Vers2 | \033[0m";
		gotoxy(50, 12); cout << "\x1B[33m|-------------------------|\033[0m";
		gotoxy(50, 14); cout << "1.> Играть ";
		gotoxy(50, 16); cout << "2.> Управление ";
		gotoxy(50, 18); cout << "3.> Выход ";
		gotoxy(50, 20); cout << "\x1B[32mДля выбора нажмите цифру: \033[0m";
		char op = getche();

		if (op == '1') play();
		else if (op == '2') instructions();
		else if (op == '3') exit(0);

	} while (1);

	return 0;
}