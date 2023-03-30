using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

///<summary>
/// -------------------------------------------------------
/// 文件：HHJT.Framework.XDR.cs
/// 版本：1.0.0
/// 时间：2011-09-07
/// 作者：刘树康
/// 版权: 上海华虹计通智能系统股份有限公司
/// 说明：类XDR数据生成、解析基础类库，单字节对齐）
/// ------------------------------------------------------
///
/// ------------------------------------------------------
/// 更新记录
/// ------------------------------------------------------
///     日期  |  作者  |                说明
/// ------------------------------------------------------
/// 2011-09-07| 刘树康 | 创建
/// ------------------------------------------------------
///</summary>
namespace HHJT.AFC.Base.XDR
{
    /// <summary>
    /// 数据生成、解析接口定义
    /// </summary>
    public interface IXDRType
    {
        /// <summary>
        /// 数据解析：将二进制数据解析为类型
        /// </summary>
        /// <param name="xdrs">要解析的数据流</param>
        void Xdr_decode(XDRStream xdrs);
        /// <summary>
        /// 数据生成：将类型生成对应的二进制数据流
        /// </summary>
        /// <param name="xdrs"></param>
        void Xdr_encode(XDRStream xdrs);
    }

    /// <summary>
    /// 数据解析异常定义
    /// </summary>
    public class XDRDecodeError : Exception
    {
        public XDRDecodeError(string s)
        {
        }
    }
    /// <summary>
    /// 二进制数据流封装
    /// </summary>
    public class XDRStream
    {
        protected internal int alloc_size;
        protected internal byte[] buffer;
        protected internal int growthRate;
        protected internal int in_Renamed;
        protected internal int out_Renamed;

        /// <summary>
        /// 默认构造函数
        /// </summary>
        public XDRStream()
        {
            this.alloc_size = 0x400;
            this.growthRate = 2;

            //this.alloc_size = 1024;
            this.buffer = new byte[this.alloc_size];
            this.in_Renamed = 0;
            this.out_Renamed = 0;
        }

        /// <summary>
        /// 指定初始大小构造函数
        /// </summary>
        /// <param name="alloc">指定分配长度</param>
        public XDRStream(int alloc)
        {
            this.alloc_size = 0x400;
            this.growthRate = 2;

            this.alloc_size = 1024;
            if (alloc > this.alloc_size)
            {
                this.alloc_size = alloc;
            }
            this.buffer = new byte[this.alloc_size];
            this.in_Renamed = 0;
            this.out_Renamed = 0;
        }

        public XDRStream(byte[] b, int length)
        {
            this.alloc_size = 0x400;
            this.growthRate = 2;
            //this.growthRate = RPCSettings.Instance.growthRate;
            this.alloc_size = b.Length;
            this.buffer = b;
            this.in_Renamed = length;
            this.out_Renamed = 0;
        }

        public virtual void Dump()
        {
            int num = this.out_Renamed;
            int num3 = this.in_Renamed;
            while (true)
            {
                int num2 = 0;
                while (num2 < 0x10)
                {
                    if ((num + num2) >= num3)
                    {
                        Console.Out.Write("   ");
                    }
                    else
                    {
                        int num4 = this.buffer[num + num2] & 0xff;
                        string str = Convert.ToString(num4, 0x10);
                        if (str.Length == 1)
                        {
                            str = "0" + str;
                        }
                        Console.Out.Write(str + " ");
                    }
                    num2++;
                }
                Console.Out.Write("   ");
                num2 = 0;
                while (num2 < 0x10)
                {
                    if ((num + num2) >= num3)
                    {
                        break;
                    }
                    char ch = (char)this.buffer[num + num2];
                    if ((ch >= ' ') && (ch <= 'z'))
                    {
                        Console.Out.Write(ch);
                    }
                    else
                    {
                        Console.Out.Write(".");
                    }
                    num2++;
                }
                Console.Out.WriteLine("");
                if ((num + num2) >= num3)
                {
                    return;
                }
                num += 0x10;
            }
        }

        public virtual byte Get_byte()
        {
            if (this.out_Renamed >= this.in_Renamed)
            {
                throw new XDRDecodeError("XDR stream empty");
            }
            byte num = this.buffer[this.out_Renamed++];
            if (this.out_Renamed == this.in_Renamed)
            {
                this.Reset();
            }
            return num;
        }

