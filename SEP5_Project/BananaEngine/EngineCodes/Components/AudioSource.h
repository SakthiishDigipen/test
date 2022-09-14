#pragma once
#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <Precompiled/pch.h>
#include "ECSComponent.h"

namespace BE
{
    class AudioSource : public IECSComponent
    {
        //volume, pitch, audioclip asset ID
        //bool mute, bool loop, 
        //Falloff distance
        //Priority - replace manually setting channel
    };
}
#endif