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

#include "../../lsMisc/WaitWindowClose.h"
#include "../../lsMisc/tstring.h"
#include "../../lsMisc/stlScopedClear.h"
#include "../../lsMisc/I18N.h"


#ifndef countof
#define countof(a) sizeof(a)/sizeof(a[0])
#endif

// #define MAX_LOADSTRING 100
#define WM_APP_AFTERINIT WM_APP+33

#define APPVERSION L"1.1.1"

using namespace std;
using namespace Ambiesoft;