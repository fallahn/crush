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
        private List<Light> m_lights = new List<Light>();
        private const int m_maxLights = 4;
        
        private Size m_sceneSize = new Size(1920, 1080);
        private SfmlControl m_sfmlControl = new SfmlControl();
        private TextureResource m_textureResource = new TextureResource();
        private List<List<SFML.Graphics.RectangleShape>> m_previewLayers = new List<List<SFML.Graphics.RectangleShape>>();

        private string lightShaderVert =
            "#define LIGHT_COUNT 4\n"
            + "#define LIGHT_DISTANCE 50.0\n"
            + "uniform mat4 u_lightPositions;\n"
            + "uniform mat4 u_inverseWorldViewMatrix;\n"

            + "varying vec3 v_pointLightDirections[LIGHT_COUNT];\n"

            + "const vec3 normal = vec3(0.0, 0.0, 1.0);\n" 
            + "const vec3 tangent = vec3(1.0, 0.0, 0.0);\n"

            + "vec3[LIGHT_COUNT] unpackLightPositions()\n"
            + "{\n"
            + "return vec3[LIGHT_COUNT](\n"
            + "vec3(u_inverseWorldViewMatrix * vec4(u_lightPositions[0][0], u_lightPositions[1][0], LIGHT_DISTANCE, 1.0)),\n"
            + "vec3(u_inverseWorldViewMatrix * vec4(u_lightPositions[3][0], u_lightPositions[0][1], LIGHT_DISTANCE, 1.0)),\n"
            + "vec3(u_inverseWorldViewMatrix * vec4(u_lightPositions[1][1], u_lightPositions[3][1], LIGHT_DISTANCE, 1.0)),\n"
            + "vec3(u_inverseWorldViewMatrix * vec4(u_lightPositions[0][3], u_lightPositions[1][3], LIGHT_DISTANCE, 1.0)));\n"
            + "}\n"

            + "void main()\n"
            + "{\n"
            + "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
            + "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"
            + "gl_FrontColor = gl_Color;\n"

            + "vec3 n = normalize(gl_NormalMatrix * normal);\n"
            + "vec3 t = normalize(gl_NormalMatrix * tangent);\n"
            + "vec3 b = cross(n, t);\n"
            + "vec3 viewVertex = vec3(gl_ModelViewMatrix * gl_Vertex);\n"

            + "vec3[LIGHT_COUNT] pointPositions = unpackLightPositions();\n"
            + "for(int i = 0; i < LIGHT_COUNT; i++)\n"
            + "{\n"
            + "vec3 viewPointLightDir = vec3(gl_ModelViewMatrix * vec4(pointPositions[i], 1.0)) - viewVertex;\n"
            + "v_pointLightDirections[i].x = dot(viewPointLightDir, t);\n"
            + "v_pointLightDirections[i].y = dot(viewPointLightDir, b);\n"
            + "v_pointLightDirections[i].z = dot(viewPointLightDir, n);\n"
            + "}\n"

            + "}";

        private string lightShaderFrag = 
            "uniform vec4 u_ambientColour = vec4(0.1, 0.1, 0.1, 1.0);\n"
            + "uniform vec4 u_lightColour = vec4(1.0, 0.0, 0.0, 1.0);\n"
            + "uniform sampler2D u_texture;\n"
            + "uniform vec4 u_pointColour0;\n"
            + "uniform vec4 u_pointColour1;\n"
            + "uniform vec4 u_pointColour2;\n"
            + "uniform vec4 u_pointColour3;\n"
            
            + "#define LIGHT_COUNT 4\n"

            + "varying vec3 v_pointLightDirections[LIGHT_COUNT];\n"

            + "vec3[LIGHT_COUNT] pointColours = vec3[LIGHT_COUNT](u_pointColour0.rgb, u_pointColour1.rgb, u_pointColour2.rgb, u_pointColour3.rgb);\n"
            + "float[LIGHT_COUNT] pointRanges = float[LIGHT_COUNT](0.00145, 0.00145, 0.00145, 0.00145);\n"

            + "const vec3 normal = vec3(0.0, 0.0, 1.0);"
            + "const vec3 lightDir = normalize(vec3(20.0, -40.0, 30.0));"

            + "vec4 diffuseColour;\n"
            + "vec3 calcLighting(vec3 normal, vec3 lightDir, vec3 lightColour, float falloff)\n"
            + "{\n"
            + "float diffuseAmount = max(dot(normal, lightDir), 0.0);\n"
            + "return lightColour * diffuseColour.rgb * diffuseAmount * falloff\n;"
            + "}\n"

            + "void main()\n"
            + "{\n"
            + "#if defined(TEXTURE)\n"
            + "diffuseColour = texture2D(u_texture, gl_TexCoord[0].xy);\n"
            + "#endif\n"
            + "#if defined(COLOUR)\n"
            + "diffuseColour = gl_Color;\n"
            + "#endif\n"
            + "vec3 ambientColour = diffuseColour.rgb * u_ambientColour.rgb;\n"

            + "for(int i = 0; i < LIGHT_COUNT; i++)\n"
            + "{\n"
            + "vec3 pointLightDir = v_pointLightDirections[i] * pointRanges[i];\n"
            + "float falloff = clamp(1.0 - dot(pointLightDir, pointLightDir), 0.0, 1.0);\n"
            + "ambientColour += calcLighting(normal, normalize(v_pointLightDirections[i]), pointColours[i], falloff);\n"
            + "}\n"

            + "ambientColour += calcLighting(normal, lightDir, u_lightColour.rgb, 1.0);\n"
            + "gl_FragColor = vec4(ambientColour, diffuseColour.a);\n"
            + "}";


        private SFML.Graphics.VertexArray m_grid = new SFML.Graphics.VertexArray(SFML.Graphics.PrimitiveType.Lines);
        private SFML.Graphics.Shader m_lightShaderTextured = null;
        private SFML.Graphics.Shader m_lightShaderColoured = null;
        private SFML.Graphics.Color m_sunColour;
        private SFML.Graphics.Color m_ambientColour;

        private void InitPreview()
        {
            m_sfmlControl.Dock = DockStyle.Fill;
            m_sfmlControl.DrawDelegates.Add(this.DrawPreviewNoLighting);
            m_sfmlControl.UpdateDelegates.Add(this.UpdateSprites);
            panelEditorOuter.Controls.Add(m_sfmlControl);

            //create layers to hold sprites
            for(var i = 0; i < Enum.GetValues(typeof(Layer)).Length; ++i)
            {
                m_previewLayers.Add(new List<SFML.Graphics.RectangleShape>());
            }

            byte[] va = Encoding.UTF8.GetBytes(lightShaderVert);
            MemoryStream vs = new MemoryStream(va);
            byte[] fa = Encoding.UTF8.GetBytes("#version 130\n#define TEXTURE\n" + lightShaderFrag);
            MemoryStream fs = new MemoryStream(fa);
            m_lightShaderTextured = new SFML.Graphics.Shader(vs, fs);

            fa = Encoding.UTF8.GetBytes("#version 130\n#define COLOUR\n" + lightShaderFrag);
            fs = new MemoryStream(fa);
            m_lightShaderColoured = new SFML.Graphics.Shader(vs, fs);

            var colour = panelAmbientColour.BackColor;
            m_ambientColour = new SFML.Graphics.Color(colour.R, colour.G, colour.B);
            colour = panelSunColour.BackColor;
            m_sunColour = new SFML.Graphics.Color(colour.R, colour.G, colour.B);

            m_sfmlControl.MouseDown += m_sfmlControl_MouseDown;
            m_sfmlControl.MouseUp += m_sfmlControl_MouseUp;
            m_sfmlControl.MouseMove += m_sfmlControl_MouseMove;
            m_sfmlControl.MouseClick += m_sfmlControl_MouseClick;
        }


        //event handlers to forward mouse events
        private Panel m_lastClickedPanel;
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
            else if(m_lastClickedPanel != null)
            {
                mouseUp(m_lastClickedPanel, e);
            }

        }
        void m_sfmlControl_MouseDown(object sender, MouseEventArgs e)
        {
            //if (e.Button == System.Windows.Forms.MouseButtons.Left)
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

                        m_lastClickedPanel = p;
                    }
                }
            }
        }
        void m_sfmlControl_MouseMove(object sender, MouseEventArgs e)
        {
            if(m_lastClickedPanel != null)
            {
                var d = ((NodeData)m_lastClickedPanel.Tag).drawable;
                var mousePos = (m_sfmlControl.GetMouseWorldPosition() - d.Position) / scale;
                MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                mouseMove(m_lastClickedPanel, evt);
            }
        }
        void m_sfmlControl_MouseClick(object sender, MouseEventArgs e)
        {
            var d = GetDrawableAtMouse();
            var mousePos = m_sfmlControl.GetMouseWorldPosition() / scale;
            if (d == null)
            {               
                MouseEventArgs evt = new MouseEventArgs(e.Button, e.Clicks, (int)mousePos.X, (int)mousePos.Y, e.Delta);
                panelEditorInner_Click(panelEditorInner, evt);
            }
            else
            {
                if(e.Button == System.Windows.Forms.MouseButtons.Right)
                {
                    var p = GetParentPanel(d);
                    if(p != null && p.ContextMenuStrip != null)
                    {
                        p.ContextMenuStrip.Show(this, this.PointToClient(Cursor.Position)); 
                    }
                }
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
        private void DrawPreviewLighting(SFML.Graphics.RenderWindow rw)
        {
            //pack data in matrix as SFML doesn't support array uniforms
            SFML.Window.Vector2f[] positions = new SFML.Window.Vector2f[m_maxLights];
            for (var i = 0; i < m_lights.Count; ++i)
                positions[i] = m_lights[i].Position;

            SFML.Graphics.Transform t = new SFML.Graphics.Transform(
                positions[0].X, positions[0].Y,
                positions[1].X, positions[1].Y,
                positions[2].X, positions[2].Y,
                positions[3].X, positions[3].Y, 0f);

            m_lightShaderTextured.SetParameter("u_lightColour", m_sunColour);
            m_lightShaderTextured.SetParameter("u_ambientColour", m_ambientColour);
            m_lightShaderTextured.SetParameter("u_lightPositions", t);

            m_lightShaderColoured.SetParameter("u_lightColour", m_sunColour);
            m_lightShaderColoured.SetParameter("u_ambientColour", m_ambientColour);
            m_lightShaderColoured.SetParameter("u_lightPositions", t);

            for(var i = 0; i < m_lights.Count; ++i)
            {
                string param = "u_pointColour" + i.ToString();
                m_lightShaderTextured.SetParameter(param, m_lights[i].Colour);
                m_lightShaderColoured.SetParameter(param, m_lights[i].Colour);
            }

            SFML.Graphics.RenderStates states = SFML.Graphics.RenderStates.Default;

            foreach(var layer in m_previewLayers)
            {
                
                //if (layer != m_previewLayers[(int)Layer.Dynamic])
                //{
                //    states.Shader = m_lightShaderTextured;
                //}
                
                foreach(var d in layer)
                {
                    if (d.Texture == null)
                    {
                        m_lightShaderColoured.SetParameter("u_inverseWorldViewMatrix", d.InverseTransform);
                        states.Shader = m_lightShaderColoured;
                    }
                    else
                    {           
                        m_lightShaderTextured.SetParameter("u_texture", SFML.Graphics.Shader.CurrentTexture);
                        m_lightShaderTextured.SetParameter("u_inverseWorldViewMatrix", d.InverseTransform);
                        states.Shader = m_lightShaderTextured;
                    }
                    rw.Draw(d, states);
                }

                if(layer == m_previewLayers[(int)Layer.Background]
                    && checkBoxSnap.Checked)
                {
                    rw.Draw(m_grid);
                }
            }           
        }
        private void DrawPreviewNoLighting(SFML.Graphics.RenderWindow rw)
        {
            foreach(var layer in m_previewLayers)
            {
                foreach(var d in layer)
                {
                    rw.Draw(d);

                }
                if (layer == m_previewLayers[(int)Layer.Background]
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
            //go backwards so we detect front-most drawable
            var mousePos = m_sfmlControl.GetMouseWorldPosition();
            for (var i = m_previewLayers.Count - 1; i > 0; --i) //skip background layer
            {
                foreach (var d in m_previewLayers[i].Reverse<SFML.Graphics.RectangleShape>())
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
