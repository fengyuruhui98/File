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
namespace HHJT.AFC.Tools.Develop
{
    /// <summary>
    /// UCCpp.xaml 的交互逻辑
    /// </summary>
    public partial class UCCpp : UserControl
    {
        public UCCpp()
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

        List<CppStructInfo> _structs = new List<CppStructInfo>();

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
                    _structs.Add(new CppStructInfo
                    {
                        StructName = s,
                        Comment = GetCellContent(xlRange, currentRow, 2)
                    });
                }
                else
                {
                    CppStructInfo curStruct = _structs.Last();
                    CppFieldInfo curField = new CppFieldInfo();
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

        private void btnGenerate_Click(object sender, RoutedEventArgs e)
        {
            int idx = lbxVMList.SelectedIndex;
            if (idx < 0)
            {
                MessageBox.Show("请选择所要转换的类型定义", "提示！", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            ReadExcelSheet(idx);

            string strOutputH = "";
            string strOutputCpp = "";
            foreach (var item in _structs)
            {
                strOutputH += item.OutputCodeH();
                strOutputCpp += item.OutputCodeCpp();
            }
            //tbxOutput.Text = strOutput;
            tbxOutputH.Text = strOutputH;
            tbxOutputCPP.Text = strOutputCpp;
        }
    }

    class CppStructInfo
    {
        public string StructName { get; set; }
        List<CppFieldInfo> _fields = new List<CppFieldInfo>();
        public string Comment { get; set; }
        internal List<CppFieldInfo> Fields
        {
            get { return _fields; }
            set { _fields = value; }
        }

        static string[] _strBasicTypes = { "char", "byte", "short", "ushort", "int", "uint", "long", "ulong", "int64", "uint64", "float", "double" };

        public bool IsBasicType(string strType)
        {
            if (_strBasicTypes.Count(d => d == strType) > 0)
            {
                return true;
            }
            else
                return false;
        }

        public string OutputCodeH()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("//======================================\n");
            strB.AppendFormat("//类    名:{0}:{1}\n", StructName, Comment);
            strB.AppendFormat("//说    明:{0}:{1}\n", StructName, Comment);
            strB.AppendFormat("//生成时间:{0}\n", DateTime.Now.ToString("yyyyMMdd HH:mm:ss"));
            strB.AppendFormat("//======================================\n");
            strB.AppendFormat("class {0}:XDRType //{1}\n", StructName, Comment);
            strB.Append("\t{\n");
            strB.Append("\t public:\n");
            foreach (var item in _fields)
            {
                strB.Append(item.OutputCodeH());
            }
            strB.Append("\n");

            //default constructor
            strB.AppendFormat("\t {0}();\n", this.StructName);
            //para constructor
            strB.AppendFormat("\t {0}(", this.StructName);
            foreach (var item in this.Fields)
            {
                if (item.IsFixedArray && !item.FieldType.Contains("HHJT"))
                {
                    strB.AppendFormat("{0} arg_{1}[],", item.FieldType.Remove(item.FieldType.Length - 2), item.FieldName);
                }
                else if (item.IsDynamicArray)
                {
                    strB.AppendFormat("{0}* arg_{1},", item.FieldType.Remove(item.FieldType.Length - 2), item.FieldName);
                }
                else if (IsBasicType(item.FieldType))
                {
                    strB.AppendFormat("{0} arg_{1},", item.FieldType, item.FieldName);
                }
                else
                {
                    strB.AppendFormat("{0}& arg_{1},", item.FieldType, item.FieldName);
                }
            }
            strB.Remove(strB.Length - 1, 1);
            strB.Append(");\n");
            //copy constructor
            strB.AppendFormat("\t {0}(const {0}& src);\n", this.StructName);
            //deconstroctor
            strB.AppendFormat("\t virtual ~{0}();\n", this.StructName);
            //equal operator
            strB.AppendFormat("\t {0}& operator=(const {0}& src);\n", this.StructName);

            strB.Append("\n\n");

            strB.Append("\t virtual void xdr_decode(XDRStream& xdrs);\n");
            strB.Append("\t virtual void xdr_encode(XDRStream& xdrs);\n");


            strB.Append("\t};\n");

            strB.AppendFormat("//[{0}]-----------------end\n\n", StructName);

            return strB.ToString();
        }

        public string OutputCodeCpp()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("//=======================================\n");
            strB.AppendFormat("//{0} {1}\n", StructName, Comment);
            strB.AppendFormat("//=======================================\n");
            //default constructor
            #region default constructor
            strB.Append("\n//default constructor\n");
            strB.AppendFormat("{0}::{0}() \n", StructName);
            strB.Append("\t{\n");
            foreach (var item in _fields)
            {
                if (item.IsDynamicArray)
                {
                    //new and for loop
                    strB.AppendFormat("{0}=NULL;\n", item.FieldName);

                }
            }

            strB.Append("\n}\n");

            #endregion

            //para constructor
            #region para constructor
            strB.Append("\n//para constructor\n");
            strB.AppendFormat("\t {0}::{0}(", this.StructName);
            foreach (var item in this.Fields)
            {
                if (item.IsFixedArray && !item.FieldType.Contains("HHJT"))
                {
                    strB.AppendFormat("{0} arg_{1}[],", item.FieldType.Remove(item.FieldType.Length - 2), item.FieldName);
                }
                else if (item.IsDynamicArray)
                {
                    strB.AppendFormat("{0}* arg_{1},", item.FieldType.Remove(item.FieldType.Length - 2), item.FieldName);
                }
                else if (IsBasicType(item.FieldType))
                {
                    strB.AppendFormat("{0} arg_{1},", item.FieldType, item.FieldName);
                }
                else
                {
                    strB.AppendFormat("{0}& arg_{1},", item.FieldType, item.FieldName);
                }
            }
            strB.Remove(strB.Length - 1, 1);
            strB.Append(")\n");
            strB.Append("\t{\n"); //函数体
            foreach (var item in this.Fields)
            {
                if (item.IsFixedArray && !item.FieldType.Contains("HHJT"))
                {
                    //for loop
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.FixedArrayCount);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = arg_{0}[i];\n", item.FieldName);
                    strB.Append("\t\t}\n");

                }
                else if (item.IsDynamicArray)
                {
                    //new and for loop
                    strB.AppendFormat("\t {0} = new {1}[{2}];\n", item.FieldName, item.FieldType.Remove(item.FieldType.Length - 2), item.DynamicArrayCountName);
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.DynamicArrayCountName);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = arg_{0}[i];\n", item.FieldName);
                    strB.Append("\t\t}\n");
                }
                else
                {
                    strB.AppendFormat("{0} = arg_{0};\n", item.FieldName);
                }
            }
            strB.Append("\n}\n");
            #endregion

