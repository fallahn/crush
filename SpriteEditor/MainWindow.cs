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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace SpriteEditor
{
    public partial class MainWindow : Form
    {
        private SfmlControl m_sfmlControl = new SfmlControl();

        public MainWindow()
        {
            InitializeComponent();

            splitContainer2.Panel1.Controls.Add(m_sfmlControl);
            m_sfmlControl.Dock = DockStyle.Fill;
            m_sfmlControl.UpdateDelegates.Add(this.m_aniSprite.Update);
            m_sfmlControl.DrawDelegates.Add(this.DrawSprite);
        }

        public void DispatchDrawingEvents()
        {
            m_sfmlControl.HandleEvents();
            m_sfmlControl.Draw();
        }


        //----event handlers----//
        private void openImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog od = new OpenFileDialog();
            od.Filter = "PNG Files|*.png|JPG Files|*.jpg";

            if(od.ShowDialog() == DialogResult.OK)
            {
                SFML.Graphics.Texture tex = new SFML.Graphics.Texture(od.FileName);
                if (tex.Size.X <= 2048 && tex.Size.Y <= 2048)
                {
                    m_aniSprite = new AnimatedSprite(tex);

                    ResetControls();
                }
                else
                {
                    MessageBox.Show("Maximum texture size is 2048 x 2048", "Texture Too Big");
                }
            }
        }

        private void numericUpDownFrameWidth_ValueChanged(object sender, EventArgs e)
        {
            m_aniSprite.FrameSize = new SFML.Window.Vector2i((int)numericUpDownFrameWidth.Value, (int)numericUpDownFrameHeight.Value);
        }

        private void numericUpDownFrameHeight_ValueChanged(object sender, EventArgs e)
        {
            m_aniSprite.FrameSize = new SFML.Window.Vector2i((int)numericUpDownFrameWidth.Value, (int)numericUpDownFrameHeight.Value);
        }
        
        private void numericUpDownFrameCount_ValueChanged(object sender, EventArgs e)
        {
            m_aniSprite.FrameCount = (uint)numericUpDownFrameCount.Value;
        }

        private void numericUpDownFrameRate_ValueChanged(object sender, EventArgs e)
        {
            m_aniSprite.FrameRate = (float)numericUpDownFrameRate.Value;
        }

        private void buttonPlayPause_Click(object sender, EventArgs e)
        {
            if (m_aniSprite.Playing)
            {
                m_aniSprite.SetPaused(true);
                buttonPlayPause.Text = "Play";
            }
            else
            {
                if (listBoxAnimations.Items.Count == 0)
                {
                    m_aniSprite.Loop = true;
                    m_aniSprite.Play(0, m_aniSprite.FrameCount - 1);
                }
                else
                {

                }
                buttonPlayPause.Text = "Pause";
            }
        }

        //----misc----//
        private void ResetControls()
        {
            numericUpDownAnimEnd.Value = 0;
            numericUpDownAnimStart.Value = 0;
            numericUpDownFrameCount.Value = 1;
            numericUpDownFrameHeight.Value = (Decimal)m_aniSprite.Texture.Size.Y;
            numericUpDownFrameRate.Value = 12;
            numericUpDownFrameWidth.Value = (Decimal)m_aniSprite.Texture.Size.X;
            
        }






    }
}
