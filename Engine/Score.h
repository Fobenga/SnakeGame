#include "Graphics.h"

class Score
{
public:
	Score(Graphics& gfx);
	void DrawScore(Color c);

public:
	int smallScoreCounter = mainScoreX; // Horizontal Line Size
	int longScoreCounter = mainScoreX;

private:
	// Small Score that grows fast in the top of the screen.
	static constexpr int mainScoreX = 70; // Position X on Screen
	static constexpr int mainScoreY = 20; // Position Y on Screen
	static constexpr int mainScoreSizeY = 25; // Vertical Size (static)

	// Long Score that grows everytime the Small Score completes it's padding.
	static constexpr int LongScoreX = 70;
	static constexpr int LongScoreY = Graphics::ScreenHeight - 300;
	static constexpr int LongScoreSizeY = 50;

	Graphics& gfx;
};