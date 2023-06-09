#pragma once

class IGameStatus
{
public:
	virtual void Draw() const = 0;
	virtual void AddToScore(int value) = 0;
	virtual void AddToScoreLost(int value) = 0;
	virtual void PlayerLifesMinusOne() = 0;
	virtual bool IsGameOver() = 0;
};

class GameStatus : public IGameStatus
{
public:
	GameStatus() = default;

	void Draw() const override;
	void AddToScore(int value) override;
	void AddToScoreLost(int value) override;
	void PlayerLifesMinusOne() override;
	bool IsGameOver() override;
private:
	int score_ = 0;
	int score_lost_ = 0;
	int player_lifes_ = 3;
};