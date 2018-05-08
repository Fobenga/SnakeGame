#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	brd(gfx),
	snek({ 2,2 }),
	rng(std::random_device()()),
	goal(rng, brd, snek),
	score(gfx)
{
	// sndTitle.Play(1.0f, 1.0f);
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		sndTitle.StopOne();
	}
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	if (gameIsStarted)
	{
		if (!gameIsOver)
		{
			// Behavior control
			{
				if (wnd.kbd.KeyIsPressed(VK_UP) && p_current_direction != down)
				{
					delta_loc = { 0,-1 };
					p_current_direction = up;
				}
				else if (wnd.kbd.KeyIsPressed(VK_DOWN) && p_current_direction != up)
				{
					delta_loc = { 0,1 };
					p_current_direction = down;
				}
				else if (wnd.kbd.KeyIsPressed(VK_LEFT) && p_current_direction != right)
				{
					delta_loc = { -1,0 };
					p_current_direction = left;
				}
				else if (wnd.kbd.KeyIsPressed(VK_RIGHT) && p_current_direction != left)
				{
					delta_loc = { 1,0 };
					p_current_direction = right;
				}

				if (wnd.kbd.KeyIsPressed(VK_SHIFT))
				{
					snekMovePeriod *= instant_multiplier;
				}
				else snekMovePeriod = 0.09f;
			}

			// Moving timing and speed configurations
			{
				snekMoveCounter += dt;
				if (snekMoveCounter >= snekMovePeriod)
				{
					snekMoveCounter -= snekMovePeriod;
					const Location next = snek.GetNextHeadLocation(delta_loc);
					if (!brd.IsInsideBoard(next) ||
						snek.IsInTileExceptEnd(next) ||
						brd.CheckForObstacle(next))
					{
						gameIsOver = true;
						sndFart.Play();
						sndMusic.StopAll();
					}
					else
					{
						if (next == goal.GetLocation())
						{
							snek.GrowAndMoveBy(delta_loc);
							goal.Respawn(rng, brd, snek);
							brd.SpawnObstacle(rng, snek, goal);
							sfxEat.Play(rng, 0.8f);

							score.smallScoreCounter += ssc_increase_ratio;
							if (score.smallScoreCounter > Graphics::ScreenWidth - score_padding)
							{
								score.smallScoreCounter = score_padding;
								score.longScoreCounter += lsc_increase_ratio;
							}
						}
						else
						{
							snek.MoveBy(delta_loc);
						}
						sfxSlither.Play(rng, 0.08f);
					}
				}
				snekMovePeriod = std::max(snekMovePeriod - dt * snekSpeedupFactor, snekMovePeriodMin);
			}
		}
	}
	else
	{
		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			sndMusic.Play(1.0f, 0.6f);
			gameIsStarted = true;
		}
	}

}

// Magic happens here
void Game::ComposeFrame()
{

	if (gameIsStarted)
	{
		snek.Draw(brd);
		goal.Draw(brd);
		brd.DrawBorder();
		brd.DrawObstacles();
		score.DrawScore(Colors::Red);

		if (gameIsOver)
		{
			// SpriteCodex::DrawGameOver(350, 265, gfx);
			if (wnd.kbd.KeyIsPressed(VK_RETURN))
				exit(0);
		}

	}
	else
	{
		snek.Draw(brd);
		goal.Draw(brd);
	}
}
