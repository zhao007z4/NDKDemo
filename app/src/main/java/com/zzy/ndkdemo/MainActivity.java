package com.zzy.ndkdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        showString("hello");
        showStaticString("hello static");
        showStaticInOtherClass("hello static in other class");

        User user = new User();
        user.name = "zhang san";
        user.age = 30;
        User.token = "2018-2011—3223";

        String name = showUserName(user);
        Log.i("MainActivity","show name:"+name);
        int age = showUserAge(user);
        Log.i("MainActivity","show age:"+age);


        String token =showUserStaticToken(user);
        Log.i("MainActivity","show static token:"+token);

        user = updateUser(user);
        Log.i("MainActivity","updateUser name:"+user.name+" age:"+user.age+" token:"+User.token);

        user = createUser();
        Log.i("MainActivity","createUser name:"+user.name+" age:"+user.age+" token:"+User.token);

    }

    public void show(String s){
        Log.i("MainActivity","show:"+s);
    }

    public static void showStatic(String s){
        Log.i("MainActivity","show static:"+s);
    }


    public native void showString(String s);
    public native void showStaticString(String s);
    public native void showStaticInOtherClass(String s);

    public native String showUserName(User user);
    public native int showUserAge(User user);
    public native String showUserStaticToken(User user);

    public native User updateUser(User user);
    public native User createUser();
}
