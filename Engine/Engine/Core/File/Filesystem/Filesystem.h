//
// Created by liamb on 6/01/2025.
//

#pragma once

#include "Core/EngineExport.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Util/Util.h"

#define IOCP_EOF 3
#define IOCP_WRITE 1
#define IOCP_CANCEL 0

namespace Engine {

	class API Filesystem {
	public:

		typedef struct {
			OVERLAPPED overlapped;
			HANDLE hFile;
			char* buffer;
			Action<const char*> callback;
		} OverlapIOInfo;

		static char* Read(const char* path);
		static HANDLE ReadAsync(const char* path, const Action<const char*>& callback);
		static HANDLE WriteAsync(const char* path, const char* content, bool append = true, const Action<const char*>& callback = nullptr);
		static HANDLE AccessFile(const char* path, DWORD access, DWORD share, DWORD creation, DWORD async);
		static bool FileExists(const char* path);

	private:
		static HANDLE thread;
		static HANDLE hIOCP;

		static DWORD WINAPI SaveFileWorkerThread(LPVOID empty);
	};

}
