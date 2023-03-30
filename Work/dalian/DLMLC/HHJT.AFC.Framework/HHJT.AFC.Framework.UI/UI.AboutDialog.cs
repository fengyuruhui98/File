using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using HHJT.AFC.Framework.Skin;

namespace HHJT.AFC.Framework.UI
{
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
            panelStack.Children.Add(new Image { Width = 40, Height = 40, Source = new BitmapImage(HHJT_AFC_Skin_Commons.GetPicUriByIndex(HHJT_PICS.HHJT)) });
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


            Label lbl = new Label { VerticalContentAlignment = System.Windows.VerticalAlignment.Center, HorizontalAlignment = System.Windows.HorizontalAlignment.Left, Content = "(C)2014 HHJT", Foreground = Brushes.DarkBlue };
            panelRoot.Children.Add(lbl);
            Grid.SetRow(lbl, 4);
            Button btn = new Button() { Content = "确认", Margin = new Thickness(3), Width = 80, HorizontalAlignment = System.Windows.HorizontalAlignment.Right };
            btn.Click += new RoutedEventHandler(btn_Click);
            panelRoot.Children.Add(btn);
            Grid.SetRow(btn, 4);

            FlowDocumentScrollViewer viewer = new FlowDocumentScrollViewer() { Foreground = Brushes.Navy };
            viewer.Document = GetDocument();
            panelRoot.Children.Add(viewer);
            Grid.SetRow(viewer, 2);

            this.Content = panelRoot;
        }

        void btn_Click(object sender, RoutedEventArgs e)
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
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("地址：上海市中山西路1291号"))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("邮编：200051"))));
            lst.ListItems.Add(new ListItem(new Paragraph(new Run("电话：021-32090258"))));
            doc.Blocks.Add(lst);
            return doc;
        }

        public virtual string GetFileVersion() { return "1.0.0.0"; }
    }
}
