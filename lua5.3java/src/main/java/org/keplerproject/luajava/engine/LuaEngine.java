package org.keplerproject.luajava.engine;


import android.util.Log;

import org.keplerproject.luajava.LuaState;
import org.keplerproject.luajava.LuaStateFactory;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;


public class LuaEngine {

    private final String TAG = "lua_engine";
    private static final LuaState mLuaState;

    static {
        mLuaState = LuaStateFactory.newLuaState();
        mLuaState.openLibs();
    }

    private LuaEngine() {
    }

    ;

    private static LuaEngine engineService = new LuaEngine();

    /**
     * 单例实现
     *
     * @return
     */
    public static LuaEngine getEngineService() {
        return engineService;
    }

    /**
     * 脚本文本执行
     *
     * @param script
     * @return
     */
    public int eval(String script) {
        mLuaState.setTop(0);
        int ok = mLuaState.LloadString(script);
        if (ok == 0) {
            mLuaState.getGlobal("debug");
            mLuaState.getField(-1, "traceback");
            mLuaState.remove(-2);
            mLuaState.insert(-2);
            ok = mLuaState.pcall(0, 0, -2);
            if (ok == 0) {
                Log.d(TAG, "eval success");
                return 0;
            }
        } else {
            return -1;
        }
        return -2;
    }


    public int evalLuaFile(String luaStr) {
        mLuaState.LdoString(luaStr);
        //执行函数
        mLuaState.getGlobal("main");
        // 文件必须有main函数，否则无法执行
        mLuaState.call(0, 0);
        Log.d(TAG, "evalLuaFile: " + mLuaState.toString(-1));
        return 0;
    }

    private String readStream(InputStream is) {
        try {
            ByteArrayOutputStream bo = new ByteArrayOutputStream();
            int i = is.read();
            while (i != -1) {
                bo.write(i);
                i = is.read();
            }
            return bo.toString();
        } catch (IOException e) {
            Log.e("ReadStream", "读取文件流失败");
            return "";
        }
    }


}
