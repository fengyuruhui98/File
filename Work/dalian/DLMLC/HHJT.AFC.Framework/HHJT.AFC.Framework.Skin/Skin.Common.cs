using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Windows;
using System.Windows.Media.Imaging;
namespace HHJT.AFC.Framework.Skin
{
    /// <summary>
    /// WPF程序皮肤类型（蓝色，黑色，绿色），可根据需要自定义扩展
    /// </summary>
    public enum SKIN_TYPE { SKIN_BLUE, SKIN_BLACK, SKIN_GREEN };

    /// <summary>
    /// 图片索引
    /// </summary>
    public enum HHJT_PICS
    {
        About,
        ActionAdd,
        ActionConfirm,
        ActionDelete,
        ActionMinus,
        ActionStop,
        Alarm,
        Attention,
        AutoUpdate,
        BroadCast,
        Caculator,
        Calender,
        Clock,
        Column,
        Computer,
        Config,
        ConfigTool,
        Cut,
        Database,
        DB,
        DBAdd,
        DBAlarm,
        DBBackup,
        DBClearup,
        DBDelete,
        DBFetch,
        DBRefresh,
        DBSave,
        DBShutDown,
        Detail,
        Directory,
        Donet,
        Down,
        Download,
        Exit,
        Export,

        FileGet,
        FileSave,
        Find,
        FlagBlue,
        FlagBrown,
        FlagGreen,
        FlagRed,
        FlagYellow,
        Group,
        Hand,
        Help,
        Help1,
        Home,
        HHJT,
        Key,
        Left,
        LightOff,
        LightOn,
        Lock,
        Log,
        Log1,
        Login,
        Map,
        MessageWhite,
        MessageYellow,
        Music,
        New,
        New1,
        Pane,
        Path,
        Pie,
        Plugin,
        Pointer,
        Press,
        Print,
        Printer,
        Refresh,
        Report,
        Report1,
        Right,
        Save,
        Up,
        UpdateSoftware,
        User,
        UserAdd,
        UserDelete,
        UserGroup
    }

    /// <summary>
    /// HHJT AFC系统UI通用函数
    /// 功能：应用皮肤
    ///       获取图片资源
    /// </summary>
    public static class HHJT_AFC_Skin_Commons
    {
        //图片集合
        private static Dictionary<HHJT_PICS, string> _pics = new Dictionary<HHJT_PICS, string>();

        //静态类初始化
        static HHJT_AFC_Skin_Commons()
        {
            #region 图片初始化
            _pics.Add(HHJT_PICS.About, "about.png");
            _pics.Add(HHJT_PICS.ActionAdd, "action_add.png");
            _pics.Add(HHJT_PICS.ActionConfirm, "action_confirm.png");
            _pics.Add(HHJT_PICS.ActionDelete, "action_delete.png");
            _pics.Add(HHJT_PICS.ActionMinus, "action_minus.png");
            _pics.Add(HHJT_PICS.ActionStop, "action_stop.png");
            _pics.Add(HHJT_PICS.Alarm, "alarm.png");
            _pics.Add(HHJT_PICS.Attention, "attention.png");
            _pics.Add(HHJT_PICS.AutoUpdate, "autoupdate.png");
            _pics.Add(HHJT_PICS.BroadCast, "broadcast.png");
            _pics.Add(HHJT_PICS.Caculator, "caculator.png");
            _pics.Add(HHJT_PICS.Calender, "calender.png");
            _pics.Add(HHJT_PICS.Clock, "clock.png");
            _pics.Add(HHJT_PICS.Column, "column.png");
            _pics.Add(HHJT_PICS.Computer, "computer.png");
            _pics.Add(HHJT_PICS.Config, "config.png");
            _pics.Add(HHJT_PICS.ConfigTool, "config_tool.png");
            _pics.Add(HHJT_PICS.Cut, "cut.png");
            _pics.Add(HHJT_PICS.Database, "database.png");
            _pics.Add(HHJT_PICS.DB, "db.png");
            _pics.Add(HHJT_PICS.DBAdd, "db_add.png");
            _pics.Add(HHJT_PICS.DBAlarm, "db_alarm.png");
            _pics.Add(HHJT_PICS.DBBackup, "db_backup.png");
            _pics.Add(HHJT_PICS.DBClearup, "db_clearup.png");
            _pics.Add(HHJT_PICS.DBDelete, "db_delete.png");
            _pics.Add(HHJT_PICS.DBFetch, "db_retch.png");
            _pics.Add(HHJT_PICS.DBRefresh, "db_refresh.png");
            _pics.Add(HHJT_PICS.DBSave, "db_save.png");
            _pics.Add(HHJT_PICS.DBShutDown, "db_shutdown.png");
            _pics.Add(HHJT_PICS.Detail, "detail.png");
            _pics.Add(HHJT_PICS.Directory, "directory.png");
            _pics.Add(HHJT_PICS.Donet, "donet.png");
            _pics.Add(HHJT_PICS.Down, "down.png");
            _pics.Add(HHJT_PICS.Download, "download.png");
            _pics.Add(HHJT_PICS.Exit, "exit.png");
            _pics.Add(HHJT_PICS.Export, "export.png");
            _pics.Add(HHJT_PICS.FileGet, "fileget.png");
            _pics.Add(HHJT_PICS.FileSave, "filesave.png");
            _pics.Add(HHJT_PICS.Find, "find.png");
            _pics.Add(HHJT_PICS.FlagBlue, "flag_blue.png");
            _pics.Add(HHJT_PICS.FlagBrown, "flag_brown.png");
            _pics.Add(HHJT_PICS.FlagGreen, "flag_green.png");
            _pics.Add(HHJT_PICS.FlagRed, "flag_red.png");
            _pics.Add(HHJT_PICS.FlagYellow, "flag_yellow.png");
            _pics.Add(HHJT_PICS.Group, "group.png");
            _pics.Add(HHJT_PICS.Hand, "hand.png");
            _pics.Add(HHJT_PICS.Help, "help.png");
            _pics.Add(HHJT_PICS.Help1, "help1.png");
            _pics.Add(HHJT_PICS.Home, "home.png");
            _pics.Add(HHJT_PICS.HHJT, "huahong.png");
            _pics.Add(HHJT_PICS.Key, "key.png");
            _pics.Add(HHJT_PICS.Left, "left.png");
            _pics.Add(HHJT_PICS.LightOff, "light_off.png");
            _pics.Add(HHJT_PICS.LightOn, "light_on.png");
            _pics.Add(HHJT_PICS.Lock, "lock.png");
            _pics.Add(HHJT_PICS.Log, "log.png");
            _pics.Add(HHJT_PICS.Log1, "log1.png");
            _pics.Add(HHJT_PICS.Login, "login.png");
            _pics.Add(HHJT_PICS.Map, "map.png");
            _pics.Add(HHJT_PICS.MessageWhite, "message_white.png");
            _pics.Add(HHJT_PICS.MessageYellow, "message_yellow.png");
            _pics.Add(HHJT_PICS.Music, "music.png");
            _pics.Add(HHJT_PICS.New, "new.png");
            _pics.Add(HHJT_PICS.New1, "new1.png");
            _pics.Add(HHJT_PICS.Pane, "pane.png");
            _pics.Add(HHJT_PICS.Path, "path.png");
            _pics.Add(HHJT_PICS.Pie, "pie.png");
            _pics.Add(HHJT_PICS.Plugin, "plugin.png");
            _pics.Add(HHJT_PICS.Pointer, "pointer.png");
            _pics.Add(HHJT_PICS.Press, "press.png");
            _pics.Add(HHJT_PICS.Print, "print.png");
            _pics.Add(HHJT_PICS.Printer, "printer.png");
            _pics.Add(HHJT_PICS.Refresh, "refresh.png");
            _pics.Add(HHJT_PICS.Report, "report.png");
            _pics.Add(HHJT_PICS.Report1, "report1.png");
            _pics.Add(HHJT_PICS.Right, "right.png");
            _pics.Add(HHJT_PICS.Save, "save.png");
            _pics.Add(HHJT_PICS.Up, "up.png");
            _pics.Add(HHJT_PICS.UpdateSoftware, "updatesoftware.png");
            _pics.Add(HHJT_PICS.User, "user.png");
            _pics.Add(HHJT_PICS.UserAdd, "user_add.png");
            _pics.Add(HHJT_PICS.UserDelete, "user_delete.png");
            _pics.Add(HHJT_PICS.UserGroup, "user_group.png");
            #endregion
        }

