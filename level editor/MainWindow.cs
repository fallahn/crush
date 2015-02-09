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
using System.IO;
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

        private Size blockSize = new Size(66, 66);
        private Size solidSize = new Size(60, 60);
        private Size lightSize = new Size(32, 32);
        private Size playerSize = new Size(41, 64);
        private Size itemSize = new Size(64, 64);
        private const int scale = 2;// ui is half the size of the actual game world
        private Color playerOneColour = Color.DodgerBlue;
        private Color playerTwoColour = Color.Gold;
        private Color blockColour = Color.Firebrick;
        private Color solidColour = Color.IndianRed;
        private Color bonusColour = Color.Yellow;
        private Color waterColour = Color.Aqua;

        private Color sunColour = Color.FromArgb(255, 255, 252, 230);
        private Color ambientColour = Color.FromArgb(255, 64, 64, 64);

        ContextMenuStrip m_nodeMenu = new ContextMenuStrip();

        private string m_textureDirectory;
        public string TextureDirectory
        {
            set { m_textureDirectory = value; }
            get { return m_textureDirectory; }
        }

        private string m_mapTextureDirectory;
        public string MapTextureDirectory
        {
            get { return m_mapTextureDirectory; }
            set { m_mapTextureDirectory = value; }
        }
        private string m_atlasTextureDirectory;
        public string AtlasTextureDirectory
        {
            get { return m_atlasTextureDirectory; }
            set { m_atlasTextureDirectory = value; }
        }

        private string m_backgroundFileName;
        private string m_platformFileName;

        private Image m_platformTexture;

        public MainWindow()
        {
            InitializeComponent();
            //WindowState = FormWindowState.Maximized;

            //generate default data for selected sprite so it won't get all
            //borked when there are no sprite sheets to load / missing
            m_selectedFrame.largeImage = defaultImage();
            m_selectedFrame.smallImage = defaultImage();

            m_textureDirectory = RegKey.read("texture_directory");
            if(m_textureDirectory == null || !Directory.Exists(m_textureDirectory))
            {
                if(MessageBox.Show("No valid texture path found. Click OK to browse\n or Cancel to continue (textures will be unavailable)",
                    "Warning", MessageBoxButtons.OKCancel) == DialogResult.OK)
                {
                    OptionsWindow ow = new OptionsWindow(this);
                    ow.ShowDialog();
                }
            }
            else
            {
                m_atlasTextureDirectory = m_textureDirectory + "\\atlases";
                loadTextures();
            }

            m_mapTextureDirectory = m_textureDirectory + "\\map";

            InitPreview();
        }
        public void DispatchDrawingEvents()
        {
            m_sfmlControl.HandleEvents();
            m_sfmlControl.Draw();
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
            toolTips.SetToolTip(buttonBackgroundImage, "Select the texture to use for this map's background");            

            ToolStripMenuItem cloneItem = new ToolStripMenuItem("Clone");
            cloneItem.MouseDown += cloneItem_MouseDown;
            m_nodeMenu.Items.Add(cloneItem);

            ToolStripMenuItem deleteItem = new ToolStripMenuItem("Delete");
            deleteItem.MouseDown += buttonDeleteNode_Click;
            m_nodeMenu.Items.Add(deleteItem);

            ToolStripMenuItem propertiesItem = new ToolStripMenuItem("Properties");
            propertiesItem.Click += propertiesItem_Click;
            m_nodeMenu.Items.Add(propertiesItem);

            //bind node types to comboboxes
            bindComboboxValues(comboBoxAddNode);
            bindComboboxValues(comboBoxNodePropertyType);

            //colour picker handlers
            panelAmbientColour.Click += pickColour;
            panelSunColour.Click += pickColour;

            //enable numeric box snapping
            checkBoxSnap_CheckedChanged(null, EventArgs.Empty);

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
                    if (m_mapPath == string.Empty || m_mapPath == null)
                        saveAsToolStripMenuItem_Click(sender, e);
                    else
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
        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OptionsWindow optionsWindow = new OptionsWindow(this);
            if(optionsWindow.ShowDialog() == DialogResult.OK)
            {
                loadTextures();
            }
        }
        private void useSoftwarePreviewToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            if(useSoftwarePreviewToolStripMenuItem.Checked)
            {
                panelEditorInner.Visible = true;
                m_sfmlControl.Visible = false;
            }
            else
            {
                panelEditorInner.Visible = false;
                m_sfmlControl.Visible = true;
            }
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
        private void pickColour(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            ColorDialog cd = new ColorDialog();
            cd.Color = p.BackColor;
            cd.FullOpen = true;
            if(cd.ShowDialog() == DialogResult.OK)
            {
                p.BackColor = cd.Color;
            }
        }
        private void pickLightColour(object sender, EventArgs e)
        {
            var p = (Panel)sender;
            ColorDialog cd = new ColorDialog();
            cd.Color = p.BackColor;
            cd.FullOpen = true;
            if (cd.ShowDialog() == DialogResult.OK)
            {
                p.BackColor = cd.Color;
                if(m_selectedNode != null
                    && ((NodeData)m_selectedNode.Tag).type == Node.BodyType.Light)
                {
                    m_selectedNode.BackColor = cd.Color;
                }
            }
        }
        private void buttonBackgroundImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog od = new OpenFileDialog();
            od.Filter = "JPEG files (Portable Network Graphic (*.png)|*.png|*.jpg)|*.jpg";
            if (m_mapTextureDirectory != null && m_mapTextureDirectory != String.Empty)
                od.InitialDirectory = m_mapTextureDirectory;

            if(od.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Image img = Image.FromFile(od.FileName);
                    panelEditorInner.BackgroundImage = (Image)(new Bitmap(img, new Size(img.Width / scale, img.Height / scale)));

                    m_backgroundFileName = Path.GetFileName(od.FileName);
                    m_modified = true;

                    m_previewLayers[(int)Layer.Background].Clear();
                    var shape = new SFML.Graphics.RectangleShape();
                    shape.Texture = m_textureResource.Get(od.FileName);
                    shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y);
                    m_previewLayers[(int)Layer.Background].Add(shape);
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message, "Failed opening file");
                }
            }
        }
        private void buttonSolidTexture_Click(object sender, EventArgs e)
        {
            OpenFileDialog od = new OpenFileDialog();
            od.Filter = "JPEG files (Portable Network Graphic (*.png)|*.png|*.jpg)|*.jpg";
            if (m_mapTextureDirectory != null && m_mapTextureDirectory != String.Empty)
                od.InitialDirectory = m_mapTextureDirectory;

            if (od.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Image img = Image.FromFile(od.FileName);
                    m_platformTexture = (Image)(new Bitmap(img, new Size(img.Width / scale, img.Height / scale)));

                    //loop through existing nodes and update image
                    foreach(Panel p in panelEditorInner.Controls)
                    {
                        NodeData nd = (NodeData)p.Tag;
                        if(nd.type == Node.BodyType.Solid)
                        {
                            p.BackgroundImage = m_platformTexture;
                        }
                    }

                    m_platformFileName = Path.GetFileName(od.FileName);
                    m_modified = true;

                    //update existinf drawables
                    SFML.Graphics.Texture t = m_textureResource.Get(od.FileName);
                    t.Repeated = true;
                    foreach (var d in m_previewLayers[(int)Layer.Solid])
                    {
                        d.Texture = t;
                        d.FillColor = SFML.Graphics.Color.White;
                        d.TextureRect = new SFML.Graphics.IntRect((int)d.Position.X, (int)d.Position.Y, (int)d.Size.X, (int)d.Size.Y);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Failed opening file");
                }
            }
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
                checkBoxFrontDetail.Enabled = false;
                //checkBoxFrontDetail.Checked = false;

                Node.BodyType type = (Node.BodyType)comboBoxNodePropertyType.SelectedValue;
                var nodeData = (NodeData)m_selectedNode.Tag;
                if (nodeData.type == type) return;

                nodeData.type = type;
                nodeData.spriteSheet = null;
                nodeData.frameName = null;
                
                switch(type)
                {
                    case Node.BodyType.Block:
                        m_selectedNode.BackColor = blockColour;
                        m_selectedNode.Width = blockSize.Width / scale;
                        m_selectedNode.Height = blockSize.Height / scale;
                        m_selectedNode.BackgroundImage = Properties.Resources.block;
                        m_selectedNode.Size = new Size(blockSize.Width / scale, blockSize.Height / scale);
                        nodeData.layer = Layer.Dynamic;
                        break;
                    case Node.BodyType.Item:
                        m_selectedNode.BackColor = bonusColour;
                        m_selectedNode.Width = itemSize.Width / scale;
                        m_selectedNode.Height = itemSize.Height / scale;
                        m_selectedNode.BackgroundImage = Properties.Resources.item;
                        m_selectedNode.Size = new Size(itemSize.Width / scale, itemSize.Height / scale);
                        nodeData.layer = Layer.Dynamic;
                        break;
                    case Node.BodyType.Solid:
                        m_selectedNode.BackColor = solidColour;
                        m_selectedNode.BackgroundImage = null;
                        nodeData.layer = Layer.Solid;
                        break;
                    case Node.BodyType.Water:
                        m_selectedNode.BackColor = waterColour;
                        m_selectedNode.BackgroundImage = null;
                        nodeData.layer = Layer.Water;
                        break;
                    case Node.BodyType.Detail:
                        if (m_spriteSheets.Count > 0)
                        {
                            m_selectedNode.BackColor = Color.Transparent;
                            m_selectedNode.BackgroundImage = m_selectedFrame.smallImage;
                            m_selectedNode.Size = m_selectedFrame.smallImage.Size;
                            nodeData.layer = Layer.RearDetail;
                            nodeData.spriteSheet = m_selectedFrame.parentSheet.meta.image;
                            nodeData.frameName = m_selectedFrame.filename;
                            checkBoxFrontDetail.Enabled = true;
                            break;
                        }
                        else
                        {
                            comboBoxNodePropertyType.SelectedIndex--;
                            return;
                        }
                    case Node.BodyType.Light:
                        m_selectedNode.Width = lightSize.Width / scale;
                        m_selectedNode.Height = lightSize.Height / scale;
                        m_selectedNode.BackgroundImage = Properties.Resources.bulb;
                        nodeData.layer = Layer.FrontDetail;
                        break;
                    default: break;
                }
                m_selectedNode.Tag = nodeData;
                
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

                //sort layers
                sortNodes();
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

                var nd = (NodeData)m_selectedNode.Tag;
                if(nd.type == Node.BodyType.Light)
                {
                    m_lightCount--;
                }

                if(nd.drawable != null)
                    m_previewLayers[(int)nd.layer].Remove(nd.drawable);

                panelEditorInner.Controls.Remove(m_selectedNode);
                m_selectedNode = null;
            }
        }
        private void propertiesItem_Click(object sender, EventArgs e)
        {
            var tag = (NodeData)m_selectedNode.Tag;
            switch (tag.type)
            {
                case Node.BodyType.Detail:
                    BrowseTexturesWindow bw = new BrowseTexturesWindow(this);
                    if(bw.ShowDialog() == DialogResult.OK)
                    {
                        setPanelTexture(ref m_selectedNode);
                    }
                    break;
                case Node.BodyType.Light:
                    if (m_selectedNode != null)
                    {
                        LightPropertiesWindow ld = new LightPropertiesWindow(ref m_selectedNode);
                        ld.ShowDialog();
                    }
                    break;
                default: break;
            }
        }
        private void checkBoxFrontDetail_CheckedChanged(object sender, EventArgs e)
        {
            if(m_selectedNode != null)
            {
                var nd = (NodeData)m_selectedNode.Tag;
                if(nd.type == Node.BodyType.Detail)
                {
                    nd.layer = (checkBoxFrontDetail.Checked) ? Layer.FrontDetail : Layer.RearDetail;
                    m_selectedNode.Tag = nd;
                    sortNodes();

                    if(nd.layer == Layer.RearDetail)
                    {
                        m_previewLayers[(int)Layer.RearDetail].Add(nd.drawable);
                        m_previewLayers[(int)Layer.FrontDetail].Remove(nd.drawable);
                    }
                    else
                    {
                        m_previewLayers[(int)Layer.FrontDetail].Add(nd.drawable);
                        m_previewLayers[(int)Layer.RearDetail].Remove(nd.drawable);
                    }
                }
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
                    size = solidSize;
                    break;
                case "Item":            
                    type = Node.BodyType.Item;
                    size = itemSize;
                    break;
                case "Water":
                    type = Node.BodyType.Water;
                    break;
                case "Light":
                    type = Node.BodyType.Light;
                    size = lightSize;
                    break;
                case "Detail":
                    if(m_spriteSheets.Count > 0)
                    {
                        type = Node.BodyType.Detail;
                        size = m_selectedFrame.largeImage.Size;
                        break;
                    }
                    MessageBox.Show("Details cannot be added when no sprite sheets are loaded");
                    return;
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

            NodeData nd = (NodeData)p.Tag;
            if(nd.drawable != null)
            {
                nd.drawable.Position = new SFML.Window.Vector2f(p.Left * scale, p.Top * scale);
            }
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

            NodeData nd = (NodeData)p.Tag;
            if (nd.drawable != null)
            {
                nd.drawable.Position = new SFML.Window.Vector2f(p.Left * scale, p.Top * scale);
            }
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

            NodeData nd = (NodeData)p.Tag;
            if (nd.drawable != null)
            {
                nd.drawable.Position = new SFML.Window.Vector2f(p.Left * scale, p.Top * scale);
            }
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

            if (checkBoxSnap.Checked)
                BuildGrid(); //builds the sfml grid display
        }
        private void numericUpDownSnap_ValueChanged(object sender, EventArgs e)
        {
            panelEditorInner.Invalidate();
            setIncrements((checkBoxSnap.Checked) ? numericUpDownSnap.Value : 1);

            if (checkBoxSnap.Checked)
                BuildGrid(); //builds the sfml grid display
        }
        private void setIncrements(decimal value)
        {
            numericUpDownPlayerOneX.Increment = value;
            //numericUpDownPlayerOneX.Value = (decimal)Math.Round((double)numericUpDownPlayerOneX.Value / (double)value) * value;
            numericUpDownPlayerOneY.Increment = value;
            //numericUpDownPlayerOneY.Value = (decimal)Math.Round((double)numericUpDownPlayerOneY.Value / (double)value) * value;

            numericUpDownPlayerTwoX.Increment = value;
            //numericUpDownPlayerTwoX.Value = (decimal)Math.Round((double)numericUpDownPlayerTwoX.Value / (double)value) * value;
            numericUpDownPlayerTwoY.Increment = value;
            //numericUpDownPlayerTwoY.Value = (decimal)Math.Round((double)numericUpDownPlayerTwoY.Value / (double)value) * value;

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
                //this.Text = lastMouseX.ToString() + ", " + lastMouseY.ToString();
            }

            var p = (Panel)sender;
            var t = ((NodeData)p.Tag).type;
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
                //clamp to bounds
                if (p.Left < 0) p.Left = 0;
                if (p.Left + p.Width > m_sceneSize.Width) p.Left = m_sceneSize.Width - p.Width;

                p.Top += e.Y - lastMouseY;
                if (p.Top < 0) p.Top = 0;
                if (p.Top + p.Height > m_sceneSize.Height) p.Top = m_sceneSize.Height - p.Height;

                m_modified = true;

                this.Text = e.X.ToString() + ", " + e.Y.ToString() + " - " + lastMouseX.ToString() + ", " + lastMouseY.ToString();
            }
            
        }
        private void panelEditorInner_Click(object sender, EventArgs e)
        {
            //deselect panel if clicking on background
            if(m_selectedNode != null)
            {
                var nd = (NodeData)m_selectedNode.Tag;
                if (nd.drawable != null) nd.drawable.OutlineThickness = 0f;
                m_selectedNode.BorderStyle = BorderStyle.None;
                m_selectedNode = null;

                numericUpDownNodePropertyPosX.Enabled = false;
                numericUpDownNodePropertyPosY.Enabled = false;
                numericUpDownNodePropertySizeX.Enabled = false;
                numericUpDownNodePropertySizeY.Enabled = false;

                comboBoxNodePropertyType.Enabled = false;
                checkBoxFrontDetail.Enabled = false;
            }
        }

        //node context menu
        void cloneItem_MouseDown(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)
            {
                var nd = (NodeData)m_selectedNode.Tag;
                var node = addNode(nd.type,
                    new Point(960, 540),
                    new Size(m_selectedNode.Width * scale, m_selectedNode.Height * scale),
                    nd.layer);
                node.Tag = nd;
                node.BackColor = m_selectedNode.BackColor;
            }
        }
    }
}
