/*
 * Uncompiled header file that provides operating system specific definitions
 * of the shared object loading functions.
 */

#ifdef __linux__
  #include <dlfcn.h>

  #define LoadShared dlopen
  #define GetFunctionPointer dlsym
  #define CloseShared dlclose

  #define HANDLE_TYPE void*
  #define SHARED_EXTENSION ".so"
  #define OPEN_FLAG ,RTLD_LAZY

#else
  #include <libloaderapi.h>

  #define LoadShared LoadLibraryA
  #define GetFunctionPointer GetProcAddress
  #define CloseShared FreeLibrary

  #define HANDLE_TYPE HMODULE
  #define SHARED_EXTENSION ".dll"
  #define OPEN_FLAG
#endif
