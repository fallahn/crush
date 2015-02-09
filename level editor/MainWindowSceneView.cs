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
        private Size m_sceneSize = new Size(1920, 1080);
        private SfmlControl m_sfmlControl = new SfmlControl();
        private TextureResource m_textureResource = new TextureResource();
        private List<List<SFML.Graphics.RectangleShape>> m_previewLayers = new List<List<SFML.Graphics.RectangleShape>>();

        //private string lightShaderVert =
        //    "void main()\n"
        //    + "{\n"
        //    + "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        //    + "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"
        //    + "gl_FrontColor = gl_Color;\n"
        //    + "}";

        //private string lightShaderFrag =
        //    "#version 120\n"
        //    +"uniform vec4 u_ambientColour;\n"
        //    +"uniform vec4 u_lightColour;\n"
        //    +"uniform sampler2D u_texture;\n"
        //    +"void main()\n"
        //    +"{\n"
        //    +"vec4 colour = gl_Texture2D(u_texture, gl_TexCoord[0].xy);\n"
        //    +"colour *= u_ambientColour;\n"
        //    +"gl_FragColor = colour + u_lightColour;\n"
        //    +"}";



        private SFML.Graphics.VertexArray m_grid = new SFML.Graphics.VertexArray(SFML.Graphics.PrimitiveType.Lines);
       // private SFML.Graphics.Shader m_lightShader = null;

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

            //byte[] va = Encoding.UTF8.GetBytes(lightShaderVert);
            //MemoryStream vs = new MemoryStream(va);
            //byte[] fa = Encoding.UTF8.GetBytes(lightShaderFrag);
            //MemoryStream fs = new MemoryStream(fa);

            //m_lightShader = new SFML.Graphics.Shader(vs, fs);

            m_sfmlControl.MouseDown += m_sfmlControl_MouseDown;
            m_sfmlControl.MouseUp += m_sfmlControl_MouseUp;
            m_sfmlControl.MouseMove += m_sfmlControl_MouseMove;
            m_sfmlControl.MouseClick += m_sfmlControl_MouseClick;
        }





        //event handlers to forward mouse events
        void m_sfmlControl_MouseUp(object sender, MouseEventArgs e)
        {
            var d = GetDrawableAtMouse();
            if(d != null)
            {
                var p = GetParentPanel(d);
                if (p != null)
                {
                    var mousePos = (m_sfmlControl.GetMouseWorldPosition() - d.Position) / scale;
                    MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                    mouseUp(p, evt);
                }
            }
        }
        void m_sfmlControl_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                //test if we have a sprite and forward event to parent panel
                var d = GetDrawableAtMouse();
                if (d != null)
                {
                    var p = GetParentPanel(d);
                    if(p != null)
                    {
                        var mousePos = (m_sfmlControl.GetMouseWorldPosition() - d.Position) / scale;
                        MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                        mouseDown(p, evt);         
                    }
                }
            }
        }
        void m_sfmlControl_MouseMove(object sender, MouseEventArgs e)
        {
            var d = GetDrawableAtMouse();
            if(d != null)
            {
                var p = GetParentPanel(d);
                if (p != null)
                {
                    var mousePos = (m_sfmlControl.GetMouseWorldPosition() - d.Position) / scale;
                    MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                    mouseMove(p, evt);

                    this.Text = mousePos.X.ToString() + ", " + mousePos.Y.ToString();
                }
            }
        }
        void m_sfmlControl_MouseClick(object sender, MouseEventArgs e)
        {
            if (GetDrawableAtMouse() == null)
            {
                var mousePos = m_sfmlControl.GetMouseWorldPosition() / scale;
                MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                panelEditorInner_Click(panelEditorInner, evt);
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

                if(layer == m_previewLayers[(int)Layer.Background]
                    && checkBoxSnap.Checked)
                {
                    rw.Draw(m_grid);
                }
            }           
        }

        private void BuildGrid()
        {
            m_grid.Clear();
            int step = (int)numericUpDownSnap.Value;
            SFML.Graphics.Color colour = new SFML.Graphics.Color(250, 250, 250, 100);

            for (var i = 0; i < m_sceneSize.Width; i += (step * 2))
            {
                SFML.Graphics.Vertex v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(i, 0), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(i, m_sceneSize.Height), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(i + step, m_sceneSize.Height), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(i + step, 0), colour);
                m_grid.Append(v);
            }

            for(var i = 0; i < m_sceneSize.Height; i += (step * 2))
            {
                SFML.Graphics.Vertex v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(m_sceneSize.Width, i), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(0, i), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(0, i + step), colour);
                m_grid.Append(v);
                v = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(m_sceneSize.Width, i + step), colour);
                m_grid.Append(v);
            }
        }

        private Panel GetParentPanel(SFML.Graphics.RectangleShape drawable)
        {
            foreach (Panel p in panelEditorInner.Controls)
            {
                var nd = (NodeData)p.Tag;
                if (nd.drawable == drawable)
                {
                    return p;
                }
            }
            return null;
        }

        private SFML.Graphics.RectangleShape GetDrawableAtMouse()
        {
            var mousePos = m_sfmlControl.GetMouseWorldPosition();
            for (var i = 1; i < m_previewLayers.Count; ++i) //skip background layer
            {
                foreach (var d in m_previewLayers[i])
                {
                    if (d.GetGlobalBounds().Contains(mousePos.X, mousePos.Y))
                    {
                        return d;
                    }
                }
            }
            return null;
        }
    }
}
