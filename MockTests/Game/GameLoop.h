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
	GameLoop(IWorld* world, IGameStatus* game_status, IPlayer* player, IComplementsManager* comps_manager);
	~GameLoop();

	void Start();
	void Run();

private:
	std::unique_ptr<IWorld> world_;
	std::unique_ptr<IGameStatus> game_status_;
	std::unique_ptr<IPlayer> player_;
	std::unique_ptr<IComplementsManager> comps_manager_;
};