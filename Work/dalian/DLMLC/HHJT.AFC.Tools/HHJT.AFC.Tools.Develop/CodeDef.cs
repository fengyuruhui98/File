using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HHJT.AFC.Base.API;

namespace HHJT.AFC.Tools.Develop
{
    class CodeBase
    {
        public string Defination { get; set; }
        public string Comment { get; set; }
        public string ItemType { get; set; }
        public string InputType { get; set; }
        public string DBColName { get; set; }
        protected string GetProp()
        {
            var str = Defination.Substring(Defination.IndexOfAny(new char[] { '_' }) + 1);
            StringBuilder strB = new StringBuilder();
            strB.Append(str.Substring(1));
            strB.Insert(0, Char.ToUpper(str.ElementAt(0)));
            return strB.ToString();
        }
    }

    class CodeCommand : CodeBase
    {
        public string Name { get; set; }
        public string AccKey { get; set; }

        private string _funName;



        public string GetFunName()
        {
            var index = Defination.IndexOf("command");
            return Defination.Substring(index + 7);
        }

        public string OutDef()
        {
            return string.Format("protected RelayCommand {0}; //{1}\n", Defination, Comment);
        }



        public string OutProp()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("[AttrMethod(\"{0}\", AccelerateKey = \"{1}\")]\n", Name, AccKey);
            string strC = GetProp();

            strB.AppendFormat("public ICommand {0}\n", strC);
            strB.Append("{\n");
            strB.Append("get\n");
            strB.Append("{\n");

            strB.AppendFormat("if ({0} == null)\n", Defination);
            strB.Append("{\n");
            strB.AppendFormat("{0} = new RelayCommand(\n", Defination);
            _funName = GetFunName();
            strB.AppendFormat("param => this.{0}(),\n", _funName);
            strB.AppendFormat("param => this.Can{0}()\n", _funName);
            strB.Append(");\n");
            strB.Append("}\n");
            strB.AppendFormat("return  {0};\n", Defination);
            strB.Append("}\n");
            strB.Append("}\n");

            return strB.ToString();
        }

        public string OutMethod()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("private void {0}()\n", _funName);
            strB.Append("{\n");
            strB.Append("\n");
            strB.Append("}\n");

            strB.AppendFormat("private bool Can{0}()\n", _funName);
            strB.Append("{\n");
            strB.Append("return true;\n");
            strB.Append("}\n");

