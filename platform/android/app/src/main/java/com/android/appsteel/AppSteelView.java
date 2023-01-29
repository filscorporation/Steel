package com.android.appsteel;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class AppSteelView extends GLSurfaceView
{
    public AppSteelView(Context context)
    {
        super(context);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer());
        Renderer.context = context;
    }

    public void Terminate()
    {
        AppSteelLib.terminate();
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public static Context context;

        public void onDrawFrame(GL10 gl)
        {
            AppSteelLib.step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            AppSteelLib.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
            AppSteelLib.initManager(context.getAssets());
            AppSteelLib.init();
        }
    }
}
