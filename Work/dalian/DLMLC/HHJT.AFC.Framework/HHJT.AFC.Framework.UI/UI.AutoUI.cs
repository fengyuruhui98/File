using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using HHJT.AFC.Framework.Skin;

namespace HHJT.AFC.Framework.UI
{
    public static class AutoUI
    {
        /// <summary>
        /// update by liusk 0517
        /// 解决第二次自动生成时字段顺序互换问题
        /// </summary>
        private static Dictionary<string, PropertyInfo[]> _propDics = new Dictionary<string, PropertyInfo[]>();

        public static PropertyInfo[] GetProps(Type type)
        {
            if (_propDics.ContainsKey(type.FullName)) return _propDics[type.FullName];

            _propDics.Add(type.FullName, type.GetProperties());
            return _propDics[type.FullName];

        }

        //public static void AutoGeneateQueryPanel(IEnumerable<>
        /// <summary>
        /// 自动生成查询界面
        /// </summary>
        /// <param name="type"></param>
        /// <param name="panel"></param>
        /// <returns></returns>
        public static List<AttrQueryPropertyAttribute> AutoGenerateQueryPanel(Type type, WrapPanel panel)
        {
            List<AttrQueryPropertyAttribute> props = new List<AttrQueryPropertyAttribute>();

            props = AutoGenerateQueryPanel(type, panel, true);
            props = AutoGenerateLayoutPanel(props, panel);
            return props;
        }


        /// <summary>
        /// 自动生成查询界面,生成控件
        /// </summary>
        /// <param name="type"></param>
        /// <param name="panel"></param>
        /// <param name="IsValidate"></param>
        /// <param name="IsThreeState"></param>
        /// <returns></returns>
        public static List<AttrQueryPropertyAttribute> AutoGenerateQueryPanel(Type type, WrapPanel panel,bool IsValidate,bool IsThreeState=true)
        {
            List<AttrQueryPropertyAttribute> props = new List<AttrQueryPropertyAttribute>();

            int controlsNum = 0;
            #region createctrl
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrQueryPropertyAttribute aProp in info.GetCustomAttributes(typeof(AttrQueryPropertyAttribute), true))
                {
                    aProp.PropertyName = info.Name;
                    aProp.PropertyType = info.PropertyType.Name;

                    if (aProp.DisplayName == "-  ")
                    {
                        aProp.DisplayLable = new Label() { Content = aProp.DisplayName };
                    }
                    else
                    {
                        controlsNum++;

                        aProp.DisplayLable = new Label
                        {
                            Content = aProp.DisplayName + "(_" + controlsNum + "):"
                        };
                    }

                    aProp.DisplayLable.VerticalAlignment = VerticalAlignment.Center;

                    switch (aProp.QueryInputType)
                    {

                        case UI_INPUT_TYPE.TEXTBOX:
                            TextBox tbx = new TextBox()
                                {
                                    VerticalContentAlignment = System.Windows.VerticalAlignment.Center
                                };
                            tbx.Style = (Style) Application.Current.Resources["textBoxInError"];
                            tbx.SetBinding(TextBox.TextProperty, new Binding(aProp.PropertyName)
                                {
                                    ValidatesOnDataErrors = IsValidate,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                    Converter = new NullableConverter()
                                });

                            tbx.GotFocus += ATextBox_GotFocus;

                            aProp.QueryControl = tbx;
                            aProp.QueryControl.Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH;
                            break;
                        case UI_INPUT_TYPE.COMBOBOX:
                            {
                                ComboBox cbx = new ComboBox()
                                    {
                                        Focusable = true,
                                        Style = (Style) Application.Current.Resources["styleInputControl"],
                                        Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                                        SelectedValuePath = "Key",
                                        DisplayMemberPath = "Value",
                                        // IsEditable = true
                                    };
                                //Combox DatasourceItems定义参见ValueDisplayPairXXXX
                                cbx.SetBinding(ComboBox.ItemsSourceProperty, new Binding(aProp.ComboDatasource)
                                    {
                                        ValidatesOnDataErrors = IsValidate,
                                        UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                                    });
                                cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(aProp.PropertyName)
                                    {
                                        ValidatesOnDataErrors = IsValidate,
                                        UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                        Mode = BindingMode.TwoWay
                                    });
                                aProp.QueryControl = cbx;

                                break;
                            }
                        case UI_INPUT_TYPE.CHECKCOMBOBOX:
                            {
                                ComboBox cbx = new ComboBox()
                                    {
                                        Focusable = true,
                                        Style = (Style) Application.Current.Resources["styleInputControl"],
                                        Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                                        SelectedValuePath = "Key",
                                        DisplayMemberPath = "Value",
                                        IsReadOnly = false,
                                        // IsEditable = true
                                    };
                                //Combox DatasourceItems定义参见ValueDisplayPairXXXX
                                cbx.SetBinding(ComboBox.ItemsSourceProperty, new Binding(aProp.ComboDatasource)
                                    {
                                        ValidatesOnDataErrors = IsValidate,
                                        UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                                    });
                                cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(aProp.PropertyName)
                                    {
                                        ValidatesOnDataErrors = IsValidate,
                                        UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                        Mode = BindingMode.TwoWay
                                    });
                                aProp.QueryControl = cbx;

                                break;
                            }
                        case UI_INPUT_TYPE.DATE:
                            //aProp.QueryControl = new DatePicker()
                            //{
                            //    Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                            //     Style = (Style)Application.Current.Resources["styleInputControl"],
                            //};

                            DatePicker myDatePicker = new DatePicker()
                                {
                                    Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                                    Style = (Style) Application.Current.Resources["styleInputControl"],
                                };
                            myDatePicker.SetBinding(DatePicker.SelectedDateProperty, new Binding(aProp.PropertyName)
                                {
                                    ValidatesOnDataErrors = IsValidate,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                    Mode = BindingMode.TwoWay
                                });
                            aProp.QueryControl = myDatePicker;
                            break;
                        case UI_INPUT_TYPE.DATETIME:
                            Xceed.Wpf.Toolkit.DateTimePicker dtPicker = new Xceed.Wpf.Toolkit.DateTimePicker
                            {
                                Format = Xceed.Wpf.Toolkit.DateTimeFormat.Custom,
                                FormatString = "yyyy-MM-dd HH:mm:ss",
                                Style = (Style)Application.Current.Resources["styleInputControl"]
                            };
                            dtPicker.SetBinding(Xceed.Wpf.Toolkit.DateTimePicker.ValueProperty,
                                                new Binding(aProp.PropertyName)
                                                    {
                                                        UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                                        Mode = BindingMode.TwoWay
                                                    });

