# 菜单添加
Base.SystemInfo.cs line119
```c#
MN0103 = 200,
MN0104 = 201,
```
MN.01.SystemMonitor.cs line27
```c#
//if (_usr.ValidateFunc((int)LCCRoleFunc.MN0103))
{
    Funcs.Add(new ExpModuleFuncsInfo
    {
        FuncsName = "参数版本",
        FunctionViewType = typeof(V_LCC_MN_0103_ParamVersion)
    });

}
//if (_usr.ValidateFunc((int)LCCRoleFunc.MN0104))
{
    Funcs.Add(new ExpModuleFuncsInfo { FuncsName = "软件版本", FunctionViewType = typeof(V_LCC_MN_0104_SoftVersion) });
}
```
MN.0103.ParamVersion.cs
```c#
using DLMLC.Base;
using HHJT.AFC.Base.MVVM;
using HHJT.AFC.Base.OracleDBI;
using HHJT.AFC.Framework.UI;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Windows;
using DLMLC.Modules.Monitor;
using System.Windows.Input;
using HHJT.AFC.Base.API;
using DLMLC.Modules.Common;
using System.Net;
using System.Net.Sockets;
using HHJT.AFC.Base.XDR;
using System.Windows.Controls;
using System.Collections;
using System.Threading;
using DLMLC.XDR;

namespace LCC.MN.MN._01.SystemMonitor
{
    internal class V_LCC_MN_0103_ParamVersion_Query : UCLayoutQueryProcess
    {
        private VM_LCC_MN0103_ParamVersion_Query _view = new VM_LCC_MN0103_ParamVersion_Query();

        protected override void FurtherInitLayout()
        {
            base.FurtherInitLayout();
            _view.Props = AutoUI.AutoGenerateQueryPanel(typeof(VM_LCC_MN0103_ParamVersion_Query), PanelQuery);
            PanelQuery.DataContext = _view;

            RegisterCommand(this, BtnReset, _view.CommandReset);
            RegisterCommand(this, BtnQuery, _view.CommandQuery);

            Collection<AttrMethodAttribute> props =
                AutoUI.AugoGenerateDataProcessPanel(typeof(VM_LCC_MN0103_ParamVersion_Query), PanelProcess);
            ICommand[] cmds = new ICommand[]
                {
                     _view.CommandUpdate,
                };
            RegisterButtonAndContextMenu(this, props.ToArray(), cmds);

            AutoUI.UIDatagridInit(typeof(VM_LCC_MN0103_ParamVersion_Result), DataGridContent,
                        VM_LCC_MN0103_ParamVersion_Result.DS_Line,
                        VM_LCC_MN0103_ParamVersion_Result.DS_StationNID,
                        VM_LCC_MN0103_ParamVersion_Result.DS_DeviceType,
                        VM_LCC_MN0103_ParamVersion_Result.DS_Device,
                        VM_LCC_MN0103_ParamVersion_Result.DS_ParamType
                      );
            DataGridContent.ItemsSource = _view.DataResult;
        }
    }
    public class VM_LCC_MN0103_ParamVersion_Query : VM_QueryTemplate
    {
        public LCC_MN0103_ParamVersion_Query DataQuery { get; set; } = new LCC_MN0103_ParamVersion_Query();

        public ObservableCollection<VM_LCC_MN0103_ParamVersion_Result> DataResult { get; set; } = new ObservableCollection<VM_LCC_MN0103_ParamVersion_Result>();

        public VM_LCC_MN0103_ParamVersion_Result CurrentSelectedDataResult { get; set; }
        public KeyValuePair<string, string>[] DS_Change { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.SpecialStations, true);

        #region Combox数据源
        public static KeyValuePair<string, string>[] DS_Line { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.MlcLineInfo, true);

        public static KeyValuePair<string, string>[] DS_LineStation { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.LineStations, true);
        public static KeyValuePair<string, string>[] DS_ParamType { get; } = AFC_SystemDS.GetKeyValuePairs(DSString.ParamType, true);
        public static KeyValuePair<int, string>[] DS_Compare { get; } = AFC_SystemDS.GetKeyValuePairs(DSInt.Compare);
        public static KeyValuePair<string, string>[] DS_DeviceType { get; } = AFC_SystemDSString.GetKeyValuePairsBySQL(
                                                        @" select '-1','全部' from dual
                                                            UNION ALL
                                                        select device_type,type_name_en from tbl_ss_device_type 
                                                        WHERE  device_type IN ('11','12','21','30','41','51','61')
                                                         ");
        #endregion

        #region View显示绑定

        [AttrQueryProperty("线路", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_Line")]
        public string LineNid
        {
            get { return DataQuery.LineNid; }
            set
            {
                if (value == DataQuery.LineNid) return;
                DataQuery.LineNid = value;
                base.OnPropertyChanged("LineNid");
                base.OnPropertyChanged("DSP_LineStation");
            }
        }

        [AttrQueryProperty("车站", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_LineStation")]
        public string StationId
        {
            get { return DataQuery.StationId; }
            set
            {
                if (value == DataQuery.StationId) return;
                DataQuery.StationId = value;
                base.OnPropertyChanged("StationId");
            }
        }
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
                    if (LineNid == "-1")
                    {
                        return DS_LineStation;
                    }
                    else
                    {
                        return DS_LineStation.Where(d => d.Key.Substring(0, 2) == LineNid || d.Key == "-1" || AFC_SystemDSString.IsTransfer(DS_Change, LineNid, d.Key)).ToArray();
                    }
                }
            }
        }

        [AttrQueryProperty("设备类型", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_DeviceType")]
        public string DeviceType
        {
            get { return DataQuery.DeviceType; }
            set
            {
                if (value == DataQuery.DeviceType) return;
                DataQuery.DeviceType = value;
                base.OnPropertyChanged("DeviceType");
            }
        }
        [AttrQueryProperty("参数类型", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_ParamType")]
        public string ParamType
        {
            get { return DataQuery.ParamType; }
            set
            {
                if (value == DataQuery.ParamType) return;
                DataQuery.ParamType = value;
                base.OnPropertyChanged("ParamType");
            }
        }
        [AttrQueryProperty("参数一致性", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_Compare")]
        public int ParamVersionCompare
        {
            get { return DataQuery.ParamVersionCompare; }
            set
            {
                if (value == DataQuery.ParamVersionCompare) return;
                DataQuery.ParamVersionCompare = value;
                base.OnPropertyChanged("ParamVersionCompare");
            }
        }


        [AttrQueryProperty("创建时间范围", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? TransTimeStart
        {
            get { return DataQuery.TransTimeStart; }
            set
            {
                if (value == DataQuery.TransTimeStart) return;
                DataQuery.TransTimeStart = value;
                base.OnPropertyChanged("TransTimeStart");
            }
        }

        [AttrQueryProperty("-  ", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? TransTimeEnd
        {
            get { return DataQuery.TransTimeEnd; }
            set
            {
                if (value == DataQuery.TransTimeEnd) return;
                DataQuery.TransTimeEnd = value;
                base.OnPropertyChanged("TransTimeEnd");
            }
        }
        //[AttrQueryProperty("生效时间范围", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? EffectTimeStart
        {
            get { return DataQuery.EffectTimeStart; }
            set
            {
                if (value == DataQuery.EffectTimeStart) return;
                DataQuery.EffectTimeStart = value;
                base.OnPropertyChanged("EffectTimeStart");
            }
        }

        //[AttrQueryProperty("-  ", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? EffectTimeEnd
        {
            get { return DataQuery.EffectTimeEnd; }
            set
            {
                if (value == DataQuery.EffectTimeEnd) return;
                DataQuery.EffectTimeEnd = value;
                base.OnPropertyChanged("EffectTimeEnd");
            }
        }
        protected RelayCommand _commandUpdate;

        [AttrMethod("更新信息", AccelerateKey = "I")]
        public ICommand CommandUpdate
        {
            get
            {
                if (_commandUpdate == null)
                {
                    _commandUpdate = new RelayCommand(
                        param => this.Update(),
                        param => this.CanUpdate()
                        );
                }
                return _commandUpdate;
            }
        }
        #endregion

        protected List<string> _errorList = new List<string>();
        public string Error
        {
            get
            {
                if (_errorList.Count > 0)
                    return "有错误";
                else
                    return null;
            }
        }
        Queue<ParamVersionForSend> _updateall = new Queue<ParamVersionForSend>();
        ObservableCollection<string> _paramForSend = new ObservableCollection<string>();
        public void Update()
        {
            Dlg_Update_SoftVersion dlg = new Dlg_Update_SoftVersion
            {
                Owner = Application.Current.MainWindow
            };

            string device_nid, file_name;
            if (dlg.ShowDialog() == true)
            {
                if (dlg.CheckedStation != null)
                {
                    foreach (var item in dlg.CheckedStation)
                    {
                        _paramForSend.Add(item);
                    }
                    if (MessageBox.Show("确认更新选中车站？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) ==
                                    MessageBoxResult.OK)
                    {
                        lock (((ICollection)_updateall).SyncRoot)
                        {
                            _updateall.Enqueue(new ParamVersionForSend { Stationid = _paramForSend });
                        }
                        Thread thread = new Thread(UpdateParam);
                        thread.Start();
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    MessageBox.Show("未选择车站，无法进行更新！");
                    return;
                }
            }
            else
            {
                //MessageBox.Show("有信息未选择，无法进行更新！");
                return;
            }
        }
        private ObservableCollection<UCStationMonitor.ParamVersion> _dataResult =
            new ObservableCollection<UCStationMonitor.ParamVersion>();
        public void UpdateParam()
        {
            SoftwareVersionForSend param = null;
            lock (((ICollection)_updateall).SyncRoot)//消息同步控制
            {
                ParamVersionForSend paramVersionforSend = _updateall.Dequeue();
                List<string> failStation = new List<string>();
                List<string> successStation = new List<string>();
                StringBuilder sb = new StringBuilder();
                sb.Clear();
                int size = 0, su = 0;

                foreach (var station in paramVersionforSend.Stationid)
                {
                    #region 6082
                    /*
                    List<RM6081ParamQueryRec_t> rm6081TempList = new List<RM6081ParamQueryRec_t>();
                    foreach (var paramTemp in param.Params)
                    {
                        RM6081ParamQueryRec_t temp = new RM6081ParamQueryRec_t();
                        temp.ParamType = DataConverter.StrHex2Bytes(paramTemp.ParamType);
                        temp.ParamVersion = new HHJT_BCD(7, paramTemp.ParamVer);
                        rm6081TempList.Add(temp);
                    }
                    */
                    XDRStream stream = new XDRStream();

                    P6081Req p6081 = new P6081Req();
                    p6081.Header = new PackageHeader_t();
                    p6081.Header.MessageType = 0x6081;
                    p6081.Header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
                    p6081.Header.PacketLength = (uint)(5); //+ param.Params.Count * 9
                    p6081.Header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
                    p6081.Header.DestinationID = DataConverter.StrHex2Bytes(station + "6101");
                    p6081.Header.MessageID = new byte[4];
                    p6081.Header.PacketNbr = new byte[12];
                    p6081.Header.CRC = new byte[4];
                    p6081.Header.ResponseCode = 0x01;
                    p6081.Header.CompressCypherFlag = 0x01;
                    p6081.Header.MacFlag = 0x01;

                    p6081.Body6081Req = new C6081Req_t();
                    p6081.Body6081Req.DestNid = DataConverter.StrHex2Bytes(station + "0000");
                    p6081.Body6081Req.ParamStatus = 0x00;
                    //p6081.Body6081Req.RecCount = param.Params.Count;
                    //p6081.Body6081Req.RM6081ParamQueryRec = rm6081TempList.ToArray();

                    p6081.CRC = new PackageCrc_t();
                    p6081.CRC.CRC = new byte[4];

                    p6081.Header.xdr_encode(stream);
                    p6081.Body6081Req.xdr_encode(stream);
                    p6081.CRC.xdr_encode(stream);

                    byte[] data = stream.Get_data();
                    Console.WriteLine(DataConverter.Bytes2HexStr(data));


                    #endregion

                    #region send Param

                    TcpClient tcpClient = new TcpClient();
                    try
                    {
                        tcpClient.Connect
                            (IPAddress.Parse(LCCConfig.LineCommIP),
                             LCCConfig.LineCommPort);
                        NetworkStream networkStream = tcpClient.GetStream();

                        networkStream.Write(data, 0, data.Length);

                        byte[] receive = new byte[1024];
                        int rcvLength = networkStream.Read(receive, 0, receive.Length);
                        if (rcvLength > 66)
                        {
                            XDRStream streamRes = new XDRStream(receive, rcvLength);
                            /*
                            P6082Res c6082 = new P6082Res();
                            c6082.xdr_decode(streamRes);

                            List<ParamversionResult> versions = new List<ParamversionResult>();

                            for (int i = 0; i < c6082.Body6082Res.RecCount; i++)
                            {
                                ParamversionResult result = new ParamversionResult();
                                result.DeviceNid = station + "0000";
                                result.DeviceResult = "查询成功";
                                result.EQU_TYPE = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftType.ToString();
                                result.EQU_VERSION = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftVersion;
                                versions.Add(result);
                            }*/
                            /*this.Dispatcher.Invoke(new Action(() =>
                            {
                                foreach (var paramVersion in versions)
                                {
                                    _dataSoftVerResult.Add(paramVersion);
                                }
                            }));*/
                            MessageBox.Show("车站" + station + "0000" + "参数信息更新命令发送成功");

                        }
                        else
                        {
                            MessageBox.Show(string.Format("{0}接收应答失败，发送不成功", station + "0000"), "错误", MessageBoxButton.OK, MessageBoxImage.Error);

                        }

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "发送失败", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                    finally
                    {
                        tcpClient.Close();
                    }
                }
            }
        }

        public bool CanUpdate()
        {
            return true;//需补充
        }
        public override void Query()
        {
            DataResult.Clear();
            var data = DataQuery.Query();
            foreach (var item in data)
            {
                DataResult.Add(new VM_LCC_MN0103_ParamVersion_Result(item));
            }
        }

        public override bool CanQuery()
        {
            return true;//需补充
        }

        public override void Reset()
        {
            AutoUI.ResetQueryInput(Props);
        }

        public override bool CanReset()
        {
            return true;//需补充
        }

        #endregion

        #region Commands
        #endregion
    }

    public class VM_LCC_MN0103_ParamVersion_Result : ViewModelBase
    {
        public LCC_MN0103_ParamVersion_Result Data { get; set; }

        #region Constructor
        public VM_LCC_MN0103_ParamVersion_Result()
        {
            Data = new LCC_MN0103_ParamVersion_Result();
        }
        public VM_LCC_MN0103_ParamVersion_Result(LCC_MN0103_ParamVersion_Result d)
        {
            Data = d;
        }
        #endregion

        #region Combox数据源
        public static KeyValuePair<string, string>[] DS_Line { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.MlcLineInfo);
        public static KeyValuePair<string, string>[] DS_StationNID { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.LineStations, true);
        public static KeyValuePair<string, string>[] DS_DeviceType { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.DeviceType);
        public static KeyValuePair<string, string>[] DS_Device { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.AllDevice);
        public static KeyValuePair<int, string>[] DS_ParamType { get; } = AFC_SystemDS.GetKeyValuePairs(DSInt.Compare);
 
        #endregion

        #region View显示绑定

        [AttrDataProperty("包序列号", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public uint DATA_SEQ
        {
            get { return Data.DATA_SEQ; }
            set
            {
                if (value == Data.DATA_SEQ) return;
                Data.DATA_SEQ = value;
                base.OnPropertyChanged("DATA_SEQ");
            }
        }

        [AttrDataProperty("线路", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string LINE_NID
        {
            get { return Data.LINE_NID; }
            set
            {
                if (value == Data.LINE_NID) return;
                Data.LINE_NID = value;
                base.OnPropertyChanged("LINE_NID");
            }
        }

        [AttrDataProperty("站点标识", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string STATION_NID
        {
            get { return Data.STATION_NID; }
            set
            {
                if (value == Data.STATION_NID) return;
                Data.STATION_NID = value;
                base.OnPropertyChanged("STATION_NID");
            }
        }

        [AttrDataProperty("设备类型", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string DEVICE_TYPE
        {
            get { return Data.DEVICE_TYPE; }
            set
            {
                if (value == Data.DEVICE_TYPE) return;
                Data.DEVICE_TYPE = value;
                base.OnPropertyChanged("DEVICE_TYPE");
            }
        }

        [AttrDataProperty("设备标识", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string DEVICE_NID
        {
            get { return Data.DEVICE_NID; }
            set
            {
                if (value == Data.DEVICE_NID) return;
                Data.DEVICE_NID = value;
                base.OnPropertyChanged("DEVICE_NID");
            }
        }

        [AttrDataProperty("参数状态", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public int PARAM_STATUS
        {
            get { return Data.PARAM_STATUS; }
            set
            {
                if (value == Data.PARAM_STATUS) return;
                Data.PARAM_STATUS = value;
                base.OnPropertyChanged("PARAM_STATUS");
            }
        }
        [AttrDataProperty("参数类型", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public string PARAM_TYPE
        {
            get { return Data.PARAM_TYPE; }
            set
            {
                if (value == Data.PARAM_TYPE) return;
                Data.PARAM_TYPE = value;
                base.OnPropertyChanged("PARAM_TYPE");
            }
        }
        [AttrDataProperty("参数版本", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public string PARAM_VERSION
        {
            get { return Data.PARAM_VERSION; }
            set
            {
                if (value == Data.PARAM_VERSION) return;
                Data.PARAM_VERSION = value;
                base.OnPropertyChanged("PARAM_VERSION");
            }
        }
        [AttrDataProperty("MLC参数版本", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public string MLCPARAM_VERSION
        {
            get { return Data.MLCPARAM_VERSION; }
            set
            {
                if (value == Data.MLCPARAM_VERSION) return;
                Data.MLCPARAM_VERSION = value;
                base.OnPropertyChanged("MLCPARAM_VERSION");
            }
        }
        [AttrDataProperty("参数版本比较", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public int PARAM_VERSION_COMPARE
        {
            get { return (Data.PARAM_VERSION_COMPARE); }
            set
            {
                if (value == PARAM_VERSION_COMPARE) return;
                Data.PARAM_VERSION_COMPARE = value;
                base.OnPropertyChanged("PARAM_VERSION_COMPARE");
            }
        }

        [AttrDataProperty("生效时间", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public string EFFECT_TIME
        {
            get { return Data.EFFECT_TIME; }
            set
            {
                if (value == Data.EFFECT_TIME) return;
                Data.EFFECT_TIME = value;
                base.OnPropertyChanged("EFFECT_TIME");
            }
        }
        [AttrDataProperty("创建时间", InputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime CREATE_TIME
        {
            get { return Data.CREATE_TIME; }
            set
            {
                if (value == Data.CREATE_TIME) return;
                Data.CREATE_TIME = value;
                base.OnPropertyChanged("CREATE_TIME");
            }
        }
       #endregion
    }

    public class LCC_MN0103_ParamVersion_Query
    {
        public string LineNid { get; set; }

        [ColumnInfo("StationNid")]
        public string StationId { get; set; }//站点号

        [ColumnInfo("DeviceType")]
        public string DeviceType { get; set; }//设备类型

        [ColumnInfo("ParamType")]
        public string ParamType { get; set; }//设备类型
        [ColumnInfo("ParamVersionCompare")]
        public int ParamVersionCompare { get; set; }//设备类型
        
        [ColumnInfo("TRANS_DATE")]
        public DateTime? TransTimeStart { get; set; }//运营日开始

        [ColumnInfo("TRANS_DATE")]
        public DateTime? TransTimeEnd { get; set; }//运营日结束
        [ColumnInfo("EFFECT_DATE")]
        public DateTime? EffectTimeStart { get; set; }//运营日开始

        [ColumnInfo("EFFECT_DATE")]
        public DateTime? EffectTimeEnd { get; set; }//运营日结束

        public ObservableCollection<LCC_MN0103_ParamVersion_Result> Query()
        {
            ObservableCollection<LCC_MN0103_ParamVersion_Result> data = new ObservableCollection<LCC_MN0103_ParamVersion_Result>();

            StringBuilder strSQL = new StringBuilder();
            strSQL.Append(@"SELECT  DATA_SEQ,CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID), SUBSTR(DEVICE_NID,1,4),SUBSTR(DEVICE_NID,5,2),DEVICE_NID,
                    PARAM_STATUS, PARAM_TYPE, PARAM_VERSION, CCDB_LIB.F_SYS_GETPARAMVER(DEVICE_NID,PARAM_TYPE,0),CCDB_LIB.F_SYS_COMPAREPARAMVER(DEVICE_NID,PARAM_TYPE,0,PARAM_VERSION),EFFECT_TIME, CREATE_TIME
                  FROM TBL_MN_DEVICE_PARAM where rownum < 5000 ");

            if (LineNid != null && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND  CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}%' ", LineNid);
            }
            AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
            if (_usr.LineNid != "00")
            {
                strSQL.AppendFormat(@" and CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}' ", _usr.LineNid);
            }
            if (StationId != null && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND  DEVICE_NID LIKE '{0}%' ", StationId);
            }
            if (ParamType != null && !"-1".Equals(ParamType))
            {
                strSQL.AppendFormat(" AND  PARAM_TYPE = '{0}' ", ParamType);
            }
            if (ParamVersionCompare != null && !"-1".Equals(ParamVersionCompare))
            {
                strSQL.AppendFormat(" AND  CCDB_LIB.F_SYS_COMPAREPARAMVER(DEVICE_NID,PARAM_TYPE,0,PARAM_VERSION) = {0} ", ParamVersionCompare);
            }
            if (DeviceType != null && !"-1".Equals(DeviceType))
            {
                if (DeviceType == "30")
                {
                    strSQL.AppendFormat(" AND (SUBSTR(DEVICE_NID,5,2) = '31' or SUBSTR(DEVICE_NID,5,2) = '32' or SUBSTR(DEVICE_NID,5,2) = '33' or SUBSTR(DEVICE_NID,5,2) = '34' )");
                }
                else
                {
                    strSQL.AppendFormat(" AND SUBSTR(DEVICE_NID,5,2) = '{0}'", DeviceType);
                }
            }
            if (EffectTimeStart != null)
            {
                strSQL.AppendFormat(" AND  EFFECT_TIME >= '{0}' ", ((DateTime)EffectTimeStart).ToString("yyyyMMddHHmmss"));
            }
            if (EffectTimeEnd != null)
            {
                strSQL.AppendFormat(" AND EFFECT_TIME <= '{0}' ", ((DateTime)EffectTimeEnd).ToString("yyyyMMddHHmmss"));
            }
            if (TransTimeStart != null)
            {
                strSQL.AppendFormat(" AND  CREATE_TIME >= to_date('{0}', 'YYYYMMDDHH24MISS') ", ((DateTime)TransTimeStart).ToString("yyyyMMddHHmmss"));
            }
            if (TransTimeEnd != null)
            {
                strSQL.AppendFormat(" AND CREATE_TIME <= to_date('{0}', 'YYYYMMDDHH24MISS') ", ((DateTime)TransTimeEnd).ToString("yyyyMMddHHmmss"));
            }

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_MN0103_ParamVersion_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
        }
    }

    public class LCC_MN0103_ParamVersion_Result
    {


        [ColumnInfo("DATA_SEQ")]
        public uint DATA_SEQ { get; set; }//包序列号

        [ColumnInfo("LINE_NID")]
        public string LINE_NID { get; set; }

        [ColumnInfo("STATION_NID")]
        public string STATION_NID { get; set; }//站点标识

        [ColumnInfo("DEVICE_TYPE")]
        public string DEVICE_TYPE { get; set; }//设备类型

        [ColumnInfo("DEVICE_NID")]
        public string DEVICE_NID { get; set; }//设备标识

        [ColumnInfo("PARAM_STATUS")]
        public int PARAM_STATUS { get; set; }

        [ColumnInfo("PARAM_TYPE")]
        public string PARAM_TYPE { get; set; }

        [ColumnInfo("PARAM_VERSION")]
        public string PARAM_VERSION { get; set; }

        [ColumnInfo("MLCPARAM_VERSION")]
        public string MLCPARAM_VERSION { get; set; }

        [ColumnInfo("PARAM_VERSION_COMPARE")]
        public int PARAM_VERSION_COMPARE { get; set; }        

        [ColumnInfo("EFFECT_TIME")]
        public string EFFECT_TIME { get; set; }//产生时间

        [ColumnInfo("CREATE_TIME")]
        public DateTime CREATE_TIME { get; set; }//产生时间

    }

    public class ParamVersionForSend
    {
        public ObservableCollection<string> Stationid { get; set; }
    }
    public class ParamversionResult
    {
        public string StationNid { get; set; }
        public string DeviceNid { get; set; }
        public string DeviceResult { get; set; }
        public string EQU_TYPE { get; set; }
        public string EQU_VERSION { get; set; }
    }
}

```
MN.0104.SoftVersion.cs
```c#
using DLMLC.Base;
using HHJT.AFC.Base.MVVM;
using HHJT.AFC.Base.OracleDBI;
using HHJT.AFC.Framework.UI;
using DLMLC.Modules.Monitor;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using HHJT.AFC.Base.API;
using DLMLC.Modules.Common;
using System.Net;
using System.Net.Sockets;
using HHJT.AFC.Base.XDR;
using System.Windows.Controls;
using System.Collections;
using System.Threading;
using DLMLC.XDR;



namespace LCC.MN.MN._01.SystemMonitor
{
    internal class V_LCC_MN_0104_SoftVersion_Query : UCLayoutQueryProcess
    {
        private VM_LCC_MN_0104_SoftVersion_Query _view = new VM_LCC_MN_0104_SoftVersion_Query();

        protected override void FurtherInitLayout()
        {
            base.FurtherInitLayout();
            _view.Props = AutoUI.AutoGenerateQueryPanel(typeof(VM_LCC_MN_0104_SoftVersion_Query), PanelQuery);
            PanelQuery.DataContext = _view;

            RegisterCommand(this, BtnReset, _view.CommandReset);
            RegisterCommand(this, BtnQuery, _view.CommandQuery);

            Collection<AttrMethodAttribute> props =
                AutoUI.AugoGenerateDataProcessPanel(typeof(VM_LCC_MN_0104_SoftVersion_Query), PanelProcess);
            ICommand[] cmds = new ICommand[]
                {
                     _view.CommandUpdate,
                };
            RegisterButtonAndContextMenu(this, props.ToArray(), cmds);

            AutoUI.UIDatagridInit(typeof(VM_LCC_MN_0104_SoftVersion_Result), DataGridContent,
                        VM_LCC_MN_0104_SoftVersion_Result.DS_Line,
                        VM_LCC_MN_0104_SoftVersion_Result.DS_StationNID,
                        VM_LCC_MN_0104_SoftVersion_Result.DS_SoftType,
                        VM_LCC_MN_0104_SoftVersion_Result.DS_DeviceType,
                        VM_LCC_MN_0104_SoftVersion_Result.DS_Device

                      );
            DataGridContent.ItemsSource = _view.DataResult;
        }
    }
    public class VM_LCC_MN_0104_SoftVersion_Query : VM_QueryTemplate
    {
        public LCC_MN_0104_SoftVersion_Query DataQuery { get; set; } = new LCC_MN_0104_SoftVersion_Query();

        public ObservableCollection<VM_LCC_MN_0104_SoftVersion_Result> DataResult { get; set; } = new ObservableCollection<VM_LCC_MN_0104_SoftVersion_Result>();

        public VM_LCC_MN_0104_SoftVersion_Result CurrentSelectedDataResult { get; set; }
        public KeyValuePair<string, string>[] DS_Change { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.SpecialStations, true);

        #region Combox数据源
        public static KeyValuePair<string, string>[] DS_Line { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.MlcLineInfo, true);

        public static KeyValuePair<string, string>[] DS_LineStation { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.LineStations, true);
        public static KeyValuePair<int, string>[] DS_SoftType { get; } = AFC_SystemDS.GetKeyValuePairs(DSInt.SoftType, true);

        public static KeyValuePair<string, string>[] DS_DeviceType { get; } = AFC_SystemDSString.GetKeyValuePairsBySQL(
                                                        @" select '-1','全部' from dual
                                                            UNION ALL
                                                        select device_type,type_name_en from tbl_ss_device_type 
                                                        WHERE  device_type IN ('11','12','21','30','41','51','61')
                                                         ");
        #endregion

        #region View显示绑定

        [AttrQueryProperty("线路", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_Line")]
        public string LineNid
        {
            get { return DataQuery.LineNid; }
            set
            {
                if (value == DataQuery.LineNid) return;
                DataQuery.LineNid = value;
                base.OnPropertyChanged("LineNid");
                base.OnPropertyChanged("DSP_LineStation");
            }
        }

        [AttrQueryProperty("车站", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DSP_LineStation")]
        public string StationId
        {
            get { return DataQuery.StationId; }
            set
            {
                if (value == DataQuery.StationId) return;
                DataQuery.StationId = value;
                base.OnPropertyChanged("StationId");
            }
        }
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
                    if (LineNid == "-1")
                    {
                        return DS_LineStation;
                    }
                    else
                    {
                        return DS_LineStation.Where(d => d.Key.Substring(0, 2) == LineNid || d.Key == "-1" || AFC_SystemDSString.IsTransfer(DS_Change, LineNid, d.Key)).ToArray();
                    }
                }
            }
        }

        [AttrQueryProperty("设备类型", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_DeviceType")]
        public string DeviceType
        {
            get { return DataQuery.DeviceType; }
            set
            {
                if (value == DataQuery.DeviceType) return;
                DataQuery.DeviceType = value;
                base.OnPropertyChanged("DeviceType");
            }
        }

        [AttrQueryProperty("软件类型", QueryInputType = UI_INPUT_TYPE.COMBOBOX, ComboDatasource = "DS_SoftType")]
        public int SoftType
        {
            get { return DataQuery.SoftType; }
            set
            {
                if (value == DataQuery.SoftType) return;
                DataQuery.SoftType = value;
                base.OnPropertyChanged("SoftType");
            }
        }

        [AttrQueryProperty("软件版本", QueryInputType = UI_INPUT_TYPE.TEXTBOX)]
        public string SoftVersion
        {
            get { return DataQuery.SoftVersion; }
            set
            {
                if (value == DataQuery.SoftVersion) return;
                DataQuery.SoftVersion = value;
                base.OnPropertyChanged("SoftVersion");
            }
        }
        [AttrQueryProperty("输入目标软件版本查询不一致信息", QueryInputType = UI_INPUT_TYPE.TEXTBOX)]
        public string SoftVersionCompare
        {
            get { return DataQuery.SoftVersionCompare; }
            set
            {
                if (value == DataQuery.SoftVersionCompare) return;
                DataQuery.SoftVersionCompare = value;
                base.OnPropertyChanged("SoftVersionCompare");
            }
        }

        [AttrQueryProperty("时间范围", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? TransTimeStart
        {
            get { return DataQuery.TransTimeStart; }
            set
            {
                if (value == DataQuery.TransTimeStart) return;
                DataQuery.TransTimeStart = value;
                base.OnPropertyChanged("TransTimeStart");
            }
        }

        [AttrQueryProperty("-  ", QueryInputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime? TransTimeEnd
        {
            get { return DataQuery.TransTimeEnd; }
            set
            {
                if (value == DataQuery.TransTimeEnd) return;
                DataQuery.TransTimeEnd = value;
                base.OnPropertyChanged("TransTimeEnd");
            }
        }

        #endregion
        protected RelayCommand _commandUpdate;

        [AttrMethod("更新信息", AccelerateKey = "I")]
        public ICommand CommandUpdate
        {
            get
            {
                if (_commandUpdate == null)
                {
                    _commandUpdate = new RelayCommand(
                        param => this.Update(),
                        param => this.CanUpdate()
                        );
                }
                return _commandUpdate;
            }
        }
        #region Query&Reset

        protected List<string> _errorList = new List<string>();
        public string Error
        {
            get
            {
                if (_errorList.Count > 0)
                    return "有错误";
                else
                    return null;
            }
        }
        Queue<SoftwareVersionForSend> _updateall = new Queue<SoftwareVersionForSend>();
        ObservableCollection<byte[]> _softwareVersionForSend = new ObservableCollection<byte[]>();
        ObservableCollection<string> _softVersionForSend = new ObservableCollection<string>();
        public void Update()
        {
            Dlg_Update_SoftVersion dlg = new Dlg_Update_SoftVersion
            {
                Owner = Application.Current.MainWindow
            };

            string device_nid, file_name;
            if (dlg.ShowDialog() == true)
            {
                if(dlg.CheckedStation != null)
                {
                    _softwareVersionForSend.Clear();
                    foreach (var item in dlg.CheckedStation)
                    {
                        _softwareVersionForSend.Add(DataConverter.StrHex2Bytes(item + "0000"));
                    }
                    foreach (var item in dlg.CheckedStation)
                    {
                        _softVersionForSend.Add(item);
                    }
                    if (MessageBox.Show("确认更新选中车站？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) ==
                                    MessageBoxResult.OK)
                    {
                        lock (((ICollection)_updateall).SyncRoot)
                        {
                            _updateall.Enqueue(new SoftwareVersionForSend { StationNid = _softwareVersionForSend, Stationid = _softVersionForSend });
                        }
                        Thread thread = new Thread(UpdateSoft);
                        thread.Start();
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    MessageBox.Show("未选择车站，无法进行更新！");
                    return;
                }
            }
            else
            {
                //MessageBox.Show("有信息未选择，无法进行更新！");
                return;
            }
        }
        private ObservableCollection<UCStationMonitor.ParamVersion> _dataResult =
            new ObservableCollection<UCStationMonitor.ParamVersion>();
        public void UpdateSoft()
        {
            SoftwareVersionForSend param = null;
            lock (((ICollection)_updateall).SyncRoot)//消息同步控制
            {
                SoftwareVersionForSend softVersionforSend = _updateall.Dequeue();
                List<string> failStation = new List<string>();
                List<string> successStation = new List<string>();
                StringBuilder sb = new StringBuilder();
                sb.Clear();
                int size = 0, su = 0;

                foreach (var station in softVersionforSend.Stationid)
                {
                    #region 6082

                    XDRStream stream = new XDRStream();
                    PackageHeader_t header = new PackageHeader_t();
                    header.MessageType = 0x6082;
                    header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
                    header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
                    header.DestinationID = DataConverter.StrHex2Bytes(station.Substring(0, 4) + "6101");
                    header.MessageID = new byte[4];
                    header.PacketNbr = new byte[12];
                    header.CRC = new byte[4];
                    header.ResponseCode = 0x01;
                    header.CompressCypherFlag = 0x01;
                    header.MacFlag = 0x01;

                    List<byte> byteList = new List<byte>();

                    byteList.AddRange(DataConverter.StrHex2Bytes(station + "0000"));
                    byteList.Add((byte)0x01);

                    header.PacketLength = (uint)byteList.Count;
                    header.xdr_encode(stream);
                    stream.Put_bytes(byteList.ToArray(), byteList.Count);
                    PackageCrc_t CRC = new PackageCrc_t();
                    CRC.CRC = new byte[4];
                    CRC.xdr_encode(stream);
                    byte[] data = stream.Get_data();
                    //Console.WriteLine(DataConverter.Bytes2HexStr(data));

                    #endregion

                    #region send Param

                    TcpClient tcpClient = new TcpClient();
                    try
                    {
                        tcpClient.Connect
                            (IPAddress.Parse(LCCConfig.LineCommIP),
                             LCCConfig.LineCommPort);
                        NetworkStream networkStream = tcpClient.GetStream();

                        networkStream.Write(data, 0, data.Length);

                        byte[] receive = new byte[1024];
                        int rcvLength = networkStream.Read(receive, 0, receive.Length);
                        if (rcvLength > 66)
                        {
                            XDRStream streamRes = new XDRStream(receive, rcvLength);

                            P6082Res c6082 = new P6082Res();
                            c6082.xdr_decode(streamRes);

                            List<SoftversionResult> versions = new List<SoftversionResult>();

                            for (int i = 0; i < c6082.Body6082Res.RecCount; i++)
                            {
                                SoftversionResult result = new SoftversionResult();
                                result.DeviceNid = station + "0000";
                                result.DeviceResult = "查询成功";
                                result.EQU_TYPE = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftType.ToString();
                                result.EQU_VERSION = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftVersion;
                                versions.Add(result);
                            }
                            /*this.Dispatcher.Invoke(new Action(() =>
                            {
                                foreach (var paramVersion in versions)
                                {
                                    _dataSoftVerResult.Add(paramVersion);
                                }
                            }));*/

                        }
                        else
                        {
                            MessageBox.Show(string.Format("{0}接收应答失败，发送不成功", station + "0000"), "错误", MessageBoxButton.OK, MessageBoxImage.Error);

                        }

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "发送失败", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                    finally
                    {
                        tcpClient.Close();
                    }
                }
            }
        }
    
        public bool CanUpdate()
        {
            return true;//需补充
        }
        public override void Query()
        {
            DataResult.Clear();
            var data = DataQuery.Query();
            foreach (var item in data)
            {
                DataResult.Add(new VM_LCC_MN_0104_SoftVersion_Result(item));
            }
        }

        public override bool CanQuery()
        {
            return true;//需补充
        }

        public override void Reset()
        {
            AutoUI.ResetQueryInput(Props);
        }

        public override bool CanReset()
        {
            return true;//需补充
        }

        #endregion

        #region Commands
        #endregion
        #endregion
    }

    public class VM_LCC_MN_0104_SoftVersion_Result : ViewModelBase
    {
        public LCC_MN_0104_SoftVersion_Result Data { get; set; }

        #region Constructor
        public VM_LCC_MN_0104_SoftVersion_Result()
        {
            Data = new LCC_MN_0104_SoftVersion_Result();
        }
        public VM_LCC_MN_0104_SoftVersion_Result(LCC_MN_0104_SoftVersion_Result d)
        {
            Data = d;
        }
        #endregion

        #region Combox数据源
        public static KeyValuePair<string, string>[] DS_Line { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.MlcLineInfo);
        public static KeyValuePair<string, string>[] DS_StationNID { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.LineStations, true);
        public static KeyValuePair<string, string>[] DS_DeviceType { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.DeviceType);
        public static KeyValuePair<string, string>[] DS_Device { get; } = AFC_SystemDSString.GetKeyValuePairs(DSString.AllDevice);
        public static KeyValuePair<int, string>[] DS_SoftType { get; } = AFC_SystemDS.GetKeyValuePairs(DSInt.SoftType);
        #endregion

        #region View显示绑定

        [AttrDataProperty("数据序列号", InputType = UI_INPUT_TYPE.TEXTBOX)]
        public uint DATA_SEQ
        {
            get { return Data.DATA_SEQ; }
            set
            {
                if (value == Data.DATA_SEQ) return;
                Data.DATA_SEQ = value;
                base.OnPropertyChanged("DATA_SEQ");
            }
        }
        [AttrDataProperty("线路标识", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string LINE_NID
        {
            get { return Data.LINE_NID; }
            set
            {
                if (value == Data.LINE_NID) return;
                Data.LINE_NID = value;
                base.OnPropertyChanged("LINE_NID");
            }
        }
        [AttrDataProperty("站点标识", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public string STATION_NID
        {
            get { return Data.STATION_NID; }
            set
            {
                if (value == Data.STATION_NID) return;
                Data.STATION_NID = value;
                base.OnPropertyChanged("STATION_NID");
            }
        }
        [AttrDataProperty("设备标识", InputType = UI_INPUT_TYPE.TEXTBOX)]//COMBOBOX
        public string DEVICE_NID
        {
            get { return Data.DEVICE_NID; }
            set
            {
                if (value == Data.DEVICE_NID) return;
                Data.DEVICE_NID = value;
                base.OnPropertyChanged("DEVICE_NID");
            }
        }

        [AttrDataProperty("软件类型", InputType = UI_INPUT_TYPE.COMBOBOX)]
        public int SOFT_TYPE
        {
            get { return Data.SOFT_TYPE; }
            set
            {
                if (value == Data.SOFT_TYPE) return;
                Data.SOFT_TYPE = value;
                base.OnPropertyChanged("SOFT_TYPE");
            }
        }

        [AttrDataProperty("软件版本", InputType = UI_INPUT_TYPE.TEXTBOX)]//COMBOBOX
        public string SOFT_VERSION
        {
            get { return Data.SOFT_VERSION; }
            set
            {
                if (value == Data.SOFT_VERSION) return;
                Data.SOFT_VERSION = value;
                base.OnPropertyChanged("SOFT_VERSION");
            }
        }

        [AttrDataProperty("创建时间", InputType = UI_INPUT_TYPE.DATETIME)]
        public DateTime CREATE_TIME
        {
            get { return Data.CREATE_TIME; }
            set
            {
                if (value == Data.CREATE_TIME) return;
                Data.CREATE_TIME = value;
                base.OnPropertyChanged("CREATE_TIME");
            }
        }
        #endregion

        protected List<string> _errorList = new List<string>();

        public string Error
        {
            get
            {
                if (_errorList.Count > 0)
                    return "有错误";
                else
                    return null;
            }
        }
    }

    public class LCC_MN_0104_SoftVersion_Query
    {
        public string LineNid { get; set; }

        [ColumnInfo("StationNid")]
        public string StationId { get; set; }//站点号

        [ColumnInfo("DeviceType")]
        public string DeviceType { get; set; }//设备类型

        [ColumnInfo("SoftType")]//软件类型
        public int SoftType { get; set; }

        [ColumnInfo("SoftVersion")]
        public string SoftVersion { get; set; }//软件版本
        [ColumnInfo("SoftVersionCompare")]
        public string SoftVersionCompare { get; set; }//软件版本
        
        [ColumnInfo("TRANS_DATE")]
        public DateTime? TransTimeStart { get; set; }//运营日开始

        [ColumnInfo("TRANS_DATE")]
        public DateTime? TransTimeEnd { get; set; }//运营日结束

        public ObservableCollection<LCC_MN_0104_SoftVersion_Result> Query()
        {
            ObservableCollection<LCC_MN_0104_SoftVersion_Result> data = new ObservableCollection<LCC_MN_0104_SoftVersion_Result>();

            StringBuilder strSQL = new StringBuilder();
            strSQL.Append(@"SELECT  DATA_SEQ,CCDB_LIB.F_SYS_GETLINENIDBYSTATION(substr(DEVICE_NID,1,4)), substr(device_nid,1,4),DEVICE_NID,SOFT_TYPE,SOFT_VERSION,CREATE_TIME
                  FROM TBL_MN_DEVICE_SOFT where rownum < 5000 ");

            if (LineNid != null && !"-1".Equals(LineNid))
            {
                strSQL.AppendFormat(" AND  CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) LIKE '{0}' ", LineNid);
            }
            AFC_LoginUser _usr = AFC_LoginUser.CurrentUser;
            if (_usr.LineNid != "00")
            {
                strSQL.AppendFormat(@" and CCDB_LIB.F_SYS_GETLINENID(DEVICE_NID) like '{0}' ", _usr.LineNid);
            }
            if (StationId != null && !"-1".Equals(StationId))
            {
                strSQL.AppendFormat(" AND  DEVICE_NID LIKE '{0}%' ", StationId);
            }
            if (SoftType != 0 && -1 != SoftType)
            {
                strSQL.AppendFormat(" AND  SOFT_TYPE = {0} ", SoftType);
            }
            if (SoftVersion != null && !"-1".Equals(SoftVersion))
            {
                strSQL.AppendFormat(" AND  SOFT_VERSION like '{0}%' ", SoftVersion);
            }
            if (SoftVersionCompare != null && !"-1".Equals(SoftVersionCompare))
            {
                strSQL.AppendFormat(" AND  trim(SOFT_VERSION) <> '{0}' ", SoftVersionCompare);
            }
            if (DeviceType != null && !"-1".Equals(DeviceType))
            {
                if (DeviceType == "30")
                {
                    strSQL.AppendFormat(" AND (SUBSTR(DEVICE_NID,5,2) = '31' or SUBSTR(DEVICE_NID,5,2) = '32' or SUBSTR(DEVICE_NID,5,2) = '33' or SUBSTR(DEVICE_NID,5,2) = '34' )");
                }
                else
                {
                    strSQL.AppendFormat(" AND SUBSTR(DEVICE_NID,5,2) = '{0}'", DeviceType);
                }
            }
            if (TransTimeStart != null)
            {
                strSQL.AppendFormat(" AND  CREATE_TIME >= to_date('{0}', 'YYYYMMDDHH24MISS') ", ((DateTime)TransTimeStart).ToString("yyyyMMddHHmmss"));
            }
            if (TransTimeEnd != null)
            {
                strSQL.AppendFormat(" AND CREATE_TIME <= to_date('{0}', 'YYYYMMDDHH24MISS') ", ((DateTime)TransTimeEnd).ToString("yyyyMMddHHmmss"));
            }

            var res = OracleDB.ST_RetrieveToObjects(strSQL.ToString(), data, typeof(LCC_MN_0104_SoftVersion_Result));
            if (res.IsFailed)
            {
                MessageBox.Show(res.ErrorMessage);
            }
            return data;
        }
    }

    public class LCC_MN_0104_SoftVersion_Result
    {


        [ColumnInfo("DATA_SEQ")]
        public uint DATA_SEQ { get; set; }//包序列号

        [ColumnInfo("LINE_NID")]
        public string LINE_NID { get; set; }//产生时间
        [ColumnInfo("STATION_NID")]
        public string STATION_NID { get; set; }//产生时间

        [ColumnInfo("DEVICE_NID")]
        public string DEVICE_NID { get; set; }

        [ColumnInfo("SOFT_TYPE")]
        public int SOFT_TYPE { get; set; }

        [ColumnInfo("SOFT_VERSION")]
        public string SOFT_VERSION { get; set; }//设备类型

        [ColumnInfo("CREATE_TIME")]
        public DateTime CREATE_TIME { get; set; }//产生时间
    }

    public class Dlg_Update_SoftVersion : HHJT_AFC_UI_Dialog
    {
        VM_LCC_MN_0104_SoftVersion_Result _data = new VM_LCC_MN_0104_SoftVersion_Result();

        #region datasource
        private static KeyValuePair<string, string>[] _ds_LineStation =
            AFC_SystemDSString.GetKeyValuePairs(DSString.HHJTStations);
        #endregion

        #region CheckList
        private List<string> _checkedStation = new List<string>();
        public List<string> CheckedStation
        {
            get { return _checkedStation; }
            set { _checkedStation = value; }
        }
        #endregion
        private List<AttrDataPropertyAttribute> props;

        public VM_LCC_MN_0104_SoftVersion_Result Data
        {
            get { return _data; }
            set { _data = value; }
        }
        private ListBox lstStation = new ListBox();
        private CheckBox cbxStation = new CheckBox();


        public Dlg_Update_SoftVersion()
        {
            Title = "更新软件信息";
            LabelTitle.Content = "更新软件版本信息";
            MaxHeight = 450;
            Grid grid = new Grid();
            DockPanel.SetDock(grid, Dock.Left);
            #region 角色

            GroupBox groupBoxFuncs = new GroupBox() { Header = "车站选择" };

            ScrollViewer scrollViewer = new ScrollViewer()
            {
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto,
                HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled
            };
            #endregion

            #region 车站
            cbxStation.Content = "车站选择";
            groupBoxFuncs = new GroupBox() { };

            groupBoxFuncs.Header = cbxStation;
            cbxStation.Checked += Cbx_Checked;
            cbxStation.Unchecked += Cbx_Checked;
            scrollViewer = new ScrollViewer()
            {
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto,
                HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled
            };
            lstStation.MinWidth = 50;
            for (int i = 0; i < _ds_LineStation.Count(); i++)
            {
                CheckBox cbTemp = new CheckBox();
                cbTemp.Content = _ds_LineStation[i].Value;
                cbTemp.Tag = _ds_LineStation[i].Key;
                cbTemp.IsChecked = false;

                lstStation.Items.Add(cbTemp);
            }
            scrollViewer.Content = lstStation;
            groupBoxFuncs.Content = scrollViewer;
            PanelMainContent.Children.Add(groupBoxFuncs);

            #endregion

            #region 车站


            #endregion
            RegisterFunction("更新", "A");
        }
        private void Cbx_Checked(object sender, RoutedEventArgs e)
        {
            foreach (var item in lstStation.Items)
            {
                (item as CheckBox).IsChecked = cbxStation.IsChecked;
            }

        }

        protected override void funcRegistered(string strAccelerateKey)
        {
            switch (strAccelerateKey)
            {
                case "A":
                    CheckList<string>(lstStation, CheckedStation);

                    if (CheckedStation.Count == 0)
                    {
                        MessageBox.Show("请选择车站", "确认", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                    if (_data.Error != null)
                    {
                        MessageBox.Show("输入有误！", "确认", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    if (MessageBox.Show("确认添加？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) == MessageBoxResult.OK)
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

        private void CheckList<T>(ListBox listBox, List<T> checkedList)
        {
            checkedList.Clear();
            foreach (var item in listBox.Items)
            {
                CheckBox cbx = (item as CheckBox);
                if (cbx.IsChecked == true)
                {
                    checkedList.Add((T)cbx.Tag);
                }
            }
        }
    }

    public class SoftwareVersionForSend
    {
        public ObservableCollection<byte[]> StationNid { get; set; }
        public ObservableCollection<string> Stationid { get; set; }
    }
}
```
# 报文修改

