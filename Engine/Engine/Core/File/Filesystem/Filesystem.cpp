//
// Created by liamb on 6/01/2025.
//

#include "pch.h"
#include "Filesystem.h"

namespace Engine {

	HANDLE CreateNewCompletionPort() {
		return CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	};

	HANDLE Filesystem::hIOCP = CreateNewCompletionPort();

	[[noreturn]] DWORD Filesystem::SaveFileWorkerThread(LPVOID empty) {
		ULONG_PTR completionKey;
		DWORD bytesTransferred;
		OVERLAPPED* overlap;

		for (;;) {
			const BOOL completionStatus = GetQueuedCompletionStatus(hIOCP, &bytesTransferred, &completionKey, &overlap, INFINITE);
			const DWORD err = GetLastError();

			if (completionStatus) {
				switch (completionKey) {
					case IOCP_WRITE: {
						break;
					}
					case IOCP_EOF: {
						const auto* info = reinterpret_cast<OverlapIOInfo*>(overlap);
						if (info->callback) {
							info->callback(info->buffer);
						}

						delete info->buffer;
						delete info;
						break;
					}
					default: break;
				}
			} else {
				if (overlap != nullptr) {
					fprintf(stderr, "overlap not null");
				} else if (err != WAIT_TIMEOUT) {
					fprintf(stderr, "timeout");
				}
			}
		}
	}

	HANDLE Filesystem::thread = CreateThread(nullptr, 0, SaveFileWorkerThread, nullptr, 0, nullptr);

	LPCTSTR ErrorMessage(const DWORD error) {
		LPVOID lpMsgBuf;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
						  FORMAT_MESSAGE_IGNORE_INSERTS,
					  nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  reinterpret_cast<LPTSTR>(&lpMsgBuf), 0, nullptr);

