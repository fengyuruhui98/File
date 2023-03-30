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
using Excel = Microsoft.Office.Interop.Excel;
//using HHJT.XA.XDR;

namespace HHJT.AFC.Tools.Develop
{
    /// <summary>
    /// UCCSharp.xaml 的交互逻辑
    /// </summary>
    public partial class UCCSharp : UserControl
    {
        public UCCSharp()
        {
            InitializeComponent();
        }
        protected string m_fileName;

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
                    lblFile.Content = m_fileName = openfileDialog.FileName;
                    //获取sheets
                    GetExcelSheets();
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void GetExcelSheets()
        {
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(m_fileName);
            lbxVMList.Items.Clear();
            Excel.Worksheet xlWorksheet;
            foreach (var item in xlWorkbook.Sheets)
            {
                xlWorksheet = item as Excel.Worksheet;
                lbxVMList.Items.Add(xlWorksheet.Name);
            }
            xlWorkbook.Close();
            xlApp.Quit();
        }

        List<StructInfo> _structs = new List<StructInfo>();

        private string GetCellContent(Excel.Range range, int i, int j)
        {
            dynamic c = range.Cells[i, j].Value2;
            return c == null ? null : c.ToString().Trim();
        }

        private void ReadExcelSheet(int sheetIndex)
        {
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(m_fileName);
            Excel.Worksheet xlWorksheet = xlWorkbook.Sheets[sheetIndex + 1];

            Excel.Range xlRange = xlWorksheet.UsedRange;

            int rowCount = xlRange.Rows.Count;
            int colCount = xlRange.Columns.Count;

            _structs.Clear();

            int currentRow = 2;

            while (currentRow <= rowCount)
            {
                string s = GetCellContent(xlRange, currentRow, 1);
                if (s != null)
                {
                    _structs.Add(new StructInfo
                    {
                        StructName = s,
                        Comment = GetCellContent(xlRange, currentRow, 2)
                    });
                }
                else
                {
                    StructInfo curStruct = _structs.Last();
                    FieldInfo curField = new FieldInfo();
                    curField.FieldName = GetCellContent(xlRange, currentRow, 2);
                    curField.FieldType = GetCellContent(xlRange, currentRow, 3);
                    curField.IsIntel = GetCellContent(xlRange, currentRow, 4) == "intel" ? true : false;
                    if (GetCellContent(xlRange, currentRow, 5) != null)
                    {
                        curField.IsFixedArray = true;
                        curField.FixedArrayCount = GetCellContent(xlRange, currentRow, 5);
                    }
                    else if (GetCellContent(xlRange, currentRow, 6) != null)
                    {
                        curField.IsDynamicArray = true;
                        curField.DynamicArrayCountName = GetCellContent(xlRange, currentRow, 6);
                    }
                    curField.Comment = GetCellContent(xlRange, currentRow, 7);
                    curField.IsUserDefined = GetCellContent(xlRange, currentRow, 8) != null ? true : false;
                    curStruct.Fields.Add(curField);
                }
                currentRow++;
            }



            xlWorkbook.Close();
            xlApp.Quit();
        }

        private void btnCopy_Click(object sender, RoutedEventArgs e)
        {
            tbxOutput.Select(0, tbxOutput.Text.Length);
        }

        private void btnGenerate_Click(object sender, RoutedEventArgs e)
        {
            int idx = lbxVMList.SelectedIndex;
            if (idx < 0)
            {
                MessageBox.Show("请选择所要转换的类型定义", "提示！", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            ReadExcelSheet(idx);

            string strOutput = "";
            strOutput += string.Format("#region {0}\n", lbxVMList.SelectedValue);
            foreach (var item in _structs)
            {
                strOutput += item.OutputCode();
            }
            strOutput += "\n#endregion\n";
            tbxOutput.Text = strOutput;
        }
    }

    class StructInfo
    {
        public string StructName { get; set; }
        List<FieldInfo> _fields = new List<FieldInfo>();
        public string Comment { get; set; }
        internal List<FieldInfo> Fields
        {
            get { return _fields; }
            set { _fields = value; }
        }



        public string OutputCode()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("public class {0} //{1}\n", StructName, Comment);
            strB.Append("\t{\n");
            foreach (var item in _fields)
            {
                strB.Append(item.OutputCode());
            }


            //default constructor
            strB.AppendFormat("\t public {0}(){{ }}\n", this.StructName);
            //para constructor
            strB.AppendFormat("\t public {0}(", this.StructName);
            foreach (var item in this.Fields)
            {
                strB.AppendFormat("{0} arg_{1},", item.FieldType, item.FieldName);
            }
            strB.Remove(strB.Length - 1, 1);
            strB.Append(")\n");
            strB.Append("\t{\n");

            foreach (var item in this._fields)
            {
                strB.AppendFormat("\t\t {0} = arg_{0};\n", item.FieldName);
            }
            strB.Append("}\n");


            //output decode
            strB.AppendFormat("\t public void xdr_decode(XDRStream xdrs)\n", this.StructName);
            strB.Append("{\n");

            foreach (var item in _fields)
            {
                strB.Append(item.OutputDecode());
            }

            strB.Append("\t}\n\n");

            //output encode
            strB.AppendFormat("\t public void xdr_encode(XDRStream xdrs)\n", this.StructName);
            strB.Append("{\n");

            foreach (var item in _fields)
            {
                strB.Append(item.OutputEncode());
            }

            strB.Append("\t}\n\n");


            strB.Append("\t}\n\n");


            return strB.ToString();
        }




    }

