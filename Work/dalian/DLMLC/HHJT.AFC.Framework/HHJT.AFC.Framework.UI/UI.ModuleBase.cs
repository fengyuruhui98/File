using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace HHJT.AFC.Framework.UI
{
    public class SubSysInfo
    {
        public string SubSysName { get; set; }
        public Type SubSysView { get; set; }
        public UserControl _subSysView;
        public UserControl GetSubsysView()
        {
            if (_subSysView == null)
            {
                _subSysView = (UserControl)System.Activator.CreateInstance(SubSysView);
            }


            return _subSysView;
        }
    }

    public interface IModuleView
    {
        // void UIInit();
    }

    public class ModuleViewInfo
    {
        private UserControl _moduleView = null;

        public UserControl ModuleView
        {
            get { return _moduleView; }
            set { _moduleView = value; }
        }

        public string ModuleName { get; set; }
        public Type ModuleViewType { get; set; }
        public UserControl GetModuleView()
        {
            if (_moduleView == null)
            {
                _moduleView = (UserControl)System.Activator.CreateInstance(ModuleViewType);
            }
            if (_moduleView is IModuleView)
            {
                _moduleView = (UserControl)System.Activator.CreateInstance(ModuleViewType);

            }
            return _moduleView;
        }
    }

    #region 第2层 子模块
    public class UCSubsysLayout : UserControl
    {
        protected WrapPanel m_panelModules;
        protected WrapPanel m_panelInfo;
        protected Grid m_gridMainContent;
        protected Collection<ModuleViewInfo> _moduleViews = new Collection<ModuleViewInfo>();

        public UCSubsysLayout()
        {
            DockPanel dockPanel = new DockPanel();
            m_panelModules = new WrapPanel();


            dockPanel.Children.Add(m_panelModules);
            DockPanel.SetDock(m_panelModules, Dock.Top);

            Border borderDecorate = new Border();
            borderDecorate.BorderThickness = new Thickness(1);
            borderDecorate.BorderBrush = Brushes.DarkBlue;
            borderDecorate.Margin = new Thickness(-2, 4, -2, 4);

            dockPanel.Children.Add(borderDecorate);
            DockPanel.SetDock(borderDecorate, Dock.Top);

            m_panelInfo = new WrapPanel();
            dockPanel.Children.Add(m_panelInfo);
            DockPanel.SetDock(m_panelInfo, Dock.Top);

            m_gridMainContent = new Grid();
            dockPanel.Children.Add(m_gridMainContent);

            RegisterModules();

            foreach (var item in _moduleViews)
            {
                RadioButton tBtn = new RadioButton()
                {
                    Tag = item,
                    Template = (ControlTemplate)Application.Current.Resources["ControlTemplateRadioButtonModule"]
                };
                tBtn.DataContext = item;

                tBtn.SetBinding(Button.ContentProperty, "ModuleName");

                tBtn.Checked += new RoutedEventHandler(tFunctionBtn_Checked);
                m_panelModules.Children.Add(tBtn);
            }

            this.Content = dockPanel;
        }

        protected virtual void RegisterModules()
        {


        }
        void tFunctionBtn_Checked(object sender, RoutedEventArgs e)
        {
            m_gridMainContent.Children.Clear();
            m_gridMainContent.Children.Add(
                ((sender as Control).Tag as ModuleViewInfo).GetModuleView()
                );
        }

    }
    #endregion

    /// <summary>
    /// 功能区域
    /// </summary>
    public class UCModule : UserControl
    {
        // public WrapPanel panelRoot;
        //public ListBox lbxFunctionlist; //---------->替换为RadioButton
        //public DockPanel panelRoot;

        public StackPanel panelFunction;

        protected List<FunctionList> _funcLists = new List<FunctionList>();

        public List<FunctionList> FuncLists
        {
            get { return _funcLists; }
            //set { _funcLists = value; }
        }

        protected FunctionList _currentFunction = null;
        protected Grid gd = new Grid();
        protected Grid gdMainContent = new Grid();
        public UCModule()
            : base()
        {
            Foreground = Brushes.Black;
            InitControls();
        }
        protected void InitControls()
        {
            //功能区域
            GroupBox groupboxFunctionList = new GroupBox() { Header = "功能区域" };
            //lbxFunctionlist = new ListBox(); //{ Width = 180 };
            //lbxFunctionlist.Style = (Style)Application.Current.Resources["listboxLSK"];
            //lbxFunctionlist.ItemContainerStyle = (Style)Application.Current.Resources["listboxItemLSK"];

            //lbxFunctionlist.SelectionChanged += new SelectionChangedEventHandler(lbxFunctionlist_SelectionChanged);

            //lbxFunctionlist.ItemsSource = _funcLists;


            //groupboxFunctionList.Content = lbxFunctionlist;

            //PanelRoot.Children.Add(groupboxFunctionList);
            //DockPanel.SetDock(groupboxFunctionList, Dock.Left);

            panelFunction = new StackPanel();
            groupboxFunctionList.Content = panelFunction;

            //主面板分3块 功能区域-缩放框-查询数据操作区域
            //gd = new Grid();
            gd.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto), MinWidth = 160 });
            gd.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            gd.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            gd.Children.Add(groupboxFunctionList);
            Grid.SetColumn(groupboxFunctionList, 0);

            GridSplitter gridSplittle = new GridSplitter()
            {
                Width = 2,
                HorizontalAlignment = System.Windows.HorizontalAlignment.Center,
                VerticalAlignment = System.Windows.VerticalAlignment.Stretch
            };
            gd.Children.Add(gridSplittle);
            Grid.SetColumn(gridSplittle, 1);


            FurtherInitLayout();//由派生类自行定义完成
            //Border borderQuerySplit = new Border()
            //{
            //    Margin = new Thickness(5, -5, 5, -5),
            //    BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
            //    BorderThickness = new Thickness(0.5)
            //};
            //Grid.SetColumn(borderQuerySplit,2);
            gd.Children.Add(gdMainContent);
            Grid.SetColumn(gdMainContent, 2);

            this.Content = gd;

        }

        // protected void FunctionSelectionChanged(FunctionListInfo rInfo) { }

        void btn_Click(object sender, RoutedEventArgs e)
        {
            btnClicked((sender as Button).Tag.ToString());
        }

        protected void btnClicked(string strBtnName) { }

        protected virtual void FurtherInitLayout() { }

        protected void RegisterFunctions()
        {
            foreach (var item in _funcLists)
            {
                RadioButton tBtn = new RadioButton()
                {
                    Tag = item,
                    Template = (ControlTemplate)Application.Current.Resources["ControlTemplateRadioButtonFunction"]
                };

                tBtn.SetBinding(Button.ContentProperty, "FunctionName");
                tBtn.SetBinding(Button.IsEnabledProperty, "IsFunctionEnabled");
                tBtn.DataContext = item;

                tBtn.Checked += tBtn_Checked;
                panelFunction.Children.Add(tBtn);
            }

        }

        void tBtn_Checked(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();

            _currentFunction = (sender as RadioButton).Tag as FunctionList;
            gdMainContent.Children.Clear();
            gdMainContent.Children.Add(_currentFunction.GetViewObject());
            // FunctionSelectionChanged(lbx.SelectedItem as FunctionListInfo);

        }
    }

}
