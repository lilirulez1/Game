//
// Created by liamb on 7/11/2024.
//

#pragma once

#ifndef BUILD_DLL
	#define LUNARIS_ENGINE __declspec(dllexport)
#else
	#define LUNARIS_ENGINE __declspec(dllimport)
#endif