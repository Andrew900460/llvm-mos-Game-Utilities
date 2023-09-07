--os.execute("compile")

emu.softreset()
emu.poweron()
--emu.loadrom("main.nes")

local outputAddress = 0;

local timeCheck = false;
local timeCheckStamp = 0;

local cpuCyclesPerFrame = 29833.3333333 --29780.492

function print_output()
    local output = memory.readbyterange(outputAddress,32)
    emu.message(output)
    print(">> "..output)
end

local memoryLoc = 0x7000

memory.registerwrite(memoryLoc, function()
    local byte = memory.readbyte(memoryLoc)

    if byte == 1 then
        print_output()
    end

    if byte == 2 then
        print("PROGRAM REACHED END");
        debugger.hitbreakpoint()
    end

    if byte == 4 then
        outputAddress = memory.readword(memoryLoc+1);
    end

    if byte == 8 then
        if timeCheck then
            local newTimeStamp = debugger.getcyclescount()
            local delay = newTimeStamp - timeCheckStamp;
            print("Delay In Cpu Cycles: "..delay)
            print("Delay In Frames: ".. (delay/cpuCyclesPerFrame) )
        else
            timeCheckStamp = debugger.getcyclescount()
            print("Starting Time Checker...")
        end
        timeCheck = not timeCheck
    end

    memory.writebyte(memoryLoc, 0)
end)