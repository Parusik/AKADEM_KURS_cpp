#include <SFML\Graphics.hpp>
#include <sstream>
const int HEIGHT_MAP = 21;// высота
const int WIDTH_MAP = 23;// ширина 

using namespace sf;
//String TileMap[HEIGHT_MAP] = {
//	"00000000000000000000000",
//	"00011111111011111111000",
//	"00010010001010001001000",
//	"00011111111111111111000",
//	"00010010100000101001000",
//	"00011110111011101111000",
//	"00000010001010001000000",
//	"00000010111111101000000",
//	"00000010100200101000000",
//	"0L1111111022201111111R0",
//	"00000010100000101000000",
//	"00000010111111101000000",
//	"00000010100000101000000",
//	"00011111111011111111000",
//	"00010010001010001001000",
//	"00011011111 11111011000",
//	"00001010100000101010000",
//	"00011110111011101111000",
//	"00010000001010000001000",
//	"00011111111111111111000",
//	"00000000000000000000000"
//};
String TileMap[HEIGHT_MAP] = {
	"00000000000000000000000",
	"000        0        000",
	"000 00 000 0 000 00 000",
	"000                 000",
	"000 00 0 00000 0 00 000",
	"000    0   0   0    000",
	"000000 000 0 000 000000",
	"000000 0       0 000000",
	"000000 0 00200 0 000000",
	"0L       02220       R0",
	"000000 0 00000 0 000000",
	"000000 0       0 000000",
	"000000 0 00000 0 000000",
	"000        0        000",
	"000 00 000 0 000 00 000",
	"000  0           0  000",
	"0000 0 0 00000 0 0 0000",
	"000    0   0   0    000",
	"000 000000 0 000000 000",
	"000                 000",
	"00000000000000000000000"
};



class Map
{
public:
	Texture texture;
	Sprite sprite;
	Font font;
	Text text;
	Map(Image &image) {
		font.loadFromFile("CyrilicOld.TTF");
		text = Text("", font, 22);
		text.setStyle(Text::Bold);
		text.setPosition(4, 0);
		text.setOutlineColor(Color::Black);
		text.setOutlineThickness(1.5);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	void map$textUpdate$Draw(RenderWindow *w, int  playerScore) {
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				if ((TileMap[i][j] == '1') || (TileMap[i][j] == ' ') || (TileMap[i][j] == '2')) sprite.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 'L')  sprite.setTextureRect(IntRect(32 * 1, 0, 32, 32));
				if (TileMap[i][j] == 'R')  sprite.setTextureRect(IntRect(32 * 2, 0, 32, 32));
				if ((TileMap[i][j] == '0')) sprite.setTextureRect(IntRect(96, 0, 32, 32));
				sprite.setPosition(j * 32, i * 32);
				w->draw(sprite);
			}
		std::ostringstream playerScoreString;
		playerScoreString << playerScore;
		text.setString("Score: " + playerScoreString.str());
		w->draw(text);
	}
};