MN.0102.StationMonitor.UCSendParam.xaml.cs
```c#
private ObservableCollection<UCStationMonitor.ParamVersion> _dataResult =
            new ObservableCollection<UCStationMonitor.ParamVersion>();
        public void UpdateSoft()
        {
            SoftwareVersionForSend param = null;
            lock (((ICollection)_updateall).SyncRoot)//消息同步控制
            {
                SoftwareVersionForSend softVersionforSend = _updateall.Dequeue();
                List<string> failStation = new List<string>();
                List<string> successStation = new List<string>();
                StringBuilder sb = new StringBuilder();
                sb.Clear();
                int size = 0, su = 0;

                foreach (var station in softVersionforSend.Stationid)
                {
                    #region build Param

                    List<RM6081ParamQueryRec_t> rm6081TempList = new List<RM6081ParamQueryRec_t>();
                    foreach (var paramTemp in param.Params)
                    {
                        RM6081ParamQueryRec_t temp = new RM6081ParamQueryRec_t();
                        temp.ParamType = DataConverter.StrHex2Bytes(paramTemp.ParamType);
                        temp.ParamVersion = new HHJT_BCD(7, paramTemp.ParamVer);
                        rm6081TempList.Add(temp);
                    }

                    XDRStream stream = new XDRStream();

                    P6081Req p6081 = new P6081Req();
                    p6081.Header = new PackageHeader_t();
                    p6081.Header.MessageType = 0x6081;
                    p6081.Header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
                    p6081.Header.PacketLength = (uint)(5); //+ param.Params.Count * 9
                    p6081.Header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
                    p6081.Header.DestinationID = DataConverter.StrHex2Bytes(station + "6101");
                    p6081.Header.MessageID = new byte[4];
                    p6081.Header.PacketNbr = new byte[12];
                    p6081.Header.CRC = new byte[4];
                    p6081.Header.ResponseCode = 0x01;
                    p6081.Header.CompressCypherFlag = 0x01;
                    p6081.Header.MacFlag = 0x01;

                    p6081.Body6081Req = new C6081Req_t();
                    p6081.Body6081Req.DestNid = DataConverter.StrHex2Bytes(station + "0000");
                    p6081.Body6081Req.ParamStatus = 0x00;
                    //p6081.Body6081Req.RecCount = param.Params.Count;
                    //p6081.Body6081Req.RM6081ParamQueryRec = rm6081TempList.ToArray();

                    p6081.CRC = new PackageCrc_t();
                    p6081.CRC.CRC = new byte[4];

                    p6081.Header.xdr_encode(stream);
                    p6081.Body6081Req.xdr_encode(stream);
                    p6081.CRC.xdr_encode(stream);

                    byte[] data = stream.Get_data();
                    // Console.WriteLine(DataConverter.Bytes2HexStr(data));


                    #endregion

                    #region send Param

                    TcpClient tcpClient = new TcpClient();
                    try
                    {
                        tcpClient.Connect
                            (IPAddress.Parse(LCCConfig.LineCommIP),
                                LCCConfig.LineCommPort);
                        NetworkStream networkStream = tcpClient.GetStream();

                        networkStream.Write(data, 0, data.Length);

                        byte[] receive = new byte[1024];
                        int rcvLength = networkStream.Read(receive, 0, receive.Length);
                        if (rcvLength > 66)
                        {
                            XDRStream streamRes = new XDRStream(receive, rcvLength);

                            P6081Res c6081 = new P6081Res();
                            c6081.xdr_decode(streamRes);

                            List<UCStationMonitor.ParamVersion> versions = new List<UCStationMonitor.ParamVersion>();

                            for (int i = 0; i < c6081.Body6081Res.RecCount; i++)
                            {
                                UCStationMonitor.ParamVersion result = new UCStationMonitor.ParamVersion();
                                result.DeviceNid = station + "0000";
                                result.ParamType = DataConverter.Bytes2HexStr(c6081.Body6081Res.RM6081ParamQueryRec[i].ParamType);
                                result.ParamVer = c6081.Body6081Res.RM6081ParamQueryRec[i].ParamVersion;

                                var v = Dlg_DeviceParam.CurrentParam.FirstOrDefault(
                                    d => d.ParamType.Equals(result.ParamType));
                                if (v == null)
                                {
                                    result.ParamConsist = "未知";
                                }
                                else if (!result.ParamVer.Equals(v.ParamVer))
                                {
                                    result.ParamConsist = "不一致";
                                }
                                else
                                {
                                    result.ParamConsist = "一致";
                                }

                                versions.Add(result);
                            }

                            /*this.Dispatcher.Invoke(new Action(() =>
                            {
                                foreach (var paramVersion in versions)
                                {
                                    _dataResult.Add(paramVersion);
                                }
                            }));*/

                        }
                        else
                        {
                            MessageBox.Show(string.Format("{0}接收应答失败，发送不成功", station + "0000"), "错误", MessageBoxButton.OK, MessageBoxImage.Error);

                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "发送失败", MessageBoxButton.OK, MessageBoxImage.Error);
                        MessageBox.Show(ex.ToString());
                    }
                    finally
                    {
                        tcpClient.Close();
                    }
                    
                }
            }
        }
```
MN.0102.StationMonitor.UCSoftVersion.xaml.cs
```C#
        public void SendSoftVersionCheck()
        {
            IsCheaking = true;

            foreach (var station in DeviceForSend)
            {
                #region 6082

                XDRStream stream = new XDRStream();
                PackageHeader_t header = new PackageHeader_t();
                header.MessageType = 0x6082;
                header.SourceID = DataConverter.StrHex2Bytes(LCCConfig.WSNID);
                header.PacketFlag = DataConverter.StrHex2Bytes("ABCDEF89");
                header.DestinationID = DataConverter.StrHex2Bytes(station.DeviceNID.Substring(0, 4) + "6101"); 
                header.MessageID = new byte[4];
                header.PacketNbr = new byte[12];
                header.CRC = new byte[4];
                header.ResponseCode = 0x01;
                header.CompressCypherFlag = 0x01;
                header.MacFlag = 0x01;

                List<byte> byteList = new List<byte>();

                byteList.AddRange(DataConverter.StrHex2Bytes(station.DeviceNID));
                byteList.Add((byte)0x01);

                header.PacketLength = (uint)byteList.Count;
                header.xdr_encode(stream);
                stream.Put_bytes(byteList.ToArray(), byteList.Count);
                PackageCrc_t CRC = new PackageCrc_t();
                CRC.CRC = new byte[4];
                CRC.xdr_encode(stream);
                byte[] data = stream.Get_data();
                Console.WriteLine(DataConverter.Bytes2HexStr(data));

                #endregion

                #region send Param

                TcpClient tcpClient = new TcpClient();
                try
                {
                    tcpClient.Connect
                        (IPAddress.Parse(LCCConfig.LineCommIP),
                         LCCConfig.LineCommPort);
                    NetworkStream networkStream = tcpClient.GetStream();

                    networkStream.Write(data, 0, data.Length);

                    byte[] receive = new byte[1024];
                    int rcvLength = networkStream.Read(receive, 0, receive.Length);
                    if (rcvLength > 66)
                    {
                        XDRStream streamRes = new XDRStream(receive, rcvLength);

                        P6082Res c6082 = new P6082Res();
                        c6082.xdr_decode(streamRes);

                        List<SoftversionResult> versions = new List<SoftversionResult>();

                        for (int i = 0; i < c6082.Body6082Res.RecCount; i++)
                        {
                            SoftversionResult result = new SoftversionResult();
                            result.DeviceNid = station.DeviceNID;
                            result.DeviceResult = "查询成功";
                            result.EQU_TYPE = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftType.ToString();
                            result.EQU_VERSION = c6082.Body6082Res.RM6082SoftQueryRec[i].SoftVersion;
                            versions.Add(result);
                        }
                        this.Dispatcher.Invoke(new Action(() =>
                        {
                            foreach (var paramVersion in versions)
                            {
                                _dataSoftVerResult.Add(paramVersion);
                            }
                        }));

                    }
                    else
                    {
                        MessageBox.Show(string.Format("{0}接收应答失败，发送不成功", station.DeviceName), "错误", MessageBoxButton.OK, MessageBoxImage.Error);

                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "发送失败", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                finally
                {
                    tcpClient.Close();
                }

                #endregion
            }

            IsCheaking = false;
        }
    
```

