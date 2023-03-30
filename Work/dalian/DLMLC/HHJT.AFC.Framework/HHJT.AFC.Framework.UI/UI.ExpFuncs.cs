using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using System.Windows.Media.Effects;
using System.Windows.Shapes;
namespace HHJT.AFC.Framework.UI
{
    public class ExpModuleInfo
    {
        public string ModuleName { get; set; }
        private List<ExpModuleFuncsInfo> _funcs = new List<ExpModuleFuncsInfo>();

        public List<ExpModuleFuncsInfo> Funcs
        {
            get { return _funcs; }
            set { _funcs = value; }
        }
    }

    public class ExpModuleFuncsInfo
    {
        public string FuncsName { get; set; }
        public Type FunctionViewType { get; set; }

        private UserControl _view;

        public UserControl CreateView()
        {
            if (_view == null)
                _view = (UserControl)System.Activator.CreateInstance(FunctionViewType);
            return _view;
        }

        public override string ToString()
        {
            return FuncsName;
        }
    }

    public abstract class ExpFuncsUIBase : UserControl
    {
        protected List<ExpModuleInfo> _modules = new List<ExpModuleInfo>();

        private Border paneLeft;
        private Border pane;
        private GridSplitter paneSplit;
        private Grid grid;
        protected Grid panelUI; //操作区域
        private ToggleButton tgb;
        protected StackPanel panelFuncsGroup; //功能列表
        private Label lblWelcome;
        public ExpFuncsUIBase()
        {
            RegisterModules();

            //根布局
            DockPanel panelRoot = new DockPanel();

            //收缩状态布局
            paneLeft = new Border() { BorderThickness = new Thickness(0, 1, 1, 1), BorderBrush = Brushes.Gray, Margin = new Thickness(0, 1, 1, 1) };
            DockPanel tempDock = new DockPanel();
            Button btnShowPanel = new Button() { Height = 20, Width = 20, Margin = new Thickness(3), BorderBrush = Brushes.Black };
            btnShowPanel.Style = (Style)Application.Current.Resources["buttonStyle"];
            Path p = new Path() { Stroke = (Brush)Application.Current.Resources["ImageBrush"], Fill = Brushes.Transparent };
            p.Data = (Geometry)System.ComponentModel.TypeDescriptor.GetConverter(typeof(Geometry)).ConvertFromInvariantString("M0,0 L3,3 0,6 M3,0 L6,3 3,6");
            btnShowPanel.Content = p;
            btnShowPanel.Click += BtnShowPanel_Click;
            tempDock.Children.Add(btnShowPanel);
            DockPanel.SetDock(btnShowPanel, Dock.Top);

            Grid tempGrid = new Grid();
            tgb = new ToggleButton
            {
                Style = (Style)Application.Current.Resources["verticalToggleButtonStyle"],
                Content = "功能列表"
            };
            tempGrid.Children.Add(tgb);

            tempDock.Children.Add(tempGrid);
            paneLeft.Child = tempDock;

            panelRoot.Children.Add(paneLeft);
            DockPanel.SetDock(paneLeft, Dock.Left);
            //end 收缩状态布局

            Border tempBorder = new Border() { BorderThickness = new Thickness(1), BorderBrush = Brushes.Gray, Margin = new Thickness(1) };
            tempBorder.Style = (Style)Application.Current.Resources["styleBackgroundRev"];
            grid = new Grid();
            grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //功能列表
            pane = new Border()
            {
                BorderBrush = Brushes.Gray,
                Background = Brushes.MintCream,
                Margin = new Thickness(0, 0, 3, 0),
                HorizontalAlignment = HorizontalAlignment.Stretch,
                Visibility = System.Windows.Visibility.Collapsed,
                VerticalAlignment = System.Windows.VerticalAlignment.Stretch
            };

            DockPanel tempDock1 = new DockPanel() { HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch };

            DockPanel tempDock2 = new DockPanel() { };
            Button btnHidePanel = new Button() { Height = 20, Width = 20, Margin = new Thickness(3), Style = (Style)Application.Current.Resources["buttonStyle"] };
            p = new Path() { Stroke = (Brush)Application.Current.Resources["ImageBrush"], Fill = Brushes.Transparent };
            p.Data = (Geometry)System.ComponentModel.TypeDescriptor.GetConverter(typeof(Geometry)).ConvertFromInvariantString("M3,0 L0,3 3,6 M6,0 L3,3 6,6");
            btnHidePanel.Content = p;
            btnHidePanel.Click += BtnHidePanel_Click;
            tempDock2.Children.Add(btnHidePanel);
            DockPanel.SetDock(btnHidePanel, Dock.Right);
            Label lbl = new Label() { Content = "功能列表" };
            tempDock2.Children.Add(lbl);
            tempDock1.Children.Add(tempDock2);
            DockPanel.SetDock(tempDock2, Dock.Top);
            ScrollViewer sv = new ScrollViewer();
            panelFuncsGroup = new StackPanel();
            sv.HorizontalScrollBarVisibility = ScrollBarVisibility.Auto;
            sv.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
            panelFuncsGroup.MinWidth = 150;
            sv.Content = panelFuncsGroup;

            tempDock1.Children.Add(sv);
            pane.Child = tempDock1;
            grid.Children.Add(pane);
            Grid.SetColumn(pane, 0);

            paneSplit = new GridSplitter()
            {
                Width = 4,
                HorizontalAlignment = System.Windows.HorizontalAlignment.Right,
                BorderBrush = Brushes.Gray,
                BorderThickness = new Thickness(1),
                VerticalAlignment = System.Windows.VerticalAlignment.Stretch,
                Visibility = System.Windows.Visibility.Collapsed,
                Background = Brushes.White
            };
            grid.Children.Add(paneSplit);
            Grid.SetColumn(paneSplit, 0);

            panelUI = new Grid();
            lblWelcome = new Label()
            {
                Content = "欢迎使用，请选择功能项目。",
                Foreground = (Brush)Application.Current.Resources["txtForeground"],
                FontSize = 38,
                FontWeight = FontWeights.Bold,
                VerticalAlignment = System.Windows.VerticalAlignment.Center,
                HorizontalAlignment = System.Windows.HorizontalAlignment.Center
            };
            //lblWelcome.BitmapEffect = new DropShadowBitmapEffect();
            lblWelcome.Effect = new DropShadowEffect();
            panelUI.Children.Add(lblWelcome);
            grid.Children.Add(panelUI);
            Grid.SetColumn(panelUI, 1);

            tempBorder.Child = grid;
            panelRoot.Children.Add(tempBorder);
            //功能列表

            this.Content = panelRoot;

            InitFuncsList();
            pane.Visibility = Visibility.Visible;
            paneSplit.Visibility = Visibility.Visible;
            paneLeft.Visibility = Visibility.Collapsed;
        }

