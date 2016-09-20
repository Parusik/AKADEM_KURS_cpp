#include <SFML\Graphics.hpp>
using  namespace sf;

View view;
void getplayercoordinateforview(float x, float y) { //функция для считывания координат игрока
	view.setCenter(x + 100, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте
}
