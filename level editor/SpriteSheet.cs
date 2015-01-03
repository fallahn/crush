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
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace Atlas
{
    public class IntRect
    {
        public int x { get; set; }
        public int y { get; set; }
        public int w { get; set; }
        public int h { get; set; }
    }

    public class SourceSize
    {
        public int w { get; set; }
        public int h { get; set; }
    }

    public class Pivot
    {
        public float x { get; set; }
        public float y { get; set; }
    }

    public class Frame
    {
        public string filename { get; set; }
        public IntRect frame { get; set; }
        public bool rotated { get; set; }
        public bool trimmed { get; set; }
        public IntRect spriteSourceSize { get; set; }
        public SourceSize sourceSize { get; set; }
        public Pivot pivot { get; set; }
        public Bitmap largeImage { get; set; }
        public Bitmap smallImage { get; set; }
        public SpriteSheet parentSheet { get; set; }
    }

    public class Size
    {
        public int w { get; set; }
        public int h { get; set; }
    }

    public class Meta
    {
        public string app { get; set; }
        public string version { get; set; }
        public string image { get; set; }
        public string format { get; set; }
        public Size size { get; set; }
        public string scale { get; set; }
        public string smartupdate { get; set; }
    }

    public class SpriteSheet
    {
        public List<Frame> frames { get; set; }
        public Meta meta { get; set; }
    }
}
