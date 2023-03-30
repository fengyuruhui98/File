using HHJT.AFC.Base.MVVM;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;

namespace HHJT.AFC.Framework.UI
{
    public class V_DataInputBase : UserControl
    {
        protected VM_DataInputBase _vm;

        protected Button BtnNew;
        protected Button BtnCancel;
        protected Button BtnSave;

        protected Button BtnPrint;
        protected Button BtnDelete;

        protected CheckedGroupDataGird _dataGridContent;
        protected Grid _gridOutter;
        protected Grid _gridDataInput;
        protected Grid gridInput;

        protected bool BtnQueryVisible = false;
        //TODO:RFID
        protected Button BtnQuery;
        public V_DataInputBase()
        {
            Foreground = Brushes.Black;
            Background = Brushes.White;
            InitControls();
        }

        public V_DataInputBase(bool btnQueryBisible)
        {
            this.BtnQueryVisible = btnQueryBisible;
            Foreground = Brushes.Black;
            Background = Brushes.White;
            InitControls();
        }

        protected List<Control> _controls = new List<Control>();

        protected void InitControls()
        {
            _gridOutter = new Grid();
            _gridOutter.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            _gridOutter.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            //1查询区域
            GroupBox groupboxInput = new GroupBox() { Header = "数据输入" };
            //1.1查询重置按钮 Grid
            gridInput = new Grid();
            gridInput.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            gridInput.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            gridInput.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            gridInput.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            gridInput.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });

            WrapPanel wrapPanelNew = new WrapPanel();

            //else
            //{
                this.BtnNew = new Button()
                {
                    Content = "新录入(_N)",
                    Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
                    //Tag = "R"
                };
                wrapPanelNew.Children.Add(BtnNew);
            //}
            // BtnNew.KeyUp += Control_KeyUp;
            // _controls.Add(BtnNew);//响应键盘切换
                if (BtnQueryVisible)
                {
                    //TODO:RFID
                    this.BtnQuery = new Button()
                    {
                        Content = "读取数据(_Q)",
                        Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
                        //Tag = "R"
                    };
                    wrapPanelNew.Children.Add(BtnQuery);
                }

            gridInput.Children.Add(wrapPanelNew);

            Border borderSplit = new Border()
            {
                Margin = new Thickness(-2, 5, -2, 5),
                BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
                BorderThickness = new Thickness(0.5)
            };
            gridInput.Children.Add(borderSplit);
            Grid.SetRow(borderSplit, 1);
            //数据录入区域
            _gridDataInput = new Grid();

            gridInput.Children.Add(_gridDataInput);
            Grid.SetRow(_gridDataInput, 2);

            borderSplit = new Border()
            {
                Margin = new Thickness(-2, 5, -2, 5),
                BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
                BorderThickness = new Thickness(0.5)
            };
            gridInput.Children.Add(borderSplit);
            Grid.SetRow(borderSplit, 3);

            //按钮区域
            WrapPanel wrapPanel = new WrapPanel();

            this.BtnCancel = new Button()
            {
                Content = "取消(_C)",
                Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
                //Tag = "Q"
            };
            this.BtnSave = new Button()
            {
                Content = "保存(_S)",
                Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
                //Tag = "Q"
            };

            BtnSave.KeyUp += Control_KeyUp;
            _controls.Add(BtnSave);//响应键盘Enter切换

            wrapPanel.Children.Add(BtnCancel);
            wrapPanel.Children.Add(BtnSave);
            gridInput.Children.Add(wrapPanel);
            Grid.SetRow(wrapPanel, 4);

            groupboxInput.Content = gridInput;
            _gridOutter.Children.Add(groupboxInput);
            //1.2 border
            //Border borderQuerySplit = new Border()
            //{
            //    Margin = new Thickness(5, -5, 5, -5),
            //    BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
            //    BorderThickness = new Thickness(0.5)
            //};

            //2数据区域
            GroupBox groupboxData = new GroupBox() { Header = "数据显示" };

