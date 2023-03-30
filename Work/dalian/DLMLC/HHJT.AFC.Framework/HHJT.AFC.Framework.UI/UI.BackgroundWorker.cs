using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HHJT.AFC.Framework.UI
{
    public delegate void BackgroundFunc(int data);

    public abstract class BackGroundWorkerWrapper
    {
        public Queue<object> msgs = new Queue<object>();//消息队列
        protected BackgroundWorker _bkWorker;
        private bool _terminateFlag;

        //public event BackgroundFunc FuncDoWork;
        public event BackgroundFunc FuncReportProgress;
        public event BackgroundFunc FuncRunCompleted;

        public bool TerminateFlag
        {
            get { return _terminateFlag; }
            set { _terminateFlag = value; }
        }
        protected BackgroundWorker BkWorker
        {
            get { return _bkWorker; }
            //  set { _bkWorker = value; }
        }
        protected EventWaitHandle _eventWait = null;

        public BackGroundWorkerWrapper(string eventName)
        {
            _bkWorker = new BackgroundWorker();
            _eventWait = new EventWaitHandle(false, EventResetMode.ManualReset, eventName);//创建命名事件
        }

        public void SetEvent()//事件触发
        {
            _eventWait.Set();
        }

        public void Init()
        {
            _bkWorker.WorkerReportsProgress = true;
            _bkWorker.DoWork += new DoWorkEventHandler(_bkWorker_DoWork);
            _bkWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bkWorker_RunWorkerCompleted);
            _bkWorker.ProgressChanged += new ProgressChangedEventHandler(_bkWorker_ProgressChanged);
        }

        public void Start()
        {
            _bkWorker.RunWorkerAsync();
        }
        void _bkWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //throw new NotImplementedException();
            if (FuncReportProgress != null)
            {
                FuncReportProgress(e.ProgressPercentage);
            }
        }

        void _bkWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            //throw new NotImplementedException();
            if (FuncRunCompleted != null)
            {
                FuncRunCompleted(0);
            }
        }

        void _bkWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            //throw new NotImplementedException();
            while (!TerminateFlag)
            {
                if (_eventWait.WaitOne())//等待事件
                {
                    Process();
                }
                _eventWait.Reset();
            }

        }

        public abstract void Process();

        public void WriteData(object obj)
        {
            lock (((ICollection)msgs).SyncRoot)
            {
                msgs.Enqueue(obj);
            }
        }

        public object GetData()//写消息
        {
            object obj = null;
            lock (((ICollection)msgs).SyncRoot)
            {
                if (msgs.Count != 0)
                    obj = msgs.Dequeue();
            }
            return obj;
        }

    }
}
