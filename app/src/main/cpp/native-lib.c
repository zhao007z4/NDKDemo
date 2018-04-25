#include <jni.h>
#include <string.h>
#include <android/log.h>

#define LOG    "B4A" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__) // 定义LOGF类型


jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    return JNI_VERSION_1_6;
}

jobject jniGlobalRef(JNIEnv *env, jobject cls) {
    jobject gcls = (*env)->NewGlobalRef(env, cls);
    if (gcls == NULL)
        LOGE("Global ref failed (out of memory?)");
    return gcls;
}

jclass jniFindClass(JNIEnv *env, const char *name) {
    jclass cls = (*env)->FindClass(env, name);
    if (cls == NULL)
        LOGE("Class %s not found", name);
    else
        jniCheckException(env);
    return cls;
}

jmethodID jniGetMethodID(JNIEnv *env, jclass cls, const char *name, const char *signature) {
    jmethodID method = (*env)->GetMethodID(env, cls, name, signature);
    if (method == NULL) {
        LOGE("Method %s %s not found", name, signature);
        jniCheckException(env);
    }
    return method;
}

jfieldID jniGetFieldID(JNIEnv *env, jclass cls, const char *name, const char *type) {
    jfieldID field = (*env)->GetFieldID(env, cls, name, type);
    if (field == NULL)
        LOGE("Field %s type %s not found", name, type);
    return field;
}

jobject jniNewObject(JNIEnv *env, jclass cls, jmethodID constructor, const char *name) {
    jobject object = (*env)->NewObject(env, cls, constructor);
    if (object == NULL)
        LOGE("Create object %s failed", name);
    else
        jniCheckException(env);
    return object;
}


int jniCheckException(JNIEnv *env) {
    jthrowable ex = (*env)->ExceptionOccurred(env);
    if (ex) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        (*env)->DeleteLocalRef(env, ex);
        return 1;
    }
    return 0;
}

//访问Java中的show方法
JNIEXPORT void JNICALL Java_com_zzy_ndkdemo_MainActivity_showString(JNIEnv *env, jobject instance, jstring s) {

    //获取instance的类名称
    jclass cls = (*env)->GetObjectClass(env,instance);
    if(cls==NULL)
    {
        LOGD("Class %s not found");
    }

    //获取方法ID，第二个参数为类名称，第三个参数为方法名称，第三个参数为方法签名，详细参见签名对照表
    jmethodID id =(*env)->GetMethodID(env,cls,"show","(Ljava/lang/String;)V");
    if(id !=NULL)
    {
        //访问方法，第二个为类实例，第三个参数为方法ID,第四和第四以后为方法参数，
        // 根据返回类型不同，调用不同的CallXXXMethod方法，xxx返回类型
        (*env)->CallVoidMethod(env,instance,id,s);
    }
}

//访问Java中的showStatic静态方法
JNIEXPORT void JNICALL Java_com_zzy_ndkdemo_MainActivity_showStaticString(JNIEnv *env, jobject instance, jstring s) {

    //获取instance的类名称
    jclass cls = (*env)->GetObjectClass(env,instance);
    if(cls==NULL)
    {
        LOGD("Class %s not found");
    }

    //获取静态方法ID，第二个参数为类名称，第三个参数为方法名称，第三个参数为方法签名，详细参见签名对照表
    jmethodID id =(*env)->GetStaticMethodID(env,cls,"showStatic","(Ljava/lang/String;)V");
    if(id !=NULL)
    {
        //访问方法，第二个为类名称，第三个参数为方法ID,第四和第四以后为方法参数，
        // 根据返回类型不同，调用不同的CallStaticXXXMethod方法，xxx返回类型
        (*env)->CallStaticVoidMethod(env,cls,id,s);
    }
}

JNIEXPORT void JNICALL Java_com_zzy_ndkdemo_MainActivity_showStaticInOtherClass(JNIEnv *env, jobject instance, jstring s) {

    jclass cls = (*env)->FindClass(env, "com/zzy/ndkdemo/JniDemo");
    if (cls == NULL) {
        LOGD("Class %s not found");
    }

    jmethodID id =(*env)->GetStaticMethodID(env,cls,"staticTest","(Ljava/lang/String;)V");
    if(id !=NULL)
    {
        (*env)->CallStaticVoidMethod(env,cls,id,s);
    }
}

//访问User中的字段name
JNIEXPORT jstring JNICALL Java_com_zzy_ndkdemo_MainActivity_showUserName(JNIEnv *env, jobject instance, jobject user) {

    //获取User的类名称
    jclass cls=(*env)->GetObjectClass(env,user);
    if (cls == NULL) {
        LOGD("Class %s not found");
        return NULL;
    }

    //获取字段ID，第二个参数类名称，第三个参数字段名，第三个参数字段签名，详细参见签名对照表
    jfieldID id = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
    if(id==NULL)
    {
        LOGD("Field token not found");
        return NULL;
    }
    //获取字段内容，调用GetXXXField，xxx为字段类型。除基本类型外，其他的都使用GetObjectField
    jstring name=(jstring) (*env)->GetObjectField(env,user,id);
    return name;
}

