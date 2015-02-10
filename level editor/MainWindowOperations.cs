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

//non event handler functions for Main Window

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Newtonsoft.Json;

namespace Level_editor
{
    public partial class MainWindow : Form
    {
        private int m_lightCount = 0;
        private const int m_maxLights = 4;

        //prevent initial sorting on loading of a map, until
        //the final node is added
        private bool m_canSort = false;
        
        //------------others---------------//

        private void centreEditor()
        {
            //centre the inner editor area to its parent
            panelEditorInner.Left = (panelEditorOuter.Width - panelEditorInner.Width) / scale;
            panelEditorInner.Top = (panelEditorOuter.Height - panelEditorInner.Height) / scale;
        }

        /// <summary>
        /// adds a node to the UI
        /// </summary>
        /// <param name="type">Type of node</param>
        /// <param name="position">Position of node</param>
        /// <param name="size">Size of node</param>
        private Panel addNode(Node.BodyType type, Point position, Size size, Layer layer = Layer.None)
        {
            //divide by two as UI is half world game world size
            Panel p = new Panel();
            p.Left = position.X / scale;
            p.Top = position.Y / scale;
            p.Width = size.Width / scale;
            p.Height = size.Height / scale;

            p.MouseDown += mouseDown;
            p.MouseUp += mouseUp;
            p.MouseMove += mouseMove;

            NodeData nd = new NodeData();
            nd.type = type;

            switch (type)
            {
                case Node.BodyType.Block:
                    p.BackColor = blockColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    p.BackgroundImage = Properties.Resources.block;
                    nd.layer = Layer.Dynamic;
                    {
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Texture = m_textureResource.Get("icons/block.png");
                        shape.Texture.Smooth = true;
                        shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y) * scale;
                        shape.TextureRect = new SFML.Graphics.IntRect(0, 0, (int)shape.Texture.Size.X, (int)shape.Texture.Size.Y);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        m_previewLayers[(int)Layer.Dynamic].Add(shape);
                        nd.drawable = shape;
                    }

                    break;
                case Node.BodyType.PlayerOne:
                    p.BackColor = playerOneColour;
                    p.Move += p1_Move;
                    p.BackgroundImage = Properties.Resources.player_one;
                    nd.layer = Layer.Dynamic;

                    numericUpDownPlayerOneX.ValueChanged -= numericUpDownPlayerOneX_ValueChanged;
                    numericUpDownPlayerOneY.ValueChanged -= numericUpDownPlayerOneY_ValueChanged;
                    numericUpDownPlayerOneX.Value = (decimal)position.X;
                    numericUpDownPlayerOneY.Value = (decimal)position.Y;
                    numericUpDownPlayerOneX.ValueChanged += numericUpDownPlayerOneX_ValueChanged;
                    numericUpDownPlayerOneY.ValueChanged += numericUpDownPlayerOneY_ValueChanged;
                    { 
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Texture = m_textureResource.Get("icons/player_one.png");
                        shape.Texture.Smooth = true;
                        shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y) * scale;
                        shape.TextureRect = new SFML.Graphics.IntRect(0, 0, (int)shape.Texture.Size.X, (int)shape.Texture.Size.Y);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        m_previewLayers[(int)Layer.Dynamic].Add(shape);
                        nd.drawable = shape;
                    }
                    break;
                case Node.BodyType.PlayerTwo:
                    p.BackColor = playerTwoColour;
                    p.Move += p2_Move;
                    p.BackgroundImage = Properties.Resources.player_two;
                    nd.layer = Layer.Dynamic;

