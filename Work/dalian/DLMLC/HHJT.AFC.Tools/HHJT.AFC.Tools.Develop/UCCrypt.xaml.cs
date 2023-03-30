using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HHJT.AFC.Tools.Develop
{
    /// <summary>
    /// UCCrypt.xaml 的交互逻辑
    /// </summary>
    public partial class UCCrypt : UserControl
    {
        public UCCrypt()
        {
            InitializeComponent();
        }
        private void buttonDecode_Click_1(object sender, RoutedEventArgs e)
        {
            textboxDest.Text = HHJT.AFC.Base.API.CryptFuncs.DESDecrypt(textboxOrig.Text);
        }

        private void buttonEncode_Click_1(object sender, RoutedEventArgs e)
        {
            textboxDest.Text = HHJT.AFC.Base.API.CryptFuncs.DESEncrypt(textboxOrig.Text);

        }
    }
}
