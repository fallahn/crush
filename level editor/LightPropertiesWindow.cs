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
    public partial class LightPropertiesWindow : Form
    {
        private Panel m_panel;


        public LightPropertiesWindow(ref Panel panel)
        {
            InitializeComponent();

            m_panel = panel;
            panelColour.BackColor = panel.BackColor;

            NodeData nd = (NodeData)panel.Tag;
            numericUpDownAnchorOffset.Value = (decimal)nd.anchorOffset;
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            NodeData nd = (NodeData)m_panel.Tag;
            nd.anchorOffset = (int)numericUpDownAnchorOffset.Value;
            nd.drawable.FillColor = new SFML.Graphics.Color(m_panel.BackColor.R, m_panel.BackColor.G, m_panel.BackColor.B);
            m_panel.Tag = nd;
            
            this.DialogResult = DialogResult.OK;
        }

        private void panelColour_DoubleClick(object sender, EventArgs e)
        {
            ColorDialog cd = new ColorDialog();
            cd.FullOpen = true;
            cd.Color = m_panel.BackColor;

            if(cd.ShowDialog() == DialogResult.OK)
            {
                panelColour.BackColor = cd.Color;
                m_panel.BackColor = cd.Color;
            }
        }
    }
}