        void BtnHidePanel_Click(object sender, RoutedEventArgs e)
        {
            pane.Visibility = Visibility.Collapsed;
            paneSplit.Visibility = Visibility.Collapsed;
            paneLeft.Visibility = Visibility.Visible;

            grid.ColumnDefinitions[0].Width = GridLength.Auto;
        }

        void BtnShowPanel_Click(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            pane.Visibility = Visibility.Visible;
            paneSplit.Visibility = Visibility.Visible;
            paneLeft.Visibility = Visibility.Collapsed;
        }

        void InitFuncsList()
        {
            foreach (var item in _modules)
            {
                Expander exp = new Expander() { Header = item.ModuleName, IsExpanded = false, Margin = new Thickness(1, 3, 3, 1) };
                ListBox lbx = new ListBox
                {
                    Style = (Style)Application.Current.Resources["listboxLSK"],
                    ItemContainerStyle = (Style)Application.Current.Resources["listboxItemLSK"]
                };
                lbx.SelectionChanged += Lbx_SelectionChanged;
                lbx.ItemsSource = item.Funcs;
                exp.Content = lbx;
                exp.Expanded += Exp_Expanded;
                panelFuncsGroup.Children.Add(exp);
            }
        }

        void Exp_Expanded(object sender, RoutedEventArgs e)
        {
            foreach (Expander exp in panelFuncsGroup.Children)
            {
                if (exp != sender)
                {
                    exp.IsExpanded = false;
                }

                (exp.Content as ListBox).SelectedIndex = -1;
            }

            panelUI.Children.Clear();
            panelUI.Children.Add(lblWelcome);
        }

        void Lbx_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((sender as ListBox).SelectedItem != null)
            {
                panelUI.Children.Clear();

                panelUI.Children.Add(((sender as ListBox).SelectedItem as ExpModuleFuncsInfo).CreateView());
            }
        }

        protected abstract void RegisterModules();
    }
}