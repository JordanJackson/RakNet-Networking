#ifndef _CORE_H_
#define _CORE_H_
#pragma once

#define _WINSOCKAPI_

#pragma comment(lib, "rpcrt4.lib")
#include <windows.h>

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <assert.h>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>

#include "tinyxml2.h"
using namespace tinyxml2;

#include "StandardTypes.h"

#include "randomc.h"

#include "CRtti.h"

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <RakSleep.h>
#include <BitStream.h>

#include "NetworkCommon.h"

#endif

