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
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Level_editor
{
    class Map
    {
        private int m_npcCount;
        public int NpcCount
        {
            get { return m_npcCount; }
            set { m_npcCount = value; }
        }

        private int m_npcTotal;
        public int NpcTotal
        {
            get { return m_npcTotal; }
            set { m_npcTotal = value; }
        }

        private string m_backgroundTexture;
        public string BackgroundTexture
        {
            get { return m_backgroundTexture; }
            set { m_backgroundTexture = value; }
        }

        private string m_platformTexture;
        public string PlatformTexture
        {
            get { return m_platformTexture; }
            set { m_platformTexture = value; }
        }

        private string m_mapName;
        public string MapName
        {
            get { return m_mapName; }
            set { m_mapName = value; }
        }

        private Point m_playerOneSpawn;
        public Point PlayerOneSpawn
        {
            get { return m_playerOneSpawn; }
            set { m_playerOneSpawn = value; }
        }

        private Point m_playerTwoSpawn;
        public Point PlayerTwoSpawn
        {
            get { return m_playerTwoSpawn; }
            set { m_playerTwoSpawn = value; }
        }

        private Int32 m_ambientColour;
        public Int32 AmbientColour
        {
            get { return m_ambientColour; }
            set { m_ambientColour = value; }
        }

        private Int32 m_sunColour;
        public Int32 SunColour
        {
            get { return m_sunColour; }
            set { m_sunColour = value; }
        }

        public string AudioTheme { get; set; }

        private List<Node> m_nodes;
        public List<Node> Nodes
        {
            get { return m_nodes; }
        }

        public Map()
        {
            m_nodes = new List<Node>();
        }
    }
}
