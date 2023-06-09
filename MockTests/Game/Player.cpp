#include "Player.h"
#include "World.h"
#include <algorithm>

Player::Player(Location2D location, IWorld* world)
	:
	loc_(location),
	number_(1),
	world_(world)
{
}

void Player::UpdateWorldLocation(Location2D displacement)
{
	const Location2D extent = world_->GetExtent();

	int prevArrayLocation = loc_.y * extent.x + loc_.x;

	std::string& world_content = world_->GetContentRef();
	world_content[prevArrayLocation] = ' ';

	loc_.x = std::clamp(loc_.x + displacement.x, 1, extent.x - 2);
	loc_.y = std::clamp(loc_.y + displacement.y, 1, extent.y - 2);

	int curArrayLocation = loc_.y * extent.x + loc_.x;

	world_content[curArrayLocation] = number_ + char('0');
}
