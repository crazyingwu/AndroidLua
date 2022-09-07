function main()
    system.init()
    local i = 0
    print(i)
    system.memoryRead(2351, 0xa2fc775c, 2)
    system.memoryWrite(2351, 0xa2fc775c, 2, 95)
    system.memoryRead(2351, 0xa2fc775c, 2)
    -- system.homePage()
end

