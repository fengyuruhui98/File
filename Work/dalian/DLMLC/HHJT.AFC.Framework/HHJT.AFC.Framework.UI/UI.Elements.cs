using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;

namespace HHJT.AFC.Framework.UI
{
    public interface IIsChecked
    {
        bool IsChecked { get; set; }
    }
    public class ParamDataGrid : GroupDataGrid
    {
        public event EventHandler EventAdd;
        public event EventHandler EventDelete;

        private ContextMenu _contextMenu;
        public ContextMenu ContextMenu1
        {
            get { return _contextMenu; }
            set { _contextMenu = value; }
        }

        public ParamDataGrid()
        {
            //this.ContextMenu = new ContextMenu();
            _contextMenu = new System.Windows.Controls.ContextMenu();

            MenuItem menuItemAdd = new MenuItem() { Header = "添加" };
            menuItemAdd.Click += new RoutedEventHandler(menuItemAdd_Click);
            _contextMenu.Items.Add(menuItemAdd);

            MenuItem menuItemDelete = new MenuItem() { Header = "删除" };
            menuItemDelete.Click += new RoutedEventHandler(menuItemDelete_Click);
            _contextMenu.Items.Add(menuItemDelete);

            // this.ContextMenu = _contextMenu;
        }

        public void ShowContextMenu(bool show)
        {
            if (show)
            {
                this.ContextMenu = _contextMenu;
            }
            else
            {

                this.ContextMenu = null;
            }
        }

        void menuItemDelete_Click(object sender, RoutedEventArgs e)
        {
            if (this.CurrentItem != null)
            {
                if (MessageBox.Show("确认删除") == MessageBoxResult.OK)
                {
                    if (EventDelete != null)
                    {
                        EventDelete(this, e);
                    }
                    ICollectionView view = (ICollectionView)CollectionViewSource.GetDefaultView(this.ItemsSource);
                    IList items = (IList)view.SourceCollection;
                    items.Remove(this.CurrentItem);

                    this.Items.Refresh();
                }
            }
        }

        void menuItemAdd_Click(object sender, RoutedEventArgs e)
        {
            if (EventAdd != null)
            {
                EventAdd(this, e);
            }
        }
    }

    public class GroupDataGrid : DataGrid
    {
        public GroupDataGrid()
            : base()
        {
            Init();
            this.LoadingRow += new EventHandler<DataGridRowEventArgs>(GroupDataGrid_LoadingRow);
        }

        void GroupDataGrid_LoadingRow(object sender, DataGridRowEventArgs e)
        {
            //throw new NotImplementedException();
            e.Row.Header = e.Row.GetIndex() + 1;
        }

        public void Init()
        {
            SelectionMode = DataGridSelectionMode.Single;
            AutoGenerateColumns = false;
            //IsReadOnly = true,
            CanUserAddRows = false;
            CanUserDeleteRows = false;
            //CanUserReorderColumns = true,
            CanUserResizeColumns = true;
            CanUserResizeRows = true;
            CanUserSortColumns = true;
            VerticalContentAlignment = System.Windows.VerticalAlignment.Center;
            AlternatingRowBackground = new SolidColorBrush(Colors.LightGoldenrodYellow);
            CellStyle = (Style)(Style)Application.Current.Resources["DataGridVcenterHleft"];
        }

        public void SetGroupStyle()
        {
            FrameworkElementFactory datagridRowsPresenter = new FrameworkElementFactory(typeof(DataGridRowsPresenter));
            ItemsPanelTemplate itemsPanelTemplate = new ItemsPanelTemplate();
            itemsPanelTemplate.VisualTree = datagridRowsPresenter;
            GroupStyle groupStyle = new GroupStyle();
            groupStyle.Panel = itemsPanelTemplate;
            groupStyle.ContainerStyle = (Style)Application.Current.Resources["groupDatagridStyle"];
            this.GroupStyle.Add(groupStyle);
        }

        public void SetItemsSource(IList ds, string groupStr)
        {
            ListCollectionView lv = new ListCollectionView(ds);
            lv.GroupDescriptions.Add(new PropertyGroupDescription(groupStr));
            this.ItemsSource = lv;
        }

        public void SetItemsSource(IEnumerable ds)
        {
            this.ItemsSource = ds;
        }
    }

    public class CheckedGroupDataGird : GroupDataGrid
    {

        public virtual void Cbx_Click(object sender, RoutedEventArgs e)
        {
            bool isChecked = ((this.Columns[0].Header as CheckBox).IsChecked == true) ? true : false;

            foreach (var item in this.ItemsSource)
            {
                (item as IIsChecked).IsChecked = isChecked;
            }
        }
    }

