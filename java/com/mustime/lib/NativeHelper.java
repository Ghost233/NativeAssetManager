/**********************************************
 * Copyright (c) 2012-2013 Irvin Pang
 * Email: lpohvbe@gmail.com
 **********************************************/
package com.mustime.lib;

import android.content.Context;
import android.content.res.AssetManager;

public class NativeHelper {
	private Context mContext;
	private static AssetManager sAssetManager;

	NativeHelper(Context context) {
		mContext = context;
	}
	
	public static void initNative() {
		Cocos2dxHelper.sAssetManager = mContext.getAssets();
		Cocos2dxHelper.nativeSetAssetManager(sAssetManager);
	}
	
	private static native void nativeSetAssetManager(AssetManager assetManager);
}
