//
// Created by liamb on 8/01/2025.
//

#pragma once

class Service {
public:
	virtual ~Service() = default;

	virtual void Startup() = 0;
	virtual void FixedUpdate() {}
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
};