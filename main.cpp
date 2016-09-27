#include "map.h"
#include "bonus.h"
using namespace sf;

class Entity {
public:
	float dx, dy, x, y, speed, moveTimer = 0;
	int w, h, lives, dir = 0;
	bool alive;
	Texture texture;
	Sprite sprite;
	Entity(Image &image, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; 
		speed = 0;  dx = 0; dy = 0;
		alive = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
};

//class Enemy :public Entity {
//public:
//	Enemy(Image &image, float X, float Y, int W, int H) :Entity(image, X, Y, W, H) {
//			sprite.setTextureRect(IntRect(0, 0, w, h));
//			dx = 0.1;//���� ��������.���� ������ ������ ���������
//
//	}
//	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
//	{
//		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
//			for (int j = x / 32; j<(x + w) / 32; j++)
//			{
//				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, ��
//				{
//					if (Dy>0) { y = i * 32 - h; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
//					if (Dy<0) { y = i * 32 + 32; }//������������ � �������� ������ �����(����� � �� �����������)
//					if (Dx>0) { x = j * 32 - w; dx = -0.1; sprite.scale(-1, 1); }//� ������ ����� �����
//					if (Dx<0) { x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }// � ����� ����� �����
//				}
//			}
//	}
//	void update(float time)
//	{
//		if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����
//								  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
//			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
//			x += dx*time;
//			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
//			if (health <= 0) { life = false; }
//		}
//	}
//};


class Player :public Entity {
private:
	void interactionWithMap(){
		for (int i = y / 32; (i < (y + h) / 32); i++)//���������� �� ��������, �������������� � �������, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; (j < (x + w) / 32); j++) {//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
	//	std::cout << char(TileMap[i][j])<<"i:"<<i<<"  j:"<<j<<"  X:"<<x<<"  Y:"<<y<<"\n";
				if (TileMap[i][j] == '0') {
					if (dy > 0)y = i * 32 - h;//����
					if (dy < 0)y = i * 32 + 32;//�����
					if (dx > 0)x = j * 32 - w;//������
					if (dx < 0)x = j * 32 + 32;//�����
				}
				if ((x < 40) && (i = 9) && (dx < 0)) { j = 21; x = 675; };//bag fixed
				if ((x > 675) && (i = 9) && (dx > 0)) { x = 40; };
				if ((TileMap[i][j] == '1') && ((x + (w / 2))<(j + 1) * 32 - 7) && ((x + (w / 2))>j * 32 + 7) && ((y + (h / 2))<(i + 1) * 32 - 7) && ((y + (w / 2))>i * 32 + 7)) {
					delete Coins[get_Num(i, j, WIDTH_MAP)];
					Coins[get_Num(i, j, WIDTH_MAP)] = NULL;
					TileMap[i][j] = ' ';}}
	}
	float CurrentFrame = 0;
public:
	int lives = 3;
	Player(Image &image, float X, float Y, int W, int H) :Entity(image,X,Y,W,H){}
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
	void control(float time) {
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
			dir = 1; speed = 0.095;;//dir =1 - ����������� �����, speed =0.1 - �������� ��������. �������� - ����� �� ��� ����� �� �� ��� �� �������� � ����� �� ���������� ������ ���
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), h, w, h)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
			dir = 0; speed = 0.095;;//����������� ������, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 3*h, w, h)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
			dir = 3; speed = 0.095;;//����������� ����, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 2*h, w, h)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //���� ������ ������� ������� ����� ��� ���� ����� �
			dir = 2; speed = 0.095;//����������� �����, �� ����
			CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
			if (CurrentFrame > 4) CurrentFrame -= 4; //���������� �� ������ � ������� �� ������ ������������. ���� ������ � �������� ����� - ������������ �����.
			sprite.setTextureRect(IntRect(w * int(CurrentFrame), 0, w, h)); //���������� �� ����������� �. ���������� 96,96*2,96*3 � ����� 96
		}
		if (speed == 0) sprite.setTextureRect(IntRect(0, 0, w, h));
	}
};




int main()
{
	RenderWindow window(sf::VideoMode(736, 672), "GAME");
	Map  map(imageCreator("map.png",Color(255,255,255) ));
	Player player(imageCreator("player_def.png",Color(138, 255, 138)),11 * 32 /*+ 6*/, 15 * 32/* + 6*/, 20, 27);
	Clock clock;
	Bonus_create();//Start Bonuses creating 
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
		player.udate(time);
		map.mapUpdate(&window,playerScore); 
		/////////////////////Draw///////////////////////////
		for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)if (Coins[i] != NULL) window.draw(Coins[i]->sprite);
		window.draw(player.sprite);
		////////////////////////////////////////////////////
		window.display();
	}
	return 0;
};

	
