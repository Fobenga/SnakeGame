#pragma once

#include "Graphics.h"
#include "Location.h"
#include "RectI.h"
#include <random>

class Board
{
public:
	Board(Graphics& gfx);
	void DrawCell(const Location& loc, Color c);
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard(const Location& loc) const;
	bool CheckForObstacle(const Location& loc) const;
	void SpawnObstacle(std::mt19937& rng, const class Snake& snake, const class Goal& goal);
	void DrawBorder();
	void DrawObstacles();
	RectI get_rect() const;


private:
	Color borderColor = Colors::White;
	Color obstacleColor = Colors::Gray;

	const int left = x;
	const int top = y;
	const int bottom = top + (borderWidth + borderPadding) * 2 + height * dimension;
	const int right = left + (borderWidth + borderPadding) * 2 + width * dimension;

	static constexpr int dimension = 20;
	static constexpr int cellPadding = 1;
	static constexpr int width = 32;
	static constexpr int height = 24;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int x = 70;
	static constexpr int y = 50;
	Graphics& gfx;
	bool hasObstacle[width * height] = { false };

};