//
// Created by liamb on 6/01/2025.
//

#pragma once

#ifdef BUILD_DLL
#define API __declspec(dllexport)
#define EXTERN
#else
#define API __declspec(dllimport)
#define EXTERN extern
#endif