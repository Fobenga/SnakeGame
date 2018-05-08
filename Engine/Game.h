#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Board.h"
#include "Snake.h"
#include "Goal.h"
#include <random>
#include "SoundEffect.h"
#include "FrameTimer.h"
#include "Score.h"

class Game
{
public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Go();

private:
	void ComposeFrame();
	void UpdateModel();

private:
	// constructor calls
	MainWindow & wnd;
	Graphics gfx;
	Board brd;
	Snake snek;
	Location delta_loc = { 1,0 };
	std::mt19937 rng;
	FrameTimer ft;
	Goal goal;
	Score score;

	// sound effect calls
	SoundEffect sfxEat = SoundEffect({ L"Sounds\\Eat.wav" });
	SoundEffect sfxSlither = SoundEffect({ L"Sounds\\Slither0.wav",L"Sounds\\Slither1.wav",L"Sounds\\Slither2.wav" });
	Sound sndMusic = Sound(L"Sounds\\Music_Loop.wav", Sound::LoopType::AutoFullSound);
	Sound sndTitle = Sound(L"Sounds\\Title.wav");
	Sound sndFart = Sound(L"Sounds\\Fart.wav");

	// snake movement variables 
	static constexpr float snekMovePeriodMin = 0.06f;
	static constexpr float instant_multiplier = 0.8f;
	float snekMovePeriod = 0.09f;
	float snekMoveCounter = 0.0f;
	float snekSpeedupFactor = 0.001f;
	enum pos { up, down, left, right };
	int p_current_direction = right;
	bool gameIsOver = false;
	bool gameIsStarted = false;

	// score constants
	static constexpr int ssc_increase_ratio = 50;
	static constexpr int lsc_increase_ratio = 40;
	static constexpr int score_padding = 70;
};