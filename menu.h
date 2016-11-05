#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
using namespace sf;
void menu(RenderWindow & window) {
	Music backsound;
	backsound.openFromFile("sounds/backsound.wav");
	backsound.play();
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/menu/111.png");
	menuTexture2.loadFromFile("images/menu/222.png");
	menuTexture3.loadFromFile("images/menu/333.png");
	aboutTexture.loadFromFile("images/menu/about.png");
	menuBackground.loadFromFile("images/menu/Penguins.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 170);
	menu2.setPosition(100, 230);
	menu3.setPosition(100, 290);
	menuBg.setPosition(200, 0);
	//////////////////////////////лемч///////////////////
	while (isMenu)
	{
		menu1.setColor(Color(127,127,127));
		menu2.setColor(Color(127, 127, 127));
		menu3.setColor(Color(127, 127, 127));
		menuNum = 0;
		window.clear(Color(0,0,0));

		if (IntRect(100, 170, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::White); menuNum = 1; }
		if (IntRect(100, 230, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::White); menuNum = 2; }
		if (IntRect(100, 290, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::White); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }
		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.display();
	}
	////////////////////////////////////////////////////
}