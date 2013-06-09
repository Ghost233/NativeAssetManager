/**********************************************
 ** Copyright (c) Irvin Pang
 ** Email: lpohvbe@gmail.com
 *********************************************/
#ifndef __NATIVE_ASSET_MANAGER_H__
#define __NATIVE_ASSET_MANAGER_H__

#include "include/asset_manager.h"
#include <jni.h>

/* windows */
#if defined(WIN32) && defined(_WINDOWS)
#include <io.h>
#else
#include <unistd.h>
#endif

class NativeAssetManager
{
public:
    NativeAssetManager* sharedManager();

    AAsset* openAssetFile(const char* pszFileName);

    bool isFileExist(const char* pszFileName);

    unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);

protected:
    bool isAssetFilePath(const char* pszFilePath);

}; 

#endif