        /// <summary>
        /// 给程序应用皮肤
        /// </summary>
        /// <param name="app"></param>
        /// <param name="skinType"></param>
        public static void ApplySkin(Application app, SKIN_TYPE skinType)
        {
            string uriStr = @"pack://application:,,,/HHJT.AFC.Framework.Skin;component/Skins/";
            switch (skinType)
            {
                case SKIN_TYPE.SKIN_BLUE:
                    uriStr += "BlueSkin.xaml";
                    break;
                case SKIN_TYPE.SKIN_BLACK:
                    uriStr += "BlackSkin.xaml";
                    break;
                case SKIN_TYPE.SKIN_GREEN:
                    uriStr += "GreenSkin.xaml";
                    break;
                default:
                    break;
            }

            //string str = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            //str = str.Remove(str.IndexOf(System.Diagnostics.Process.GetCurrentProcess().MainModule.ModuleName));
            //Environment.CurrentDirectory = str;
            //Assembly.LoadFrom(@"HHJT.AFC.Framework.Skin.dll");

            ResourceDictionary skinDict = new ResourceDictionary
            {
                Source = new Uri(uriStr)
            };

            Collection<ResourceDictionary> mergedDicts = app.Resources.MergedDictionaries;
            if (mergedDicts.Count > 0)
                mergedDicts.Clear();

            mergedDicts.Add(skinDict);
        }

        /// <summary>
        /// 按照图片名称获取HHJT.AFC.UI里 Pics目录下的图片资源Uri
        /// </summary>
        /// <param name="picName"></param>
        /// <returns></returns>
        public static Uri GetPicUriByName(string picName)
        {
            return new Uri(@"pack://application:,,,/HHJT.AFC.Framework.Skin;component/Pics/" + picName);
        }

        /// <summary>
        /// 按照图片索引获取Harbin.CCHS.UIBase里 Pics目录下的图片资源Uri
        /// </summary>
        /// <param name="picIndex"></param>
        /// <returns></returns>
        public static Uri GetPicUriByIndex(HHJT_PICS picIndex)
        {
            return GetPicUriByName(_pics[picIndex]);
        }

        public static BitmapImage GetBitmapImageByIndex(HHJT_PICS picIndex)
        {
            return new BitmapImage(GetPicUriByIndex(picIndex));
        }

        public static string GetPicUriStringByIndex(HHJT_PICS picIndex)
        {
            return @"pack://application:,,,/HHJT.AFC.Framework.Skin;component/Pics/" + _pics[picIndex];
        }
    }
}
