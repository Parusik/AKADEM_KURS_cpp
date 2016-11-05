#pragma once
#include "SFML\Audio.hpp"
using namespace sf;

int const enem_num = 3;
class Entity {
public:
	int lives;
	String Name;
	float dx, dy, x, y, speed, moveTimer = 0;
	float CurrentFrame = 0;
	int w, h, dir = 0;
	bool alive;
	Texture texture;
	Sprite sprite;
	Entity(Image &image, float X, float Y, int W, int H, String name) {
		Name = name;
		x = X; y = Y; w = W; h = H;
		speed = 0;  dx = 0; dy = 0;
		alive = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
};

class Player :public Entity {
private:
	void interactionWithMap() {
		for (int i = y / 32; (i < (y + h) / 32); i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; (j < (x + w) / 32); j++) {//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
														   //	std::cout << char(TileMap[i][j])<<"i:"<<i<<"  j:"<<j<<"  X:"<<x<<"  Y:"<<y<<"\n";
				if (TileMap[i][j] == '0' || TileMap[i][j] == '2') {
					if (dy > 0)y = i * 32 - h;//вниз
					if (dy < 0)y = i * 32 + 32;//вверх
					if (dx > 0)x = j * 32 - w;//вправо
					if (dx < 0)x = j * 32 + 32;//влево
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
				if ((TileMap[i][j] == '1') && ((x + (w / 2)) < (j + 1) * 32 - 7) && ((x + (w / 2)) > j * 32 + 7) && ((y + (h / 2)) < (i + 1) * 32 - 7) && ((y + (w / 2)) > i * 32 + 7)) {
					Bonus_deleter(i, j);
				}
				if ((Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)] != NULL) && (getRect().intersects(Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)]->getRect()))) {
					Bonus_deleter(bonus_last_I, bonus_last_J);
					music.play();
				}
			}
	}
	float const  playerspeed = 0.08;
public:
	bool sheald_flag = false;
	Music sheald_sound;
	Sprite sheald;
	Texture txt;
	static float px;
	static float py;
	Player(Image &image, float X, float Y, int W, int H, String k,int ilives ) :Entity(image, X, Y, W, H, k) {
		lives = ilives;
		sheald_sound.openFromFile("sounds/sheald.wav");
	}
	void udate(float time) {
		if (alive) {
			control(time);
			txt.loadFromImage(imageCreator("player_def.png", Color(138, 255, 138)));
			sheald.setTexture(txt);
			sheald.setTextureRect(IntRect(w * (int(CurrentFrame) + 5), 0, w, h + 7));
			sheald.setPosition(x, y - 7);
			switch (dir)
			{
			case 0: dx = speed; dy = 0; break;
			case 1: dx = -speed; dy = 0; break;
			case 2: dx = 0; dy = speed; break;
			case 3: dx = 0; dy = -speed; break;
			}
			x += dx*time;
			y += dy*time;
			px = x;
			py = y;
			speed = 0;
			sprite.setPosition(x, y - 7);
			interactionWithMap();
			if (lives < 0) { alive = false; }
		}
	};
	void control(float time) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			dir = 1; speed = playerspeed;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), h + 7, w, h + 7));
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			dir = 0; speed = playerspeed;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3 * (h + 7), w, h + 7));
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			dir = 3; speed = playerspeed;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2 * (h + 7), w, h + 7));
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			dir = 2; speed = playerspeed;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h + 7));
		}
		if (speed == 0) sprite.setTextureRect(IntRect(0, 0, w, h + 7));
	}
};
float Player::px;
float Player::py;