# 配置文件
```c#
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using HHJT.AFC.Base.API;
using HHJT.AFC.Base.OracleDBI;
using DLMLC.Base;

namespace DLMLC.Modules.Common
{
    public class LCCConfig
    {
        private static LCCConfigItem _data;

        public static int UserNameMaxLength
        {
            get { return _data.UserNameMaxLength; }
            set { }
        }

        public static int UserPWDMaxLength
        {
            get { return _data.UserPWDMaxLength; }
            set { }
        }
        public static int ReportTime
        {
            get { return _data.ReportTime; }
            set { }
        }
        public static int AutoLogout
        {
            get { return _data.AutoLogout; }
            set { }
        }
 
        public static int AutoQueryTimeSpan
        {
            get { return _data.AutoQueryTimeSpan; }
            set { }
        }
        public static string LineMapBackgound { get { return _data.LineMapBackgound; } set { } }
        public static string StationMapWelcome { get { return _data.StationMapWelcome; } set { } }
        public static string LoadMapTip { get { return _data.LoadMapTip; } set { } }
        public static string IconBOM_Normal { get { return _data.IconBOM_Normal; } set { } }
        public static string IconBOM_Warning { get { return _data.IconBOM_Warning; } set { } }
        public static string IconBOM_Error { get { return _data.IconBOM_Error; } set { } }
        public static string IconBOM_Offline { get { return _data.IconBOM_Offline; } set { } }
        public static string IconBOM_Close { get { return _data.IconBOM_Close; } set { } }

        public static string IconISM_Normal { get { return _data.IconISM_Normal; } set { } }
        public static string IconISM_Warning { get { return _data.IconISM_Warning; } set { } }
        public static string IconISM_Error { get { return _data.IconISM_Error; } set { } }
        public static string IconISM_Offline { get { return _data.IconISM_Offline; } set { } }
        public static string IconISM_Close { get { return _data.IconISM_Close; } set { } }





        public static string IconSSM_Normal { get { return _data.IconSSM_Normal; } set { } }
        public static string IconSSM_Warning { get { return _data.IconSSM_Warning; } set { } }
        public static string IconSSM_Error { get { return _data.IconSSM_Error; } set { } }
        public static string IconSSM_Offline { get { return _data.IconSSM_Offline; } set { } }
        public static string IconSSM_Close { get { return _data.IconSSM_Close; } set { } }
        public static string IconTVM_Normal { get { return _data.IconTVM_Normal; } set { } }
        public static string IconTVM_Warning { get { return _data.IconTVM_Warning; } set { } }
        public static string IconTVM_Error { get { return _data.IconTVM_Error; } set { } }
        public static string IconTVM_Offline { get { return _data.IconTVM_Offline; } set { } }
        public static string IconTVM_Close { get { return _data.IconTVM_Close; } set { } }
        public static string IconTCM_Normal { get { return _data.IconTCM_Normal; } set { } }
        public static string IconTCM_Warning { get { return _data.IconTCM_Warning; } set { } }
        public static string IconTCM_Error { get { return _data.IconTCM_Error; } set { } }
        public static string IconTCM_Offline { get { return _data.IconTCM_Offline; } set { } }
        public static string IconTCM_Close { get { return _data.IconTCM_Close; } set { } }
        public static string IconSTM_Normal { get { return _data.IconSTM_Normal; } set { } }
        public static string IconSTM_Warning { get { return _data.IconSTM_Warning; } set { } }
        public static string IconSTM_Error { get { return _data.IconSTM_Error; } set { } }
        public static string IconSTM_Offline { get { return _data.IconSTM_Offline; } set { } }
        public static string IconSTM_Close { get { return _data.IconSTM_Close; } set { } }
        public static string IconEnG_Normal { get { return _data.IconEnG_Normal; } set { } }
        public static string IconEnG_Warning { get { return _data.IconEnG_Warning; } set { } }
        public static string IconEnG_Error { get { return _data.IconEnG_Error; } set { } }
        public static string IconEnG_Offline { get { return _data.IconEnG_Offline; } set { } }
        public static string IconEnG_Close { get { return _data.IconEnG_Close; } set { } }
        public static string IconExG_Normal { get { return _data.IconExG_Normal; } set { } }
        public static string IconExG_Warning { get { return _data.IconExG_Warning; } set { } }
        public static string IconExG_Error { get { return _data.IconExG_Error; } set { } }
        public static string IconExG_Offline { get { return _data.IconExG_Offline; } set { } }
        public static string IconExG_Close { get { return _data.IconExG_Close; } set { } }
        public static string IconRvG_Normal { get { return _data.IconRvG_Normal; } set { } }
        public static string IconRvG_Warning { get { return _data.IconRvG_Warning; } set { } }
        public static string IconRvG_Error { get { return _data.IconRvG_Error; } set { } }
        public static string IconRvG_Offline { get { return _data.IconRvG_Offline; } set { } }
        public static string IconRvG_Close { get { return _data.IconRvG_Close; } set { } }
        public static string IconRvG_Single { get { return _data.IconRvG_Single; } set { } }
        public static string IconSnC_Offline { get { return _data.IconSnC_Offline; } set { } }
        public static string IconSnC_Warning { get { return _data.IconSnC_Warning; } set { } }
        public static string IconSnC_Error { get { return _data.IconSnC_Error; } set { } }
        public static string IconSnC_Close { get { return _data.IconSnC_Close; } set { } }
        public static string IconSnC_Normal { get { return _data.IconSnC_Normal; } set { } }
        public static string MLC_Type { get { return _data.MLC_Type; } set { } }
        public static string LineCommIP { get { return _data.LineCommIP; } set { } }
        public static int LineCommPort { get { return _data.LineCommPort; } set { } }
        public static string WSNID { get { return _data.WSNID; } set { } }

        public static int WarningThreshold { get { return _data.WarningThreshold; } set { _data.WarningThreshold = value; } }
        public static int FailureThreshold { get { return _data.FailureThreshold; } set { _data.FailureThreshold = value; } }
       
        
        static LCCConfig()
        {
            _data = new LCCConfigItem();
            XMLManipulater.ReadXMLInfoToObject(DLMLC_ProjectInfo.ConfigFileName, _data, typeof(LCCConfigItem));
            DefaultPWD = "00000000";
        }
        public static string DefaultPWD { get; set; }
    }

    public class LCCConfigItem
    {

        public string LineMapBackgound { get; set; }
        public string StationMapWelcome { get; set; }
        public string LoadMapTip { get; set; }
        public string IconBOM_Normal { get; set; }
        public string IconBOM_Warning { get; set; }
        public string IconBOM_Error { get; set; }
        public string IconBOM_Offline { get; set; }
        public string IconBOM_Close { get; set; }

        public string IconISM_Normal { get; set; }
        public string IconISM_Warning { get; set; }
        public string IconISM_Error { get; set; }
        public string IconISM_Offline { get; set; }
        public string IconISM_Close { get; set; }

        public string IconSSM_Normal { get; set; }
        public string IconSSM_Warning { get; set; }
        public string IconSSM_Error { get; set; }
        public string IconSSM_Offline { get; set; }
        public string IconSSM_Close { get; set; }
        public string IconTVM_Normal { get; set; }
        public string IconTVM_Warning { get; set; }
        public string IconTVM_Error { get; set; }
        public string IconTVM_Offline { get; set; }
        public string IconTVM_Close { get; set; }
        public string IconTCM_Normal { get; set; }
        public string IconTCM_Warning { get; set; }
        public string IconTCM_Error { get; set; }
        public string IconTCM_Offline { get; set; }
        public string IconTCM_Close { get; set; }
        public string IconSTM_Normal { get; set; }
        public string IconSTM_Warning { get; set; }
        public string IconSTM_Error { get; set; }
        public string IconSTM_Offline { get; set; }
        public string IconSTM_Close { get; set; }
        public string IconEnG_Normal { get; set; }
        public string IconEnG_Warning { get; set; }
        public string IconEnG_Error { get; set; }
        public string IconEnG_Offline { get; set; }
        public string IconEnG_Close { get; set; }
        public string IconExG_Normal { get; set; }
        public string IconExG_Warning { get; set; }
        public string IconExG_Error { get; set; }
        public string IconExG_Offline { get; set; }
        public string IconExG_Close { get; set; }
        public string IconRvG_Normal { get; set; }
        public string IconRvG_Warning { get; set; }
        public string IconRvG_Error { get; set; }
        public string IconRvG_Offline { get; set; }
        public string IconRvG_Close { get; set; }
        public string IconRvG_Single { get; set; }
        public string IconSnC_Offline { get; set; }
        public string IconSnC_Error { get; set; }
        public string IconSnC_Close { get; set; }
        public string IconSnC_Warning { get; set; }
        public string IconSnC_Normal { get; set; }
        public string MLC_Type { get; set; }
        public string LineCommIP { get; set; }
        public int LineCommPort { get; set; }
        public string WSNID { get; set; }
        public int AutoLogout { get; set; }
        public int AutoQueryTimeSpan { get; set; }
        public int ReportTime { get; set; }
        public int UserNameMaxLength { get; set; }
        public int UserPWDMaxLength { get; set; }

        public int WarningThreshold { get; set; }
        public int FailureThreshold { get; set; }


    }
}

```
LCC.App.cs
```C#
using DLMLC.Modules.Common;

 ProjectInfo = (LCCConfig.MLC_Type == "5") ? ("大连地铁线路分中心五号线中央计算机系统") : ("大连地铁多线路中央计算机系统")
```
LCC.MainWindows.cs
```c#
lblTitle.Content = (LCCConfig.MLC_Type == "5") ? ("大连地铁线路分中心五号线中央计算机系统") : ("大连地铁多线路中央计算机系统");
SoftWareTitle = (LCCConfig.MLC_Type == "5") ? ("大连地铁线路分中心五号线中央计算机系统") : ("大连地铁多线路中央计算机系统"); 
```
## 功能项
监控管理子系统 综合设置  
报表管理子系统 清分类  
票务管理子系统 票卡申请  
系统管理子系统
参数管理子系统
系统监控 车站监控 按钮
系统监控 线路监控 按钮
系统监控 参数版本 更新信息
系统监控 软件版本 更新信息 

