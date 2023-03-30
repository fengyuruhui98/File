using HHJT.AFC.Framework.Skin;
using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;

namespace HHJT.AFC.Framework.UI
{
    public class AFCAppInfo
    {
        //子程序（子系统）所包含模块
        private Collection<ModuleInfo> m_modules = new Collection<ModuleInfo>();

        #region properties

        public int AppID { get; set; }                  //应用程序ID，如 01
        public string AppName { get; set; }             //应用程序名称，如 数据交换子系统
        public string AppAbbreviation { get; set; }     //应用程序缩写，如 DX
        public string ProjectInfo { get; set; }
        public Collection<ModuleInfo> Modules
        {
            get { return m_modules; }
        }
        #endregion

        /// <summary>
        /// 为子程序（子系统）添加应用模块，如账务子系统之可疑帐调整
        /// </summary>
        /// <param name="module">应用模块</param>
        public void AddModule(ModuleInfo module)
        {
            m_modules.Add(module);
        }
    }

    /// <summary>
    /// 应用子程序或应用子系统（Sub Application or Sub System）
    /// 如账务子系统包括可疑帐调整、结算调整、日终处理、综合查询等模块
    /// </summary>
    public class ModuleInfo
    {
        #region fields
        private Collection<ModuleFunction> moduleFunctions  //模块所包含的功能
            = new Collection<ModuleFunction>();
        private UserControl moduleView = null;              //模块对应的视图
        #endregion

        #region properties
        /// <summary>
        /// 对应的应用子系统，如账务管理子系统
        /// </summary>
        public AFCAppInfo ParentAppInfo { get; set; }

        /// <summary>
        /// 模块名称
        /// </summary>
        public string ModuleName { get; set; }

        /// <summary>
        /// 获得模块功能列表，只读，添加功能键AddModuleFunction
        /// </summary>
        public Collection<ModuleFunction> ModuleFunctions
        {
            get { return moduleFunctions; }
        }
        /// <summary>
        /// 设置模块对应视图类型，以便系统自动生成
        /// </summary>
        public Type ModuleViewType { get; set; }
        #endregion

        #region methods

        /// <summary>
        /// 功能模块
        /// </summary>
        /// <param name="aFunctionModule"></param>
        public void AddModuleFunction(ModuleFunction moduleFunction)
        {
            moduleFunctions.Add(moduleFunction);
        }

        /// <summary>
        /// 获取模块对应界面视图，若未创建则创建新视图
        /// </summary>
        /// <returns></returns>
        public UserControl GetModuleView()
        {
            if (moduleView == null)
            {
                moduleView = (UserControl)System.Activator.CreateInstance(ModuleViewType);
            }
            return moduleView;

        }
        #endregion
    }

    /// <summary>
    /// 模块功能信息，满足功能分级可能包含功能、子功能两级分级要求
    /// </summary>
    public class ModuleFunction
    {
        #region properties

        public ModuleInfo ParentModule { get; set; }    //对应模块
        public string FunName { get; set; }             //功能名称
        public string FunCode { get; set; }             //功能代码
        public bool IsSubFun { get; set; }              //设置是否为子功能
        public string ParentFunName { get; set; }       //若为子功能设置上级功能名称
        #endregion
    }

    public class AFCAppBase<T> : Application where T : AFCMainWindowBase, new()
    {
        #region fields
        private AFCAppInfo _appInfo;    //程序信息
        private T m_mainWindowBase;         //主窗口
        #endregion

        #region properties
        /// <summary>
        /// 获得程序信息
        /// </summary>
        public AFCAppInfo AppInfo
        {
            get { return _appInfo; }
        }

        /// <summary>
        /// 获取主窗口
        /// </summary>
        public T MainWindowBase
        {
            get { return m_mainWindowBase; }
        }
        #endregion

        #region methods

        /// <summary>
        /// 构造函数，必须提供程序ID
        /// </summary>
        /// <param name="appID">程序ID，参照HarbinInfo static程序初始化设置</param>
        public AFCAppBase(AFCAppInfo appInfo)
            : base()
        {
            this._appInfo = appInfo;
            this.ShutdownMode = System.Windows.ShutdownMode.OnLastWindowClose;
        }

        /// <summary>
        /// OnStartup虚函数,需调用base对应OnStartup，若AppBase被继承，处理方法相同
        /// </summary>
        /// <param name="args">启动参数</param>
        protected override void OnStartup(StartupEventArgs args)
        {
            base.OnStartup(args);

            HHJT_AFC_Skin_Commons.ApplySkin(this, SKIN_TYPE.SKIN_GREEN);

            m_mainWindowBase = new T
            {
                //m_mainWindowBase.Title = AFCInfo.AFCProjectInfo;
                Title = _appInfo.ProjectInfo
            };

            m_mainWindowBase.Show();//在调用之前Run请确认MainWindowBase已创建
        }
        #endregion
    }
}