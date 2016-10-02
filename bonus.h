#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
int playerScore = 0;
bool randomBonusFlag = true;
float createObjectForMapTimer = 0;
int bonus_last_J, bonus_last_I = 0;


int get_Num(int i, int j, int max) {
	return i*max + j;
}

Image imageCreator(String Name, const sf::Color c = sf::Color::Transparent) {
	Image bufer;
	bufer.loadFromFile("images/" + Name);
	if (c != c.Transparent)bufer.createMaskFromColor(c);
	return bufer;
};

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
}p(imageCreator("pokeball.png"), 0, 32 * 3, 32 * 3, 32 * 2);

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
};

class Coin :public Simple_Bonus {
public:
	Coin(Image &image, int X, int Y, float W, float H, int score, String n) :Simple_Bonus(image, X, Y, W, H, score, n) {};
	~Coin() {
		playerScore += Score;
	}
	void update(float time) {
		show_bonus_timer();
		C_frame += 0.005*time;
		if (C_frame > 6) C_frame -= 6;
		sprite.setTextureRect(IntRect(w * int(C_frame), 0, w, h));
	};
	virtual void show_bonus_timer() { }
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
	~GoldCoin() {
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
	int ii, jj,l_i,l_j, dir, nextdir;
	bool f=true;
	String RunMap[HEIGHT_MAP] = {
		"                       ",
		"   4  9   3 4   9  3   ",
		"                       ",
		"   7  0 9 8 8 9 0  6   ",
		"                       ",
		"   5  6 5 3 4 2 7  2   ",
		"                       ",
		"        4 8 8 3        ",
		"                       ",
		"      0 6     7 0      ",
		"                       ",
		"        7     6        ",
		"                       ",
		"   4  0 8 3 4 8 0  3   ",
		"                       ",
		"   53 7 9 8 8 9 6 42   ",
		"                       ",
		"   48 2 5 3 4 2 5 83   ",
		"                       ",
		"   5      8 8      2   ",
		"                       "
	};
public:
	float speed, dx, dy, CurrentFrame = 0;
	Pokemon(Image &image, float X, float Y, int W, int H, int score, String name) : GoldCoin(image, X, Y, W, H, score, name) {
		speed = 0.11;  dir = rand() % 4;
	};
	int choiser(int a1, int a2, int a3, int N) {
		int A[3] = { a1,a2,a3 };
		return A[rand() % N];
	}

	void update(float time)
	{
		show_bonus_timer();
		if (CurrentFrame > 2) CurrentFrame -= 2;
		CurrentFrame += 0.005*time;
		ii = y / 32;
		jj = x / 32;
		nextdir = dir;
		if ((RunMap[ii][jj] != ' ')) {
			switch ((RunMap[ii][jj])&f)
			{
			case '0': {std::cout << char(RunMap[ii][jj]);  break; }
			case '2': {std::cout << char(RunMap[ii][jj]);
				if ((dy > 0) ) nextdir = 1;
				if ((dx > 0) ) nextdir = 3;
				switch (dir)
					break; }

			case '3': {std::cout << char(RunMap[ii][jj]);
				if ((dy < 0) ) nextdir = 1;
				if ((dx > 0)) nextdir = 2;
				break; }

			case '4': { std::cout << char(RunMap[ii][jj]);
				if ((dy < 0) ) nextdir = 0;
				if ((dx < 0) ) nextdir = 2;
				break; }

			case '5': { std::cout << char(RunMap[ii][jj]);
				if ((dy > 0)) nextdir = 0;
				if ((dx < 0) ) nextdir = 3;
				break; }

			case '6': { std::cout << char(RunMap[ii][jj]);
				if ((dy > 0) ) nextdir = choiser(1, 2, 0, 2);
				if ((dy < 0) ) nextdir = choiser(1, 3, 0, 2);
				if ((dx > 0)) nextdir = choiser(2, 3, 0, 2);
				break; }

			case '7': { std::cout << char(RunMap[ii][jj]);
				if ((dy > 0) ) nextdir = choiser(0, 2, 0, 2);
				if ((dy < 0) ) nextdir = choiser(0, 3, 0, 2);
				if ((dx < 0) ) nextdir = choiser(2, 3, 0, 2);
				switch (dir)
					break; }

			case '8': { std::cout << char(RunMap[ii][jj]);
				if ((dy > 0) ) nextdir = choiser(0, 1, 0, 2);
				if ((dx < 0) ) nextdir = choiser(1, 3, 0, 2);
				if ((dx > 0) ) nextdir = choiser(0, 3, 0, 2);
				switch (dir)
					break; }


			case '9': { std::cout << char(RunMap[ii][jj]);
				if ((dy < 0) ) nextdir = choiser(0, 1, 0, 2);
				if ((dx < 0) ) nextdir = choiser(1, 2, 0, 2);
				if ((dx > 0) ) nextdir = choiser(0, 2, 0, 2);
				switch (dir)
					break; }
			}
			l_i = ii;
			l_j = jj;
			f = false;
		}
		if ((l_i != ii) || (l_j != jj)) f = true;
		dir = nextdir;
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
					if (dy > 0) { y = i * 32 - h;  dir = choiser(0, 1, 0, 2); }//вниз
					if (dy < 0) { y = i * 32 + 32; dir = choiser(0, 1, 0, 2); }//вверх
					if (dx > 0) { x = j * 32 - w;  dir = choiser(2, 3, 0, 2); }//вправо
					if (dx < 0) { x = j * 32 + 32; dir = choiser(2, 3, 0, 2); }//влево
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
			}
	
	}
};

Simple_Bonus *Bonuses_on_map[HEIGHT_MAP* WIDTH_MAP];

void Coins_create() {
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '1')
				Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = new Coin(imageCreator("coin.png"), j * 32 + 7, i * 32 + 7, 18, 18, 1, "coin");
			else
				Bonuses_on_map[get_Num(i, j, WIDTH_MAP)] = NULL;
		};
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
		//std::cout << " X: " << randomElementX << "  Y: " << randomElementY << "\n";

	}

	if (TileMap[randomElementY][randomElementX] == ' ') {
		randomBonusFlag = false;
		switch (int choise = 1 /* rand() % 2*/)
		{
		case 0:Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new GoldCoin(imageCreator("coin50.png"), randomElementX * 32 + 7, randomElementY * 32 + 7, 18, 18, 50, "goldcoin"); break;
		case 1:	Bonuses_on_map[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Pokemon(imageCreator("pikachu.png"), randomElementX * 32 + 5.5, randomElementY * 32 + 4, 21, 24, 100, "pika"); break;
		}
		std::cout << "BONUS CREATED!\n X: " << randomElementX << "  Y: " << randomElementY << "\n\n";
		TileMap[randomElementY][randomElementX] = '1';
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

void Bonuses_update(RenderWindow *w, float time) {
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

			if (Bonuses_on_map[i]->Name != "coin") {
				w->draw(p.sprite);
				w->draw(Bonuses_on_map[i]->text);
			}
		};
	}
};
