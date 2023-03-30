//#define WinOFFICE
using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using HorizontalAlignment = System.Windows.HorizontalAlignment;
using Path = System.Windows.Shapes.Path;

namespace HHJT.AFC.Framework.UI
{
    #region
    /// <summary>
    /// 查询类布局
    /// </summary>
    public class UCLayoutQuery : UserControl
    {
        protected DockPanel PanelRoot;
        protected WrapPanel PanelQuery;
        protected Button BtnReset;
        protected Button BtnQuery;
        protected CheckedGroupDataGird DataGridContent;
        protected object CurrentItem;

        public UCLayoutQuery()
            : base()
        {
            Foreground = Brushes.Black;
            Background = Brushes.White;
            InitControls();
        }

        protected void InitControls()
        {
            //1查询区域
            #region 1
            GroupBox groupboxQuery = new GroupBox() { Header = "查询区域" };
            DockPanel dockpanelQuery = new DockPanel();
            //1.1查询重置按钮 Grid
            Grid gridQuery = new Grid();
            #region layout
            gridQuery.RowDefinitions.Add(new RowDefinition());
            gridQuery.RowDefinitions.Add(new RowDefinition());

            this.BtnReset = new Button()
            {
                Content = "重置(_R)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                //Tag = "R"
            };
            this.BtnQuery = new Button()
            {
                Content = "查询(_Q)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                //Tag = "Q"
            };

            // BtnQuery.Click += new RoutedEventHandler(Btn_Click);
            // BtnReset.Click += new RoutedEventHandler(Btn_Click);


            gridQuery.Children.Add(this.BtnReset);
            gridQuery.Children.Add(this.BtnQuery);
            Grid.SetRow(this.BtnReset, 0);
            Grid.SetRow(this.BtnQuery, 1);
            #endregion

            dockpanelQuery.Children.Add(gridQuery);
            DockPanel.SetDock(gridQuery, Dock.Right);
            //1.2 border
            Border borderQuerySplit = new Border()
            {
                Margin = new Thickness(5, -5, 5, -5),
                BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
                BorderThickness = new Thickness(0.5)
            };
            dockpanelQuery.Children.Add(borderQuerySplit);
            DockPanel.SetDock(borderQuerySplit, Dock.Right);
            //1.3
            PanelQuery = new WrapPanel();
            dockpanelQuery.Children.Add(PanelQuery);
            groupboxQuery.Content = dockpanelQuery;
            //end 1
            #endregion

            //2数据区域
            Border borderData = new Border()
            {
                Margin = new Thickness(2, 5, 5, 2),
                Padding = new Thickness(5),
                BorderBrush = new SolidColorBrush(Colors.Teal),
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(3)
            };

            DataGridContent = new CheckedGroupDataGird();
            DataGridContent.SelectionChanged += new SelectionChangedEventHandler(DataGridContent_SelectionChanged);

            _datagridContextMenu = new System.Windows.Controls.ContextMenu();
            MenuItem itemExport = new MenuItem() { Header = "数据导出" };
            itemExport.Click += ItemExport_Click;
            _datagridContextMenu.Items.Add(itemExport);
            DataGridContent.ContextMenu = _datagridContextMenu;

            borderData.Child = DataGridContent;

            PanelRoot = new DockPanel();
            PanelRoot.Children.Add(groupboxQuery);
            DockPanel.SetDock(groupboxQuery, Dock.Top);

            FurtherInitLayout();//由派生类自行定义完成

            PanelRoot.Children.Add(borderData);

            this.Content = PanelRoot;
        }
        
