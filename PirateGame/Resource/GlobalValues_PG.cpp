#include "GlobalValues_PG.h"

using namespace PirateGame;

std::string GlobalValues::keyToString(sf::Keyboard::Key key) {
	switch (key) {
		case sf::Keyboard::A: return "A";
		case sf::Keyboard::B: return "B";
		case sf::Keyboard::C: return "C";
		case sf::Keyboard::D: return "D";
		case sf::Keyboard::E: return "E";
		case sf::Keyboard::F: return "F";
		case sf::Keyboard::G: return "G";
		case sf::Keyboard::H: return "H";
		case sf::Keyboard::I: return "I";
		case sf::Keyboard::J: return "J";
		case sf::Keyboard::K: return "K";
		case sf::Keyboard::L: return "L";
		case sf::Keyboard::M: return "M";
		case sf::Keyboard::N: return "N";
		case sf::Keyboard::O: return "O";
		case sf::Keyboard::P: return "P";
		case sf::Keyboard::Q: return "Q";
		case sf::Keyboard::R: return "R";
		case sf::Keyboard::S: return "S";
		case sf::Keyboard::T: return "T";
		case sf::Keyboard::U: return "U";
		case sf::Keyboard::V: return "V";
		case sf::Keyboard::W: return "W";
		case sf::Keyboard::X: return "X";
		case sf::Keyboard::Y: return "Y";
		case sf::Keyboard::Z: return "Z";
		case sf::Keyboard::Num0: return "0";
		case sf::Keyboard::Num1: return "1";
		case sf::Keyboard::Num2: return "2";
		case sf::Keyboard::Num3: return "3";
		case sf::Keyboard::Num4: return "4";
		case sf::Keyboard::Num5: return "5";
		case sf::Keyboard::Num6: return "6";
		case sf::Keyboard::Num7: return "7";
		case sf::Keyboard::Num8: return "8";
		case sf::Keyboard::Num9: return "9";
		case sf::Keyboard::Escape: return "Esc";
		case sf::Keyboard::LControl: return "LCtrl";
		case sf::Keyboard::LShift: return "LShift";
		case sf::Keyboard::LAlt: return "LAlt";
		case sf::Keyboard::LSystem: return "LSys";
		case sf::Keyboard::RControl: return "RCtrl";
		case sf::Keyboard::RShift: return "RShift";
		case sf::Keyboard::RAlt: return "RAlt";
		case sf::Keyboard::RSystem: return "RSys";
		case sf::Keyboard::Menu: return "Menu";
		case sf::Keyboard::LBracket: return "[";
		case sf::Keyboard::RBracket: return "]";
		case sf::Keyboard::SemiColon: return ";";
		case sf::Keyboard::Comma: return ",";
		case sf::Keyboard::Period: return ".";
		case sf::Keyboard::Quote: return "'";
		case sf::Keyboard::Slash: return "/";
		case sf::Keyboard::BackSlash: return "\\";
		case sf::Keyboard::Tilde: return "~";
		case sf::Keyboard::Equal: return "=";
		case sf::Keyboard::Dash: return "-";
		case sf::Keyboard::Space: return "Space";
		case sf::Keyboard::Return: return "Enter";
		case sf::Keyboard::BackSpace: return "Backspace";
		case sf::Keyboard::Tab: return "Tab";
	}
}

std::string GlobalValues::buttonToString(sf::Mouse::Button button) {
	switch (button) {
		case sf::Mouse::Left: return "LMB";
		case sf::Mouse::Right: return "RMB";
		case sf::Mouse::Middle: return "MMB";
		case sf::Mouse::XButton1: return "X1";
		case sf::Mouse::XButton2: return "X2";
	}
}

sf::VertexArray GlobalValues::createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color) {
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = start;
	line[0].color = color;
	line[1].position = start + vector;
	line[1].color = color;

	return line;
}