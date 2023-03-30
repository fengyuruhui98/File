using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Xps.Packaging;

namespace HHJT.AFC.Framework.UI
{
    public class HHJT_AFC_UI_Dialog : Window
    {

        public Label LabelTitle;
        public DockPanel PanelMainContent;
        public Button BtnCancel;
        public Grid GridBottom;

        public List<AttrMethodAttribute> props;

        public HHJT_AFC_UI_Dialog()
        {


            //Title
            #region
            Grid gridTitle = new Grid();
            LinearGradientBrush lineBrush = new LinearGradientBrush();
            lineBrush.StartPoint = new Point(0.95, 1);
            lineBrush.EndPoint = new Point(1, 0);
            lineBrush.GradientStops.Add(new GradientStop(Colors.White, 0.0));
            lineBrush.GradientStops.Add(new GradientStop(Colors.LightGoldenrodYellow, 1.0));
            gridTitle.Background = lineBrush;

            Image image = new Image()
            {
                HorizontalAlignment = System.Windows.HorizontalAlignment.Right,
                Margin = new Thickness(5),
                Height = 60,
                Width = 60,//(@"pack://application:,,,/HHJT.Resource;component/Pics/expologo_txt.png"
                Source = HHJT_AFC_UI_Functions.GetProjectImageSource()
            };
            gridTitle.Children.Add(image);

            LabelTitle = new Label()
            {
                Margin = new Thickness(5),
                Foreground = new SolidColorBrush(Colors.Teal),
                FontSize = 14,
                FontWeight = (FontWeight)new FontWeightConverter().ConvertFromString("Bold")
            };
            gridTitle.Children.Add(LabelTitle);
            #endregion

            //Bottom buttons
            #region
            GridBottom = new Grid() { Height = 30 };
            GridBottom.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            for (int i = 0; i < 5; i++)
            {
                GridBottom.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });

            }
            GridBottom.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(10, GridUnitType.Pixel) });

            BtnCancel = new Button() { Style = (Style)Application.Current.Resources["styleBtnDialog"], Content = "取消(_C)" };
            BtnCancel.IsCancel = true;
            BtnCancel.Click += new RoutedEventHandler(btnCancel_Click);
            GridBottom.Children.Add(BtnCancel);
            Grid.SetColumn(BtnCancel, 5);
            #endregion

            DockPanel rootPanel = new DockPanel();
            rootPanel.Children.Add(gridTitle);
            DockPanel.SetDock(gridTitle, Dock.Top);

            Border borderDecorate = new Border()
            {
                Margin = new Thickness(-1, 0, -1, 2),
                BorderThickness = new Thickness(1),
                BorderBrush = new SolidColorBrush(Colors.Teal)
            };
            rootPanel.Children.Add(borderDecorate);
            DockPanel.SetDock(borderDecorate, Dock.Top);

            rootPanel.Children.Add(GridBottom);
            DockPanel.SetDock(GridBottom, Dock.Bottom);

            borderDecorate = new Border()
            {
                Margin = new Thickness(-1, 2, -1, 0),
                BorderThickness = new Thickness(1),
                BorderBrush = new SolidColorBrush(Colors.Teal)
            };
            rootPanel.Children.Add(borderDecorate);
            DockPanel.SetDock(borderDecorate, Dock.Bottom);

            PanelMainContent = new DockPanel();
            rootPanel.Children.Add(PanelMainContent);

            this.MinHeight = 400;
            this.MinWidth = 500;
            this.ShowInTaskbar = false;
            this.WindowStyle = System.Windows.WindowStyle.ToolWindow;
            //this.SizeToContent = System.Windows.SizeToContent.WidthAndHeight;
            this.WindowStartupLocation = System.Windows.WindowStartupLocation.CenterOwner;
            //this.Icon = new ImageSourceConverter().ConvertFromString(@"pack://application:,,,/HHJT.Resource;component/Pics/expologo_single.png") as ImageSource; //invalid in a toolwindow
            this.Content = rootPanel;
        }

        void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();//throw new NotImplementedException();
        }

        //List<Button> userRegisteredButtons = new List<Button>();
        int buttonCount = 4;
        protected List<Button> _userButtons = new List<Button>();
        protected void RegisterFunction(string strFuncName, string strAccelerateKey)
        {//禁止使用加速键C【Cancel】

            Button btn = new Button();
            _userButtons.Add(btn);
            btn.Style = (Style)Application.Current.Resources["styleBtnDialog"];
            btn.Content = string.Format("{0}(_{1})", strFuncName, strAccelerateKey);
            btn.Click += new RoutedEventHandler(func_Click);
            btn.Tag = strAccelerateKey;
            GridBottom.Children.Add(btn);
            Grid.SetColumn(btn, buttonCount--);

            this.SizeToContent = System.Windows.SizeToContent.WidthAndHeight;

        }
        void func_Click(object sender, RoutedEventArgs e)
        {
            funcRegistered((sender as Button).Tag.ToString());
        }

        protected virtual void funcRegistered(string strAccelerateKey)
        { }
        /****
        * 
        * 打印dialog    liuw 20120405
        * 
        **/
        protected void PrintFunction(string strFuncName, string strAccelerateKey)
        {//禁止使用加速键C【Cancel】

            Button printBtn = new Button();
            printBtn.Style = (Style)Application.Current.Resources["styleBtnDialog"];
            printBtn.Content = string.Format("{0}(_{1})", strFuncName, strAccelerateKey);
            printBtn.Click += new RoutedEventHandler(printButton_Click);
            printBtn.Tag = strAccelerateKey;
            GridBottom.Children.Add(printBtn);
            Grid.SetColumn(printBtn, buttonCount--);
        }
        void printButton_Click(object sender, RoutedEventArgs e)
        {
            PrintDialog pDialog = new PrintDialog();
            pDialog.PageRangeSelection = PageRangeSelection.AllPages;
            pDialog.UserPageRangeEnabled = true;
            Nullable<bool> result = pDialog.ShowDialog();
            if (result == true)
            {

                XpsDocument xpsDocument = new XpsDocument("C:\\FixedDocumentSequence.xps", FileAccess.ReadWrite);
                FixedDocumentSequence fixedDocSeq = xpsDocument.GetFixedDocumentSequence();
                //pDialog.PrintDocument(fixedDocSeq.DocumentPaginator, "Test print job");

                //// Print document
            }


        }
    }
}
