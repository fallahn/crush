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
    public partial class MainWindow : Form
    {
        private SfmlControl m_sfmlControl = new SfmlControl();

        private SFML.Graphics.RectangleShape m_testShape = new SFML.Graphics.RectangleShape(new SFML.Window.Vector2f(1920f, 1080f));
        private SFML.Graphics.RectangleShape m_testShape2 = new SFML.Graphics.RectangleShape(new SFML.Window.Vector2f(60f, 80f));
        
        private void InitPreview()
        {
            m_sfmlControl.Dock = DockStyle.Fill;
            //m_sfmlControl.BackgroundColour = SFML.Graphics.Color.Green;
            m_testShape2.FillColor = SFML.Graphics.Color.Blue;
            m_testShape2.Position = new SFML.Window.Vector2f(960f, 540f);
            m_sfmlControl.DrawDelegates.Add(this.DrawPreview);
            panelEditorOuter.Controls.Add(m_sfmlControl);
        }

        private void DrawPreview(SFML.Graphics.RenderWindow rw)
        {
            rw.Draw(m_testShape);
            rw.Draw(m_testShape2);
        }
    }
}