    public static class PasswordBoxHelper
    {
        public static readonly DependencyProperty PasswordProperty =
            DependencyProperty.RegisterAttached("Password",
            typeof(string), typeof(PasswordBoxHelper),
            new FrameworkPropertyMetadata(string.Empty, OnPasswordPropertyChanged));
        public static readonly DependencyProperty AttachProperty =
            DependencyProperty.RegisterAttached("Attach",
            typeof(bool), typeof(PasswordBoxHelper), new PropertyMetadata(false, Attach));
        private static readonly DependencyProperty IsUpdatingProperty =
           DependencyProperty.RegisterAttached("IsUpdating", typeof(bool),
           typeof(PasswordBoxHelper));

        public static void SetAttach(DependencyObject dp, bool value)
        {
            dp.SetValue(AttachProperty, value);
        }
        public static bool GetAttach(DependencyObject dp)
        {
            return (bool)dp.GetValue(AttachProperty);
        }
        public static string GetPassword(DependencyObject dp)
        {
            return (string)dp.GetValue(PasswordProperty);
        }
        public static void SetPassword(DependencyObject dp, string value)
        {
            dp.SetValue(PasswordProperty, value);
        }
        private static bool GetIsUpdating(DependencyObject dp)
        {
            return (bool)dp.GetValue(IsUpdatingProperty);
        }
        private static void SetIsUpdating(DependencyObject dp, bool value)
        {
            dp.SetValue(IsUpdatingProperty, value);
        }
        private static void OnPasswordPropertyChanged(DependencyObject sender,
            DependencyPropertyChangedEventArgs e)
        {
            PasswordBox passwordBox = sender as PasswordBox;
            passwordBox.PasswordChanged -= PasswordChanged;
            if (!(bool)GetIsUpdating(passwordBox))
            {
                passwordBox.Password = (string)e.NewValue;
            }
            passwordBox.PasswordChanged += PasswordChanged;
        }
        private static void Attach(DependencyObject sender,
            DependencyPropertyChangedEventArgs e)
        {
            PasswordBox passwordBox = sender as PasswordBox;
            if (passwordBox == null)
                return;
            if ((bool)e.OldValue)
            {
                passwordBox.PasswordChanged -= PasswordChanged;
            }
            if ((bool)e.NewValue)
            {
                passwordBox.PasswordChanged += PasswordChanged;
            }
        }
        private static void PasswordChanged(object sender, RoutedEventArgs e)
        {
            PasswordBox passwordBox = sender as PasswordBox;
            SetIsUpdating(passwordBox, true);
            SetPassword(passwordBox, passwordBox.Password);
            SetIsUpdating(passwordBox, false);
        }
    }

    public class DataGridComboBoxColumnBinding : DataGridComboBoxColumn
    {
        protected override FrameworkElement GenerateEditingElement(DataGridCell cell, object dataItem)
        {
            FrameworkElement element = base.GenerateEditingElement(cell, dataItem);
            CopyItemsSource(element);
            return element;
        }

        protected override FrameworkElement GenerateElement(DataGridCell cell, object dataItem)
        {
            FrameworkElement element = base.GenerateElement(cell, dataItem);
            CopyItemsSource(element);
            return element;
        }

        private void CopyItemsSource(FrameworkElement element)
        {
            BindingOperations.SetBinding(element, ComboBox.ItemsSourceProperty,
              BindingOperations.GetBinding(this, ComboBox.ItemsSourceProperty));
        }
    }

    public class HHJTPanMoveScrollViewer : ScrollViewer
    {
        private Point _mouseDragStartPoint;
        private Point _scrollStartOffset;

        public bool PanFlag { get; set; }

        protected override void OnPreviewMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            if (PanFlag)
            {
                _mouseDragStartPoint = e.GetPosition(this);
                _scrollStartOffset.X = this.HorizontalOffset;
                _scrollStartOffset.Y = this.VerticalOffset;

                // 若平易则更改鼠标图标
                this.Cursor = (this.ExtentWidth > this.ViewportWidth) ||
                    (this.ExtentHeight > this.ViewportHeight) ?
                    Cursors.ScrollAll : Cursors.Arrow;

                this.CaptureMouse();
            }
            base.OnPreviewMouseDown(e);
        }

        protected override void OnPreviewMouseMove(MouseEventArgs e)
        {
            if (PanFlag)
            {
                if (this.IsMouseCaptured)
                {
                    // 得到新鼠标位置
                    Point mouseDragCurrentPoint = e.GetPosition(this);

                    // 计算平移距离 
                    Point delta = new Point(
                        (mouseDragCurrentPoint.X > this._mouseDragStartPoint.X) ?
                        -(mouseDragCurrentPoint.X - this._mouseDragStartPoint.X) :
                        (this._mouseDragStartPoint.X - mouseDragCurrentPoint.X),
                        (mouseDragCurrentPoint.Y > this._mouseDragStartPoint.Y) ?
                        -(mouseDragCurrentPoint.Y - this._mouseDragStartPoint.Y) :
                        (this._mouseDragStartPoint.Y - mouseDragCurrentPoint.Y));

                    // 滚动至新位置 
                    this.ScrollToHorizontalOffset(this._scrollStartOffset.X + delta.X);
                    this.ScrollToVerticalOffset(this._scrollStartOffset.Y + delta.Y);
                }
            }
            base.OnPreviewMouseMove(e);
        }

