/*********************************************************************
Matt Marchant 2014
http://trederia.blogspot.com

Crush - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

//category for node targets

#ifndef COMMAND_CAT_H_
#define COMMAND_CAT_H_

namespace Category
{
    enum Type
    {
        None            = 0,
        PlayerOne       = (1 << 0), //don't rely on these actually being '1' and '2' respectively
        PlayerTwo       = (1 << 1),
        Block           = (1 << 2),
        Npc             = (1 << 3),
        GrabbedOne      = (1 << 4), //for dragging
        GrabbedTwo      = (1 << 5),
        LastTouchedOne  = (1 << 6),
        LastTouchedTwo  = (1 << 7),
        Solid           = (1 << 8),
        CarriedOne      = (1 << 9), //for carrying
        CarriedTwo      = (1 << 10),
        Item            = (1 << 11),
        Water           = (1 << 12)
    };
}

#endif //COMMAND_CAT_H_