//
//  MXTime.cpp
//  9Game
//
//  Created by mixi on 16/1/20.
//
//

#include "MXTime.hpp"

unsigned long MXTime::millisecondNow()
{
    return time(nullptr);
}
