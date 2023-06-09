#pragma once

#include <memory>

class IWorld;
class IGameStatus;
class IPlayer;
class IComplementsManager;

class GameLoop
{
public:
	GameLoop();
	GameLoop(std::shared_ptr<IWorld> world, std::shared_ptr<IGameStatus> game_status, std::shared_ptr<IPlayer> player, std::shared_ptr<IComplementsManager> comps_manager);
	~GameLoop();

	void Start();
	void Run();

private:
	std::shared_ptr<IWorld> world_;
	std::shared_ptr<IGameStatus> game_status_;
	std::shared_ptr<IPlayer> player_;
	std::shared_ptr<IComplementsManager> comps_manager_;
};