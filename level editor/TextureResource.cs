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

//manages sfml textures

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Level_editor
{
    class TextureResource
    {
        private Dictionary<string, SFML.Graphics.Texture> m_textures;


        public TextureResource()
        {
            m_textures = new Dictionary<string, SFML.Graphics.Texture>();
        }

        public SFML.Graphics.Texture Get(string path)
        {
            //TODO this needs error checking instead assuming
            //texture loading always succeeds
            if (!m_textures.ContainsKey(path))
            {
                //try
                //{
                    m_textures.Add(path, new SFML.Graphics.Texture(path));
                //}
                //finally
                //{
                //    throw new FileNotFoundException();
                //}
            }   
            return m_textures[path];
        }
    }
}
