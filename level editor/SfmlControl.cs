/*********************************************************************
Matt Marchant 2015
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
using System.Windows.Forms;

using SFML.Graphics;
using SFML.Window;

namespace Level_editor
{


    partial class SfmlControl : Control
    {        
        public delegate void DrawDelegate(RenderWindow rw);
        public delegate void UpdateDelegate(float dt);   
     
        private RenderWindow m_renderWindow;
        private Color m_clearColour = new Color();
        public Color BackgroundColour
        {
            get { return m_clearColour; }
            set { m_clearColour = value; }
        }

        //delegates for custom drawing
        private List<DrawDelegate> m_drawDelegates = new List<DrawDelegate>();
        public List<DrawDelegate> DrawDelegates
        {
            get { return m_drawDelegates; }
        }

        private List<UpdateDelegate> m_updateDelegates = new List<UpdateDelegate>();
        public List<UpdateDelegate> UpdateDelegates
        {
            get { return m_updateDelegates; }
        }

        public SfmlControl()
        {
            m_renderWindow = new RenderWindow(this.Handle);
            m_renderWindow.SetVerticalSyncEnabled(true);

            //add event handlers
            this.Resize += SfmlControl_Resize;
            SfmlControl_Resize(this, EventArgs.Empty);

            this.MouseWheel += SfmlControl_MouseWheel;
            this.MouseDown += SfmlControl_MouseDown;
            this.MouseUp += SfmlControl_MouseUp;
            this.MouseMove += SfmlControl_MouseMove;
        }





        //event handlers
        Vector2f m_currentViewPosition = new Vector2f(960f, 540f);
        void SfmlControl_Resize(object sender, EventArgs e)
        {
            Vector2f size = new Vector2f(1920f, 1080f);
            SFML.Graphics.View v = new SFML.Graphics.View(m_currentViewPosition, size);
            v.Zoom(m_currentZoom);

            float windowRatio = (float)this.Size.Width / (float)this.Size.Height;
            float viewRatio = 16f / 9f;
            float sizeX = 1f;
            float sizeY = 1f;
            float posX = 0f;
            float posY = 0f;

            bool horizontalSpacing = (windowRatio > viewRatio);

            if (horizontalSpacing)
            {
                sizeX = viewRatio / windowRatio;
                posX = (1f - sizeX) / 2f;
            }
            else
            {
                sizeY = windowRatio / viewRatio;
                posY = (1f - sizeY) / 2f;
            }
            v.Viewport = new FloatRect(posX, posY, sizeX, sizeY);

            m_renderWindow.SetView(v);
        }
        void SfmlControl_MouseWheel(object sender, MouseEventArgs e)
        {
            if (e.Delta > 0) //up
            {
                ZoomIn();
            }
            else if (e.Delta < 0) //down
            {
                ZoomOut();
            }
        }
        private bool m_middleButtonPressed = false;
        private System.Drawing.Point m_lastMousePosition;
        void SfmlControl_MouseUp(object sender, MouseEventArgs e)
        {
            if(e.Button == System.Windows.Forms.MouseButtons.Middle)
            {
                m_middleButtonPressed = false;
            }
        }
        void SfmlControl_MouseDown(object sender, MouseEventArgs e)
        {
            if(e.Button == System.Windows.Forms.MouseButtons.Middle)
            {
                m_middleButtonPressed = true;
                m_lastMousePosition = e.Location;
            }

            this.Focus();
        }        
        void SfmlControl_MouseMove(object sender, MouseEventArgs e)
        {
            if(m_middleButtonPressed)
            {
                SFML.Window.Vector2f movement = new Vector2f(m_lastMousePosition.X - e.Location.X, m_lastMousePosition.Y - e.Location.Y);
                movement *= m_currentZoom;
                var view = m_renderWindow.GetView();
                view.Move(movement);
                m_currentViewPosition = view.Center;
                m_renderWindow.SetView(view);
                m_lastMousePosition = e.Location;
            }
        } 
       



        //public functions        
        private Stopwatch clock = new Stopwatch();
        public void HandleEvents()
        {
            m_renderWindow.DispatchEvents();

            float dt = clock.Elapsed.Milliseconds / 1000f;
            foreach(UpdateDelegate u in m_updateDelegates)
            {
                u(dt);
            }
            clock.Restart();
        }
        public void Draw()
        {
            m_renderWindow.Clear(m_clearColour);
            foreach (DrawDelegate d in m_drawDelegates)
                d(m_renderWindow);
            m_renderWindow.Display();
        }


        //private functions
        private float m_currentZoom = 1f;
        private const float m_zoomAmount = 0.75f;
        private void ZoomIn()
        {
            m_currentZoom *= m_zoomAmount;
            
            var v = m_renderWindow.GetView();
            v.Zoom(m_zoomAmount);
            m_renderWindow.SetView(v);
        }
        private void ZoomOut()
        {
            float zoom = (1f + (1f - m_zoomAmount));
            m_currentZoom *= zoom;

            var v = m_renderWindow.GetView();
            v.Zoom(zoom);
            m_renderWindow.SetView(v);
        }

        //----control overrides-----//
        protected override void OnPaint(PaintEventArgs pe)
        {
            //base.OnPaint(pe);
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            base.OnPaintBackground(pevent);
        }
    }
}
