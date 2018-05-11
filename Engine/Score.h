#include "Graphics.h"
#include "FrameTimer.h"

class score
{
public:
	explicit score(Graphics& gfx);
	void draw_score(Color c) const;

	int small_score_counter = main_score_x; // Horizontal Line Size
	int long_score_counter = main_score_x;

private:

	// graphical score

	// Small Score that grows fast in the top of the screen.
	static constexpr int padding = 10;
	static constexpr int main_score_x = 70; // Position X on Screen
	static constexpr int main_score_y = 20 + padding; // Position Y on Screen
	static constexpr int main_score_size_y = 30 + padding; // Vertical Size (static)

	// Long Score that grows everytime the Small Score completes it's padding.
	static constexpr int long_score_x = 70;
	static constexpr int bottom_padding = 300;
	static constexpr int long_score_y = Graphics::ScreenHeight - bottom_padding;
	static constexpr int long_score_size_y = 50;
	static constexpr int long_score_padding = 10;

	Graphics& gfx;
};