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
        private string m_currentFile = "new";
        private bool m_modified = false;
        private const string fileFilter = "Map Files (.crm)|*.crm";
        
        public MainWindow()
        {
            InitializeComponent();
            WindowState = FormWindowState.Maximized;
        }


        //--------------event handlers-----------------//

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

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = fileFilter;

            if(fd.ShowDialog() == DialogResult.OK)
            {
                openFile(fd.FileName);
                m_currentFile = fd.FileName;
            }
        }


        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFile();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sd = new SaveFileDialog();
            sd.Filter = fileFilter;

            if(sd.ShowDialog() == DialogResult.OK)
            {
                saveFileAs(sd.FileName);
                m_currentFile = sd.FileName;
            }
        }

        //------------others---------------//

        private void centreEditor()
        {
            //centre the inner editor area to its parent
            panelEditorInner.Left = (panelEditorOuter.Width - panelEditorInner.Width) / 2;
            panelEditorInner.Top = (panelEditorOuter.Height - panelEditorInner.Height) / 2;            
        }

        private void newFile()
        {

        }

        private void openFile(string path)
        {

        }

        private void saveFile()
        {
            //use current path
        }

        private void saveFileAs(string path)
        {

        }



    }
}
