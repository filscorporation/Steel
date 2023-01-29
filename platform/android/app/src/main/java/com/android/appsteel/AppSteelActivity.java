package com.android.appsteel;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import java.io.File;


public class AppSteelActivity extends Activity
{
    AppSteelView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        mView = new AppSteelView(getApplication());
	    setContentView(mView);
    }

    @Override protected void onDestroy()
    {
        super.onDestroy();
        mView.Terminate();
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }
}
