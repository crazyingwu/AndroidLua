// IEngineService.aidl
package org.keplerproject.luajava;

// Declare any non-default types here with import statements

interface IEngineService {
    int getPid();
    int getUid();
    String getUUID();
    int eval(String script);
    int evalLuaFile(String luaStr);
}