# 用户信息查询数据库
LCC.UCLogin.xaml.cs line129 
```c#
object rtn = 0;
string strSQL = "";
if(LCCConfig.MLC_Type != "5")
{
    strSQL =
    string.Format(
        "select count(*) from T_SUPPERUSER where OPERATOR_NBR=to_number('{0}') and OPERATOR_PASSWORD=to_number('{1}') and rownum=1",
        userId, password);
        
        OracleDB.ST_GetScalarObj(strSQL, out rtn);
}
else
{
    string strIP = OracleDB.GetElementByName("DBConfig.xml", "MlcServerIP");
    string strPort = OracleDB.GetElementByName("DBConfig.xml", "MlcServerPort");
    string strServiceName = OracleDB.GetElementByName("DBConfig.xml", "MlcServiceName");
    string strUserID = OracleDB.GetElementByName("DBConfig.xml", "MlcUserID");
    string strPassword = OracleDB.GetElementByName("DBConfig.xml", "MlcPassword");

    string _staticStrConn = string.Format("Data Source=(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP) (HOST = {0})(PORT = {1})))(CONNECT_DATA =(SERVICE_NAME = {2}))); User ID={3};Password={4}",
strIP, strPort, strServiceName, strUserID, strPassword);

    strSQL =
    string.Format(
        "select count(*) from T_SUPPERUSER where OPERATOR_NBR=to_number('{0}') and OPERATOR_PASSWORD=to_number('{1}') and rownum=1",
        userId, password);
    var v = (new OracleDB(_staticStrConn)).DB_GetScalarObj(strSQL, out rtn);

}
```
LCC.UCLogin.xaml.cs line157 
```c#
ObservableCollection<M_UserInfo> data = new ObservableCollection<M_UserInfo>();
if (LCCConfig.MLC_Type != "5")
{
    strSQL =
    string.Format(
        "select USER_ID,USER_NAME,USER_PIN,VALID_TIME,USE_TIME,COMPANY_NAME From TBL_SS_USER where USER_ID='{0}' and rownum=1",
        userId);
    
    //OracleResult v;
    //var v = OracleDB.ST_RetrieveToSingleObject(strSQL, data, typeof (M_UserInfo));
    _ = OracleDB.ST_RetrieveToObjects(strSQL, data, typeof(M_UserInfo));
}
else
{
    string strIP = OracleDB.GetElementByName("DBConfig.xml", "MlcServerIP");
    string strPort = OracleDB.GetElementByName("DBConfig.xml", "MlcServerPort");
    string strServiceName = OracleDB.GetElementByName("DBConfig.xml", "MlcServiceName");
    string strUserID = OracleDB.GetElementByName("DBConfig.xml", "MlcUserID");
    string strPassword = OracleDB.GetElementByName("DBConfig.xml", "MlcPassword");

    string _staticStrConn = string.Format("Data Source=(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP) (HOST = {0})(PORT = {1})))(CONNECT_DATA =(SERVICE_NAME = {2}))); User ID={3};Password={4}",
strIP, strPort, strServiceName, strUserID, strPassword);

    strSQL =
    string.Format(
        "select USER_ID,USER_NAME,USER_PIN,VALID_TIME,USE_TIME,COMPANY_NAME From TBL_SS_USER where USER_ID='{0}' and rownum=1",
        userId);
    _ = (new OracleDB(_staticStrConn)).DB_RetrieveToObjects(strSQL, data, typeof(M_UserInfo));

}
```

