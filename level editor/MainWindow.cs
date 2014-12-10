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
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Level_editor
{   
    public partial class MainWindow : Form
    {
        private Map m_currentMap = new Map();
        private bool m_modified = false;
        private const string fileFilter = "Map Files (.crm)|*.crm";

        private Panel m_playerOnePanel = null;
        private Panel m_playerTwoPanel = null;
        private Panel m_selectedNode = null;

        private Size blockSize = new Size(100, 70);

        public MainWindow()
        {
            InitializeComponent();
            WindowState = FormWindowState.Maximized;
        }


        //--------------event handlers-----------------//
        //form--------
        private void splitContainer2_Panel1_Resize(object sender, EventArgs e)
        {
            centreEditor();
        }
        private void MainWindow_Resize(object sender, EventArgs e)
        {
            centreEditor();
        }
        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(m_modified)
            {
                if(MessageBox.Show("Do you wish to save changes?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    //save changes
                    saveFile();
                }
            }
        }
        private void MainWindow_Load(object sender, EventArgs e)
        {
            newFile();

            toolTips.SetToolTip(numericUpDownNpcCount, "Minimum number of NPCs on screen");
            toolTips.SetToolTip(numericUpDownNpcTotal, "Total number of NPCs for this map");
            toolTips.SetToolTip(buttonNpcTexture, "Select the texture to use for this map's NPCs");
        }

        //toolstrip---
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(m_modified)
            {
                if(MessageBox.Show("Save changes to current map?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    saveFile();
                }
            }
            newFile();
        }               
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = fileFilter;

            if(fd.ShowDialog() == DialogResult.OK)
            {
                openFile(fd.FileName);
                m_currentMap.MapName = fd.FileName;
            }
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_currentMap.MapName == string.Empty)
                saveAsToolStripMenuItem_Click(sender, e);
            else
                saveFile();
        }
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sd = new SaveFileDialog();
            sd.Filter = fileFilter;

            if(sd.ShowDialog() == DialogResult.OK)
            {
                saveFileAs(sd.FileName);
                m_currentMap.MapName = sd.FileName;
            }
        }
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        //map properties
        private void numericUpDownNpcCount_ValueChanged(object sender, EventArgs e)
        {
            numericUpDownNpcTotal.Minimum = numericUpDownNpcCount.Value;
            m_currentMap.NpcCount = (int)numericUpDownNpcCount.Value;
            m_modified = true;
        }
        private void numericUpDownNpcTotal_ValueChanged(object sender, EventArgs e)
        {
            m_currentMap.NpcTotal = (int)numericUpDownNpcTotal.Value;
            m_modified = true;
        }
        private void numericUpDownPlayerOneX_ValueChanged(object sender, EventArgs e)
        {
            m_playerOnePanel.Left = (int)numericUpDownPlayerOneX.Value / 2;
            m_modified = true;
        }
        private void numericUpDownPlayerOneY_ValueChanged(object sender, EventArgs e)
        {
            m_playerOnePanel.Top = (int)numericUpDownPlayerOneY.Value / 2;
            m_modified = true;
        }
        private void numericUpDownPlayerTwoX_ValueChanged(object sender, EventArgs e)
        {
            m_playerTwoPanel.Left = (int)numericUpDownPlayerTwoX.Value / 2;
            m_modified = true;
        }
        private void numericUpDownPlayerTwoY_ValueChanged(object sender, EventArgs e)
        {
            m_playerTwoPanel.Top = (int)numericUpDownPlayerTwoY.Value / 2;
            m_modified = true;
        }

        //node properties
        private void numericUpDownNodePropertyPosX_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Left = (int)numericUpDownNodePropertyPosX.Value / 2;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertyPosY_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Top = (int)numericUpDownNodePropertyPosY.Value / 2;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertySizeX_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Width = (int)numericUpDownNodePropertySizeX.Value / 2;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertySizeY_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Height = (int)numericUpDownNodePropertySizeY.Value / 2;
                m_modified = true;
            }
        }
        private void comboBoxNodePropertyType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_selectedNode != null)
            {
                var text = comboBoxNodePropertyType.Text;
                switch (text)
                {
                    case "Block":
                        //restore size to block size
                        m_selectedNode.Width = blockSize.Width / 2;
                        m_selectedNode.Height = blockSize.Height / 2;

                        m_selectedNode.Tag = Node.BodyType.Block;
                        m_selectedNode.BackColor = Color.Firebrick;

                        numericUpDownNodePropertySizeX.Enabled = false;
                        numericUpDownNodePropertySizeY.Enabled = false;
                        break;
                    case "Solid":
                        m_selectedNode.Tag = Node.BodyType.Solid;
                        m_selectedNode.BackColor = Color.Maroon;

                        numericUpDownNodePropertySizeX.Enabled = true;
                        numericUpDownNodePropertySizeY.Enabled = true;
                        break;
                    default: break;
                }
            }
        }
        private void buttonDeleteNode_Click(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                numericUpDownNodePropertyPosX.Enabled = false;
                numericUpDownNodePropertyPosY.Enabled = false;
                numericUpDownNodePropertySizeX.Enabled = false;
                numericUpDownNodePropertySizeY.Enabled = false;
                panelEditorInner.Controls.Remove(m_selectedNode);
                m_selectedNode = null;
            }
        }

        //node addition
        private void buttonAddNode_Click(object sender, EventArgs e)
        {
            Node.BodyType type = Node.BodyType.Block;
            if(comboBoxAddNode.Text == "Block")
            {
                type = Node.BodyType.Block;
            }
            else if(comboBoxAddNode.Text == "Solid")
            {
                type = Node.BodyType.Solid;
            }

            selectNode(addNode(type, new PointF(960f, 540f), new SizeF(100f, 70f)));
        }

        //player panel movement
        void p1_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownPlayerOneX.ValueChanged -= numericUpDownPlayerOneX_ValueChanged;
            numericUpDownPlayerOneY.ValueChanged -= numericUpDownPlayerOneY_ValueChanged;
            numericUpDownPlayerOneX.Value = (decimal)p.Left * 2; //don't forget to scale
            numericUpDownPlayerOneY.Value = (decimal)p.Top * 2;
            numericUpDownPlayerOneX.ValueChanged += numericUpDownPlayerOneX_ValueChanged;
            numericUpDownPlayerOneY.ValueChanged += numericUpDownPlayerOneY_ValueChanged;
            m_modified = true;
        }
        void p2_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownPlayerTwoX.ValueChanged -= numericUpDownPlayerTwoX_ValueChanged;
            numericUpDownPlayerTwoY.ValueChanged -= numericUpDownPlayerTwoY_ValueChanged;
            numericUpDownPlayerTwoX.Value = (decimal)p.Left * 2; //don't forget to scale
            numericUpDownPlayerTwoY.Value = (decimal)p.Top * 2;
            numericUpDownPlayerTwoX.ValueChanged += numericUpDownPlayerTwoX_ValueChanged;
            numericUpDownPlayerTwoY.ValueChanged += numericUpDownPlayerTwoY_ValueChanged;
            m_modified = true;
        }
        void node_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownNodePropertyPosX.ValueChanged -= numericUpDownNodePropertyPosX_ValueChanged;
            numericUpDownNodePropertyPosY.ValueChanged -= numericUpDownNodePropertyPosY_ValueChanged;
            numericUpDownNodePropertyPosX.Value = (decimal)p.Left * 2;
            numericUpDownNodePropertyPosY.Value = (decimal)p.Top * 2;
            numericUpDownNodePropertyPosX.ValueChanged += numericUpDownNodePropertyPosX_ValueChanged;
            numericUpDownNodePropertyPosY.ValueChanged += numericUpDownNodePropertyPosY_ValueChanged;
            m_modified = true;
        }


        //drag events for panels
        private bool mousePressed = false;
        private int lastMouseX, lastMouseY;
        private void mouseDown(Object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mousePressed = true;
                lastMouseX = e.X;
                lastMouseY = e.Y;
            }
        }
        private void mouseUp(Object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                mousePressed = false;
        }
        private void mouseMove(Object sender, MouseEventArgs e)
        {
            if(mousePressed)
            {
                var p = (Panel)sender;
                p.Left += e.X - lastMouseX;
                p.Top += e.Y - lastMouseY;
                m_modified = true;
            }
        }

        //selecting nodes
        private void mouseClick(Object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                var p = (Panel)sender;
                selectNode(p);
            }
        }

    }
}
