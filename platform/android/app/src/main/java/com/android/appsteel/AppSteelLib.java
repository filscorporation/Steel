package com.android.appsteel;

import android.content.res.AssetManager;

public class AppSteelLib
{
     static
     {
         System.loadLibrary("SteelAndroid");
     }

     public static native void init();
     public static native void resize(int width, int height);
     public static native void initManager(AssetManager assetManager);
     public static native void step();
     public static native void terminate();
}
