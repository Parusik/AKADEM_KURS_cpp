#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFML\Audio.hpp"

using namespace sf;
Music music;
int playerScore = 0;////
bool randomBonusFlag = true;////
float createObjectForMapTimer = 0;////
float kill_timer = 0;
float bonusX, bonusY;////
int bonus_last_J, bonus_last_I;
int get_Num(int i, int j, int max);
int RandomChoiser(int a1, int a2, int a3, int N);
Image imageCreator(String Name, const sf::Color c = sf::Color::Transparent);

class Pokeball {
public:
	Texture texture;
	Sprite sprite;
	Pokeball(Image &image, float x, float y, int w, int h) {
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}
};

class Simple_Bonus {
public:
	float x, y;
	int Score;
	Text text;
	float w, h;
	Texture texture;
	Sprite sprite;
	String Name;
	float C_frame = 0;
	Simple_Bonus(Image &image, int X, int Y, int W, int H, int score, String n) {
		w = W; h = H;
		Name = n;
		Score = score;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setPosition(x, y);
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	void Score_reset() { Score = 0; }
	virtual ~Simple_Bonus() {}
	virtual void update(float time) {}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
};

class Coin :public Simple_Bonus {
public:
	Coin(Image &image, int X, int Y, float W, float H, int score, String n) :Simple_Bonus(image, X, Y, W, H, score, n) {
	};
	~Coin() {
		playerScore += Score;
	}
	void update(float time) {
		show_bonus_timer();
		C_frame += 0.0042*time;
		if (C_frame > 6) C_frame -= 6;
		sprite.setTextureRect(IntRect(w * int(C_frame), 0, w, h));
	};
	virtual void show_bonus_timer() { }
};
class Ball :public Simple_Bonus {
public:
	Ball(Image &image, int X, int Y, float W, float H, int score, String n) :Simple_Bonus(image, X, Y, W, H, score, n) {};
	~Ball() {
		playerScore += Score;
		kill_timer = 5000;
		music.play();
	}
	void update(float time) {
		C_frame += 0.005*time;
		if (C_frame > 8) C_frame -= 8;
		sprite.setTextureRect(IntRect(0, int(C_frame)*h, w, h));
	}
};

class GoldCoin :public Coin {
public:
	Font font;
	int colorloop = 0;
	bool colorflag = true;
	GoldCoin(Image &image, int X, int Y, float W, float H, int score, String n) :Coin(image, X, Y, W, H, score, n) {
		font.loadFromFile("CyrilicOld.TTF");
		text = Text("", font, 22);
		text.setStyle(Text::Bold);
		text.setFillColor(Color(34, 177, 76));
		text.setPosition(32 + 4, 32 * 3 + 15);
		text.setOutlineThickness(2);
		text.setOutlineColor(Color::Black);
	};
	virtual ~GoldCoin() {
		randomBonusFlag = true;
		createObjectForMapTimer = 0;
	}
	void show_bonus_timer() {
		std::ostringstream timerbonus;
		timerbonus << 20 - int(createObjectForMapTimer) / 1000;
		text.setString(timerbonus.str());
		if (createObjectForMapTimer >= 11000) { text.setPosition(32 + 10, 32 * 3 + 14); }
		if (createObjectForMapTimer >= 15000)
		{
			if (colorflag) {
				text.setPosition(32 + 10, 32 * 3 + 13);
				text.setFillColor(Color(255, 19, 0));
				text.setCharacterSize(25);
				text.setOutlineColor(Color::Yellow);
				if (++colorloop == 40) { colorflag = !colorflag; colorloop = 0; };
			}
			else
			{
				text.setPosition(32 + 10, 32 * 3 + 14);
				text.setFillColor(Color(255, 255, 255));
				text.setCharacterSize(22);
				if (++colorloop == 25) { colorflag = !colorflag; colorloop = 0; };
			}

		}
	}
};

class Pokemon :public GoldCoin {
private:
	Music poc_sound;
	int ii, jj,l_i,l_j, dir, nextdir;
	bool f=true;
public:
	float speed, dx, dy, CurrentFrame = 0;
	Pokemon(Image &image, float X, float Y, int W, int H, int score, String name) : GoldCoin(image, X, Y, W, H, score, name) {
		speed = 0.09;  dir = rand() % 4;
		poc_sound.openFromFile("sounds/poc.wav");
		poc_sound.play();
	};
	void update(float time)
	{
		show_bonus_timer();
		if (CurrentFrame > 2) CurrentFrame -= 2;
		CurrentFrame += 0.005*time;
		ii = y / 32;
		jj = x / 32;
		nextdir = dir;
		if ((char(RunMap[ii][jj]) != ' ') && f) {
			switch (char(RunMap[ii][jj]))
			{
			case '0': {  }
			case '2': {
				if ((dy > 0))dir = 1;
				if ((dx > 0))dir = 3; 
				break; }

			case '3': {
				if ((dy < 0) && y <= ii * 32 + (32 - h) / 2) dir = 1;
				if ((dx > 0)) dir = 2;
				break; }

			case '4': { 
				if ((dy < 0) && y <= ii * 32 + (32 - h) / 2) dir = 0;
				if ((dx < 0) && x <= jj * 32 +(32-w)/2)dir = 2;
				break; }

			case '5': { /*std::cout << char(RunMap[ii][jj]);*/
				if ((dy > 0)) dir = 0;
				if ((dx < 0) && x <= jj * 32 +(32 - w) / 2)dir = 3;
				break; }

			case '6': { /*std::cout << char(RunMap[ii][jj]);*/
				if ((dy > 0)) dir = RandomChoiser(1, 2, 0, 2);
				if ((dy < 0) && y <= ii * 32 + (32 - h) / 2)dir = RandomChoiser(1, 3, 0, 2);
				if ((dx > 0)) dir = RandomChoiser(2, 3, 0, 2);
				break; }

			case '7': { /*std::cout << char(RunMap[ii][jj]);*/
				if ((dy > 0)) dir = RandomChoiser(0, 2, 0, 2);
				if ((dy < 0) && y <= ii * 32 + (32 - h) / 2) dir = RandomChoiser(0, 3, 0, 2);
				if ((dx < 0) && x <= jj * 32 + (32 - w) / 2) dir = RandomChoiser(2, 3, 0, 2);
				break; }

			case '8': { /*std::cout << char(RunMap[ii][jj]);*/
				if ((dy > 0)) dir = RandomChoiser(0, 1, 0, 2);
				if ((dx < 0) && x <= jj * 32 + (32 - w) / 2) dir = RandomChoiser(1, 3, 0, 2);
				if ((dx > 0)) dir = RandomChoiser(0, 3, 0, 2);
				break; }
			case '9': { /*std::cout << char(RunMap[ii][jj]);*/
				if ((dy < 0)&& y<=ii*32+(32-h)/2) dir = RandomChoiser(0, 1, 0, 2);
				if ((dx < 0) && x <= jj * 32 + (32 - w) / 2) dir = RandomChoiser(1, 2, 0, 2);
				if ((dx > 0)) dir = RandomChoiser(0, 2, 0, 2);
				break; }
			}
			/*speed += 0.0003;*/
			if (nextdir != dir) {
				l_i = ii;
				l_j = jj;
				f = false;
			}
		}
		if (((l_i != ii) || (l_j != jj))&&!f) f = true;
		switch (dir)
		{
		case 0:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3 * h, w, h)); dx = speed; dy = 0; break;
		case 1:sprite.setTextureRect(IntRect(w * int(CurrentFrame), h, w, h)); dx = -speed; dy = 0; break;
		case 2:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h)); dx = 0; dy = speed; break;
		case 3:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2 * h, w, h)); dx = 0; dy = -speed; break;
		}	
		sprite.setPosition(x, y);
		x += dx*time*0.95;
		y += dy*time*0.95;
		for (int i = y / 32; (i < (y + h) / 32); i++)
			for (int j = x / 32; (j < (x + w) / 32); j++)
			{
				if (TileMap[i][j] == '0') {
					if (dy > 0) { y = i * 32 - h;  dir = RandomChoiser(0, 1, 0, 2); }//вниз
					if (dy < 0) { y = i * 32 + 32; dir = RandomChoiser(0, 1, 0, 2); }//вверх
					if (dx > 0) { x = j * 32 - w;  dir = RandomChoiser(2, 3, 0, 2); }//вправо
					if (dx < 0) { x = j * 32 + 32; dir = RandomChoiser(2, 3, 0, 2); }//влево
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
			}
	}
};

