/*********************************************************************
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
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Newtonsoft.Json;

namespace Level_editor
{
    enum Layer
    {
        Background,
        RearDetail,
        Dynamic,
        Water,
        Solid,
        FrontDetail
    }

    class Node
    {
        public Node(Panel p)
        {
            //multiply the sizes by two as UI size if half that of
            //actual game area
            m_position = new Point(p.Left * 2, p.Top * 2);
            m_size = new Size(p.Width * 2, p.Height * 2);
            var tag = (NodeData)p.Tag;
            var type = tag.type;
            switch (type)
            {
                case BodyType.Solid:
                    m_type = "Solid";
                    break;
                case BodyType.Block:
                    m_type = "Block";
                    break;
                case BodyType.PlayerOne:
                    m_type = "PlayerOne";
                    break;
                case BodyType.PlayerTwo:
                    m_type = "PlayerTwo";
                    break;
                case BodyType.Item:
                    m_type = "Item";
                    break;
                case BodyType.Water:
                    m_type = "Water";
                    break;
                case BodyType.Light:
                    m_type = "Light";
                    m_colour = p.BackColor.ToArgb();
                    break;
                default: break;
            }
        }

        [JsonConstructor]
        public Node(Point position, Size size, string type, Int32 colour = 0)
        {
            m_position = position;
            m_size = size;
            m_type = type;
            m_colour = colour;
        }
        
        public enum BodyType
        {
            [Description("Block")]
            Block = 0,
            [Description("Solid")]
            Solid,
            [Description("Player One")]
            PlayerOne,
            [Description("Player Two")]
            PlayerTwo,
            [Description("Item")]
            Item,
            [Description("Water")]
            Water,
            [Description("Light")]
            Light
        }

        private Point m_position;

        public Point Position
        {
            get { return m_position; }
            set { m_position = value; }
        }

        private Size m_size;

        public Size Size
        {
            get { return m_size; }
            set { m_size = value; }
        }

        private string m_type;
        public string Type
        {
            get { return m_type; }
            set { m_type = value; }
        }

        private Int32 m_colour;
        public Int32 Colour
        {
            get { return m_colour; }
            set { m_colour = value; }
        }
    }

    struct NodeData
    {
        public Layer layer;
        public Node.BodyType type;
    }
}
