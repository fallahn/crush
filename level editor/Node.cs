﻿/*********************************************************************
Matt Marchant 2014
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
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Level_editor
{
    class Node
    {
        public Node(Panel p)
        {
            //multiply the sizes by two as UI size if half that of
            //actual game area
            m_position = new PointF(p.Top * 2, p.Left * 2);
            m_size = new SizeF(p.Width * 2, p.Height * 2);
            m_type = BodyType.Solid;
        }
        
        public enum BodyType
        {
            Block,
            Solid,
            PlayerOne,
            PlayerTwo
        }

        private PointF m_position;

        public PointF Position
        {
            get { return m_position; }
            set { m_position = value; }
        }

        private SizeF m_size;

        public SizeF Size
        {
            get { return m_size; }
            set { m_size = value; }
        }

        private BodyType m_type;
        public BodyType Type
        {
            get { return m_type; }
            set { m_type = value; }
        }
    }
}