        void ItemExport_Click(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            string strStartDateTime = DateTime.Now.ToString("yyyyMMdd");

            if (MessageBox.Show("确认导出查询的记录？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) ==
                MessageBoxResult.OK)
            {
                try
                {
                    string saveFileName = "";
#if WinOFFICE
                    #region WindowsOffice导出(需要安装office)
                    Microsoft.Win32.SaveFileDialog saveDialog = new Microsoft.Win32.SaveFileDialog();
                    saveDialog.DefaultExt = "xlsx";
                    saveDialog.Filter = "Excel文件|*.xlsx";
                    saveDialog.FileName = "数据导出" + strStartDateTime;
                    saveDialog.ShowDialog();
                    saveFileName = saveDialog.FileName;
                    if (saveFileName.IndexOf(":") < 0) return; //被点了取消

                    //创建Excel  

                    Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
                    Excel.Workbook excelWB = excelApp.Workbooks.Add(System.Type.Missing); //创建工作簿（WorkBook：即Excel文件主体本身）  

                    int currentSheet = 1;
                    int currentRow = 2;
                    Excel.Worksheet excelWS = (Excel.Worksheet)excelWB.Worksheets[currentSheet];
                    excelWS.Cells.NumberFormat = "@";
                    //excelWS = (Worksheet)excelWB.Worksheets[i];   //创建工作表（即Excel里的子表sheet） 1表示在子表sheet1里进行数据导出  

                    ////excelWS.Cells.NumberFormat = "@";     //  如果数据中存在数字类型 可以让它变文本格式显示  
                    ////将数据导入到工作表的单元格  

                    #region cells
                    int cCount = 0;
                    for (int c = 0; c < DataGridContent.Columns.Count; c++)
                    {
                        if (!(DataGridContent.Columns[c].Header is CheckBox))
                        {
                            cCount++;
                            excelWS.Cells.Cells[1, cCount] = DataGridContent.Columns[c].Header;
                        }

                    }
                    if (DataGridContent.Items != null)
                    {
                        for (int r = 0; r < DataGridContent.Items.Count; r++)
                        {
                            int currentCol = 1;
                            var props = AutoUI.GetProps(DataGridContent.Items[r].GetType());
                            //PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(DataGridContent.Items[r].GetType());
                            foreach (PropertyInfo info in props)
                            {
                                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                                {
                                    //PropertyDescriptor itemProperty = properties.Find(info.Name, false);
                                    if (!a.IsNotDatagridColumn)
                                    {
                                        //var vv = itemProperty.GetValue(DataGridContent.Items[r]);
                                        var vvv =
                                            DataGridContent.Columns.Count(
                                                d =>
                                                !(d.Header is CheckBox) && d.Header.ToString().Equals(a.DisplayName));
                                        if (vvv==0)
                                        {
                                            continue;
                                        }
                                        var vv = info.GetValue(DataGridContent.Items[r]);
                                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                                        {
                                            
                                            if (a.DataConvertType == DataConverterType.Currency)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? string.Format("{0:0.00}", vv.ToString()) : "";//!=null?"@"+vv.ToString():"";
                                            }
                                            else if (a.DataConvertType == DataConverterType.DateTime)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ?((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss"):"" ;//!=null?"@"+vv.ToString():"";
                                            }
                                            else if (a.DataConvertType == DataConverterType.Date)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ?((DateTime)vv).ToString("yyyy-MM-dd"):"";//!=null?"@"+vv.ToString():"";
                                            }
                                            else
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv;//!=null?"@"+vv.ToString():"";
                                            }
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                                        {
                                            excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "";

                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATE)
                                        {
                                            excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "";

                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                                        {
                                            //PropertyDescriptor cbxSourceProp = properties.Find(a.ComboDatasource, false);
                                            var cbxSourceProp=props.FirstOrDefault(d => d.Name.Equals(a.ComboDatasource));
                                            var cbxSource = cbxSourceProp.GetValue(DataGridContent.Items[r]);
                                            string str = "";
                                            if (cbxSource is KeyValuePair<int,string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<int, string>[]).FirstOrDefault(d => d.Key == (int)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<int, string>))? itemGet.Value : "";
                                            }
                                            else if (cbxSource is KeyValuePair<string, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<string, string>[]).FirstOrDefault(d => d.Key == (string)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<string, string>)) ? itemGet.Value : "";
                                            }

                                            excelWS.Cells.Cells[r + 2, currentCol] = str;
                                        }

                                        currentCol++;
                                    }

                                }
                            }
                        }
                    }

                    excelWS.Columns.AutoFit();


                    #endregion

                    excelWB.SaveAs(saveFileName); //保存到指定的路径  
                    excelWB.Close();
                    excelApp.Quit(); //KillAllExcel(excelApp); 释放可能还没释放的进程  
                    #endregion
#else
                    #region 不需要WindowsOffice导出
                    Microsoft.Win32.SaveFileDialog saveDialog = new Microsoft.Win32.SaveFileDialog
                    {
                        DefaultExt = "xls",
                        Filter = "Excel文件|*.xls",
                        FileName = "数据导出" + strStartDateTime
                    };
                    saveDialog.ShowDialog();
                    saveFileName = saveDialog.FileName;
                    if (saveFileName.IndexOf(":") < 0) return; //被点了取消

                    //创建Excel  
                    HSSFWorkbook hssfWorkbook = new HSSFWorkbook();