        public virtual byte[] Get_bytes(int n)
        {
            byte[] destinationArray = new byte[n];
            if ((this.out_Renamed + n) > this.in_Renamed)
            {
                throw new XDRDecodeError("XDR stream empty");
            }
            Array.Copy(this.buffer, this.out_Renamed, destinationArray, 0, n);
            this.out_Renamed += n;
            if (this.out_Renamed == this.in_Renamed)
            {
                this.Reset();
            }
            return destinationArray;
        }

        public virtual byte[] Get_data()
        {
            byte[] destinationArray = new byte[this.in_Renamed - this.out_Renamed];
            Array.Copy(this.buffer, this.out_Renamed, destinationArray, 0, this.in_Renamed - this.out_Renamed);
            return destinationArray;
        }

        public virtual int Get_length()
        {
            return (this.in_Renamed - this.out_Renamed);
        }

        public virtual int GetGrowthRate()
        {
            return this.growthRate;
        }

        public virtual void Put_byte(byte b)
        {
            if (this.in_Renamed >= this.buffer.Length)
            {
                byte[] sourceArray = this.buffer;
                if (1 == this.growthRate)
                {
                    this.buffer = new byte[sourceArray.Length + this.alloc_size];
                }
                else
                {
                    this.buffer = new byte[sourceArray.Length * this.growthRate];
                }
                Array.Copy(sourceArray, 0, this.buffer, 0, sourceArray.Length);
            }
            this.buffer[this.in_Renamed++] = b;
        }

        public virtual void Put_bytes(byte[] b, int n)
        {
            if ((this.in_Renamed + n) >= this.buffer.Length)
            {
                byte[] sourceArray = this.buffer;
                int length = sourceArray.Length;
                if (1 == this.growthRate)
                {
                    if (n > this.alloc_size)
                    {
                        this.buffer = new byte[length + n];
                    }
                    else
                    {
                        this.buffer = new byte[length + this.alloc_size];
                    }
                }
                else if (n > ((length * this.growthRate) - this.in_Renamed))
                {
                    this.buffer = new byte[length + n];
                }
                else
                {
                    this.buffer = new byte[length * this.growthRate];
                }
                Array.Copy(sourceArray, 0, this.buffer, 0, sourceArray.Length);
                this.Put_bytes(b, n);
            }
            else
            {
                Array.Copy(b, 0, this.buffer, this.in_Renamed, n);
                this.in_Renamed += n;
            }
        }

        public virtual void Reset()
        {
            this.in_Renamed = 0;
            this.out_Renamed = 0;
        }

        public virtual void SetGrowthRate(int r)
        {
            if (r < 1)
            {
                r = 1;
            }
            if (r > 10)
            {
                r = 10;
            }
            this.growthRate = r;
        }

        public virtual bool Xdr_decode_bool()
        {
            return (this.Xdr_decode_int() != 0);
        }

        public virtual byte Xdr_decode_byte()
        {
            //return (byte)this.xdr_decode_uint();
            return Get_byte();
        }

        public virtual double Xdr_decode_double()
        {
            byte[] buffer = new byte[8];
            buffer[7] = this.Get_byte();
            buffer[6] = this.Get_byte();
            buffer[5] = this.Get_byte();
            buffer[4] = this.Get_byte();
            buffer[3] = this.Get_byte();
            buffer[2] = this.Get_byte();
            buffer[1] = this.Get_byte();
            buffer[0] = this.Get_byte();
            MemoryStream input = new MemoryStream(buffer);
            BinaryReader reader = new BinaryReader(input);
            return reader.ReadDouble();
        }

        public virtual float Xdr_decode_float()
        {
            byte[] buffer = new byte[4];
            buffer[3] = this.Get_byte();
            buffer[2] = this.Get_byte();
            buffer[1] = this.Get_byte();
            buffer[0] = this.Get_byte();
            MemoryStream input = new MemoryStream(buffer);
            BinaryReader reader = new BinaryReader(input);
            return reader.ReadSingle();
        }

        public virtual int Xdr_decode_int()
        {
            return (((((this.Get_byte() & 0xff) << 0x18) | ((this.Get_byte() & 0xff) << 0x10)) | ((this.Get_byte() & 0xff) << 8)) | (this.Get_byte() & 0xff));
        }

        public virtual long Xdr_decode_long()
        {
            return (long)(((this.Xdr_decode_int() & 0xffffffffL) << 0x20) | (this.Xdr_decode_int() & 0xffffffffL));
        }

        public virtual byte[] Xdr_decode_opaque()
        {
            int n = this.Xdr_decode_int();
            if (n != 0)
            {
                return this.Xdr_decode_opaque(n);
            }
            return new byte[0];
        }