                    numericUpDownPlayerTwoX.ValueChanged -= numericUpDownPlayerTwoX_ValueChanged;
                    numericUpDownPlayerTwoY.ValueChanged -= numericUpDownPlayerTwoY_ValueChanged;
                    numericUpDownPlayerTwoX.Value = (decimal)position.X;
                    numericUpDownPlayerTwoY.Value = (decimal)position.Y;
                    numericUpDownPlayerTwoX.ValueChanged += numericUpDownPlayerTwoX_ValueChanged;
                    numericUpDownPlayerTwoY.ValueChanged += numericUpDownPlayerTwoY_ValueChanged;
                    {
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Texture = m_textureResource.Get("icons/player_two.png");
                        shape.Texture.Smooth = true;
                        shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y) * scale;
                        shape.TextureRect = new SFML.Graphics.IntRect(0, 0, (int)shape.Texture.Size.X, (int)shape.Texture.Size.Y);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        m_previewLayers[(int)Layer.Dynamic].Add(shape);
                        nd.drawable = shape;
                    }
                    break;
                case Node.BodyType.Solid:
                    p.BackColor = solidColour;
                    if (m_platformTexture != null)
                        p.BackgroundImage = m_platformTexture;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    nd.layer = Layer.Solid;
                    {
                        var shape = new SFML.Graphics.RectangleShape(new SFML.Window.Vector2f(Size.Width, size.Height));
                        shape.FillColor = new SFML.Graphics.Color(solidColour.R, solidColour.G, solidColour.B);
                        string texturePath = m_mapTextureDirectory + "\\" + m_platformFileName;
                        if(File.Exists(texturePath))
                        {
                            shape.Texture = m_textureResource.Get(texturePath);
                            shape.Texture.Repeated = true;
                            shape.TextureRect = new SFML.Graphics.IntRect(position.X, position.Y, size.Width, size.Height);
                            shape.FillColor = SFML.Graphics.Color.White;
                        }
                        
                        shape.Size = new SFML.Window.Vector2f(size.Width, size.Height);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        m_previewLayers[(int)Layer.Solid].Add(shape);
                        nd.drawable = shape;
                    }


