#pragma once

#include "targetver.h"

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>


#include <Shlwapi.h>
#include <Commctrl.h>

#include <stlsoft/smartptr/scoped_handle.hpp>

#include "../../MyUtility/WaitWindowClose.h"
#include "../../MyUtility/tstring.h"
#include "../../MyUtility/stlScopedClear.h"
#include "../../MyUtility/I18N.h"


#ifndef countof
#define countof(a) sizeof(a)/sizeof(a[0])
#endif

// #define MAX_LOADSTRING 100
#define WM_APP_AFTERINIT WM_APP+33


using namespace std;
using namespace Ambiesoft;