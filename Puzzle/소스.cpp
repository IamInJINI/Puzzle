#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES	1


#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <bangtal.h>
#include <string.h>
#include <string>
SceneID scene;
ObjectID start;

using namespace std;

struct POS {
	int x, y;
};
const POS positions[16] = {
	{300, 470}, {450, 470}, {600, 470}, {750, 470},
	{300, 320}, {450, 320}, {600, 320}, {750, 320},
	{300, 170}, {450, 170}, {600, 170}, {750, 170},
	{300, 20}, {450, 20}, {600, 20}, {750, 20},
};

ObjectID initObjects[16];
ObjectID gameObjects[16];
int blank = 15;

TimerID timerMixing;
const int mixCount = 200;
const Second animationTime = 0.05f;
int mixing = 0;

//타이머 만들자
TimerID timer1;
int number = 10000 - timer1;
int time1;
int timeStart;
int time2;

int time_ed;
int time_st;
const char* time_f;
string time_s;

TimerID timerStart;

bool game = false;


void startTimer()
{

}

void game_init()
{
	scene = createScene("어드벤쳐타임", "Images/배경.png");

	char image[20];
	for (int i = 0; i < 16; i++) {
		sprintf(image, "Images/%d.png", i + 1);
		initObjects[i] = createObject(image);
		gameObjects[i] = initObjects[i];

		locateObject(gameObjects[i], scene, positions[i].x, positions[i].y);
		showObject(gameObjects[i]);
	}

	start = createObject("Images/start.png");
	locateObject(start, scene, 540, 400);
	scaleObject(start, 2);
	showObject(start);

	blank = 15;
	game = false;

	srand((unsigned int)time(NULL));
	timerMixing = createTimer();
}

int game_index(ObjectID oid) {
	for (int i = 0; i < 16; i++) {
		if (gameObjects[i] == oid) return i;
	}

	return -1;
}

bool game_move(int index)
{
	if (index < 0) return false;
	if (index > 15) return false;

	if (index == (blank - 4) || index == (blank + 4) ||
		(index == (blank - 1) && (blank % 4) != 0) ||
		(index == (blank + 1) && (blank % 4) != 3)) {

		locateObject(gameObjects[index], scene, positions[blank].x, positions[blank].y);
		locateObject(gameObjects[blank], scene, positions[index].x, positions[index].y);

		ObjectID o = gameObjects[blank];
		gameObjects[blank] = gameObjects[index];
		gameObjects[index] = o;

		blank = index;

		return true;
	}

	return false;
}

void game_random_move()
{
	bool possible = false;
	int next = -1;
	while (!possible) {
		switch (rand() % 4) {
		case 0:		// UP
			next = blank - 4;
			break;
		case 1:		// DOWN
			next = blank + 4;
			break;
		case 2:		// LEFT
			next = (blank % 4 != 0) ? (blank - 1) : -1;
			break;
		case 3:		// RIGHT
			next = (blank % 4 != 3) ? (blank + 1) : -1;
			break;
		}

		possible = game_move(next);
	}
}

void game_prepare()
{
	hideObject(gameObjects[blank]);

	mixing = mixCount;

	// start to mix
	setTimer(timerMixing, animationTime);
	startTimer(timerMixing);

	
}

bool game_check()
{
	for (int i = 0; i < 16; i++) {
		if (gameObjects[i] != initObjects[i]) return false;
	}

	return true;
}

void game_end()
{
	game = false;

	showObject(gameObjects[blank]);
	setObjectImage(start, "Images/restart.png");
	showObject(start);

	time_ed = time(NULL);
	int time_d = (time_ed - time_st);
	time_s = std::to_string(time_d);
	time_f = time_s.c_str();
	showMessage(time_f);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (game) {
		game_move(game_index(object));

		if (game_check()) {
			game_end();
			
			//showMessage("소요시간은 %d초 입니다.", number);
		}
	}
	else if (mixing == 0 && object == start) {
		game_prepare();
		time_st = time(NULL);
	}
}

void timerCallback(TimerID timer)
{
	game_random_move();

	if (mixing > 0) {
		--mixing;

		setTimer(timerMixing, animationTime);
		startTimer(timerMixing);
	}
	else {
		hideObject(start);
		game = true;
	}
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, true);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, true);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	game_init();

	startGame(scene);
}