                    ISheet sheet1 = hssfWorkbook.CreateSheet("Sheet1");
                    IRow row = sheet1.CreateRow(0);
                    int cCount = 0;
                    for (int i = 0; i < DataGridContent.Columns.Count; i++)
                    {
                        if (!(DataGridContent.Columns[i].Header is CheckBox))
                        {
                            cCount++;
                            ICell cell = row.CreateCell(i);
                            cell.SetCellValue(DataGridContent.Columns[i].Header.ToString());
                        }
                    }
                    if (DataGridContent.Items != null)
                    {
                        for (int j = 2; j <= DataGridContent.Items.Count + 1; j++)
                        {
                            #region data
                            row = sheet1.CreateRow(j - 1);
                            int i = 0;
                            ICell cell = null;

                            var props = AutoUI.GetProps(DataGridContent.Items[j - 2].GetType());
                            //PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(DataGridContent.Items[r].GetType());
                            foreach (PropertyInfo info in props)
                            {
                                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                                {
                                    //PropertyDescriptor itemProperty = properties.Find(info.Name, false);
                                    if (!a.IsNotDatagridColumn)
                                    {
                                        //var vv = itemProperty.GetValue(DataGridContent.Items[r]);
                                        var vvv =
                                            DataGridContent.Columns.Count(
                                                d =>
                                                !(d.Header is CheckBox) && d.Header.ToString().Equals(a.DisplayName));
                                        if (vvv==0)
                                        {
                                            continue;
                                        }
                                        var vv = info.GetValue(DataGridContent.Items[j - 2]);
                                        cell=row.CreateCell(i++);
                                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                                        {

                                            if (a.DataConvertType == DataConverterType.Currency)
                                            {
                                                cell.SetCellValue(vv != null ? string.Format("{0:0.00}", vv.ToString()) : "");
                                            }
                                            else if (a.DataConvertType == DataConverterType.DateTime)
                                            {
                                                cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "");
                                            }
                                            else if (a.DataConvertType == DataConverterType.Date)
                                            {
                                                cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "");
                                            }
                                            else
                                            {
                                                cell.SetCellValue(vv != null ?vv.ToString(): "");
                                            }
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                                        {
                                            cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "");
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATE)
                                        {
                                            cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "");
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                                        {
                                            //PropertyDescriptor cbxSourceProp = properties.Find(a.ComboDatasource, false);
                                            var cbxSourceProp = props.FirstOrDefault(d => d.Name.Equals(a.ComboDatasource));
                                            var cbxSource = cbxSourceProp.GetValue(DataGridContent.Items[j-2]);
                                            string str = "";
                                            if (cbxSource is KeyValuePair<int, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<int, string>[]).FirstOrDefault(d => d.Key == (int)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<int, string>)) ? itemGet.Value : "";
                                            }
                                            else if (cbxSource is KeyValuePair<string, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<string, string>[]).FirstOrDefault(d => d.Key == (string)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<string, string>)) ? itemGet.Value : "";
                                            }
                                            cell.SetCellValue(str);
                                        }
                                    }
                                }
                            }
                            #endregion
                        }
                    }

                    for (int i = 0; i < cCount; i++)
                    {
                        sheet1.AutoSizeColumn(i);
                    }
                    #region cells

                    #endregion
                    FileStream file = new FileStream(saveFileName, FileMode.Create);
                    hssfWorkbook.Write(file);
                    file.Close();

                    #endregion
                    #endif

                    MessageBox.Show("导出成功！");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("错误：" + ex.Message);
                }
            }
        }

        void DataGridContent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CurrentItem = DataGridContent.SelectedItem;
            CurrentSelectonChanged();
        }

        public virtual void CurrentSelectonChanged()
        {

        }

        protected virtual void FurtherInitLayout() { }
        protected ContextMenu _datagridContextMenu;
        protected void RegisterCommand(UserControl ucBind, Button btnSource, ICommand cmd)
        {
            CommandBinding cb = new CommandBinding(cmd);
            this.CommandBindings.Add(cb);
            btnSource.Command = cmd;
        }

        protected void RegisterButtonAndContextMenu(UserControl ucBind, AttrMethodAttribute[] props, ICommand[] cmds)
        {
            //DataGridContent.ContextMenu = new ContextMenu();
            _datagridContextMenu.Items.Add(new Separator());
            for (int i = 0; i < props.Count(); i++)
            {
                CommandBinding cb = new CommandBinding(cmds[i]);
                this.CommandBindings.Add(cb);
                props[i].ButtonCtrl.Command = cmds[i];

                DataGridContent.ContextMenu.Items.Add(
                    new MenuItem()
                    {
                        Header = props[i].FunctionDisplayName,
                        Command = cmds[i]
                    }
                    );
            }
        }

        //protected void RegisterContextMunu()
        //{

        //}

        protected ComboBox CreateCombox(string path, string selectValue, string displayMember)
        {
            ComboBox cbx = new ComboBox()
            {
                Style = (Style)Application.Current.Resources["styleInputControl"],
                Focusable = true,
                SelectedValuePath = path,
                DisplayMemberPath = displayMember,
                IsEditable = true
            };

            cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(selectValue)
            {
                ValidatesOnDataErrors = false,
                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                Mode = BindingMode.TwoWay
            });
            return cbx;
        }
    }

    //操作区域
    public class UCLayoutQueryProcess : UCLayoutQuery
    {
        protected StackPanel PanelProcess;

        protected override void FurtherInitLayout()
        {
            base.FurtherInitLayout();
            GroupBox groupboxProcess = new GroupBox() { Header = "操作区域" };
            PanelProcess = new StackPanel();
            groupboxProcess.Content = PanelProcess;

            PanelRoot.Children.Add(groupboxProcess);
            DockPanel.SetDock(groupboxProcess, Dock.Right);
        }
    }

    //选择区域
    public class UCLayoutQueryProcessSelect : UCLayoutQueryProcess
    {
        protected GroupBox GroupBoxSelect;

        protected override void FurtherInitLayout()
        {
            base.FurtherInitLayout();

            GroupBoxSelect = new GroupBox() { Header = "选择区域" };

            PanelRoot.Children.Add(GroupBoxSelect);
            DockPanel.SetDock(GroupBoxSelect, Dock.Left);
        }
    }

    //选择区域（无操作区域）
    public class UCLayoutQuerySelect : UCLayoutQuery
    {
        protected GroupBox GroupBoxSelect;

        protected override void FurtherInitLayout()
        {
            GroupBoxSelect = new GroupBox() { Header = "选择区域" };

            PanelRoot.Children.Add(GroupBoxSelect);
            DockPanel.SetDock(GroupBoxSelect, Dock.Left);
        }
    }

    //操作区域（无查询区域）
    public class UCLayoutProcess : UserControl
    {
        protected DockPanel PanelRoot;
        protected CheckedGroupDataGird DataGridContent;
        protected object CurrentItem;
        protected StackPanel PanelProcess;
        public UCLayoutProcess()
            : base()
        {
            Foreground = Brushes.Black;
            InitControls();
        }

        protected void InitControls()
        {
            //数据区域
            Border borderData = new Border()
            {
                Margin = new Thickness(2, 5, 5, 2),
                Padding = new Thickness(5),
                BorderBrush = new SolidColorBrush(Colors.Teal),
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(3)
            };

            DataGridContent = new CheckedGroupDataGird();
            DataGridContent.SelectionChanged += new SelectionChangedEventHandler(DataGridContent_SelectionChanged);
            borderData.Child = DataGridContent;

            //操作区域
            GroupBox groupboxProcess = new GroupBox() { Header = "操作区域" };
            PanelProcess = new StackPanel();
            groupboxProcess.Content = PanelProcess;

            PanelRoot = new DockPanel();
            PanelRoot.Children.Add(groupboxProcess);
            DockPanel.SetDock(groupboxProcess, Dock.Right);

            FurtherInitLayout();//由派生类自行定义完成

            PanelRoot.Children.Add(borderData);

            this.Content = PanelRoot;
        }

        void DataGridContent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CurrentItem = DataGridContent.SelectedItem;
            CurrentSelectonChanged();
        }

        public virtual void CurrentSelectonChanged()
        {

        }

        protected virtual void FurtherInitLayout() { }
    }

    
    //选择区域翻页（无操作区域）
    #region
    /// <summary>
    /// 查询类(分页)布局
    /// </summary>
    public class UCLayoutQueryPagging : UserControl
    {
        protected DockPanel PanelRoot;
        protected WrapPanel PanelQuery;
        protected Button BtnReset;
        protected Button BtnQuery;
        protected CheckedGroupDataGird DataGridContent;
        protected UIDataGirdPagging ControlPagging;
        protected Object CurrentItem;

        public UCLayoutQueryPagging()
            : base()
        {
            Foreground = Brushes.Black;
            Background = Brushes.White;
            InitControls();
        }

        protected void InitControls()
        {
            //1查询区域
            #region 1
            GroupBox groupboxQuery = new GroupBox() { Header = "查询区域" };
            DockPanel dockpanelQuery = new DockPanel();
            //1.1查询重置按钮 Grid
            Grid gridQuery = new Grid();
            #region layout
            gridQuery.RowDefinitions.Add(new RowDefinition());
            gridQuery.RowDefinitions.Add(new RowDefinition());

            this.BtnReset = new Button()
            {
                Content = "重置(_R)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                //Tag = "R"
            };
            this.BtnQuery = new Button()
            {
                Content = "查询(_Q)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                //Tag = "Q"
            };

            // BtnQuery.Click += new RoutedEventHandler(Btn_Click);
            // BtnReset.Click += new RoutedEventHandler(Btn_Click);


            gridQuery.Children.Add(this.BtnReset);
            gridQuery.Children.Add(this.BtnQuery);
            Grid.SetRow(this.BtnReset, 0);
            Grid.SetRow(this.BtnQuery, 1);
            #endregion

            dockpanelQuery.Children.Add(gridQuery);
            DockPanel.SetDock(gridQuery, Dock.Right);
            //1.2 border
            Border borderQuerySplit = new Border()
            {
                Margin = new Thickness(5, -5, 5, -5),
                BorderBrush = new SolidColorBrush(Color.FromRgb(213, 233, 229)),
                BorderThickness = new Thickness(0.5)
            };
            dockpanelQuery.Children.Add(borderQuerySplit);
            DockPanel.SetDock(borderQuerySplit, Dock.Right);
            //1.3
            PanelQuery = new WrapPanel();
            dockpanelQuery.Children.Add(PanelQuery);
            groupboxQuery.Content = dockpanelQuery;
            //end 1
            #endregion

            //2数据区域(带翻页栏)
            Grid gridPagging = new Grid();

            gridPagging.RowDefinitions.Add(new RowDefinition());
            gridPagging.RowDefinitions.Add(new RowDefinition());

            //gridPagging.RowDefinitions[0].Height = System.Windows.GridLength.Auto;
            gridPagging.RowDefinitions[1].Height = new System.Windows.GridLength(25);

            DataGridContent = new CheckedGroupDataGird();
            DataGridContent.SelectionChanged += new  SelectionChangedEventHandler(DataGridContent_SelectionChanged);

            _datagridContextMenu = new System.Windows.Controls.ContextMenu();
            MenuItem itemExport = new MenuItem() { Header = "数据导出" };
            itemExport.Click += itemExport_Click;
            _datagridContextMenu.Items.Add(itemExport);
            DataGridContent.ContextMenu = _datagridContextMenu;

            ControlPagging = new UIDataGirdPagging();
            ControlPagging.DataGridContent = DataGridContent;
            ControlPagging.PagerIndexChanged += ControlPagging_PagerIndexChanged;
            ControlPagging.cbxPageSizeList.SelectedIndex = 0;  //默认选择下拉框的第一个数据源
            
            gridPagging.Children.Add(this.DataGridContent);
            gridPagging.Children.Add(this.ControlPagging);
            Grid.SetRow(this.DataGridContent, 0);
            Grid.SetRow(this.ControlPagging, 1);
  
            Border borderData = new Border()
            {
                Margin = new Thickness(2, 5, 5, 2),
                Padding = new Thickness(5),
                BorderBrush = new SolidColorBrush(Colors.Teal),
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(3)
            };
            borderData.Child = gridPagging;

            PanelRoot = new DockPanel();
            PanelRoot.Children.Add(groupboxQuery);
            DockPanel.SetDock(groupboxQuery, Dock.Top);

            FurtherInitLayout();//由派生类自行定义完成

            PanelRoot.Children.Add(borderData);
 
            this.Content = PanelRoot;

        }
       

        public virtual void ControlPagging_PagerIndexChanged(object sender, EventArgs e)
        {
           
        }

        void itemExport_Click(object sender, RoutedEventArgs e)
        {
            //throw new NotImplementedException();
            string strStartDateTime = DateTime.Now.ToString("yyyyMMdd");


            if (MessageBox.Show("确认导出查询的记录？", "确认", MessageBoxButton.OKCancel, MessageBoxImage.Question) ==
                MessageBoxResult.OK)
            {
                try
                {
                    string saveFileName = "";
#if WinOFFICE
                    #region WindowsOffice导出(需要安装office)
                    Microsoft.Win32.SaveFileDialog saveDialog = new Microsoft.Win32.SaveFileDialog();
                    saveDialog.DefaultExt = "xlsx";
                    saveDialog.Filter = "Excel文件|*.xlsx";
                    saveDialog.FileName = "数据导出" + strStartDateTime;
                    saveDialog.ShowDialog();
                    saveFileName = saveDialog.FileName;
                    if (saveFileName.IndexOf(":") < 0) return; //被点了取消

                    //创建Excel  

                    Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
                    Excel.Workbook excelWB = excelApp.Workbooks.Add(System.Type.Missing); //创建工作簿（WorkBook：即Excel文件主体本身）  

                    int currentSheet = 1;
                    int currentRow = 2;
                    Excel.Worksheet excelWS = (Excel.Worksheet)excelWB.Worksheets[currentSheet];
                    excelWS.Cells.NumberFormat = "@";
                    //excelWS = (Worksheet)excelWB.Worksheets[i];   //创建工作表（即Excel里的子表sheet） 1表示在子表sheet1里进行数据导出  

                    ////excelWS.Cells.NumberFormat = "@";     //  如果数据中存在数字类型 可以让它变文本格式显示  
                    ////将数据导入到工作表的单元格  

                    #region cells
                    int cCount = 0;
                    for (int c = 0; c < DataGridContent.Columns.Count; c++)
                    {
                        if (!(DataGridContent.Columns[c].Header is CheckBox))
                        {
                            cCount++;
                            excelWS.Cells.Cells[1, cCount] = DataGridContent.Columns[c].Header;
                        }

                    }
                    if (DataGridContent.Items != null)
                    {
                        for (int r = 0; r < DataGridContent.Items.Count; r++)
                        {
                            int currentCol = 1;
                            var props = AutoUI.GetProps(DataGridContent.Items[r].GetType());
                            //PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(DataGridContent.Items[r].GetType());
                            foreach (PropertyInfo info in props)
                            {
                                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                                {
                                    //PropertyDescriptor itemProperty = properties.Find(info.Name, false);
                                    if (!a.IsNotDatagridColumn)
                                    {
                                        //var vv = itemProperty.GetValue(DataGridContent.Items[r]);
                                        var vvv =
                                            DataGridContent.Columns.Count(
                                                d =>
                                                !(d.Header is CheckBox) && d.Header.ToString().Equals(a.DisplayName));
                                        if (vvv==0)
                                        {
                                            continue;
                                        }
                                        var vv = info.GetValue(DataGridContent.Items[r]);
                                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                                        {
                                            
                                            if (a.DataConvertType == DataConverterType.Currency)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? string.Format("{0:0.00}", vv.ToString()) : "";//!=null?"@"+vv.ToString():"";
                                            }
                                            else if (a.DataConvertType == DataConverterType.DateTime)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ?((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss"):"" ;//!=null?"@"+vv.ToString():"";
                                            }
                                            else if (a.DataConvertType == DataConverterType.Date)
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv != null ?((DateTime)vv).ToString("yyyy-MM-dd"):"";//!=null?"@"+vv.ToString():"";
                                            }
                                            else
                                            {
                                                excelWS.Cells.Cells[r + 2, currentCol] = vv;//!=null?"@"+vv.ToString():"";
                                            }
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                                        {
                                            excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "";

                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATE)
                                        {
                                            excelWS.Cells.Cells[r + 2, currentCol] = vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "";

                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                                        {
                                            //PropertyDescriptor cbxSourceProp = properties.Find(a.ComboDatasource, false);
                                            var cbxSourceProp=props.FirstOrDefault(d => d.Name.Equals(a.ComboDatasource));
                                            var cbxSource = cbxSourceProp.GetValue(DataGridContent.Items[r]);
                                            string str = "";
                                            if (cbxSource is KeyValuePair<int,string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<int, string>[]).FirstOrDefault(d => d.Key == (int)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<int, string>))? itemGet.Value : "";
                                            }
                                            else if (cbxSource is KeyValuePair<string, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<string, string>[]).FirstOrDefault(d => d.Key == (string)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<string, string>)) ? itemGet.Value : "";
                                            }

                                            excelWS.Cells.Cells[r + 2, currentCol] = str;
                                        }

                                        currentCol++;
                                    }

                                }
                            }
                        }
                    }

                    excelWS.Columns.AutoFit();


                    #endregion

                    excelWB.SaveAs(saveFileName); //保存到指定的路径  
                    excelWB.Close();
                    excelApp.Quit(); //KillAllExcel(excelApp); 释放可能还没释放的进程  
                    #endregion
#else
                    #region 不需要WindowsOffice导出
                    Microsoft.Win32.SaveFileDialog saveDialog = new Microsoft.Win32.SaveFileDialog();
                    saveDialog.DefaultExt = "xls";
                    saveDialog.Filter = "Excel文件|*.xls";
                    saveDialog.FileName = "数据导出" + strStartDateTime;
                    saveDialog.ShowDialog();
                    saveFileName = saveDialog.FileName;
                    if (saveFileName.IndexOf(":") < 0) return; //被点了取消

                    //创建Excel  
                    HSSFWorkbook hssfWorkbook = new HSSFWorkbook();

                    ISheet sheet1 = hssfWorkbook.CreateSheet("Sheet1");
                    IRow row = sheet1.CreateRow(0);
                    int cCount = 0;
                    for (int i = 0; i < DataGridContent.Columns.Count; i++)
                    {
                        if (!(DataGridContent.Columns[i].Header is CheckBox))
                        {
                            cCount++;
                            ICell cell = row.CreateCell(i);
                            cell.SetCellValue(DataGridContent.Columns[i].Header.ToString());
                        }
                    }
                    if (DataGridContent.Items != null)
                    {
                        for (int j = 2; j <= DataGridContent.Items.Count + 1; j++)
                        {
                            #region data
                            row = sheet1.CreateRow(j - 1);
                            int i = 0;
                            ICell cell = null;

                            var props = AutoUI.GetProps(DataGridContent.Items[j - 2].GetType());
                            //PropertyDescriptorCollection properties = TypeDescriptor.GetProperties(DataGridContent.Items[r].GetType());
                            foreach (PropertyInfo info in props)
                            {
                                foreach (AttrDataPropertyAttribute a in info.GetCustomAttributes(typeof(AttrDataPropertyAttribute), true))
                                {
                                    //PropertyDescriptor itemProperty = properties.Find(info.Name, false);
                                    if (!a.IsNotDatagridColumn)
                                    {
                                        //var vv = itemProperty.GetValue(DataGridContent.Items[r]);
                                        var vvv =
                                            DataGridContent.Columns.Count(
                                                d =>
                                                !(d.Header is CheckBox) && d.Header.ToString().Equals(a.DisplayName));
                                        if (vvv == 0)
                                        {
                                            continue;
                                        }
                                        var vv = info.GetValue(DataGridContent.Items[j - 2]);
                                        cell = row.CreateCell(i++);
                                        if (a.InputType == UI_INPUT_TYPE.TEXTBOX)
                                        {

                                            if (a.DataConvertType == DataConverterType.Currency)
                                            {
                                                cell.SetCellValue(vv != null ? string.Format("{0:0.00}", vv.ToString()) : "");
                                            }
                                            else if (a.DataConvertType == DataConverterType.DateTime)
                                            {
                                                cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "");
                                            }
                                            else if (a.DataConvertType == DataConverterType.Date)
                                            {
                                                cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "");
                                            }
                                            else
                                            {
                                                cell.SetCellValue(vv != null ? vv.ToString() : "");
                                            }
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATETIME)
                                        {
                                            cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd HH:mm:ss") : "");
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.DATE)
                                        {
                                            cell.SetCellValue(vv != null ? ((DateTime)vv).ToString("yyyy-MM-dd") : "");
                                        }
                                        else if (a.InputType == UI_INPUT_TYPE.COMBOBOX)
                                        {
                                            //PropertyDescriptor cbxSourceProp = properties.Find(a.ComboDatasource, false);
                                            var cbxSourceProp = props.FirstOrDefault(d => d.Name.Equals(a.ComboDatasource));
                                            var cbxSource = cbxSourceProp.GetValue(DataGridContent.Items[j - 2]);
                                            string str = "";
                                            if (cbxSource is KeyValuePair<int, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<int, string>[]).FirstOrDefault(d => d.Key == (int)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<int, string>)) ? itemGet.Value : "";
                                            }
                                            else if (cbxSource is KeyValuePair<string, string>[])
                                            {
                                                var itemGet = (cbxSource as KeyValuePair<string, string>[]).FirstOrDefault(d => d.Key == (string)vv);
                                                str = !itemGet.Equals(default(KeyValuePair<string, string>)) ? itemGet.Value : "";
                                            }
                                            cell.SetCellValue(str);
                                        }
                                    }
                                }
                            }
                            #endregion
                        }
                    }

                    for (int i = 0; i < cCount; i++)
                    {
                        sheet1.AutoSizeColumn(i);
                    }
                    #region cells

                    #endregion
                    FileStream file = new FileStream(saveFileName, FileMode.Create);
                    hssfWorkbook.Write(file);
                    file.Close();
                    #endregion
