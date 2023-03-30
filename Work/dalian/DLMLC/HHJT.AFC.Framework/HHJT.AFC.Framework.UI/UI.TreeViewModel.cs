using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HHJT.AFC.Framework.UI
{
    /// <summary>
    /// Selected TreeViewModel接口，用户自定义的SelectedTreeViewModel需实现该接口
    /// </summary>
    public interface ISelectedTreeViewModel
    {
        string Name { get; set; }
        bool? IsChecked { get; set; }
        bool IsLeaf { get; set; }
    }

    /*-------------说明-----------------
     * Treeview元素需实现ISelectedTreeViewModel接口，父子节点类型不要求统一
     * 如现实设备列表
     * 最外一层可为线路 定义LineTreeViewItem，第二层StationTreeViewItem，第三层DeviceCategoryTreeViewItem，第四层DeviceTreeViewItem
     */


    /// <summary>
    /// SelectedTreeview模型
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class SelectedTreeViewModel<T> : INotifyPropertyChanged where T : ISelectedTreeViewModel
    {
        #region Data

        //bool? m_isChecked = false;
        SelectedTreeViewModel<T> m_parent;

        public T data;

        #endregion // Data

        #region CreateFoos


        public SelectedTreeViewModel(T d)
        {
            data = d;

            this.Name = d.Name;
            this.Children = new List<SelectedTreeViewModel<T>>();
        }

        public void Initialize()
        {
            SelectedTreeViewModel<T> lastChildren = null;
            if (this.Children.Count > 0)
            {
                lastChildren = this.Children.Last();
            }
            foreach (SelectedTreeViewModel<T> child in this.Children)
            {
                child.m_parent = this;
                child.Initialize();
                if (child.Equals(lastChildren) && child.IsLeaf)
                {
                    child.IsChecked = child.data.IsChecked;
                }
            }
        }

        #endregion

        #region Properties

        public List<SelectedTreeViewModel<T>> Children { get; private set; }

        public bool IsInitiallySelected { get; private set; }

        public string Name { get; private set; }

        #region IsChecked
        public bool IsLeaf
        {
            get { return data.IsLeaf; }
            set { }
        }

        public bool? IsChecked
        {
            get { return data.IsChecked; }
            set { this.SetIsChecked(value, true, true); }
        }

        void SetIsChecked(bool? value, bool updateChildren, bool updateParent)
        {
            //if (value == data.IsChecked)
            //    return;

            data.IsChecked = value;


            if (updateChildren && data.IsChecked.HasValue)
                this.Children.ForEach(c => c.SetIsChecked(data.IsChecked, true, false));

            if (updateParent && m_parent != null)
                m_parent.VerifyCheckState();

            data.IsChecked = value;

            this.OnPropertyChanged("IsChecked");
        }

        void VerifyCheckState()
        {
            bool? state = null;
            for (int i = 0; i < this.Children.Count; ++i)
            {
                bool? current = this.Children[i].IsChecked;
                if (i == 0)
                {
                    state = current;
                }
                else if (state != current)
                {
                    state = null;
                    break;
                }
            }
            this.SetIsChecked(state, false, true);
        }

        #endregion

        #endregion

        #region INotifyPropertyChanged Members

        void OnPropertyChanged(string prop)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(prop));
        }

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

    }



    /*
    public class SelectedTreeViewStation : ISelectedTreeViewModel
    {
        #region Interface
        private string _stationNameorLinename;
        private bool? _isChecked;

        public string Name //显示内容窗口
        {
            get
            {
                return this._stationNameorLinename;
            }
            set
            {
                this._stationNameorLinename = value;
            }
        }

        public bool? IsChecked
        {
            get
            {
                return _isChecked;
            }
            set
            {
                this._isChecked = value;
            }
        }
        #endregion

        public int LineID { get; set; } //全路网00，其他为线路号
        public int StationID { get; set; } //若为路网或线路节点，StationID为0

        public static List<SelectedTreeViewModel<SelectedTreeViewStation>> CreateTreeView() //test
        {
            SelectedTreeViewModel<SelectedTreeViewStation> root
                = new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "全路网", LineID = 0, StationID = 0 })
                {

                    Children =
                {
                    new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "线路1", LineID = 1, StationID = 0 })
                    {
                        Children =
                        {
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站11", LineID = 1, StationID = 1 }),
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站12", LineID = 1, StationID = 2 }),
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站13", LineID = 1, StationID = 3 }),
                        }
                    },
                    new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "线路2", LineID = 2, StationID = 0 })
                    {
                        Children =
                        {
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站21", LineID = 2, StationID = 1 }),
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站22", LineID = 2, StationID = 2 }),
                            new SelectedTreeViewModel<SelectedTreeViewStation>(new SelectedTreeViewStation() { Name = "车站23", LineID = 2, StationID = 3 }),
                        }
                    },
                }
                };

            root.Initialize();
            return new List<SelectedTreeViewModel<SelectedTreeViewStation>> { root };
        }
    }
    */

    public class SelectedTreeViewFunction : ISelectedTreeViewModel
    {
        private IFunctionInfo _funcInfo;

        public IFunctionInfo FuncInfo
        {
            get { return _funcInfo; }
            // set { _funcInfo = value; }
        }

        public SelectedTreeViewFunction(IFunctionInfo funcInfo)
        {
            this._funcInfo = funcInfo;
        }

        public string Name
        {
            get
            {
                return _funcInfo.FuncName;
            }
            set
            {
            }
        }

        public bool? IsChecked
        {
            get
            {
                return this._funcInfo.IsSelected;
            }
            set
            {
                this._funcInfo.IsSelected = value;
            }
        }

        public bool IsLeaf
        {
            get { return this._funcInfo.IsLeaf; }
            set { }
        }

        public static List<SelectedTreeViewModel<SelectedTreeViewFunction>>
            CreateTreeViewByFuncInfo(List<IFunctionInfo> listData)
        {
            List<SelectedTreeViewModel<SelectedTreeViewFunction>> views = new List<SelectedTreeViewModel<SelectedTreeViewFunction>>();

            foreach (var item in listData)
            {
                views.Add(GenerateLevelsTree(item));
            }

            foreach (var item in views)
            {
                item.Initialize();
            }
            return views;
        }

        public static List<SelectedTreeViewModel<SelectedTreeViewFunction>>
   CreateTreeViewByFuncInfoList(List<IFunctionInfo> listData)
        {
            List<SelectedTreeViewModel<SelectedTreeViewFunction>> views = new List<SelectedTreeViewModel<SelectedTreeViewFunction>>();
            foreach (var item in listData.Where(d => d.ParentFuncId == 0))
            {
                views.Add(GenerateTree(listData, item));
            }

            foreach (var item in views)
            {
                item.Initialize();

            }

            return views;
        }

        public static List<SelectedTreeViewModel<SelectedTreeViewFunction>>
   CreateTreeViewByFuncInfoList(List<IFunctionInfo> listData, int temp)
        {
            List<SelectedTreeViewModel<SelectedTreeViewFunction>> views = new List<SelectedTreeViewModel<SelectedTreeViewFunction>>();
            foreach (var item in listData.Where(d => d.ParentFuncId == temp))
            {
                views.Add(GenerateTree(listData, item));
            }

            foreach (var item in views)
            {
                item.Initialize();

            }

            return views;
        }

        public static SelectedTreeViewModel<SelectedTreeViewFunction> GenerateLevelsTree(IFunctionInfo func)
        {
            SelectedTreeViewModel<SelectedTreeViewFunction> v =
                new SelectedTreeViewModel<SelectedTreeViewFunction>(new SelectedTreeViewFunction(func));
            foreach (var item in func.Children)
            {
                v.Children.Add(GenerateLevelsTree(item));
            }
            return v;
        }

        private static SelectedTreeViewModel<SelectedTreeViewFunction> GenerateTree(
            IEnumerable<IFunctionInfo> functionInfos, IFunctionInfo func)
        {
            SelectedTreeViewModel<SelectedTreeViewFunction> v =
                new SelectedTreeViewModel<SelectedTreeViewFunction>(new SelectedTreeViewFunction(func));

            if (!func.IsLeaf)
            {
                foreach (var functionInfo in functionInfos.Where(d => d.ParentFuncId == func.FuncId))
                {
                    v.Children.Add(GenerateTree(functionInfos, functionInfo));
                }
            }
            return v;
        }

    }

    public interface IFunctionInfo
    {
         bool? IsSelected { get; set; }
         string FuncName { get; set; }
         List<IFunctionInfo> Children { get; set; }
         int ParentFuncId { get; set; }
         IFunctionInfo ParentFunc { get; set; }
         int Level { get; set; }
         int FuncId { get; set; }
         bool IsLeaf { get; set; }
    }
}
