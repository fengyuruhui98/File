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
using Microsoft.Win32;

namespace HHJT.AFC.Tools.Develop
{
    /// <summary>
    /// UCAutoCode.xaml 的交互逻辑
    /// </summary>
    public partial class UCAutoCode : UserControl
    {
        public UCAutoCode()
        {
            InitializeComponent();
        }

        private ExcelOP _excelOP;

        private void btnOpen_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openfileDialog = new OpenFileDialog();
            //openfileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.CommonDesktopDirectory);
            openfileDialog.Filter = "Excel文件(*.xlsx)|*.xlsx";
            openfileDialog.ValidateNames = true;     //文件有效性验证ValidateNames
            openfileDialog.CheckFileExists = true;  //验证路径有效性
            openfileDialog.CheckPathExists = true; //验证文件有效性

            try
            {
                if (openfileDialog.ShowDialog() == true)
                {
                    lblFile.Content = openfileDialog.FileName;
                    _excelOP = new ExcelOP(openfileDialog.FileName);
                    //获取sheets
                    var sheets = _excelOP.GetExcelSheets();
                    lbxVMList.ItemsSource = sheets;
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnGenerate_Click(object sender, RoutedEventArgs e)
        {
            int idx = lbxVMList.SelectedIndex;
            if (idx < 0)
            {
                MessageBox.Show("请选择所要转换的类型定义", "提示！", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            if (_excelOP == null)
            {
                MessageBox.Show("请先选择文件", "提示！", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            var data = _excelOP.ReadExcelSheet(lbxVMList.SelectedItem.ToString());
            if (data != null)
            {
                if (data[0, 0] == "Class")
                {
                    ClassCode codes = new ClassCode(data);
                    tbxOutputM.Text = codes.OutputM();
                    tbxOutputVM.Text = codes.OutputVM();
                }
                else if (data[0, 0] == "Command")
                {
                    CommandCode codes = new CommandCode(data);
                    tbxOutputM.Text = "";
                    tbxOutputVM.Text = codes.OutputCode();
                }
                else if (data[0, 0] == "Prop")
                {
                    PropCode codes = new PropCode(data);
                    tbxOutputM.Text = codes.OutputCodeM();
                    tbxOutputVM.Text = codes.OutputCodeVM();
                }
                else
                {
                    MessageBox.Show("未知转换类型：" + data[0, 0]);
                }
            }

        }



    }




}
