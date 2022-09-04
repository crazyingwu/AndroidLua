package org.keplerproject.luajava.engine;

import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;

import androidx.annotation.NonNull;

import com.topjohnwu.superuser.ipc.RootService;

import java.util.UUID;

/**
 * EngineBootstrapService层提供服务启动，获取接口层
 */
public class EngineBootstrapService extends RootService {

    private final static String LUAJAVA_LIB = "luajava";
    private static boolean isLoad = false;

    static {
        // root授权下才加载动态库
        if (Process.myUid() == 0) {
            System.loadLibrary(LUAJAVA_LIB);
            isLoad = true;
        }
    }

    private final String TAG = "engine_api_service";

    private final String uuid = UUID.randomUUID().toString();

    @Override
    public void onCreate() {
        Log.d(TAG, "AIDLService: onCreate, " + uuid);
    }

    @Override
    public void onRebind(@NonNull Intent intent) {
        // This callback will be called when we are reusing a previously started root process
        Log.d(TAG, "AIDLService: onRebind, daemon process reused");
    }

    @Override
    public IBinder onBind(@NonNull Intent intent) {
        Log.d(TAG, "AIDLService: onBind");
        if (!isLoad) {
            Log.d(TAG, "lua library didn't load, application can't work!");
        }
        return new IEngineServiceImpl();
    }

    @Override
    public boolean onUnbind(@NonNull Intent intent) {
        Log.d(TAG, "AIDLService: onUnbind, client process unbound");
        // Return true here so onRebind will be called
        return true;
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "AIDLService: onDestroy");
    }
}
