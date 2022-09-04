package org.keplerproject.luajava.engine;

import android.app.Activity;
import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;


import org.keplerproject.luajava.IEngineService;


public class EngineConnection implements ServiceConnection {

    private final String TAG = "engine_connect";
    private IEngineService engineService;
//    private Consumer<IEngineService> callBack;

    public EngineConnection() {

    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        Log.d(TAG, "AIDL onServiceConnected");
        engineService = IEngineService.Stub.asInterface(service);
//        callBack.accept(engineService);
        try {
            Log.d(TAG, "onServiceConnected: :" + engineService.getPid());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        Log.d(TAG, "AIDL onServiceDisconnected");
        engineService = null;
    }

    public IEngineService getEngineService() {
        return engineService;
    }

}