#endif


                    MessageBox.Show("导出成功！");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("错误：" + ex.Message);
                }
            }
        }


        void DataGridContent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            CurrentItem = DataGridContent.SelectedItem;
            CurrentSelectonChanged();
        }

        public virtual void CurrentSelectonChanged()
        {

        }

        protected virtual void FurtherInitLayout() { }
        protected ContextMenu _datagridContextMenu;
        protected void RegisterCommand(UserControl ucBind, Button btnSource, ICommand cmd)
        {
            CommandBinding cb = new CommandBinding(cmd);
            this.CommandBindings.Add(cb);
            btnSource.Command = cmd;
        }

        protected void RegisterButtonAndContextMenu(UserControl ucBind, AttrMethodAttribute[] props, ICommand[] cmds)
        {
            CommandBinding cb = null;

            //DataGridContent.ContextMenu = new ContextMenu();
            _datagridContextMenu.Items.Add(new Separator());
            for (int i = 0; i < props.Count(); i++)
            {
                cb = new CommandBinding(cmds[i]);
                this.CommandBindings.Add(cb);
                props[i].ButtonCtrl.Command = cmds[i];

                DataGridContent.ContextMenu.Items.Add(
                    new MenuItem()
                    {
                        Header = props[i].FunctionDisplayName,
                        Command = cmds[i]
                    }
                    );

            }

        }

        protected ComboBox CreateCombox(string path, string selectValue, string displayMember)
        {
            ComboBox cbx = new ComboBox()
            {
                Style = (Style)Application.Current.Resources["styleInputControl"],
                Focusable = true,
                SelectedValuePath = path,
                DisplayMemberPath = displayMember,
                IsEditable = true
            };

            cbx.SetBinding(ComboBox.SelectedValueProperty, new Binding(selectValue)
            {
                ValidatesOnDataErrors = false,
                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
                Mode = BindingMode.TwoWay
            });
            return cbx;
        }

    }
    #endregion



    #endregion

    #region 第3层 功能

    public class FunctionList
    {
        public string FunctionName { get; set; }
        public Type FunctionView { get; set; }
        public bool IsFunctionEnabled { get; set; }
        private UserControl _viewObject;

        public FunctionList()
        {
            IsFunctionEnabled = true;
        }

        public UserControl GetViewObject()
        {
            if (_viewObject == null)
                _viewObject = (UserControl)Activator.CreateInstance(FunctionView);

            return _viewObject;
        }

        public override string ToString()
        {
            //return base.ToString();
            return FunctionName;
        }
    }

    public class FunctionListInfo
    {
        public string DSName { get; set; }
        public string DSName2 { get; set; }
        public string FunctionName { get; set; }
        public string FunctionSource { get; set; }
        public Base.MVVM.ViewModelBase VMObject { get; set; }
        //public Type ViewModel { get; set; }

        public Parameter[] Params;

        public override string ToString()
        {
            return FunctionName;//return base.ToString();
        }
    }

    public class ExpReportModuleInfo:UserControl
    {
        public string ModuleName { get; set; }
        private List<FunctionListInfo> _funcs = new List<FunctionListInfo>();
        //Data
        protected ObservableCollection<object> data = new ObservableCollection<object>();
        protected ObservableCollection<object> data2 = new ObservableCollection<object>();

        Microsoft.Reporting.WinForms.ReportViewer reportViewer;

                //UIS
        public WrapPanel panelQuery;
        public Button btnResetQuery;
        public Button btnQuery;

        public List<FunctionListInfo> Funcs
        {
            get { return _funcs; }
            set { _funcs = value; }
        }

        protected FunctionListInfo _currentReport = null;

        protected Grid gd = new Grid();
        protected Grid gdMainContent = new Grid();

        private List<AttrQueryPropertyAttribute> _props = new List<AttrQueryPropertyAttribute>();
        //查询Attribute熟悉
        public List<AttrQueryPropertyAttribute> Props
        {
            get { return _props; }
            set { _props = value; }
        }

        internal void InitComponents()
        {
            //1查询区域
            #region 1
            GroupBox groupboxQuery = new GroupBox() { Header = "查询区域" };
            DockPanel dockpanelQuery = new DockPanel();
            //1.1查询重置按钮 Grid
            Grid gridQuery = new Grid();
            #region layout
            gridQuery.RowDefinitions.Add(new RowDefinition());
            gridQuery.RowDefinitions.Add(new RowDefinition());

            btnResetQuery = new Button()
            {
                Content = "重置(_R)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                Tag = "R"
            };
            btnQuery = new Button()
            {
                Content = "查询(_Q)",
                Style = (Style)System.Windows.Application.Current.Resources["styleBtnQuery"],
                Tag = "Q"
            };

            btnQuery.Click += new RoutedEventHandler(Btn_Click);
            btnResetQuery.Click += new RoutedEventHandler(Btn_Click);


            gridQuery.Children.Add(btnResetQuery);
            gridQuery.Children.Add(btnQuery);
            Grid.SetRow(btnResetQuery, 0);
            Grid.SetRow(btnQuery, 1);
            #endregion
            dockpanelQuery.Children.Add(gridQuery);
            DockPanel.SetDock(gridQuery, Dock.Right);
            //1.2 border
            Border borderQuerySplit = new Border()
            {
                Margin = new Thickness(5, -5, 5, -5),
                BorderBrush = new SolidColorBrush(Colors.LightGray),
                BorderThickness = new Thickness(1)
            };
            dockpanelQuery.Children.Add(borderQuerySplit);
            DockPanel.SetDock(borderQuerySplit, Dock.Right);
            //1.3 查询区域
            panelQuery = new WrapPanel();
            dockpanelQuery.Children.Add(panelQuery);
            groupboxQuery.Content = dockpanelQuery;
            //end 1
            #endregion

            //2数据区域
            Border borderData = new Border()
            {
                Margin = new Thickness(2, 5, 5, 2),
                Padding = new Thickness(5),
                BorderBrush = new SolidColorBrush(Colors.Teal),
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(3)
            };

            System.Windows.Forms.Integration.WindowsFormsHost host = new System.Windows.Forms.Integration.WindowsFormsHost
            {
                Margin = new Thickness(3)
            };

            reportViewer = new Microsoft.Reporting.WinForms.ReportViewer();
            host.Child = reportViewer;

            borderData.Child = host;
            DockPanel dockPanel = new DockPanel();
            dockPanel.Children.Add(groupboxQuery);
            DockPanel.SetDock(groupboxQuery, Dock.Top);

            dockPanel.Children.Add(borderData);
            this.Content = dockPanel;
        }

        protected virtual void ReportSelectionChanged(FunctionListInfo rInfo) { }

        internal void ReportChange(FunctionListInfo currentReport)
        {
            _currentReport = currentReport;
            ReportSelectionChanged(currentReport);
        }

        void Btn_Click(object sender, RoutedEventArgs e)
        {
            BtnClicked((sender as Button).Tag.ToString());
        }

        protected virtual void BtnClicked(string strBtnName)
        { }

        public void InitUI(Type aModelType)
        {

            //    UIFactory.UIDatagridInit(aModelType, datagridContent);
        }

        /// <summary>
        /// 重置ReportViewer
        /// </summary>
        public void GenerateReport()
        {
            reportViewer.Reset();
        }

        public void GenerateReport(string strDSName, string strReportDispName, string strReportSource, Parameter[] paras)
        {
            Microsoft.Reporting.WinForms.ReportDataSource rds = new Microsoft.Reporting.WinForms.ReportDataSource();

            reportViewer.Reset();
            reportViewer.LocalReport.ReportPath = strReportSource;// "ReportTest.rdlc";

            rds.Name = strDSName;
            rds.Value = data;
            reportViewer.LocalReport.DataSources.Add(rds);

            var v = from d in paras select new Microsoft.Reporting.WinForms.ReportParameter(d.PName, d.PValue);
            reportViewer.LocalReport.SetParameters(v);

            reportViewer.LocalReport.DisplayName = strReportDispName;
            reportViewer.RefreshReport();
        }

        public void GenerateReport(string strDSName, string strDSName2, string strReportDispName, string strReportSource, Parameter[] paras)
        {
            Microsoft.Reporting.WinForms.ReportDataSource rds = new Microsoft.Reporting.WinForms.ReportDataSource();

            reportViewer.Reset();
            reportViewer.LocalReport.ReportPath = strReportSource;// "ReportTest.rdlc";

            rds.Name = strDSName;
            rds.Value = data;
            reportViewer.LocalReport.DataSources.Add(rds);
            if (strDSName2 != null)
            {
                rds = new Microsoft.Reporting.WinForms.ReportDataSource
                {
                    Name = strDSName2,
                    Value = data2
                };
                reportViewer.LocalReport.DataSources.Add(rds);
            }

            var v = from d in paras select new Microsoft.Reporting.WinForms.ReportParameter(d.PName, d.PValue);
            reportViewer.LocalReport.SetParameters(v);

            reportViewer.LocalReport.DisplayName = strReportDispName;
            reportViewer.RefreshReport();
        }
    }

    #region 报表
    public abstract class ExpReportFuncsUIBase : UserControl
    {
        private Border paneLeft;
        private Border pane;
        private GridSplitter paneSplit;
        private Grid grid;
        protected Grid panelUI; //操作区域
        private ToggleButton tgb;
        protected StackPanel panelFuncsGroup; //功能列表
        protected DockPanel dockPanel;
        public StackPanel panelFunction;

        protected List<ExpReportModuleInfo> _modules = new List<ExpReportModuleInfo>();

        protected ExpReportModuleInfo _currentModules = null;
        public List<ExpReportModuleInfo> ModuleFuncLists
        {
            get { return _modules; }
            //set { _funcLists = value; }
        }

        public ExpReportFuncsUIBase()
        {
            Foreground = Brushes.Black;
            InitComponents();
        }

        /// <summary>
        /// 初始化
        /// </summary>
        private void InitComponents()
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

            //panelFuncsGroup = new StackPanel();
            //tempDock1.Children.Add(panelFuncsGroup);
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

            #region

            dockPanel= new DockPanel();
            panelUI = new Grid
            {
                //DockPanel.SetDock(groupboxQuery, Dock.Bottom);
                Background = Brushes.White
            };
            panelUI.Children.Add(dockPanel);
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
            #endregion
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
            ExpReportModuleInfo module=_modules.FirstOrDefault(d => d.ModuleName.Equals((sender as Expander).Header));
            if (module != null)
            {
                module.InitComponents();
                _currentModules = module;
                panelUI.Children.Add(module);
            }
        }

        void Lbx_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if ((sender as ListBox).SelectedItem != null)
            {
                _currentModules.ReportChange((sender as ListBox).SelectedItem as FunctionListInfo);
            }
        }

        protected abstract void RegisterModules();
    }

    public class Parameter
    {
        public string PName { get; set; }
        public string PValue { get; set; }
    }

    public interface IReportVM
    {
        void GetReportData(ObservableCollection<object> data, ObservableCollection<object> data2 = null);
        void InitReportParams(out Parameter[] rptParams);
        bool ValidateFunc();

        //bool IsFieldUnNull();
    }
    #endregion

    #endregion

    public interface IFuncView
    {
        // void UIInit();
    }

    public class FunctionViewInfo
    {
        private UserControl funView = null;

        public string FunName { get; set; }
        public Type FunViewType { get; set; }
        public UserControl GetFunView()
        {
            if (funView == null)
            {
                funView = (UserControl)System.Activator.CreateInstance(FunViewType);
            }
            if (funView is IFuncView)
            {
                funView = (UserControl)System.Activator.CreateInstance(FunViewType);

            }
            return funView;
        }
    }

    //左边listview选择功能
    public class UCListFunctionBase : UserControl
    {
        //功能列表框
        protected GroupBox _groupboxFunction;
        public ListBox _listboxFunction;

        //功能实现区域
        protected GroupBox _groupboxOperation;

        //功能定义
        protected ObservableCollection<FunctionViewInfo> _functions =
            new ObservableCollection<FunctionViewInfo>();
        public UCListFunctionBase()
        {
            Foreground = Brushes.Black;
            UCInit();
        }

        protected virtual void UCInit()
        {
            _groupboxFunction = new GroupBox() { Header = "功能选择" };
            _listboxFunction = new ListBox() { Width = 180 };
            _listboxFunction.Style = (Style)Application.Current.Resources["listboxLSK"];
            _listboxFunction.ItemContainerStyle = (Style)Application.Current.Resources["listboxItemLSK"];

            _listboxFunction.SelectionChanged += new SelectionChangedEventHandler(ListboxFunction_SelectionChanged);
            _listboxFunction.DisplayMemberPath = "FunName";
            _listboxFunction.ItemsSource = _functions;
            _groupboxFunction.Content = _listboxFunction;

            _groupboxOperation = new GroupBox();

            DockPanel panelRoot = new DockPanel() { LastChildFill = true };

            panelRoot.Children.Add(_groupboxFunction);
            DockPanel.SetDock(_groupboxFunction, Dock.Left);
            panelRoot.Children.Add(_groupboxOperation);
            this.Content = panelRoot;
        }

        void ListboxFunction_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //throw new NotImplementedException();
            FunctionViewInfo fv = _listboxFunction.SelectedItem as FunctionViewInfo;
            _groupboxOperation.Content = fv.GetFunView();
            _groupboxOperation.Header = fv.FunName;
        }
    }
}