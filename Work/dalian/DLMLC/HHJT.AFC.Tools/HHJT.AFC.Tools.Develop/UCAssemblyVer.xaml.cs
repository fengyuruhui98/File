using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
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
using HHJT.AFC.Base.API;
using Microsoft.Win32;

namespace HHJT.AFC.Tools.Develop
{
    /// <summary>
    /// UCAssemblyVer.xaml 的交互逻辑
    /// </summary>
    public partial class UCAssemblyVer : UserControl
    {
        public UCAssemblyVer()
        {
            InitializeComponent();
        }

        private string origPath;
        private string destPath;
        private void btnPath_Click_1(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new System.Windows.Forms.FolderBrowserDialog();
            dlg.ShowDialog();
            if (dlg.SelectedPath != string.Empty)
                lblPath.Content = origPath = dlg.SelectedPath;

        }

        private void btnOutput_Click_1(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new System.Windows.Forms.FolderBrowserDialog();
            dlg.ShowDialog();
            if (dlg.SelectedPath != string.Empty)
                this.lblOutputPath.Content = destPath = dlg.SelectedPath;
        }

        private void btnGenerate_Click_1(object sender, RoutedEventArgs e)
        {
            List<string> strs = new List<string>();
            foreach (var item in lbxAssembly.Items)
            {
                if ((item as CheckBox).IsChecked == true)
                {
                    try
                    {
                        string str = (item as CheckBox).Content.ToString();
                        var verInfo = FileVersionInfo.GetVersionInfo(origPath + "//" + str);
                        //Assembly asm = Assembly.LoadFrom(origPath + "//" + str);
                        //var vv = asm.GetName();
                        //str = str + ";" + asm.GetName().Version;
                        str = str + ";" + verInfo.FileVersion;
                        strs.Add(str);
                        tbxSoftVer.Text = tbxSoftVer.Text + str + "\n";
                    }
                    catch (Exception)
                    {

                        ;
                    }

                }
            }

            FileOP.WriteTextFile(destPath + "\\" + tbxOutputFileName.Text, strs.ToArray());

        }

        private void btnAssembly_Click_1(object sender, RoutedEventArgs e)
        {


            foreach (string fullFileName in Directory.GetFiles(origPath, "*.exe"))//检索待处理目录
            {
                //Assembly asm = Assembly.LoadFrom(fullFileName);
                string str = fullFileName.Remove(0, origPath.Length + 1);// +";" + asm.GetName().Version;
                //strs.Add();

                CheckBox cbx = new CheckBox();
                cbx.IsChecked = true;
                cbx.Content = str;
                lbxAssembly.Items.Add(cbx);

            }
            foreach (string fullFileName in Directory.GetFiles(origPath, "*.dll"))//检索待处理目录
            {
                //Assembly asm = Assembly.LoadFrom(fullFileName);
                string str = fullFileName.Remove(0, origPath.Length + 1);// +";" + asm.GetName().Version;
                //strs.Add();

                CheckBox cbx = new CheckBox();
                cbx.IsChecked = true;
                cbx.Content = str;
                lbxAssembly.Items.Add(cbx);

            }
        }
    }
}