            Grid gridDataView = new Grid();
            gridDataView.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });
            gridDataView.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });

            //数据显示
            Border borderData = new Border()
            {
                Margin = new Thickness(2, 5, 5, 2),
                Padding = new Thickness(5),
                BorderBrush = new SolidColorBrush(Colors.Teal),
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(3)
            };

            _dataGridContent = new CheckedGroupDataGird();
            _dataGridContent.SelectionChanged += new SelectionChangedEventHandler(DataGridContent_SelectionChanged);
            borderData.Child = _dataGridContent;

            gridDataView.Children.Add(borderData);
            Grid.SetRow(borderData, 0);

            //数据操作
            //WrapPanel wrapPanelData = new WrapPanel() { HorizontalAlignment = System.Windows.HorizontalAlignment.Right};
            //this.BtnPrint = new Button()
            //{
            //    Content = "打印(_P)",
            //    Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
            //    //Tag = "Q"
            //};

            //this.BtnDelete = new Button()
            //{
            //    Content = "删除(_D)",
            //    Style = (Style)System.Windows.Application.Current.Resources["styleOperationButton"],
            //    //Tag = "Q"
            //};
            //wrapPanelData.Children.Add(BtnPrint);
            //wrapPanelData.Children.Add(BtnDelete);
            //gridDataView.Children.Add(wrapPanelData);
            //Grid.SetRow(wrapPanelData, 1);

            groupboxData.Content = gridDataView;

            _gridOutter.Children.Add(groupboxData);
            Grid.SetColumn(groupboxData, 1);

            this.Content = _gridOutter;

            FurtherInitLayout();

            _gridDataInput.SetBinding(Grid.IsEnabledProperty, new Binding() { Source = _vm, Path = new PropertyPath("FlagNew") });
            _gridDataInput.SetBinding(Grid.DataContextProperty, "VmInput");
            _dataGridContent.SetBinding(DataGrid.ItemsSourceProperty, "VmDataCollection");

            //if (!BtnQueryVisible)
            //{
                BtnNew.Click += BtnNew_Click;
                BtnNew.IsDefault = true;
                BtnNew.Focus();
            //}
            //     BtnSave.Click += BtnSave_Click;
        }

        void BtnNew_Click(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            _vm.New();
            //_vm.FlagNew = true;
            firstItem = true;
            //_controls[1].IsEnabled = true;
            _controls[0].Focus();
        }

        private bool firstItem;

        protected void Control_KeyUp(object sender, KeyEventArgs e)
        {
            //throw new NotImplementedException();
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                if (firstItem) { firstItem = !firstItem; return; }

                int i = _controls.FindIndex(d => d == sender);
                if (i != _controls.Count - 1)
                {
                    _controls[i + 1].IsEnabled = true;
                    _controls[i + 1].Focus();
                }
                //else
                //{
                //    _controls[0].IsEnabled = true;
                //    _controls[0].Focus();
                //}
            }
        }

        void DataGridContent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (_vm != null)
            {
                _vm.VmCurrentItem = _dataGridContent.SelectedItem;
                _vm.UpdateInput();
            }
        }

        protected virtual void FurtherInitLayout() { }

        protected void RegisterCommand(Button btnSource, ICommand cmd)
        {
            CommandBinding cb = new CommandBinding(cmd);
            this.CommandBindings.Add(cb);
            btnSource.Command = cmd;
        }
    }

    public abstract class VM_DataInputBase : ViewModelBase
    {
        public VM_DataInputBase()
        {
            FlagNew = false;
        }

        private bool _isUpdate;
        public bool IsUpdate { get; set; }
        private VMDataErrorBase _vmInput;

        public VMDataErrorBase VmInput
        {
            get { return _vmInput; }
            set
            {
                if (_vmInput == value) return;
                _vmInput = value;
                OnPropertyChanged("VmInput");
            }
        }
        private ObservableCollection<VMDataErrorBase> _vmDataCollection = new ObservableCollection<VMDataErrorBase>();

        public ObservableCollection<VMDataErrorBase> VmDataCollection
        {
            get { return _vmDataCollection; }
            set { _vmDataCollection = value; }
        }
        private object _vmCurrentItem;

        public object VmCurrentItem
        {
            get { return _vmCurrentItem; }
            set { _vmCurrentItem = value; }
        }

        private bool _flagNew;

        public bool FlagNew
        {
            get { return _flagNew; }
            set
            {
                if (_flagNew == value) return;
                _flagNew = value;
                OnPropertyChanged("FlagNew");
            }
        }

        #region Commands
        protected RelayCommand _commandNew; //新录入
        protected RelayCommand _commandCancel; //取消
        protected RelayCommand _commandSave; //保存
        protected RelayCommand _commandPrint; //打印
        protected RelayCommand _commandDelete; //删除
        protected RelayCommand _commandQuery; //查询

        [AttrMethod("新录入", AccelerateKey = "N")]
        public ICommand CommandNew
        {
            get
            {
                if (_commandNew == null)
                {
                    _commandNew = new RelayCommand(
                    param => this.New(),
                    param => this.CanNew()
                    );
                }
                return _commandNew;
            }
        }

        [AttrMethod("查询", AccelerateKey = "Q")]
        public ICommand CommandQuery
        {
            get
            {
                if (_commandQuery == null)
                {
                    _commandQuery = new RelayCommand(
                    param => this.Query(),
                    param => this.CanQuery()
                    );
                }
                return _commandQuery;
            }
        }
        [AttrMethod("取消", AccelerateKey = "C")]
        public ICommand CommandCancel
        {
            get
            {
                if (_commandCancel == null)
                {
                    _commandCancel = new RelayCommand(
                    param => this.Cancel(),
                    param => this.CanCancel()
                    );
                }
                return _commandCancel;
            }
        }
        [AttrMethod("保存", AccelerateKey = "S")]
        public ICommand CommandSave
        {
            get
            {
                if (_commandSave == null)
                {
                    _commandSave = new RelayCommand(
                    param => this.Save(),
                    param => this.CanSave()
                    );
                }
                return _commandSave;
            }
        }
        [AttrMethod("打印", AccelerateKey = "P")]
        public ICommand CommandPrint
        {
            get
            {
                if (_commandPrint == null)
                {
                    _commandPrint = new RelayCommand(
                    param => this.Print(),
                    param => this.CanPrint()
                    );
                }
                return _commandPrint;
            }
        }

        [AttrMethod("删除", AccelerateKey = "D")]
        public ICommand CommandDelete
        {
            get
            {
                if (_commandDelete == null)
                {
                    _commandDelete = new RelayCommand(
                    param => this.Delete(),
                    param => this.CanDelete()
                    );
                }
                return _commandDelete;
            }
        }
        public virtual void New()
        {
            FlagNew = true;
            OnPropertyChanged("VmInput");

        }
        protected virtual bool CanNew()
        {
            return !_flagNew;
        }
        protected virtual void Cancel()
        {
            if (!IsUpdate)
            {
                FlagNew = false;
            }
        }
        protected virtual bool CanCancel()
        {
            return _flagNew;
        }
        protected virtual void Save()
        {
            if (!IsUpdate)
            {
                FlagNew = false;
            }
        }
        protected virtual bool CanSave()
        {
            return _flagNew;
        }
        protected virtual void Print()
        {

        }
        protected virtual bool CanPrint()
        {
            return _vmCurrentItem != null;
        }

        protected virtual void Delete()
        {

        }
        protected virtual bool CanDelete()
        {
            return _vmCurrentItem != null;
        }

        protected virtual void Query()
        {

        }
        protected virtual bool CanQuery()
        {
            return _flagNew;
        }
        #endregion

        public virtual void UpdateInput()
        {

        }
    }
}