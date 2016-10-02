#include "map.h"
#include "bonus.h"
#include <vector>
using namespace sf;

class Entity {
public:
	String Name;
	float dx, dy, x, y, speed, moveTimer = 0;
	float CurrentFrame = 0;
	int w, h, lives, dir = 0;
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
				if (TileMap[i][j] == '0') {
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
			}
	}
public:
	Player(Image &image, float X, float Y, int W, int H, String k) :Entity(image, X, Y, W, H, k) { lives = 3; }
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
			sprite.setPosition(x, y - 7);
			interactionWithMap();
			if (lives < 0) { alive = false; }
		}
	};
	void control(float time) {
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
			dir = 1; speed = 0.095;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), h + 7, w, h + 7)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
			dir = 0; speed = 0.095;;//направление вправо, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3 * (h + 7), w, h + 7)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
			dir = 3; speed = 0.095;;//направление вниз, см выше
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2 * (h + 7), w, h + 7)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
			dir = 2; speed = 0.095;//направление вверх, см выше
			CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (CurrentFrame > 4) CurrentFrame -= 4; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h + 7)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
		}
		if (speed == 0) sprite.setTextureRect(IntRect(0, 0, w, h + 7));
	}

};

class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, String k) :Entity(image, X, Y, W, H, k) {
		lives = 1;
		speed = 0.059;
	}
	void interactionWithMap() {
		for (int i = y / 32; (i < (y + h) / 32); i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; (j < (x + w) / 32); j++)
			{//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			 //std::cout << char(TileMap[i][j])<<"i:"<<i<<"  j:"<<j<<"  X:"<<x<<"  Y:"<<y<<"\n";
				if (TileMap[i][j] == '0') {
					if (dy > 0)y = i * 32 - h;//вниз
					if (dy < 0)y = i * 32 + 32;//вверх
					if (dx > 0)x = j * 32 - w;//вправо
					if (dx < 0)x = j * 32 + 32;//влево
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
			}
	}
	void update(float time)
	{
		if (alive) {
			control(time);
			//if (Name == "pikachu") {//для персонажа с таким именем логика будет такой
			//						  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			//	interactionWithMap();//обрабатываем столкновение по Х
			//	x += dx*time;
			//	sprite.setPosition(x, y); //задаем позицию спрайта в место его центра
			//	if (lives <= 0) { alive = false; }
			//}
			if (CurrentFrame > 2) CurrentFrame -= 2;
			CurrentFrame += 0.005*time;
			switch (dir)
			{
			case 0:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3 * h, w, h)); dx = speed; dy = 0; break;
			case 1:sprite.setTextureRect(IntRect(w * int(CurrentFrame), h, w, h)); dx = -speed; dy = 0; break;
			case 2:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h)); dx = 0; dy = speed; break;
			case 3:sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2 * h, w, h)); dx = 0; dy = -speed; break;
			}
			x += dx*time;
			y += dy*time;
			sprite.setPosition(x, y);
			interactionWithMap();
			if (lives < 0) { alive = false; }
		}
	}

	void control(float time) {
		if (Keyboard::isKeyPressed(Keyboard::J)) {
			dir = 1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
		}
		if (Keyboard::isKeyPressed(Keyboard::L)) {
			dir = 0;

		}
		if (Keyboard::isKeyPressed(Keyboard::I)) {
			dir = 3;

		}
		if (Keyboard::isKeyPressed(Keyboard::K)) { //если нажата клавиша стрелка влево или англ буква А
			dir = 2;

		}
		if (speed == 0) sprite.setTextureRect(IntRect(0, 0, w, h));
	}
};
int main()
{
	RenderWindow window(sf::VideoMode(736, 672), "GAME");
	Map  map(imageCreator("map.png", Color(255, 255, 255)));
	Player player(imageCreator("player_def.png", Color(138, 255, 138)), 11 * 32 + 6, 15 * 32 + 6 + 3, 20, 20, "player");
	//Enemy pika(imageCreator("pikachu.png"), 6 * 32, 9 * 32, 21, 24);
	Clock clock;
	Coins_create();//Start Coins creating 
	while (window.isOpen())
	{
		for (int i = 0; i < 10000000; i++) {};
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
		player.udate(time);
		//pika.update(time);

		//if (pika.getRect().intersects(player.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
		//{
		//	pika.speed = 0;
		//	if (pika.dx < 0)
		//	{
		//		pika.x = player.x + player.w;
		//		pika.dx = 0;
		//	}
		//	if (pika.dx > 0)
		//	{
		//		pika.x = player.x - pika.w;
		//		pika.dx = 0;
		//	}
		//	if (pika.dy > 0)
		//	{
		//		pika.y = player.y - pika.h;
		//		pika.dy = 0;
		//	}
		//	if (pika.dy < 0)
		//	{
		//		pika.y = player.y + player.h;
		//		pika.dy = 0;
		//	}
		//	if (player.dy > 0) { player.y = pika.y - player.h; player.dy = 0; }
		//	if (player.dy < 0) { player.y = pika.y + pika.h; player.dy = 0; }
		//	if (player.dx > 0) { player.x = pika.x - player.w; player.dx = 0; }
		//	if (player.dx < 0) { player.x = pika.x + pika.w; player.dx = 0; }
		//}
		//else { pika.speed = 0.059; }


		map.map$textUpdate$Draw(&window, playerScore);
		Bonuses_update(&window, time);
		/////////////////////Draw///////////////////////////	
	//	window.draw(pika.sprite);
		window.draw(player.sprite);
		////////////////////////////////////////////////////
		window.display();
	}
	return 0;
};

