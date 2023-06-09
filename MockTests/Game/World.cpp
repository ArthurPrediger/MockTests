#include "World.h"
#include <iostream>
#include <format>

World::World(Location2D extent)
	:
	extent_(extent)
{
	for (int y = 0; y < extent_.y; y++)
	{
		for (int x = 0; x < extent_.x; x++)
		{
			if (x == 0 || x == extent_.x - 1)
				content_.append("|");
			else if (y == extent_.y - 1)
				content_.append("-");
			else
				content_.append(" ");
		}
	}
}

void World::Draw() const
{
	for (int y = 0; y < extent_.y * extent_.x; y += extent_.x)
	{
		std::cout << std::format("    {}\n", content_.substr(y, size_t(extent_.x)));
	}
}
