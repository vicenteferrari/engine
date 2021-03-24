//
// Created by vfs on 5/23/2020.
//

// from stdlib
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <cassert>

// from windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// from extern
/* #include "raknet/RakPeerInterface.h" */
/* #include "raknet/MessageIdentifiers.h" */
/* #include "raknet/BitStream.h" */
/* #include "raknet/RakNetTypes.h" */

// to compile
#include "common.h"
#include "crc32.h"
#include "maths.h"
#include "memory.h"
#include "n_point_light.h"
#include "n_zombie.h"
#include "network.h"
#include "ring_buffer.h"

#include "crc32.cpp"
#include "maths.cpp"
#include "memory.cpp"
#include "n_point_light.cpp"
#include "n_zombie.cpp"
#include "network.cpp"
#include "ring_buffer.cpp"



//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
//    // Perform actions based on the reason for calling.
//    switch (fdwReason)
//    {
//        case DLL_PROCESS_ATTACH:
//            // Initialize once for each new process.
//            // Return FALSE to fail DLL load.
//            break;
//
//        case DLL_THREAD_ATTACH:
//            // Do thread-specific initialization.
//            break;
//
//        case DLL_THREAD_DETACH:
//            // Do thread-specific cleanup.
//            break;
//
//        case DLL_PROCESS_DETACH:
//            // Perform any necessary cleanup.
//            break;
//    }
//    return TRUE;  // Successful DLL_PROCESS_ATTACH.
//}