            //copy constructor
            #region copy constructor
            strB.Append("\n//copy constructor\n");
            strB.AppendFormat("{0}::{0}(const {0}& src) \n", StructName);
            strB.Append("\t{\n");

            foreach (var item in _fields)
            {
                if (item.IsFixedArray && !item.FieldType.Contains("HHJT"))
                {
                    //for loop
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.FixedArrayCount);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = src.{0}[i];\n", item.FieldName);
                    strB.Append("\t\t}\n");

                }
                else if (item.IsDynamicArray)
                {
                    //new and for loop
                    strB.AppendFormat("\t {0} = new {1}[{2}];\n", item.FieldName, item.FieldType.Remove(item.FieldType.Length - 2), item.DynamicArrayCountName);
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.DynamicArrayCountName);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = src.{0}[i];", item.FieldName);
                    strB.Append("\t\t}\n");
                }
                else
                {
                    strB.AppendFormat("{0} = src.{0};\n", item.FieldName);
                }
            }
            strB.Append("\n}\n");
            #endregion

            //deconstructor
            #region deconstructor
            strB.Append("\n//deconstructor\n");
            strB.AppendFormat("{0}::~{0}() \n", StructName);
            strB.Append("\t{\n");

            foreach (var item in _fields)
            {
                if (item.IsDynamicArray)
                {
                    //new and for loop
                    strB.AppendFormat("if({0}!=NULL)\n", item.FieldName);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("delete [] {0};\n", item.FieldName);
                    strB.Append("\t\t}\n");
                }

            }
            strB.Append("\n}\n");
            #endregion

            //equal operator
            #region equal operator
            strB.Append("\n//equal operator\n");
            strB.AppendFormat("{0}& {0}::operator=(const {0}& src) \n", StructName);
            strB.Append("\t{\n");
            strB.Append("if(this==&src)\n");
            strB.Append("return (*this);\n");
            foreach (var item in _fields)
            {
                if (item.IsFixedArray && !item.FieldType.Contains("HHJT"))
                {
                    //for loop
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.FixedArrayCount);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = src.{0}[i];\n", item.FieldName);
                    strB.Append("\t\t}\n");

                }
                else if (item.IsDynamicArray)
                {
                    //new and for loop
                    strB.AppendFormat("\t {0} = new {1}[{2}];\n", item.FieldName, item.FieldType.Remove(item.FieldType.Length - 2), item.DynamicArrayCountName);
                    strB.AppendFormat("\t for (int i=0; i<{0}; i++)\n", item.DynamicArrayCountName);
                    strB.Append("\t\t{\n");
                    strB.AppendFormat("{0}[i] = src.{0}[i];\n", item.FieldName);
                    strB.Append("\t\t}\n");
                }
                else
                {
                    strB.AppendFormat("{0} = src.{0};\n", item.FieldName);
                }


            }
            strB.Append("return (*this);\n");
            strB.Append("\n}\n");
            #endregion

            //decode
            strB.Append("\n//decode\n");
            strB.AppendFormat("void {0}::xdr_decode(XDRStream& xdrs) \n", StructName);
            strB.Append("\t{\n");
            foreach (var item in _fields)
            {
                strB.Append(item.OutputDecodeCpp());
            }
            strB.Append("\n}\n");

            //encode
            strB.Append("\n//encode\n");
            strB.AppendFormat("void {0}::xdr_encode(XDRStream& xdrs) \n", StructName);
            strB.Append("\t{\n");
            foreach (var item in _fields)
            {
                strB.Append(item.OutputEncodeCpp());
            }
            strB.Append("\n}\n");
            return strB.ToString();
        }


    }

    class CppFieldInfo
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
        public string OutputCodeH()
        {
            StringBuilder strB = new StringBuilder();
            if (IsFixedArray && !FieldType.Contains("HHJT"))
            {
                strB.AppendFormat("\t {0} {1}[{2}];//{3}\n", FieldType.Remove(FieldType.Length - 2, 2), FieldName, FixedArrayCount, Comment);
            }
            else if (IsDynamicArray)
            {
                strB.AppendFormat("\t {0}* {1};//{2}\n", FieldType.Remove(FieldType.Length - 2, 2), FieldName, Comment);
            }
            else
            {
                strB.AppendFormat("\t {0} {1};//{2}\n", FieldType, FieldName, Comment);
            }

            return strB.ToString();
        }

        public string OutputDecodeCpp()
        {
            StringBuilder strB = new StringBuilder();

            switch (this.FieldType)
            {
                case "byte":
                case "sbyte":
                case "char":
                    strB.AppendFormat("{0}=xdrs.xdr_decode_{1}();\n", this.FieldName, this.FieldType);
                    break;
                case "byte[]":
                case "sbyte[]":
                case "char[]":
                    if (IsFixedArray)
                    {
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
                case "int64":
                case "uint64":
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
                case "int64[]":
                case "uint64[]":
                case "float[]":
                case "double[]":
                    if (IsFixedArray)
                    {
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
                    strB.AppendFormat("{0} = HHJT_STRING({1});\n", this.FieldName, this.FixedArrayCount);
                    strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    break;
                case "HHJT_BCD":
                    strB.AppendFormat("{0} = HHJT_BCD({1});\n", this.FieldName, this.FixedArrayCount);
                    strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    break;
                default:
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_decode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("{0} = new {1}[{2}];\n", this.FieldName, this.FieldType.Remove(FieldType.Length - 2, 2), this.DynamicArrayCountName);
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("{0}[i_v].xdr_decode(xdrs);\n", this.FieldName);
                        strB.Append("}\n");
                    }
                    else
                    {
                        strB.AppendFormat("{0}.xdr_decode(xdrs);\n", this.FieldName);
                    }
                    break;
            }


            return strB.ToString();
        }

        public string OutputEncodeCpp()
        {
            StringBuilder strB = new StringBuilder();

            switch (this.FieldType)
            {
                case "byte":
                case "sbyte":
                case "char":
                    strB.AppendFormat("xdrs.xdr_encode_{0}({1});\n", this.FieldType, this.FieldName);
                    break;
                case "byte[]":
                case "sbyte[]":
                case "char[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        strB.AppendFormat("xdrs.xdr_encode_{0}({1}[i_v]);\n", this.FieldType.Remove(FieldType.Length - 2), this.FieldName);
                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        strB.AppendFormat("xdrs.xdr_encode_{0}({1}[i_v]);\n", this.FieldType.Remove(FieldType.Length - 2), this.FieldName);
                        strB.Append("}\n");
                    }
                    break;

                case "short":
                case "ushort":
                case "int":
                case "uint":
                case "long":
                case "ulong":
                case "int64":
                case "uint64":
                case "float":
                case "double":
                    if (this.IsIntel)
                        strB.AppendFormat("xdrs.xdr_encode_{0}_intel({1});\n", this.FieldType, this.FieldName);
                    else
                        strB.AppendFormat("xdrs.xdr_encode_{0}({1});\n", this.FieldType, this.FieldName);
                    break;
                case "short[]":
                case "ushort[]":
                case "int[]":
                case "uint[]":
                case "long[]":
                case "ulong[]":
                case "int64[]":
                case "uint64[]":
                case "float[]":
                case "double[]":
                    if (IsFixedArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.FixedArrayCount);
                        strB.Append("{\n");
                        if (IsIntel)
                            strB.AppendFormat("xdrs.xdr_encode_{0}_intel({1}[i_v]);\n", this.FieldType, this.FieldName);
                        else
                            strB.AppendFormat("xdrs.xdr_encode_{0}({1}[i_v]);\n", this.FieldType, this.FieldName);


                        strB.Append("}\n");
                    }
                    else if (IsDynamicArray)
                    {
                        strB.AppendFormat("for (int i_v = 0; i_v<{0}; i_v++)\n", this.DynamicArrayCountName);
                        strB.Append("{\n");
                        if (IsIntel)
                            strB.AppendFormat("xdrs.xdr_encode_{0}_intel({1}[i_v]);\n", this.FieldType, this.FieldName);
                        else
                            strB.AppendFormat("xdrs.xdr_encode_{0}({1}[i_v]);\n", this.FieldType, this.FieldName);
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
