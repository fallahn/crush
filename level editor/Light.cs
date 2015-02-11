/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

Crush Map Editor - Zlib license.

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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Level_editor
{
    class Light
    {
        public SFML.Window.Vector2f Position { get; set; }
        public SFML.Graphics.Color Colour { get; set; }

        private float m_range = 0f;
        public float Range
        {
            get { return m_range; }
            set
            {
                Debug.Assert(value > 0f);
                m_range = value;
                m_inverseRange = 1f / m_range;
            }
        }

        private float m_inverseRange = 1f;
        public float InverseRange { get { return m_inverseRange; } }
    }
}
