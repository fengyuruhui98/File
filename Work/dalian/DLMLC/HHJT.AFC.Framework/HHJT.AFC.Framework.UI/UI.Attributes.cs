using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace HHJT.AFC.Framework.UI
{
    /// <summary>
    /// 命令自定义特性
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true, Inherited = false)]
    public class AttrMethodAttribute : System.Attribute
    {
        public AttrMethodAttribute(string dName)
        {
            FunctionDisplayName = dName;
        }

        public string FunctionDisplayName { get; set; }//显示名称
        public string AccelerateKey { get; set; }//Q R [0,9],允许使用
        public Button ButtonCtrl { get; set; }
        //public ICommand Cmd { get; set; }
    }

    public enum UI_INPUT_TYPE
    {
        TEXTBOX,
        COMBOBOX,
        COLORCOMBOBOX,
        CHECKCOMBOBOX,      //* add by qinyy(2012/9/6)
        DATE,
        IP,
        DATETIME,
        DATASET,             //* add by qinyy
        CHECKBOX,
        PASSWORDBOX,     //add by liuw(20120921)
    }


    public class UI_LAYOUT
    {
        public const int INPUT_QUERY_CONTROL_WIDTH = 120;
        public const int INPUT_QUERY_LABEL_WIDTH = 150;
        public const int INPUT_QUERY_DOCKPANEL_WIDTH = 280;
        public const int LABEL_FROM_TO_WIDTH = 5;
        public const int DISTANCE_GROUP = 25;                   //* 这个方法（值）不太好，至少要设置成可以随窗口变化的！！（2012-5-22）
    }

    /// <summary>
    /// 查询界面自定义特性
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true, Inherited = false)]
    public class AttrQueryPropertyAttribute : System.Attribute
    {
        public AttrQueryPropertyAttribute(string displayName)
        {
            DisplayName = displayName;
        }

        public UI_INPUT_TYPE QueryInputType { get; set; }//查询输入类型
        public string DisplayName { get; set; }//显示名称
        public string PropertyName { get; set; }//属性名称
        public string PropertyType { get; set; }//属性类型

        //public bool IsKey { get; set; }//是否主键
        //public bool IsForeignKey { get; set; }//是否外键
        //public string ForeignTableName { get; set; }
        //public string TableColumnName { get; set; }

        public Label DisplayLable { get; set; }//显示信息
        public Control QueryControl { get; set; }
        public string ComboDatasource { get; set; }//共用ComboboxDatasource

        //public bool IsQueryItem { get; set; }//是否查询项

        //public bool IsDatagridColumn { get; set; }//是否显示在Datagrid中

        //public bool IsOperationItem { get; set; }//是否数据操作项目
        //public bool IsNotUpdateItem { get; set; }//是否可更新
        //public UI_INPUT_TYPE OperationInputType { get; set; }//操作输入类型
        //public Control OperationControl { get; set; }
        //public int ControlHeight { get; set; }//非标准控件高度

        public bool IsValidate { get; set; }//是否验证
        public string ValidateRexStr { get; set; }//验证正则表达式
        public string ValidatePrompt { get; set; }//验证不通过提示信息

        //public bool ValidateCheck()
        //{
        //    if (IsValidate == false && ValidateRexStr == null) return true;
        //    bool flag = true;
        //    if (OperationInputType == UIInputType.InputTextBox)
        //    {
        //        flag = CommonFuncs.IsMacthRegExp((OperationControl as TextBox).Text.Trim(), ValidateRexStr);
        //        if (flag == false)
        //        {
        //            MessageBox.Show(ValidatePrompt, "错误", MessageBoxButton.OK, MessageBoxImage.Warning);
        //            OperationControl.Focus();
        //        }
        //    }
        //    else if (OperationInputType == UIInputType.InputComboBox)
        //    {
        //        if ((OperationControl as ComboBox).SelectedItem == null)
        //        {
        //            flag = false;
        //            MessageBox.Show(ValidatePrompt, "错误", MessageBoxButton.OK, MessageBoxImage.Warning);
        //            OperationControl.Focus();
        //        }
        //    }
        //    return flag;
        //}

        //public object PropertyValue { get; set; }
    }

    public enum DataConverterType
    {
        Date = 1,
        DateTime,
        Currency,

    }

    /// <summary>
    /// 数据界面自定义特性
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true, Inherited = false)]
    public class AttrDataPropertyAttribute : System.Attribute
    {
        public AttrDataPropertyAttribute(string displayName)
        {
            DisplayName = displayName;
        }

        public UI_INPUT_TYPE InputType { get; set; }//查询输入类型
        public string DisplayName { get; set; }//显示名称
        public string PropertyName { get; set; }//属性名称
        public string PropertyType { get; set; }//属性类型

        public Label DisplayLable { get; set; }//显示信息
        public Control InputControl { get; set; }
        public string ComboDatasource { get; set; }//共用ComboboxDatasource

        public bool Disabled { get; set; }

        public bool IsNotDatagridColumn { get; set; }//是否显示在Datagrid中
        public bool IsCellCentered { get; set; }//Datagrid Cell中是否水平居中

        //public bool IsOperationItem { get; set; }//是否数据操作项目
        public bool IsNotOperateItem { get; set; }//是否数据操作性
        public bool IsNotUpdateItem { get; set; }//是否可更新
        //public UI_INPUT_TYPE OperationInputType { get; set; }//操作输入类型
        //public Control OperationControl { get; set; }
        public int ControlHeight { get; set; }//非标准控件高度

        public bool IsValidate { get; set; }//是否验证
        public string ValidateRexStr { get; set; }//验证正则表达式
        public string ValidatePrompt { get; set; }//验证不通过提示信息

        public int OrderByIndex { get; set; }//字段排序用
        public bool IsNumericFormat { get; set; }//字段数字需要格式化

        public bool IsEditable { get; set; }

        public DataConverterType DataConvertType { get; set; }
        public bool IsEnterKey { get; set; }
        //public bool ValidateCheck()
        //{
        //    if (IsValidate == false && ValidateRexStr == null) return true;
        //    bool flag = true;
        //    if (OperationInputType == UIInputType.InputTextBox)
        //    {
        //        flag = CommonFuncs.IsMacthRegExp((OperationControl as TextBox).Text.Trim(), ValidateRexStr);
        //        if (flag == false)
        //        {
        //            MessageBox.Show(ValidatePrompt, "错误", MessageBoxButton.OK, MessageBoxImage.Warning);
        //            OperationControl.Focus();
        //        }
        //    }
        //    else if (OperationInputType == UIInputType.InputComboBox)
        //    {
        //        if ((OperationControl as ComboBox).SelectedItem == null)
        //        {
        //            flag = false;
        //            MessageBox.Show(ValidatePrompt, "错误", MessageBoxButton.OK, MessageBoxImage.Warning);
        //            OperationControl.Focus();
        //        }
        //    }
        //    return flag;
        //}

        //public object PropertyValue { get; set; }


        public string GroupBoxName { get; set; }
        public bool IsDataGridGroup { get; set; }
        public Type DataGridItemSourceType { get; set; }


    }
}
