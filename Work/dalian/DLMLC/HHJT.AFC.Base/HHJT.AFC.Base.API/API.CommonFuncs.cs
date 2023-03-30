using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace HHJT.AFC.Base.API
{
    public static class DataValidateFuncs
    {

        public static bool IsNumeric(string str)
        {
            Regex r = new Regex("^\\d+$");
            return r.IsMatch(str);
        }

        public static bool IsMacthRegExp(string str, string exp)
        {
            Regex r = new Regex(exp);
            return r.IsMatch(str);
        }

        public static bool IsNumericWithLength(string str, int length)
        {
            Regex r = new Regex("^\\d{" + length + "}$");
            return r.IsMatch(str);
        }

        public static bool IsHexWithLength(string str, int length)
        {
            Regex r = new Regex("^[0-9a-fA-F]{" + length + "," + length + "}$");
            return r.IsMatch(str);
        }

        public static bool IsValidIPAddr(string str)
        {
            //^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$ 
            //^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$ 

            Regex r = new Regex(@"^(25[0-5]|2[0-4][0-9]|[0-1]{1" +
                @"}[0-9]{2" +
                @"}|[1-9]{1" +
                @"}[0-9]{1" +
                @"}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1" +
                @"}[0-9]{2" +
                @"}|[1-9]{1" +
                @"}[0-9]{1" +
                @"}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1" +
                @"}[0-9]{2" +
                @"}|[1-9]{1" +
                @"}[0-9]{1" +
                @"}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1" +
                @"}[0-9]{2" +
                @"}|[1-9]{1" +
                @"}[0-9]{1" +
                @"}|[0-9])$");
            return r.IsMatch(str);

        }
    }

    /// <summary>
    /// 数据转换
    /// </summary>
    public static class DataConverter
    {
        public static ushort Bytes2UShort(byte[] data)
        {
            return (ushort)(data[0] << 8 | data[1]);
        }

        public static ushort Bytes2UShort(byte[] data, int offset)
        {
            return (ushort)(data[offset] << 8 | data[offset + 1]);
        }

        public static uint Bytes2UInt(byte[] data)
        {
            return (uint)(data[0] << 0x18 | data[1] << 0x10 | data[2] << 0x08 | data[3]);
        }

        public static uint Bytes2UInt(byte[] data, int offset)
        {
            return (uint)(data[offset] << 0x18 | data[offset + 1] << 0x10 | data[offset + 2] << 0x08 | data[offset + 3]);
        }

        public static byte[] Value2Bytes(short v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
        public static byte[] Value2Bytes(ushort v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
        public static byte[] Value2Bytes(int v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
        public static byte[] Value2Bytes(uint v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
        public static byte[] Value2Bytes(double v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }
        public static byte[] Value2Bytes(float v)
        {
            byte[] bytes = BitConverter.GetBytes(v);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }

        public static byte[] StrHex2Bytes(string str)
        {
            if (str == null) return null;
            byte[] bytes;
            if (str.Length%2==1)
            {
                bytes = new byte[str.Length / 2+1];
            }
            else
            {
                bytes = new byte[str.Length / 2];
            }
             

            for (int i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(str.Substring(i * 2, 2), 16);
            }

            return bytes;
        }

        public static string Bytes2HexStr(byte[] data)
        {
            if (data == null) return null;
            string strRes = "";
            foreach (var item in data)
            {
                strRes += string.Format("{0:X2}", item);
            }
            return strRes;
        }

        public static string Bytes2HexStr(byte[] data, int start, int length)
        {
            if (data == null) return null;
            string strRes = "";
            for (int i = start; i < start + length; i++)
            {
                strRes += string.Format("{0:X2}", data[i]);

            }

            return strRes;
        }

        public static string Bytes2HexStr(byte[] data, int length)
        {
            if (data == null) return null;
            string strRes = "";
            for (int i = 0; i < length; i++)
            {
                strRes += string.Format("{0:X2}", data[i]);

            }

            return strRes;
        }


        public static byte[] IPStr2Block(string strIP)
        {
            if (strIP == null) return null;
            if (!DataValidateFuncs.IsValidIPAddr(strIP)) return null;
            List<byte> d = new List<byte>();
            foreach (var item in strIP.Split('.'))
            {
                d.Add(byte.Parse(item));
            }
            return d.ToArray();
        }

        public static string IPBlock2Str(byte[] ipBlock)
        {
            if (ipBlock == null) return null;
            if (ipBlock.Length != 4) return null;
            else
                return string.Format("{0}.{1}.{2}.{3}", ipBlock[0], ipBlock[1], ipBlock[2], ipBlock[3]);
        }
    }


    public static class FileOP
    {
        /// <summary>
        /// 读二进制文件
        /// </summary>
        /// <param name="fName"></param>
        /// <param name="data"></param>
        public static void ReadBinaryFile(string fName, out byte[] data)
        {
            try
            {
                FileStream f = new FileStream(fName, FileMode.Open);
                data = new byte[f.Length];
                f.Read(data, 0, (int)f.Length);

                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }
        }

        /// <summary>
        /// 写二进制文件1
        /// </summary>
        /// <param name="fName"></param>
        /// <param name="data"></param>
        public static void WriteBinaryFile(string fName, byte[] data)
        {
            if (data == null) return;
            try
            {
                FileStream f = new FileStream(fName, FileMode.Create);
                f.Write(data, 0, data.Length);
                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }

        }

        /// <summary>
        /// 写二进制文件2
        /// </summary>
        /// <param name="fName"></param>
        /// <param name="data"></param>
        /// <param name="dLength"></param>
        public static void WriteBinaryFile(string fName, byte[] data, int dLength)
        {
            if (data == null) return;
            try
            {
                FileStream f = new FileStream(fName, FileMode.Create);
                f.Write(data, 0, dLength);
                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }

        }

        /// <summary>
        /// 写二进制文件3
        /// </summary>
        /// <param name="fName"></param>
        /// <param name="data"></param>
        /// <param name="dLength"></param>
        public static void WriteBinaryFile(string fName, byte[] data, FileMode fileMode)
        {
            if (data == null) return;
            try
            {
                FileStream f = new FileStream(fName, fileMode);
                f.Write(data, 0, data.Length);
                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }

        }

        public static void ReadTextFile(string fName, out string[] strs)
        {
            try
            {
                StreamReader f = new StreamReader(fName);
                //data = new byte[f.Length];
                //f.Read(data, 0, (int)f.Length);
                List<string> strReadList = new List<string>();
                string strRead;
                do
                {
                    strRead = f.ReadLine();
                    if (strRead != null && strRead != "")
                        strReadList.Add(strRead);
                } while (strRead != null);
                strs = strReadList.ToArray();
                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }
        }

        public static void WriteTextFile(string fName, string[] strs)
        {
            try
            {
                StreamWriter f = new StreamWriter(fName);
                //data = new byte[f.Length];
                //f.Read(data, 0, (int)f.Length);
                foreach (var item in strs)
                {
                    f.WriteLine(item);
                }

                f.Close();
            }
            catch (IOException ex)
            {
                throw ex;
            }

        }

    }


    public static class FormatOutput
    {
        public static void OutputHex(byte[] buf)
        {
            int r = 0;
            int c = 0;

            int maxc = 4;
            List<string> strs = new List<string>();
            for (int i = 0; i < buf.Length; i += 4)
            {
                if (i < buf.Length - 3)
                    strs.Add(string.Format("{0:X2} {1:X2} {2:X2} {3:X2}", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]));

                if (i == buf.Length - 3)
                    strs.Add(string.Format("{0:X2} {1:X2} {2:X2} ", buf[i], buf[i + 1], buf[i + 2]));

                if (i == buf.Length - 2)
                    strs.Add(string.Format("{0:X2} {1:X2} ", buf[i], buf[i + 1]));

                if (i == buf.Length - 1)
                    strs.Add(string.Format("{0:X2}  ", buf[i]));
            }

            for (int i = 0; i < strs.Count; i++)
            {
                if (i % maxc == 0)
                {
                    r++;
                    Console.WriteLine();
                    Console.Write("{0:000}  ", r);
                }

                Console.Write("{0}  ", strs[i]);
            }

            Console.WriteLine();
            Console.WriteLine();
        }

        public static string StrDateTime_()
        {
            return DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
        }
        public static string StrDateTime_(DateTime dt)
        {
            return dt.ToString("yyyy-MM-dd HH:mm:ss");
        }

        public static string StrDateTime()
        {
            return DateTime.Now.ToString("yyyyMMdd HH:mm:ss");
        }
        public static string StrDateTime(DateTime dt)
        {
            return dt.ToString("yyyyMMdd HH:mm:ss");
        }

        public static string StrTime()
        {
            return DateTime.Now.ToString("HH:mm:ss");
        }
        public static string StrTime(DateTime dt)
        {
            return dt.ToString("HH:mm:ss");
        }

        public static string StrDate()
        {
            return DateTime.Now.ToString("yyyyMMdd");
        }
        public static string StrDate(DateTime dt)
        {
            return dt.ToString("yyyyMMdd");
        }

        public static string StrDate_()
        {
            return DateTime.Now.ToString("yyyy-MM-dd");
        }
        public static string StrDate_(DateTime dt)
        {
            return dt.ToString("yyyy-MM-dd");
        }
    }
}