                    break;
                case Node.BodyType.Item:
                    p.BackColor = bonusColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    p.BackgroundImage = Properties.Resources.item;
                    nd.layer = Layer.Dynamic;
                    {
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Texture = m_textureResource.Get("icons/item.png");
                        shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y) * scale;
                        shape.TextureRect = new SFML.Graphics.IntRect(0, 0, (int)shape.Texture.Size.X, (int)shape.Texture.Size.Y);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        m_previewLayers[(int)Layer.Dynamic].Add(shape);
                        nd.drawable = shape;
                    }
                    break;
                case Node.BodyType.Water:
                    p.BackColor = waterColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    nd.layer = Layer.Water;
                    {
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Size = new SFML.Window.Vector2f(size.Width, size.Height);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        shape.FillColor = waterFillColour;

                        m_previewLayers[(int)Layer.Water].Add(shape);
                        nd.drawable = shape;
                    }
                    break;
                case Node.BodyType.Light:
                    if(m_lightCount < m_maxLights)
                    {
                        m_lightCount++;
                        p.BackColor = sunColour;
                        p.Move += node_Move;
                        p.ContextMenuStrip = m_nodeMenu;
                        p.BackgroundImage = Properties.Resources.bulb;
                        nd.layer = Layer.FrontDetail;
                        {
                            var shape = new SFML.Graphics.RectangleShape();
                            shape.Texture = m_textureResource.Get("icons/bulb2.png");
                            shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y) * scale;
                            shape.TextureRect = new SFML.Graphics.IntRect(0, 0, (int)shape.Texture.Size.X, (int)shape.Texture.Size.Y);
                            shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                            shape.FillColor = new SFML.Graphics.Color(sunColour.R, sunColour.G, sunColour.B);
                            m_previewLayers[(int)Layer.Dynamic].Add(shape);
                            nd.drawable = shape;
                        }
                    }
                    else
                    {
                        MessageBox.Show("Maps cannot contain more than 4 dynamic lights");
                        return null;
                    }
                    break;
                case Node.BodyType.Detail:
                    p.BackColor = Color.Transparent;
                    p.BackgroundImage = m_selectedFrame.smallImage;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    nd.layer = Layer.RearDetail;//layer;
                    nd.frameName = m_selectedFrame.filename;
                    nd.spriteSheet = m_selectedFrame.parentSheet.meta.image;
                    {
                        var shape = new SFML.Graphics.RectangleShape();
                        shape.Size = new SFML.Window.Vector2f(size.Width, size.Height);
                        shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                        shape.Texture = m_textureResource.Get(m_atlasTextureDirectory + "\\" + nd.spriteSheet);
                        shape.TextureRect = m_selectedFrame.subrect;
                        if(m_selectedFrame.rotated)
                        {
                            shape.Size = new SFML.Window.Vector2f(size.Height, size.Width);
                            shape.Rotation = -90f;
                            shape.Origin = new SFML.Window.Vector2f(size.Height, 0f);
                        }

                        m_previewLayers[(int)layer].Add(shape);
                        nd.drawable = shape;
                    }
                    break;
                default: break;
            }

            if (layer != Layer.None) nd.layer = layer;
            p.Tag = nd;
            panelEditorInner.Controls.Add(p);
            if(m_canSort) sortNodes();
            return p;
        }

        private void selectNode(Panel p)
        {
            if (p == null) return;

            if (m_selectedNode != null)
            {
                m_selectedNode.BorderStyle = BorderStyle.None;
                var nd = (NodeData)m_selectedNode.Tag;
                if (nd.drawable != null)
                    nd.drawable.OutlineThickness = 0f;
                m_selectedNode.Tag = nd;
            }
            m_selectedNode = p;
            m_selectedNode.BorderStyle = BorderStyle.Fixed3D;

            numericUpDownNodePropertyPosX.Enabled = true;
            numericUpDownNodePropertyPosY.Enabled = true;

            numericUpDownNodePropertyPosX.Value = (decimal)m_selectedNode.Left * scale;
            numericUpDownNodePropertyPosY.Value = (decimal)m_selectedNode.Top * scale;

            numericUpDownNodePropertySizeX.Value = (decimal)m_selectedNode.Width * scale;
            numericUpDownNodePropertySizeY.Value = (decimal)m_selectedNode.Height * scale;

            checkBoxFrontDetail.Enabled = false;
            //checkBoxFrontDetail.Checked = false;

            var tag = ((NodeData)p.Tag);
            var type = tag.type;
            if(type == Node.BodyType.Solid || type == Node.BodyType.Water)
            {
                numericUpDownNodePropertySizeX.Enabled = true;
                numericUpDownNodePropertySizeY.Enabled = true;
            }
            else// if(tag == Node.BodyType.Block)
            {
                numericUpDownNodePropertySizeX.Enabled = false;
                numericUpDownNodePropertySizeY.Enabled = false;
            }
            comboBoxNodePropertyType.SelectedValue = type;
            comboBoxNodePropertyType.Enabled = true;

            if(type == Node.BodyType.Detail)
            {
                selectFrame(tag.spriteSheet, tag.frameName);
                checkBoxFrontDetail.Enabled = true;
                checkBoxFrontDetail.Checked = (tag.layer == Layer.FrontDetail);

                if(m_selectedFrame.rotated)
                {
                    tag.drawable.Size = new SFML.Window.Vector2f((float)numericUpDownNodePropertySizeY.Value, (float)numericUpDownNodePropertySizeX.Value);
                }
            }

            if (tag.drawable != null)
                tag.drawable.OutlineThickness = -2f;

            p.Tag = tag;
        }

        private void setPanelTexture(ref Panel p)
        {
            p.BackgroundImage = m_selectedFrame.smallImage;
            p.Size = p.BackgroundImage.Size;
            var nd = (NodeData)p.Tag;
            nd.spriteSheet = m_selectedFrame.parentSheet.meta.image;
            nd.frameName = m_selectedFrame.filename;

            //update drawable
            if(nd.drawable != null)
            {
                var size = new Size(p.Size.Width * scale, p.Size.Height * scale);
                nd.drawable.Size = new SFML.Window.Vector2f(size.Width, size.Height);
                //shape.Position = new SFML.Window.Vector2f(position.X, position.Y);
                nd.drawable.Texture = m_textureResource.Get(m_atlasTextureDirectory + "\\" + nd.spriteSheet);
                nd.drawable.TextureRect = m_selectedFrame.subrect;
                if (m_selectedFrame.rotated)
                {
                    nd.drawable.Size = new SFML.Window.Vector2f(size.Height, size.Width);
                    nd.drawable.Rotation = -90f;
                    nd.drawable.Origin = new SFML.Window.Vector2f(size.Height, 0f);
                }
                else
                {
                    nd.drawable.Rotation = 0f;
                    nd.drawable.Origin = new SFML.Window.Vector2f();
                }
            }

            p.Tag = nd;
        }

        /// <summary>
        /// Updates internal map data with changes from the UI
        /// </summary>
        private void updateMapData()
        {
            m_currentMap.NpcCount = (int)numericUpDownNpcCount.Value;
            m_currentMap.NpcTotal = (int)numericUpDownNpcTotal.Value;
            m_currentMap.PlayerOneSpawn = new Point((int)numericUpDownPlayerOneX.Value, (int)numericUpDownPlayerOneY.Value);
            m_currentMap.PlayerTwoSpawn = new Point((int)numericUpDownPlayerTwoX.Value, (int)numericUpDownPlayerTwoY.Value);
            m_currentMap.AmbientColour = panelAmbientColour.BackColor.ToArgb();
            m_currentMap.SunColour = panelSunColour.BackColor.ToArgb();
            m_currentMap.BackgroundTexture = m_backgroundFileName;
            m_currentMap.PlatformTexture = m_platformFileName;
            m_currentMap.Nodes.Clear();

            foreach(Panel p in panelEditorInner.Controls)
            {
                m_currentMap.Nodes.Add(new Node(p));
            }

            m_modified = true;
        }

        private void newFile()
        {
            //clear out sprites
            foreach (var layer in m_previewLayers)
                layer.Clear();
            
            m_currentMap = new Map();
            m_lightCount = 0;

            //set window title to 'untitled'
            this.Text = "Untitled";

            panelEditorInner.Controls.Clear();
            comboBoxAddNode.SelectedIndex = 0;

            //add the default bounds
            addNode(Node.BodyType.Solid, new Point(), new Size(20, 1060));
            addNode(Node.BodyType.Solid, new Point(1900, 0), new Size(20, 1060));
            addNode(Node.BodyType.Solid, new Point(0, 1060), new Size(1920, 20));

            //add player spawn points
            m_playerOnePanel = addNode(Node.BodyType.PlayerOne, new Point(40, 500), playerSize);
            m_playerTwoPanel = addNode(Node.BodyType.PlayerTwo, new Point(1820, 500), playerSize);

            //reset map properties
            numericUpDownNpcCount.Value = 3;
            numericUpDownNpcTotal.Value = 12;
            
            numericUpDownPlayerOneX.Value = 80;
            numericUpDownPlayerOneY.Value = 500;

            numericUpDownPlayerTwoX.Value = 1680;
            numericUpDownPlayerTwoY.Value = 500;

            panelAmbientColour.BackColor = ambientColour;
            panelSunColour.BackColor = sunColour;

            numericUpDownNodePropertyPosX.Enabled = false;
            numericUpDownNodePropertyPosY.Enabled = false;
            numericUpDownNodePropertySizeX.Enabled = false;
            numericUpDownNodePropertySizeY.Enabled = false;

            centreEditor();
        }

        private void openFile(string path)
        {
            //prevent sorting until load complete
            m_canSort = false;

            //clear out sprites
            foreach (var layer in m_previewLayers)
                layer.Clear();

            //create new map object and parse data into it
            m_currentMap = new Map();
            m_lightCount = 0;

            JsonSerializer js = new JsonSerializer();
            js.NullValueHandling = NullValueHandling.Ignore;

            using (StreamReader sr = new StreamReader(path))
            using(JsonReader jr = new JsonTextReader(sr))
            {
                m_currentMap = js.Deserialize<Map>(jr);
            }

            //update UI from object
            numericUpDownNpcCount.Value = (decimal)m_currentMap.NpcCount;
            numericUpDownNpcTotal.Value = (decimal)m_currentMap.NpcTotal;

            panelAmbientColour.BackColor = Color.FromArgb(m_currentMap.AmbientColour);
            panelSunColour.BackColor = Color.FromArgb(m_currentMap.SunColour);

            try
            {
                Image img = Image.FromFile(m_mapTextureDirectory + "\\" + m_currentMap.PlatformTexture);
                m_platformTexture = (Image)(new Bitmap(img, new Size(img.Width / scale, img.Height / scale)));
                m_platformFileName = m_currentMap.PlatformTexture;
            }
            catch { }

            try
            {
                Image img = Image.FromFile(m_mapTextureDirectory + "\\" + m_currentMap.BackgroundTexture);
                panelEditorInner.BackgroundImage = (Image)(new Bitmap(img, new Size(img.Width / scale, img.Height / scale)));
                m_backgroundFileName = m_currentMap.BackgroundTexture;

                m_previewLayers[(int)Layer.Background].Clear();
                var shape = new SFML.Graphics.RectangleShape();
                shape.Texture = m_textureResource.Get(m_mapTextureDirectory + "\\" + m_currentMap.BackgroundTexture); ;
                shape.Size = new SFML.Window.Vector2f(shape.Texture.Size.X, shape.Texture.Size.Y);
                m_previewLayers[(int)Layer.Background].Add(shape);
            }
            catch { }

            panelEditorInner.Controls.Clear();
            m_selectedNode = null;
            m_playerOnePanel = null;
            m_playerTwoPanel = null;

            foreach(Node n in m_currentMap.Nodes)
            {
                switch(n.Type)
                {
                    case "Solid":
                        addNode(Node.BodyType.Solid, n.Position, n.Size);
                        break;
                    case "Block":
                        addNode(Node.BodyType.Block, n.Position, n.Size);
                        break;
                    case "PlayerOne":
                        m_playerOnePanel = addNode(Node.BodyType.PlayerOne, n.Position, n.Size);
                        break;
                    case "PlayerTwo":
                        m_playerTwoPanel = addNode(Node.BodyType.PlayerTwo, n.Position, n.Size);
                        break;
                    case "Water":
                        addNode(Node.BodyType.Water, n.Position, n.Size);
                        break;
                    case "Item":
                        addNode(Node.BodyType.Item, n.Position, n.Size);
                        break;
                    case "Light":
                        var node = addNode(Node.BodyType.Light, n.Position, n.Size);
                        node.BackColor = Color.FromArgb(n.Colour);
                        NodeData nd = (NodeData)node.Tag;
                        nd.anchorOffset = n.AnchorOffset;
                        nd.drawable.FillColor = new SFML.Graphics.Color(node.BackColor.R, node.BackColor.G, node.BackColor.B);
                        node.Tag = nd;
                        break;
                    case "Detail":
                        selectFrame(n.SpriteSheet, n.FrameName);
                        //Layer l = (n.Layer == "RearDetail") ? Layer.RearDetail : Layer.FrontDetail;
                        addNode(Node.BodyType.Detail, n.Position, n.Size, (n.Layer == "RearDetail") ? Layer.RearDetail : Layer.FrontDetail);
                        break;
                }
            }
            
            if(m_playerOnePanel == null || m_playerTwoPanel == null)
            {
                MessageBox.Show("Missing Player Data. Map File Not Loaded.", "Error");
                newFile();
                return;
            }

            //set window title to map name
            this.Text = m_currentMap.MapName;
            m_modified = false;

            //reenable sort and perform inital sorting
            m_canSort = true;
            sortNodes();
        }

        private void saveFile()
        {
            updateMapData();

            //save json file
            JsonSerializer srlz = new JsonSerializer();
            srlz.NullValueHandling = NullValueHandling.Ignore;

            using (StreamWriter sw = new StreamWriter(m_mapPath))
            using (JsonWriter jw = new JsonTextWriter(sw))
            {
                srlz.Serialize(jw, m_currentMap);
            }

            m_modified = false;

            this.Text = m_currentMap.MapName;
        }

        private void saveFileAs(string path)
        {
            //update map object path and save
            m_currentMap.MapName = Path.GetFileName(path);
            saveFile();
        }

        private void sortNodes()
        {
            Control[] controls = new Control[panelEditorInner.Controls.Count];
            panelEditorInner.Controls.CopyTo(controls, 0);

            Array.Sort(controls, new ControlsComparer());

            panelEditorInner.Controls.Clear();
            panelEditorInner.Controls.AddRange(controls);
        }

        private class ControlsComparer : IComparer<Control>
        {
            public int Compare(Control a, Control b)
            {
                Panel pA = (Panel)a;
                Panel pB = (Panel)b;

                NodeData ndA = (NodeData)pA.Tag;
                NodeData ndB = (NodeData)pB.Tag;

                if ((int)ndA.layer < (int)ndB.layer) return 1;
                else if ((int)ndA.layer > (int)ndB.layer) return -1;
                else return 0;
            }
        }

        private void clampPanelToBounds(ref Panel p)
        {
            //clamp to bounds
            if (p.Left < 0) p.Left = 0;
            int max = (int)numericUpDownNodePropertyPosX.Maximum / scale;
            if (p.Left > max) p.Left = max;

            if (p.Top < 0) p.Top = 0;
            max = (int)numericUpDownNodePropertyPosY.Maximum / scale;
            if (p.Top > max) p.Top = max;
        }

    }
}
