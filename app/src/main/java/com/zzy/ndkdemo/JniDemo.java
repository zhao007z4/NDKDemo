package com.zzy.ndkdemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

public class JniDemo {

    public static void staticTest(String s)
    {
        Log.i("JniDemo","staticTest:"+s);
    }
}