        public virtual byte[] Xdr_decode_opaque(int n)
        {
            //  int num = ((n + 3) / 4) * 4;
            byte[] buffer = this.Get_bytes(n);
            //for (int i = 0; i <  n; i++)
            //{
            //    this.get_byte();
            //}
            return buffer;
        }

        public virtual sbyte Xdr_decode_sbyte()
        {
            //return (sbyte)this.xdr_decode_int();
            return (sbyte)this.Get_byte();
        }

        public virtual short Xdr_decode_short()
        {
            //return (short)this.xdr_decode_int();
            return (short)(((this.Get_byte() & 0xff) << 8) | (this.Get_byte() & 0xff));

        }

        //public virtual string xdr_decode_string()
        //{
        //    int n = this.xdr_decode_int();
        //    return this.xdr_decode_string(n);
        //}

        public virtual string Xdr_decode_string(int n)
        {
            byte[] buffer = Xdr_decode_opaque(n);
            //char[] chArray = new char[n];

            // Encoding ansiCode = Encoding.GetEncoding("GB2312");
            // ansiCode.GetBytes(s);
            Encoding unsCode = Encoding.GetEncoding("GB2312");
            return unsCode.GetString(buffer);

            //for (int i = 0; i < n; i++)
            //{
            //    chArray[i] = (char)buffer[i];
            //}
            //return new string(chArray);
        }

        public virtual string Xdr_decode_bcd(int n)
        {
            byte[] buffer = this.Xdr_decode_opaque(n);
            string strBCD = "";
            for (int i = 0; i < n; i++)
            {
                strBCD += System.Convert.ToString(buffer[i], 16).PadLeft(2, '0');
            }

            return strBCD;
        }

        public virtual uint Xdr_decode_uint()
        {
            return (uint)this.Xdr_decode_int();
        }

        public virtual ulong Xdr_decode_ulong()
        {
            return (ulong)this.Xdr_decode_long();
        }

        public virtual ushort Xdr_decode_ushort()
        {
            return (ushort)this.Xdr_decode_short();
        }

        public virtual void Xdr_encode_bool(bool x)
        {
            if (x)
            {
                this.Xdr_encode_int(1);
            }
            else
            {
                this.Xdr_encode_int(0);
            }
        }

        public virtual void Xdr_encode_byte(byte x)
        {
            //this.xdr_encode_uint(x);
            this.Put_byte(x);
        }

        public virtual void Xdr_encode_double(double data)
        {
            byte[] buffer = new byte[8];
            MemoryStream output = new MemoryStream(buffer, true);
            new BinaryWriter(output).Write(data);
            this.Put_byte(buffer[7]);
            this.Put_byte(buffer[6]);
            this.Put_byte(buffer[5]);
            this.Put_byte(buffer[4]);
            this.Put_byte(buffer[3]);
            this.Put_byte(buffer[2]);
            this.Put_byte(buffer[1]);
            this.Put_byte(buffer[0]);
        }

        public virtual void Xdr_encode_float(float data)
        {
            byte[] buffer = new byte[4];
            MemoryStream output = new MemoryStream(buffer, true);
            new BinaryWriter(output).Write(data);
            this.Put_byte(buffer[3]);
            this.Put_byte(buffer[2]);
            this.Put_byte(buffer[1]);
            this.Put_byte(buffer[0]);
        }

        public virtual void Xdr_encode_int(int x)
        {
            this.Put_byte((byte)((x >> 0x18) & 0xff));
            this.Put_byte((byte)((x >> 0x10) & 0xff));
            this.Put_byte((byte)((x >> 8) & 0xff));
            this.Put_byte((byte)(x & 0xff));
        }

        public virtual void Xdr_encode_long(long x)
        {
            this.Xdr_encode_int((int)(x >> 0x20));
            this.Xdr_encode_int((int)(((ulong)x) & 0xffffffffL));
        }

        public virtual void Xdr_encode_opaque(byte[] data)
        {
            this.Xdr_encode_int(data.Length);
            this.Xdr_encode_opaque(data, data.Length);
        }

        public virtual void Xdr_encode_opaque(byte[] data, int n)
        {
            // int num = ((n + 3) / 4) * 4;
            //  this.put_bytes(data, n);
            for (int i = 0; i < n; i++)
            {
                this.Put_byte(data[i]);
            }
        }

        public virtual void Xdr_encode_sbyte(sbyte x)
        {
            //this.xdr_encode_int(x);
            this.Put_byte((byte)x);
        }

        public virtual void Xdr_encode_short(short x)
        {
            //this.xdr_encode_int(x);
            this.Put_byte((byte)((x >> 8) & 0xff));
            this.Put_byte((byte)(x & 0xff));
        }

