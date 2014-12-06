using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Level_editor
{
    class Node
    {
        public Node(Panel p)
        {
            m_position = new SizeF(p.Top, p.Left);
            m_size = new SizeF(p.Width, p.Height);
            m_type = Type.Solid;
        }
        
        public enum Type
        {
            Block,
            Solid,
            PlayerOne,
            PlayerTwo
        }

        private SizeF m_position;

        public SizeF position
        {
            get { return m_position; }
            set { m_position = value; }
        }

        private SizeF m_size;

        public SizeF size
        {
            get { return m_size; }
            set { m_size = value; }
        }

        private Type m_type;
        public Type type
        {
            get { return m_type; }
            set { m_type = value; }
        }


    }
}
