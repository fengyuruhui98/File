using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Xml;

namespace HHJT.AFC.Base.OracleDBI
{
    public class OracleResult
    {
        public bool IsFailed;
        public string ErrorMessage;
        public static implicit operator bool(OracleResult res)
        {
            return !res.IsFailed;
        }
    }

    public class OracleDB
    {
        private static string _staticStrConn;
        private string _strConn;

        private OracleConnection _conn;

        public OracleConnection Conn
        {
            get { return _conn; }
        }
        private OracleCommand _command;
        private OracleDataReader _reader;

        private bool _isOpen;

        public bool IsOpen
        {
            get { return _isOpen; }
        }

        public static string GetElementByName(string xmlFileName, string tagName)
        {
            try
            {
                string result = null;
                XmlDocument doc = new XmlDocument();
                doc.Load(xmlFileName);
                XmlNodeList nodes = doc.GetElementsByTagName(tagName);
                foreach (XmlNode v in nodes)
                {
                    result = v.InnerText;
                }
                return result;
            }
            catch
            {
                throw new Exception("打开文件错误");
            }
            //return null;
        }

        private const string STR_DES_KEY = "tiantian";
        private const string STR_DES_IV = "descarts";
        public static string DESDecrypt(string strCipher)
        {
            //把密钥转换成字节数组
            byte[] bytesDESKey = ASCIIEncoding.ASCII.GetBytes(STR_DES_KEY);
            //把向量转换成字节数组
            byte[] bytesDESIV = ASCIIEncoding.ASCII.GetBytes(STR_DES_IV);
            //把密文转换成字节数组
            //byte[] bytesCipher = UnicodeEncoding.Unicode.GetBytes(strCipher);

            List<byte> bytes = new List<byte>();

            for (int i = 0; i < strCipher.Length - 1; i += 2)
            {
                bytes.Add(byte.Parse(strCipher.Substring(i, 2), global::System.Globalization.NumberStyles.HexNumber));
            }


            byte[] bytesCipher = bytes.ToArray();


            //声明1个新的DES对象
            DESCryptoServiceProvider desDecrypt = new DESCryptoServiceProvider();
            //开辟一块内存流，并存放密文字节数组
            MemoryStream msDecrypt = new MemoryStream(bytesCipher);
            //把内存流对象包装成解密流对象
            CryptoStream csDecrypt = new CryptoStream(msDecrypt, desDecrypt.CreateDecryptor(bytesDESKey, bytesDESIV), CryptoStreamMode.Read);
            //把解密流对象包装成读出流对象
            StreamReader srDecrypt = new StreamReader(csDecrypt);
            //明文=读出流的读出内容
            string strPlainText = srDecrypt.ReadLine();
            //读出流关闭
            srDecrypt.Close();
            //解密流关闭
            csDecrypt.Close();
            //内存流关闭
            msDecrypt.Close();
            //返回明文
            return strPlainText;
        }

        static OracleDB()
        {
            try
            {
                string strIP = GetElementByName("DBConfig.xml", "ServerIP");
                string strPort = GetElementByName("DBConfig.xml", "ServerPort");
                string strServiceName = GetElementByName("DBConfig.xml", "ServiceName");
                string strUserID = GetElementByName("DBConfig.xml", "UserID");
                string strPassword = GetElementByName("DBConfig.xml", "Password");

                _staticStrConn = string.Format("Data Source=(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP) (HOST = {0})(PORT = {1})))(CONNECT_DATA =(SERVICE_NAME = {2}))); User ID={3};Password={4}",
    strIP, strPort, strServiceName, strUserID, strPassword);
 
 
                /*
                strIP = "172.210.200.1";
                strPort = "1521";
                strServiceName = "orcl";
                strUserID = "mlcdb";
                strPassword = "mlcdb";
               
                strIP = "10.103.57.102";
                strPort = "1521";
                strServiceName = "orcl";
                strUserID = "dlmlcdb";
                strPassword = "dlmlcdb";
                _staticStrConn = string.Format("Data Source=(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP) (HOST = {0})(PORT = {1})))(CONNECT_DATA =(SERVICE_NAME = {2}))); User ID={3};Password={4}",
                    strIP, strPort, strServiceName, strUserID, strPassword); */
            }
            catch (Exception)
            {

            }
        }

        #region 静态类型信息

