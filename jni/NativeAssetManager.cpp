/******************************************************
 ** Copyright (c) Irvin Pang
 ** Email: lpohvbe@gmail.com
 ******************************************************/
#include "NativeAssetManager.h"
#include <string>
#include <dlfcn.h>


/******************************************************
 ** For the sake of compatibility to Android 2.2,
 ** we use 'dlopen' instead of libandroid.so from ndk.
 ******************************************************/
#define LIBANDROID  "libandroid.so"

static AAssetManager* s_pAssetManager = NULL;
static void* s_pAndroidHandle  = NULL;

typedef AAssetManager*  (*type_AAssetManager_fromJava)   (JNIEnv*, jobject);
typedef AAsset*         (*type_AAssetManager_open)       (AAssetManager*, const char*, int);
typedef off_t           (*type_AAsset_getLength)         (AAsset*);
typedef int             (*type_AAsset_read)              (AAsset*, void*, size_t);
typedef void            (*type_AAsset_close)             (AAsset*);

type_AAssetManager_fromJava AAssetManager_fromJava;
type_AAssetManager_open     AAssetManager_open;
type_AAsset_getLength       AAsset_getLength;
type_AAsset_read            AAsset_read;
type_AAsset_close           AAsset_close;

extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_mustime_lib_NativeHelper_nativeSetAssetManager(JNIEnv* env,
                                                            jobject thiz,
                                                            jobject java_assetmanager) {
        // clear the error stack
        dlerror();
        s_pAndroidHandle = dlopen(LIBANDROID, RTLD_LAZY);
        const char* errorInfo = dlerror();
        if (errorInfo)
        {
            // CCLOG(errorInfo);
            return;
        }

        /* find needed symbols from android.so */
        AAssetManager_fromJava = (AAssetManager* (*)(JNIEnv*, jobject))
            dlsym(s_pAndroidHandle, "AAssetManager_fromJava");
        AAssetManager_open = (AAsset* (*)(AAssetManager*, const char*, int))
            dlsym(s_pAndroidHandle, "AAssetManager_open");
        AAsset_getLength = (off_t (*)(AAsset*))
            dlsym(s_pAndroidHandle, "AAsset_getLength");
        AAsset_read = (int (*)(AAsset*, void*, size_t))
            dlsym(s_pAndroidHandle, "AAsset_read");
        AAsset_close = (void (*)(AAsset* asset))
            dlsym(s_pAndroidHandle, "AAsset_close");

        /* initial AAssetManager */
        s_pAssetManager = AAssetManager_fromJava(env, java_assetmanager);
        if (NULL == s_pAssetManager)
        {
            // CCLOG("s_pAssetManager : is NULL");
        }
    }
}

/******************************************************
 **             API of NativeAssetManager            **
 ******************************************************/
static NativeAssetManager* s_pNativeManager = NULL;
static std::string m_sDefaultAssetPath;

NativeAssetManager* NativeAssetManager::sharedManager()
{
    if (s_pNativeManager == NULL)
    {
        s_pNativeManager = new NativeAssetManager();
        m_sDefaultAssetPath = "assets/";
    }
    return s_pNativeManager;
}

bool NativeAssetManager::isAssetFilePath(const char* pszFilePath)
{
    return (pszFilePath[0] != '/');
}

AAsset* NativeAssetManager::openAssetFile(const char* pszFileName)
{
    std::string strFileName = std::string(pszFileName);
    if (strFileName.find(m_sDefaultAssetPath) == 0)
    {
        // Found "assets/" at the beginning of the path and we don't want it
        pszFileName += strlen(m_sDefaultAssetPath.c_str());
    }

    AAsset* pAAsset = NULL;
    if (s_pAssetManager)
    {
        pAAsset = AAssetManager_open(s_pAssetManager, pszFileName, AASSET_MODE_UNKNOWN);
    }
    return pAAsset;
}

bool NativeAssetManager::isFileExist(const char* pszFileName)
{
    if (0 == strlen(pszFileName))
    {
        return false;
    }

    bool bFound = false;
    
    /* Check whether to find file from apk. */
    if (isAssetFilePath(pszFileName))
    {
        /* the file to find is relative path to assets/ */
        AAsset* pAAsset = openAssetFile(pszFileName);
        if (NULL != pAAsset)
        {
            bFound = true;
            AAsset_close(pAAsset);
        }
    }
    else
    {
        /* the file to find is out of apk */
        FILE *fp = fopen(pszFileName, "r");
        if(NULL != fp)
        {
            bFound = true;
            fclose(fp);
        }
    }
    return bFound;
}

unsigned char* NativeAssetManager::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pData = 0;

    if ((! pszFileName) || (! pszMode) || 0 == strlen(pszFileName))
    {
        return NULL;
    }

    if (isAssetFilePath(pszFileName))
    {
        AAsset* pAAsset = openAssetFile(pszFileName);
        if (NULL == pAAsset)
        {
            // CCLOG("pAAsset is NULL");
            return NULL;
        }

        /* found file in assets/! read its data & size */
        off_t size = AAsset_getLength(pAAsset);
        pData = new unsigned char[size];
        int bytesread = AAsset_read(pAAsset, (void*)pData, size);
        if (NULL != pSize)
        {
            *pSize = bytesread;
        }
        AAsset_close(pAAsset);
    }
    else
    {
        FILE *pFile = fopen(pszFileName, pszMode);
        if (!pFile)
        {
            // CCLOG("");
            return NULL;
        }

        /* found file! read its data & size */
        fseek(pFile, 0, SEEK_END);
        unsigned long size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        pData = new unsigned char[size];
        size = fread(pData,sizeof(unsigned char), size, pFile);
        if (NULL != pSize)
        {
            *pSize = size;
        }
        fclose(pFile);
    }

    return pData;
}
