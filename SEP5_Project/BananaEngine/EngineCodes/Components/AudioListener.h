#pragma once
#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <Precompiled/pch.h>
#include "ECSComponent.h"

namespace BE
{
    class AudioListener : public IECSComponent
    {
        //No parameters.
        
        //Requires audio source to work.
        //Attach to any object to act as "Ears"
        //When receiving sound from audio source, will play on your computer's speakers

        AudioListener() = default;
        ~AudioListener() = default;
    };
}
#endif