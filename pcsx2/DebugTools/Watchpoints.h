#pragma once

#include <vector>

#include "DebugInterface.h"
#include "Pcsx2Types.h"

enum WPC_COMPTYPE { WPC_EQU, WPC_GT, WPC_GTE, WPC_LT, WPC_LTE };
struct WatchPointCond
{
    DebugInterface *debug;
    u32 regCat,regNum;
    
    bool onChange;
    WPC_COMPTYPE compType;
    u128 compVal;
    u128 lastVal;

    // Currently the caller is required to supply the current value of pc
    bool Evaluate(){
        u128 regVal = debug->getRegister(regCat,regNum);
        if(lastVal == regVal)
            return false;
        if(Compare(regVal)){
            lastVal = regVal;
            return true;
        }
        return false;
    }
private:
    bool Compare(u128 regVal)
    {    
        switch(compType){
            case WPC_EQU:
                return regVal == compVal;
            case WPC_GTE:
            {
                if(regVal == compVal)
                {
                    return true;
                }
                // Let fallthrough otherwise
            }
            case WPC_GT:
            {
            //
            // Hack?
            // u128 doesn't overload > or < operators, maybe I can add that... 
            //
                if(regVal.hi == compVal.hi)
                {
                    return regVal.lo > compVal.lo;
                }
                return regVal.hi > compVal.hi;
            }
            break;
            case WPC_LTE:
            {
                if(regVal == compVal)
                {
                    return true;
                }
            }
            case WPC_LT:
            {
                if(regVal.hi == compVal.hi)
                {
                    return regVal.lo < compVal.lo;
                }
                return regVal.hi < compVal.hi;
            }
            default:
                return false; // Silently fails when unknown comparison type, maybe prompt user with notification
        }
    }
};

struct WatchPoint
{
	WatchPoint() : enabled(false)
	{}

	bool enabled;
    
	WatchPointCond cond;

	bool operator == (const WatchPoint &other) const {
		return cond.regCat == other.cond.regCat && cond.regNum == other.cond.regNum;
	}
    static std::vector<WatchPoint> watchPoints_;
    static bool WatchPointTriggered;
};



