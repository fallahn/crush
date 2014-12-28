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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Microsoft.Win32;

namespace Level_editor
{
    class RegKey
    {
        private static RegistryKey m_baseRegistryKey = Registry.LocalMachine;
        public RegistryKey BaseRegistryKey
        {
            get { return m_baseRegistryKey; }
            set { m_baseRegistryKey = value; }
        }

        private static string m_subkey = "SOFTWARE\\" + Application.ProductName;
        public string SubKey
        {
            get { return m_subkey; }
            set { m_subkey = value; }
        }

        public static string read(string keyName)
        {
            RegistryKey rk = m_baseRegistryKey;
            RegistryKey sk1 = rk.OpenSubKey(m_subkey);

            if (sk1 == null)
            {
                return null;
            }
            else
            {
                try
                {
                    return (string)sk1.GetValue(keyName.ToUpper());
                }
                catch (Exception e)
                {
                    alert(e, "Reading registry " + keyName.ToUpper());
                    return null;
                }
            }
        }

        public static bool write(string keyName, object keyVal)
        {
            try
            {
                RegistryKey rk = m_baseRegistryKey;
                RegistryKey sk1 = rk.CreateSubKey(m_subkey);
                sk1.SetValue(keyName.ToUpper(), keyVal);

                return true;
            }
            catch (Exception e)
            {
                alert(e, "Writing registry " + keyName.ToUpper());
                return false;
            }
        }

        private static void alert(Exception e, string title)
        {
            MessageBox.Show(e.Message, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}
