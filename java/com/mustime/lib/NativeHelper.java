/**********************************************
 * Copyright (c) 2012-2013 Irvin Pang
 * Email: lpohvbe@gmail.com
 **********************************************/
package com.mustime.lib;

import android.content.Context;
import android.content.res.AssetManager;

public class NativeHelper {
	private static AssetManager sAssetManager;
	
	public static void initNative(Context context) {
		NativeHelper.sAssetManager = context.getAssets();
		NativeHelper.nativeSetAssetManager(sAssetManager);
	}
	
	private static native void nativeSetAssetManager(AssetManager assetManager);
}
