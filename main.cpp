#include "map.h"
#include "bonus.h"
#include "menu.h"
#include "entitys.h"

bool exit_chek(Simple_Bonus *Bonuses_on_map[HEIGHT_MAP* WIDTH_MAP]) {
	for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++) { 
		if (Bonuses_on_map[i] != NULL)return false;
	}
}

bool startGame(RenderWindow &window, int level, int &plives) {
	////////////////////START CONFIGS///////////////////////
	char exit='0';
	Map  map(imageCreator("map.png", Color(255, 255, 255)), level);
	Player player(imageCreator("player_def.png", Color(138, 255, 138)), 11 * 32 + 6, 15 * 32 + 8, 20, 20, "player",plives);
	Pokeball p(imageCreator("pokeball.png"), 0, 32 * 3, 32 * 3, 32 * 2);
	Hearts phearts(imageCreator("Heart.png"), 32, 32);
	Enemy *enems[enem_num];
	Music dead_sound;
	dead_sound.openFromFile("sounds/dead.wav");
	Bullet *pulia = NULL;
	enems[0] = new Enemy(imageCreator("/enems/enem1.png", Color(0, 128, 128)), 12 * 32, 9 * 32, 28, 28, "enem1", 4000,0.078 ,0.09, &Player::px, &Player::py);
	enems[1] = new Enemy(imageCreator("/enems/enem2.png", Color(0, 128, 128)), 10 * 32, 9 * 32, 26, 24, "enem2", 4000,0.07,0.1, &bonusX, &bonusY);
	enems[2] = new Enemy(imageCreator("/enems/enem3.png", Color(0, 128, 128)), 10 * 32, 9 * 32, 26, 27, "enem3", 2000,0.06 ,0.095, &Player::px, &Player::py);
	Clock clock;
	Coins_create();
	while (window.isOpen())
	{
		if (Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)] != NULL) {
			bonusX = Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)]->x;
			bonusY = Bonuses_on_map[get_Num(bonus_last_I, bonus_last_J, WIDTH_MAP)]->y;
		}
		else
		{
			bonusX = Player::px;
			bonusY = Player::py;
		}
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / (900-(5*level));
		////////////////////////////////////Killing Enem timer
		if (kill_timer>0) {
			kill_timer -= time;
		}
		/////////////////CLOSE WINDOW/////////////////
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)window.close();
			if (Keyboard::isKeyPressed(Keyboard::Tab)) { ++player.lives; player.alive = true; }
			if ((exit == '2') || (Keyboard::isKeyPressed(Keyboard::F2))) {
				Texture endgame;endgame.loadFromFile("images/menu/game_over.png");
				Sprite end(endgame);
				end.setPosition(0,0);
				window.draw(end);
				std::ostringstream playerScoreString;
				playerScoreString << playerScore;
				map.text.setPosition(310, 290);
				map.text.setString(playerScoreString.str());
				window.draw(map.text);
				window.display();
				do
				{
				} while (!Keyboard::isKeyPressed(Keyboard::Escape));
				for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)  delete Bonuses_on_map[i];
				for (int i = 0; i < HEIGHT_MAP; i++)  TileMap[i] = TileMapCONST[i];
				randomBonusFlag = true;
				createObjectForMapTimer = 0;
				kill_timer = 0;
				playerScore = 0;
				return false;
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)  delete Bonuses_on_map[i];
				for (int i = 0; i < HEIGHT_MAP; i++)  TileMap[i] = TileMapCONST[i];
				randomBonusFlag = true;
				createObjectForMapTimer = 0;
				kill_timer = 0;
				playerScore=0;
				return false; }//если эскейп, то выходим из игры
			if (Keyboard::isKeyPressed(Keyboard::F1)||( exit == '1')) {
				plives = player.lives;
				int scores = playerScore;
				for (int i = 0; i < (HEIGHT_MAP* WIDTH_MAP); i++)  delete Bonuses_on_map[i];
				for (int i = 0; i < HEIGHT_MAP; i++)  TileMap[i] = TileMapCONST[i];
				playerScore = scores;
				randomBonusFlag = true;
				createObjectForMapTimer = 0;
				kill_timer = 0;
				return true;
			}// выход в меню
			if (enems[2]->fire == true) { enems[2]->fire = false; pulia = new Bullet(imageCreator("bullet.png", Color(0, 0, 0)), enems[2]->x, enems[2]->y, 16, 16, enems[2]->dir, "bullet"); }
			if ((kill_timer > 0) && (!player.sheald_flag)) { player.sheald_sound.play(); player.sheald_flag = true; }
			if ((kill_timer < 0) && (player.sheald_flag)) { player.sheald_sound.stop(); player.sheald_flag = false; }
		}
		////////////////////UPDATING/////////////////////////////////////////
		window.clear();
		player.udate(time);
		///////Interaction Player-Enemy///////
		for (int i = 0; i < enem_num; i++) {
			enems[i]->update(time);
			if (enems[i]->getRect().intersects(player.getRect()) && (kill_timer <= 0)) {
				if ((i == 0) && (enems[i]->ENEM1_sleep_flag == true)) continue;///////////////////////////dead fix
				for (int k = 0; k < enem_num; k++) {
					enems[k]->alive = false;
					player.x = 11 * 32 + 6;
					player.y = 15 * 32 + 8;
				}
				--player.lives;
				dead_sound.play();
			}
			if (enems[i]->getRect().intersects(player.getRect()) && (kill_timer > 0))
			{
				enems[i]->alive = false;
				playerScore += 100;
			}
		}
		if (pulia != NULL) {
			if (!pulia->alive) { delete pulia; pulia = NULL; }
			else
			{
				pulia->update(time, player);
			}
		}
		/////////////////////ReDRAWing/////////////////////////////////////////////	
		map.map$textUpdate$Draw(&window, playerScore);
		Bonuses_update(&window, time, p);
		phearts.Drawing(&window, player.lives);
		for (int i = 0; i < enem_num; i++) {
			window.draw(enems[i]->sprite);
		}
		if (kill_timer > 0) { window.draw(player.sheald); }
		window.draw(player.sprite);
		if (pulia != NULL)window.draw(pulia->sprite);
		window.display();
		if (exit_chek(Bonuses_on_map)) exit = '1';
		if (player.lives <= 0) exit = '2';
	}
}

void gameRunning(RenderWindow &window,int &level,int &lives ) {
	if (startGame(window, level,lives)) { level++; gameRunning(window, level,lives); }
	else
	{
		menu(window);
		level = 1;
		gameRunning(window, level,lives);
	}
}
int main()
{	
	int levelnum = 1;
	int lives=5;
	RenderWindow window(sf::VideoMode(736, 672), "GAME");
	menu(window);
	gameRunning(window,levelnum,lives);
	return 0;
};

