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

    /**
     *  Checks whether a file exists.
     *
     *  @param 	pszFileName The path of the file, it could be a relative or absolute path.
     *  @return true if the file exists, otherwise it will return false.
     */
    bool isFileExist(const char* pszFileName);

    /**
     *  Gets file data by filename
     *
     *  @param[in]  pszFileName The resource file name which contains the path.
     *  @param[in]  pszMode The read mode of the file.
     *  @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
     *  @return Upon success, a pointer to the data is returned, otherwise NULL.
     *  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
     */
    unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);


protected:
	/**
     *  Open file in "assets/" directory from apk.
     *
     *  @param 	pszFileName The path of the file relatived to "assets/".
     *  @return Upon success, a pointer to the AAsset is returned, otherwise NULL.
     */
    AAsset* openAssetFile(const char* pszFileName);
		
    /**
     *  Checks whether a file path is relative or absolute.
     *
     *  @param 	pszFilePath The path of the file.
     *  @return true if the file path is under "assets/", otherwise it will return false.
     */
    bool isAssetFilePath(const char* pszFilePath);

}; 

#endif