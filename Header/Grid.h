#pragma once
#include "Vector2f.h"
#include <list>

//Cell Class
class Cell
{
public:
	Cell(){ }
	Cell(Vector2f pos, sf::Vector2i gridPos) :
		position(pos),
		gridPosition(gridPos),
		cost(9999),
		visited(false),
		isWall(false)
	{
	}

	void setRectangle(float sizeX, float sizeY)
	{
		rect = sf::IntRect(position.x - (sizeX / 2), position.y - (sizeY / 2), sizeX, sizeY);
	}

	void addSplitGridPosition(float x, float y)
	{
		splitGridPos.push_back(Vector2f(x, y));
	}
	void addSplitGridPosition(Vector2f pos)
	{
		splitGridPos.push_back(pos);
	}

	int cost;
	bool visited, isWall;
	Vector2f position; //Position in the world, (2500, 2500) ex.
	sf::Vector2i gridPosition; //Grid position on the map (1,1), (2,5) etc..
	Cell* previous; //Previous Cell
	std::vector<Vector2f> splitGridPos; //Split Grid Positions (split the grid down to 16 smaller grids, this will be one of them)
	sf::IntRect rect; //Rectangle of the cell, we can do simple AABB collision detection to see if the cell is colliding with something
};

class Grid
{
public:
	Grid();

	void BFS(Cell& from, Cell& goal, std::vector<Cell*>& cells);

	void draw(sf::RenderWindow& window);
private:
	Vector2f m_cellSize; //Size of each grid cell
	std::map<std::string, Cell> m_splitGridcells; //The 16 squares that split the overall grid
	std::map<std::string, std::vector<Cell*>> m_splitCells; //The map of Vector of cells for each split grid 
	std::map<std::string, Cell> m_cells; //Every cell in the map
	sf::RectangleShape m_rectangle; //Rectangle for drawing each cell

	//Text for outputting grid position
	sf::Text m_text;
	sf::Font m_font;

	//Pathfinding variables
	std::list<Cell*> m_bfsQueue;
};