        //public virtual void xdr_encode_string(string s)
        //{
        //    int length = s.Length;
        //    this.xdr_encode_int(length);
        //    this.xdr_encode_string(s, length);
        //}

        public virtual void Xdr_encode_string(string s, int n)
        {
            byte[] data = new byte[n];
            //for (int i = 0; (i < n) && (i < s.Length); i++)
            //{
            //    data[i] = (byte)s[i];
            //}
            //this.xdr_encode_opaque(data, n);
            Encoding ansiCode = Encoding.GetEncoding("GB2312");
            byte[] d = ansiCode.GetBytes(s);
            for (int i = 0; (i < n) && (i < d.Length); i++)
            {
                data[i] = (byte)d[i];
            }
            this.Xdr_encode_opaque(data, n);
        }

        public virtual void Xdr_encode_bcd(string s, int n)
        {
            byte[] data = new byte[n];

            for (int i = 0; i < n; i++)
            {
                if (i < s.Length / 2)
                {
                    data[i] = (byte)Convert.ToInt64(s.Substring(i * 2, 2), 16);
                    //Console.WriteLine( string.Format("{0:d}", Convert.ToByte(s.Substring(i * 2, 2))));
                }
            }

            this.Xdr_encode_opaque(data, n);
        }


        //public virtual string xdr_decode_bcd(int n)
        //{
        //    byte[] buffer = this.xdr_decode_opaque(n);
        //    string strBCD = "";
        //    for (int i = 0; i < n; i++)
        //    {
        //        strBCD += System.Convert.ToString(buffer[i], 16).PadLeft(2, '0');
        //    }

        //    return strBCD;
        //}


        public virtual void Xdr_encode_uint(uint x)
        {
            this.Xdr_encode_int((int)x);
        }

        public virtual void Xdr_encode_ulong(ulong x)
        {
            this.Xdr_encode_long((long)x);
        }

        public virtual void Xdr_encode_ushort(ushort x)
        {
            this.Xdr_encode_short((short)x);
        }
    }

    public static class XDRExtensionMethods
    {
        public static void Xdr_encode(this byte value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_byte(value);
        }
        //public static void xdr_decode(this byte value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_byte();
        //}

        public static void Xdr_encode(this sbyte value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_sbyte(value);
        }
        //public static void xdr_decode(this sbyte value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_sbyte();
        //}

        public static void Xdr_encode(this short value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_short(value);
        }
        //public static void xdr_decode(this short value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_short();
        //}

        public static void Xdr_encode(this ushort value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_ushort(value);
        }
        //public static void xdr_decode(this ushort value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_ushort();
        //}

        public static void Xdr_encode(this int value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_int(value);
        }
        //public static void xdr_decode(this int value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_int();
        //}

        public static void Xdr_encode(this uint value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_uint(value);
        }
        //public static void xdr_decode(this uint value, XDRStream xdrs)
        //{
        //    value = xdrs.xdr_decode_uint();
        //}

        public static void Xdr_encode(this long value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_long(value);
        }
        public static void Xdr_encode(this ulong value, XDRStream xdrs)
        {
            xdrs.Xdr_encode_ulong(value);
        }
    }

    public class HHJT_STRING : IXDRType //【Int】
    {
        public string value;

        // public byte[] data;

        private int strStoreLength;

        public HHJT_STRING(int arg_length)
        {
            //    data = new byte[arg_length];
            strStoreLength = arg_length;
        }

        public HHJT_STRING(int arg_length, string arg_value)
        {
            //     data = new byte[arg_length];
            strStoreLength = arg_length;
            value = arg_value;
        }
        public void Xdr_encode(XDRStream xdrs)
        {
            //value.xdr_encode(xdrs);
            xdrs.Xdr_encode_string(value, strStoreLength);

        }
        public void Xdr_decode(XDRStream xdrs)
        {
            // value = new int();
            //value = xdrs.xdr_decode_int();
            value = xdrs.Xdr_decode_string(strStoreLength);
        }

        public static implicit operator string(HHJT_STRING v)
        {
            return v.value;
        }

        /*  public static implicit operator HHJT_STRING(string v)
          {
              return new HHJT_STRING(v);
          }*/
    }

    public class HHJT_GMT : IXDRType //【Int】
    {
        public DateTime dtValue;
        public uint sValue;
        // public byte[] data;
        private DateTime refDate = new DateTime(2000, 1, 1);

