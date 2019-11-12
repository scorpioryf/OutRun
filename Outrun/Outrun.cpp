// Outrun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include "Mylist.h"

using namespace sf;
using namespace std;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

bool bgameOver = false;
bool bInvincible = false;
bool bInjured = false;

float fSmall = 0;
int iCoin = 0;
int iBarrier = 0;
int iBooter = 0;
float fCoin = 0;
float fBarrier = 0;
float fBooter = 0;
float playerX = 0;
int iScore = 0;
int needprintH = 0;
int speed = 200;
int circle_count = 0;
int iHealth = 4;
int pos = 0;
int H = 1500;

RenderWindow app(VideoMode(width, height), "Outrun Racing!");

clock_t begin, end;
Texture ttt[50];
Texture bg;
Sprite object[50];
Sprite sBackground(bg);
void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1 - w1, y1));
	shape.setPoint(1, Vector2f(x2 - w2, y2));
	shape.setPoint(2, Vector2f(x2 + w2, y2));
	shape.setPoint(3, Vector2f(x1 + w1, y1));
	w.draw(shape);
}

struct Line
{
	float x, y, z; //3d center of line
	float X, Y, W; //screen coord
	float curve, spriteX, clip, scale;
	float coinX = 10;
	float barrierX = 10;
	float booterX = 10;
	Sprite sprite;
	bool SpeedUp = true;
	bool SpeedDown = true;

	Line()
	{
		spriteX = curve = x = y = z = 0;
	}

	void project(int camX, int camY, int camZ)
	{
		scale = camD / (z - camZ);
		X = (1 + scale * (x - camX)) * width / 2;
		Y = (1 - scale * (y - camY)) * height / 2;
		W = scale * roadW  * width / 2;
	}

	void drawSprite(RenderWindow &app)
	{
		Sprite s = sprite;
		int w = s.getTextureRect().width;
		int h = s.getTextureRect().height;

		float destX = X + scale * spriteX * width / 2;
		float destY = Y + 4;
		float destW = w * W / 266;
		float destH = h * W / 266;

		destX += destW * spriteX; //offsetX
		destY += destH * (-1);    //offsetY

		float clipH = destY + destH - clip;
		if (clipH < 0) clipH = 0;

		if (clipH >= destH) return;
		s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
		s.setScale(destW / w, destH / h);
		s.setPosition(destX, destY);
		app.draw(s);
	}
};


std::vector<Line> lines;
Line line;
void slowrun()
{
	for (int k = 8; k > 0; k--) {
		Sleep(0.3);
		playerX = playerX + 0.1 / (2 ^ (9 - k));
	}
}

void Inilaze() {

	app.setFramerateLimit(60);
	for (int i = 1; i <= 10; i++)
	{
		ttt[i].loadFromFile("C:/Users/as/Desktop/homework/游戏制作/outrun/Outrun/Outrun/images/" + std::to_string(i) + ".png");
		ttt[i].setSmooth(true);
		object[i].setTexture(ttt[i]);
	}
	bg.loadFromFile("C:/Users/as/Desktop/homework/游戏制作/outrun/Outrun/Outrun/images/tooopen_sy_156107161264.jpg");
	bg.setRepeated(true);
	sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
	sBackground.setPosition(-2000, 0);

}

void restart_inilaize() {
	bgameOver = false;
	bInvincible = false;
	bInjured = false;

	fSmall = 0;
	iCoin = 0;
	iBarrier = 0;
	iBooter = 0;
	fCoin = 0;
	fBarrier = 0;
	fBooter = 0;
	playerX = 0;
	iScore = 0;
	needprintH = 0;
	speed = 200;
	circle_count = 0;
	iHealth = 4;
	pos = 0;
	H = 1500;

}

void draw() {
	for (int i = 0; i < 1600; i++)
	{
		Line line;
		line.z = i * segL;

		if (i > 300 && i < 700) line.curve = 0.5;
		if (i > 1100) line.curve = -0.7;

		if (i < 300 && i % 20 == 0) { line.spriteX = -2.5; line.sprite = object[5]; }
		if (i % 17 == 0) { line.spriteX = 2.0; line.sprite = object[6]; }
		if (i > 300 && i % 20 == 0) { line.spriteX = -0.7; line.sprite = object[4]; }
		if (i > 800 && i % 20 == 0) { line.spriteX = -1.2; line.sprite = object[1]; }
		if (i == 400) { line.spriteX = -1.3; line.sprite = object[7]; }

		if (i % 50 == 0) {
			iCoin = rand() % 4;
			fSmall = rand() / float(RAND_MAX);
			fCoin = iCoin + fSmall - 2.5;
			line.coinX = (fCoin + 0.5) / 2;
			line.spriteX = fCoin;
			//lcout << "IN: " << fCoin <<endl;
			line.sprite = object[8];
		}
		if (i % 103 == 0) {
			//line.spriteX = -1.4;
			//line.spriteX = 0.6;
			iBarrier = rand() % 2;
			fSmall = rand() / float(RAND_MAX);
			fBarrier = iBarrier + fSmall - 1.4;
			line.barrierX = fBarrier + 0.4;
			line.spriteX = fBarrier;
			line.sprite = object[9];
		}

		if (i % 1000 == 0) {
			iBooter = rand() % 2;
			fSmall = rand() / float(RAND_MAX);
			fBooter = iBooter + fSmall - 1.4;
			line.booterX = fBooter + 0.4;
			line.spriteX = fBooter;
			line.sprite = object[10];
		}

		if (i % 555 == 0) {
			line.SpeedUp = false;
		}

		if (i % 5 == 0) {
			line.SpeedDown = false;
		}

		if (i > 750) line.y = sin(i / 30.0) * 1500;

		lines.push_back(line);
	}
}

