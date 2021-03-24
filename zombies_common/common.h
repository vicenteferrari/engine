//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_COMMON_H
#define ZOMBIES_COMMON_H

#include "types.h"

#ifdef ZOMBIESCOMMON_EXPORTS
#define ZOMBIESCOMMON_API __declspec(dllexport)
#else
#define ZOMBIESCOMMON_API __declspec(dllimport)
#endif

enum ZOMBIES_RESULT {
	ZOMBIES_ERROR = -1,
	ZOMBIES_SUCCESS = 0,
};

#define DELTATIME 1.0f / 1000.0f

#define LOG_DEBUG(string)\
{\
printf(buf, sizeof(buf), "[DEBUG] %s, %d, %s,\n%s\n", __FILE__, __LINE__, __FUNCTION__, string);\
}

#define LOG_INFO(string)\
{\
printf("[INFO] %s, %s\n", __FUNCTION__, string);\
}

#define LOG_WARNING(string)\
{\
printf("[WARNING] %s, %s\n", __FUNCTION__, string);\
}

#define LOG_ERROR(string)\
{\
printf("[ERROR] %s, %d, %s,\n%s\n", __FILE__, __LINE__, __FUNCTION__, string);\
}

#define MESSAGE_ERROR(string)\
{\
char buf[1024];\
sprintf_s(buf, sizeof(buf), "[ERROR] %s, %d, %s, %s\n", __FILE__, __LINE__, __FUNCTION__, string);\
MessageBoxA(nullptr, buf, "ERROR", MB_OK | MB_ICONERROR);\
}

#define ARRAYCOUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

#define KILOBYTES(amount) ((amount) * 1024LL)
#define MEGABYTES(amount) (KILOBYTES(amount) * 1024LL)
#define GIGABYTES(amount) (MEGABYTES(amount) * 1024LL)
#define TERABYTES(amount) (GIGABYTES(amount) * 1024LL)

#endif
