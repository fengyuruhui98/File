using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using System.Collections;

namespace HHJT.AFC.Base.API
{
    public enum LogLevel
    {
        Debug,
        Info,
        Warnning,
        Error,
        SysError,
        Others
    }

    public interface ILogger
    {
        void Init(string pathName, string fileName);
        void WriteLog(LogLevel logLevel, string strFormat, params object[] paraLists);
        void EndLog();
    }

    public class FileLogger : ILogger
    {
        public string LogFilePath { get; set; }
        public string LogFileName { get; set; }
        public int MaxLogFileSize { get; set; }

        private string _logFileFullName;
        private StreamWriter _LogFile;

        bool _isContinue;

        private Queue<string> _strLogs = new Queue<string>();
        string _currentLog;
        protected EventWaitHandle _eventWaitHandle = null;
        protected Thread _thread = null;

        public FileLogger()
        {
            MaxLogFileSize = 1024 * 1024;
            _isContinue = true;
        }

        private void StartLogThread(string eventName)
        {
            _eventWaitHandle = new EventWaitHandle(false, EventResetMode.ManualReset, eventName);
            if (_thread == null)
            {
                _thread = new Thread(run);
                _thread.Start();
            }
        }

        protected void run()
        {
            while (_isContinue)
            {
                if (_eventWaitHandle.WaitOne())//等待事件
                {
                    while (_strLogs.Count > 0)
                    {
                        lock (((ICollection)_strLogs).SyncRoot)//消息同步控制
                        {
                            _currentLog = _strLogs.Dequeue();
                        }
                        WriteToFile();
                    }
                    _eventWaitHandle.Reset();
                }
            }
            //Console.WriteLine("quit thread");
        }

        protected void WriteToFile()
        {
            try
            {
                _LogFile = new StreamWriter(_logFileFullName, true);

                if (_LogFile != null)
                {
                    _LogFile.WriteLine(_currentLog);
                }

                _LogFile.Close();

                FileInfo fInfo = new FileInfo(_logFileFullName);
                if (fInfo.Length > MaxLogFileSize)
                {
                    string strDest = LogFilePath + "//" + LogFileName + ".bak.log";
                    File.Delete(strDest);
                    File.Move(_logFileFullName, strDest);

                }

            }
            catch (Exception ex)
            {
                _LogFile.Close();
                Console.WriteLine(DateTime.Now.ToString("yyyMMdd HH:mm:ss") + "WriteLog Error" + ex.Message);
            }

        }

        public void WriteLog(LogLevel logLevel, string strFormat, params object[] paraLists)
        {
            string strLog;

            strLog = string.Format(logLevel + DateTime.Now.ToString("\tyyyyMMdd HH:mm:ss\t") + strFormat, paraLists);
#if OutputInfo
            Console.WriteLine(strLog);
#endif
            lock (((ICollection)_strLogs).SyncRoot)
            {
                _strLogs.Enqueue(strLog);
            }

            _eventWaitHandle.Set();//驱动写日志
        }


        public void Init(string pathName, string fileName)
        {
            LogFilePath = pathName;
            LogFileName = fileName;
            _logFileFullName = pathName + fileName + ".log";
            Directory.CreateDirectory(pathName);

            StartLogThread(fileName);
        }

        public void EndLog()
        {

            _isContinue = false;
            _eventWaitHandle.Set();
        }

    }

    public class DailyLogger : ILogger
    {
        public string LogFilePath { get; set; }
        public string LogFileName { get; set; }
        public int MaxLogFileSize { get; set; }

        private string _logFileFullName;
        private StreamWriter _LogFile;

        public void Init(string pathName, string fileName)
        {
            //throw new NotImplementedException();
            if (!Directory.Exists(pathName))
            {
                Directory.CreateDirectory(pathName);
            }
            LogFilePath = pathName;
            LogFileName = fileName;

        }

        public void WriteLog(LogLevel logLevel, string strFormat, params object[] paraLists)
        {
            //throw new NotImplementedException();
            _logFileFullName = LogFilePath + LogFileName + "_" + DateTime.Now.ToString("yyyyMMdd") + ".log";

            string strLog;
            strLog = string.Format(logLevel + DateTime.Now.ToString("\tyyyyMMdd HH:mm:ss\t") + strFormat, paraLists);

            try
            {
                _LogFile = new StreamWriter(_logFileFullName, true);

                if (_LogFile != null)
                {
                    _LogFile.WriteLine(strLog);
                }

                _LogFile.Close();



            }
            catch (Exception ex)
            {
                _LogFile.Close();
                Console.WriteLine(DateTime.Now.ToString("yyyMMdd HH:mm:ss") + "WriteLog Error" + ex.Message);
            }

        }

        public void EndLog()
        {
            //throw new NotImplementedException();

        }
    }

}