                            aProp.QueryControl = dtPicker;
                            break;

                        case UI_INPUT_TYPE.CHECKBOX:
                            CheckBox checkBox;
                            if (IsThreeState)
                            {
                                checkBox = new CheckBox()
                                    {
                                        VerticalAlignment = VerticalAlignment.Center,
                                        Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                                        IsThreeState = IsThreeState
                                    };
                            }
                            else
                            {
                                checkBox = new CheckBox()
                                {
                                    VerticalAlignment = VerticalAlignment.Center,
                                    Width = 25,
                                    IsThreeState = IsThreeState
                                };
                            }
                            checkBox.SetBinding(CheckBox.IsCheckedProperty, aProp.PropertyName);
                            aProp.QueryControl = checkBox;
                            break;
                        default:
                            break;
                    }
                    props.Add(aProp);

                }

            }
            #endregion

            return props;
        }

        /// <summary>
        /// 自动生成查询界面
        /// CHECKBox为TwoState
        /// </summary>
        /// <param name="type"></param>
        /// <param name="panel"></param>
        /// <returns></returns>
        public static List<AttrQueryPropertyAttribute> AutoGenerateQueryPanelTwoState(Type type, WrapPanel panel)
        {
            List<AttrQueryPropertyAttribute> props = new List<AttrQueryPropertyAttribute>();

            props = AutoGenerateQueryPanel(type, panel, true,false);

            props = AutoGenerateLayoutPanel(props, panel);

            return props;
        }

        /// <summary>
        /// 产生控件后，将控件放入Panel中
        /// </summary>
        /// <param name="props"></param>
        /// <param name="panel"></param>
        /// <returns></returns>
        public static List<AttrQueryPropertyAttribute> AutoGenerateLayoutPanel(List<AttrQueryPropertyAttribute> props, WrapPanel panel)
        {
            foreach (var item in props)
            {
                StackPanel stPanel = new StackPanel();
                if (item.DisplayName != "-  ")
                {
                    stPanel.Margin = new System.Windows.Thickness(2, 2, 5, 2);
                }
                stPanel.Orientation = Orientation.Horizontal;

                AttrQueryPropertyAttribute uia = item as AttrQueryPropertyAttribute;

                uia.DisplayLable.SetBinding(Label.TargetProperty, new System.Windows.Data.Binding() { Source = uia.QueryControl });//快捷访问绑定

                stPanel.Children.Add(uia.DisplayLable);//查询项名称
                stPanel.Children.Add(uia.QueryControl);//查询按钮


                panel.Children.Add(stPanel);
            }

            return props;
        }


        public static Collection<AttrMethodAttribute> AugoGenerateDataProcessPanel(Type type, StackPanel panel)
        {
            Collection<AttrMethodAttribute> props = new Collection<AttrMethodAttribute>();

            int controlsNum = 0;
            #region createctrl
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrMethodAttribute aProp in info.GetCustomAttributes(typeof(AttrMethodAttribute), true))
                {
                    Button btn = new Button
                    {
                        Content = aProp.FunctionDisplayName + "(_" + aProp.AccelerateKey + ")",
                        Tag = aProp.FunctionDisplayName,

                        Style = (System.Windows.Style)System.Windows.Application.Current.Resources["styleOperationButton"]
                    };

                    aProp.ButtonCtrl = btn;

                    panel.Children.Add(btn);
                    props.Add(aProp);
                    controlsNum++;
                }
            }
            #endregion

            return props;
        }

        public static Collection<AttrMethodAttribute> AugoGenerateButtonsInWrapPanel(Type type, WrapPanel panel)
        {
            Collection<AttrMethodAttribute> props = new Collection<AttrMethodAttribute>();

            int controlsNum = 0;
            #region createctrl
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrMethodAttribute aProp in info.GetCustomAttributes(typeof(AttrMethodAttribute), true))
                {
                    Button btn = new Button
                    {
                        Content = aProp.FunctionDisplayName + "(_" + aProp.AccelerateKey + ")",
                        Tag = aProp.FunctionDisplayName,

                        Style = (System.Windows.Style)System.Windows.Application.Current.Resources["styleOperationButton"]
                    };

                    aProp.ButtonCtrl = btn;

                    panel.Children.Add(btn);
                    props.Add(aProp);
                    controlsNum++;
                }

            }
            #endregion

            return props;

        }
       public static void UIDatagridInit(Type type, CheckedGroupDataGird datagrid, params IEnumerable[] datasourceList)
        {
            datagrid.Columns.Clear();

            if (type.GetInterface("IIsChecked") != null) //如果为真，handler必须提供，handler的实现一般在基类中
            {
                CheckBox cbx = new CheckBox();
                cbx.Click += datagrid.Cbx_Click;
                Binding bd = new Binding
                {
                    Path = new PropertyPath("IsChecked"),
                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                };
                datagrid.Columns.Add(
                    new DataGridCheckBoxColumn()
                    {
                        Header = cbx,
                        Binding = bd,
                        IsReadOnly = false,
                        CanUserReorder = false,

                    }
                    );
            }

            int dataSourceIndex = 0;

            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    if (!a.IsNotDatagridColumn)
                    {
                        a.PropertyName = info.Name;
                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                        {
                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.DataConvertType == DataConverterType.Currency)
                            {
                                tbx.Binding = new System.Windows.Data.Binding(a.PropertyName) { StringFormat = "{0:0.00}" };

                            }
                            else if (a.DataConvertType == DataConverterType.DateTime)
                            {
                                tbx.Binding = new System.Windows.Data.Binding(a.PropertyName) { StringFormat = "yyyy-MM-dd HH:mm:ss" };

                            }
                            else if (a.DataConvertType == DataConverterType.Date)
                            {
                                tbx.Binding = new System.Windows.Data.Binding(a.PropertyName) { StringFormat = "yyyy-MM-dd" };

                            }
                            else
                            {
                                tbx.Binding = new System.Windows.Data.Binding(a.PropertyName);

                            }

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.DATE)
                        {
                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName) { Converter = new DateConverter() },
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                        {
                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName) { Converter = new DateTimeConverter() },
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.CHECKBOX)
                        {
                            DataGridCheckBoxColumn cbx = new DataGridCheckBoxColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName),
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                cbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(cbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                        {
                            DataGridComboBoxColumn cbxColumn = new DataGridComboBoxColumn()
                            {
                                Header = a.DisplayName,
                                IsReadOnly = true,

                                SelectedValuePath = "Key",
                                DisplayMemberPath = "Value",
                                SelectedValueBinding = new Binding(a.PropertyName),
                                ItemsSource = datasourceList[dataSourceIndex++],
                                CanUserReorder = true,

                            };

                            if (a.IsCellCentered)
                                cbxColumn.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            //BindingOperations.SetBinding(cbxColumn, 
                            //    DataGridComboBoxColumn.ItemsSourceProperty, 
                            //    new Binding(a.ComboDatasource)); 
                            datagrid.Columns.Add(cbxColumn);

                        }
                    }
                }
            }

        }

        //数据显示DataGrid初始化，根据选择项
        public static void UIDatagridInit(Type type, CheckedGroupDataGird datagrid, List<string> checkedProps, params IEnumerable[] datasourceList)
        {
            datagrid.Columns.Clear();

            List<AttrDataPropertyAttribute> dataProps = new List<AttrDataPropertyAttribute>();

            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    a.PropertyName = info.Name;
                    dataProps.Add(a);
                    break;
                }
            }

            dataProps = dataProps.OrderBy(d => d.OrderByIndex).ToList();

            int dataSourceIndex = -1;
            foreach (var item in checkedProps)
            {
                AttrDataPropertyAttribute a = dataProps.FirstOrDefault(d => d.PropertyName == item);
                if (a != null)
                {
                    if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                    {
                        Binding PropertyBinding = new System.Windows.Data.Binding(a.PropertyName);
                        if (a.IsNumericFormat)
                        {
                            PropertyBinding.Converter = new FloatConverter();
                        }
                        DataGridTextColumn tbx = new DataGridTextColumn()
                        {
                            Header = a.DisplayName,
                            Binding = PropertyBinding,
                            IsReadOnly = true,
                            CanUserReorder = true,
                        };

                        if (a.IsCellCentered)
                            tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                        datagrid.Columns.Add(tbx);
                    }
                    else if (a.InputType == UI_INPUT_TYPE.DATE)
                    {
                        DataGridTextColumn tbx = new DataGridTextColumn()
                        {
                            Header = a.DisplayName,
                            Binding = new System.Windows.Data.Binding(a.PropertyName) { Converter = new DateConverter() },
                            IsReadOnly = true,
                            CanUserReorder = true,
                        };

                        if (a.IsCellCentered)
                            tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                        datagrid.Columns.Add(tbx);
                    }
                    else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                    {
                        DataGridTextColumn tbx = new DataGridTextColumn()
                        {
                            Header = a.DisplayName,
                            Binding = new System.Windows.Data.Binding(a.PropertyName) { Converter = new DateTimeConverter() },
                            IsReadOnly = true,
                            CanUserReorder = true,
                        };

                        if (a.IsCellCentered)
                            tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                        datagrid.Columns.Add(tbx);
                    }
                    else if (a.InputType == UI_INPUT_TYPE.CHECKBOX)
                    {
                        DataGridCheckBoxColumn cbx = new DataGridCheckBoxColumn()
                        {
                            Header = a.DisplayName,
                            Binding = new System.Windows.Data.Binding(a.PropertyName),
                            IsReadOnly = true,
                            CanUserReorder = true,
                        };

                        if (a.IsCellCentered)
                            cbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                        datagrid.Columns.Add(cbx);
                    }
                    else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                    {

                        dataSourceIndex = dataProps.Where(d => d.InputType == UI_INPUT_TYPE.COMBOBOX).ToList().FindIndex(d => d == a);

                        DataGridComboBoxColumn cbxColumn = new DataGridComboBoxColumn()
                        {
                            Header = a.DisplayName,
                            IsReadOnly = true,
                            SelectedValuePath = "Key",
                            DisplayMemberPath = "Value",

                            SelectedValueBinding = new Binding(a.PropertyName),
                            ItemsSource = datasourceList[dataSourceIndex],
                            CanUserReorder = true,

                        };

                        if (a.IsCellCentered)
                            cbxColumn.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                        //BindingOperations.SetBinding(cbxColumn,
                        //    DataGridComboBoxColumn.ItemsSourceProperty,
                        //    new Binding(a.ComboDatasource) ); 
                        datagrid.Columns.Add(cbxColumn);

                    }
                }

            }


        }

        public static void ResetQueryInput(List<AttrQueryPropertyAttribute> props)
        {
            foreach (var item in props)
            {
                switch (item.QueryInputType)
                {
                    case UI_INPUT_TYPE.TEXTBOX:
                        (item.QueryControl as TextBox).Text = null;
                        break;
                    case UI_INPUT_TYPE.COMBOBOX:
                        (item.QueryControl as ComboBox).SelectedItem = null;
                        break;
                    case UI_INPUT_TYPE.DATE:
                        (item.QueryControl as DatePicker).SelectedDate = null;
                        (item.QueryControl as DatePicker).Text = null;

                        break;
                    case UI_INPUT_TYPE.DATETIME:
                        (item.QueryControl as Xceed.Wpf.Toolkit.DateTimePicker).Value = null;
                        break;

                    case UI_INPUT_TYPE.CHECKBOX:
                        (item.QueryControl as CheckBox).IsChecked = false;
                        break;
                    default:
                        break;
                }
            }

        }

        public static TreeView CreateSelectedTreeView(IEnumerable itemSource)
        {
            TreeView treeView = new TreeView
            {
                Width = 200,
                Margin = new Thickness(5, 0, 0, 0),
                ItemContainerStyle = (Style)Application.Current.Resources["TreeViewItemStyle"],
                ItemTemplate = (HierarchicalDataTemplate)Application.Current.Resources["CheckBoxItemTemplate"],
                ItemsSource = itemSource
            };
            return treeView;
        }

        #region add by qinyy (2012/10/22)

        /// <summary>
        /// 自动生成“添加”界面2：只参数控件，没有布局
        /// Create by qinyy(2012/10/22)
        /// 
        /// </summary>
        /// <param name="type"></param>
        /// <param name="gridmaster"></param>
        /// <param name="columnCount"></param>
        /// <param name="keyDisabled"></param>
        /// <returns></returns>
        public static List<AttrDataPropertyAttribute> AutoGenerateDialogGridContent(Type type, List<AttrDataPropertyAttribute> props)
        {
            Grid grid = new Grid();
            int controlsNum = props.Count;
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    if (a.IsNotOperateItem) continue;

                    a.PropertyName = info.Name;
                    a.PropertyType = info.PropertyType.Name;

                    controlsNum++;

                    a.DisplayLable = new Label()
                    {
                        Content = a.DisplayName + "(_" + controlsNum + "):",
                        Margin = new Thickness(5, 0, 0, 0),
                        VerticalContentAlignment = VerticalAlignment.Center,
                        HorizontalContentAlignment = HorizontalAlignment.Right
                    };

                    #region switch

                    switch (a.InputType)
                    {
                        case UI_INPUT_TYPE.TEXTBOX:
                            TextBox aTextBox = new TextBox();
                            if (a.DisplayName != "备注")
                            {
                                aTextBox.Style = (Style)Application.Current.Resources["styleInputControl2"];
                                aTextBox.TextWrapping = TextWrapping.Wrap;
                                aTextBox.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
                            }
                            else
                                aTextBox.Style = (Style)Application.Current.Resources["styleInputControl"];

                            aTextBox.SetBinding(TextBox.TextProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });

                            aTextBox.GotFocus += ATextBox_GotFocus;

                            a.InputControl = aTextBox;

                            break;
                        case UI_INPUT_TYPE.IP:
                            TextBox ipTextBox = new TextBox
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"]
                            };

                            ipTextBox.SetBinding(TextBox.TextProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                //UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });

                            ipTextBox.GotFocus += ATextBox_GotFocus;

                            a.InputControl = ipTextBox;

                            break;
                        case UI_INPUT_TYPE.PASSWORDBOX:
                            PasswordBox aPasswordBox = new PasswordBox
                            {
                                //aPasswordBox.Style = (Style)Application.Current.Resources["styleInputControl2"];
                                Width = 160,
                                Height = 25,
                                VerticalAlignment = VerticalAlignment.Center,
                                VerticalContentAlignment = VerticalAlignment.Center,
                                Margin = new Thickness(0, 5, 5, 5)
                            };
                            PasswordBoxHelper.SetAttach(aPasswordBox, true);
                            aPasswordBox.SetBinding(PasswordBoxHelper.PasswordProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                Mode = BindingMode.TwoWay,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });

                            a.InputControl = aPasswordBox;
                            break;
                        case UI_INPUT_TYPE.COMBOBOX:
                            ComboBox cbx = new ComboBox()
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"],
                                Focusable = true,
                                SelectedValuePath = "Key",
                                DisplayMemberPath = "Value",
                                // IsEditable = true
                            };
                            //Combox DatasourceItems定义参见ValueDisplayPairXXXX
                            cbx.SetBinding(ComboBox.ItemsSourceProperty, new Binding(a.ComboDatasource)
                            {
                                ValidatesOnDataErrors = true,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });
                            cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                Mode = BindingMode.TwoWay
                            });
                            a.InputControl = cbx;
                            break;
                        case UI_INPUT_TYPE.COLORCOMBOBOX:
                            ComboBox ccbx = new ComboBox
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"],
                                SelectedValuePath = "ColorName",
                                ItemTemplate = (DataTemplate)Application.Current.Resources["dtColor"],

                                ItemsSource = (new ItemList()).GetListOfColorName
                            };
                            ccbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = false,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                Mode = BindingMode.TwoWay
                            });
                            a.InputControl = ccbx;
                            break;
                        case UI_INPUT_TYPE.DATE:
                            {
                                DatePicker aDatePicker = new DatePicker
                                {
                                    Style = (Style)Application.Current.Resources["styleInputControl"]
                                };
                                aDatePicker.SetBinding(DatePicker.SelectedDateProperty,
                                    new Binding(a.PropertyName) { ValidatesOnDataErrors = true, UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
                                aDatePicker.SelectedDateFormat = DatePickerFormat.Short;
                                a.InputControl = aDatePicker;
                                break;
                            }
                        case UI_INPUT_TYPE.DATETIME:
                            {
                                Xceed.Wpf.Toolkit.DateTimePicker dtPicker = new Xceed.Wpf.Toolkit.DateTimePicker
                                {
                                    Format = Xceed.Wpf.Toolkit.DateTimeFormat.Custom,
                                    FormatString = "yyyy-MM-dd HH:mm:ss",
                                    Style = (Style)Application.Current.Resources["styleInputControl"]
                                };
                                dtPicker.SetBinding(Xceed.Wpf.Toolkit.DateTimePicker.ValueProperty, new Binding(a.PropertyName)
                                {
                                    ValidatesOnDataErrors = true,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                    Mode = BindingMode.TwoWay
                                });

                                a.InputControl = dtPicker;
                                break;
                            }
                        case UI_INPUT_TYPE.CHECKBOX:
                            CheckBox aCheckBox = new CheckBox() { VerticalAlignment = VerticalAlignment.Center };
                            //aCheckBox.Style = (Style)Application.Current.Resources["styleInputControl"];
                            aCheckBox.SetBinding(CheckBox.IsCheckedProperty,
                                new Binding(a.PropertyName) { ValidatesOnDataErrors = true, UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
                            a.InputControl = aCheckBox;

                            break;
                        case UI_INPUT_TYPE.CHECKCOMBOBOX:
                            {
                                ComboBox cbx1 = new ComboBox()
                                {
                                    Focusable = true,
                                    Style = (Style)Application.Current.Resources["styleInputControl"],
                                    Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH,
                                    SelectedValuePath = "Key",
                                    DisplayMemberPath = "Value",
                                    IsReadOnly = false,
                                    // IsEditable = true
                                };
                                //Combox DatasourceItems定义参见ValueDisplayPairXXXX
                                cbx1.SetBinding(ComboBox.ItemsSourceProperty, new Binding(a.ComboDatasource)
                                {
                                    ValidatesOnDataErrors = true,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                                });
                                cbx1.SetBinding(ComboBox.SelectedValueProperty, new Binding(a.PropertyName)
                                {
                                    ValidatesOnDataErrors = true,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                    Mode = BindingMode.TwoWay
                                });
                                a.InputControl = cbx1;

                                break;
                            }
                        default:
                            break;
                    }

                    #endregion

                    if (a.Disabled)
                    {
                        a.InputControl.IsEnabled = false;
                    }
                    a.DisplayLable.SetBinding(Label.TargetProperty, new System.Windows.Data.Binding() { Source = a.InputControl });//快捷访问绑定
                    props.Add(a);
                }
            }
            return props;
        }
        /// <summary>
        /// 布局
        /// </summary>
        /// <param name="props"></param>
        /// <param name="panel"></param>
        /// <param name="columnCount"></param>
        /// <param name="keyDisabled"></param>
        /// <returns></returns>
        public static List<AttrDataPropertyAttribute> AutoGenerateDialogLayoutPanel(List<AttrDataPropertyAttribute> props, Grid gridmaster, int columnCount, bool keyDisabled)
        {
            Grid grid = new Grid();
            //System.Collections.IList props = new List<AttrDataPropertyAttribute>();

            //columnCount
            for (int i = 0; i < columnCount; i++) //create grid
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
                grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            }

            //grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            for (int i = 0; i < props.Count / columnCount + 1; i++)
            {
                grid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            }
            grid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            int controlsNum = 0;
            foreach (var item in props)
            {
                AttrDataPropertyAttribute uia = item as AttrDataPropertyAttribute;
                if (keyDisabled)
                {
                    if (uia.IsNotUpdateItem)
                    {
                        uia.InputControl.IsEnabled = false;
                    }
                }

                if (uia.DisplayName == "备注")
                {
                    while (controlsNum % columnCount != 0)
                    {
                        controlsNum++;
                    }
                }

                grid.Children.Add(uia.DisplayLable);
                Grid.SetRow(uia.DisplayLable, controlsNum / columnCount);
                Grid.SetColumn(uia.DisplayLable, (controlsNum % columnCount) * 2);

                grid.Children.Add(uia.InputControl);
                Grid.SetRow(uia.InputControl, controlsNum / columnCount);
                Grid.SetColumn(uia.InputControl, (controlsNum % columnCount) * 2 + 1);

                if (uia.DisplayName == "备注")
                {
                    Grid.SetColumnSpan(uia.InputControl, columnCount * 2 - 1);
                    uia.InputControl.Height = 45;
                    uia.InputControl.Margin = new Thickness(0, 2, 5, 2);
                }
                controlsNum++;
            }

            ScrollViewer sv = new ScrollViewer
            {
                HorizontalScrollBarVisibility = ScrollBarVisibility.Auto,
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto,
                Content = grid
            };
            gridmaster.Children.Add(sv);

            return props;
        }

        #endregion
        /// <summary>
        /// 自动生成“添加”界面
        /// modify3 add：DateTime Code(2012/7/12)
        /// modify2 add：ValidatesOnDataErrors = true（2012/7/10）        
        /// modify by qinyy(2012/7/5)：增加了滚动条
        /// </summary>
        /// <param name="type"></param>
        /// <param name="gridmaster"></param>
        /// <param name="columnCount"></param>
        /// <param name="keyDisabled"></param>
        /// <returns></returns>
        public static List<AttrDataPropertyAttribute> AutoGenerateDialogGridContent(Type type, Grid gridmaster, int columnCount, bool keyDisabled)
        {
            List<AttrDataPropertyAttribute> props = new List<AttrDataPropertyAttribute>();
            props = AutoGenerateDialogGridContent(type, props);

            props = AutoGenerateDialogLayoutPanel(props, gridmaster, columnCount, keyDisabled);

            return props;
        }

        static void ATextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            (sender as TextBox).SelectAll();
        }

        public static void UIGroupDatagridInit(Type type, DataGrid datagrid, params IEnumerable[] datasourceList)
        {
            datagrid.Columns.Clear();


            int dataSourceIndex = 0;

            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    if (!a.IsNotDatagridColumn)
                    {
                        a.PropertyName = info.Name;
                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                        {
                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName),
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                        {
                            DataGridComboBoxColumn cbxColumn = new DataGridComboBoxColumn()
                            {
                                Header = a.DisplayName,
                                IsReadOnly = true,
                                SelectedValuePath = "Key",
                                DisplayMemberPath = "Value",

                                SelectedValueBinding = new Binding(a.PropertyName),
                                ItemsSource = datasourceList[dataSourceIndex++],
                                CanUserReorder = true,

                            };

                            if (a.IsCellCentered)
                                cbxColumn.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            //BindingOperations.SetBinding(cbxColumn, 
                            //    DataGridComboBoxColumn.ItemsSourceProperty, 
                            //    new Binding(a.ComboDatasource)); 
                            datagrid.Columns.Add(cbxColumn);

                        }
                    }
                }
            }
        }

        /// <summary>
        /// add by qinyy(2012-7-2)
        /// </summary>
        /// <param name="type"></param>
        /// <param name="datagrid"></param>
        /// <param name="datasourceList"></param>
        public static void UIGroupDatagridInit(Type type, DataGrid datagrid)
        {
            datagrid.Columns.Clear();
            //int dataSourceIndex = 0;

            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    if (!a.IsNotDatagridColumn)
                    {
                        a.PropertyName = info.Name;
                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                        {
                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName),
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);
                        }
                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                        {
                            ////DataGridComboBoxColumn cbxColumn = new DataGridComboBoxColumn()
                            ////{
                            ////    Header = a.DisplayName,
                            ////    IsReadOnly = true,
                            ////    DisplayMemberPath = "Display",
                            ////    SelectedValuePath = "Value",
                            ////    SelectedValueBinding = new Binding(a.PropertyName),
                            ////    ItemsSource = datasourceList[dataSourceIndex++],
                            ////    CanUserReorder = true,
                            ////};

                            DataGridTextColumn tbx = new DataGridTextColumn()
                            {
                                Header = a.DisplayName,
                                Binding = new System.Windows.Data.Binding(a.PropertyName),
                                IsReadOnly = true,
                                CanUserReorder = true,
                            };

                            if (a.IsCellCentered)
                                tbx.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            datagrid.Columns.Add(tbx);

                            ////if (a.IsCellCentered)
                            ////    cbxColumn.CellStyle = (Style)Application.Current.Resources["DataGridVcenterHcenter"];

                            //BindingOperations.SetBinding(cbxColumn, 
                            //    DataGridComboBoxColumn.ItemsSourceProperty, 
                            //    new Binding(a.ComboDatasource)); 
                            ////datagrid.Columns.Add(cbxColumn);

                        }
                    }
                }
            }
        }

        /// <summary>
        /// 自动生成GroupBox控件，含DataGrid...
        /// modify by qinyy(2012/09/01):
        /// set GroupBox.Foreground = Color(0,0,0)
        /// modify by qinyy(2012/10/11):
        /// add datasourceList
        /// </summary>
        /// <param name="type"></param>
        /// <param name="grid"></param>
        /// <param name="columnCount"></param>
        /// <returns></returns>
        public static IList AutoGenerateGroupGridContent(Type type, Grid grid, int columnCount, params IEnumerable[] datasourceList)
        {
            int groupCount = 0;

            List<AttrDataPropertyAttribute> attrs = new List<AttrDataPropertyAttribute>();
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    a.PropertyName = info.Name;
                    a.PropertyType = info.PropertyType.Name;

                    attrs.Add(a);
                }
            }
            //按groupbox分组
            var groupedAttrs = from d in attrs group d by d.GroupBoxName;

            foreach (var item in groupedAttrs)
            {
                //分组增加groupbox
                grid.RowDefinitions.Add(new RowDefinition()
                {
                    Height = item.Any(d => d.InputType == UI_INPUT_TYPE.DATASET) ?
                     new GridLength(1, GridUnitType.Star) :
                     new GridLength(1, GridUnitType.Auto)
                    //Height = new GridLength(1, GridUnitType.Star)
                });
            }

            foreach (var item in groupedAttrs)
            {
                GroupBox groupBox = new GroupBox()
                {
                    Header = item.Key,
                    Foreground = new SolidColorBrush(Color.FromRgb(0, 0, 0)),
                    Padding = new Thickness(2)
                };

                //分组内容
                if (item.Count() > 1) //多输入
                {
                    #region 多输入
                    //Grid gridInGroupBox = new Grid();
                    WrapPanel wrp = new WrapPanel();

                    //for (int i = 0; i < columnCount; i++)  //columns
                    //{
                    //    gridInGroupBox.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                    //}
                    //for (int i = 0; i < (item.Count() + columnCount) / columnCount + 1; i++) //rows
                    //{
                    //    gridInGroupBox.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });
                    //}

                    for (int i = 0; i < item.Count(); i++)
                    {
                        StackPanel stackPanelItem = AutoGenerateStackPanel(item.ElementAt(i));
                        wrp.Children.Add(stackPanelItem);
                        Grid.SetRow(stackPanelItem, (i + columnCount) / columnCount);
                        Grid.SetColumn(stackPanelItem, i % columnCount);

                    }

                    groupBox.Content = wrp;
                    #endregion
                }
                else //1单个元素
                {
                    if (item.Any(d => d.InputType == UI_INPUT_TYPE.DATASET))
                    {
                        ParamDataGrid groupDatagrid = new ParamDataGrid() { VerticalAlignment = VerticalAlignment.Stretch, IsEnabled = true };
                        if (item.Any(d => d.IsDataGridGroup == true)) //datagrid with group
                        {
                            groupDatagrid.SetGroupStyle();
                        }

                        // modify(2012/10/11)
                        if (datasourceList.Length > 0)
                            UIGroupDatagridInit(item.ElementAt(0).DataGridItemSourceType, groupDatagrid, datasourceList);
                        else
                            UIGroupDatagridInit(item.ElementAt(0).DataGridItemSourceType, groupDatagrid);

                        item.ElementAt(0).InputControl = groupDatagrid;
                        groupDatagrid.SetBinding(DataGrid.ItemsSourceProperty, item.ElementAt(0).PropertyName);
                        groupDatagrid.Tag = item.ElementAt(0).PropertyName;
                        groupBox.Content = groupDatagrid;

                    }
                    else //非DataGrid输入类型
                    {
                        groupBox.Content = AutoGenerateStackPanel(item.ElementAt(0));
                    }
                }
                grid.Children.Add(groupBox);
                Grid.SetRow(groupBox, groupCount++);
            }
            return attrs;
        }

        /// <summary>
        /// 自动生成StackPanel控件，含：Label和TEXTBOX/COMBOBOX/DATE...
        /// </summary>
        /// <param name="attr"></param>
        /// <returns></returns>
        public static StackPanel AutoGenerateStackPanel(AttrDataPropertyAttribute attr)
        {
            StackPanel dp = new StackPanel()
            {
                Orientation = Orientation.Horizontal,
                HorizontalAlignment = HorizontalAlignment.Left,
                Margin = new Thickness(0, 2, 0, 2)
            };

            Label lbl = new Label()
            {
                Content = attr.DisplayName + ":",
                Margin = new Thickness(5, 0, 0, 0),
                VerticalContentAlignment = VerticalAlignment.Center,
                HorizontalContentAlignment = HorizontalAlignment.Right,
                MinWidth = 150
            };
            dp.Width = UI_LAYOUT.INPUT_QUERY_DOCKPANEL_WIDTH;
            dp.Children.Add(lbl);

            switch (attr.InputType)
            {
                case UI_INPUT_TYPE.TEXTBOX:
                    {
                        #region TEXTBOX

                        TextBox tbx = new TextBox
                        {
                            Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH
                        };
                        if (attr.IsNotUpdateItem)
                        {
                            tbx.IsEnabled = false;
                        }
                        tbx.VerticalContentAlignment = VerticalAlignment.Center;
                        tbx.Style = (Style)Application.Current.Resources["textBoxInError"];
                        tbx.SetBinding(TextBox.TextProperty, new Binding(attr.PropertyName)
                        {
                            ValidatesOnDataErrors = true,
                            UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                        });

                        tbx.GotFocus += ATextBox_GotFocus;

                        dp.Children.Add(tbx);

                        break;
                        #endregion
                    }
                case UI_INPUT_TYPE.COMBOBOX:
                    {
                        #region COMBOBOX

                        ComboBox cbx = new ComboBox
                        {
                            VerticalContentAlignment = VerticalAlignment.Center
                        };
                        cbx.SetBinding(ComboBox.ItemsSourceProperty, new Binding(attr.ComboDatasource)
                        {
                            // ValidatesOnDataErrors = false,
                            UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                        });
                        cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(attr.PropertyName)
                        {
                            ValidatesOnDataErrors = false,
                            UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                            Mode = BindingMode.TwoWay
                        });

                        dp.Children.Add(cbx);

                        break;
                        #endregion
                    }
                case UI_INPUT_TYPE.DATE:
                    {
                        #region DATE

                        DatePicker myDatePicker = new DatePicker
                        {
                            Width = UI_LAYOUT.INPUT_QUERY_CONTROL_WIDTH
                        };
                        myDatePicker.SetBinding(DatePicker.SelectedDateProperty, new Binding(attr.PropertyName)
                        {
                            UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                            Mode = BindingMode.TwoWay
                        });
                        myDatePicker.VerticalContentAlignment = VerticalAlignment.Center;

                        dp.Children.Add(myDatePicker);

                        break;
                        #endregion
                    }
                case UI_INPUT_TYPE.DATETIME:
                    {
                        #region DATETIME

                        Xceed.Wpf.Toolkit.DateTimePicker mydtPicker = new Xceed.Wpf.Toolkit.DateTimePicker
                        {
                            Format = Xceed.Wpf.Toolkit.DateTimeFormat.Custom,
                            FormatString = "yyyy-MM-dd HH:mm:ss"
                        };

                        mydtPicker.SetBinding(Xceed.Wpf.Toolkit.DateTimePicker.ValueProperty, new Binding(attr.PropertyName)
                        {
                            UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                            Mode = BindingMode.TwoWay
                        });

                        dp.Children.Add(mydtPicker);

                        break;
                        #endregion
                    }

                default:
                    {

                        break;
                    }
            }
            return dp;
        }

        /// <summary>
        /// 字段验证
        /// </summary>
        /// <param name="prop"></param>
        /// <returns></returns>
        public static bool ValidateInput(List<AttrDataPropertyAttribute> prop)
        {
            if (prop != null)
            {
                var v = prop.FirstOrDefault(d => d.InputControl.ToolTip != null);
                if (v!= null)
                {
                    MessageBox.Show(string.Format("输入项【{0}】\n{1}", v.DisplayName, v.InputControl.ToolTip.ToString()));
                    v.InputControl.Focus();
                    return false;
                }
            }

            return true;
        }

        public static void RegisterCommand(UserControl ucBind, Button btnSource, ICommand cmd)
        {
            CommandBinding cb = new CommandBinding(cmd);
            ucBind.CommandBindings.Add(cb);
            btnSource.Command = cmd;
        }

        public static IList AutoGenerateDataInput(Type type, Grid grid, int columnCount, List<Control> controls, KeyEventHandler keyHandler,bool keyDisabled=false)
        {

            System.Collections.IList props = new List<AttrDataPropertyAttribute>();
            int controlsNum = 0;
            foreach (PropertyInfo info in GetProps(type))
            {
                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                {
                    if (a.IsNotOperateItem) continue;


                    a.PropertyName = info.Name;
                    a.PropertyType = info.PropertyType.Name;


                    controlsNum++;

                    a.DisplayLable = new Label()
                    {
                        Content = a.DisplayName + "(_" + controlsNum + "):",
                        Margin = new Thickness(5, 0, 0, 0),
                        VerticalContentAlignment = VerticalAlignment.Center,
                        HorizontalContentAlignment = HorizontalAlignment.Right
                    };

                    switch (a.InputType)
                    {
                        case UI_INPUT_TYPE.TEXTBOX:
                            TextBox aTextBox = new TextBox();
                            if (a.DisplayName != "备注")
                            {
                                aTextBox.Style = (Style)Application.Current.Resources["styleInputControl2"];
                                aTextBox.TextWrapping = TextWrapping.Wrap;
                                aTextBox.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
                            }
                            else
                                aTextBox.Style = (Style)Application.Current.Resources["styleInputControl"];

                            if (a.DataConvertType == DataConverterType.Currency)
                            {
                                aTextBox.SetBinding(TextBox.TextProperty, new Binding(a.PropertyName)
                                {
                                    ValidatesOnDataErrors = true,
                                    StringFormat = "{0:0.00}",
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                                });

                            }

                            else
                            {
                                aTextBox.SetBinding(TextBox.TextProperty, new Binding(a.PropertyName)
                                {
                                    ValidatesOnDataErrors = true,
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                                });

                            }
                            aTextBox.GotFocus += ATextBox_GotFocus;
                            //aTextBox_GotFocus;

                            a.InputControl = aTextBox;

                            break;
                        case UI_INPUT_TYPE.IP:
                            TextBox ipTextBox = new TextBox
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"]
                            };

                            ipTextBox.SetBinding(TextBox.TextProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                //UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });

                            ipTextBox.GotFocus += ATextBox_GotFocus;
                            //aTextBox_GotFocus;

                            a.InputControl = ipTextBox;

                            break;
                        case UI_INPUT_TYPE.PASSWORDBOX:
                            PasswordBox aPasswordBox = new PasswordBox
                            {
                                //aPasswordBox.Style = (Style)Application.Current.Resources["styleInputControl2"];
                                Width = 160,
                                Height = 25,
                                VerticalAlignment = VerticalAlignment.Center,
                                VerticalContentAlignment = VerticalAlignment.Center,
                                Margin = new Thickness(0, 5, 5, 5)
                            };
                            PasswordBoxHelper.SetAttach(aPasswordBox, true);


                            aPasswordBox.SetBinding(PasswordBoxHelper.PasswordProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                Mode = BindingMode.TwoWay,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });

                            a.InputControl = aPasswordBox;
                            break;
                        case UI_INPUT_TYPE.COMBOBOX:
                            ComboBox cbx = new ComboBox()
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"],
                                Focusable = true,
                                SelectedValuePath = "Key",
                                DisplayMemberPath = "Value",
                                // IsEditable = true
                            };

                            if (a.IsEditable)
                            {
                                cbx.IsEditable = true;
                            }

                            //Combox DatasourceItems定义参见ValueDisplayPairXXXX
                            cbx.SetBinding(ComboBox.ItemsSourceProperty, new Binding(a.ComboDatasource)
                            {
                                ValidatesOnDataErrors = true,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
                            });
                            cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = true,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                Mode = BindingMode.TwoWay
                            });
                            a.InputControl = cbx;
                            break;
                        case UI_INPUT_TYPE.COLORCOMBOBOX:
                            ComboBox ccbx = new ComboBox
                            {
                                Style = (Style)Application.Current.Resources["styleInputControl"],
                                SelectedValuePath = "ColorName",
                                ItemTemplate = (DataTemplate)Application.Current.Resources["dtColor"],

                                ItemsSource = (new ItemList()).GetListOfColorName
                            };
                            ccbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(a.PropertyName)
                            {
                                ValidatesOnDataErrors = false,
                                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                Mode = BindingMode.TwoWay
                            });
                            a.InputControl = ccbx;
                            break;
                        case UI_INPUT_TYPE.DATE:
                            {
                                DatePicker aDatePicker = new DatePicker
                                {
                                    Style = (Style)Application.Current.Resources["styleInputControl"]
                                };
                                aDatePicker.SetBinding(DatePicker.SelectedDateProperty,
                                    new Binding(a.PropertyName) { ValidatesOnDataErrors = true, UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
                                aDatePicker.SelectedDateFormat = DatePickerFormat.Short;
                                a.InputControl = aDatePicker;
                                break;
                            }
                        case UI_INPUT_TYPE.DATETIME:
                            {
                                Xceed.Wpf.Toolkit.DateTimePicker dtPicker = new Xceed.Wpf.Toolkit.DateTimePicker
                                {
                                    Format = Xceed.Wpf.Toolkit.DateTimeFormat.Custom,
                                    FormatString = "yyyy-MM-dd HH:mm:ss",
                                    Style = (Style)Application.Current.Resources["styleInputControl"]
                                };
                                dtPicker.SetBinding(Xceed.Wpf.Toolkit.DateTimePicker.ValueProperty, new Binding(a.PropertyName)
                                {
                                    UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                                    Mode = BindingMode.TwoWay
                                });

                                a.InputControl = dtPicker;
                                break;
                            }
                        case UI_INPUT_TYPE.CHECKBOX:
                            CheckBox aCheckBox = new CheckBox() { VerticalAlignment = VerticalAlignment.Center };
                            //aCheckBox.Style = (Style)Application.Current.Resources["styleInputControl"];
                            aCheckBox.SetBinding(CheckBox.IsCheckedProperty,
                                new Binding(a.PropertyName) { ValidatesOnDataErrors = false, UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
                            a.InputControl = aCheckBox;

                            break;
                        default:
                            break;
                    }

                    if (a.Disabled)
                    {
                        a.InputControl.IsEnabled = false;
                    }
                    if (a.IsEnterKey)
                    {
                        controls.Insert(controls.Count - 1, a.InputControl);
                        a.InputControl.KeyUp += keyHandler;
                    }
                    a.DisplayLable.SetBinding(Label.TargetProperty, new System.Windows.Data.Binding() { Source = a.InputControl });//快捷访问绑定
                    props.Add(a);

                }
            }

            //columnCount
            for (int i = 0; i < columnCount; i++) //create grid
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
                grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) });
            }

            //grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            for (int i = 0; i < props.Count / columnCount + 1; i++)
            {
                grid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            }
            grid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            controlsNum = 0;
            foreach (var item in props)
            {
                AttrDataPropertyAttribute uia = item as AttrDataPropertyAttribute;
                if (keyDisabled)
                {
                    if (uia.IsNotUpdateItem)
                    {
                        uia.InputControl.IsEnabled = false;
                    }
                }
                if (uia.DisplayName == "备注")
                {
                    while (controlsNum % columnCount != 0)
                    {
                        controlsNum++;
                    }
                }

                grid.Children.Add(uia.DisplayLable);
                Grid.SetRow(uia.DisplayLable, controlsNum / columnCount);
                Grid.SetColumn(uia.DisplayLable, (controlsNum % columnCount) * 2);

                grid.Children.Add(uia.InputControl);
                Grid.SetRow(uia.InputControl, controlsNum / columnCount);
                Grid.SetColumn(uia.InputControl, (controlsNum % columnCount) * 2 + 1);

                if (uia.DisplayName == "备注")
                {
                    Grid.SetColumnSpan(uia.InputControl, columnCount * 2 - 1);
                    uia.InputControl.Height = 45;
                    uia.InputControl.Margin = new Thickness(0, 2, 5, 2);
                }
                controlsNum++;
            }


            return props;
        }

        public static void CreateComlumnInDataGrid(DataGrid dg, string strHeader, string strBind)
        {
            DataGridTextColumn tbx = new DataGridTextColumn()
            {
                Header = strHeader,
                Binding = new System.Windows.Data.Binding(strBind),
                IsReadOnly = true,
                CanUserReorder = true,
            };
            dg.Columns.Add(tbx);
        }
    }
}
