#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
int playerScore = 0;
bool randomBonusFlag = true;
float createObjectForMapTimer = 0;
int get_Num(int i, int j, int max) {
	return i*max + j;
}

class Bonus {
private:float x, y;
public:
	int Score;
	float w, h = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Bonus(String F, int X, int Y, float W, float H,int score) {//Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		File = F;
		w = W; h = H;
		Score = score;
		image.loadFromFile("images/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	};
	Bonus() {
	};
	~Bonus()
	{
		playerScore += Score;
		if (Score > 10) randomBonusFlag = true;
	};
	float C_frame = 0;
	void udate(float time) {
		C_frame += 0.005*time;
		if (C_frame > 6) C_frame -= 6;
		sprite.setTextureRect(IntRect(18 * int(C_frame), 0, 18, 18));
	};
};

Bonus *Coins[HEIGHT_MAP* WIDTH_MAP];

void Bonus_create() {
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '1')
				Coins[get_Num(i, j, WIDTH_MAP)] = new Bonus("coin5.png", j * 32 + 7, i * 32 + 7, 18, 18, 10);
			else
				Coins[get_Num(i, j, WIDTH_MAP)] = NULL;
		};
};

void randomBonusGenerate() {
	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int itry = 0;
	randomElementX = 3 + rand() % (WIDTH_MAP - 3);
	randomElementY = 3 + rand() % (HEIGHT_MAP - 3);
	while ((TileMap[randomElementY][randomElementX] != ' ')&&(itry<4)) {
		randomElementX = 3 + rand() % (WIDTH_MAP - 3);
		randomElementY = 3 + rand() % (HEIGHT_MAP - 3);
		itry++;
	}
		if (TileMap[randomElementY][randomElementX] == ' ') {
			randomBonusFlag = false;
			Coins[get_Num(randomElementY, randomElementX, WIDTH_MAP)] = new Bonus("coin10.png", randomElementX * 32 + 7, randomElementY * 32 + 7, 18, 18, 20);
			std::cout << "BONUS:\n";
			std::cout << "coordinate of Bonus X:" << randomElementX << "\n" << "coordinate of Bonus Y:" << randomElementY << "\n\n";
			TileMap[randomElementY][randomElementX] = '1';
		}
};

void Bonus_update(float time) {
	for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)if (Coins[i] != NULL)Coins[i]->udate(time);
	if (randomBonusFlag) {
		createObjectForMapTimer += time;
		if (createObjectForMapTimer > 5000) {
			createObjectForMapTimer = 0;
			randomBonusGenerate();
		}
	}
};