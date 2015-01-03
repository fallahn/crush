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
    public partial class OptionsWindow : Form
    {
        private MainWindow m_mainWindow;

        public OptionsWindow(MainWindow mainWindow)
        {
            InitializeComponent();

            m_mainWindow = mainWindow;
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            if (textBoxTexturePath.Text != null && textBoxTexturePath.Text != string.Empty)
            {
                m_mainWindow.TextureDirectory = textBoxTexturePath.Text;
                RegKey.write("texture_directory", textBoxTexturePath.Text);
                this.DialogResult = DialogResult.OK;
            }
            else
            {
                MessageBox.Show("Path to texture directory is missing");
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void buttonBrowseTextures_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fd = new FolderBrowserDialog();
            if(fd.ShowDialog() == DialogResult.OK)
            {
                textBoxTexturePath.Text = fd.SelectedPath;
            }
        }
    }
}
