using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;
namespace HHJT.AFC.Base.API
{
    public class CryptFuncs
    {
        #region 不再使用

        /*
        private static byte[] entropy = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        public static string MachineCrypt(string s)
        {
            // return s;
            try
            {
                byte[] enc = ProtectedData.Protect(Encoding.Unicode.GetBytes(s),
                 entropy, DataProtectionScope.LocalMachine);
                return BitConverter.ToString(enc);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            return null;
        }
        public static string MachineDeCrypt(string s)
        {
            // return s;
            try
            {
                string[] sp = s.Split('-');
                byte[] bs = new byte[sp.Length];
                for (int i = 0; i < sp.Length; i++)
                {
                    bs[i] = byte.Parse(sp[i], System.Globalization.NumberStyles.AllowHexSpecifier);
                }
                
                byte[] dec = ProtectedData.Unprotect(bs,
                entropy, DataProtectionScope.CurrentUser);
                return Encoding.Unicode.GetString(dec);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return null;

        }*/
        #endregion
        public static byte[] MD5Crypt(byte[] data)
        {
            return (MD5.Create()).ComputeHash(data);
        }

        public static string Md5Crypt(string str)
        {
            StringBuilder pwd = new StringBuilder();
            MD5 md5 = MD5.Create();
            byte[] s = md5.ComputeHash(Encoding.UTF8.GetBytes(str));

            foreach (byte d in s)
            {
                pwd.Append(d.ToString("X2"));
            }

            return pwd.ToString();
        }

        public static bool VerifyMAC(byte[] data)
        {
            int dataLength = data.Length;
            byte[] MAC = (MD5.Create()).ComputeHash(data, 0, dataLength - 16);
            for (int i = 0; i < MAC.Length; i++)
            {
                if (MAC[i] != data[dataLength - 16 + i])
                { return false; }
            }
            return true;

        }

        private const string STR_DES_KEY = "tiantian";
        private const string STR_DES_IV = "descarts";

        public static string DESEncrypt(string strPlain)
        {
            //把密钥转换成字节数组
            byte[] bytesDESKey = ASCIIEncoding.ASCII.GetBytes(STR_DES_KEY);
            //把向量转换成字节数组
            byte[] bytesDESIV = ASCIIEncoding.ASCII.GetBytes(STR_DES_IV);
            //声明1个新的DES对象
            DESCryptoServiceProvider desEncrypt = new DESCryptoServiceProvider();
            //开辟一块内存流
            MemoryStream msEncrypt = new MemoryStream();
            //把内存流对象包装成加密流对象
            CryptoStream csEncrypt = new CryptoStream(msEncrypt, desEncrypt.CreateEncryptor(bytesDESKey, bytesDESIV), CryptoStreamMode.Write);
            //把加密流对象包装成写入流对象
            StreamWriter swEncrypt = new StreamWriter(csEncrypt);
            //写入流对象写入明文
            swEncrypt.WriteLine(strPlain);
            //写入流关闭
            swEncrypt.Close();
            //加密流关闭
            csEncrypt.Close();
            //把内存流转换成字节数组，内存流现在已经是密文了
            byte[] bytesCipher = msEncrypt.ToArray();
            //内存流关闭
            msEncrypt.Close();
            //把密文字节数组转换为字符串，并返回
            //return UnicodeEncoding.Unicode.GetString(bytesCipher);
            StringBuilder strB = new StringBuilder();
            foreach (byte b in bytesCipher)
            {
                strB.Append(b.ToString("X2"));
            }
            return strB.ToString();

        }


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
    }

    public sealed class CRC
    {
        private static ushort[] CRC16Table = null;
        private static uint[] CRC32Table = null;
        private static void MakeCRC16Table()
        {
            if (CRC16Table != null) return;
            CRC16Table = new ushort[256];
            for (ushort i = 0; i < 256; i++)
            {
                ushort vCRC = i;
                for (int j = 0; j < 8; j++)
                    if (vCRC % 2 == 0)
                        vCRC = (ushort)(vCRC >> 1);
                    else vCRC = (ushort)((vCRC >> 1) ^ 0x8408);
                CRC16Table[i] = vCRC;
            }
        }
        
        private static void MakeCRC32Table()
        {
            if (CRC32Table != null) return;
            CRC32Table = new uint[256];
            for (uint i = 0; i < 256; i++)
            {
                uint vCRC = i;
                for (int j = 0; j < 8; j++)
                    if (vCRC % 2 == 0)
                        vCRC = (uint)(vCRC >> 1);
                    else vCRC = (uint)((vCRC >> 1) ^ 0xEDB88320);
                CRC32Table[i] = vCRC;
            }
        }
        public static ushort UpdateCRC16(byte AByte, ushort ASeed)
        {
            return (ushort)(CRC16Table[(ASeed & 0x000000FF) ^ AByte] ^ (ASeed >> 8));
        }
        public static uint UpdateCRC32(byte AByte, uint ASeed)
        {
            return (uint)(CRC32Table[(ASeed & 0x000000FF) ^ AByte] ^ (ASeed >> 8));
        }
        public static ushort CRC16(byte[] ABytes)
        {
            MakeCRC16Table();
            ushort Result = 0xFFFF;
            foreach (byte vByte in ABytes)
                Result = UpdateCRC16(vByte, Result);
            return (ushort)(~Result);
        }
        public static ushort CRC16(string AString, Encoding AEncoding)
        {
            return CRC16(AEncoding.GetBytes(AString));
        }
        public static ushort CRC16(string AString)
        {
            return CRC16(AString, Encoding.UTF8);
        }
        public static uint CRC32(byte[] ABytes)
        {
            MakeCRC32Table();
            uint Result = 0xFFFFFFFF;
            foreach (byte vByte in ABytes)
                Result = UpdateCRC32(vByte, Result);
            return (uint)(~Result);
        }
        public static uint CRC32(string AString, Encoding AEncoding)
        {
            return CRC32(AEncoding.GetBytes(AString));
        }
        public static uint CRC32(string AString)
        {
            return CRC32(AString, Encoding.UTF8);
        }
    }

}
