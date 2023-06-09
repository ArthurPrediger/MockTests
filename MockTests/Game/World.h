#pragma once

#include <string>
#include "Location2D.h"

class IWorld
{
public:
	virtual Location2D GetExtent() const = 0;
	virtual std::string& GetContentRef() = 0;
};

class World : public IWorld
{
public:
	World(Location2D extent);

	void Draw() const;
	Location2D GetExtent() const override
	{
		return extent_;
	}
	std::string& GetContentRef()
	{
		return content_;
	}
private:
	Location2D extent_;
	std::string content_;
};