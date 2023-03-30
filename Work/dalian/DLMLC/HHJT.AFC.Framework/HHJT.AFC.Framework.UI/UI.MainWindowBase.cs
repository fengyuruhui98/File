using HHJT.AFC.Framework.Skin;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace HHJT.AFC.Framework.UI
{
    public interface IMainWindow
    {
        void SetTips(string strTips);
    }

    public class AFCMainWindowBase : Window, IMainWindow
    {
        #region fields
        protected bool _logoutbutton = false;//true:退出按钮，false:关闭键
        //最外层Grid
        protected Grid _rootGrid;
        //主布局
        protected DockPanel _parentPanel;

        //工具栏
        protected Button[] m_toolButtons = new Button[5];//工具栏按钮
        private string[] btnContent = new string[] { //对应工具栏显示内容
          "返回", /* "帮助", */ "关于...",  "打印屏幕", "修改密码" ,"更新","关机"};
        private HHJT_PICS[] btnPics = new HHJT_PICS[] { //对应工具栏图像
            HHJT_PICS.Exit, /*HARBIN_PICS.Help,*/ HHJT_PICS.About,  HHJT_PICS.Print, HHJT_PICS.Key,HHJT_PICS.LightOff, HHJT_PICS.Computer };

        //状态栏
        Label _labelStatus;
        Label _labelPCName;
        protected Label _labelUserName;
        protected Label _labelReportTime;
        Label _labelTime;
        protected Label _labelTips;

        protected Label _labelVersion;

        //更新界面时间
        DispatcherTimer _timer;

        protected int _countAutoLogout;//自动签退计数
        protected int _maxSecondsAutoLogout;//自动签退时间
        protected bool _flagLoggedIn;//登录状态
        protected bool _flagUpdate;//更新状态

        //用户区域
        protected Border m_userContent;
        protected Label lblTitle;

        //subsys列表
        protected List<SubSysInfo> _listSubsys = new List<SubSysInfo>();

        #endregion

        //private LoggedUserInfo _loggedUserInfo;

        //public LoggedUserInfo LoggedInfo
        //{
        //    get { return _loggedUserInfo; }
        //    set { _loggedUserInfo = value; }
        //}

        #region IPC-Message

        protected int ProcessID;

        //private UserRoleStruct _userInfo;

        //protected UserRoleStruct UserInfo
        //{
        //    get { return _userInfo; }
        //    set { _userInfo = value; }
        //}


        #endregion

        public AFCMainWindowBase()
            : base()
        {
            InitSubsystem();
            InitWindowBase();
            //_labelPCName.Content = System.Environment.MachineName;
            this.Closing += AFCMainWindowBase_Closing;
            this.PreviewKeyDown += Window_PreviewKeyDown;
            this.PreviewMouseDown += Window_PreviewMouseDown;
        }

        private void AFCMainWindowBase_Closing(object sender, CancelEventArgs e)
        {
            this.LogOut();
            if (_flagLoggedIn)
            {
                e.Cancel = true;
            }
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            //注册事件处理机制
            //HwndSource source = PresentationSource.FromVisual(this) as HwndSource;
            //if (source != null) source.AddHook(WndProc);

            //GetUserInfo
            //SharedMemory<UserRoleStruct> shMem = new SharedMemory<UserRoleStruct>("HarbinUserInfo", 256);

            //shMem.Open();
            //_userInfo = shMem.Data;
            //LoggedInfo = new LoggedUserInfo();

            //LoggedInfo.UserID = _userInfo.UserID;
            //LoggedInfo.RoleID = _userInfo.RoleID;
            //LoggedInfo.UserLoginTime = _userInfo.UserLoginTime;

            //LoggedInfo.FillInfo();

            //LoggedInfo.WorkStationName = Environment.MachineName;
            //LoggedInfo.WorkStationIP = getLocalIP();

            //LoggedInfo.AppStartTime = DateTime.Now;

            //LoggedInfo.UserPriviledges = LoggedInfo.RoleRights == null ? new string[] { "" } : LoggedInfo.RoleRights.Split(';');

            //            string test = "";
            //            foreach (var item in LoggedInfo.UserPriviledges)
            //            {
            //                test = test + item.ToString();
            //                test += "  ";
            //            }

            //            string strToShow = String.Format(@"用户ID：{0}
            //                                               角色ID：{1}
            //                                               用户登陆时间：{2}
            //                                               用户名：{3}
            //                                               角色名：{4}
            //                                               工作站名称：{5}
            //                                               工作站IP：{6}
            //                                               权限：{7}
            //                                            ", LoggedInfo.UserID, LoggedInfo.RoleID, LoggedInfo.UserLoginTime,
            //                                             LoggedInfo.UserName, LoggedInfo.RoleName, LoggedInfo.WorkStationName,
            //                                             LoggedInfo.WorkStationIP, test
            //                                             );
            //            MessageBox.Show(strToShow); 
        }

        /// <summary>
        /// 得到本机IP地址：得到当前上网使用的IP地址
        /// </summary>
        /// <returns></returns>
        private string getLocalIP()
        {
            string loginIP = "";
            System.Net.IPHostEntry ipEntry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress[] addrs = ipEntry.AddressList;
            string strIP = "127.0.0.1";
            for (int i = 0; i < addrs.Count(); i++)
            {
                if (addrs[i].AddressFamily.ToString() == "InterNetwork")
                {
                    strIP = addrs[i].ToString();
                    break;
                }
            }
            loginIP = strIP;

            return loginIP;
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            _countAutoLogout = 0;
            //Process[] vProcesses = Process.GetProcessesByName(this.Name);
            //if (vProcesses.Length <= 0) vProcesses = Process.GetProcessesByName(this.Name + ".vshost"); // 处理调试运行 

            //if (vProcesses.Length > 0)
            //{
            //    SendMessage(vProcesses[0].MainWindowHandle, WM_USER_IPC, 0, 12);    //* IParam = 12作为窗口点击事件
            //}
        }

        private void Window_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            _countAutoLogout = 0;
            // Process[] vProcesses = Process.GetProcessesByName(this.Name);
            //if (vProcesses.Length <= 0) vProcesses = Process.GetProcessesByName(this.Name + ".vshost"); // 处理调试运行 

            //if (vProcesses.Length > 0)
            //{
            //    SendMessage(vProcesses[0].MainWindowHandle, WM_USER_IPC, 0, 12);    //* IParam = 12作为窗口点击事件
            //}
        }

        private void InitWindowBase()
        {
            //设置图标-->地铁Logo
            Icon = new BitmapImage(HHJT_AFC_UI_Functions.ProjectInfo.GetPicUri(ProjectPics.AppIcon));

            //设置启动为止-->屏幕中心位置
            WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;

            //设置窗口状况-->最大化
            WindowState = System.Windows.WindowState.Maximized;

            //初始化系统布局
            _parentPanel = new DockPanel(); //创建根布局DockPanel

            #region 顶部工具栏部分

            //工具栏托盘，以DockPanel实现
            DockPanel toolbarTray = new DockPanel() { LastChildFill = false };
            //设置工具栏背景，动态资源，参见BaseSkin.xaml
            toolbarTray.Style = (Style)Application.Current.Resources["styleBackground"];

            ////透明背景工具栏（可创建多个）
            //ToolBar toolBar = new ToolBar() { Background = Brushes.Transparent, 
            //    HorizontalAlignment = System.Windows.HorizontalAlignment.Right,
            //    HorizontalContentAlignment = System.Windows.HorizontalAlignment.Right
            //};

            //工具栏按钮

            for (int i = 0; i < m_toolButtons.Length; i++)
            {
                //构造Button，并设置如下属性
                Button btn = new Button();
                btn.Style = (Style)Application.Current.Resources["normalButtonStyle"];
                btn.VerticalAlignment = System.Windows.VerticalAlignment.Center;
                btn.Margin = new Thickness(2);
                btn.ToolTip = btnContent[i];
                ////设置Content，包含图片和文字显示，由StackPanel实现
                //StackPanel stackPanel = new StackPanel()
                //{
                //    Orientation = Orientation.Horizontal,
                //    Margin = new Thickness(5, 1, 5, 1)
                //};
                //stackPanel.Children.Add();
                //stackPanel.Children.Add(new Label { Content = btnContent[i] });
                //btn.Content = stackPanel;

                btn.Content = new Image
                {
                    Height = 25,
                    Width = 25,
                    Source = new BitmapImage(HHJT_AFC_Skin_Commons.GetPicUriByIndex(btnPics[i]))
                };

                //设置工具栏按钮响应事件
                btn.Click += new RoutedEventHandler(ToolBarBtn_Click);

                //添加到工具栏
                toolbarTray.Children.Add(btn);
                DockPanel.SetDock(btn, Dock.Right);
                m_toolButtons[i] = btn;

            }

            lblTitle = new Label
            {
                FontSize = 18,
                Foreground = Brushes.DarkBlue,
                Margin = new Thickness(5, 0, 0, 0),
                //Content = "账务管子系统"
            };
            toolbarTray.Children.Add(lblTitle);
            DockPanel.SetDock(lblTitle, Dock.Left);

            //添加工具栏托盘到主布局DockPanel，并设置停靠方式
            _parentPanel.Children.Add(toolbarTray);
            DockPanel.SetDock(toolbarTray, Dock.Top);




            #endregion

            #region 底部状态栏部分
            //构造状态栏
            //StatusBar statusBar = new StatusBar()
            //{
            //    HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch,
            //    VerticalAlignment = System.Windows.VerticalAlignment.Bottom
            //};
            ////设置状态栏背景，动态资源，参见BaseSkin.xaml
            //statusBar.Style = (Style)Application.Current.Resources["styleBackgroundRev"];

            Grid gridStatus = new Grid();
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
           
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gridStatus.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });

            gridStatus.Style = (Style)Application.Current.Resources["styleBackgroundRev"];

            _labelTips = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                FontSize = 13,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.DarkBlue,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray,
                Margin = new Thickness(3, 0, 0, 0),
                Content = "Ready"
            };
            gridStatus.Children.Add(_labelTips);
            Grid.SetColumn(_labelTips, 0);

            //版本
            _labelVersion = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                FontSize = 13,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.DarkBlue,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray,
                Margin = new Thickness(3, 0, 0, 0)

            };
            TextBlock textblock = new TextBlock() { VerticalAlignment = System.Windows.VerticalAlignment.Center };
            textblock.Text = "软件版本：";
            textblock.Margin = new Thickness(10, 0, 0, 0);
            gridStatus.Children.Add(textblock);
            Grid.SetColumn(textblock, 2);

            gridStatus.Children.Add(_labelVersion);
            Grid.SetColumn(_labelVersion, 3);
              


            //时间
            _labelTime = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                HorizontalContentAlignment = System.Windows.HorizontalAlignment.Center,
                Width = 200,
                FontSize = 13,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.DarkBlue,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray,
                Margin = new Thickness(10, 0, 0, 0)
            };
            gridStatus.Children.Add(_labelTime);
            Grid.SetColumn(_labelTime, 10);
            //终端名称
            _labelPCName = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                Width = 100,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray,
                Content = System.Environment.MachineName
            };

            textblock = new TextBlock() { VerticalAlignment = System.Windows.VerticalAlignment.Center };
            textblock.Text = "终端名称：";
            textblock.Margin = new Thickness(10, 0, 0, 0);
            gridStatus.Children.Add(textblock);
            Grid.SetColumn(textblock, 8);
            gridStatus.Children.Add(_labelPCName);
            Grid.SetColumn(_labelPCName, 9);

            //登录用户
            this._labelUserName = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                Width = 100,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray
            };
            textblock = new TextBlock() { Text = "登录用户：", VerticalAlignment = System.Windows.VerticalAlignment.Center };
            textblock.Margin = new Thickness(10, 0, 0, 0);
            gridStatus.Children.Add(textblock);
            Grid.SetColumn(textblock, 6);
            gridStatus.Children.Add(_labelUserName);
            Grid.SetColumn(_labelUserName, 7);

            //报表生成时间
            this._labelReportTime = new Label()
            {
                VerticalContentAlignment = System.Windows.VerticalAlignment.Center,
                Width = 200,
                BorderThickness = new Thickness(1),
                BorderBrush = Brushes.DarkGray
            };
            textblock = new TextBlock() { Text = "报表生成时间：", VerticalAlignment = System.Windows.VerticalAlignment.Center };
            textblock.Margin = new Thickness(10, 0, 0, 0);
            gridStatus.Children.Add(textblock);
            Grid.SetColumn(textblock, 4);
            gridStatus.Children.Add(_labelReportTime);
            Grid.SetColumn(_labelReportTime, 5);

            _parentPanel.Children.Add(gridStatus);

            DockPanel.SetDock(gridStatus, Dock.Bottom);
            #endregion

            //用户区域
            m_userContent = new Border() { BorderBrush = Brushes.LightGray, BorderThickness = new Thickness(2), Padding = new Thickness(0, 3, 0, 0) };
            _parentPanel.Children.Add(m_userContent);

            _rootGrid = new Grid();
            _rootGrid.Children.Add(_parentPanel);
            this.Content = _rootGrid;

            InitMainContent();

            //
            _timer = new DispatcherTimer();
            _timer.Tick += Timer_Tick;
            _timer.Interval = TimeSpan.FromSeconds(1);
            _timer.Start();
        }

        void Timer_Tick(object sender, EventArgs e)
        {
            //throw new NotImplementedException();
            _labelTime.Content = DateTime.Now.ToString("yyyy-MM-dd dddd HH:mm:ss");

            //自动签退部分
            _countAutoLogout++;
            if (_countAutoLogout > _maxSecondsAutoLogout && _flagLoggedIn)
            {
                AutoLogOut();
            }
        }

        protected virtual void AutoLogOut()
        {

        }

        void ToolBarBtn_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            if (btn == m_toolButtons[2])//打印屏幕
            {
                #region 打印屏幕

                PrintDialog printDialog = new PrintDialog();
                if (printDialog.ShowDialog() == true)
                {//120,96
                    RenderTargetBitmap bmp = new RenderTargetBitmap((int)this.ActualWidth, (int)this.ActualHeight, 96, 96, PixelFormats.Default);
                    bmp.Render(this);
                    Image myImage = new Image();

                    myImage.Source = bmp;


                    myImage.LayoutTransform = new RotateTransform(90);
                    // Define a margin.
                    int pageMargin = 1;
                    // Get the size of the page.
                    Size pageSize = new Size(printDialog.PrintableAreaWidth - pageMargin * 2,
                    printDialog.PrintableAreaHeight - 4);
                    // Trigger the sizing of the element.
                    myImage.Measure(pageSize);
                    myImage.Arrange(new Rect(pageMargin, pageMargin,
                    pageSize.Width, pageSize.Height));
                    printDialog.PrintVisual(myImage, "屏幕打印" + DateTime.Now.ToString("yyyyMMdd"));
                }
                #endregion
            }
            else if (btn == m_toolButtons[0])//返回
            {
                //MessageBox.Show("back");
                // Application.Current.Shutdown();
                Shutdown();
                _logoutbutton = true;
                LogOut();
            }
            //else if (btn == m_toolButtons[2])//帮助
            //{
            //    MessageBox.Show("help");

            //}
            else if (btn == m_toolButtons[1])//关于
            {
                //MessageBox.Show("about");
                ShowAbout();
            }
            else if (btn == m_toolButtons[3])//修改密码
            {
                ModifyPasswd();
            }else if (btn == m_toolButtons[4]) //更新代码
            {
                UpdateSoftware();
            }
            else if (btn == m_toolButtons[5]) //关机代码
            {
                Shutdown();
            }
        }

        protected virtual void Shutdown()
        {
            
        }

        protected virtual void ShowAbout()
        {

        }

        protected virtual void LogOut()
        {
           
        }

        protected virtual void ModifyPasswd()
        {

        }

        protected virtual void UpdateSoftware()
        {

        }

        /// <summary>
        /// 以用户自定义控件替代用户区域
        /// </summary>
        /// <param name="uc"></param>
        public void SetUserContent(UserControl uc)
        {
            m_userContent.Child = uc;
        }

        /// <summary>
        /// 初始化主用户区域，由派生类自定义完成
        /// </summary>
        protected virtual void InitMainContent() { }
        /// <summary>
        /// 初始化子系统信息，由派生类自定义完成
        /// </summary>
        protected virtual void InitSubsystem() { }

        public void SetTips(string strTips)
        {
            //throw new NotImplementedException();
            this._labelTips.Content = strTips;
        }
    }

    public class AboutBase : Window
    {
        public string SoftWareTitle { set { _lblTitle.Content = value; } }

        private Label _lblTitle;

        public AboutBase()
        {
            //窗口属性
            this.Title = "关于...";
            this.WindowStyle = System.Windows.WindowStyle.ToolWindow;
            this.WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            this.Height = 380;
            this.Width = 430;

            //根布局Grid
            Grid panelRoot = new Grid();

            panelRoot.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Auto) });
            panelRoot.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Auto) });
            panelRoot.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Star) });
            panelRoot.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Auto) });
            panelRoot.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Auto) });

            panelRoot.Effect = new DropShadowEffect();

            //背景效果
            LinearGradientBrush brush = new LinearGradientBrush();
            brush.StartPoint = new Point(0, 0);
            brush.EndPoint = new Point(1, 1);
            brush.GradientStops.Add(new GradientStop { Color = Colors.LightBlue, Offset = 0 });
            brush.GradientStops.Add(new GradientStop { Color = Colors.Azure, Offset = 0.8 });
            brush.GradientStops.Add(new GradientStop { Color = Colors.White, Offset = 1 });
            panelRoot.Background = brush;

            //标题
            StackPanel panelStack = new StackPanel() { Orientation = Orientation.Horizontal, Margin = new Thickness(3), HorizontalAlignment = System.Windows.HorizontalAlignment.Center };
            panelStack.Children.Add(new Image { Width = 40, Height = 40, Source = new BitmapImage(new Uri(@"pack://application:,,,/HHJT.AFC.Framework.Skin;component/Pics/huahong.png")) });
            _lblTitle = new Label { FontSize = 18, VerticalContentAlignment = System.Windows.VerticalAlignment.Center, Foreground = Brushes.Navy };
            panelStack.Children.Add(_lblTitle);

            panelRoot.Children.Add(panelStack);
            Grid.SetRow(panelStack, 0);

            Border border = new Border { BorderThickness = new Thickness(2), BorderBrush = Brushes.Teal, Margin = new Thickness(0, 3, 0, 3) };
            panelRoot.Children.Add(border);
            Grid.SetRow(border, 1);

            border = new Border { BorderThickness = new Thickness(2), BorderBrush = Brushes.Teal, Margin = new Thickness(0, 3, 0, 3) };
            panelRoot.Children.Add(border);
            Grid.SetRow(border, 3);

            Label lbl = new Label { VerticalContentAlignment = System.Windows.VerticalAlignment.Center, HorizontalAlignment = System.Windows.HorizontalAlignment.Left, Content = "(C)2015 HHJT", Foreground = Brushes.DarkBlue };
            panelRoot.Children.Add(lbl);
            Grid.SetRow(lbl, 4);
            Button btn = new Button() { Content = "确认", Margin = new Thickness(3), Width = 80, HorizontalAlignment = System.Windows.HorizontalAlignment.Right };
            btn.Click += new RoutedEventHandler(Btn_Click);
            panelRoot.Children.Add(btn);
            Grid.SetRow(btn, 4);

            FlowDocumentScrollViewer viewer = new FlowDocumentScrollViewer() { Foreground = Brushes.Navy };
            viewer.VerticalScrollBarVisibility = ScrollBarVisibility.Disabled;
            viewer.HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled;

            viewer.Document = GetDocument();
            panelRoot.Children.Add(viewer);
            Grid.SetRow(viewer, 2);

            this.Content = panelRoot;
        }

        void Btn_Click(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            this.Close();
        }

        public FlowDocument GetDocument()
        {
            FlowDocument doc = new FlowDocument() { FontSize = 14 };
            doc.Blocks.Add(new Paragraph(new Run("程序信息：") { FontSize = 18 }));
            List lst = new List();
            lst.ListItems.Add(new ListItem(new Paragraph(new Run(string.Format("版本：{0}", GetFileVersion())))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("开发：上海华虹计通智能系统股份有限公司"))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("地址：上海市浦东新区锦绣东路2777弄9号11号楼"))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("邮编：201206"))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("电话：021-31016900"))));
            doc.Blocks.Add(lst);
            return doc;
        }

        public virtual string GetFileVersion() { return "1.0.0.0"; }
    }
}