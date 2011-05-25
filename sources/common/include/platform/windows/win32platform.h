/*
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef WIN32
#ifndef _WIN32PLATFORM_H
#define _WIN32PLATFORM_H

//define missing PRI_64 specifiers
#ifndef PRId64
#define PRId64 "lld"
#endif /* PRId64 */

#ifndef PRIu64
#define PRIu64 "llu"
#endif /* PRIu64 */

#ifndef PRIx64
#define PRIx64 "llx"
#endif /* PRIx64 */

#ifndef PRIX64
#define PRIX64 "llX"
#endif /* PRIX64 */

#include "platform/baseplatform.h"
#include <assert.h>
#include <time.h>
#include <io.h>
#include <Shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <process.h>

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long int uint64_t;
typedef char int8_t;
typedef short int int16_t;
typedef long int int32_t;
typedef long long int int64_t;
#define atoll atol

#define DLLEXP __declspec(dllexport)
#define __func__ __FUNCTION__
#define COLOR_TYPE WORD
#define FATAL_COLOR FOREGROUND_INTENSITY | FOREGROUND_RED
#define ERROR_COLOR FOREGROUND_RED
#define WARNING_COLOR 6
#define INFO_COLOR FOREGROUND_GREEN | FOREGROUND_BLUE
#define DEBUG_COLOR 7
#define FINE_COLOR 8
#define FINEST_COLOR 8
#define NORMAL_COLOR 8
#define SET_CONSOLE_TEXT_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)

#define MSG_NOSIGNAL 0
#define READ_FD _read
#define WRITE_FD _write
#define CLOSE_SOCKET(fd) closesocket(fd)
#define LASTSOCKETERROR WSAGetLastError()
#define SOCKERROR_CONNECT_IN_PROGRESS	WSAEWOULDBLOCK
#define SOCKERROR_SEND_IN_PROGRESS		WSAEWOULDBLOCK
#define SET_UNKNOWN 0
#define SET_READ 1
#define SET_WRITE 2
#define SET_TIMER 3
#define RESET_TIMER(timer,sec,usec)
#define FD_READ_CHUNK 32768
#define FD_WRITE_CHUNK FD_READ_CHUNK
#define PATH_SEPARATOR '\\'
#define LIBRARY_NAME_PATTERN "%s.dll"
#define LIB_HANDLER HMODULE
#define LOAD_LIBRARY(file,flags) UnicodeLoadLibrary((file))
#define OPEN_LIBRARY_ERROR "OPEN_LIBRARY_ERROR NOT IMPLEMENTED YET"
#define GET_PROC_ADDRESS(libHandler, procName) GetProcAddress((libHandler), (procName))
#define FREE_LIBRARY(libHandler) FreeLibrary((libHandler))
#define SRAND() srand((uint32_t)time(NULL));
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IRGRP 0000
#define S_IWGRP 0000
#define Timestamp struct tm
#define Timestamp_init {0, 0, 0, 0, 0, 0, 0, 0, 0}
#define snprintf sprintf_s


#define gmtime_r(_p_time_t, _p_struct_tm) *(_p_struct_tm) = *gmtime(_p_time_t);

#define CLOCKS_PER_SECOND 1000000L
#define GETCLOCKS(result) \
do { \
    struct timeval ___timer___; \
    gettimeofday(&___timer___,NULL); \
    result=(double)___timer___.tv_sec*(double)CLOCKS_PER_SECOND+(double) ___timer___.tv_usec; \
}while(0);

#define GETNTP(result) \
do { \
	struct timeval tv; \
	gettimeofday(&tv,NULL); \
	result=(((uint64_t)tv.tv_sec + 2208988800U)<<32)|((((uint32_t)tv.tv_usec) << 12) + (((uint32_t)tv.tv_usec) << 8) - ((((uint32_t)tv.tv_usec) * 1825) >> 5)); \
}while (0);

