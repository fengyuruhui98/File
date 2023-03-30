using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HHJT.AFC.Base.OracleDBI
{
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true, Inherited = false)]
    public class ColumnInfoAttribute : System.Attribute
    {
        public ColumnInfoAttribute(string cName)
        {
            ColumnName = cName;
        }

        public bool IsKey { get; set; }//是否主键
        //public bool IsForeignKey { get; set; }//是否外键
        //public string ForeignTableName { get; set; }//引用表名
        public string ColumnName { get; set; }//字段名称
        public string PropertyName { get; set; }
        public string PropertyType { get; set; }
        public bool IsFuzzyMatch { get; set; }//是否模糊匹配
        public bool IsNotUpdateItem { get; set; }//不可更新
        public bool IsDateTime { get; set; }
    }

    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true, Inherited = false)]
    public class TableInfoAttribute : System.Attribute
    {
        public TableInfoAttribute(string dName)
        {
            TableName = dName;
        }

        public string TableName { get; set; }//表名
    }


    public class ValueDisplayPair<T>
    {
        public T Value { get; set; }
        public string Display { get; set; }
    }

    public class IntDisplayPair
    {
        public int Value { get; set; }
        public string Display { get; set; }
    }

    public class StringDisplayPair
    {
        public string Value { get; set; }
        public string Display { get; set; }
    }

}
