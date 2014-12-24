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
        private const int m_maxLights = 3;
        
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
        private Panel addNode(Node.BodyType type, Point position, Size size)
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

            p.Tag = type;            

            switch (type)
            {
                case Node.BodyType.Block:
                    p.BackColor = blockColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    break;
                case Node.BodyType.PlayerOne:
                    p.BackColor = playerOneColour;
                    p.Move += p1_Move;
                    break;
                case Node.BodyType.PlayerTwo:
                    p.BackColor = playerTwoColour;
                    p.Move += p2_Move;
                    break;
                case Node.BodyType.Solid:
                    p.BackColor = solidColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    break;
                case Node.BodyType.Item:
                    p.BackColor = bonusColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    break;
                case Node.BodyType.Water:
                    p.BackColor = waterColour;
                    p.Move += node_Move;
                    p.ContextMenuStrip = m_nodeMenu;
                    break;
                case Node.BodyType.Light:
                    if(m_lightCount < m_maxLights)
                    {
                        m_lightCount++;
                        p.BackColor = sunColour;
                        p.Move += node_Move;
                        p.ContextMenuStrip = m_nodeMenu;
                        p.BackgroundImage = Image.FromFile(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + @"\icons\bulb.png"); 
                    }
                    else
                    {
                        MessageBox.Show("Maps cannot contain more than 3 dynamic lights");
                        return null;
                    }
                    break;
                default: break;
            }

            panelEditorInner.Controls.Add(p);

            return p;
        }

        private void selectNode(Panel p)
        {
            if (p == null) return;

            if (m_selectedNode != null)
            {
                m_selectedNode.BorderStyle = BorderStyle.None;
                if((Node.BodyType)m_selectedNode.Tag == Node.BodyType.Light)
                {
                    //remove colour picker
                    panelNodeColour.Click -= pickLightColour;
                    panelNodeColour.BackColor = Color.DarkGray;
                }
            }
            m_selectedNode = p;
            m_selectedNode.BorderStyle = BorderStyle.Fixed3D;

            numericUpDownNodePropertyPosX.Enabled = true;
            numericUpDownNodePropertyPosY.Enabled = true;

            numericUpDownNodePropertyPosX.Value = (decimal)m_selectedNode.Left * scale;
            numericUpDownNodePropertyPosY.Value = (decimal)m_selectedNode.Top * scale;

            numericUpDownNodePropertySizeX.Value = (decimal)m_selectedNode.Width * scale;
            numericUpDownNodePropertySizeY.Value = (decimal)m_selectedNode.Height * scale;


            var tag = (Node.BodyType)p.Tag;
            if(tag == Node.BodyType.Solid || tag == Node.BodyType.Water)
            {
                numericUpDownNodePropertySizeX.Enabled = true;
                numericUpDownNodePropertySizeY.Enabled = true;
            }
            else// if(tag == Node.BodyType.Block)
            {
                numericUpDownNodePropertySizeX.Enabled = false;
                numericUpDownNodePropertySizeY.Enabled = false;
            }
            comboBoxNodePropertyType.SelectedValue = tag;
            comboBoxNodePropertyType.Enabled = true;

            if(tag == Node.BodyType.Light)
            {
                //add event handler and set colour
                panelNodeColour.Click += pickLightColour;
                panelNodeColour.BackColor = m_selectedNode.BackColor;
            }
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
            m_currentMap.Nodes.Clear();

            foreach(Panel p in panelEditorInner.Controls)
            {
                m_currentMap.Nodes.Add(new Node(p));
            }

            m_modified = true;
        }

        private void newFile()
        {
            m_currentMap = new Map();

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
            //TODO NPC texture
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
        }

        private void openFile(string path)
        {
            //create new map object and parse data into it
            m_currentMap = new Map();

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
                        addNode(Node.BodyType.Light, n.Position, n.Size).BackColor = Color.FromArgb(n.Colour);
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

    }
}