		return static_cast<LPCTSTR>(lpMsgBuf);
	}

	void GetReadWriteError() {
		switch (DWORD dwError = 0; dwError = GetLastError()) {
			case ERROR_IO_PENDING:
				break;
			case ERROR_INVALID_USER_BUFFER:
				throw std::exception("ERROR_INVALID_USER_BUFFER");
			case ERROR_NOT_ENOUGH_MEMORY:
				throw std::exception("ERROR_NOT_ENOUGH_MEMORY");
			case ERROR_NOT_ENOUGH_QUOTA:
				throw std::exception("ERROR_NOT_ENOUGH_QUOTA");
			case ERROR_INSUFFICIENT_BUFFER:
				throw std::exception("ERROR_INSUFFICIENT_BUFFER");
			case ERROR_OPERATION_ABORTED:
				throw std::exception("ERROR_OPERATION_ABORTED");
			default: {
				const LPCTSTR errMsg = ErrorMessage(dwError);
				_tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
				LocalFree(LPVOID(errMsg));
				char buffer[1023];
				const int n = sprintf_s(buffer, 1023, "GetOverlappedResult failed (%d): %s\n", dwError, errMsg);
				buffer[n] = '\0';
				throw std::exception(buffer);
			}
		}
	}

	DWORD GetFileError() {
		DWORD dwError = 0;
		switch (dwError = GetLastError()) {
			case 0:
			case ERROR_ALREADY_EXISTS:
				break;
			case ERROR_FILE_EXISTS:
				throw std::exception("ERROR_FILE_EXISTS");
			case ERROR_FILE_NOT_FOUND:
				throw std::exception("ERROR_FILE_NOT_FOUND");
			default: {
				const LPCTSTR errMsg = ErrorMessage(dwError);
				_tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
				LocalFree((LPVOID)errMsg);
				char buffer[1023];
				const int n = sprintf_s(buffer, 1023, "GetOverlappedResult failed (%d): %s\n", dwError, errMsg);
				buffer[n] = '\0';
				throw std::exception(buffer);
			}
		}
		return dwError;
	}

	BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile, const DWORD completionKey) {
		return CreateIoCompletionPort(hFile, hIoPort, completionKey, 0) == hIoPort;
	}

	char* Filesystem::Read(const char* path) {
		HANDLE hFile = AccessFile(path, GENERIC_READ, NULL, OPEN_EXISTING, NULL);
		try {
			GetFileError();
		} catch (const std::exception& exception) {
			throw;
		}

		const DWORD dwFileSize = GetFileSize(hFile, nullptr);
		DWORD dwBytesRead = 0;

		const auto buffer = new char[dwFileSize + 1];

		if (!ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, nullptr)) {
			try {
				GetReadWriteError();
			} catch (const std::exception& exception) {
				throw;
			}
		}
		CloseHandle(hFile);
		buffer[dwFileSize] = '\0';

		return buffer;
	}

	HANDLE Filesystem::ReadAsync(const char* path, const Action<const char*>& callback) {
		HANDLE hFile = AccessFile(path, GENERIC_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED);
		try {
			GetFileError();
		} catch (const std::exception& exception) {
			throw;
		}
		AssociateFileCompletionPort(hIOCP, hFile, IOCP_WRITE);

		const DWORD dwFileSize = GetFileSize(hFile, nullptr);
		DWORD dwBytesRead = 0;

		auto* info = new OverlapIOInfo{};
		info->hFile = hFile;
		info->callback = callback;
		info->buffer = new char[dwFileSize + 1];
		info->buffer[dwFileSize] = '\0';

		if (!ReadFile(hFile, info->buffer, dwFileSize, &dwBytesRead, &info->overlapped)) {
			try {
				GetReadWriteError();
			} catch (const std::exception& exception) {
				throw;
			}
		}
		PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));

		return hFile;
	}

	HANDLE Filesystem::WriteAsync(const char* path, const char* content, const bool append, const Action<const char*>& callback) {
		HANDLE hFile;
		if (append) {
			hFile = AccessFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING,
							   FILE_FLAG_OVERLAPPED);
		} else {
			hFile = AccessFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS,
							   FILE_FLAG_OVERLAPPED);
		}
		try {
			GetFileError();
		} catch (const std::exception& exception) {
			throw;
		}

		AssociateFileCompletionPort(hIOCP, hFile, IOCP_WRITE);

		const DWORD dwBufferSize = strlen(content);
		DWORD dwBytesRead = 0;

		auto* info = new OverlapIOInfo{};
		info->hFile = hFile;
		info->callback = callback;
		info->buffer = new char[dwBufferSize + 1];
		info->buffer[dwBufferSize] = '\0';
		strncpy_s(info->buffer, dwBufferSize + 1, content, dwBufferSize);

		if (append) {
			info->overlapped.Offset += GetFileSize(hFile, nullptr);
		}
		if (!WriteFile(info->hFile, info->buffer, strlen(info->buffer), &dwBytesRead, &info->overlapped)) {
			try {
				GetReadWriteError();
			} catch (const std::exception& exception) {
				throw;
			}
		}
		PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));
		return hFile;
	}

	bool Filesystem::FileExists(const char* path) {
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile(path, &findFileData);
		const bool exists = hFind != INVALID_HANDLE_VALUE;
		if (exists) {
			FindClose(hFind);
		}
		return exists;
	}

	HANDLE Filesystem::AccessFile(const char* path, const DWORD access, const DWORD share, const DWORD creation, const DWORD async) {
		if (!FileExists(path)) {
			const auto delimiter = "\\";
			char* nextToken;
			auto* filePathToken = new char[strlen(path) + 1];
			strncpy_s(filePathToken, strlen(path) + 1, path,
					  strlen(path) + 1);
			const char* folder = strtok_s(filePathToken, delimiter, &nextToken);
			while (folder != nullptr) {
				const char* next = strtok_s(nullptr, delimiter, &nextToken);
				if (next == nullptr) {
					break;
				}
				CreateDirectory(folder, nullptr);
				folder = next;
			}
			delete filePathToken;
		}
		return CreateFile(path, access, share, nullptr, creation, async, nullptr);
	}

}
