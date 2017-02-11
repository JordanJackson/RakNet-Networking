#ifndef _ISYSTEM_H_
#define _ISYSTEM_H_
#pragma once

class ISystem
{
protected:
	virtual void initialize()
	{
	};

	virtual void update(float deltaTime)
	{
	};

	virtual void processPacket(RakNet::BitStream& bitStream)
	{
	};
};

#endif

