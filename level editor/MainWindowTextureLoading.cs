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

//handles loading texture atlases so they can be displayed in the editor

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

using Atlas;
using Newtonsoft.Json;

namespace Level_editor
{
    partial class MainWindow : Form
    {
        private List<SpriteSheet> m_spriteSheets = new List<SpriteSheet>();
        private Frame m_selectedFrame = null;

        public List<SpriteSheet> SpriteSheets
        {
            get { return m_spriteSheets; }
        }
        public Frame SelectedFrame
        {
            get { return m_selectedFrame; }
            set { m_selectedFrame = value; }
        }

        //------------------------------------
        private void loadTextures()
        {
            if(m_textureDirectory == null || m_textureDirectory == string.Empty)
            {
                MessageBox.Show("Invalid texture directory, textures will not be loaded");
                return;
            }

            //get all json files in directory
            var files = Directory.GetFiles(m_textureDirectory);
            files = filterJson(files);
            if(files.Length < 1)
            {
                MessageBox.Show("No json files were found in texture directory.");
                return;
            }
            
            //for each file parse into sprite sheet object
            m_spriteSheets.Clear();
            foreach(string file in files)
            {
                loadFile(file);
            }

            //for each sprite sheet load image and cut into frames
            foreach(SpriteSheet s in m_spriteSheets)
            {
                string imagePath = m_textureDirectory + "\\" + s.meta.image;
                if(File.Exists(imagePath))
                {
                    var ext = Path.GetExtension(imagePath);
                    if(ext != ".png" && ext != ".jpg")
                    {
                        MessageBox.Show(ext + "\n File format not supported. Please use .png or .jpg");
                        return;
                    }

                    Bitmap bitmap = (Bitmap)Bitmap.FromFile(imagePath);
                    foreach(Frame frame in s.frames)
                    {
                        Rectangle subrect = new Rectangle(frame.frame.x, frame.frame.y, frame.frame.w, frame.frame.h);
                        if(frame.rotated) //width and height are swapped!!
                        {
                            var i = subrect.Width;
                            subrect.Width = subrect.Height;
                            subrect.Height = i;
                        }

                        try
                        {
                            frame.largeImage = bitmap.Clone(subrect, bitmap.PixelFormat);
                            if (frame.rotated)
                            {
                                frame.largeImage.RotateFlip(RotateFlipType.Rotate270FlipNone);
                            }
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show(e.Message + "\nAre you trying to create a sub image which is out of bounds?", "Cropping Sprite Sheet");
                        }
                        frame.smallImage = new Bitmap(frame.largeImage, new System.Drawing.Size(frame.largeImage.Size.Width / 2, frame.largeImage.Size.Height / 2));
                        frame.parentSheet = s;
                    }
                }
                else
                {
                    MessageBox.Show(imagePath + "\nFile Not Found");
                    return;
                }

                if (m_spriteSheets.Count > 0 && m_spriteSheets[0].frames.Count > 0) 
                    m_selectedFrame = m_spriteSheets[0].frames[0];
            }
        }

        private string[] filterJson(string[] array)
        {
            List<string> indices = new List<string>();
            foreach(var str in array)
            {
                if(Path.GetExtension(str) != ".json")
                {
                    indices.Add(str);
                }
            }

            if(indices.Count > 0)
            {              
                List<string> paths = new List<string>(array);
                foreach(string str in indices)
                {
                    paths.Remove(str);
                }
                return paths.ToArray();
            }
            return array;
        }

        public bool loadFile(string path)
        {           
            try
            {
                SpriteSheet sheet = new SpriteSheet();

                JsonSerializer js = new JsonSerializer();
                js.NullValueHandling = NullValueHandling.Ignore;

                using (StreamReader sr = new StreamReader(path))
                using (JsonReader jr = new JsonTextReader(sr))
                {
                    sheet = js.Deserialize<SpriteSheet>(jr);
                }
                m_spriteSheets.Add(sheet);
                return true;
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Failed to parse atlas json");
                return false;
            }
        }
    }
}