LCC.UCLogin.xaml.cs line248 
```c#
 if(LCCConfig.MLC_Type != "5")
{
    OracleDB.ST_RetrieveToIntArray(strCmd, funcList);
}
else
{
    string strIP = OracleDB.GetElementByName("DBConfig.xml", "MlcServerIP");
    string strPort = OracleDB.GetElementByName("DBConfig.xml", "MlcServerPort");
    string strServiceName = OracleDB.GetElementByName("DBConfig.xml", "MlcServiceName");
    string strUserID = OracleDB.GetElementByName("DBConfig.xml", "MlcUserID");
    string strPassword = OracleDB.GetElementByName("DBConfig.xml", "MlcPassword");

    string _staticStrConn = string.Format("Data Source=(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP) (HOST = {0})(PORT = {1})))(CONNECT_DATA =(SERVICE_NAME = {2}))); User ID={3};Password={4}",
strIP, strPort, strServiceName, strUserID, strPassword);

    strSQL =
    string.Format(
        "select USER_ID,USER_NAME,USER_PIN,VALID_TIME,USE_TIME,COMPANY_NAME From TBL_SS_USER where USER_ID='{0}' and rownum=1",
        userId);
    (new OracleDB(_staticStrConn)).DB_RetrieveToIntArray(strCmd, funcList);
}
```

