

- [6022报文](#6022报文)
- [下载命令](#下载命令)
- [下载UI](#下载ui)
- [参数管理前两项选择添加换乘站](#参数管理前两项选择添加换乘站)
- [数据库](#数据库)
- [6101 \& 6121](#6101--6121)
- [配置](#配置)
- [命令](#命令)
- [oracle](#oracle)
- [报表特殊线路判断](#报表特殊线路判断)
  - [changed](#changed)


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

# 命令
tail -fn 10 02_09AppLog.txt

grep 6022 02_09_AppLog.txt

grep 6022 02_09_AppLog.txt -A100 -B50

ftp 172.210.200.1

get
put 

lcd 
!rcd
cd 

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

    }
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

## changed
RP0101  $\$
RP0102
RP0103
RP0104