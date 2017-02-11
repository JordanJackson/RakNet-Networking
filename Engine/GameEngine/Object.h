#ifndef _OBJECT_H_
#define _OBJECT_H_
#pragma once

#include "CRtti.h"

class Object
{
	DECLARE_ABSTRACT_BASE_CLASS(Object)

private:
	bool initialized;

protected:
	Object() = default;

public:
	virtual ~Object() = default;

	bool isInitialized()
	{
		return initialized;
	}

	virtual void load(XMLElement* element);
	virtual void save(XMLElement* element);

	virtual void initialize();
};

#endif

