#pragma once

/// Market texture handler

#include "TextureHandler_PG.h"

namespace PirateGame {
	class MarketTextureHandler : public TextureHandler {
	public:
		MarketTextureHandler() {
			// Load all the textures
			bottomRightLeft.loadFromFile("PirateGameSprites/market_bottom_right_left.png");
			buySell.loadFromFile("PirateGameSprites/market_buy_sell_dark_gray.png");
			marketFarLeftRight.loadFromFile("PirateGameSprites/market_interactable_far_left_right_dark_gray.png");
			leftRightNavigation.loadFromFile("PirateGameSprites/market_left_right_navigation.png");
			marketMenu.loadFromFile("PirateGameSprites/market_menu.png");
			marketMiddle.loadFromFile("PirateGameSprites/market_middle.png");
			marketTopLeftRight.loadFromFile("PirateGameSprites/market_top_left_right_dark_gray.png");
			initialMenu.loadFromFile("PirateGameSprites/enter_island_menu_initial.png");
			initialMenuButton.loadFromFile("PirateGameSprites/enter_island_menu_button.png");
			islandBanner.loadFromFile("PirateGameSprites/island_banner.png");
			marketTopMiddle.loadFromFile("PirateGameSprites/market_top_middle_dark_gray.png");
			marketBottomMiddle.loadFromFile("PirateGameSprites/market_bottom_middle.png");
			marketLeftRightNavigation.loadFromFile("PirateGameSprites/market_left_right_navigation.png");

			shipBuyMenu.loadFromFile("PirateGameSprites/market_menu_buy_ship.png");
			shipBuyMenuTopLeft.loadFromFile("PirateGameSprites/market_buy_ship_top_left.png");
			shipBuyMenuTopRight.loadFromFile("PirateGameSprites/market_buy_ship_top_right.png");
			shipBuyMenuMiddleLeft.loadFromFile("PirateGameSprites/market_buy_ship_middle_left.png");
			shipBuyMenuMiddleRight.loadFromFile("PirateGameSprites/market_buy_ship_middle_right.png");
		}
		~MarketTextureHandler() {};

		// Getters
		sf::Texture& getBottomRightLeft() { return bottomRightLeft; }
		sf::Texture& getBuySell() { return buySell; }
		sf::Texture& getMarketFarLeftRight() { return marketFarLeftRight; }
		sf::Texture& getLeftRightNavigation() { return leftRightNavigation; }
		sf::Texture& getMarketMenu() { return marketMenu; }
		sf::Texture& getMarketMiddle() { return marketMiddle; }
		sf::Texture& getMarketTopLeftRight() { return marketTopLeftRight; }
		sf::Texture& getInitialMenu() { return initialMenu; }
		sf::Texture& getInitialMenuButton() { return initialMenuButton; }
		sf::Texture& getIslandBanner() { return islandBanner; }
		sf::Texture& getMarketTopMiddle() { return marketTopMiddle; }
		sf::Texture& getMarketBottomMiddle() { return marketBottomMiddle; }
		sf::Texture& getMarketLeftRightNavigation() { return leftRightNavigation; }

		sf::Texture& getShipBuyMenu() { return shipBuyMenu; }
		sf::Texture& getShipBuyMenuTopLeft() { return shipBuyMenuTopLeft; }
		sf::Texture& getShipBuyMenuTopRight() { return shipBuyMenuTopRight; }
		sf::Texture& getShipBuyMenuMiddleLeft() { return shipBuyMenuMiddleLeft; }
		sf::Texture& getShipBuyMenuMiddleRight() { return shipBuyMenuMiddleRight; }
	private:
		// Sprites
		sf::Texture bottomRightLeft;
		sf::Texture buySell;
		sf::Texture marketFarLeftRight;
		sf::Texture leftRightNavigation;
		sf::Texture marketMenu;
		sf::Texture marketMiddle;
		sf::Texture marketTopLeftRight;
		sf::Texture marketTopMiddle;
		sf::Texture marketBottomMiddle;
		sf::Texture initialMenu;
		sf::Texture initialMenuButton;
		sf::Texture islandBanner;
		sf::Texture marketLeftRightNavigation;

		sf::Texture shipBuyMenu;
		sf::Texture shipBuyMenuTopLeft;
		sf::Texture shipBuyMenuTopRight;
		sf::Texture shipBuyMenuMiddleLeft;
		sf::Texture shipBuyMenuMiddleRight;
	};
}