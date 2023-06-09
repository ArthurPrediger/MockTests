#pragma once

struct Location2D
{
	bool operator==(const Location2D& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	int x;
	int y;
};
