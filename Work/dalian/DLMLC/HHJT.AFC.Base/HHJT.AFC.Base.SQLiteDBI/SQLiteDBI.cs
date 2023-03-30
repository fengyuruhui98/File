using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RefSQLite = System.Data.SQLite;
using System.ComponentModel;
using System.Reflection;

namespace HHJT.AFC.Base.SQLiteDBI
{
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true, Inherited = false)]
    public class SQLiteColumnInfoAttribute : System.Attribute
    {
        public SQLiteColumnInfoAttribute(string cName)
        {
            ColumnName = cName;
        }

        public string ColumnName { get; set; }//字段名称
        public string PropertyName { get; set; }
        public string PropertyType { get; set; }
        //        public bool IsFuzzyMatch { get; set; }//是否模糊匹配
        //       public bool IsNotUpdateItem { get; set; }//不可更新
        //      public bool IsDateTime { get; set; }
    }

    public class SQLiteResult
    {
        public bool IsSuccess { get; set; }
        public string ErrorMessage { get; set; }
        public SQLiteResult()
        {
            IsSuccess = true;
        }
    }

    public class SQLiteDB
    {
        private RefSQLite.SQLiteConnection _conn;
        private RefSQLite.SQLiteCommand _command;
        private RefSQLite.SQLiteDataReader _reader;

        private bool _isOpen;
        private string _strConn;

        public string DBFile { get; set; }

        //public string DBPassWord { get; set; }

        public SQLiteDB(string dbFile, string dbPwd)
        {
            DBFile = dbFile;
            //DBPassWord = dbPwd;
            _strConn = string.Format("data source={0};password={1}", dbFile, dbPwd);
        }

        public SQLiteDB(string dbFile)
        {
            _strConn = string.Format("data source={0}", dbFile);
        }



        /// <summary>
        /// 创建数据库文件
        /// </summary>
        /// <returns></returns>
        public SQLiteResult CreateDBFile()
        {
            SQLiteResult ret = new SQLiteResult();

            if (System.IO.File.Exists(DBFile))
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = "文件: " + DBFile + "已经存在";
                return ret;
            }

            try
            {
                RefSQLite.SQLiteConnection.CreateFile(DBFile);

            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;

        }

        /// <summary>
        /// 打开数据库
        /// </summary>
        /// <returns></returns>
        public SQLiteResult OpenDB()
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }

        /// <summary>
        /// 关闭数据库
        /// </summary>
        /// <returns></returns>
        public SQLiteResult CloseDB()
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                if (_isOpen)
                {
                    _conn.Close();
                    _isOpen = false;
                }

            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }

        /// <summary>
        /// 执行SQL语句
        /// </summary>
        /// <param name="strSQL"></param>
        /// <returns></returns>
        public SQLiteResult ExecuteCommand(string strSQL)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _command.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }


        public SQLiteResult DB_ExecuteCommand(string strSQL)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _command.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen)
                    _conn.Close();
            }

            return ret;
        }

        /// <summary>
        /// 单数据
        /// </summary>
        /// <param name="strSQL"></param>
        /// <param name="obj"></param>
        /// <returns></returns>
        public SQLiteResult GetScalarObj(string strSQL, out object obj)
        {
            SQLiteResult ret = new SQLiteResult();
            obj = null;
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                obj = _command.ExecuteScalar();

            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }
        public SQLiteResult DB_GetScalarObj(string strSQL, out object obj)
        {
            SQLiteResult ret = new SQLiteResult();
            obj = null;
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                obj = _command.ExecuteScalar();

            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen)
                    _conn.Close();
            }

            return ret;
        }
        /// <summary>
        /// 读取整数列表 单列
        /// </summary>
        /// <param name="strSQL"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        public SQLiteResult RetrieveToIntArray(string strSQL, System.Collections.IList data)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                data.Clear();
                while (_reader.Read())
                {
                    data.Add(_reader.GetInt32(0));
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }
        public SQLiteResult DB_RetrieveToIntArray(string strSQL, System.Collections.IList data)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                data.Clear();
                while (_reader.Read())
                {
                    data.Add(_reader.GetInt32(0));
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;
        }
        /// <summary>
        /// 读取字符串列表 单列
        /// </summary>
        /// <param name="strSQL"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        public SQLiteResult RetrieveToStringArray(string strSQL, System.Collections.IList data)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                data.Clear();
                while (_reader.Read())
                {
                    data.Add(_reader.GetString(0));
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;
        }
        public SQLiteResult DB_RetrieveToStringArray(string strSQL, System.Collections.IList data)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                data.Clear();
                while (_reader.Read())
                {
                    data.Add(_reader.GetString(0));
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;
        }
        static Dictionary<string, List<SQLiteColumnInfoAttribute>> typeColumnInfos = new Dictionary<string, List<SQLiteColumnInfoAttribute>>();

        /// <summary>
        /// 首次获取数据对应的表结构描述信息
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        private static List<SQLiteColumnInfoAttribute> GetColumnInfoPropsFirsttime(Type type)
        {
            List<SQLiteColumnInfoAttribute> props = new List<SQLiteColumnInfoAttribute>();
            foreach (PropertyInfo info in type.GetProperties())
            {
                foreach (SQLiteColumnInfoAttribute a in info.GetCustomAttributes(typeof(SQLiteColumnInfoAttribute), true))
                {
                    a.PropertyName = info.Name;
                    a.PropertyType = info.PropertyType.Name;
                    props.Add(a);
                }
            }
            return props;
        }
        /// <summary>
        /// 获取数据对应的表结构描述信息
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static List<SQLiteColumnInfoAttribute> GetColumnInfos(Type type)
        {
            if (!typeColumnInfos.ContainsKey(type.FullName))
                typeColumnInfos.Add(type.FullName, GetColumnInfoPropsFirsttime(type));
            return typeColumnInfos[type.FullName];
        }

        public SQLiteResult RetrieveToObjects(string strSQL, System.Collections.IList data, Type type)
        {

            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<SQLiteColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    count = 0;
                    //* count是指DB中字段的顺序
                    foreach (var item in selectProps)
                    {
                        itemProperty = properties.Find(item.PropertyName, false);
                        if (!DBNull.Value.Equals(_reader[count]))
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(t, _reader[count].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(t, (short)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(t, (int)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(t, _reader.GetDouble(count));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(t, _reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(t, _reader.GetDateTime(count));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(t, (byte[])_reader[count]);
                        }
                        count++;
                    }
                    data.Add(t);
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;


        }

        public SQLiteResult DB_RetrieveToObjects(string strSQL, System.Collections.IList data, Type type)
        {

            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<SQLiteColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    count = 0;
                    //* count是指DB中字段的顺序
                    foreach (var item in selectProps)
                    {
                        itemProperty = properties.Find(item.PropertyName, false);
                        if (!DBNull.Value.Equals(_reader[count]))
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(t, _reader[count].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(t, (short)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(t, (int)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(t, _reader.GetDouble(count));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(t, _reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(t, _reader.GetDateTime(count));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(t, (byte[])_reader[count]);
                        }
                        count++;
                    }
                    data.Add(t);
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;


        }

        public SQLiteResult RetrieveToSingleObject(string strSQL, object data, Type type)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<SQLiteColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {

                    count = 0;

                    foreach (var item in selectProps)
                    {
                        itemProperty = properties.Find(item.PropertyName, false);
                        if (!DBNull.Value.Equals(_reader[count]))
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(data, _reader[count].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(data, (short)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(data, (int)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(data, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(data, _reader.GetDouble(count));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(data, _reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(data, _reader.GetDateTime(count));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(data, (byte[])_reader[count]);

                        }
                        count++;
                    }
                    break;
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;

        }
        public SQLiteResult DB_RetrieveToSingleObject(string strSQL, object data, Type type)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<SQLiteColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {

                    count = 0;

                    foreach (var item in selectProps)
                    {
                        itemProperty = properties.Find(item.PropertyName, false);
                        if (!DBNull.Value.Equals(_reader[count]))
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(data, _reader[count].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(data, (short)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(data, (int)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(data, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(data, _reader.GetDouble(count));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(data, _reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(data, _reader.GetDateTime(count));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(data, (byte[])_reader[count]);

                        }
                        count++;
                    }
                    break;
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;

        }
        public SQLiteResult RetrieveToObjectsByCol(string strSQL, System.Collections.IList data, Type type)
        {

            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;

                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        itemProperty = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]) && itemProperty != null)
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(t, _reader[i].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(t, (short)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(t, (int)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(t, _reader.GetDouble(i));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(t, _reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(t, _reader.GetDateTime(i));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(t, (byte[])_reader[i]);
                        }

                    }
                    data.Add(t);
                }

            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;


        }
        public SQLiteResult DB_RetrieveToObjectsByCol(string strSQL, System.Collections.IList data, Type type)
        {

            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;

                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        itemProperty = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]) && itemProperty != null)
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(t, _reader[i].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(t, (short)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(t, (int)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(t, _reader.GetDouble(i));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(t, _reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(t, _reader.GetDateTime(i));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(t, (byte[])_reader[i]);
                        }

                    }
                    data.Add(t);
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;


        }
        public SQLiteResult RetrieveToSingleObjectByCol(string strSQL, object data, Type type)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;

                while (_reader.Read())
                {

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        itemProperty = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]) && itemProperty != null)
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(data, _reader[i].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(data, (short)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(data, (int)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(data, (long)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(data, _reader.GetDouble(i));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(data, _reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(data, _reader.GetDateTime(i));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(data, (byte[])_reader[i]);

                        }
                    }
                    break;
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;

        }
        public SQLiteResult DB_RetrieveToSingleObjectByCol(string strSQL, object data, Type type)
        {
            SQLiteResult ret = new SQLiteResult();
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;

                while (_reader.Read())
                {

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        itemProperty = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]) && itemProperty != null)
                        {
                            if (itemProperty.PropertyType == typeof(string))
                                itemProperty.SetValue(data, _reader[i].ToString());
                            else if (itemProperty.PropertyType == typeof(short) || itemProperty.PropertyType == typeof(short?))
                                itemProperty.SetValue(data, (short)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(int) || itemProperty.PropertyType == typeof(int?))
                                itemProperty.SetValue(data, (int)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(data, (long)_reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(data, _reader.GetDouble(i));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(data, _reader.GetDecimal(i));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(data, _reader.GetDateTime(i));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(data, (byte[])_reader[i]);

                        }
                    }
                    break;
                }
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;

        }

        public SQLiteResult RetrieveToKVs(string strSQL, out KeyValuePair<string, string>[] data)
        {
            SQLiteResult ret = new SQLiteResult();
            data = null;
            try
            {
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                List<KeyValuePair<string, string>> listData = new List<KeyValuePair<string, string>>();
                while (_reader.Read())
                {
                    listData.Add(
                      new KeyValuePair<string, string>(
                          _reader.GetString(0), _reader.GetString(1)
                          )
                        );


                }
                data = listData.ToArray();
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }

            return ret;

        }
        public SQLiteResult DB_RetrieveToKVs(string strSQL, out KeyValuePair<string, string>[] data)
        {
            SQLiteResult ret = new SQLiteResult();
            data = null;
            try
            {
                _conn = new RefSQLite.SQLiteConnection();
                _conn.ConnectionString = _strConn;
                _conn.Open();
                _isOpen = true;
                _command = new RefSQLite.SQLiteCommand(strSQL, _conn);
                _reader = _command.ExecuteReader();
                List<KeyValuePair<string, string>> listData = new List<KeyValuePair<string, string>>();
                while (_reader.Read())
                {
                    listData.Add(
                      new KeyValuePair<string, string>(
                          _reader.GetString(0), _reader.GetString(1)
                          )
                        );


                }
                data = listData.ToArray();
                _reader.Close();
            }
            catch (Exception ex)
            {
                ret.IsSuccess = false;
                ret.ErrorMessage = ex.Message;
            }
            finally
            {
                if (_isOpen) _conn.Close();
            }

            return ret;

        }

    }
}
