#include "GameStatus.h"
#include <iostream>
#include <format>

void GameStatus::Draw() const
{
	std::cout << std::format("\n    SCORE: {}\n", score_);
	std::cout << std::format("    SCORE LOST: {}\n", score_lost_);
	std::cout << std::format("    LIFES: {}\n", std::string(std::max(player_lifes_, 0), '*'));
}

void GameStatus::AddToScore(int value)
{
	score_ += value;
}

void GameStatus::AddToScoreLost(int value)
{
	score_lost_ += value;
}

void GameStatus::PlayerLifesMinusOne()
{
	player_lifes_--;
}

bool GameStatus::IsGameOver()
{
	return player_lifes_ < 0 || score_ < score_lost_;
}
