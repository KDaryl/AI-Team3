#pragma once
#include "SFML/Graphics.hpp"
#include "Player.h"

class MiniMap
{
public:
	MiniMap(Player& player);

	void update();
	void draw(sf::Drawable& item); //Draw drawable to 
	void display(sf::RenderWindow& win); //Draw Minimap to view
	sf::View& getView() { return m_view; }
	sf::FloatRect& getViewDetector() { return m_viewDetector; }
private:
	bool m_cleared; //Bool to hold if the render texture was cleared or not
	sf::RenderTexture m_rt; //The Render Texture of the MiniMap
	sf::Sprite m_sprite; //The sprite to display at the end
	sf::RectangleShape m_rect; //For the outline of the minimap
	sf::View m_view; //The view of the render texture
	sf::FloatRect m_viewDetector; //Determiens if something is in view of the minimap, if so, draw it
	Player* m_playerPtr; //A pointer to our player, we will use this to set the views position
};