#define GETCUSTOMNTP(result,value) \
do { \
	struct timeval tv; \
	tv.tv_sec=(long)(value/CLOCKS_PER_SECOND); \
	tv.tv_usec=(long)(value-tv.tv_sec*CLOCKS_PER_SECOND); \
	result=(((uint64_t)tv.tv_sec + 2208988800U)<<32)|((((uint32_t)tv.tv_usec) << 12) + (((uint32_t)tv.tv_usec) << 8) - ((((uint32_t)tv.tv_usec) * 1825) >> 5)); \
}while (0);

typedef void (*SignalFnc)(void);

typedef struct _select_event {
	uint8_t type;
} select_event;

struct iovec {
	void *iov_base; /* Base address. */
	size_t iov_len; /* Length. */
};

struct msghdr {
	void *msg_name; /* optional address */
	int msg_namelen; /* size of address */
	struct iovec *msg_iov; /* scatter/gather array */
	int msg_iovlen; /* # elements in msg_iov */
	void *msg_control; /* ancillary data, see below */
	int msg_controllen; /* ancillary data buffer len */
	int msg_flags; /* flags on received message */
};

#define FD_COPY(f, t)   (void)(*(t) = *(f))

DLLEXP string format(string fmt, ...);
DLLEXP string vFormat(string fmt, va_list args);
DLLEXP void replace(string &target, string search, string replacement);
DLLEXP bool fileExists(string path);
DLLEXP string lowerCase(string value);
DLLEXP string upperCase(string value);
DLLEXP string changeCase(string &value, bool lowerCase);
DLLEXP string tagToString(uint64_t tag);
DLLEXP bool setFdNonBlock(int32_t fd);
DLLEXP bool setFdNoSIGPIPE(int32_t fd);
DLLEXP bool setFdKeepAlive(int32_t fd);
DLLEXP bool setFdNoNagle(int32_t fd);
DLLEXP bool setFdReuseAddress(int32_t fd);
DLLEXP bool setFdOptions(int32_t fd);
DLLEXP bool deleteFile(string path);
DLLEXP string getHostByName(string name);
DLLEXP bool isNumeric(string value);
DLLEXP void split(string str, string separator, vector<string> &result);
DLLEXP uint64_t getTagMask(uint64_t tag);
DLLEXP string generateRandomString(uint32_t length);
DLLEXP void lTrim(string &value);
DLLEXP void rTrim(string &value);
DLLEXP void trim(string &value);
DLLEXP map<string, string> mapping(string str, string separator1, string separator2, bool trimStrings);
DLLEXP void splitFileName(string fileName, string &name, string &extension, char separator = '.');
DLLEXP double getFileModificationDate(string path);
DLLEXP string normalizePath(string base, string file);
DLLEXP bool listFolder(string path, vector<string> &result,
		bool normalizeAllPaths = true, bool includeFolders = false,
		bool recursive = true);
DLLEXP bool moveFile(string src, string dst);
DLLEXP void installQuitSignal(SignalFnc pQuitSignalFnc);
DLLEXP void installConfRereadSignal(SignalFnc pConfRereadSignalFnc);
DLLEXP time_t timegm(struct tm *tm);
DLLEXP char *strptime(const char *buf, const char *fmt, struct tm *timeptr);
DLLEXP int strcasecmp(const char *s1, const char *s2);
DLLEXP int strncasecmp(const char *s1, const char *s2, size_t n);
DLLEXP int vasprintf(char **strp, const char *fmt, va_list ap, int size = 256);
DLLEXP int gettimeofday(struct timeval *tv, void* tz);
DLLEXP void InitNetworking();
DLLEXP int sendmsg(int s, const struct msghdr *msg, int flags);
DLLEXP HMODULE UnicodeLoadLibrary(string fileName);
DLLEXP int inet_aton(const char *pStr, struct in_addr *pRes);


#endif /* _WIN32PLATFORM_H */
#endif /* WIN32 */