            return strB.ToString();
        }
    }

    class CodeMQuery : CodeBase
    {

        public string OutputCode()
        {
            string strDB = string.Format("[ColumnInfo(\"{0}\")]\n", DBColName);
            string strProp = string.Format("public {0} {1} {{get; set;}}//{2}\n\n", ItemType, Defination, Comment);
            return strDB + strProp;
        }
    }

    class CodeVMQuery : CodeBase
    {
        public string DisplayName { get; set; }

        public string OutputCode()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("\n[AttrQueryProperty(\"{0}\", QueryInputType = UI_INPUT_TYPE.{1}", DisplayName, InputType);
            if (InputType == "COMBOBOX")
            {
                strB.AppendFormat(",ComboDatasource = \"DS_{0}\")]\n", Defination);
            }
            else
            {
                strB.Append(")]\n");
            }

            strB.AppendFormat("public {0} {1}\n", ItemType, Defination);
            strB.Append("{\n");
            strB.AppendFormat("get {{ return _dataQuery.{0};}}\n", Defination);
            strB.Append("set\n");
            strB.Append("{\n");
            strB.AppendFormat("if (value == _dataQuery.{0}) return;\n", Defination);
            strB.AppendFormat(" _dataQuery.{0} = value;\n", Defination);
            strB.AppendFormat(" base.OnPropertyChanged(\"{0}\");\n", Defination);
            strB.Append("}\n");
            strB.Append("}\n");
            if (InputType == "COMBOBOX")
            {
                strB.AppendFormat("public KeyValuePair<string,string>[] DS_{0}\n", Defination);
                strB.Append("{\n");
                strB.AppendFormat("get {{ return _ds_{0};}}\n", Defination);
                strB.Append("}\n");

            }
            strB.Append("\n");

            return strB.ToString();
        }
    }

    class CodeMResult : CodeBase
    {
        public string OutputCode()
        {
            string strDB = string.Format("[ColumnInfo(\"{0}\")]\n", DBColName);
            string strProp = string.Format("public {0} {1} {{get; set;}}//{2}\n\n", ItemType, Defination, Comment);
            return strDB + strProp;
        }
    }

    class CodeVMResult : CodeBase
    {
        public string DisplayName { get; set; }
        public bool IsNotUpdate { get; set; }
        public bool IsNotGridItem { get; set; }
        public bool IsCentered { get; set; }
        public string OutputCode()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("\n[AttrDataProperty(\"{0}\",InputType = UI_INPUT_TYPE.{1}", DisplayName, InputType);
            if (InputType == "COMBOBOX")
            {
                strB.AppendFormat(",ComboDatasource = \"DSP_{0}\"", Defination);
            }

            if (IsNotGridItem)
            {
                strB.Append(",IsNotDatagridColumn = true ");
            }

            if (IsNotUpdate)
            {
                strB.Append(",IsNotUpdateItem = true ");
            }

            if (IsCentered)
            {
                strB.Append("IsCellCentered = true ");
            }
            strB.Append(")]\n");


            strB.AppendFormat("public {0} {1}\n", ItemType, Defination);
            strB.Append("{\n");
            strB.AppendFormat("get {{ return _data.{0};}}\n", Defination);
            strB.Append("set\n");
            strB.Append("{\n");
            strB.AppendFormat("if (value == _data.{0}) return;\n", Defination);
            strB.AppendFormat(" _data.{0} = value;\n", Defination);
            strB.AppendFormat(" base.OnPropertyChanged(\"{0}\");\n", Defination);
            strB.Append("}\n");
            strB.Append("}\n");
            if (InputType == "COMBOBOX")
            {
                strB.AppendFormat("public KeyValuePair<string,string>[] DSP_{0}\n", Defination);
                strB.Append("{\n");
                strB.AppendFormat("get {{ return _ds_{0};}}\n", Defination);
                strB.Append("}\n");

            }
            strB.Append("\n");

            return strB.ToString();
        }

    }

    class ClassCode
    {
        public string ClassBaseName { get; set; }
        public string DBTTableName { get; set; }
        protected const int Cols = 23;

        private List<CodeMQuery> _mQueries = new List<CodeMQuery>();

        private List<CodeCommand> _vmCommands = new List<CodeCommand>();
        private List<CodeVMQuery> _vmQueries = new List<CodeVMQuery>();

        private List<CodeMResult> _mResults = new List<CodeMResult>();

        private List<CodeVMResult> _vmResults = new List<CodeVMResult>();

        public ClassCode(string[,] strMatrix)
        {
            ClassBaseName = strMatrix[1, 0];
            DBTTableName = strMatrix[2, 0];
            for (int i = 1; i < strMatrix.Length / Cols; i++)
            {
                #region Query
                if (strMatrix[i, 1] != null && DataValidateFuncs.IsNumeric(strMatrix[i, 1]))
                {
                    _mQueries.Add(new CodeMQuery()
                    {
                        Defination = strMatrix[i, 2],
                        ItemType = strMatrix[i, 3],
                        DBColName = strMatrix[i, 4],
                        Comment = strMatrix[i, 5]
                    }
                    );

                    _vmQueries.Add(new CodeVMQuery()
                    {
                        Defination = strMatrix[i, 2],
                        ItemType = strMatrix[i, 3],
                        InputType = strMatrix[i, 6],
                        Comment = strMatrix[i, 5],
                        DisplayName = strMatrix[i, 7]
                    }
                    );
                }
                #endregion

                #region Commands
                if (strMatrix[i, 8] != null && DataValidateFuncs.IsNumeric(strMatrix[i, 8]))
                {
                    _vmCommands.Add(new CodeCommand()
                    {
                        Defination = strMatrix[i, 9],
                        Name = strMatrix[i, 10],
                        AccKey = strMatrix[i, 11],
                        Comment = strMatrix[i, 12]
                    }
                    );
                }
                #endregion

                #region Results
                if (strMatrix[i, 13] != null && DataValidateFuncs.IsNumeric(strMatrix[i, 13]))
                {
                    _mResults.Add(new CodeMResult()
                    {
                        Defination = strMatrix[i, 14],
                        ItemType = strMatrix[i, 15],
                        DBColName = strMatrix[i, 16],
                        Comment = strMatrix[i, 17]
                    }
                    );

                    _vmResults.Add(new CodeVMResult()
                    {
                        Defination = strMatrix[i, 14],
                        ItemType = strMatrix[i, 15],
                        Comment = strMatrix[i, 17],
                        InputType = strMatrix[i, 18],
                        DisplayName = strMatrix[i, 19],
                        IsNotGridItem = strMatrix[i, 20] == "true" ? true : false,
                        IsNotUpdate = strMatrix[i, 21] == "true" ? true : false,
                        IsCentered = strMatrix[i, 22] == "true" ? true : false,
                    }
                    );
                }
                #endregion

            }

        }

        public string OutputM()
        {
            StringBuilder strB = new StringBuilder();

            //M_Query
            strB.AppendFormat("public class M_{0}_Query\n", ClassBaseName);
            strB.Append("{\n\n");

            foreach (var item in _mQueries)
            {
                strB.Append(item.OutputCode());
            }

            strB.AppendFormat("public ObservableCollection<M_{0}_Result> Query()\n", ClassBaseName);
            strB.Append("{\n");
            strB.AppendFormat("ObservableCollection<M_{0}_Result> data = new ObservableCollection<M_{0}_Result>();\n", ClassBaseName);
            strB.Append("StringBuilder strSQL = new StringBuilder();\n");
            strB.Append("//需补充\n");

            strB.AppendFormat("var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(),data,typeof(M_{0}_Result));\n", ClassBaseName);
            strB.Append("if(res.IsFailed)\n");
            strB.Append("{\n");
            strB.Append("MessageBox.Show(res.ErrorMessage);\n");
            strB.Append("}\n");
            strB.Append("return data;\n");
            strB.Append("}\n");

            strB.Append("}\n\n\n");

            //M_Result
            string strDB = string.Format("[TableInfo(\"{0}\")]\n", DBTTableName);
            strB.AppendFormat("public class M_{0}_Result\n", ClassBaseName);
            strB.Append("{\n\n");
            foreach (var item in _mResults)
            {
                strB.Append(item.OutputCode());
            }
            strB.Append("}");

            return strB.ToString();
        }

        public string OutputVM()
        {
            StringBuilder strB = new StringBuilder();
            #region VM_Query
            //-------------VM_Query
            strB.AppendFormat("public class VM_{0}_Query:VM_QueryTemplate\n", ClassBaseName);
            strB.Append("{\n\n");

            //QueryData
            strB.Append("#region DataQuery\n");
            strB.AppendFormat("M_{0}_Query _dataQuery = new M_{0}_Query();\n", ClassBaseName);
            strB.AppendFormat("public M_{0}_Query DataQuery\n", ClassBaseName);
            strB.Append("{\n");
            strB.Append("get { return _dataQuery; }\n");
            strB.Append("set { _dataQuery = value; }\n");
            strB.Append("}\n");
            strB.Append("#endregion\n\n");

            strB.Append("#region DataResult\n");
            strB.AppendFormat("ObservableCollection<VM_{0}_Result> _dataResult = new ObservableCollection<VM_{0}_Result>();\n", ClassBaseName);
            strB.AppendFormat("public ObservableCollection<VM_{0}_Result> DataResult\n", ClassBaseName);
            strB.Append("{\n");
            strB.Append("get { return _dataResult; }\n");
            strB.Append("set { _dataResult = value; }\n");
            strB.Append("}\n\n");
            strB.AppendFormat("public VM_{0}_Result CurrentSelectedDataResult {{get; set;}}\n", ClassBaseName);
            strB.Append("#endregion\n\n");

            strB.Append("#region Combox数据源\n");
            foreach (var item in _vmQueries.Where(d => d.InputType == "COMBOBOX"))
            {
                strB.AppendFormat("private KeyValuePair<string,string>[] _ds_{0};//=需补充\n", item.Defination);
            }
            strB.Append("#endregion\n\n");

            strB.Append("#region View显示绑定\n");
            foreach (var item in _vmQueries)
            {
                strB.Append(item.OutputCode());
            }
            strB.Append("#endregion\n\n");

            strB.Append("#region Query&Reset\n");
            strB.Append("public override void Query()\n");
            strB.Append("{\n");
            strB.Append("_dataResult.Clear();\n");
            strB.Append("var data = _dataQuery.Query();\n");
            strB.Append("foreach(var item in data)\n");
            strB.Append("{\n");
            strB.AppendFormat("_dataResult.Add(new VM_{0}_Result(item));\n", ClassBaseName);
            strB.Append("}\n");
            strB.Append("}\n\n");

            strB.Append("public override bool CanQuery()\n");
            strB.Append("{\n");
            strB.Append("return true;//需补充\n");
            strB.Append("}\n\n");

            strB.Append("public override void Reset()\n");
            strB.Append("{\n");
            strB.Append("AutoUI.ResetQueryInput(Props);\n");
            strB.Append("}\n\n");

            strB.Append("public override bool CanReset()\n");
            strB.Append("{\n");
            strB.Append("return true;//需补充\n");
            strB.Append("}\n\n");

            strB.Append("#endregion\n\n");

            strB.AppendFormat("#region Commands\n");
            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutDef());
            }

            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutProp());
            }

            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutMethod());
            }
            strB.AppendFormat("#endregion\n");



            strB.Append("}\n\n\n");
            #endregion

            #region VM_Result
            //-------------VM_Result
            strB.AppendFormat("public class VM_{0}_Result:ViewModelBase\n", ClassBaseName);
            strB.Append("{\n\n");

            //QueryData
            strB.Append("#region DataResult\n");
            strB.AppendFormat("M_{0}_Result _data ;\n", ClassBaseName);
            strB.AppendFormat("public M_{0}_Result Data\n", ClassBaseName);
            strB.Append("{\n");
            strB.Append("get { return _data; }\n");
            strB.Append("set { _data = value; }\n");
            strB.Append("}\n");
            strB.Append("#endregion\n\n");

            //构造函数
            strB.Append("#region Constructor\n");
            strB.AppendFormat("public VM_{0}_Result()\n", ClassBaseName);
            strB.Append("{\n");
            strB.AppendFormat("_data = new M_{0}_Result();\n", ClassBaseName);
            strB.Append("}\n");
            strB.AppendFormat("public VM_{0}_Result(M_{0}_Result d)\n", ClassBaseName);
            strB.Append("{\n");
            strB.Append("_data = d;\n");
            strB.Append("}\n");
            strB.Append("#endregion\n\n");

            //ComboBox数据源
            strB.Append("#region Combox数据源\n");
            foreach (var item in _vmResults.Where(d => d.InputType == "COMBOBOX"))
            {
                strB.AppendFormat("private static KeyValuePair<string,string>[] _ds_{0};//=需补充\n", item.Defination);
            }
            foreach (var item in _vmResults.Where(d => d.InputType == "COMBOBOX"))
            {
                strB.AppendFormat("public static KeyValuePair<string,string>[] DS_{0}", item.Defination);
                strB.Append("{\n");
                strB.AppendFormat("get {{ return _ds_{0};}}", item.Defination);
                strB.Append("}\n");
            }
            strB.Append("#endregion\n\n");

            //
            strB.Append("#region View显示绑定\n");
            foreach (var item in _vmResults)
            {
                strB.Append(item.OutputCode());
            }
            strB.Append("#endregion\n\n");

            strB.Append("}\n");
            #endregion

            return strB.ToString();

        }
    }

    class CommandCode
    {
        protected const int Cols = 6;
        private List<CodeCommand> _vmCommands = new List<CodeCommand>();
        public CommandCode(string[,] strMatrix)
        {
            for (int i = 1; i < strMatrix.Length / Cols; i++)
            {
                if (strMatrix[i, 1] != null && DataValidateFuncs.IsNumeric(strMatrix[i, 1]))
                {
                    _vmCommands.Add(new CodeCommand
                    {
                        Defination = strMatrix[i, 2],
                        Name = strMatrix[i, 3],
                        AccKey = strMatrix[i, 4],
                        Comment = strMatrix[i, 5]
                    });
                }
            }
        }

        public string OutputCode()
        {
            StringBuilder strB = new StringBuilder();
            strB.AppendFormat("#region Commands\n");
            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutDef());
            }

            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutProp());
            }

            foreach (var item in _vmCommands)
            {
                strB.Append(item.OutMethod());
            }
            strB.AppendFormat("#endregion\n");
            return strB.ToString();
        }

    }

    class PropCode
    {
        protected const int Cols = 11;
        private List<CodeMResult> _mResults = new List<CodeMResult>();
        private List<CodeVMResult> _vmResults = new List<CodeVMResult>();
        public PropCode(string[,] strMatrix)
        {
            for (int i = 1; i < strMatrix.Length / Cols; i++)
            {
                if (strMatrix[i, 1] != null && DataValidateFuncs.IsNumeric(strMatrix[i, 1]))
                {
                    _mResults.Add(new CodeMResult
                    {
                        Defination = strMatrix[i, 2],
                        ItemType = strMatrix[i, 3],
                        DBColName = strMatrix[i, 4],
                        Comment = strMatrix[i, 5]
                    });

                    _vmResults.Add(new CodeVMResult
                    {
                        Defination = strMatrix[i, 2],
                        ItemType = strMatrix[i, 3],
                        InputType = strMatrix[i, 6],
                        DisplayName = strMatrix[i, 7],
                        IsNotGridItem = strMatrix[i, 8] == "true" ? true : false,
                        IsNotUpdate = strMatrix[i, 9] == "true" ? true : false,
                        IsCentered = strMatrix[i, 10] == "true" ? true : false,
                    }
                    );

                }
            }
        }

        public string OutputCodeM()
        {
            StringBuilder strB = new StringBuilder();
            foreach (var item in _mResults)
            {
                strB.Append(item.OutputCode());
            }

            return strB.ToString();
        }

        public string OutputCodeVM()
        {
            StringBuilder strB = new StringBuilder();
            //ComboBox数据源
            strB.Append("#region Combox数据源\n");
            foreach (var item in _vmResults.Where(d => d.InputType == "COMBOBOX"))
            {
                strB.AppendFormat("private static KeyValuePair<string,string>[] _ds_{0};//=需补充\n", item.Defination);
            }
            foreach (var item in _vmResults.Where(d => d.InputType == "COMBOBOX"))
            {
                strB.AppendFormat("private static KeyValuePair<string,string>[] DS_{0}", item.Defination);
                strB.Append("{\n");
                strB.AppendFormat("get {{ return _ds_{0};}}", item.Defination);
                strB.Append("}\n");
            }
            strB.Append("#endregion\n\n");

            //
            strB.Append("#region View显示绑定\n");
            foreach (var item in _vmResults)
            {
                strB.Append(item.OutputCode());
            }
            strB.Append("#endregion\n\n");


            return strB.ToString();
        }
    }
}
