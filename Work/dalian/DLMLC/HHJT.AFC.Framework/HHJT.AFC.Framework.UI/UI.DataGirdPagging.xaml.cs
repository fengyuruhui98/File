using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HHJT.AFC.Framework.UI
{

    public class PageSizeDefine
    {
        public int PageSizeKey { get; set; }
        public int PageSizeValue { get; set; }
    }


    /// <summary>
    /// 定义页码改变后激发的委托事件
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    public delegate void PagerIndexChangedEventHandler(object sender, EventArgs e);

      
    /// <summary>
    /// UI.xaml 的交互逻辑
    /// </summary>
    public partial class UIDataGirdPagging : UserControl
    {
        private List<PageSizeDefine> _pageSizeDefine = new List<PageSizeDefine>();
         

        public CheckedGroupDataGird DataGridContent;

        public UIDataGirdPagging()
        {
            InitializeComponent();

            _pageSizeDefine.Add(new PageSizeDefine { PageSizeKey = 30, PageSizeValue = 30});
            _pageSizeDefine.Add(new PageSizeDefine { PageSizeKey = 60, PageSizeValue = 60 });
            _pageSizeDefine.Add(new PageSizeDefine { PageSizeKey = 150, PageSizeValue = 150 });
            _pageSizeDefine.Add(new PageSizeDefine { PageSizeKey = 9999999, PageSizeValue = 9999999 });
            cbxPageSizeList.ItemsSource = _pageSizeDefine;           
        }

        #region 私有变量
        /// <summary>
        /// 当前页码
        /// </summary>
        private int _pageIndex = 1;
        /// <summary>
        /// 分页大小
        /// </summary>
        private int _pageSize = 30;
        /// <summary>
        /// 记录总数
        /// </summary>
        private int _totalCount = 0;
        /// <summary>
        /// 总页数
        /// </summary>
        private int _pageCount = 0;
        #endregion



        #region 事件声明
        /// <summary>
        /// 页码改变后的事件
        /// </summary>
        public event PagerIndexChangedEventHandler PagerIndexChanged;        
        #endregion

        #region 属性
        /// <summary>
        /// 当前页码
        /// </summary>
        public int PageIndex
        {
            get { return _pageIndex; }
            set { _pageIndex = value; SetPageButtonEnabled(); }
        }

        /// <summary>
        /// 分页大小
        /// </summary>
        public int PageSize
        {
            get { return _pageSize; }
            set
            {
                _pageSize = value;

                //计算总页数
                _pageIndex = 1; //分页大小改变之后,当前页数改为1
                _pageCount = (int)Math.Ceiling((double)_totalCount / _pageSize);
                SetPagerInfo(_pageIndex, _pageCount, _totalCount);
                SetPageButtonEnabled();
            }
        }
 
        /// <summary>
        /// 记录总数
        /// </summary>
        public int TotalCount
        {
            get { return _totalCount; }
            set
            {
                _totalCount = value;
                //计算总页数
                _pageCount = (int)Math.Ceiling((double)_totalCount / _pageSize);
                SetPagerInfo(_pageIndex, _pageCount, _totalCount);
                SetPageButtonEnabled();
            }
        }

        /// <summary>
        /// 总页数
        /// </summary>
        public int PageCount
        {
            get { return _pageCount; }
            set { _pageCount = value; }
        }

        #endregion

        #region 私有方法

        /// <summary>
        /// 设置控件显示信息
        /// </summary>
        /// <param name="pageIndex">当前页码</param>
        /// <param name="pageCount">共有页数</param>
        /// <param name="totalCount">总记录条数</param>
        private void SetPagerInfo(int pageIndex, int pageCount, int totalCount)
        {
            txtPagerInfo.Text = String.Format("当前第【{0}】页，共【{1}】页，共【{2}】条记录", pageIndex, pageCount, totalCount);
        }

        /// <summary>
        /// 设置控件显示信息
        /// </summary>
        /// <param name="pageIndex">当前页码</param>
        /// <param name="pageCount">共有页数</param>
        /// <param name="totalCount">总记录条数</param>
        private void SetPagerInfo()
        {
            txtPagerInfo.Text = String.Format("当前第【{0}】页，共【{1}】页，共【{2}】条记录", _pageIndex, _pageCount, _totalCount);
        }

        /// <summary>
        /// 设置分页按钮是否可用
        /// </summary>
        private void SetPageButtonEnabled()
        {
            SetPagerInfo();
            //确定分页按钮的是否可用
            if (_pageCount <= 1)
            {
                btnFirstPage.IsEnabled = false;
                btnPageDown.IsEnabled = false;
                btnPageUp.IsEnabled = false;
                btnEndPage.IsEnabled = false;
            }
            else
            {
                if (_pageIndex == _pageCount)
                {
                    btnFirstPage.IsEnabled = true;
                    btnPageDown.IsEnabled = false;
                    btnPageUp.IsEnabled = true;
                    btnEndPage.IsEnabled = false;
                }
                else if (_pageIndex <= 1)
                {
                    btnFirstPage.IsEnabled = false;
                    btnPageDown.IsEnabled = true;
                    btnPageUp.IsEnabled = false;
                    btnEndPage.IsEnabled = true;
                }
                else
                {
                    btnFirstPage.IsEnabled = true;
                    btnPageDown.IsEnabled = true;
                    btnPageUp.IsEnabled = true;
                    btnEndPage.IsEnabled = true;
                }
            }
        }

       
        #endregion

        #region 私有事件
        /// <summary>
        /// 控件加载事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            SetPageButtonEnabled();
        }

        /// <summary>
        /// 首页按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnFirstPage_Click(object sender, RoutedEventArgs e)
        {
            _pageIndex = 1;
            SetPageButtonEnabled();
            PagerIndexChanged(sender, e);
        }

        /// <summary>
        /// 下一页按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnPageDown_Click(object sender, RoutedEventArgs e)
        {
            _pageIndex++;
            SetPageButtonEnabled();
            PagerIndexChanged(sender, e);
        }

        /// <summary>
        /// 上一页按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnPageUp_Click(object sender, RoutedEventArgs e)
        {
            _pageIndex--;
            SetPageButtonEnabled();
            PagerIndexChanged(sender, e);
        }

        /// <summary>
        /// 尾页按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnEndPage_Click(object sender, RoutedEventArgs e)
        {
            _pageIndex = _pageCount;
            SetPageButtonEnabled();
            PagerIndexChanged(sender, e);
        }
        #endregion

        private void cbxPageSizeList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            PageSize = (int)cbxPageSizeList.SelectedValue;
            SetPageButtonEnabled();
            PagerIndexChanged(sender, e);
        }

    }
}