        protected override void OnPreviewMouseUp(MouseButtonEventArgs e)
        {
            if (PanFlag)
            {
                if (this.IsMouseCaptured)
                {
                    this.Cursor = Cursors.Arrow;
                    this.ReleaseMouseCapture();
                }
            }
            base.OnPreviewMouseUp(e);
        }

        protected override void OnPreviewKeyDown(KeyEventArgs e)
        {
            e.Handled = true;
            base.OnPreviewKeyDown(e);
        }
    }

    public class HHJTDrawingCanvas : Canvas
    {
        private List<Visual> visuals = new List<Visual>();
        protected override Visual GetVisualChild(int index)
        {
            return visuals[index];
        }
        protected override int VisualChildrenCount
        {
            get
            {
                return visuals.Count;
            }
        }

        public void AddVisual(Visual visual)
        {
            visuals.Add(visual);

            base.AddVisualChild(visual);
            base.AddLogicalChild(visual);
        }

        public void DeleteAllVisual()
        {
            foreach (var item in visuals)
            {
                base.RemoveVisualChild(item);
                base.RemoveLogicalChild(item);
            }
            visuals.Clear();
        }

        public void DeleteVisual(Visual visual)
        {
            visuals.Remove(visual);

            base.RemoveVisualChild(visual);
            base.RemoveLogicalChild(visual);
        }

        public DrawingVisual GetVisual(Point point)
        {
            HitTestResult hitResult = VisualTreeHelper.HitTest(this, point);
            return hitResult.VisualHit as DrawingVisual;
        }

        public DrawingVisual GetVisualLink(Point point)
        {
            HitTestResult hitResult = VisualTreeHelper.HitTest(this, point);
            return hitResult.VisualHit as DrawingVisual;
        }

        private List<DrawingVisual> hits = new List<DrawingVisual>();
        public List<DrawingVisual> GetVisuals(Geometry region)
        {
            hits.Clear();
            GeometryHitTestParameters parameters = new GeometryHitTestParameters(region);
            HitTestResultCallback callback = new HitTestResultCallback(this.HitTestCallback);
            VisualTreeHelper.HitTest(this, null, callback, parameters);
            return hits;
        }

        private HitTestResultBehavior HitTestCallback(HitTestResult result)
        {
            GeometryHitTestResult geometryResult = (GeometryHitTestResult)result;
            DrawingVisual visual = result.VisualHit as DrawingVisual;
            if (visual != null &&
                geometryResult.IntersectionDetail == IntersectionDetail.FullyInside)
            {
                hits.Add(visual);
            }
            return HitTestResultBehavior.Continue;
        }
    }

    public abstract class HHJTDrawingVisual : DrawingVisual, INotifyPropertyChanged
    {
        protected SolidColorBrush _foregroundBrush;
        protected SolidColorBrush _backgroundBrush;
        protected double _penWidth;
        protected string _fontName = "微软雅黑";
        protected SolidColorBrush _fontBrush = Brushes.Black;

        public HHJTDrawingVisual()
        {
            _foregroundBrush = Brushes.Black;
            _backgroundBrush = Brushes.White;
            _penWidth = 1;
        }

        public HHJTDrawingVisual(SolidColorBrush foregroundBrush, SolidColorBrush backgroundBrush, double penWidth)
        {
            _foregroundBrush = foregroundBrush;
            _backgroundBrush = backgroundBrush;
            _penWidth = penWidth;
        }

        public SolidColorBrush BackgroundBrush
        {
            get { return _backgroundBrush; }
            set { _backgroundBrush = value; GetDrawingVisual(); }
        }
        public SolidColorBrush ForegroundBrush
        {
            get { return _foregroundBrush; }
            set { _foregroundBrush = value; GetDrawingVisual(); }
        }
        public double PenWidth
        {
            get { return _penWidth; }
            set { _penWidth = value; GetDrawingVisual(); }
        }

        public abstract Visual GetDrawingVisual();

        #region INotifyPropertyChanged 成员

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName)
        {

            PropertyChangedEventHandler handler = this.PropertyChanged;
            if (handler != null)
            {
                var e = new PropertyChangedEventArgs(propertyName);
                handler(this, e);
            }
        }
        #endregion
    }


}