JNIEXPORT jint JNICALL Java_com_zzy_ndkdemo_MainActivity_showUserAge(JNIEnv *env, jobject instance, jobject user) {

    jclass cls=(*env)->GetObjectClass(env,user);
    if (cls == NULL) {
        LOGD("Class %s not found");
        return 0;
    }

    jfieldID id = (*env)->GetFieldID(env,cls,"age","I");
    if(id==NULL)
    {
        LOGD("Field token not found");
        return 0;
    }
    jint age=(*env)->GetIntField(env,user,id);
    return age;
}

//访问User中的静态字段Token
JNIEXPORT jstring JNICALL Java_com_zzy_ndkdemo_MainActivity_showUserStaticToken(JNIEnv *env, jobject instance, jobject user) {

    //获取User的类名称
    jclass cls=(*env)->GetObjectClass(env,user);
    if (cls == NULL) {
        LOGD("Class %s not found");
        return NULL;
    }

    //获取静态字段ID，第二个参数类名称，第三个参数字段名，第三个参数字段签名，详细参见签名对照表
    jfieldID id = (*env)->GetStaticFieldID(env,cls,"token","Ljava/lang/String;");
    if(id==NULL)
    {
        LOGD("Field token not found");
        return NULL;
    }

    //获取字段内容，调用GetStaticXXXField，xxx为字段类型。除基本类型外，其他的都使用GetStaticObjectField
    jstring token=(*env)->GetStaticObjectField(env,cls,id);
    return token;
}

//更新user内容
JNIEXPORT jobject JNICALL Java_com_zzy_ndkdemo_MainActivity_updateUser(JNIEnv *env, jobject instance,jobject user)
{
    //获取User的类名称
    jclass cls=(*env)->GetObjectClass(env,user);
    if (cls == NULL) {
        LOGD("Class %s not found");
        return NULL;
    }

    //获取每一个字段ID，第二个参数类名称，第三个参数字段名，第三个参数字段签名，详细参见签名对照表
    jfieldID idName = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
    jfieldID idAge = (*env)->GetFieldID(env,cls,"age","I");
    jfieldID idToken = (*env)->GetStaticFieldID(env,cls,"token","Ljava/lang/String;");

    //将cha字符串转成jstring
    jstring name = (*env)->NewStringUTF(env, "李四");
    jstring token = (*env)->NewStringUTF(env, "new token");

    //更新字段内容，调用setxxxField，xxx为字段类型，如果是静态字段还需要加上static
    //第二个参数为需要修改的类实例或则类名，第三个参数为字段Id,第四个参数为需要修改的内容
    (*env)->SetObjectField(env,user,idName,name);
    (*env)->SetIntField(env,user,idAge,20);
    (*env)->SetStaticObjectField(env,cls,idToken,token);

    return user;
}

//创建user实例
JNIEXPORT jobject JNICALL Java_com_zzy_ndkdemo_MainActivity_createUser(JNIEnv *env, jobject instance)
{
    //在指定路径查找到USer类名称
    jclass cls = (*env)->FindClass(env, "com/zzy/ndkdemo/User");
    if (cls == NULL) {
        LOGD("Class %s not found");
    }

    //获取User类的构造方法ID,第二个参数为类名称，第三个参数固定为"<init>"
    //第四个参数为构造函数签名，详细参见签名对照表
    jmethodID id= (*env)->GetMethodID(env, cls, "<init>", "()V");

    //实例化User类，第二个参数类名称，第三个参数构造方法ID
    jobject user = (*env)->NewObject(env, cls, id);
    if (user == NULL) {
        LOGD("Create User failed");
    }

    //获取每一个字段ID，第二个参数类名称，第三个参数字段名，第三个参数字段签名，详细参见签名对照表
    jfieldID idName = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
    jfieldID idAge = (*env)->GetFieldID(env,cls,"age","I");
    jfieldID idToken = (*env)->GetStaticFieldID(env,cls,"token","Ljava/lang/String;");

    //将cha字符串转成jstring
    jstring name = (*env)->NewStringUTF(env, "王五");
    jstring token = (*env)->NewStringUTF(env, "second token");

    //赋予字段内容，调用setxxxField，xxx为字段类型，如果是静态字段还需要加上static
    //第二个参数为需要修改的类实例或则类名，第三个参数为字段Id,第四个参数为需要修改的内容
    (*env)->SetObjectField(env,user,idName,name);
    (*env)->SetIntField(env,user,idAge,10);
    (*env)->SetStaticObjectField(env,cls,idToken,token);

    return user;
}
