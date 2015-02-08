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

//functions related to drawig the preview via sfml

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Level_editor
{
    public class Pair<T, U>
    {
        public Pair() { }
        public Pair(T first)
        {
            this.First = first;
        }
        public Pair(T first, U second)
        {
            this.First = first;
            this.Second = second;
        }

        public T First { get; set; }
        public U Second { get; set; }
    }

    public partial class MainWindow : Form
    {
        private SfmlControl m_sfmlControl = new SfmlControl();
        private TextureResource m_textureResource = new TextureResource();
        private List<List<SFML.Graphics.RectangleShape>> m_previewLayers = new List<List<SFML.Graphics.RectangleShape>>();
        
        private void InitPreview()
        {
            m_sfmlControl.Dock = DockStyle.Fill;
            m_sfmlControl.DrawDelegates.Add(this.DrawPreview);
            m_sfmlControl.UpdateDelegates.Add(this.UpdateSprites);
            panelEditorOuter.Controls.Add(m_sfmlControl);

            //create layers to hold sprites
            for(var i = 0; i < Enum.GetValues(typeof(Layer)).Length; ++i)
            {
                m_previewLayers.Add(new List<SFML.Graphics.RectangleShape>());
            }
        }

        private void UpdateSprites(float dt)
        {
            //update sprite positions based on nodes and vice versa
            foreach (var layer in m_previewLayers)
            {
                foreach (var d in layer)
                {
                   //d.Position
                    
                }
            }
        }

        private void DrawPreview(SFML.Graphics.RenderWindow rw)
        {
            foreach(var layer in m_previewLayers)
            {
                foreach(var d in layer)
                {
                    rw.Draw(d);
                }
            }
        }
    }
}
