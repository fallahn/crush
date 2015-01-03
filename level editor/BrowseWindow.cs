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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Level_editor
{
    public partial class BrowseWindow : Form
    {
        private MainWindow m_mainWindow;

        public BrowseWindow(MainWindow mainWindow)
        {
            InitializeComponent();

            m_mainWindow = mainWindow;

            //create a tab page for each sheet and fill with images
            foreach (var sheet in mainWindow.SpriteSheets)
            {
                TabPage tp = new TabPage(sheet.meta.image);
                tp.Size = tabControl1.Size;

                FlowLayoutPanel flp = new FlowLayoutPanel();
                flp.AutoScroll = true;
                flp.FlowDirection = FlowDirection.LeftToRight;
                flp.WrapContents = true;
                flp.Size = tp.Size;

                tp.Controls.Add(flp);
                foreach(Atlas.Frame frame in sheet.frames)
                {
                    PictureBox pb = new PictureBox();
                    pb.SizeMode = PictureBoxSizeMode.AutoSize;
                    pb.Image = frame.largeImage;
                    pb.Click += pb_Click;
                    pb.DoubleClick += pb_DoubleClick;
                    pb.Tag = frame;

                    if(frame == mainWindow.SelectedFrame)
                    {
                        pb.BackColor = Color.DodgerBlue;
                    }

                    flp.Controls.Add(pb);
                }
                tabControl1.TabPages.Add(tp);
            }

            foreach(TabPage tp in tabControl1.TabPages)
            {
                if(tp.Text == mainWindow.SelectedFrame.parentSheet.meta.image)
                {
                    tabControl1.SelectedTab = tp;
                    break;
                }
            }
        }


        //------event handlers-------
        private void pb_Click(object sender, EventArgs e)
        {
            foreach (Control c in tabControl1.SelectedTab.Controls[0].Controls)
            {
                var p = (PictureBox)c;
                p.BackColor = Color.Transparent;
            }
            var pb = (PictureBox)sender;
            pb.BackColor = Color.DodgerBlue;

            m_mainWindow.SelectedFrame = (Atlas.Frame)pb.Tag;
        }
        void pb_DoubleClick(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }
        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }
    }
}