class Hearts {
public:
	Texture texture;
	Sprite sprite;
	Hearts(Image &image, int w, int h) {
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	void Drawing(RenderWindow *w, int plives) {
		for (int i = 0; i < plives; i++) {
			sprite.setTextureRect(IntRect(0, 0, 32, 32));
			sprite.setPosition((23 - (i + 1)) * 32, 0);
			w->draw(sprite);
		}
	}
};

class Enemy :public Entity {
private:
	float *GoX;
	float *GoY;
	int ii, jj, l_i, l_j, dirchange, oneline;
	bool f = true;
	float max_sleeping_timer;
	float is_sleeping_timer = 0;
	float enemy_max_speed;
	float enemy_speed;
public:
	bool fire;
	bool ENEM1_sleep_flag = false;
	Enemy(Image &image, float X, float Y, int W, int H, String k) :Entity(image, X, Y, W, H, k) {
		if (k == "enem1") {
			dir = 1;
		}
		else
		if (k == "enem2") { dir = 0; }
		else { dir = 3; }
	}
	Enemy(Image &image, float X, float Y, int W, int H, String k, float maxtimer, float ispeed,float imaxspeed, float *iX, float *iY) :Enemy(image, X, Y, W, H, k) {
		GoX = iX;
		GoY = iY;
		max_sleeping_timer = maxtimer;
		enemy_max_speed = imaxspeed;
		enemy_speed = ispeed;
		speed = enemy_speed;
	}
	int WayToPlayer(int povorot) {
		int nextdirX = (x <= *GoX) ? 0 : 1;
		int nextdirY = (y <= *GoY) ? 2 : 3;//dir to Player
		int betterdir = (abs(*GoX - x) > abs(*GoY - y)) ? nextdirX : nextdirY;//the  less way
		int nextdir = (betterdir == 0 || betterdir == 1) ? nextdirY : nextdirX;//
		nextdir = (rand() % 2) ? nextdirY : nextdirX;
		switch (povorot)
		{
		case 0:  return betterdir;
		case 2: {
			switch (betterdir)
			{
			case 3: return 3;
			case 1: return 1;
			}
			switch (nextdir)
			{
			case 3: return 3;
			case 1: return 1;
			}
		}
		case 3: {
			switch (betterdir)
			{
			case 2: return 2;
			case 1: return 1;
			}
			switch (nextdir)
			{
			case 2: return 2;
			case 1: return 1;
			}
		}
		case 4: {
			switch (betterdir)
			{
			case 0: return 0;
			case 2: return 2;
			}
			switch (nextdir)
			{
			case 0: return 0;
			case 2: return 2;
			}
		}
		case 5: {
			switch (betterdir)
			{
			case 0: return 0;
			case 3: return 3;
			}
			switch (nextdir)
			{
			case 0: return 0;
			case 3: return 3;
			}
		}
		case 6: {
			switch (betterdir)
			{
			case 2: return 2;
			case 1: return 1;
			case 3: return 3;
			}
			switch (nextdir)
			{
			case 2: return 2;
			case 1: return 1;
			case 3: return 3;
			}
		}
		case 7: {
			switch (betterdir)
			{
			case 0: return 0;
			case 2: return 2;
			case 3: return 3;
			}
			switch (nextdir)
			{
			case 2: return 2;
			case 0: return 0;
			case 3: return 3;
			}
		}
		case 8: {
			switch (betterdir)
			{
			case 0: return 0;
			case 1: return 1;
			case 3: return 3;
			}
			switch (nextdir)
			{
			case 1: return 1;
			case 0: return 0;
			case 3: return 3;
			}
		}
		case 9: {
			switch (betterdir)
			{
			case 0: return 0;
			case 1: return 1;
			case 2: return 2;
			}
			switch (nextdir)
			{
			case 1: return 1;
			case 0: return 0;
			case 2: return 2;
			}
		}
		}
	}
	void update(float time)
	{
		//////ENEMY DYING //////////////////////////////////////////////////////
		if (!alive) {
			speed = enemy_speed;
			alive = true;
			if (Name == String("enem1")) {
				ENEM1_sleep_flag = false;
				is_sleeping_timer = 0;
				dir = 1;
				x = 12 * 32;
				y = 9 * 32;
			}
			else if (Name == "enem2") {
				dir = 3;
				x = 11 * 32;
				y = 9 * 32;
			}
			else
			{
				dir = 0;
				x = 10 * 32;
				y = 9 * 32;
			}
		};
		/////////////////////////////////////////////////Drawing///////////////////////////////////////////////
		if (CurrentFrame > 2) CurrentFrame -= 2;
		CurrentFrame += 0.005*time;
		switch (dir)
		{
		case 0:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3 * h, w, h)); dx = speed; dy = 0; break;
		case 1:sprite.setTextureRect(IntRect(w * int(CurrentFrame), h, w, h)); dx = -speed; dy = 0; break;
		case 2:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h)); dx = 0; dy = speed; break;
		case 3:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2 * h, w, h)); dx = 0; dy = -speed; break;
		case 4:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 4 * h, w, h)); dx = 0; dy = -speed; break;
		}
		sprite.setPosition(x, y);
		control(time);
		///////////////////////////////////////////////////////////////////////////////////////////////
		if (ENEM1_sleep_flag) {
			is_sleeping_timer += time;
			speed = 0;
			dir = 4;
			if (is_sleeping_timer > max_sleeping_timer) {
				is_sleeping_timer = 0;
				dir = dirchange;
				speed = enemy_speed;
				ENEM1_sleep_flag = false;
			}
		}
		else {
			if (Name == "enem1") {
				is_sleeping_timer += time;
				if (is_sleeping_timer > 10000) { ENEM1_sleep_flag = true;  is_sleeping_timer = 0; }
			}
			if (Name == "enem3") {
				is_sleeping_timer += time;
				if (is_sleeping_timer > max_sleeping_timer) { fire = true; is_sleeping_timer = 0; }
			}
			//////////////////////////////////////////////////MOVING////////////////////////////////////////////////////
			ii = y / 32;
			jj = x / 32;

			dirchange = dir;
			if ((char(RunMap[ii][jj]) != ' ') && f) {
				switch (char(RunMap[ii][jj]))
				{
				case '0': {
					if (((dy < 0) && y <= ii * 32 + (32 - h) / 2) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2) || (dy > 0) || (dx > 0))
						dir = WayToPlayer(0);
					break;
				}
				case '1': {
					if (((dx < 0) && x <= jj * 32 + (32 - w) / 2) || (dx > 0)) dir = 3;
					break;
				}
				case '2': {
					if ((dy > 0) || (dx > 0))dir = WayToPlayer(2);
					break;
				}
				case '3': {
					if (((dy < 0) && y <= ii * 32 + (32 - h) / 2) || (dx > 0)) dir = WayToPlayer(3);
					break;
				}

				case '4': {
					if (((dy < 0) && y <= ii * 32 + (32 - h) / 2) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2)) dir = WayToPlayer(4);
					break; }

				case '5': {
					if ((dy > 0) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2)) dir = WayToPlayer(5);
					break;
				}
				case '6': {
					if ((dy > 0) || ((dy < 0) && y <= ii * 32 + (32 - h) / 2) || (dx > 0)) dir = WayToPlayer(6);
					break; }

				case '7': {
					if ((dy > 0) || ((dy < 0) && y <= ii * 32 + (32 - h) / 2) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2)) dir = WayToPlayer(7);
					break;
				}
				case '8': {
					if ((dy > 0) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2) || (dx > 0)) dir = WayToPlayer(8);
					break;
				}
				case '9': {
					if ((dx > 0) || ((dy < 0) && y <= ii * 32 + (32 - h) / 2) || ((dx < 0) && x <= jj * 32 + (32 - w) / 2)) dir = WayToPlayer(9);
					break;
				}
				}
				if (dirchange != dir) {
					l_i = ii;
					l_j = jj;
					f = false;
				}
			}
			if (((l_i != ii) || (l_j != jj)) && !f) f = true;
			x += dx*time;
			y += dy*time;
			for (int i = y / 32; (i < (y + h) / 32); i++)
				for (int j = x / 32; (j < (x + w) / 32); j++)
				{
					if (TileMap[i][j] == '0') {
						if (dy > 0) { y = i * 32 - h; dir = (rand() % 2) ? WayToPlayer(6) : RandomChoiser(0, 1, 0, 2); }
						if (dy < 0) { y = i * 32 + 32; dir = (rand() % 2) ? WayToPlayer(7) : RandomChoiser(0, 1, 0, 2); }
						if (dx > 0) { x = j * 32 - w; dir = (rand() % 2) ? WayToPlayer(8) : RandomChoiser(2, 3, 0, 2); }
						if (dx < 0) { x = j * 32 + 32; dir = (rand() % 2) ? WayToPlayer(9) : RandomChoiser(2, 3, 0, 2); }
					}
					if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
					if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
				}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	void control(float time) {
		if (Keyboard::isKeyPressed(Keyboard::J)) {
			dir = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			alive = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::L)) {
			dir = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::I)) {
			dir = 3;

		}
		if (Keyboard::isKeyPressed(Keyboard::K)) {
			dir = 2;

		}
	}
};

class Bullet :public Entity {
private:
	Music fire_sound;
public:
	int direction;
	Bullet(Image &image, float X, float Y, int W, int H, int dir,String iName) :Entity(image, X, Y, W, H,iName) {
		fire_sound.openFromFile("sounds/fire.wav");
		fire_sound.play();
		x = X;
		y = Y;
		direction = dir;
		speed = 0.11;
		w = h = 16;
		alive = true;
	}
	~Bullet(){ fire_sound.stop(); }
	void update(float time,Player &obj)
	{
		switch (direction)
		{
		case 0: dx = speed; dy = 0;   break;
		case 1: dx = -speed; dy = 0;   break;
		case 2: dx = 0; dy = speed;   break;
		case 3: dx = 0; dy = -speed;   break;
		}

		x += dx*time;
		y += dy*time;

		if (x <= 0) x = 1;//out of the map fix
		if (y <= 0) y = 1;

		int ii = y / 32;
		int jj = x / 32;

		if (TileMap[ii][jj] == '0') {
			alive = false;
			fire_sound.stop();
		}
		if (getRect().intersects(obj.getRect())&&!obj.sheald_flag)
		{	
			alive = false;
			--obj.lives;
			fire_sound.stop();
		}
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};