        public HHJT_GMT(uint arg_value)
        {
            //    data = new byte[arg_length];
            sValue = arg_value;
            dtValue = refDate.AddSeconds(arg_value);
        }

        public HHJT_GMT(DateTime arg_dtvalue)
        {
            dtValue = arg_dtvalue;
            sValue = (uint)(dtValue - refDate).TotalSeconds;
        }

        public HHJT_GMT()
        {

        }
        public void Xdr_encode(XDRStream xdrs)
        {
            //value.xdr_encode(xdrs);
            xdrs.Xdr_encode_uint(sValue);

        }
        public void Xdr_decode(XDRStream xdrs)
        {
            // value = new int();
            //value = xdrs.xdr_decode_int();
            sValue = xdrs.Xdr_decode_uint();
            dtValue = refDate.AddSeconds(sValue);
        }

        public static implicit operator uint(HHJT_GMT v)
        {
            return v.sValue;
        }

        public static implicit operator DateTime(HHJT_GMT v)
        {
            return v.dtValue;
        }

        public static implicit operator HHJT_GMT(uint v)
        {
            HHJT_GMT a = new HHJT_GMT
            {
                sValue = v
            };
            return a;
        }

        public static implicit operator HHJT_GMT(DateTime v)
        {
            HHJT_GMT a = new HHJT_GMT
            {
                dtValue = v
            };
            return a;
        }
    }

    public class HHJT_BCD : IXDRType //【Int】
    {
        public string value;

        // public byte[] data;

        private int strStoreLength;

        public HHJT_BCD(int arg_length)
        {
            //    data = new byte[arg_length];
            strStoreLength = arg_length;
        }

        public HHJT_BCD(int arg_length, string arg_value)
        {
            //     data = new byte[arg_length];
            strStoreLength = arg_length;
            value = arg_value;
        }
        public void Xdr_encode(XDRStream xdrs)
        {
            //value.xdr_encode(xdrs);
            xdrs.Xdr_encode_bcd(value, strStoreLength);

        }
        public void Xdr_decode(XDRStream xdrs)
        {
            // value = new int();
            //value = xdrs.xdr_decode_int();
            value = xdrs.Xdr_decode_bcd(strStoreLength);
        }

        public static implicit operator string(HHJT_BCD v)
        {
            return v.value;
        }

        public static bool operator !=(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) != 0)
                return true;

            return false;
        }

        public static bool operator ==(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) == 0)
                return true;

            return false;
        }

        public static bool operator >(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) > 0)
                return true;

            return false;
        }

        public static bool operator <(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) < 0)
                return true;

            return false;
        }

        public static bool operator >=(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) >= 0)
                return true;

            return false;
        }

        public static bool operator <=(HHJT_BCD p1, HHJT_BCD p2)
        {
            if (string.Compare(p1.value, p2.value) <= 0)
                return true;

            return false;
        }
    }

    public class HHJT_DATE2 : IXDRType
    {
        public DateTime dtValue;
        public ushort sValue;
        // public byte[] data;
        private DateTime refDate = new DateTime(2000, 1, 1);

        public HHJT_DATE2(ushort arg_value)
        {
            //    data = new byte[arg_length];
            sValue = arg_value;
            dtValue = refDate.AddDays(arg_value);
        }

        public HHJT_DATE2(DateTime arg_dtvalue)
        {
            dtValue = arg_dtvalue;
            sValue = (ushort)(dtValue - refDate).Days;
        }

        public HHJT_DATE2()
        {

        }
        public void Xdr_encode(XDRStream xdrs)
        {
            //value.xdr_encode(xdrs);
            xdrs.Xdr_encode_ushort(sValue);

        }
        public void Xdr_decode(XDRStream xdrs)
        {
            // value = new int();
            //value = xdrs.xdr_decode_int();
            sValue = xdrs.Xdr_decode_ushort();
            dtValue = refDate.AddDays(sValue);
        }

        public static implicit operator uint(HHJT_DATE2 v)
        {
            return v.sValue;
        }

        public static implicit operator DateTime(HHJT_DATE2 v)
        {
            return v.dtValue;
        }

        public static implicit operator string(HHJT_DATE2 v)
        {
            return v.dtValue.ToString("yyyyMMdd");
        }

        public static implicit operator HHJT_DATE2(ushort v)
        {
            HHJT_DATE2 a = new HHJT_DATE2
            {
                sValue = v
            };
            return a;
        }

        public static implicit operator HHJT_DATE2(DateTime v)
        {
            HHJT_DATE2 a = new HHJT_DATE2
            {
                dtValue = v
            };
            return a;
        }
    }
}