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
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            tabCtrl.Items.Add(new TabItem() { Header = "数据加解密", Content = new UCCrypt() });
            tabCtrl.Items.Add(new TabItem() { Header = "VM&M代码生成", Content = new UCAutoCode() });
            tabCtrl.Items.Add(new TabItem() { Header = "接口代码生成(C#)", Content = new UCCSharp() });
            tabCtrl.Items.Add(new TabItem() { Header = "接口代码生成(C++)", Content = new UCCpp() });
            tabCtrl.Items.Add(new TabItem() { Header = "组件版本信息", Content = new UCAssemblyVer() });

        }
    }
}