Simple_Bonus *Bonuses_on_map[HEIGHT_MAP* WIDTH_MAP];

void Coins_create() {
	music.openFromFile("sounds/coin.wav");
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '1')
				Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = new Coin(imageCreator("coin.png",Color(0,128,128)), j * 32 + 7, i * 32 + 7, 18, 18, 1, "coin");
			else
				Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = NULL;
			if (((j==3)&&((i==18)||(i==2)))||((j== 19) && ((i == 18) || (i == 2))))Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = new Ball(imageCreator("ball.png", Color(0, 128, 128)), j * 32 + 7, i * 32 + 7, 22, 22, 1, "ball");
			if ((((j == 8) || (j == 14)) && ((i >= 7) && (i <= 11))) || ( ((i == 7)||(i==11) )&& ((j >= 8) && (j <= 14)))) Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = new Coin(imageCreator("coin50.png"), j * 32 + 7, i * 32 + 7, 18, 18, 25, "goldcoin");
		}
};

void randomBonusGenerate() {
	srand(time(0));
	int randomElementX = 2 + rand() % (WIDTH_MAP - 4);
	int randomElementY = 1 + rand() % (HEIGHT_MAP - 2);
	int itry = randomElementY = 1 + rand() % 10;
	std::cout << "TRIED TO CREATE THE BONUS\n";
	while ((TileMap[randomElementY][randomElementX] != ' ') && (itry)) {
		randomElementX = 2 + rand() % (WIDTH_MAP - 4);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 2);
		itry--;
	}

	if (TileMap[randomElementY][randomElementX] == ' ') {
		randomBonusFlag = false;
		switch (int choise = rand() % 9)
		{
		case 0:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/pikachu.png"), randomElementX * 32 + 5.5, randomElementY * 32 + 4, 21, 24, 100, "poc"); break;
		case 1:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc1.png",Color(0,128,128)), randomElementX * 32 + 6.5, randomElementY * 32 + 6, 19, 20, 100, "poc"); break;
		case 2:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc2.png", Color(0, 128, 128)), randomElementX * 32 + 5, randomElementY * 32 + 6, 22, 20, 100, "poc"); break;
		case 3:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc3.png", Color(0, 128, 128)), randomElementX * 32 + 6.5, randomElementY * 32 + 6.5, 19, 19, 100, "poc"); break;
		case 4:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc4.png", Color(0, 128, 128)), randomElementX * 32 + 4, randomElementY * 32 + 3.5, 24, 25, 100, "poc"); break;
		case 5:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc5.png", Color(0, 128, 128)), randomElementX * 32 + 4.5, randomElementY * 32 + 4.5, 23, 23, 100, "poc"); break;
		case 6:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc6.png", Color(0, 128, 128)), randomElementX * 32 + 5.5, randomElementY * 32 + 5.5, 21, 21, 100, "poc"); break;
		case 7:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc7.png", Color(0, 128, 128)), randomElementX * 32 + 6.5, randomElementY * 32 + 5.5, 19, 21, 100, "poc"); break;
		case 8:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pocs/poc8.png", Color(0, 128, 128)), randomElementX * 32 + 6.5, randomElementY * 32 + 6, 19, 20, 100, "poc"); break;
		}
		std::cout << "BONUS CREATED!\n X: " << randomElementX << "  Y: " << randomElementY << "\n\n";
		bonus_last_I = randomElementY;
		bonus_last_J = randomElementX;
	}
};

