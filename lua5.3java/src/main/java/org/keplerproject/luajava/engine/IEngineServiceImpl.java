package org.keplerproject.luajava.engine;

import android.os.RemoteException;

import org.keplerproject.luajava.IEngineService;

public class IEngineServiceImpl extends IEngineService.Stub {

    // 接口实现类通过组合封装引擎功能，向上层提供服务
    private LuaEngine luaEngine = LuaEngine.getEngineService();


    @Override
    public int getPid() throws RemoteException {
        return 0;
    }

    @Override
    public int getUid() throws RemoteException {
        return 0;
    }

    @Override
    public String getUUID() throws RemoteException {
        return null;
    }

    @Override
    public int eval(String script) throws RemoteException {
        return luaEngine.eval(script);
    }

    @Override
    public int evalLuaFile(String luaStr) throws RemoteException {
        return luaEngine.evalLuaFile(luaStr);
    }

}
