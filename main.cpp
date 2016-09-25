#include <SFML/Graphics.hpp>
#include "map.h"
#include "bonus.h"
#include <iostream>

using namespace sf;
class Player {
private:float x, y;
public:
	bool alive;
	float w, h, dx, dy, speed = 0;
	int lives,dir = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, int X, int Y, float W, float H) {
		File = F;
		alive = true;
		lives = 3;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(138, 255, 138));/////////////////
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
	//	sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	void udate(float time) {
		if (alive) {
			control(time);
			switch (dir)
			{
			case 0: dx = speed; dy = 0; break;
			case 1: dx = -speed; dy = 0; break;
			case 2: dx = 0; dy = speed; break;
			case 3: dx = 0; dy = -speed; break;
			}
			x += dx*time;
			y += dy*time;
			speed = 0;
			sprite.setPosition(x, y);
			interactionWithMap();
			if (lives < 0) { alive = false; }
		}
	};
	float getplayercoordinateX() {	//этим методом будем забирать координату Х	
		return x;
	}
	float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
		return y;
	}
	float CurrentFrame = 0;
	void control(float time) {
	
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
			dir = 1; speed = 0.095;;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(20 * int(CurrentFrame), 20, 20, 20)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
			dir = 0; speed = 0.095;;//направление вправо, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			sprite.setTextureRect(IntRect(20 * int(CurrentFrame), 60, 20, 20)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
			dir = 3; speed = 0.095;;//направление вниз, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			sprite.setTextureRect(IntRect(20 * int(CurrentFrame), 40, 20, 20)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
			dir = 2; speed = 0.095;//направление вверх, см выше
			CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
			sprite.setTextureRect(IntRect(20 * int(CurrentFrame), 0, 20, 20)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
		}
		if (speed == 0) sprite.setTextureRect(IntRect(0, 0, 20, 20));
	}
	void interactionWithMap()
	{
		for (int i = y / 32; (i < (y + h) / 32); i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; (j < (x + w) / 32); j++){//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
				std::cout << char(TileMap[i][j])<<"i:"<<i<<"  j:"<<j<<"  X:"<<x<<"  Y:"<<y<<"\n";
				if (TileMap[i][j] == '0'){//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
					if (dy > 0)y = i * 32 - h;//если мы шли вниз,
					if (dy < 0)y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					if (dx > 0)x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					if (dx < 0)x = j * 32 + 32;//аналогично идем влево
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
				if ((TileMap[i][j] == '1')&&((x+(w/2))<(j+1)*32 - 7) && ((x + (w / 2))>j * 32 + 7) &&((y+(h/2))<(i+1 )* 32 - 7) && ((y + (w / 2))>i * 32 + 7)){
					delete Coins[get_Num(i, j, WIDTH_MAP)];
					Coins[get_Num(i, j, WIDTH_MAP)] = NULL;
					TileMap[i][j] = ' ';
				}
			}
	}
};///////////////////////////////////////////////////////////////////

int main()
{
	RenderWindow window(sf::VideoMode(736, 672), "GAME");
	Map  map("map.png");
	Player player("player2.png", 11 * 32 + 6, 15 * 32 + 6, 20, 20);
	Clock clock;
	Bonus_create();

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)window.close();
		}
		////////////////////Update////////////////////////////
		window.clear();
		Bonus_update(time);
		map.Map_update(&window);
		player.udate(time);
		/////////////////////Draw/////////////////////////////
		for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)if (Coins[i] != NULL) window.draw(Coins[i]->sprite);
		std::ostringstream playerScoreString;
		playerScoreString << playerScore;
		map.text.setString("Score:"+ playerScoreString.str());
		window.draw(map.text);
		window.draw(player.sprite);
		////////////////////////////////////////////////////
		window.display();
	}
	return 0;
};

	