        //为提高语句生成效率，讲用户使用过的类型数据库描述信息记录在静态Dictionary中
        static Dictionary<string, List<ColumnInfoAttribute>> typeColumnInfos = new Dictionary<string, List<ColumnInfoAttribute>>();

        /// <summary>
        /// 获取数据对应的表结构描述信息
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static List<ColumnInfoAttribute> GetColumnInfos(Type type)
        {
            if (!typeColumnInfos.ContainsKey(type.FullName))
                typeColumnInfos.Add(type.FullName, GetColumnInfoPropsFirsttime(type));
            return typeColumnInfos[type.FullName];
        }

        /// <summary>
        /// 首次获取数据对应的表结构描述信息
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        private static List<ColumnInfoAttribute> GetColumnInfoPropsFirsttime(Type type)
        {
            List<ColumnInfoAttribute> props = new List<ColumnInfoAttribute>();
            foreach (PropertyInfo info in type.GetProperties())
            {
                foreach (ColumnInfoAttribute a in info.GetCustomAttributes(typeof(ColumnInfoAttribute), true))
                {
                    a.PropertyName = info.Name;
                    a.PropertyType = info.PropertyType.Name;
                    props.Add(a);
                }
            }
            return props;
        }
        #endregion


        public OracleDB() //DBConfig.xml定义的链接串连接串
        {
            _strConn = _staticStrConn;
        }

        public OracleDB(string strConn)
        {
            _strConn = strConn;
        }

        private void SetDBOPResult(OracleResult r, Exception e)
        {
            r.IsFailed = true;
            r.ErrorMessage = e.Message;
        }

        private bool CheckIsOpen(OracleResult r)
        {
            if (!_isOpen)
            {
                r.IsFailed = true;
                r.ErrorMessage = "数据库未打开";
                return false;
            }
            return true;
        }


        #region 数据库操作

        public OracleResult OpenDB()
        {
            OracleResult r = new OracleResult();

            if (_strConn == null)
            {
                r.IsFailed = true;
                r.ErrorMessage = "连接串未初始化";
                return r;
            }

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _isOpen = true;
            }
            catch (Exception ex)
            {
                SetDBOPResult(r, ex);
            }


            return r;
        }

