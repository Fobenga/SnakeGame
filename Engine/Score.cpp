#include "Score.h"

score::score(Graphics & gfx)
	:
	gfx(gfx)
{}



void score::draw_score(const Color c) const
{
	// small score drawing
	gfx.DrawRect(main_score_x, main_score_y, small_score_counter, main_score_size_y, c);


	// long score drawing
	gfx.DrawRect(long_score_x, Graphics::ScreenHeight - long_score_size_y, long_score_counter, Graphics::ScreenHeight - 40, c);

}
