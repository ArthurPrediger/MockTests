#pragma once

#include "Location2D.h"

class IWorld;

class IPlayer
{
public:
	virtual void UpdateWorldLocation(Location2D displacement) = 0;
	virtual Location2D GetLocation() const = 0;
	virtual int GetNumber() const = 0;
	virtual void SetNumber(int number) = 0;
};

class Player : public IPlayer
{
public:
	Player(Location2D location, IWorld* world);

	void UpdateWorldLocation(Location2D displacement) override;
	Location2D GetLocation() const override
	{
		return loc_;
	}
	int GetNumber() const override
	{
		return number_;
	}
	void SetNumber(int number) override
	{
		number_ = number;
	}
private:
	Location2D loc_;
	char number_;
	IWorld* world_;
};