void Bonus_deleter(int i, int j) {
	delete Bonuses_on_map[get_Num(i, j, WIDTH_MAP)];
	Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = NULL;
	TileMap[i][j] = ' ';
}

void Bonus_deleter_byTimer(Simple_Bonus *b, float &timer, float time, float maxTime, int i, int j) {
	if (timer >= maxTime) {
		b->Score_reset();
		Bonus_deleter(i, j);
		timer = 0;
		std::cout << "BONUS FAILD!\n\n";
	}
	else timer += time;
}

void Bonuses_update(RenderWindow *w, float time, Pokeball p) {
	for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)if (Bonuses_on_map[i] != NULL)Bonuses_on_map[i]->update(time);
	if (randomBonusFlag) {
		createObjectForMapTimer += time;
		if (createObjectForMapTimer > 6000) {
			createObjectForMapTimer = 0;
			randomBonusGenerate();
		}
	}
	else
	{
		Bonus_deleter_byTimer(Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)], createObjectForMapTimer, time, 20000, bonus_last_I, bonus_last_J);
	}
	for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++) {
		if (Bonuses_on_map[i] != NULL) {
			w->draw(Bonuses_on_map[i]->sprite);
			if (Bonuses_on_map[i]->Name == "poc") {
				w->draw(p.sprite);
				w->draw(Bonuses_on_map[i]->text);
			}
		}
	}
	if (Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)] != NULL) w->draw(Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)]->sprite);
};

int get_Num(int i, int j, int max)
{
	return i*max + j;
}
int RandomChoiser(int a1, int a2, int a3, int N) {
	int A[3] = { a1,a2,a3 };
	return A[rand() % N];
}
Image imageCreator(String Name, const sf::Color c ) {
	Image bufer;
	bufer.loadFromFile("images/" + Name);
	if (c != c.Transparent)bufer.createMaskFromColor(c);
	return bufer;
};