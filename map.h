#include <SFML\Graphics.hpp>
#include <sstream>
const int HEIGHT_MAP = 21;// высота
const int WIDTH_MAP = 23;// ширина 

using namespace sf;
String TileMap[HEIGHT_MAP] = {
	"00000000000000000000000",
	"00011111111011111111000",
	"00010010001010001001000",
	"00011111111111111111000",
	"00010010100000101001000",
	"00011110111011101111000",
	"00000010001010001000000",
	"00000010111111101000000",
	"00000010100200101000000",
	"0L1111111022201111111R0",
	"00000010100000101000000",
	"00000010111111101000000",
	"00000010100000101000000",
	"00011111111011111111000",
	"00010010001010001001000",
	"00011011111 11111011000",
	"00001010100000101010000",
	"00011110111011101111000",
	"00010000001010000001000",
	"00011111111111111111000",
	"00000000000000000000000"
};
String TileMapCONST[HEIGHT_MAP] = {
	"00000000000000000000000",
	"00011111111011111111000",
	"00010010001010001001000",
	"00011111111111111111000",
	"00010010100000101001000",
	"00011110111011101111000",
	"00000010001010001000000",
	"00000010111111101000000",
	"00000010100200101000000",
	"0L1111111022201111111R0",
	"00000010100000101000000",
	"00000010111111101000000",
	"00000010100000101000000",
	"00011111111011111111000",
	"00010010001010001001000",
	"00011011111 11111011000",
	"00001010100000101010000",
	"00011110111011101111000",
	"00010000001010000001000",
	"00011111111111111111000",
	"00000000000000000000000"
};
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
	"  *   0 6  1  7 0   *  ",
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
class Map
{
public:
	Texture texture;
	Sprite sprite;
	Font font;
	Text text;
	Text level_text;
	int level_number;
	Map(Image &image,int level) {
		level_number = level;
		font.loadFromFile("CyrilicOld.TTF");
		text = Text("", font, 22);
		text.setStyle(Text::Bold);
		text.setOutlineColor(Color::Black);
		text.setOutlineThickness(1.5);
		text.setPosition(4, 0);
		level_text = Text("", font, 22);
		level_text.setStyle(Text::Bold);
		level_text.setOutlineColor(Color::Black);
		level_text.setOutlineThickness(1.5);
	
		level_text.setPosition(4, 32);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	void map$textUpdate$Draw(RenderWindow *w, int  playerScore) {
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				if ((TileMap[i][j] == '1') || (TileMap[i][j] == ' ')) sprite.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 'L')  sprite.setTextureRect(IntRect(32 * 1, 0, 32, 32));
				if (TileMap[i][j] == 'R')  sprite.setTextureRect(IntRect(32 * 2, 0, 32, 32));
				if (TileMap[i][j] == '2')  sprite.setTextureRect(IntRect(32 * 5, 0, 32, 32));
				if ((TileMap[i][j] == '0')) sprite.setTextureRect(IntRect(96, 0, 32, 32));
				sprite.setPosition(j * 32, i * 32);
				w->draw(sprite);
			}
		std::ostringstream playerScoreString;
		playerScoreString << playerScore;
		text.setString("Score: " + playerScoreString.str());
		std::ostringstream leveling;
		leveling << level_number;
		level_text.setString("Level: " + leveling.str());
		w->draw(text);
		w->draw(level_text);
	}
};