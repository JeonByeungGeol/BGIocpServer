#pragma once

#include "BGIOObject.h"

class BGTestTimerObject : public BGIOObject
{
public:
	virtual void OnTimerCallback(int id = 0)
	{
		std::cout << "TestTimer : " << id << std::endl;
		switch (id) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
		};
	}
};