        public OracleResult CloseDB()
        {
            OracleResult r = new OracleResult();
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
                SetDBOPResult(r, ex);
            }
            return r;
        }

        public bool TestConnection()
        {
            bool r = true;
            _conn = new OracleConnection(_strConn);

            try { _conn.Open(); }
            catch (Exception) { r = false; }
            finally { _conn.Close(); }

            return r;
        }

        public static bool ST_TestConnection()
        {
            return (new OracleDB()).TestConnection();
        }

        public static OracleConnection GetDBConnection()
        {
            return new OracleConnection(OracleDB._staticStrConn);
        }

        public static OracleCommand GetCommand(string strCmd, OracleConnection conn)
        {
            return new OracleCommand(strCmd, conn);
        }

        public static OracleDataReader GetDataReader(string strCmd, OracleConnection conn)
        {
            return new OracleCommand(strCmd, conn).ExecuteReader();
        }
        public static OracleParameter CreateOracleParameter(string pName, OracleDbType pType, int pSize)
        {
            OracleParameter p = new OracleParameter();
            switch (pType)
            {
                case OracleDbType.Date:
                    p.Value = DateTime.Now;
                    break;

                case OracleDbType.NChar:
                case OracleDbType.Varchar2:
                case OracleDbType.NVarchar2:
                    p.Value = "";
                    break;

                default:
                    p.Value = 0;
                    break;
            }
            p.ParameterName = pName;
            p.OracleDbType = pType;
            p.Size = pSize;
            return p;
        }
        #endregion

        #region 执行Command

        public OracleResult ExecuteCmd(string strCmd)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _command.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_ExecuteCmd(string strCmd) //无需打开关闭
        {
            OracleResult r = new OracleResult();

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _command.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_ExecuteCmd(string strCmd)
        {
            return (new OracleDB()).DB_ExecuteCmd(strCmd);
        } //静态函数


        public OracleResult ExecuteCmd(string strCmd, DataSet ds)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                OracleDataAdapter dataAdapter = new OracleDataAdapter(strCmd, _conn);
                dataAdapter.Fill(ds);
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_ExecuteCmd(string strCmd, DataSet ds) //无需打开关闭
        {
            OracleResult r = new OracleResult();

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                OracleDataAdapter dataAdapter = new OracleDataAdapter(strCmd, _conn);
                dataAdapter.Fill(ds);
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_ExecuteCmd(string strCmd, DataSet ds)
        {
            return (new OracleDB()).DB_ExecuteCmd(strCmd, ds);
        } //静态函数

        #endregion

        #region 执行Procedure

        public OracleResult ExecuteProcedure(string procedureName)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        }

        public OracleResult DB_ExecuteProcedure(string procedureName)
        {
            OracleResult r = new OracleResult();

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_ExecuteProcedure(string procedureName)
        {
            return (new OracleDB()).DB_ExecuteProcedure(procedureName);
        }

        public OracleResult ExecuteProcedure(string procedureName, string[] paramList, string[] dataList)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                for (int i = 0; i < paramList.Length; i++)
                {
                    _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Varchar2) { Value = dataList[i] != null ? dataList[i] : " " });
                }

                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        }

        public OracleResult DB_ExecuteProcedure(string procedureName, string[] paramList, string[] dataList)
        {
            OracleResult r = new OracleResult();

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                for (int i = 0; i < paramList.Length; i++)
                {
                    _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Varchar2) { Value = dataList[i] != null ? dataList[i] : " " });
                }

                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_ExecuteProcedure(string procedureName, string[] paramList, string[] dataList)
        {
            return (new OracleDB()).DB_ExecuteProcedure(procedureName, paramList, dataList);
        }
        //typelist 0字符串， 1 整数 2 浮点
        public OracleResult ExecuteProcedure(string procedureName, int[] typeList, string[] paramList, object[] dataList)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                for (int i = 0; i < typeList.Length; i++)
                {
                    if (typeList[i] == 0)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Varchar2) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                    else if (typeList[i] == 1)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Int32) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                    else if (typeList[i] == 2)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Double) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                }

                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        }

        public OracleResult DB_ExecuteProcedure(string procedureName, int[] typeList, string[] paramList, object[] dataList)
        {
            OracleResult r = new OracleResult();

            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand();
                _command.Connection = _conn;
                _command.CommandText = procedureName;
                _command.CommandType = CommandType.StoredProcedure;
                for (int i = 0; i < typeList.Length; i++)
                {
                    if (typeList[i] == 0)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Varchar2) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                    else if (typeList[i] == 1)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Int32) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                    else if (typeList[i] == 2)
                    {
                        _command.Parameters.Add(new OracleParameter(paramList[i], OracleDbType.Double) { Value = dataList[i] != null ? dataList[i] : DBNull.Value });
                    }
                }

                _command.ExecuteNonQuery();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_ExecuteProcedure(string procedureName, int[] typeList, string[] paramList, object[] dataList)
        {
            return (new OracleDB()).DB_ExecuteProcedure(procedureName, typeList, paramList, dataList);
        }

        #endregion

        #region 获得单个object

        public OracleResult GetScalarObj(string strCmd, out object obj)
        {
            OracleResult r = new OracleResult();
            obj = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                obj = _command.ExecuteScalar();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_GetScalarObj(string strCmd, out object obj) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            obj = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                obj = _command.ExecuteScalar();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_GetScalarObj(string strCmd, out object obj)
        {
            return (new OracleDB()).DB_GetScalarObj(strCmd, out obj);
        } //静态函数

        #endregion

        #region 获取int array

        public OracleResult RetrieveToIntArray(string strCmd, System.Collections.IList data)
        {
            OracleResult r = new OracleResult();
            data.Clear();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetInt32(0));
                }

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToIntArray(string strCmd, System.Collections.IList data) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            data.Clear();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetInt32(0));
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToIntArray(string strCmd, System.Collections.IList data)
        {
            return (new OracleDB()).DB_RetrieveToIntArray(strCmd, data);
        } //静态函数

        #endregion

        #region 获取long array

        public OracleResult RetrieveToLongArray(string strCmd, System.Collections.IList data)
        {
            OracleResult r = new OracleResult();
            data.Clear();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetInt64(0));
                }

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToLongArray(string strCmd, System.Collections.IList data) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            data.Clear();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetInt64(0));
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToLongArray(string strCmd, System.Collections.IList data)
        {
            return (new OracleDB()).DB_RetrieveToLongArray(strCmd, data);
        } //静态函数

        #endregion

        #region 获取string array

        public OracleResult RetrieveToStringArray(string strCmd, System.Collections.IList data)
        {
            OracleResult r = new OracleResult();
            data.Clear();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetString(0));
                }

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToStringArray(string strCmd, System.Collections.IList data) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            data.Clear();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                while (_reader.Read())
                {
                    data.Add(_reader.GetString(0));
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToStringArray(string strCmd, System.Collections.IList data)
        {
            return (new OracleDB()).DB_RetrieveToStringArray(strCmd, data);
        } //静态函数

        #endregion

        #region 获取 objects

        public OracleResult RetrieveToObjects(string strCmd, System.Collections.IList data, Type type)
        {
            OracleResult r = new OracleResult();
            data.Clear();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<ColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    count = 0;
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
                            else if (itemProperty.PropertyType == typeof(uint) || itemProperty.PropertyType == typeof(uint?))
                                itemProperty.SetValue(t, (uint)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(float) || itemProperty.PropertyType == typeof(float?))
                                itemProperty.SetValue(t, _reader.GetFloat(count));
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

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToObjects(string strCmd, System.Collections.IList data, Type type) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            data.Clear();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<ColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    count = 0;
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
                            else if (itemProperty.PropertyType == typeof(uint) || itemProperty.PropertyType == typeof(uint?))
                                itemProperty.SetValue(t, (uint)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(long) || itemProperty.PropertyType == typeof(long?))
                                itemProperty.SetValue(t, (long)_reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(float) || itemProperty.PropertyType == typeof(float?))
                                itemProperty.SetValue(t, _reader.GetFloat(count));
                            else if (itemProperty.PropertyType == typeof(double) || itemProperty.PropertyType == typeof(double?))
                                itemProperty.SetValue(t, _reader.GetDouble(count));
                            else if (itemProperty.PropertyType == typeof(decimal) || itemProperty.PropertyType == typeof(decimal?))
                                itemProperty.SetValue(t, _reader.GetDecimal(count));
                            else if (itemProperty.PropertyType == typeof(DateTime) || itemProperty.PropertyType == typeof(DateTime?))
                                itemProperty.SetValue(t, _reader.GetDateTime(count));//* 转化？？
                            else if (itemProperty.PropertyType == typeof(bool) || itemProperty.PropertyType == typeof(bool?))
                                itemProperty.SetValue(t, (int)_reader.GetDecimal(count) == 1 ? true : false);//* 转化？？
                            else if (itemProperty.PropertyType == typeof(byte[]))
                                itemProperty.SetValue(t, (byte[])_reader[count]);
                        }
                        count++;
                    }
                    data.Add(t);
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToObjects(string strCmd, System.Collections.IList data, Type type)
        {
            return (new OracleDB()).DB_RetrieveToObjects(strCmd, data, type);
        } //静态函数

        #endregion

        #region 获取 objects by col

        public OracleResult RetrieveToObjectsByCol(string strCmd, System.Collections.IList data, Type type)
        {
            OracleResult r = new OracleResult();
            data.Clear();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();
                PropertyDescriptor d;
                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);

                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        d = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]))
                        {
                            if (d.PropertyType == typeof(string))
                                d.SetValue(t, _reader[i].ToString());
                            else if (d.PropertyType == typeof(short) || d.PropertyType == typeof(short?))
                                d.SetValue(t, (short)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(int) || d.PropertyType == typeof(int?))
                                d.SetValue(t, (int)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(uint) || d.PropertyType == typeof(uint?))
                                d.SetValue(t, (uint)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(long) || d.PropertyType == typeof(long?))
                                d.SetValue(t, (long)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(float) || d.PropertyType == typeof(float?))
                                d.SetValue(t, _reader.GetFloat(i));
                            else if (d.PropertyType == typeof(double) || d.PropertyType == typeof(double?))
                                d.SetValue(t, _reader.GetDouble(i));
                            else if (d.PropertyType == typeof(decimal) || d.PropertyType == typeof(decimal?))
                                d.SetValue(t, _reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(DateTime) || d.PropertyType == typeof(DateTime?))
                                d.SetValue(t, _reader.GetDateTime(i));//* 转化？？
                            else if (d.PropertyType == typeof(byte[]))
                                d.SetValue(t, (byte[])_reader[i]);
                        }

                    }
                    data.Add(t);
                }

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToObjectsByCol(string strCmd, System.Collections.IList data, Type type) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            data.Clear();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();
                PropertyDescriptor d;
                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);

                while (_reader.Read())
                {
                    Object t = System.Activator.CreateInstance(type);

                    for (int i = 0; i < _reader.FieldCount; i++)
                    {
                        d = properties.Find(_reader.GetName(i), true);
                        if (!DBNull.Value.Equals(_reader[i]))
                        {
                            if (d.PropertyType == typeof(string))
                                d.SetValue(t, _reader[i].ToString());
                            else if (d.PropertyType == typeof(short) || d.PropertyType == typeof(short?))
                                d.SetValue(t, (short)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(int) || d.PropertyType == typeof(int?))
                                d.SetValue(t, (int)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(uint) || d.PropertyType == typeof(uint?))
                                d.SetValue(t, (uint)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(long) || d.PropertyType == typeof(long?))
                                d.SetValue(t, (long)_reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(float) || d.PropertyType == typeof(float?))
                                d.SetValue(t, _reader.GetFloat(i));
                            else if (d.PropertyType == typeof(double) || d.PropertyType == typeof(double?))
                                d.SetValue(t, _reader.GetDouble(i));
                            else if (d.PropertyType == typeof(decimal) || d.PropertyType == typeof(decimal?))
                                d.SetValue(t, _reader.GetDecimal(i));
                            else if (d.PropertyType == typeof(DateTime) || d.PropertyType == typeof(DateTime?))
                                d.SetValue(t, _reader.GetDateTime(i));//* 转化？？
                            else if (d.PropertyType == typeof(byte[]))
                                d.SetValue(t, (byte[])_reader[i]);
                        }

                    }
                    data.Add(t);
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToObjectsByCol(string strCmd, System.Collections.IList data, Type type)
        {
            return (new OracleDB()).DB_RetrieveToObjectsByCol(strCmd, data, type);
        } //静态函数

        #endregion

        #region 获取 单个 object

        public OracleResult RetrieveToSingleObject(string strCmd, object data, Type type)
        {
            OracleResult r = new OracleResult();
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<ColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    // Object t = System.Activator.CreateInstance(type);

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
                            else if (itemProperty.PropertyType == typeof(float) || itemProperty.PropertyType == typeof(float?))
                                itemProperty.SetValue(data, _reader.GetFloat(count));
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
                    //data = t;
                    break;
                }

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveToSingleObject(string strCmd, object data, Type type) //无需打开关闭
        {
            OracleResult r = new OracleResult();
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(type);
                PropertyDescriptor itemProperty;
                List<ColumnInfoAttribute> selectProps = GetColumnInfos(type);

                int count = 0;
                while (_reader.Read())
                {
                    // Object t = System.Activator.CreateInstance(type);

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
                            else if (itemProperty.PropertyType == typeof(float) || itemProperty.PropertyType == typeof(float?))
                                itemProperty.SetValue(data, _reader.GetFloat(count));
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
                    //data = t;

                    break;
                }
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveToSingleObject(string strCmd, object data, Type type)
        {
            return (new OracleDB()).DB_RetrieveToSingleObject(strCmd, data, type);
        } //静态函数

        #endregion

        #region 获取ValuePair 分别为 int long string

        public OracleResult RetrieveValuePair(string strCmd, out ValueDisplayPair<int?>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<int?>> datalist = new List<ValueDisplayPair<int?>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<int?> { Value = _reader.GetInt32(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveValuePair(string strCmd, out ValueDisplayPair<int?>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<int?>> datalist = new List<ValueDisplayPair<int?>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<int?> { Value = _reader.GetInt32(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveValuePair(string strCmd, out ValueDisplayPair<int?>[] data)
        {
            return (new OracleDB()).DB_RetrieveValuePair(strCmd, out data);
        } //静态函数

        public OracleResult RetrieveValuePair(string strCmd, out ValueDisplayPair<int>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<int>> datalist = new List<ValueDisplayPair<int>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<int> { Value = _reader.GetInt32(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveValuePair(string strCmd, out ValueDisplayPair<int>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<int>> datalist = new List<ValueDisplayPair<int>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<int> { Value = _reader.GetInt32(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveValuePair(string strCmd, out ValueDisplayPair<int>[] data)
        {
            return (new OracleDB()).DB_RetrieveValuePair(strCmd, out data);
        } //静态函数


        public OracleResult RetrieveValuePair(string strCmd, out ValueDisplayPair<long?>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<long?>> datalist = new List<ValueDisplayPair<long?>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<long?> { Value = _reader.GetInt64(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveValuePair(string strCmd, out ValueDisplayPair<long?>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<long?>> datalist = new List<ValueDisplayPair<long?>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<long?> { Value = _reader.GetInt64(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveValuePair(string strCmd, out ValueDisplayPair<long?>[] data)
        {
            return (new OracleDB()).DB_RetrieveValuePair(strCmd, out data);
        } //静态函数

        public OracleResult RetrieveValuePair(string strCmd, out ValueDisplayPair<long>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<long>> datalist = new List<ValueDisplayPair<long>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<long> { Value = _reader.GetInt64(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveValuePair(string strCmd, out ValueDisplayPair<long>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<long>> datalist = new List<ValueDisplayPair<long>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<long> { Value = _reader.GetInt64(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveValuePair(string strCmd, out ValueDisplayPair<long>[] data)
        {
            return (new OracleDB()).DB_RetrieveValuePair(strCmd, out data);
        } //静态函数

        public OracleResult RetrieveValuePair(string strCmd, out ValueDisplayPair<string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<string>> datalist = new List<ValueDisplayPair<string>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<string> { Value = _reader.GetString(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveValuePair(string strCmd, out ValueDisplayPair<string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<ValueDisplayPair<string>> datalist = new List<ValueDisplayPair<string>>();
                while (_reader.Read())
                {
                    datalist.Add(new ValueDisplayPair<string> { Value = _reader.GetString(0), Display = _reader.GetString(1) });
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveValuePair(string strCmd, out ValueDisplayPair<string>[] data)
        {
            return (new OracleDB()).DB_RetrieveValuePair(strCmd, out data);
        } //静态函数

        #endregion

        #region KVP
        public OracleResult RetrieveKeyValuePair(string strCmd, out KeyValuePair<int, string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<KeyValuePair<int, string>> datalist = new List<KeyValuePair<int, string>>();
                while (_reader.Read())
                {
                    datalist.Add(new KeyValuePair<int, string>(_reader.GetInt32(0), _reader.GetString(1)));
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveKeyValuePair(string strCmd, out KeyValuePair<int, string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<KeyValuePair<int, string>> datalist = new List<KeyValuePair<int, string>>();
                while (_reader.Read())
                {
                    datalist.Add(new KeyValuePair<int, string>(_reader.GetInt32(0), _reader.GetString(1)));
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveKeyValuePair(string strCmd, out KeyValuePair<int, string>[] data)
        {
            return (new OracleDB()).DB_RetrieveKeyValuePair(strCmd, out data);
        } //静态函数
        public OracleResult RetrieveKeyValuePair(string strCmd, out KeyValuePair<string, string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            if (!CheckIsOpen(r)) return r;

            try
            {
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<KeyValuePair<string, string>> datalist = new List<KeyValuePair<string, string>>();
                while (_reader.Read())
                {
                    datalist.Add(new KeyValuePair<string, string>(_reader.GetString(0), _reader.GetString(1)));
                }
                data = datalist.ToArray();

            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }

            return r;
        } //需显式打开关闭数据库

        public OracleResult DB_RetrieveKeyValuePair(string strCmd, out KeyValuePair<string, string>[] data)
        {
            OracleResult r = new OracleResult();
            data = null;
            try
            {
                _conn = new OracleConnection(_strConn);
                _conn.Open();
                _command = new OracleCommand(strCmd, _conn);
                _reader = _command.ExecuteReader();

                List<KeyValuePair<string, string>> datalist = new List<KeyValuePair<string, string>>();
                while (_reader.Read())
                {
                    datalist.Add(new KeyValuePair<string, string>(_reader.GetString(0), _reader.GetString(1)));
                }
                data = datalist.ToArray();
            }
            catch (Exception ex)
            {
                r.IsFailed = true;
                r.ErrorMessage = ex.Message;
            }
            finally
            {
                _conn.Close();
            }

            return r;
        }

        public static OracleResult ST_RetrieveKeyValuePair(string strCmd, out KeyValuePair<string, string>[] data)
        {
            return (new OracleDB()).DB_RetrieveKeyValuePair(strCmd, out data);
        } //静态函数
        #endregion
    }
}