void play() {


	int N = lines.size();


	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		if (bInjured == true) {
			speed = 100;
		}


		if (bInvincible == true) {
			speed = 600;
		}
		else if (bInjured == false)
		{
			speed = 200;
		}


		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (playerX < 0.8) {
				playerX += 0.1;
				//thread t (slowrun);
				//t.join();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (playerX > -0.8) {
				playerX -= 0.1;
			}
		}
		//if (Keyboard::isKeyPressed(Keyboard::Up)) speed = 200;
		//if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200;
		//if (Keyboard::isKeyPressed(Keyboard::Tab)) speed *= 3;
		if (Keyboard::isKeyPressed(Keyboard::W)) H += 100;
		if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100;



		pos += speed;
		while (pos >= N * segL) pos -= N * segL;
		while (pos < 0) pos += N * segL;

		app.clear(Color(205, 145, 4));   // 105 205 4
		app.draw(sBackground);
		int startPos = pos / segL;
		int camH = lines[startPos].y + H;
		if (speed > 0) sBackground.move(-lines[startPos].curve * 2, 0);
		if (speed < 0) sBackground.move(lines[startPos].curve * 2, 0);
		/*if (lines[startPos].coinX != 10) {
			cout << "OUT: " << lines[startPos].coinX << " , " << playerX << endl;
		}*/
		if (lines[startPos].SpeedUp == false && bgameOver == false) {
			cout << "Slow Down!" << endl;
			bInvincible = false;
		}

		if (lines[startPos].SpeedDown == false) {
			bInjured = false;
		}

		if (abs(lines[startPos].coinX - playerX) <= 0.15) {
			iScore++;
			cout << "Score: " << iScore << endl;
		}

		if (abs(lines[startPos].booterX - playerX) <= 0.15) {
			iScore += 20;
			cout << "Boot!" << endl;
			bInvincible = true;
			lines[startPos].SpeedUp = true;
		}

		if ((lines[startPos].barrierX - playerX <= 0.02) && bgameOver == false) {
			if (bInvincible == false) {
				iHealth = iHealth - 1;
				bInjured = true;
				needprintH = needprintH + 1;
			}
			if (iHealth <= 0) {
				bgameOver = true;
			}
			if (needprintH == 2) {
				cout << "Life: " << iHealth / 2 << endl;
				needprintH = needprintH - 2;
			}
		}



		/*if (abs(lines[startPos].coinX - playerX) < 0.2) {
			cout << "Get score!" << endl;
		}*/

		int maxy = height;
		float x = 0, dx = 0;
		///////draw road////////
		for (int n = startPos; n < startPos + 300; n++)
		{
			Line &l = lines[n%N];

			l.project(playerX*roadW - x, camH, startPos*segL - (n >= N ? N * segL : 0));

			x += dx;
			dx += l.curve;

			l.clip = maxy;
			if (l.Y >= maxy) continue;
			maxy = l.Y;

			Color grass = (n / 3) % 2 ? Color(150, 20, 16) : Color(95, 14, 10);   //草地 Color(16, 200, 16) : Color(0, 154, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1) % N]; //previous line

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble, p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
		}
		////////draw objects////////
		for (int n = startPos + 300; n > startPos; n--) {
			if (!bgameOver) {
				lines[n%N].drawSprite(app);
			}
			else
			{

			}
		}
		app.display();

		if (bgameOver == true) {
			while (1) {
				if (circle_count == 0) {
					cout << "再来一次：请按下Y键 " << endl;
					cout << "退出游戏：请按Ese键" << endl;
					circle_count = circle_count + 1;
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					exit(0);

				if (Keyboard::isKeyPressed(Keyboard::Y)) {
					restart_inilaize();
					break;
				}
			}
		}


	}
}

int main()
{
	Inilaze();
	draw();
	play();


	return 0;
}
