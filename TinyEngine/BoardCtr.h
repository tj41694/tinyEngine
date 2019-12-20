#pragma once
#include "Script.h"

using namespace TEngine;
class BoardCtr :
	public Script {
public:
	BoardCtr();
	void Start();
	void Update();
	virtual ~BoardCtr();
};