-- require "/sdcard/hello.lua"
require("lib/hello")

function getAddress(address_battleAddress)
     -- 根据屏蔽基址获取其他的地址
     address_mount = address_battleAddress - 2 -- 坐骑的基址，独孤的是改为6号坐骑
     address_enemyRow = address_battleAddress - 34 -- 怪物行数基址
     address_attack = address_battleAddress - 16 -- 选择攻击，技能，物品，招降什么的
     address_flag = address_attack + 10 -- 战斗标识，未进入战斗为2305 ,战斗选择操作时为256，动画中为778
     address_scenes = address_attack + 40 -- 游戏里的场景 1为普通，95为战斗，310为物品,20为登录页面
     address_area = address_scenes + 92 -- 游戏大区地址
     address_role = address_scenes + 34 -- 选择哪个角色
     address_even = address_battleAddress - 36 -- 事件基址，如果当前人物有可对话的或者猛将队，就会为256
     address_select = address_battleAddress + 32 -- 选择哪个药品或者副将
     address_switch = address_attack + 42 -- 切换人物，设施，移动，功能
     address_zoom = address_switch + 22 -- 调整画面缩放
     address_moveNum = address_enemyRow + 2 -- 指示了有几个移动的按钮
     address_operate = address_attack + 2 -- 操作使用药品还是攻击还是其他什么的
     address_attackRefresh = address_attack - 2 -- 刷新出招的动画，写入257
     address_operateAims = address_attack + 4 -- 操作要对谁使用
     address_drug = address_battleAddress + 336 -- 药品选择地址，282为金疮药
     address_submitting = address_battleAddress - 38 -- 向服务器提交的标识 257正在提交，512未提交
     address_refresh = address_battleAddress + 156 -- 刷新页面的基址，写入0 即可刷新
     address_dialog = address_battleAddress - 44 -- 弹窗的地址
     address_selectDialog = address_dialog + 2 -- 选择弹窗,1为取消，257为确定
     address_dist = address_battleAddress + 208 -- 传送的目的地
     address_sendAuthority = address_battleAddress + 44 -- 必须设为2000才能传送过去
     address_sendAuthority2 = address_battleAddress + 42 -- 必须设为大于1的玩意才能传送过去，设1即可
     address_dialogAnimation = address_battleAddress - 72 -- 弹窗的动画，弹窗时跟着设为98
     address_newBase = address_battleAddress - 632 -- 最新基址
     address_type = address_newBase + 6 * 44 + 13410 -- 你要干什么，比如要跳转
     address_target = address_newBase + 6 * 44 + 0x3464 -- 你的目标，比如跳转到新手村

     -- 以下地址之前是单独定位的，现在测试合在一起
     address_location = address_newBase + 7356
     address_bloodExitAddress = address_location - 36
     address_experienceAddress = address_bloodExitAddress - 8
     address_level = address_bloodExitAddress - 4 -- 等级的地址

     -- 战斗血量
     address_bloodAddress = address_newBase + 0x2bdc + 6 * 76
     -- 根据定位到的战斗血量取其他的
     address_enemyBlood1 = address_bloodAddress - 456 -- 敌人1的血量

     address_attackCount = address_newBase + 0x41ec -- 秒怪基址
     address_attackTime = address_attackCount + 4

     -- 挖宝相关
     address_wbFlag = address_battleAddress + 56 -- 挖宝的标识
     address_selectExit = address_battleAddress - 34 -- 外面的选择框，用来点进挖宝
end


function mapJump(mapID)
    -- 这个跳转方法需要回到主场景，但是可以只写内存不刷新
    system.memoryWrite(pid, address_scenes, 2, 1)
    system.memoryWrite(pid, address_type, 2, 2000)
    system.memoryWrite(pid, address_target, 4, mapID)
    system.key(8)
end

function currDir()
    os.execute("cd > cd.tmp")
    local f = io.open("cd.tmp", r)
    local cwd = f:read("*a")
    f:close()
    os.remove("cd.tmp")
    return cwd
  end


function main()
    print("text")
    -- print(text)
    -- str = currDir() 
    -- logcat (string.sub(str, 1, -2) .. 'aa.txt')
    -- logcat(debug.getinfo(1).source, 2, string.len("/luafile.lua") * -1)
    tv = toast("hello java")
    logcat(tv)
    -- res()
    -- logcat(text)
    -- return
    -- system.init()
    -- system.sleep(3000)
    -- pid = 2038
    -- local baseAddr = 2736813892
    -- getAddress(baseAddr)
    -- mapJump(684)
end

