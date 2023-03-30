using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using HHJT.AFC.Base.MVVM;

namespace HHJT.AFC.Framework.UI
{
    public abstract class VM_QueryTemplate : ViewModelBase
    {
        protected RelayCommand m_CommandQuery;
        protected RelayCommand m_CommandReset;

        public ICommand CommandQuery
        {
            get
            {
                if (m_CommandQuery == null)
                {
                    m_CommandQuery = new RelayCommand(
                        param => this.Query(),
                        param => this.CanQuery()
                        );
                }
                return m_CommandQuery;
            }
        }

        public ICommand CommandReset
        {
            get
            {
                if (m_CommandReset == null)
                {
                    m_CommandReset = new RelayCommand(
                        param => this.Reset(),
                        param => this.CanReset()
                        );
                }
                return m_CommandReset;
            }
        }

        public abstract void Query();
        public abstract bool CanQuery();

        public abstract void Reset();
        public abstract bool CanReset();

        public virtual bool ValidateQuery()
        {
            return true;
        }

        private List<AttrQueryPropertyAttribute> _props = null;
        //查询Attribute熟悉
        public List<AttrQueryPropertyAttribute> Props
        {
            get { return _props; }
            set { _props = value; }
        }

    }

    public abstract class VM_IsCheckedTemplate : ViewModelBase, IIsChecked
    {
        private bool _isChecked;
        public bool IsChecked
        {
            get
            {
                return _isChecked;
            }
            set
            {
                if (_isChecked == value) return; _isChecked = value; OnPropertyChanged("IsChecked");
            }
        }
    }

    public abstract class VMDataErrorBase : ViewModelBase, IDataErrorInfo
    {
        protected List<string> _errorList = new List<string>();
        public string Error
        {
            get
            {
                if (_errorList.Count > 0) return string.Format("有输入项不正确，请检查！");
                else return null;
            }
        }
        public abstract string this[string columnName]
        {
            get;
        }

    }
}
