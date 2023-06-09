#pragma once

#include "Location2D.h"
#include <vector>
#include <random>

class IWorld;
class IGameStatus;
class IPlayer;

class IComplementsManager
{
public:
	virtual void UpdateComplementsLifetime(float dt) = 0;
};

class ComplementsManager : public IComplementsManager
{
public:
	ComplementsManager(IWorld* world, IGameStatus* game_status, IPlayer* player);

	void UpdateComplementsLifetime(float dt) override;

private:
	struct Complement
	{
		Location2D loc_;
		char number_;
		constexpr static float update_rate_ = 0.5f;
		float time_since_last_update_;
		bool dirty_ = false;
	};

private:
	IWorld* world_;
	IGameStatus* game_status_;
	IPlayer* player_;
	std::vector<Complement> complements;
	float spawn_rate_;
	float time_since_last_spawn_;

	std::mt19937 rnd_gen_;
	std::uniform_int_distribution<int> complements_dist_;
	std::uniform_int_distribution<int> location_dist_;
};