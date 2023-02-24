

- [6022报文](#6022报文)
- [下载命令](#下载命令)
- [下载UI](#下载ui)
- [参数管理前两项选择添加换乘站](#参数管理前两项选择添加换乘站)
- [数据库](#数据库)
- [6101 \& 6121](#6101--6121)
- [配置](#配置)
- [oracle](#oracle)
- [报表特殊线路判断](#报表特殊线路判断)
  - [LCC.MN监控管理子系统](#lccmn监控管理子系统)
    - [设备监控](#设备监控)
  - [LCC.RP报表管理子系统](#lccrp报表管理子系统)
    - [收益类](#收益类)
    - [客流类](#客流类)
    - [综合类](#综合类)
    - [全收益](#全收益)
  - [LCC.TK票务管理子系统](#lcctk票务管理子系统)
    - [车票跟踪](#车票跟踪)
    - [设备跟踪](#设备跟踪)
    - [交易查询统计](#交易查询统计)
    - [对账数据查询](#对账数据查询)
    - [票卡申请管理](#票卡申请管理)
  - [LCC.CH现金管理子系统](#lccch现金管理子系统)
    - [备用金查询统计](#备用金查询统计)
  - [LCC.PM参数管理子系统](#lccpm参数管理子系统)
    - [MLC参数](#mlc参数)
    - [参数管理](#参数管理)
- [20230211数据库查询修改](#20230211数据库查询修改)
- [数据库根据设备编号查询线路](#数据库根据设备编号查询线路)
  - [车票跟踪-TVM故障退款明细查询](#车票跟踪-tvm故障退款明细查询)
  - [车票跟踪-BOM故障退款明细查询](#车票跟踪-bom故障退款明细查询)
  - [车票跟踪-非营业性收入](#车票跟踪-非营业性收入)
  - [设备跟踪-钱箱更换记录](#设备跟踪-钱箱更换记录)
  - [设备跟踪-票箱更换记录](#设备跟踪-票箱更换记录)
- [现金管理子系统](#现金管理子系统)
  - [备用金查询统计-备用金调入调出统计](#备用金查询统计-备用金调入调出统计)
  - [备用金查询统计-备用金领用上缴统计](#备用金查询统计-备用金领用上缴统计)
  - [备用金查询统计-日备用金余额查询](#备用金查询统计-日备用金余额查询)
- [监控管理子系统](#监控管理子系统)
  - [综合设置-车站布局绘制](#综合设置-车站布局绘制)
- [修改6022报文头](#修改6022报文头)
- [批量下发软件](#批量下发软件)
  - [全部设备下发](#全部设备下发)
- [更新登录缓存](#更新登录缓存)
- [操作员权限](#操作员权限)
  - [LCC.MN监控管理子系统](#lccmn监控管理子系统-1)
    - [系统监控](#系统监控)
    - [设备监控](#设备监控-1)
    - [客流监控](#客流监控)
    - [综合设置](#综合设置)
  - [LCC.RP报表管理子系统](#lccrp报表管理子系统-1)
    - [收益类](#收益类-1)
    - [客流类](#客流类-1)
    - [综合类](#综合类-1)
    - [全收益](#全收益-1)
  - [LCC.TK票务管理子系统](#lcctk票务管理子系统-1)
    - [车票跟踪](#车票跟踪-1)
    - [设备跟踪](#设备跟踪-1)
    - [交易查询统计](#交易查询统计-1)
    - [对账数据查询](#对账数据查询-1)
    - [设备跟踪](#设备跟踪-2)
    - [票卡申请管理](#票卡申请管理-1)
  - [LCC.ST库存管理](#lccst库存管理)
    - [票卡库存](#票卡库存)
    - [票卡调配](#票卡调配)
  - [LCC.CH现金管理子系统](#lccch现金管理子系统-1)
    - [备用金查询统计](#备用金查询统计-1)
    - [运营现金查询统计](#运营现金查询统计)
  - [LCC.PM参数管理子系统](#lccpm参数管理子系统-1)
    - [MLC参数](#mlc参数-1)
    - [参数管理](#参数管理-1)
- [剩余杂项修改](#剩余杂项修改)


# 6022报文
DLMLC.HHJTInterface.cs line10067
```c#
    //HHJT_ZDD 20230103 程序软件下载命令
    public class C6022Req_t //6022程序软件下载命令
    {
        public byte[] ExcuNID;//执行方节点ID
        public HHJT_STRING FTProute;//FTP路径
        public HHJT_STRING Filename;//文件名
        public HHJT_STRING RFU;//保留
        public C6022Req_t() { }
        public C6022Req_t(byte[] arg_ExcuNID, HHJT_STRING arg_FTProute, HHJT_STRING arg_Filename, HHJT_STRING arg_RFU)
        {
            ExcuNID = arg_ExcuNID;
            FTProute = arg_FTProute;
            Filename = arg_Filename;
            RFU = arg_RFU;
        }
        public void xdr_decode(XDRStream xdrs)
        {
            ExcuNID = new byte[4];
            for (int i_v = 0; i_v < 4; i_v++)
            {
                ExcuNID[i_v] = xdrs.Xdr_decode_byte();
            }
            FTProute = new HHJT_STRING(64);
            FTProute.Xdr_decode(xdrs);

            Filename = new HHJT_STRING(16);
            Filename.Xdr_decode(xdrs);

            RFU = new HHJT_STRING(32);
            RFU.Xdr_decode(xdrs);

        }

        public void xdr_encode(XDRStream xdrs)
        {
            for (int i_v = 0; i_v < 4; i_v++)
            {
                ExcuNID[i_v].Xdr_encode(xdrs);
            }
            FTProute.Xdr_encode(xdrs);
            Filename.Xdr_encode(xdrs);
            RFU.Xdr_encode(xdrs);
        }

    }

    public class P6022Req //0x600D调票结果通知指令 请求报文
    {
        public PackageHeader_t Header;//
        public C6022Req_t Body6022Req;//
        public PackageCrc_t CRC;//
        public P6022Req() { }
        public P6022Req(PackageHeader_t arg_Header, C6022Req_t arg_Body6022Req, PackageCrc_t arg_CRC)
        {
            Header = arg_Header;
            Body6022Req = arg_Body6022Req;
            CRC = arg_CRC;
        }
        public void xdr_decode(XDRStream xdrs)
        {
            Header = new PackageHeader_t();
            Header.xdr_decode(xdrs);
            Body6022Req = new C6022Req_t();
            Body6022Req.xdr_decode(xdrs);
            CRC = new PackageCrc_t();
            CRC.xdr_decode(xdrs);
        }

        public void xdr_encode(XDRStream xdrs)
        {
            Header.xdr_encode(xdrs);
            Body6022Req.xdr_encode(xdrs);
            CRC.xdr_encode(xdrs);
        }

    }
```

# 下载命令

PM.0304.SoftQuery.cs line359

```c#
private void Download()
        {
            Dlg_Select_Device dlg = new Dlg_Select_Device
            {
                Owner = Application.Current.MainWindow
            };

            string device_nid, file_name;
            if (dlg.ShowDialog() == true)
            {
                device_nid = dlg.Data.DeviceSelect;
                file_name = dlg.Data.SoftSelect;
            }
            else
            {
                MessageBox.Show("有信息未选择，无法进行更新！");
                return;
            }
            /*
            System.Windows.Forms.FolderBrowserDialog path = new System.Windows.Forms.FolderBrowserDialog
            {
                Description = "请选择需导入的参数所在文件夹",
                ShowNewFolderButton = false
            };

            string ParaLocalDir;
            if ((path.ShowDialog()) == System.Windows.Forms.DialogResult.OK)
            {
                ParaLocalDir = path.SelectedPath;
            }
            else
            {
                return;
            }
            */
            //HHJT_ZDD 20230203 发送报文通知

            #region 发送报文通知
            
            DLMLC.XDR.P6022Req p6022 = new DLMLC.XDR.P6022Req
            {
                Header = new DLMLC.XDR.PackageHeader_t()
            };
            p6022.Header.MessageType = 0x6022;
            p6022.Header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
            p6022.Header.PacketLength = 116;
            p6022.Header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
            p6022.Header.DestinationID = new byte[4];
            p6022.Header.MessageID = new byte[4];
            p6022.Header.PacketNbr = new byte[12];
            p6022.Header.CRC = new byte[4];

            string tempFTP = XMLManipulater.GetElementByName(DLMLC_ProjectInfo.ConfigFileName, "FTPSoftDirectory");

            p6022.Body6022Req = new DLMLC.XDR.C6022Req_t
            {
                //excu
                ExcuNID = DataConverter.StrHex2Bytes(device_nid),
                FTProute = new HHJT_STRING(64, tempFTP),
                Filename = new HHJT_STRING(16, file_name),
                RFU = new HHJT_STRING(32, ""),
            };

            p6022.CRC = new DLMLC.XDR.PackageCrc_t
            {
                CRC = new byte[4]
            };

            HHJT.AFC.Base.XDR.XDRStream stream = new HHJT.AFC.Base.XDR.XDRStream();
            p6022.Header.xdr_encode(stream);
            p6022.Body6022Req.xdr_encode(stream);
            p6022.CRC.xdr_encode(stream);
            byte[] PackageFinish = stream.Get_data();

            TcpClient tcpClient = new TcpClient();
            try
            {
                tcpClient.Connect(IPAddress.Parse(LCCConfig.LineCommIP), LCCConfig.LineCommPort);
                NetworkStream networkStream = tcpClient.GetStream();

                networkStream.Write(PackageFinish, 0, PackageFinish.Length);

                byte[] receive = new byte[1024 * 50];
                int rcvLength = networkStream.Read(receive, 0, receive.Length);

                if (rcvLength > 0)
                {
                    MessageBox.Show("程序软件下载命令发送成功");

                    AFC_System_Function.InsertLog(
                                                    string.Format(@"程序软件下载命令发送成功"),
                                                    DateTime.Now.ToString("yyyyMMddHHmmss"),
                                                    ExeResult.Success,
                                                    AFC_LoginUser.CurrentUser.UserID
                                                  );
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("程序软件下载命令发送失败:" + ex.Message);
            }
            finally
            {
                tcpClient.Close();
            }
            
            #endregion
        }
```

# 下载UI
Base.SystemFunction.cs line452

```c#
   public class VM_Device_Select : ViewModelBase, IDataErrorInfo
    {
        #region DataResult

        private Device_Select _data;

        public Device_Select Data
        {
            get { return _data; }
            set { _data = value; }
        }

        #endregion

        #region Constructor

        public VM_Device_Select()
        {
            _data = new Device_Select();

        }

        public VM_Device_Select(Device_Select d)
        {
            _data = d;

        }

        #endregion

        #region Combox数据源

        public static KeyValuePair<string, string>[] _ds_LineSelect
                = AFC_SystemDSString.GetKeyValuePairsBySQL(@"    select SUBSTR(LINE_NID,1,2),line_name from tbl_ss_mlc_line   ");

        public KeyValuePair<string, string>[] DS_LineSelect
        {
            get { return _ds_LineSelect; }
            set { _ds_LineSelect = value; }
        }

        public static KeyValuePair<string, string>[] _ds_StationSelect
                = AFC_SystemDSString.GetKeyValuePairsBySQL(@"    select  station_nid, '['||line_id||']'||station_name_cn from tbl_ss_station_info 
                                        where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1   ");

        public KeyValuePair<string, string>[] DS_StationSelect
        {
            get { return _ds_StationSelect; }
            set { _ds_StationSelect = value; }
        }
        
        public static KeyValuePair<string, string>[] _ds_DeviceSelect
                = AFC_SystemDSString.GetKeyValuePairsBySQL(@"    select DEVICE_NID,'['||DEVICE_NID||']'||DEVICE_NAME from TBL_SS_DEVICE order by 1   ");

        public KeyValuePair<string, string>[] DS_DeviceSelect
        {
            get { return _ds_DeviceSelect; }
            set { _ds_DeviceSelect = value; }
        }

        public static KeyValuePair<string, string>[] _ds_SoftSelect
                = AFC_SystemDSString.GetKeyValuePairsBySQL(@"    select FILE_NAME,FILE_NAME from tbl_pm_soft   ");
        public KeyValuePair<string, string>[] DS_SoftSelect
        {
            get { return _ds_SoftSelect; }
            set { _ds_SoftSelect = value; }
        }

        #endregion

        #region View显示绑定

        [AttrDataProperty("线路选择", InputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_LineSelect")]
        public string LineSelect
        {
            get { return _data.LINE_NID; }
            set
            {
                if (value == _data.LINE_NID) return;
                _data.LINE_NID = value;
                base.OnPropertyChanged("LineSelect");
                base.OnPropertyChanged("DSP_StationSelect");
            }
        }
        public KeyValuePair<string, string>[] DSP_LineSelect
        {
            get 
            { 
                return _ds_LineSelect; 
            }
        }

        [AttrDataProperty("站点选择", InputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_StationSelect")]
        public string StationSelect
        {
            get 
            {
                return _data.STATION_NID;
            }
            set
            {
                if (value == _data.STATION_NID) return;
                _data.STATION_NID = value;
                base.OnPropertyChanged("StationSelect");
                base.OnPropertyChanged("DSP_DeviceSelect");
            }
        }

        public KeyValuePair<string, string>[] DSP_StationSelect
        {
            get 
            {

                if (LineSelect == "-1" || LineSelect == null)
                {
                    return _ds_StationSelect;
                }
                else
                {
                    return _ds_StationSelect.Where(d => d.Key.Substring(0, 2) == LineSelect).ToArray();
                }
            }
        }
        
        [AttrDataProperty("设备选择", InputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_DeviceSelect")]
        public string DeviceSelect
        {
            get { return _data.DEVICE_NID; }
            set
            {
                if (value == _data.DEVICE_NID) return;
                _data.DEVICE_NID = value;
                base.OnPropertyChanged("DeviceSelect");
            }
        }

        public KeyValuePair<string, string>[] DSP_DeviceSelect
        {
            get
            {
                if (StationSelect == "-1" || StationSelect == null)
                {
                    return _ds_DeviceSelect;
                }
                else
                {
                    return _ds_DeviceSelect.Where(d => d.Key.Substring(0, 4) == StationSelect).ToArray();
                }
            }
        }

        [AttrDataProperty("软件包选择", InputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_SoftSelect")]
        public string SoftSelect
        {
            get { return _data.FILE_NAME; }
            set
            {
                if (value == _data.FILE_NAME) return;
                _data.FILE_NAME = value;
                base.OnPropertyChanged("SoftSelect");
            }
        }
        public KeyValuePair<string, string>[] DSP_SoftSelect
        {
            get
            {
                return _ds_SoftSelect;
            }
        }


        #endregion
        private bool isUpdate = false;
        public bool IsUpdate
        {
            get { return isUpdate; }
            set { isUpdate = value; }
        }
        #region IDataErrorInfo

        protected List<string> _errorList = new List<string>();

        public string Error
        {
            get
            {
                if (_errorList.Count > 0)
                {
                    return "有错误";
                }
                else
                    return null;
            }
        }

        public string this[string columnName]
        {
            get
            {
                _errorList.Remove(columnName);
                string strError = null;

                if (columnName == "LineSelect")
                {
                    // string pattern = "[0-9]{10}"; //必须为10位
                    string pattern = "[0-9]";
                    if (string.IsNullOrWhiteSpace(LineSelect))
                    {
                        strError = "线路不能为空";
                    }
                }


                if (strError != null)
                {
                    _errorList.Add(columnName);
                }
                return strError;
            }
        }

        #endregion

    }


    public class Device_Select
    {
        [ColumnInfo("LINE_NID")]
        public string LINE_NID { get; set; }//线路NID

        [ColumnInfo("STATION_NID")]
        public string STATION_NID { get; set; }//车站NID

        [ColumnInfo("DEVICE_NID")]
        public string DEVICE_NID { get; set; }//设备NID

        [ColumnInfo("FILE_NAME")]
        public string FILE_NAME { get; set; }//软件名称

    }

    public class Dlg_Select_Device : HHJT_AFC_UI_Dialog
    {
        VM_Device_Select _data = new VM_Device_Select();

        private List<AttrDataPropertyAttribute> props;

        public VM_Device_Select Data
        {
            get { return _data; }
            set { _data = value; }
        }


        public Dlg_Select_Device()
        {
            Title = "选择设备";
            LabelTitle.Content = "选择对应设备";
            MaxHeight = 450;
            Grid grid = new Grid();
            props = new List<AttrDataPropertyAttribute>();
            props = AutoUI.AutoGenerateDialogGridContent(typeof(VM_Device_Select), props);
            props = AutoUI.AutoGenerateDialogLayoutPanel(props, grid, 2, false);

            grid.DataContext = _data;
            PanelMainContent.Children.Add(grid);
            DockPanel.SetDock(grid, Dock.Left);

            RegisterFunction("选择", "A");
        }

        protected override void funcRegistered(string strAccelerateKey)
        {
            switch (strAccelerateKey)
            {
                case "A":

                    if (_data.Error != null)
                    {
                        MessageBox.Show("输入有误！", "确认", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    if (MessageBox.Show("确认选择？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) == MessageBoxResult.OK)
                    {
                        DialogResult = true;
                    }
                    if (DialogResult == true)
                    {
                        this.Close();
                    }
                    break;

                default:
                    break;
            }
        }
    }
```



# 参数管理前两项选择添加换乘站

Parm.AFC.UI.ViewEdit.cs line226
```c#
 List<ParamCtrlNID> data = new List<ParamCtrlNID>();
            string strSQL = @"SELECT STATION_NID||'6101' NID, STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_STATION_INFO where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) UNION ALL SELECT SPECIAL_STATION_NID||'6101' NID, SPECIAL_STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_SPECIAL_STATION where special_line_nid in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line)";
```


# 数据库

Base.SystemDS.cs line334
```c#
case DSString.LineStations:
                    strSQL += @"  select  station_nid, '['||line_id||']'||station_name_cn from tbl_ss_station_info 
                                        where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) union all select  special_station_nid, '['||line_id||']'||station_name_cn from tbl_ss_special_station 
                                        where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1";
                    break;
                case DSString.HHJTStations:
                    strSQL += @" select  station_nid, '['||line_id||']'||station_name_cn from tbl_ss_station_info 
                            where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) union all select  special_station_nid, '['||line_id||']'||station_name_cn from tbl_ss_special_station 
                                        where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1";
```

# 6101 & 6121
Param.AFC_UC_ViewEdit.cs line227

```c#
string strSQL = @"SELECT STATION_NID||'6121' NID, STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_STATION_INFO where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) UNION ALL SELECT SPECIAL_STATION_NID||'6121' NID, SPECIAL_STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_SPECIAL_STATION where special_line_nid in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line)";
```

~RP.0101.Transtype.cs line190~
```c#
if (LineId!=null&&!"-1".Equals(LineId))
            {
                if (StationId.Substring(0, 2) != LineId)
                {
                    sb.AppendFormat(@" and line_nid='{0}' ", StationId.Substring(0, 2));
                }
                else sb.AppendFormat(@" and line_nid='{0}' ", LineId);
            }
```

~RP.0101.Transtype.cs line104~
```c#
public KeyValuePair<string, string>[] DSP_LineStation
        {
            get
            {
                if (DS_LineStation == null || DS_LineStation.Count() == 0)
                {
                    return null;
                }
                else
                {
                    if (LineId == "-1")
                    {
                        return DS_LineStation;
                    }
                    else if(LineId == "05")
                    {
                        return DS_LineStation.Where(d => d.Key.Substring(0, 2) != "0D" || d.Key == "-1" ).ToArray();
                    }
                    else
                    {
                        return DS_LineStation.Where(d => d.Key.Substring(0, 2) == LineId || d.Key == "-1").ToArray();
                    }
                }
            }
        }
```



# 配置
- MLC：172.210.200.1 metro hhjt2777
- SC: 172.5.201.222/223 root DLmetro#5  metro !!11qqaazz 
- SCDB: 172.5.201.223 MariaDB scdb

# oracle 
SS.0101.user.cs line925

```c#
public bool Add(List<string> checkedStation, List<int> checkedRole, List<string> checkedDevice)
        {
            OracleConnection conn = OracleDB.GetDBConnection();
            this.CreateTime = this.UpdateTime = this.UseTime = DateTime.Now;
            this.CreateUser = AFC_LoginUser.CurrentUser.UserID;
            string current = CreateTime.ToString("yyyyMMddHHmmss");
            conn.Open();
            OracleCommand cmdInsert1 = new OracleCommand(string.Format(@"INSERT INTO  TBL_SS_USER 
(USER_ID, USER_NAME, COMPANY_NAME, 
   USER_PIN, USE_TIME, VALID_TIME, 
   DEVICE_PRIV, CREATE_TIME, CREATE_USER, 
   MEMO, UPDATE_TIME)
VALUES('{0}','{1}',{2},'{3}',to_date('{4}','yyyyMMddHH24miss'),to_date('{5}','yyyyMMddHH24miss'),
null,to_date('{6}','yyyyMMddHH24miss'),'{7}','{8}',to_date('{9}','yyyyMMddHH24miss'))",
                                                this.UserId, this.UserName, this.CompanyName,
                                                CryptFuncs.Md5Crypt(LCCConfig.DefaultPWD),
                                                this.UseTime.ToString("yyyyMMddHHmmss"),
                                                this.ValidTime.ToString("yyyyMMddHHmmss"), current,
                                                CreateUser, this.Memo, current),
                                  conn);
            OracleCommand cmdInsert2 =
                new OracleCommand(
                    string.Format(
                        "INSERT INTO TBL_SS_USER_TO_STATION(OPERATOR_ID,STATION_NID) VALUES('{0}',:stationNid)",
                        this.UserId), conn);
            cmdInsert2.Parameters.Add(OracleDB.CreateOracleParameter("stationNid", OracleDbType.Varchar2, 4));
            //OracleCommand cmdInsert3 =
            //    new OracleCommand(
            //        string.Format(
            //            "INSERT INTO TBL_SS_USER_TO_DEVICE(OPERATOR_ID,DEVICE_TYPE) VALUES('{0}',:deviceType)",
            //            this.UserId), conn);
            //cmdInsert3.Parameters.Add(OracleDB.CreateOracleParameter("deviceType", OracleDbType.Varchar2, 2));
            OracleCommand cmdInsert4 =
               new OracleCommand(
                   string.Format(
                       "INSERT INTO TBL_SS_USER_TO_ROLE(USER_ID,ROLE_ID) VALUES('{0}',:role_id)",
                       this.UserId), conn);
            cmdInsert4.Parameters.Add(OracleDB.CreateOracleParameter("role_id", OracleDbType.Int32, 3));
            
            OracleTransaction tx = null;
            try
            {
                tx = (OracleTransaction)conn.BeginTransaction();
                ;
                //将命令加入到事务
                cmdInsert1.Transaction = tx;
                cmdInsert2.Transaction = tx;
                //cmdInsert3.Transaction = tx;
                cmdInsert4.Transaction = tx;

                //执行命令
                cmdInsert1.ExecuteNonQuery();
                foreach (var item in checkedStation)
                {
                    cmdInsert2.Parameters["stationNid"].Value = item;
                    cmdInsert2.ExecuteNonQuery();
                }
                //foreach (var item in checkedDevice)
                //{
                //    cmdInsert3.Parameters["deviceType"].Value = item;
                //    cmdInsert3.ExecuteNonQuery();
                //}
                foreach (var item in checkedRole)
                {
                    cmdInsert4.Parameters["role_id"].Value = item;
                    cmdInsert4.ExecuteNonQuery();
                }

                //提交
                tx.Commit();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                //有任何错误都会回滚事务
                tx.Rollback();
                return false;
            }
            finally
            {
                conn.Close();
            }
        }
        #endregion
        #region update

        public bool Update(List<string> checkedStation, List<int> checkedRole, List<string> checkedDevice)
        {
            OracleConnection conn = (OracleConnection)OracleDB.GetDBConnection();
            string current = CreateTime.ToString("yyyyMMddHHmmss");
            conn.Open();
            OracleCommand cmdInsert1 =
                new OracleCommand(string.Format(@"
update TBL_SS_USER set USER_NAME='{0}',COMPANY_NAME='{1}',
VALID_TIME=to_date('{2}','yyyyMMddHH24miss'),
MEMO='{3}' where USER_ID='{4}'", UserName, CompanyName, this.ValidTime.ToString("yyyyMMddHHmmss"), Memo, UserId),
                                  conn);
            OracleCommand cmdInsert5 =
    new OracleCommand(
        string.Format("DELETE TBL_SS_USER_TO_STATION where OPERATOR_ID='{0}'",
                      this.UserId), conn);
            //OracleCommand cmdInsert6 =
            //    new OracleCommand(
            //        string.Format("DELETE TBL_SS_USER_TO_DEVICE where OPERATOR_ID='{0}'",
            //                      this.UserId), conn);
            OracleCommand cmdInsert7=
                new OracleCommand(
                    string.Format("DELETE TBL_SS_USER_TO_ROLE where USER_ID='{0}'",
                                  this.UserId), conn);
            OracleCommand cmdInsert2 =
                new OracleCommand(
                    string.Format(
                        "INSERT INTO TBL_SS_USER_TO_STATION(OPERATOR_ID,STATION_NID) VALUES('{0}',:stationNid)",
                        this.UserId), conn);
            cmdInsert2.Parameters.Add(OracleDB.CreateOracleParameter("stationNid", OracleDbType.Varchar2, 4));
            //OracleCommand cmdInsert3 =
            //    new OracleCommand(
            //        string.Format(
            //            "INSERT INTO TBL_SS_USER_TO_DEVICE(OPERATOR_ID,DEVICE_TYPE) VALUES('{0}',:deviceType)",
            //            this.UserId), conn);
            //cmdInsert3.Parameters.Add(OracleDB.CreateOracleParameter("deviceType", OracleDbType.Varchar2, 2));
            OracleCommand cmdInsert4 =
               new OracleCommand(
                   string.Format(
                       "INSERT INTO TBL_SS_USER_TO_ROLE(USER_ID,ROLE_ID) VALUES('{0}',:role_id)",
                       this.UserId), conn);
            cmdInsert4.Parameters.Add(OracleDB.CreateOracleParameter("role_id", OracleDbType.Int32, 3));

            OracleTransaction tx = null;
            try
            {
                tx = (OracleTransaction)conn.BeginTransaction();
                ;
                //将命令加入到事务
                cmdInsert1.Transaction = tx;
                cmdInsert2.Transaction = tx;
                //cmdInsert3.Transaction = tx;
                cmdInsert4.Transaction = tx;
                cmdInsert5.Transaction = tx;
                //cmdInsert6.Transaction = tx;
                cmdInsert7.Transaction = tx;

                //执行命令
                cmdInsert1.ExecuteNonQuery();
                cmdInsert5.ExecuteNonQuery();
                //cmdInsert6.ExecuteNonQuery();
                cmdInsert7.ExecuteNonQuery();
                foreach (var item in checkedStation)
                {
                    cmdInsert2.Parameters["stationNid"].Value = item;
                    cmdInsert2.ExecuteNonQuery();
                }
                //foreach (var item in checkedDevice)
                //{
                //    cmdInsert3.Parameters["deviceType"].Value = item;
                //    cmdInsert3.ExecuteNonQuery();
                //}
                foreach (var item in checkedRole)
                {
                    cmdInsert4.Parameters["role_id"].Value = item;
                    cmdInsert4.ExecuteNonQuery();
                }

                //提交
                tx.Commit();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                //有任何错误都会回滚事务
                tx.Rollback();
                return false;
            }
            finally
            {
                conn.Close();
            }
        }
        #endregion

    
```

# 报表特殊线路判断
Base.SystemDS.cs line39, 287, 337
```c#
enum {
    SpecialStations,
}

public static bool IsTransfer(KeyValuePair<string,string>[] DS_Change, string line)
{
    for (int i = 0; i < DS_Change.Length; i++)
    {
        if (DS_Change[i].Value == line) return true;
    }
    return false;
}
public static bool IsTransfer(KeyValuePair<string,string>[] DS_Change, string line, string station_id)
{
    for (int i = 0; i < DS_Change.Length; i++)
    {
        if (DS_Change[i].Key == station_id && line == DS_Change[i].Value) return true;
    }
    return false;
}

case DSString.SpecialStations:
                    strSQL += @" select special_station_nid, special_line_nid from tbl_ss_special_station
                                          where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1";
                    break;
```

RP.0101.Transtype.cs line104
```c#

/*public KeyValuePair<string, string>[] DS_Change { get; } = AFC_SystemDSString.GetKeyValuePairsBySQL(@"
                        select special_station_nid, '['||line_id||']'||station_name_cn from tbl_ss_special_station
                                          where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1
                         ");*/
public KeyValuePair<string, string>[] DS_Change { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.SpecialStations, true);



//判断
if (LineId == "-1")
{
    return DS_LineStation;
}
else
{
    return DS_LineStation.Where(d => d.Key.Substring(0, 2) == LineId || d.Key == "-1" || AFC_SystemDSString.IsTransfer(DS_Change,LineId)).ToArray();
}
```

TK.0401.CashBox.cs line103
```c#
public KeyValuePair<string, string>[] DS_Change { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.SpecialStations, true);

if (cbbLineID.SelectedItem == null)
            {
                return;
            }
            else if (cbbLineID.SelectedIndex == 0)
            {
                cbbStationID.ItemsSource = _view.DS_LineStation;
            }
            else
            {
                cbbStationID.ItemsSource = _view.DS_LineStation.Where(d => d.Key.Substring(0, 2).Equals(((sender as ComboBox).SelectedItem as KeyValuePair<string, string>?).Value.Key)
                    || d.Key == "-1" || AFC_SystemDSString.IsTransfer(DS_Change, ((sender as ComboBox).SelectedItem as KeyValuePair<string, string>?).Value.Key));
            }
```

## LCC.MN监控管理子系统
### 设备监控
设备状态统计  已修改  

## LCC.RP报表管理子系统
### 收益类
RP0101  已修改  
RP0102  已修改  
RP0103  已修改  
RP0104  已修改  
RP0106  已修改  
RP0107  已修改  
RP0108  已修改  
RP0109  已修改  
RP0110  已修改  
RP0111  已修改  
RP0112  已修改  
RP0113  已修改  
RP0114  已修改  
RP0115  已修改  
RP0120  已修改  
### 客流类
RP0201  已修改  
RP0202  已修改  
RP0207  已修改  
### 综合类
RP0301  已修改  
RP0302  已修改  
RP0303  已修改  
RP0304  已修改  
RP0305  已修改  
RP0306  已修改
RP0307  已修改    
### 全收益
RP0501  已修改  
RP0503  已修改  
RP0504  已修改  
RP0505  已修改  
RP0509  已修改  
RP0510  已修改  
RP0512  已修改  

## LCC.TK票务管理子系统
### 车票跟踪
TK0101  已修改  
TK0103  已修改  
TK0104  已修改  
TK0105  已修改  
### 设备跟踪
TK0401  已修改  
TK0402  已修改  
### 交易查询统计
TK0201  已修改  
TK0202  已修改  
TK0203  已修改  
TK0204  已修改  
TK0205  已修改  
TK0206  已修改  
TK0207  已修改  
TK0208  已修改  
TK0209  已修改  **青泥洼桥BOM数据暂时缺失-数据库中无数据**
TK0210  已修改  
TK0211  已修改  
TK0212  已修改  
### 对账数据查询
TK0306  已修改  
### 票卡申请管理
TK0501  已修改  
TK0502  已修改  

## LCC.CH现金管理子系统
### 备用金查询统计
CH0301  已修改  
CH0302  已修改  
CH0303  已修改  
CH0304  已修改  
CH0305  已修改  

## LCC.PM参数管理子系统
### MLC参数
PM02  已修改  
### 参数管理
PM0301  已修改  
PM0304  已修改  

# 20230211数据库查询修改
Base.SystemDS.cs line396
```c#
case DSString.ST_STATIONS:
                    strSQL += @" select '0200','线路中心' from dual union all select station_nid, station_name_cn from tbl_ss_station_info
                                    where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) union all select special_station_nid, station_name_cn from tbl_ss_special_station
                                    where special_line_nid in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) order by 1 ";
                    break;
```

# 数据库根据设备编号查询线路
## 车票跟踪-TVM故障退款明细查询
TK.0103.FaultRefund.cs line345
```c#
strSQL.Append(@"SELECT  PACKET_SEQ,CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID),SUBSTR(DEVICE_NID,1,4),SUBSTR(DEVICE_NID,5,2),DEVICE_NID,
                                    OCCUR_TIME,ERROR_SEQ,ERROR_NO,VALUE_TRANS/100,ERROR_REASON 
                            FROM TBL_MN_5020_TVM where rownum < 5000 ");
if (LineNid != null && !"-1".Equals(LineNid))
{
    strSQL.AppendFormat(" AND  CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}%' ", LineNid);
}
```
## 车票跟踪-BOM故障退款明细查询
TK.0104.FaultRefund.cs line370
```c#
strSQL.Append(@"SELECT PACKET_SEQ, CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID), SUBSTR(DEVICE_NID,1,4),SUBSTR(DEVICE_NID,5,2),DEVICE_NID, 
                OCCUR_TIME, TRANS_SEQ, OPERATOR_ID, FAULT_DEVICE_NID, ERROR_NO, TRANS_VALUE/100, PAYMENT_METHOD
                FROM TBL_MN_5021_BOM where rownum < 5000 ");

if (LineNid != null && !"-1".Equals(LineNid))
{
    strSQL.AppendFormat(" AND  PACKET_SEQ,CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}%' ", LineNid);
}
```
## 车票跟踪-非营业性收入
TK.0105.UnbusinessInout.cs line457
```c#
strSQL.Append(@"SELECT  PACKET_SEQ,CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID), SUBSTR(DEVICE_NID,1,4),SUBSTR(DEVICE_NID,5,2),DEVICE_NID,
                    OCCUR_TIME, TRANS_SEQ, OPERATOR_ID, INOUT_FLAG, INOUT_REASON, TRANS_VALUE, PAYMENT_METHOD, MEMO, 
                    SALE_TICKET_FLAG, SALE_TICKET_TYPE, SALE_TRANS_VALUE, SALE_PAYMENT_METHOD, SALE_CARD_PHY_ID, SALE_DEVICE_TRACE_NBR
                  FROM TBL_MN_5022_BOM where rownum < 5000 ");

            if (LineNid != null && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND  CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}%' ", LineNid);
            }
```
## 设备跟踪-钱箱更换记录
TK.0401.CashBox.cs line557 line575
```c#
ObservableCollection<LCC_TK0401_CashBox_Result> data = new ObservableCollection<LCC_TK0401_CashBox_Result>();
            StringBuilder strSQL = new StringBuilder();
            strSQL.Append("SELECT ");
            strSQL.Append("CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID),");
            strSQL.Append("substr(DEVICE_NID,1,4),");
            strSQL.Append("DEVICE_NID,");
            strSQL.Append("OPERATOR_ID,");
            strSQL.Append("b.user_name,");

            strSQL.Append("CHANGE_TIME,");
            strSQL.Append("CHANGE_SEQ,");
            strSQL.Append("OPERATE_TYPE,");
            strSQL.Append("CASHBOX_ID,");
            strSQL.Append("LOC_ID,");
            strSQL.Append("CHANGE_STATUS,");
            strSQL.Append("upper(BILL_TYPE),");
            strSQL.Append("BILL_COUNT");
            strSQL.Append(" FROM TBL_DM_4002_CASHBOX a left outer join tbl_ss_user b " +
              "on a.operator_id=b.user_id WHERE 1=1 ");
            if (!string.IsNullOrWhiteSpace(LineNid) && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}%'", LineNid);
            }
            if (!string.IsNullOrWhiteSpace(StationId) && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND DEVICE_NID like '{0}%'", StationId);
            }
            if (!string.IsNullOrWhiteSpace(DeviceNid))
            {
                strSQL.AppendFormat(" AND DEVICE_NID = '{0}'", DeviceNid);
            }
            if (!string.IsNullOrWhiteSpace(OperatorId))
            {
                strSQL.AppendFormat(" AND OPERATOR_ID like '%{0}%'", OperatorId.Trim());
            }
            if (!string.IsNullOrWhiteSpace(OperatorName))
            {
                strSQL.AppendFormat(" AND b.user_name like '%{0}%'", OperatorName.Trim());
            }
            if (ChangeTimeStart != null)
            {
                strSQL.AppendFormat(" AND CHANGE_TIME>to_date('{0}','yyyyMMddHH24miss')", ((DateTime)ChangeTimeStart).ToString("yyyyMMddHHmmss"));
            }
            if (ChangeTimeEnd != null)
            {
                strSQL.AppendFormat(" AND CHANGE_TIME<to_date('{0}','yyyyMMddHH24miss')", ((DateTime)ChangeTimeEnd).ToString("yyyyMMddHHmmss"));
            }
            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_TK0401_CashBox_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
```
## 设备跟踪-票箱更换记录
TK.0402.TicketBox.cs line529 line546
```c#
ObservableCollection<LCC_TK0402_TicketBox_Result> data = new ObservableCollection<LCC_TK0402_TicketBox_Result>();
            StringBuilder strSQL = new StringBuilder();
            strSQL.Append("SELECT ");
            strSQL.Append("CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID), ");
            strSQL.Append("substr(DEVICE_NID,1,4),");
            strSQL.Append("DEVICE_NID,");
            strSQL.Append("OPERATOR_ID,");
            strSQL.Append("b.user_name,");
            strSQL.Append("CHANGE_TIME,");
            strSQL.Append("CHANGE_SEQ,");
            strSQL.Append("OPERATE_TYPE,");
            strSQL.Append("TICKETBOX_ID,");
            strSQL.Append("LOC_ID,");
            strSQL.Append("TICKET_TYPE,");
            strSQL.Append("TICKET_COUNT,");
            strSQL.Append("CHANGE_STATUS");
            strSQL.Append(" FROM TBL_DM_4003_TICKETBOX a left outer join tbl_ss_user b " +
                          "on a.operator_id=b.user_id WHERE 1=1 ");
            if (!string.IsNullOrWhiteSpace(LineNid) && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}%'", LineNid);
            }
            if (!string.IsNullOrWhiteSpace(StationId) &&!"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND DEVICE_NID like '{0}%'", StationId);
            }
            if (!string.IsNullOrWhiteSpace(DeviceNid))
            {
                strSQL.AppendFormat(" AND DEVICE_NID = '{0}'", DeviceNid);
            }
            if (!string.IsNullOrWhiteSpace(OperatorId))
            {
                strSQL.AppendFormat(" AND OPERATOR_ID like '%{0}%'", OperatorId.Trim());
            }
            if (!string.IsNullOrWhiteSpace(OperatorName))
            {
                strSQL.AppendFormat(" AND b.user_name like '%{0}%'", OperatorName.Trim());
            }
            if (ChangeTimeStart != null)
            {
                strSQL.AppendFormat(" AND CHANGE_TIME>to_date('{0}','yyyyMMddHH24miss')", ((DateTime)ChangeTimeStart).ToString("yyyyMMddHHmmss"));
            }
            if (ChangeTimeEnd != null)
            {
                strSQL.AppendFormat(" AND CHANGE_TIME<to_date('{0}','yyyyMMddHH24miss')", ((DateTime)ChangeTimeEnd).ToString("yyyyMMddHHmmss"));
            }
            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_TK0402_TicketBox_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
```
# 现金管理子系统
## 备用金查询统计-备用金调入调出统计
CH.0304.CashInOut.Stat.cs line349 line 360
```c#
ObservableCollection<LCC_CH_0304_CashInOut_Stat_Result> data = new ObservableCollection<LCC_CH_0304_CashInOut_Stat_Result>();
            StringBuilder strSQL = new StringBuilder();
            //需补充
            strSQL.Append(@"  SELECT operator_id,CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID),substr(device_nid,1,4),device_nid,
                                             NVL(SUM (DECODE (operate_type, 2, INOUT_AMOUNT)),0)+NVL(SUM (DECODE (operate_type, 6, INOUT_AMOUNT)),0),
                                             NVL(SUM (DECODE (operate_type, 1, INOUT_AMOUNT)),0)
                                        FROM TBL_CH_PETTYCASH_INOUT WHERE 1=1 ");
                if (DateStart != null && DateEnd != null)
                {
                    strSQL.AppendFormat(" AND (INOUT_TIME>=TO_DATE('{0}','YYYYMMDDHH24MISS') AND INOUT_TIME<=TO_DATE('{1}','YYYYMMDDHH24MISS'))",
                        ((DateTime)DateStart).ToString("yyyyMMddHHmmss"), ((DateTime)DateEnd).ToString("yyyyMMddHHmmss")
                        );
            }
            if (!string.IsNullOrWhiteSpace(LineNid) && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}%' ", LineNid);
            }
            if (!string.IsNullOrWhiteSpace(StationId) && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND DEVICE_NID like '{0}%' ", StationId);
            }
            //if (OperatorID != null&&!"-1".Equals(OperatorID))
            //{
            //    strSQL.AppendFormat(" AND operator_id = {0}", OperatorID);
            //}
            //if (DeviceNID != null&&!"-1".Equals(DeviceNID))
            //{
            //    strSQL.AppendFormat(" AND device_nid = '{0}' ", DeviceNID);
            //}
            strSQL.Append(" GROUP BY operator_id, device_nid");

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_CH_0304_CashInOut_Stat_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
```
## 备用金查询统计-备用金领用上缴统计
CH.0305.CashSupplyExtract.Stat.cs line327 line344 line350
```c#
            ObservableCollection<LCC_CH_0305_CashSupplyExtract_Stat_Result> data = new ObservableCollection<LCC_CH_0305_CashSupplyExtract_Stat_Result>();
            StringBuilder strSQL = new StringBuilder();
            //需补充
            strSQL.Append(@"  SELECT SE_OPERATOR operator,line_nid,station_nid,
                                        SUM (DECODE (se_type, 3, SE_AMOUNT)),
                                        SUM (DECODE (se_type, 4, SE_AMOUNT)),
                                        SUM (DECODE (se_type, 5, SE_AMOUNT))
                                FROM TBL_CH_PETTYCASH_SE WHERE 1=1 ");
            if (DateStart != null && DateEnd != null)
            {
                strSQL.AppendFormat(" AND (SE_TIME>=TO_DATE('{0}','YYYYMMDDHH24MISS') AND SE_TIME<=TO_DATE('{1}','YYYYMMDDHH24MISS')) ",
                    ((DateTime)DateStart).ToString("yyyyMMddHHmmss"), ((DateTime)DateEnd).ToString("yyyyMMddHHmmss")
                    );
            }
            //if (OperatorID != null&&!"-1".Equals(OperatorID))
            //{
            //    strSQL.AppendFormat(" AND operator_id = {0}", OperatorID);
            //}
            if (!string.IsNullOrWhiteSpace(LineNid) && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND LINE_NID like '{0}%' ", LineNid);
            }
            if (!string.IsNullOrWhiteSpace(StationId) && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND STATION_NID = '{0}' ", StationId);
            }
            strSQL.Append("GROUP BY SE_OPERATOR,line_nid, station_nid");

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_CH_0305_CashSupplyExtract_Stat_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
```
## 备用金查询统计-日备用金余额查询
CH.0301.CashStore.cs line307 line308 line330
```c#
 /*ObservableCollection<LCC_CH_0301_CashStore_Result> data = new ObservableCollection<LCC_CH_0301_CashStore_Result>(); 
            StringBuilder strSQL = new StringBuilder();
            //需补充
            strSQL.Append(@"select substr(station_nid,1,2),station_nid,settle_date,pettycash_amount,prior_value,diff from (");
            strSQL.AppendFormat(@"select station_nid,settle_date, pettycash_amount , nvl(lag(pettycash_amount,1,null) over( partition by station_nid order by settle_date),0) prior_value,  
                pettycash_amount-nvl(lag(pettycash_amount,1,null) over( partition by station_nid order by settle_date),0) diff
                from TBL_CH_PETTYCASH_BALANCE_HIS
                where rownum < 10000 ");

            if (!string.IsNullOrWhiteSpace(StationId) && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND station_nid = '{0}' ", StationId);
            }
            if (DateStart != null && DateEnd != null)
            {
                strSQL.AppendFormat(" AND SETTLE_DATE >= '{0}' AND SETTLE_DATE <= {1}", FormatOutput.StrDate(((DateTime)DateStart).AddDays(-1)), FormatOutput.StrDate((DateTime)DateEnd));
            }

            strSQL.Append(" ORDER BY SETTLE_DATE");
            strSQL.Append(") where rownum < 10000 ");
            if (DateStart != null && DateEnd != null)
            {
                strSQL.AppendFormat(" and SETTLE_DATE >= '{0}' AND SETTLE_DATE <= '{1}'", FormatOutput.StrDate((DateTime)DateStart), FormatOutput.StrDate((DateTime)DateEnd));
            }
            if (!string.IsNullOrWhiteSpace(LineNid) &&!"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND station_nid like '{0}%' ", LineNid);
            }

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_CH_0301_CashStore_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }

            return data;*/
 ObservableCollection<LCC_CH_0301_CashStore_Result> data = new ObservableCollection<LCC_CH_0301_CashStore_Result>(); 
            StringBuilder strSQL = new StringBuilder();
            //需补充
            strSQL.Append(@"select line_nid,station_nid,settle_date,pettycash_amount,prior_value,diff from (");
            strSQL.AppendFormat(@"select line_nid,station_nid,settle_date, pettycash_amount , nvl(lag(pettycash_amount,1,null) over( partition by station_nid order by settle_date),0) prior_value,  
                pettycash_amount-nvl(lag(pettycash_amount,1,null) over( partition by station_nid order by settle_date),0) diff
                from TBL_CH_PETTYCASH_BALANCE_HIS
                where rownum < 10000 ");

            if (!string.IsNullOrWhiteSpace(StationId) && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND station_nid = '{0}' ", StationId);
            }
            if (DateStart != null && DateEnd != null)
            {
                strSQL.AppendFormat(" AND SETTLE_DATE >= '{0}' AND SETTLE_DATE <= {1}", FormatOutput.StrDate(((DateTime)DateStart).AddDays(-1)), FormatOutput.StrDate((DateTime)DateEnd));
            }

            strSQL.Append(" ORDER BY SETTLE_DATE");
            strSQL.Append(") where rownum < 10000 ");
            if (DateStart != null && DateEnd != null)
            {
                strSQL.AppendFormat(" and SETTLE_DATE >= '{0}' AND SETTLE_DATE <= '{1}'", FormatOutput.StrDate((DateTime)DateStart), FormatOutput.StrDate((DateTime)DateEnd));
            }
            if (!string.IsNullOrWhiteSpace(LineNid) &&!"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND line_nid like '{0}%' ", LineNid);
            }

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_CH_0301_CashStore_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }

            return data;
```

# 监控管理子系统
## 综合设置-车站布局绘制
MN.0301.UCLineMap.xaml.cs line247
```c#
SELECT A.STATION_NID,  '['||A.line_id||']'||A.STATION_NAME_CN, B.POINT_X, B.POINT_Y, B.TEXT_X, B.TEXT_Y, B.TEXT_ROTATE
                    FROM TBL_SS_STATION_INFO A LEFT OUTER JOIN TBL_MN_LINE_LAYOUT B
                    ON  A.STATION_NID = B.STATION_NID where A.line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line)
UNION ALL SELECT A.SPECIAL_STATION_NID,  '['||A.line_id||']'||A.STATION_NAME_CN, B.POINT_X, B.POINT_Y, B.TEXT_X, B.TEXT_Y, B.TEXT_ROTATE
                    FROM TBL_SS_SPECIAL_STATION A LEFT OUTER JOIN TBL_MN_LINE_LAYOUT B
                    ON  A.SPECIAL_STATION_NID = B.STATION_NID where A.line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line)
```
# 修改6022报文头
```c#
public byte[] PacketFlag;//标识数据包开始，约定为0xABCDEF89
public byte[] SourceID;//数据包发送方节点编码
public byte[] DestinationID;//数据包接收方节点编码
public ushort MessageType;//消息类别，对应命令代码
public byte[] MessageID;//数据包的序号，每发送一个数据包自动增加1
public byte[] PacketNbr;//数据包编号
public uint PacketLength;//本数据包包体长度，不包括CRC
public byte ResponseCode;//是否需要应答
public byte CompressCypherFlag;//是否需要对数据域进行压缩
public uint OrigLength;//可选，若选择压缩，则对应加密前的数据包体长度
public byte MacFlag;//是否需要对数据域进行加密
public byte[] CRC;//可选，若选择加密，则对应加密前的数据包头及数据包体的CRC
```

# 批量下发软件
## 全部设备下发
MN.0101.LineMonitor.UCSoftwareUpdate.cd line127 line133 line153 line185 line240 line284
```c#
private string _fileName;

ObservableCollection<DrawingStationNode> _stationsForSend = new ObservableCollection<DrawingStationNode>();
ObservableCollection<byte[]> _deviceNid = new ObservableCollection<byte[]>();

public ObservableCollection<byte[]> DeviceNid
{
    get { return _deviceNid; }
    set { _deviceNid = value; }
}

Queue<UCLineMonitor.SoftwareForSend> _updateall = new Queue<UCLineMonitor.SoftwareForSend>();

foreach(var item in _stationsForSend.Where(d => d.FlagSelect))
{
    string temp = item.StationID + String.Format("{0:X2}", (int)_uc.cbxDeviceType.SelectedValue) + "00";
    _deviceNid.Add(DataConverter.StrHex2Bytes(temp));
}

if (_uc.rbUpdateTypeAll.IsChecked == true)
{
    if (MessageBox.Show("确认更新选中车站所有设备？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) ==
                MessageBoxResult.OK)
    {
        _dataResult.Clear();
        lock (((ICollection)_updateall).SyncRoot)
        {
            _updateall.Enqueue(new UCLineMonitor.SoftwareForSend {DeviceNid = DeviceNid, FullFileName = FullFileName});
        }
        Thread thread = new Thread(SendLineUpdate);
        thread.Start();
    }
    else
    {
        return;
    }
}

public void SendLineUpdate()
{
    UCLineMonitor.SoftwareForSend softforSend = _updateall.Dequeue();
    lock (((ICollection)_updateall).SyncRoot)//消息同步控制
    {
        StringBuilder sb = new StringBuilder();
        sb.Clear();
        foreach (var item in softforSend.DeviceNid)
        {
            #region 发送报文通知
                DLMLC.XDR.P6022Req p6022 = new DLMLC.XDR.P6022Req
            {
                Header = new DLMLC.XDR.PackageHeader_t()
            };
            p6022.Header.MessageType = 0x6022;
            p6022.Header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
            p6022.Header.PacketLength = 116;
            p6022.Header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
            p6022.Header.DestinationID = new byte[4];
            p6022.Header.MessageID = new byte[4];
            p6022.Header.PacketNbr = new byte[12];
            p6022.Header.CRC = new byte[4];
            p6022.Header.ResponseCode = 0x01;
            p6022.Header.CompressCypherFlag = 0x01;
            p6022.Header.MacFlag = 0x01;

            string tempFTP = XMLManipulater.GetElementByName(DLMLC_ProjectInfo.ConfigFileName, "FTPSoftDirectory");
            p6022.Body6022Req = new DLMLC.XDR.C6022Req_t
            {
                //excu
                ExcuNID = item,
                FTProute = new HHJT_STRING(64, tempFTP),
                Filename = new HHJT_STRING(16, _fileName),
                RFU = new byte[32],
            };

            p6022.CRC = new DLMLC.XDR.PackageCrc_t
            {
                CRC = new byte[4]
            };

            HHJT.AFC.Base.XDR.XDRStream stream = new HHJT.AFC.Base.XDR.XDRStream();
            p6022.Header.xdr_encode(stream);
            p6022.Body6022Req.xdr_encode(stream);
            p6022.CRC.xdr_encode(stream);
            byte[] PackageFinish = stream.Get_data();

            TcpClient tcpClient = new TcpClient();
            try
            {
                tcpClient.Connect(IPAddress.Parse(LCCConfig.LineCommIP), LCCConfig.LineCommPort);
                NetworkStream networkStream = tcpClient.GetStream();

                networkStream.Write(PackageFinish, 0, PackageFinish.Length);

                byte[] receive = new byte[1024 * 50];
                int rcvLength = networkStream.Read(receive, 0, receive.Length);

                if (rcvLength > 0)
                {
                    MessageBox.Show("程序软件下载命令发送成功");

                    AFC_System_Function.InsertLog(
                                                    string.Format(@"程序软件下载命令发送成功"),
                                                    DateTime.Now.ToString("yyyyMMddHHmmss"),
                                                    ExeResult.Success,
                                                    AFC_LoginUser.CurrentUser.UserID
                                                    );
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("程序软件下载命令发送失败:" + ex.Message);
            }
            finally
            {
                tcpClient.Close();
            }
            #endregion

        }
        if (sb.ToString().Length > 0)
        {
            // MessageBox.Show(sb.ToString(), "软件更新通知结果");
        }
        IsCheaking = false;
    }
}
```
MN.0101.LineMonitor.UCUpdateSomeDevice.cd line116 line222 line228 line235 
```c#
Queue<UCLineMonitor.SoftwareForSendSome> _updatesome = new Queue<UCLineMonitor.SoftwareForSendSome>();
ObservableCollection<byte[]> _deviceNid = new ObservableCollection<byte[]>();
public ObservableCollection<byte[]> DeviceNid
{
    get { return _deviceNid; }
    set { _deviceNid = value; }
}

 _deviceNid.Clear();

_deviceNid.Add(DataConverter.StrHex2Bytes(item.Tag.ToString()));

lock (((ICollection)_updatesome).SyncRoot)
{
    _updatesome.Enqueue(new UCLineMonitor.SoftwareForSendSome { DeviceNid = DeviceNid, FullFileName = FullFileName });
}

public void SendDeviceSoftUpdate()
{
    UCLineMonitor.SoftwareForSendSome softforSendSome = _updatesome.Dequeue();
    lock (((ICollection)_updatesome).SyncRoot)//消息同步控制
    {
        string fileName = FullFileName.Substring(FullFileName.LastIndexOf('\\') + 1);

        foreach (var item in softforSendSome.DeviceNid)
        {
            //挨个设备发送更新通知
            #region 发送报文通知

            DLMLC.XDR.P6022Req p6022 = new DLMLC.XDR.P6022Req
            {
                Header = new DLMLC.XDR.PackageHeader_t()
            };
            p6022.Header.MessageType = 0x6022;
            p6022.Header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
            p6022.Header.PacketLength = 116;
            p6022.Header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
            p6022.Header.DestinationID = new byte[4];
            p6022.Header.MessageID = new byte[4];
            p6022.Header.PacketNbr = new byte[12];
            p6022.Header.CRC = new byte[4];
            p6022.Header.ResponseCode = 0x01;
            p6022.Header.CompressCypherFlag = 0x01;
            p6022.Header.MacFlag = 0x01;


            string tempFTP = XMLManipulater.GetElementByName(DLMLC_ProjectInfo.ConfigFileName, "FTPSoftDirectory");

            p6022.Body6022Req = new DLMLC.XDR.C6022Req_t
            {
                //excu
                ExcuNID = item,
                FTProute = new HHJT_STRING(64, tempFTP),
                Filename = new HHJT_STRING(16, FullFileName),
                RFU = new byte[32],
            };

            p6022.CRC = new DLMLC.XDR.PackageCrc_t
            {
                CRC = new byte[4]
            };

            HHJT.AFC.Base.XDR.XDRStream stream = new HHJT.AFC.Base.XDR.XDRStream();
            p6022.Header.xdr_encode(stream);
            p6022.Body6022Req.xdr_encode(stream);
            p6022.CRC.xdr_encode(stream);
            byte[] PackageFinish = stream.Get_data();

            TcpClient tcpClient = new TcpClient();
            try
            {
                tcpClient.Connect(IPAddress.Parse(LCCConfig.LineCommIP), LCCConfig.LineCommPort);
                NetworkStream networkStream = tcpClient.GetStream();

                networkStream.Write(PackageFinish, 0, PackageFinish.Length);

                byte[] receive = new byte[1024 * 50];
                int rcvLength = networkStream.Read(receive, 0, receive.Length);

                if (rcvLength > 0)
                {
                    MessageBox.Show("程序软件下载命令发送成功");

                    AFC_System_Function.InsertLog(
                                                    string.Format(@"程序软件下载命令发送成功"),
                                                    DateTime.Now.ToString("yyyyMMddHHmmss"),
                                                    ExeResult.Success,
                                                    AFC_LoginUser.CurrentUser.UserID
                                                    );
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("程序软件下载命令发送失败:" + ex.Message);
            }
            finally
            {
                tcpClient.Close();
            }
            #endregion
        }
        IsCheaking = false;
    }
}
```

MN.0101.LineMonitor.xmal.cs line182
```c#
public class SoftwareForSend
{
    public ObservableCollection<byte[]> DeviceNid { get; set; }
    public string FullFileName { get; set; }
}
public class SoftwareForSendSome
{
    public ObservableCollection<byte[]> DeviceNid { get; set; }
    public string FullFileName { get; set; }
}
```

# 更新登录缓存
LCC.UCLogin.xaml.cs line118 line222
```c#
AFC_SystemDSInt.ReloadKeyValuePairs();
AFC_SystemDSString.ReloadKeyValuePairs();
```

# 操作员权限
LCC.UCLogin.xmal.cs line95 line132 line148 line204
```c#
M_UserInfo user = null;

"select USER_ID,USER_NAME,USER_PIN,VALID_TIME,USE_TIME,COMPANY_NAME From TBL_SS_USER where USER_ID='{0}' and rownum=1",

user = data.ElementAt(0);

currentUser.LineId = user.lineId;
currentUser.LineNid = String.Format("{0:X2}", user.lineId);


```

UserInfo.cs line255
```c#
public DateTime? UseDate { get; set; }
[ColumnInfo("A")]
public int lineId { get; set; }
```

Base.SystemDB.cs line158 line325 line338
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
string lineStrSQL = "";
if(_usr.LineNid != "00")
{
    lineStrSQL = string.Format(@" WHERE SUBSTR(Line_Nid,1,2) = '{0}'", _usr.LineNid);
}

AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL += string.Format(@" and line_nid = '{0}' ", _usr.LineNid);
}

```
RP.0109.Cash.Amount.Device.cs line66
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    sb.AppendFormat(@" and line_nid = '{0}' ", _usr.LineNid);
}
```

MN.0101.LineMonitor.xmal.cs line398 line432
```c#
 AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
StringBuilder sb = new StringBuilder();
if(_usr.LineNid == "00")
{
    sb.AppendFormat(@"SELECT A.STATION_NID, '['||A.line_id||']'||A.STATION_NAME_CN, B.POINT_X, B.POINT_Y, B.TEXT_X, B.TEXT_Y, B.TEXT_ROTATE
    FROM TBL_SS_STATION_INFO A INNER JOIN TBL_MN_LINE_LAYOUT B
    ON  A.STATION_NID = B.STATION_NID ");
}
else
{
    sb.AppendFormat(@"SELECT A.STATION_NID, '['||A.line_id||']'||A.STATION_NAME_CN, B.POINT_X, B.POINT_Y, B.TEXT_X, B.TEXT_Y, B.TEXT_ROTATE
    FROM TBL_SS_STATION_INFO A INNER JOIN TBL_MN_LINE_LAYOUT B
    ON  A.STATION_NID = B.STATION_NID where A.line_id = '{0}'", _usr.LineNid);
}
OracleDataReader reader = OracleDB.GetDataReader(sb.ToString(), conn);

StringBuilder sb1 = new StringBuilder();
if(_usr.LineNid == "00")
{
    sb1.AppendFormat(@"SELECT ORIG_STATION_NID, DEST_STATION_NID FROM TBL_MN_LINE_PATH ");
}
else
{
    sb1.AppendFormat(@"SELECT ORIG_STATION_NID, DEST_STATION_NID FROM TBL_MN_LINE_PATH where substr(ORIG_STATION_NID,1,2) = '{0}' and substr(DEST_STATION_NID,1,2) = '{1}' ", _usr.LineNid, _usr.LineNid);
}

reader = OracleDB.GetDataReader(sb1.ToString(), conn);
```

MN.0201.FlowQuery.xaml.cs line216
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid == "00")
{
    cbxLineId.ItemsSource = _ds_LineNid;
}
else
{
    cbxLineId.ItemsSource = _ds_LineNid.Where(d => d.Key == _usr.LineNid || d.Key == "-1");
}
```

MN.0202.StationFlowQuery.xaml.cs line217 line726
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
_ds_LineStation = AFC_SystemDSString.GetKeyValuePairs(DSString.LineStations, true);
if (_usr.LineNid == "00")
{
    cbxLineStation.ItemsSource = _ds_LineStation;
}
else
{
    cbxLineStation.ItemsSource = _ds_LineStation.Where(d => d.Key.Substring(0, 2) == _usr.LineNid || d.Key == "-1" || AFC_SystemDSString.IsTransfer(DS_Change, _usr.LineNid, d.Key));
}

AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL += string.Format(@" and line_nid = '{0}' ", _usr.LineNid);
}


```

TK.0201.SJTQuery.cs line55
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid == "00")
{
    cbbLineID.ItemsSource = _view.DS_LineNid;
}
else
{
    cbbLineID.ItemsSource = _view.DS_LineNid.Where(d => d.Key == _usr.LineNid || d.Key == "-1");
}

AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL.AppendFormat(@" and line_nid = '{0}' ", _usr.LineNid);
}
```

TK.0204.SJTStats.cs line173 line57
```c#
case DSString.someLines:
                    strSQL += string.Format(@" select SUBSTR(LINE_NID,1,2),line_name from tbl_ss_mlc_line WHERE SUBSTR(LINE_NID,1,2) = '{0}'", _usr.LineNid);
                    break;

public KeyValuePair<string, string>[] DS_Line { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.someLines, true);

AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    sbSQL.AppendFormat(@" and line_id = '{0}' ", _usr.LineNid);
}
```

TK.0401.CashBox.cs line577
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL.AppendFormat(@" and CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}' ", _usr.LineNid);
}
```
ST.0101.TicketSocket.In.VM.cs line142
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL.AppendFormat(@" and CENTER_ID = '{0}' ", _usr.LineNid);
}
```

```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL.AppendFormat(@" and substr(line_nid,1,2) = '{0}' ", _usr.LineNid);
}
```

ST.0202.TicketDeploy.Inform.VM.cs line112
```c#
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    string temp = "";

    strSQL.AppendFormat(@" and CCDB_LIB.F_SYS_GETLINENIDBYSTATION(IN_NID) = '{0}' ", _usr.LineNid);
}
```
Param.AFC_UC_ViewEdit.cs line262
```c#
 AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
string strSQL = "";
if (_usr.LineNid != "00")
{
    strSQL += string.Format(@"SELECT LINE_NID NID, LINE_NAME ParamRange FROM TBL_SS_MLC_LINE where substr(line_nid,1,2) = '{0}'",_usr.LineNid);
}
else 
{
    strSQL = @"SELECT LINE_NID NID, LINE_NAME ParamRange FROM TBL_SS_MLC_LINE ";
}
```

## LCC.MN监控管理子系统
### 系统监控
线路监控 已修改  -
车站监控 已修改  -
### 设备监控
设备状态统计  已修改  -
### 客流监控
线路客流查询  已修改  
车站客流查询  已修改  
线路累积客流查询  已修改
车站累积客流查询  已修改
### 综合设置
线路地图绘制  已修改  
车站布局绘制  已修改    

## LCC.RP报表管理子系统
### 收益类
RP0101  已修改  已修改  
RP0102  已修改  已修改  
RP0103  已修改  已修改  
RP0104  已修改  已修改  
RP0106  已修改  已修改  
RP0107  已修改  已修改  
RP0108  已修改  已修改  
RP0109  已修改  已修改  
RP0110  已修改  已修改  
RP0111  已修改  已修改  
RP0112  已修改  已修改  
RP0113  已修改  已修改  
RP0114  已修改  已修改  
RP0115  已修改  已修改  
RP0120  已修改  已修改  
### 客流类
RP0201  已修改  已修改  
RP0202  已修改  已修改  
RP0207  已修改  已修改  
### 综合类
RP0301  已修改  已修改  
RP0302  已修改  已修改  
RP0303  已修改  已修改  
RP0304  已修改  已修改  
RP0305  已修改  已修改  
RP0306  已修改  已修改    
RP0307  已修改  已修改  
### 全收益
RP0501  已修改  已修改  
RP0503  已修改  已修改  
RP0504  已修改  已修改  
RP0505  已修改  已修改  
RP0509  已修改  已修改  
RP0510  已修改  已修改  
RP0512  已修改  已修改  

## LCC.TK票务管理子系统
### 车票跟踪
TK0101  已修改  已修改  
TK0103  已修改  已修改  
TK0104  已修改  已修改  
TK0105  已修改  已修改  
### 设备跟踪
TK0401  已修改  已修改  
TK0402  已修改  已修改  
### 交易查询统计
TK0201  已修改  已修改  
TK0202  已修改  已修改  
TK0203  已修改  已修改  
TK0204  已修改  已修改  
TK0205  已修改  已修改  
TK0206  已修改  已修改  
TK0207  已修改  已修改  
TK0208  已修改  已修改  
TK0209  已修改  已修改  **青泥洼桥BOM数据暂时缺失-数据库中无数据**
TK0210  已修改  已修改  
TK0211  已修改  已修改  
TK0212  已修改  已修改  
### 对账数据查询
TK0301  已修改  -
TK0302  已修改  -
TK0303  已修改  -
TK0304  已修改  -
TK0305  已修改  -
TK0306  已修改  已修改
TK0307  已修改  -
TK0308  已修改  -
TK0309  已修改  -
TK0310  已修改  -
TK0311  已修改  - 
TK0312  已修改  -
TK0313  已修改  -
TK0314  已修改  -
TK0315  已修改  -
TK0316  已修改  -
TK0317  已修改  -
### 设备跟踪
TK0401  已修改  -
TK0402  已修改  -
### 票卡申请管理
TK0501  已修改  已修改  
TK0502  已修改  已修改  

## LCC.ST库存管理
### 票卡库存
ST0101  已修改  -  
ST0102  已修改  -  
ST0103  已修改  -  
ST0104  已修改  -  
ST0105  已修改  -  
ST0106  已修改  - 
ST0107  已修改  -  
ST0108  已修改  -   
### 票卡调配
ST0201  已修改  -  
ST0202  已修改  -  
ST0203  已修改  -  

## LCC.CH现金管理子系统
### 备用金查询统计
CH0301  已修改  已修改  
CH0302  已修改  已修改  
CH0303  已修改  已修改  
CH0304  已修改  已修改  
CH0305  已修改  已修改  
### 运营现金查询统计


## LCC.PM参数管理子系统
### MLC参数
PM02  已修改  
### 参数管理
PM0301  已修改  
PM0304  已修改  

# 剩余杂项修改
SS.0101.User.cs line1161
```c#
_ds_LineStation = AFC_SystemDSString.GetKeyValuePairs(DSString.HHJTStations);
```

Param.AFC_UC_ViewEdit.cs line228
```c#

string strSQL = "";
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    strSQL += string.Format(@"SELECT STATION_NID||'6121' NID, STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_STATION_INFO where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line where substr(line_nid,1,2) = '{0}') UNION ALL SELECT SPECIAL_STATION_NID||'6121' NID, SPECIAL_STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_SPECIAL_STATION where special_line_nid in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line where substr(line_nid,1,2) = '{0}')", _usr.LineNid);
}
else
{
    strSQL = @"SELECT STATION_NID||'6121' NID, STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_STATION_INFO where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line) UNION ALL SELECT SPECIAL_STATION_NID||'6121' NID, SPECIAL_STATION_NID||'-'||STATION_NAME_CN ParamRange 
FROM TBL_SS_SPECIAL_STATION where special_line_nid in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line)";
}
```

# 角色树选择更改
UI.TreeViewModel.cs line301
```c#
public static List<SelectedTreeViewModel<SelectedTreeViewFunction>>
CreateTreeViewByFuncInfoList(List<IFunctionInfo> listData, int temp)
{
    List<SelectedTreeViewModel<SelectedTreeViewFunction>> views = new List<SelectedTreeViewModel<SelectedTreeViewFunction>>();
    foreach (var item in listData.Where(d => d.ParentFuncId == temp))
    {
        views.Add(GenerateTree(listData, item));
    }

    foreach (var item in views)
    {
        item.Initialize();

    }

    return views;
}
```
SS.0102.Role.cs line870
```c#
var v = FunctionUtil.GetFunc();
AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
if (_usr.LineNid != "00")
{
    _treeViewFunc = SelectedTreeViewFunction.CreateTreeViewByFuncInfoList(v,_usr.LineId);
}
else
{
    _treeViewFunc = SelectedTreeViewFunction.CreateTreeViewByFuncInfoList(v);
}
```

# 线路分中心缩小权限
LCC.MN.cs line41
LCC.RP.cs line34
LCC.TK.cs line34

# 线路分中心之后修改代码
TK.0103.FaultRefund.cs line107 
TK.0104.FaultRefund.cs line107 line376
```c#
strSQL.AppendFormat(" AND CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}' ", LineNid);
```
## 签退关闭软件
UI.Main.WindowBase.cs line491
```c#
Shutdown();
```
LCC.MainWindow.cs 
```c#
protected override void Shutdown()
{
    _mainLCCWin.Close();
}
```
Base.SystemDS.cs line336 line441 line446
```c#
 string lineStrSQL = "";
string lineStrSQL1 = "";
if (_usr.LineNid != "00")
{
    lineStrSQL = string.Format(@" WHERE SUBSTR(LINE_NID,1,2) = '{0}'", _usr.LineNid);
    lineStrSQL1 = string.Format(@" WHERE SUBSTR(LINE_ID,1,2) = '{0}'", _usr.LineId);
}


strSQL += string.Format(@"select DEVICE_NID,'['||DEVICE_NID||']'||DEVICE_NAME from TBL_SS_DEVICE {0} order by 1",lineStrSQL);


strSQL += string.Format(@"select DEVICE_NID,'['||DEVICE_NID||']'||DEVICE_NAME from TBL_SS_DEVICE {0}
                        union all
                        select SC_NID,'['||SC_NID||']'||'SC' from tbl_ss_sc_info  
                            where line_id in (select SUBSTR(LINE_NID,1,2) from tbl_ss_mlc_line {0}) order by 1 ", lineStrSQL1);
```