# 票箱类型
LCC.RP.cs line144 未修改
```c#
        public static string BillBoxTypeQuery =
    @" case loc_id
                when '00' THEN '纸币废钞箱'
                when '01' THEN '1元钞箱'
                when '02' THEN '5元钞箱'
                when '03' THEN '纸币回收箱'
                when '04' THEN '硬币备用箱A'
                when '05' THEN '硬币备用箱B'
                when '06' THEN '硬币回收箱'
                else '未知钱箱'
                end ";


        public static string BillBoxTypeQuery =
    @" case loc_id
                when '01' THEN '纸币找零箱1'
                when '02' THEN '纸币找零箱2'
                when '03' THEN '纸币找零箱3'
                when '04' THEN '纸币找零箱4'
                when '05' THEN '纸币找零废票箱'
                when '06' THEN '纸币回收箱1'
                when '07' THEN '纸币回收箱2'
                when '08' THEN '硬币备用钱箱1'
                when '09' THEN '硬币备用钱箱2'
                when '10' THEN '硬币缓存钱箱1'
                when '11' THEN '硬币缓存钱箱2'
                when '12' THEN '硬币回收箱'
                else '未知钱箱'
                end ";
```


new Parameter() {PName = "LineId", PValue = LineId == null ? "全部":DS_Line.FirstOrDefault(d => d.Key == LineId).Value},

# 票卡目录及线路中心显示
未修改  
Base.SystemDS.cs 
```c#
case DSString.TICKET_DIRECTORY:
                    //strSQL += @"select DIREC_CODE,DIREC_NAME from TBL_SS_TICKET_DIRECTORY";
                    strSQL += @"select DIREC_CODE,DIREC_NAME from TBL_SS_TICKET_DIRECTORY
                                where DIREC_NAME in ('普通单程票','普通明珠卡','乘次票A','乘次票B')  order by 1";
case DSString.CENTER_ID:
                    strSQL += @"select substr(station_nid,1,2),to_number(substr(station_nid,1,2),'XX')||'号线' from tbl_ss_current_station where rownum = 1 order by 1";








                            
```