    class FieldInfo
    {
        public string FieldType { get; set; }
        public string FieldName { get; set; }
        public bool IsIntel { get; set; }
        public bool IsFixedArray { get; set; }
        public string FixedArrayCount { get; set; }
        public bool IsDynamicArray { get; set; }
        public string DynamicArrayCountName { get; set; }
        public string Comment { get; set; }
        public bool IsUserDefined { get; set; }
        public string OutputCode()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("\t public {0} {1};//{2}\n", FieldType, FieldName, Comment);

            return strB.ToString();
        }

        public string OutputDecode()
        {
            StringBuilder strB = new StringBuilder();

            switch (this.FieldType)
            {
                case "byte":
                case "sbyte":
                    strB.AppendFormat("{0}=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType);
                    break;
                case "byte[]":
                case "sbyte[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.FixedArrayCount);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.DynamicArrayCountName);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.Append("}\n");
                    }
                    break;

                case "short":
                case "ushort":
                case "int":
                case "uint":
                case "long":
                case "ulong":
                case "float":
                case "double":
                    if (this.IsIntel)
                        strB.AppendFormat("{0}=xdrs.xdr_decode_{1}_intel();\n", this.FieldName, this.FieldType);
                    else
                        strB.AppendFormat("{0}=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType);
                    break;
                case "short[]":
                case "ushort[]":
                case "int[]":
                case "uint[]":
                case "long[]":
                case "ulong[]":
                case "float[]":
                case "double[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.FixedArrayCount);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        if (this.IsIntel)
                            strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}_intel();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        else
                            strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.DynamicArrayCountName);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        if (this.IsIntel)
                            strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}_intel();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        else
                            strB.AppendFormat("{0}[i_v]=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.Append("}\n");
                    }
                    break;


                case "HHJT_STRING":
                    strB.AppendFormat("{0} = new HHJT_STRING({1});\n", this.FieldName, this.FixedArrayCount);
                    strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    break;
                case "HHJT_BCD":
                    strB.AppendFormat("{0} = new HHJT_BCD({1});\n", this.FieldName, this.FixedArrayCount);
                    strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    break;
                default:
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.FixedArrayCount);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v]=new {1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.AppendFormat("{0}[i_v].xdr_decode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.DynamicArrayCountName);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v]=new {1}();\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2));
                        strB.AppendFormat("{0}[i_v].xdr_decode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else
                    {
                        strB.AppendFormat("{0}=new {1}();\n", this.FieldName, this.FieldType);
                        strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    }
                    break;
            }


            return strB.ToString();
        }

        public string OutputEncode()
        {
            StringBuilder strB = new StringBuilder();

            switch (this.FieldType)
            {
                case "byte":
                case "sbyte":
                    strB.AppendFormat("{0}.xdr_encode(xdrs);\n", this.FieldName);
                    break;
                case "byte[]":
                case "sbyte[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    break;

                case "short":
                case "ushort":
                case "int":
                case "uint":
                case "long":
                case "ulong":
                case "float":
                case "double":
                    if (this.IsIntel)
                        strB.AppendFormat("{0}.xdr_encode_intel(xdrs);\n", this.FieldName);
                    else
                        strB.AppendFormat("{0}.xdr_encode(xdrs);\n", this.FieldName);
                    break;
                case "short[]":
                case "ushort[]":
                case "int[]":
                case "uint[]":
                case "long[]":
                case "ulong[]":
                case "float[]":
                case "double[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        if (IsIntel)
                            strB.AppendFormat("{0}[i_v].xdr_encode_intel(xdrs);\n", this.FieldName);
                        else
                            strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);

                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        if (IsIntel)
                            strB.AppendFormat("{0}[i_v].xdr_encode_intel(xdrs);\n", this.FieldName);
                        else
                            strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    break;


                case "HHJT_STRING":
                    strB.AppendFormat("{0}.xdr_encode(xdrs);\n", this.FieldName);
                    break;
                case "HHJT_BCD":
                    strB.AppendFormat("{0}.xdr_encode(xdrs);\n", this.FieldName);
                    break;
                default:
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_encode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else
                    {
                        strB.AppendFormat("{0}.xdr_encode(xdrs);\n", this.FieldName);
                    }
                    break;
            }


            return strB.ToString();
        }

    }

}
