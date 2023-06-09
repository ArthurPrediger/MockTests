#pragma once

#include <memory>

class World;
class GameStatus;
class Player;
class ComplementsManager;
class ScoreCounter;

class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void Start();
	void Run();

private:
	std::unique_ptr<World> world_;
	std::unique_ptr<GameStatus> game_status_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<ComplementsManager> comps_manager_;
};