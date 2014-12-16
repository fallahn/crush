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
        private string m_mapPath;
        private bool m_modified = false;
        private const string fileFilter = "Map Files (.crm)|*.crm";

        private Panel m_playerOnePanel = null;
        private Panel m_playerTwoPanel = null;
        private Panel m_selectedNode = null;

        private Size blockSize = new Size(100, 70);
        private Size itemSize = new Size(40, 60);
        private const int scale = 2;// ui is half the size of the actual game world
        private Color playerOneColour = Color.DodgerBlue;
        private Color playerTwoColour = Color.Gold;
        private Color blockColour = Color.Firebrick;
        private Color solidColour = Color.IndianRed;
        private Color bonusColour = Color.Yellow;
        private Color lifeColour = Color.ForestGreen;
        private Color waterColour = Color.Aqua;

        ContextMenuStrip m_nodeMenu = new ContextMenuStrip();

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
                    if (m_mapPath != null)
                        saveFile();
                    else
                        saveAsToolStripMenuItem_Click(sender, EventArgs.Empty);
                }
            }
        }
        private void MainWindow_Load(object sender, EventArgs e)
        {     
            toolTips.SetToolTip(numericUpDownNpcCount, "Minimum number of NPCs on screen");
            toolTips.SetToolTip(numericUpDownNpcTotal, "Total number of NPCs for this map");
            toolTips.SetToolTip(buttonNpcTexture, "Select the texture to use for this map's NPCs");

            ToolStripMenuItem cloneItem = new ToolStripMenuItem("Clone");
            cloneItem.MouseDown += cloneItem_MouseDown;
            m_nodeMenu.Items.Add(cloneItem);

            ToolStripMenuItem deleteItem = new ToolStripMenuItem("Delete");
            deleteItem.MouseDown += buttonDeleteNode_Click;
            m_nodeMenu.Items.Add(deleteItem);

            //bind node types to comboboxes
            bindComboboxValues(comboBoxAddNode);
            bindComboboxValues(comboBoxNodePropertyType);

            newFile();
        }
        private void bindComboboxValues(ComboBox cb)
        {
            var data = Enum.GetValues(typeof(Node.BodyType))
                            .Cast<Enum>()
                            .Select(value => new
                            {
                                (Attribute.GetCustomAttribute(value.GetType().GetField(value.ToString()), typeof(DescriptionAttribute)) as DescriptionAttribute).Description,
                                value
                            }).OrderBy(item => item.value).ToList();

            //remove player one and two
            var result = data.Find(r => r.Description == "Player One");
            data.Remove(result);
            result = data.Find(r => r.Description == "Player Two");
            data.Remove(result);

            cb.DataSource = data;
            cb.DisplayMember = "Description";
            cb.ValueMember = "value";
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
            if (m_modified)
            {
                if (MessageBox.Show("Save changes to current map?", "Confirm", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    saveFile();
                }
            }
            
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = fileFilter;

            if(fd.ShowDialog() == DialogResult.OK)
            {
                openFile(fd.FileName);
                m_mapPath = fd.FileName;
            }
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_mapPath == string.Empty || m_mapPath == null)
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
                m_mapPath = sd.FileName;
                saveFileAs(sd.FileName); 
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
            m_playerOnePanel.Left = (int)numericUpDownPlayerOneX.Value / scale;
            m_modified = true;
        }
        private void numericUpDownPlayerOneY_ValueChanged(object sender, EventArgs e)
        {
            m_playerOnePanel.Top = (int)numericUpDownPlayerOneY.Value / scale;
            m_modified = true;
        }
        private void numericUpDownPlayerTwoX_ValueChanged(object sender, EventArgs e)
        {
            m_playerTwoPanel.Left = (int)numericUpDownPlayerTwoX.Value / scale;
            m_modified = true;
        }
        private void numericUpDownPlayerTwoY_ValueChanged(object sender, EventArgs e)
        {
            m_playerTwoPanel.Top = (int)numericUpDownPlayerTwoY.Value / scale;
            m_modified = true;
        }

        //node properties
        private void numericUpDownNodePropertyPosX_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Left = (int)numericUpDownNodePropertyPosX.Value / scale;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertyPosY_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Top = (int)numericUpDownNodePropertyPosY.Value / scale;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertySizeX_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Width = (int)numericUpDownNodePropertySizeX.Value / scale;
                m_modified = true;
            }
        }
        private void numericUpDownNodePropertySizeY_ValueChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                m_selectedNode.Height = (int)numericUpDownNodePropertySizeY.Value / scale;
                m_modified = true;
            }
        }
        private void comboBoxNodePropertyType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_selectedNode != null)
            {
                Node.BodyType type = (Node.BodyType)comboBoxNodePropertyType.SelectedValue;
                m_selectedNode.Tag = type;
                switch(type)
                {
                    case Node.BodyType.Block:
                        m_selectedNode.BackColor = blockColour;
                        m_selectedNode.Width = blockSize.Width / scale;
                        m_selectedNode.Height = blockSize.Height / scale;
                        break;
                    case Node.BodyType.Bonus:
                        m_selectedNode.BackColor = bonusColour;
                        m_selectedNode.Width = itemSize.Width / scale;
                        m_selectedNode.Height = itemSize.Height / scale;
                        break;
                    case Node.BodyType.ExtraLife:
                        m_selectedNode.BackColor = lifeColour;
                        m_selectedNode.Width = itemSize.Width / scale;
                        m_selectedNode.Height = itemSize.Height / scale;
                        break;
                    case Node.BodyType.Solid:
                        m_selectedNode.BackColor = solidColour;
                        break;
                    case Node.BodyType.Water:
                        m_selectedNode.BackColor = waterColour;
                        break;
                    default: break;
                }
                
                if(type == Node.BodyType.Solid
                    || type == Node.BodyType.Water)
                {
                    numericUpDownNodePropertySizeX.Enabled = true;
                    numericUpDownNodePropertySizeY.Enabled = true;
                }
                else
                {
                    numericUpDownNodePropertySizeX.Enabled = false;
                    numericUpDownNodePropertySizeY.Enabled = false;
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
            Size size = blockSize;
            //if(comboBoxAddNode.Text == "Block")
            //{
            //    type = Node.BodyType.Block;
            //}
            //else
            switch(comboBoxAddNode.Text)   
            {
                case "Solid":
                    type = Node.BodyType.Solid;
                    break;
                case "Extra Life":            
                    type = Node.BodyType.ExtraLife;
                    size = itemSize;
                    break;
                case "Bonus":            
                    type = Node.BodyType.Bonus;
                    size = itemSize;
                    break;
                case "Water":
                    type = Node.BodyType.Water;
                    break;
                default: break;
            }

            selectNode(addNode(type, new Point(960, 540), size));
        }

        //player panel movement
        void p1_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownPlayerOneX.ValueChanged -= numericUpDownPlayerOneX_ValueChanged;
            numericUpDownPlayerOneY.ValueChanged -= numericUpDownPlayerOneY_ValueChanged;
            numericUpDownPlayerOneX.Value = (decimal)p.Left * scale; //don't forget to scale
            numericUpDownPlayerOneY.Value = (decimal)p.Top * scale;
            numericUpDownPlayerOneX.ValueChanged += numericUpDownPlayerOneX_ValueChanged;
            numericUpDownPlayerOneY.ValueChanged += numericUpDownPlayerOneY_ValueChanged;
            m_modified = true;
        }
        void p2_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownPlayerTwoX.ValueChanged -= numericUpDownPlayerTwoX_ValueChanged;
            numericUpDownPlayerTwoY.ValueChanged -= numericUpDownPlayerTwoY_ValueChanged;
            numericUpDownPlayerTwoX.Value = (decimal)p.Left * scale; //don't forget to scale
            numericUpDownPlayerTwoY.Value = (decimal)p.Top * scale;
            numericUpDownPlayerTwoX.ValueChanged += numericUpDownPlayerTwoX_ValueChanged;
            numericUpDownPlayerTwoY.ValueChanged += numericUpDownPlayerTwoY_ValueChanged;
            m_modified = true;
        }
        void node_Move(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            numericUpDownNodePropertyPosX.ValueChanged -= numericUpDownNodePropertyPosX_ValueChanged;
            numericUpDownNodePropertyPosY.ValueChanged -= numericUpDownNodePropertyPosY_ValueChanged;
            numericUpDownNodePropertyPosX.Value = (decimal)p.Left * scale;
            numericUpDownNodePropertyPosY.Value = (decimal)p.Top * scale;
            numericUpDownNodePropertyPosX.ValueChanged += numericUpDownNodePropertyPosX_ValueChanged;
            numericUpDownNodePropertyPosY.ValueChanged += numericUpDownNodePropertyPosY_ValueChanged;
            m_modified = true;
        }

        //draw grid
        private void panelEditorInner_Paint(object sender, PaintEventArgs e)
        {
            //draw grid
            if (checkBoxSnap.Checked)
            {
                var step = (int)numericUpDownSnap.Value / scale;
                Pen pen = new Pen(Color.Gray);
                for (int i = 0; i < panelEditorInner.Width; i += step)
                    e.Graphics.DrawLine(pen, i, 0, i, panelEditorInner.Height);

                for (int i = 0; i < panelEditorInner.Height; i += step)
                    e.Graphics.DrawLine(pen, 0, i, panelEditorInner.Width, i);
            }
        }
        private void checkBoxSnap_CheckedChanged(object sender, EventArgs e)
        {
            panelEditorInner.Invalidate();
            setIncrements((checkBoxSnap.Checked) ? numericUpDownSnap.Value : 1);
        }
        private void numericUpDownSnap_ValueChanged(object sender, EventArgs e)
        {
            panelEditorInner.Invalidate();
            setIncrements((checkBoxSnap.Checked) ? numericUpDownSnap.Value : 1);
        }
        private void setIncrements(decimal value)
        {
            numericUpDownPlayerOneX.Increment = value;
            numericUpDownPlayerOneY.Increment = value;

            numericUpDownPlayerTwoX.Increment = value;
            numericUpDownPlayerTwoY.Increment = value;

            numericUpDownNodePropertyPosX.Increment = value;
            numericUpDownNodePropertyPosY.Increment = value;

            numericUpDownNodePropertySizeX.Increment = value;
            numericUpDownNodePropertySizeY.Increment = value;
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

            var p = (Panel)sender;
            var t = (Node.BodyType)p.Tag;
            if (t != Node.BodyType.PlayerOne && t != Node.BodyType.PlayerTwo)
            {
                selectNode(p);
            }
        }
        private void mouseUp(Object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mousePressed = false;
                
                //grid snap
                var p = (Panel)sender;
                if (checkBoxSnap.Checked)
                {
                    var step = (int)numericUpDownSnap.Value / scale;
                    p.Left = (int)Math.Round((double)p.Left / step) * step;
                    p.Top = (int)Math.Round((double)p.Top / step) * step;  
                }

                //clamp to bounds
                if (p.Left < 0) p.Left = 0;
                if (p.Top < 0) p.Top = 0;

                if (p.Left > panelEditorInner.Width) p.Left = panelEditorInner.Width - 20;
                if (p.Top > panelEditorInner.Height) p.Top = panelEditorInner.Height - 20;
            }
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
        private void panelEditorInner_Click(object sender, EventArgs e)
        {
            //deselect panel if clicking on background
            if(m_selectedNode != null)
            {
                m_selectedNode.BorderStyle = BorderStyle.None;
                m_selectedNode = null;

                numericUpDownNodePropertyPosX.Enabled = false;
                numericUpDownNodePropertyPosY.Enabled = false;
                numericUpDownNodePropertySizeX.Enabled = false;
                numericUpDownNodePropertySizeY.Enabled = false;

                comboBoxNodePropertyType.Enabled = false;
            }
        }

        //node context menu
        void cloneItem_MouseDown(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)
            {
                addNode((Node.BodyType)m_selectedNode.Tag,
                    new Point(960, 540),
                    new Size(m_selectedNode.Width * scale, m_selectedNode.Height * scale));
            }
        }

    }
}
