#include "Score.h"

Score::Score(Graphics & gfx)
	:
	gfx(gfx)
{}

void Score::DrawScore(Color c)
{
	// small score drawing
	gfx.DrawRect(mainScoreX, mainScoreY, smallScoreCounter, mainScoreSizeY, c);

	// long score drawing
	gfx.DrawRect(LongScoreX, Graphics::ScreenHeight - 50, longScoreCounter, Graphics::ScreenHeight - 40, c);
}
