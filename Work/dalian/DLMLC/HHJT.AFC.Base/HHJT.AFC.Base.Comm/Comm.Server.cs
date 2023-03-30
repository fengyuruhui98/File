using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;

///<summary>
/// -------------------------------------------------------
/// 文件：HHJT.Framework.Communication.cs
/// 版本：1.0.0
/// 时间：2011-11-11
/// 作者：刘树康
/// 版权: 上海华虹计通智能系统股份有限公司
/// 说明：通讯实现基础类库
///       1、多线程异步通讯服务器端，客户端
///       2、FTP客户端                  
/// ------------------------------------------------------
///
/// ------------------------------------------------------
/// 更新记录
/// ------------------------------------------------------
///     日期  |  作者  |                说明
/// ------------------------------------------------------
/// 2011-11-11| 刘树康 | 创建
/// ------------------------------------------------------
///</summary>
namespace HHJT.AFC.Base.Comm
{
    public class CommunicationConfig
    {
        #region fields
        protected int m_serverPort = 9002; //端口

        protected int m_receiveBufferSize = 16 * 1024;  // 16 K
        protected int m_sendBufferSize = 16 * 1024;

        protected int m_minDatagramSize = 8;//根据最小报文大小定义
        protected int m_maxDatagramSize = 1024 * 1024;  // 1M 根据最大报文约定定义

        protected int m_maxSessionTimeout = 30;   // 2 minutes

        #endregion

        #region properties
        public int ServerPort
        {
            get { return m_serverPort; }
            set { m_serverPort = value; }
        }

        public int ReceiveBufferSize
        {
            get { return m_receiveBufferSize; }
            set { m_receiveBufferSize = value; }
        }

        public int SendBufferSize
        {
            get { return m_sendBufferSize; }
            set { m_sendBufferSize = value; }
        }


        public int MinDatagramSize
        {
            get { return m_minDatagramSize; }
            set { m_minDatagramSize = value; }
        }

        public int MaxDatagramSize
        {
            get { return m_maxDatagramSize; }
            set { m_maxDatagramSize = value; }
        }

        public int MaxSessionTimeout
        {
            get { return m_maxSessionTimeout; }
            set { m_maxSessionTimeout = value; }
        }
        #endregion

    }

    public class AllowedIPInfo
    {
        public int ID { get; set; }
        public string IPAddr { get; set; }
        public string Desc { get; set; }

    }

    public class SocketServerBase<TSession> : CommunicationConfig, IDisposable, ISessionEvent
        where TSession : SessionBase, new()
    {
        #region  member fields

        private Socket m_serverSocket;
        private bool m_serverClosed = true;
        private bool m_serverListenPaused = false;

        private int m_acceptListenTimeInterval = 25;         // 侦听轮询时间间隔(ms)
        private int m_checkSessionTableTimeInterval = 100;   // 清理Timer的时间间隔(ms)
        private int m_checkDatagramQueueTimeInterval = 100;  // 检查数据包队列时间休息间隔(ms)

        private int m_sessionSequenceNo = 0;  // session 流水号
        private int m_sessionCount;
        private int m_receivedDatagramCount;
        private int m_errorDatagramCount;
        private int m_datagramQueueLength;

        private int m_databaseExceptionCount;
        private int m_serverExceptCount;
        private int m_sessionExceptionCount;

        private int m_maxSessionCount = 1024;

        private int m_maxListenQueueLength = 16;
        private int m_maxSameIPCount = 1;

        private Dictionary<int, TSession> m_sessionTable;

        private bool m_disposed = false;

        private ManualResetEvent m_checkAcceptListenResetEvent;
        private ManualResetEvent m_checkSessionTableResetEvent;
        private ManualResetEvent m_checkDatagramQueueResetEvent;

        private Mutex m_ServerMutex;  // 只能有一个服务器


        private AllowedIPInfo[] m_allowedIP;


        private bool m_checkAllowedIP = true;

        public bool CheckAllowedIP
        {
            get { return m_checkAllowedIP; }
            set { m_checkAllowedIP = value; }
        }
        private bool m_checkSameIP = true;

        public bool CheckSameIP
        {
            get { return m_checkSameIP; }
            set { m_checkSameIP = value; }
        }

        #endregion

        #region  public properties

        public bool Closed
        {
            get { return m_serverClosed; }
        }

        public bool ListenPaused
        {
            get { return m_serverListenPaused; }
        }


        public int ServerExceptionCount
        {
            get { return m_serverExceptCount; }
        }

        public int DatabaseExceptionCount
        {
            get { return m_databaseExceptionCount; }
        }

        public int SessionExceptionCount
        {
            get { return m_sessionExceptionCount; }
        }

        public int SessionCount
        {
            get { return m_sessionCount; }
        }

        public int ReceivedDatagramCount
        {
            get { return m_receivedDatagramCount; }
        }

        public int ErrorDatagramCount
        {
            get { return m_errorDatagramCount; }
        }

        public int DatagramQueueLength
        {
            get { return m_datagramQueueLength; }
        }

        public int AcceptListenTimeInterval
        {
            get { return m_acceptListenTimeInterval; }
            set
            {
                if (value < 10)
                {
                    m_acceptListenTimeInterval = 10;
                }
                else
                {
                    m_acceptListenTimeInterval = value;
                }
            }
        }

        public int CheckSessionTableTimeInterval
        {
            get { return m_checkSessionTableTimeInterval; }
            set
            {
                if (value < 10)
                {
                    m_checkSessionTableTimeInterval = 10;
                }
                else
                {
                    m_checkSessionTableTimeInterval = value;
                }
            }
        }

        public int CheckDatagramQueueTimeInterval
        {
            get { return m_checkDatagramQueueTimeInterval; }
            set
            {
                if (value < 10)
                {
                    m_checkDatagramQueueTimeInterval = 10;
                }
                else
                {
                    m_checkDatagramQueueTimeInterval = value;
                }
            }
        }

        public int MaxSessionCount
        {
            get { return m_maxSessionCount; }
        }


        public int MaxListenQueueLength
        {
            get { return m_maxListenQueueLength; }
            set
            {
                if (value <= 1)
                {
                    m_maxListenQueueLength = 2;
                }
                else
                {
                    m_maxListenQueueLength = value;
                }
            }
        }


        public int MaxSameIPCount
        {
            get { return m_maxSameIPCount; }
            set
            {
                if (value < 1)
                {
                    m_maxSameIPCount = 1;
                }
                else
                {
                    m_maxSameIPCount = value;
                }
            }
        }

        public AllowedIPInfo[] AllowedIP
        {
            get { return m_allowedIP; }
            set { m_allowedIP = value; }
        }

        public Collection<SessionCoreInfo> SessionCoreInfoCollection
        {
            get
            {
                Collection<SessionCoreInfo> sessionCollection = new Collection<SessionCoreInfo>();
                lock (m_sessionTable)
                {
                    foreach (TSession session in m_sessionTable.Values)
                    {
                        sessionCollection.Add((SessionCoreInfo)session);
                    }
                }
                return sessionCollection;
            }
        }

        #endregion

        #region  class events

        public event EventHandler ServerStarted;
        public event EventHandler ServerClosed;
        public event EventHandler ServerListenPaused;
        public event EventHandler ServerListenResumed;
        public event EventHandler<ExceptionEventArgs> ServerException;

        public event EventHandler<ExceptionEventArgs> SessionRejected;
        public event EventHandler<SessionEventArgs> SessionConnected;
        public event EventHandler<SessionEventArgs> SessionDisconnected;
        public event EventHandler<SessionEventArgs> SessionTimeout;

        public event EventHandler<SessionEventArgs> DatagramUndersizeError;
        public event EventHandler<SessionEventArgs> DatagramOversizeError;
        public event EventHandler<SessionExceptionEventArgs> SessionReceiveException;
        public event EventHandler<SessionExceptionEventArgs> SessionSendException;
        public event EventHandler<SessionEventArgs> DatagramAccepted;
        public event EventHandler<SessionEventArgs> DatagramError;
        public event EventHandler<SessionEventArgs> DatagramHandled;

        public event EventHandler<ExceptionEventArgs> ShowDebugMessage;

        #endregion

        #region  class constructor

        public SocketServerBase()
        {
            this.Initiate(m_maxSessionCount, m_receiveBufferSize, m_sendBufferSize);
        }



        public SocketServerBase(int maxSessionCount, int receiveBufferSize, int sendBufferSize)
        {
            this.Initiate(maxSessionCount, receiveBufferSize, sendBufferSize);
        }


        private void Initiate(int maxSessionCount, int receiveBufferSize, int sendBufferSize)
        {
            bool canCreateNew;
            m_ServerMutex = new Mutex(true, "HHJT_SERVER", out canCreateNew);
            if (!canCreateNew)
            {
                throw new Exception("Can not create two or more server!");
            }

            m_maxSessionCount = maxSessionCount;
            m_receiveBufferSize = receiveBufferSize;
            m_sendBufferSize = sendBufferSize;

            //           m_bufferManager = new BufferManager(maxSessionCount, receiveBufferSize, sendBufferSize);
            m_sessionTable = new Dictionary<int, TSession>();

            m_checkAcceptListenResetEvent = new ManualResetEvent(true);
            m_checkSessionTableResetEvent = new ManualResetEvent(true);
            m_checkDatagramQueueResetEvent = new ManualResetEvent(true);

        }


        ~SocketServerBase()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            if (!m_disposed)
            {
                m_disposed = true;
                this.Close();
                this.Dispose(true);
                GC.SuppressFinalize(this);  // Finalize 不会第二次执行
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)  // 对象正在被显示释放, 不是执行 Finalize()
            {
                m_sessionTable = null;  // 释放托管资源
            }

            if (m_ServerMutex != null)
            {
                m_ServerMutex.Close();
            }

            if (m_checkAcceptListenResetEvent != null)
            {
                m_checkAcceptListenResetEvent.Close();  // 释放非托管资源
            }

            if (m_checkSessionTableResetEvent != null)
            {
                m_checkSessionTableResetEvent.Close();
            }

            if (m_checkDatagramQueueResetEvent != null)
            {
                m_checkDatagramQueueResetEvent.Close();
            }

            //if (m_bufferManager != null)
            //{
            //    m_bufferManager.Clear();
            //}
        }

        #endregion

        #region  public methods

        public bool Start()
        {
            if (!m_serverClosed)
            {
                return true;
            }

            m_serverClosed = true;  // 在其它方法中要判断该字段
            m_serverListenPaused = true;

            this.Close();
            this.ClearCountValues();

            try
            {

                if (!this.CreateServerSocket()) return false;
                if (!ThreadPool.QueueUserWorkItem(this.CheckDatagramQueue)) return false;
                if (!ThreadPool.QueueUserWorkItem(this.StartServerListen)) return false;
                if (!ThreadPool.QueueUserWorkItem(this.CheckSessionTable)) return false;

                m_serverClosed = false;
                m_serverListenPaused = false;

                this.OnServerStarted();
            }
            catch (Exception err)
            {
                this.OnServerException(err);
            }
            return !m_serverClosed;
        }

        public void PauseListen()
        {
            m_serverListenPaused = true;
            this.OnServerListenPaused();
        }

        public void ResumeListen()
        {
            m_serverListenPaused = false;
            this.OnServerListenResumed();
        }

        public void Stop()
        {
            this.Close();
        }

        public void CloseSession(int sessionId)
        {
            TSession session = null;
            lock (m_sessionTable)
            {
                if (m_sessionTable.ContainsKey(sessionId))  // 包含该会话 ID
                {
                    session = (TSession)m_sessionTable[sessionId];
                }
            }

            if (session != null)
            {
                session.SetInactive();
            }
        }

        public void CloseAllSessions()
        {
            lock (m_sessionTable)
            {
                foreach (TSession session in m_sessionTable.Values)
                {
                    session.SetInactive();
                }
            }
        }

        public bool SendToSession(int sessionId, byte[] datagramBytes)
        {
            TSession session = null;
            lock (m_sessionTable)
            {
                if (m_sessionTable.ContainsKey(sessionId))
                {
                    session = (TSession)m_sessionTable[sessionId];
                    //Console.WriteLine("Find{0}",sessionId);
                }
            }

            if (session != null)
            {
                return session.SendDatagram(datagramBytes);
            }
            return false;
        }

        public bool SendToSession(string ipAddr, byte[] datagramBytes)
        {
            TSession session = null;
            lock (m_sessionTable)
            {
                // session = (TSession)(m_sessionTable.Values.(elem => elem.IP == ipAddr));//(TSession)m_sessionTable[sessionId];
                foreach (var item in m_sessionTable.Values)
                {
                    if (item.IP == ipAddr)
                    {
                        session = item;
                    }
                }
            }

            if (session != null)
            {
                return session.SendDatagram(datagramBytes);
            }

            return false;
        }

        public void SendToAllSessions(byte[] datagramBytes)
        {
            lock (m_sessionTable)
            {
                foreach (TSession session in m_sessionTable.Values)
                {
                    session.SendDatagram(datagramBytes);
                }
            }
        }

        #endregion

        #region  private methods

        private void Close()
        {
            if (m_serverClosed)
            {
                return;
            }

            m_serverClosed = true;
            m_serverListenPaused = true;

            m_checkAcceptListenResetEvent.WaitOne();  // 等待3个线程
            m_checkSessionTableResetEvent.WaitOne();
            m_checkDatagramQueueResetEvent.WaitOne();


            if (m_sessionTable != null)
            {
                lock (m_sessionTable)
                {
                    foreach (TSession session in m_sessionTable.Values)
                    {
                        session.Close();
                    }
                }
            }

            this.CloseServerSocket();

            if (m_sessionTable != null)  // 清空会话列表
            {
                lock (m_sessionTable)
                {
                    m_sessionTable.Clear();
                }
            }

            this.OnServerClosed();
        }

        private void ClearCountValues()
        {
            m_sessionCount = 0;
            m_receivedDatagramCount = 0;
            m_errorDatagramCount = 0;
            m_datagramQueueLength = 0;

            m_databaseExceptionCount = 0;
            m_serverExceptCount = 0;
            m_sessionExceptionCount = 0;
        }

        private bool CreateServerSocket()
        {
            try
            {
                m_serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                //IPAddress ipAddress = IPAddress.Parse("192.168.0.101");
                m_serverSocket.Bind(new IPEndPoint(IPAddress.Any, m_serverPort));
                m_serverSocket.Listen(m_maxListenQueueLength);

                return true;
            }
            catch (Exception err)
            {
                this.OnServerException(err);
                return false;
            }
        }

        private bool CheckSocketIP(string clientIP)
        {
            // IPEndPoint iep = (IPEndPoint)clientSocket.RemoteEndPoint;
            // string ip = iep.Address.ToString();
            if (!this.m_checkSameIP) return true;

            if (clientIP.Substring(0, 7) == "127.0.0")   // local machine
            {
                return true;
            }

            lock (m_sessionTable)
            {
                //int sameIPCount = 0;


                if (m_sessionTable.Values.Any<TSession>(session => session.IP == clientIP))
                {
                    //   sameIPCount++;
                    //   if (sameIPCount > m_maxSameIPCount)
                    //    {
                    return false;
                    //  }
                }


            }
            return true;
        }

        private bool CheckIsAllowedIP(string clientIP)
        {
            // IPEndPoint iep = (IPEndPoint)clientSocket.RemoteEndPoint;
            // string ip = iep.Address.ToString();
            if (!m_checkAllowedIP) return true;


            if (clientIP.Substring(0, 7) == "127.0.0")   // local machine
            {
                return true;
            }

            if (m_allowedIP == null) //列表为空则不做判断
            {
                return true;
            }
            else
            {
                if (m_allowedIP.Any(elem => elem.IPAddr == clientIP)) return true;

            }
            return false;
        }

        /// <summary>
        /// 侦听客户端连接请求
        /// </summary>
        private void StartServerListen(object state)
        {
            m_checkAcceptListenResetEvent.Reset();
            Socket clientSocket = null;

            while (!m_serverClosed)
            {
                if (m_serverListenPaused)  // pause server
                {
                    this.CloseServerSocket();
                    Thread.Sleep(m_acceptListenTimeInterval);
                    continue;
                }

                if (m_serverSocket == null)
                {
                    this.CreateServerSocket();
                    continue;
                }

                try
                {
                    if (m_serverSocket.Poll(m_acceptListenTimeInterval, SelectMode.SelectRead))
                    {
                        // 频繁关闭、启动时，这里容易产生错误（提示套接字只能有一个）
                        clientSocket = m_serverSocket.Accept();

                        if (clientSocket != null && clientSocket.Connected)
                        {
                            IPEndPoint iep = (IPEndPoint)clientSocket.RemoteEndPoint;
                            string clientIP = iep.Address.ToString();

                            //Console.WriteLine("sessionCount:"+m_sessionCount);

                            if (m_sessionCount >= m_maxSessionCount)  // 会话连接数超过允许限额
                            {
                                this.OnSessionRejected(new ExceptionEventArgs(string.Format("IP:{0} rejected, over max session count", clientIP))); // 拒绝登录请求
                                this.CloseClientSocket(clientSocket);
                            }
                            else if (!this.CheckSocketIP(clientIP)) // 当前列表已经存在该 IP 地址
                            {
                                this.OnSessionRejected(new ExceptionEventArgs(string.Format("IP:{0} rejected, same ip connected again", clientIP))); // 拒绝登录请求
                                this.CloseClientSocket(clientSocket);
                            }
                            else if (!this.CheckIsAllowedIP(clientIP)) //判断是否为允许IP
                            {
                                this.OnSessionRejected(new ExceptionEventArgs(string.Format("IP:{0} rejected, the ip is not registered", clientIP))); // 拒绝登录请求
                                this.CloseClientSocket(clientSocket);

                            }
                            else
                            {
                                this.AddSession(clientSocket);  // 添加到队列中, 并调用异步接收方法
                            }
                        }
                        else  // clientSocket is null or connected == false
                        {
                            this.CloseClientSocket(clientSocket);
                        }
                    }
                }
                catch (Exception)  // 侦听连接的异常频繁, 不捕获异常
                {
                    this.CloseClientSocket(clientSocket);
                }
            }

            m_checkAcceptListenResetEvent.Set();
        }

        private void CloseServerSocket()
        {
            if (m_serverSocket == null)
            {
                return;
            }

            try
            {
                lock (m_sessionTable)
                {
                    if (m_sessionTable != null && m_sessionTable.Count > 0)
                    {
                        // 可能结束服务器端的 AcceptClientConnect 的 Poll
                        //                        m_serverSocket.Shutdown(SocketShutdown.Both);  // 有连接才关
                    }
                }
                m_serverSocket.Close();
            }
            catch (Exception err)
            {
                this.OnServerException(err);
            }
            finally
            {
                m_serverSocket = null;
            }
        }

        /// <summary>
        /// 强制关闭客户端请求时的 Socket
        /// </summary>
        private void CloseClientSocket(Socket socket)
        {
            if (socket != null)
            {
                try
                {
                    socket.Shutdown(SocketShutdown.Both);
                    socket.Close();
                }
                catch (Exception) { }  // 强制关闭, 忽略错误
            }
        }

        /// <summary>
        /// 增加一个会话对象
        /// </summary>
        private void AddSession(Socket clientSocket)
        {
            Interlocked.Increment(ref m_sessionSequenceNo);

            TSession session = new TSession();
            session.Initiate(m_sessionSequenceNo, clientSocket, m_minDatagramSize, m_maxDatagramSize, m_allowedIP);



            session.DatagramUndersizeError += new EventHandler<SessionEventArgs>(this.OnDatagramDelimiterError);
            session.DatagramOversizeError += new EventHandler<SessionEventArgs>(this.OnDatagramOversizeError);
            session.DatagramError += new EventHandler<SessionEventArgs>(this.OnDatagramError);
            session.DatagramAccepted += new EventHandler<SessionEventArgs>(this.OnDatagramAccepted);
            session.DatagramHandled += new EventHandler<SessionEventArgs>(this.OnDatagramHandled);
            session.SessionReceiveException += new EventHandler<SessionExceptionEventArgs>(this.OnSessionReceiveException);
            session.SessionSendException += new EventHandler<SessionExceptionEventArgs>(this.OnSessionSendException);

            //session.ShowDebugMessage += new EventHandler<TExceptionEventArgs>(this.ShowDebugMessage);

            lock (m_sessionTable)
            {
                m_sessionTable.Add(session.ID, session);
            }
            session.ReceiveDatagram();

            this.OnSessionConnected(session);
        }

        /// <summary>
        /// 资源清理线程, 分若干步完成
        /// </summary>
        private void CheckSessionTable(object state)
        {
            m_checkSessionTableResetEvent.Reset();

            while (!m_serverClosed)
            {
                lock (m_sessionTable)
                {
                    List<int> sessionIDList = new List<int>();

                    foreach (TSession session in m_sessionTable.Values)
                    {
                        if (m_serverClosed)
                        {
                            break;
                        }

                        if (session.State == ESessionState.Inactive)  // 分三步清除一个 Session
                        {
                            session.Shutdown();  // 第一步: shutdown, 结束异步事件
                        }
                        else if (session.State == ESessionState.Shutdown)
                        {
                            session.Close();  // 第二步: Close
                        }
                        else if (session.State == ESessionState.Closed)
                        {
                            sessionIDList.Add(session.ID);
                            this.DisconnectSession(session);

                        }
                        else // 正常的会话 Active
                        {
                            session.CheckTimeout(m_maxSessionTimeout); // 判超时，若是则标记
                        }
                    }

                    foreach (int id in sessionIDList)  // 统一清除
                    {
                        m_sessionTable.Remove(id);
                    }

                    sessionIDList.Clear();
                }

                Thread.Sleep(m_checkSessionTableTimeInterval);
            }

            m_checkSessionTableResetEvent.Set();
        }

        /// <summary>
        /// 数据包处理线程
        /// </summary>
        private void CheckDatagramQueue(object state)
        {
            m_checkDatagramQueueResetEvent.Reset();

            while (!m_serverClosed)
            {
                lock (m_sessionTable)
                {
                    foreach (TSession session in m_sessionTable.Values)
                    {
                        if (m_serverClosed)
                        {
                            break;
                        }

                        session.HandleDatagram();
                    }
                }
                Thread.Sleep(m_checkDatagramQueueTimeInterval);
            }

            m_checkDatagramQueueResetEvent.Set();
        }

        private void DisconnectSession(TSession session)
        {
            if (session.DisconnectType == EDisconnectType.Normal)
            {
                this.OnSessionDisconnected(session);
            }
            else if (session.DisconnectType == EDisconnectType.Timeout)
            {
                this.OnSessionTimeout(session);
            }
            else if (session.DisconnectType == EDisconnectType.SendException)
            {
                this.OnSessionDisconnected(session);
            }
            else if (session.DisconnectType == EDisconnectType.RecvExcepteion)
            {
                this.OnSessionDisconnected(session);
            }
        }

        /// <summary>
        /// 输出调试信息
        /// </summary>
        private void OnShowDebugMessage(string message)
        {
            if (this.ShowDebugMessage != null)
            {
                ExceptionEventArgs e = new ExceptionEventArgs(message);
                this.ShowDebugMessage(this, e);
            }
        }

        #endregion

        #region  protected virtual methods



        protected virtual void OnSessionRejected(ExceptionEventArgs args)
        {
            EventHandler<ExceptionEventArgs> handler = this.SessionRejected;
            if (handler != null)
            {
                handler(this, args);
            }
        }

        protected virtual void OnSessionConnected(TSession session)
        {
            Interlocked.Increment(ref m_sessionCount);
            //Console.WriteLine("OnSessionConnected-" + m_sessionCount);
            EventHandler<SessionEventArgs> handler = this.SessionConnected;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionDisconnected(TSession session)
        {
            Interlocked.Decrement(ref m_sessionCount);

            //Console.WriteLine("OnSessionDisconnected-" + m_sessionCount);
            EventHandler<SessionEventArgs> handler = this.SessionDisconnected;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionTimeout(TSession session)
        {
            Interlocked.Decrement(ref m_sessionCount);
            //Console.WriteLine("OnSessionTimeout-" + m_sessionCount);

            EventHandler<SessionEventArgs> handler = this.SessionTimeout;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionReceiveException(object sender, SessionExceptionEventArgs e)
        {
            //Interlocked.Decrement(ref m_sessionCount);
            Interlocked.Increment(ref m_sessionExceptionCount);
            //Console.WriteLine("OnSessionReceiveException-" + m_sessionCount);

            EventHandler<SessionExceptionEventArgs> handler = this.SessionReceiveException;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnSessionSendException(object sender, SessionExceptionEventArgs e)
        {
            //Interlocked.Decrement(ref m_sessionCount);
            Interlocked.Increment(ref m_sessionExceptionCount);
            //Console.WriteLine("OnSessionSendException-" + m_sessionCount);

            EventHandler<SessionExceptionEventArgs> handler = this.SessionSendException;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnServerException(Exception err)
        {
            Interlocked.Increment(ref m_serverExceptCount);

            EventHandler<ExceptionEventArgs> handler = this.ServerException;
            if (handler != null)
            {
                ExceptionEventArgs e = new ExceptionEventArgs(err);
                handler(this, e);
            }
        }

        protected virtual void OnServerStarted()
        {
            EventHandler handler = this.ServerStarted;
            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }
        }

        protected virtual void OnServerListenPaused()
        {
            EventHandler handler = this.ServerListenPaused;
            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }
        }

        protected virtual void OnServerListenResumed()
        {
            EventHandler handler = this.ServerListenResumed;
            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }
        }

        protected virtual void OnServerClosed()
        {
            EventHandler handler = this.ServerClosed;
            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }
        }

        protected virtual void OnDatagramDelimiterError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_errorDatagramCount);

            EventHandler<SessionEventArgs> handler = this.DatagramUndersizeError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramOversizeError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_errorDatagramCount);

            EventHandler<SessionEventArgs> handler = this.DatagramOversizeError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramAccepted(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramAccepted;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_errorDatagramCount);
            Interlocked.Decrement(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramHandled(object sender, SessionEventArgs e)
        {
            Interlocked.Decrement(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramHandled;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        #endregion

    }

    /// <summary>
    /// 会话类核心成员
    /// </summary>
    public class SessionCoreInfo
    {
        #region  member fields

        private int m_index;
        private int m_id;
        private string m_ip = string.Empty;
        private string m_name = string.Empty;
        private ESessionState m_state = ESessionState.Active;
        private EDisconnectType m_disconnectType = EDisconnectType.Normal;

        private DateTime m_loginTime;
        private DateTime m_lastSessionTime;

        #endregion

        #region  public properties

        public int Index
        {
            get { return m_index; }
            protected set { m_index = value; }
        }

        public int ID
        {
            get { return m_id; }
            set { m_id = value; }
        }

        public string IP
        {
            get { return m_ip; }
            protected set { m_ip = value; }
        }

        /// <summary>
        /// 数据包发送者的名称/编号
        /// </summary>
        public string Name
        {
            get { return m_name; }
            protected set { m_name = value; }
        }

        public DateTime LoginTime
        {
            get { return m_loginTime; }
            protected set
            {
                m_loginTime = value;
                m_lastSessionTime = value;
            }
        }

        public DateTime LastSessionTime
        {
            get { return m_lastSessionTime; }
            protected set { m_lastSessionTime = value; }
        }

        public ESessionState State
        {
            get { return m_state; }
            protected set
            {
                lock (this)
                {
                    m_state = value;
                }
            }
        }

        public EDisconnectType DisconnectType
        {
            get { return m_disconnectType; }
            protected set
            {
                lock (this)
                {
                    m_disconnectType = value;
                }
            }
        }

        #endregion
    }

    /// <summary>
    /// 会话基类(抽象类, 必须实现其 AnalyzeDatagram 方法)
    /// </summary>
    public abstract class SessionBase : SessionCoreInfo, ISessionEvent
    {
        #region  member fields

        private Socket m_socket;

        private int m_minDatagramSize;
        private int m_maxDatagramSize;


        private byte[] m_receiveBuffer;
        private byte[] m_sendBuffer;

        private byte[] m_datagramBuffer;

        private Queue<byte[]> m_datagramQueue;

        #endregion

        #region class events

        public event EventHandler<SessionExceptionEventArgs> SessionReceiveException;
        public event EventHandler<SessionExceptionEventArgs> SessionSendException;
        public event EventHandler<SessionEventArgs> DatagramUndersizeError;
        public event EventHandler<SessionEventArgs> DatagramOversizeError;
        public event EventHandler<SessionEventArgs> DatagramAccepted;
        public event EventHandler<SessionEventArgs> DatagramError;
        public event EventHandler<SessionEventArgs> DatagramHandled;

        public event EventHandler<ExceptionEventArgs> ShowDebugMessage;

        #endregion

        #region  class constructor
        /// <summary>
        /// 作泛型参数类型时, 必须有无参构造函数
        /// </summary>
        protected SessionBase() { }

        /// <summary>
        /// 替构造函数初始化对象
        /// </summary>
        public virtual void Initiate(int index, Socket socket, int minDatagramsize, int maxDatagramsize, AllowedIPInfo[] allowedIPInfo)
        {
            base.Index = index;
            base.LoginTime = DateTime.Now;
            base.LastSessionTime = DateTime.Now;

            m_receiveBuffer = new byte[maxDatagramsize * 2];
            m_sendBuffer = new byte[maxDatagramsize];

            m_minDatagramSize = minDatagramsize;
            m_maxDatagramSize = maxDatagramsize;


            m_socket = socket;


            m_datagramQueue = new Queue<byte[]>();

            if (allowedIPInfo != null)
            {

                IPEndPoint iep = m_socket.RemoteEndPoint as IPEndPoint;
                base.IP = iep.Address.ToString();

                var vIPInfo = allowedIPInfo.First(ipInfo => ipInfo.IPAddr == IP);
                if (vIPInfo != null)
                {
                    base.ID = vIPInfo.ID;
                    base.Name = vIPInfo.Desc;
                }
            }
            //if (m_socket != null)
            //{
            //    IPEndPoint iep = m_socket.RemoteEndPoint as IPEndPoint;
            //    if (iep != null)
            //    {
            //        base.IP = iep.Address.ToString();
            //        if (base.IP == "192.168.0.101")//for test
            //        {
            //            ID = 1;
            //        }
            //        else if (base.IP == "192.168.0.106")
            //        {
            //            ID = 2;
            //        }
            //        else
            //        {
            //            ID = 0;
            //        }
            //    }
            //}
        }

        /// <summary>
        /// 客户端初始化
        /// </summary>
        /// <param name="socket"></param>
        /// <param name="minDatagramsize"></param>
        /// <param name="maxDatagramsize"></param>
        /// <param name="serverIP"></param>
        public virtual void Initiate(Socket socket, int minDatagramsize, int maxDatagramsize, string serverIP)
        {
            ///base.Index = index;
            base.LoginTime = DateTime.Now;
            base.LastSessionTime = DateTime.Now;
            m_receiveBuffer = new byte[maxDatagramsize * 2];
            m_sendBuffer = new byte[maxDatagramsize];

            m_minDatagramSize = minDatagramsize;
            m_maxDatagramSize = maxDatagramsize;


            m_socket = socket;


            m_datagramQueue = new Queue<byte[]>();

            base.IP = serverIP;

        }


        #endregion



        #region  public methods

        public void Shutdown()
        {
            lock (this)
            {
                if (this.State != ESessionState.Inactive || m_socket == null)  // Inactive 状态才能 Shutdown
                {
                    return;
                }

                this.State = ESessionState.Shutdown;
                try
                {
                    m_socket.Shutdown(SocketShutdown.Both);  // 目的：结束异步事件

                }
                catch (Exception) { }
            }
        }

        public void Close()
        {
            lock (this)
            {
                if (this.State != ESessionState.Shutdown || m_socket == null)  // Shutdown 状态才能 Close
                {
                    return;
                }

                m_datagramBuffer = null;

                if (m_datagramQueue != null)
                {
                    while (m_datagramQueue.Count > 0)
                    {

                        m_datagramQueue.Dequeue();
                    }
                    m_datagramQueue.Clear();
                }

                //m_bufferManager.FreeBufferBlockIndex(m_bufferBlockIndex);

                try
                {
                    this.State = ESessionState.Closed;
                    m_socket.Close();
                }
                catch (Exception) { }
            }
        }

        public void SetInactive()
        {
            lock (this)
            {
                if (this.State == ESessionState.Active)
                {
                    this.State = ESessionState.Inactive;
                    this.DisconnectType = EDisconnectType.Normal;
                }
            }
        }

        public void SetInactiveTimeout()
        {
            lock (this)
            {
                if (this.State == ESessionState.Active)
                {
                    this.State = ESessionState.Inactive;
                    this.DisconnectType = EDisconnectType.Timeout;
                }
            }
        }

        public void HandleDatagram()
        {
            lock (this)
            {
                if (this.State != ESessionState.Active || m_datagramQueue.Count == 0)
                {
                    return;
                }
                while (m_datagramQueue.Count > 0)
                {
                    byte[] datagramBytes = m_datagramQueue.Dequeue();
                    this.AnalyzeDatagram(datagramBytes);
                }
            }
        }

        public void ReceiveDatagram()
        {
            lock (this)
            {
                if (this.State != ESessionState.Active)
                {
                    return;
                }

                try  // 一个客户端连续做连接 或连接后立即断开，容易在该处产生错误，系统不认为是错误
                {
                    // 开始接受来自该客户端的数据
                    //int bufferOffset = m_bufferManager.GetReceivevBufferOffset(m_bufferBlockIndex);
                    m_socket.BeginReceive(m_receiveBuffer, 0, m_receiveBuffer.Length, SocketFlags.None, this.EndReceiveDatagram, this);

                }
                catch (Exception err)  // 读 Socket 异常，准备关闭该会话
                {
                    this.DisconnectType = EDisconnectType.RecvExcepteion;
                    this.State = ESessionState.Inactive;

                    this.OnSessionReceiveException(err);
                }
            }
        }

        public bool SendDatagram(byte[] datagramBytes)
        {
            lock (this)
            {
                if (this.State != ESessionState.Active)
                {
                    return false;
                }

                try
                {
                    m_socket.BeginSend(datagramBytes, 0, datagramBytes.Length, SocketFlags.None, this.EndSendDatagram, this);
                }
                catch (Exception err)  // 写 socket 异常，准备关闭该会话
                {
                    this.DisconnectType = EDisconnectType.SendException;
                    this.State = ESessionState.Inactive;
                    this.OnSessionSendException(err);
                    return false;
                }
            }
            return true;
        }

        public void CheckTimeout(int maxSessionTimeout)
        {
            TimeSpan ts = DateTime.Now.Subtract(this.LastSessionTime);
            int elapsedSecond = Math.Abs((int)ts.TotalSeconds);

            if (elapsedSecond > maxSessionTimeout)  // 超时，则准备断开连接
            {

                this.SetInactiveTimeout();  // 标记为将关闭、准备断开
                // this.DisconnectType = EDisconnectType.Timeout;
            }
        }

        #endregion

        #region  private methods

        /// <summary>
        /// 发送数据完成处理函数, iar 为目标客户端 Session
        /// </summary>
        private void EndSendDatagram(IAsyncResult iar)
        {
            lock (this)
            {
                if (this.State != ESessionState.Active)
                {
                    return;
                }

                if (!m_socket.Connected)
                {
                    this.SetInactive();
                    return;
                }

                try
                {
                    m_socket.EndSend(iar);
                    iar.AsyncWaitHandle.Close();
                }
                catch (Exception err)  // 写 socket 异常，准备关闭该会话
                {
                    this.DisconnectType = EDisconnectType.SendException;
                    this.State = ESessionState.Inactive;

                    this.OnSessionSendException(err);
                }
            }
        }

        private void EndReceiveDatagram(IAsyncResult iar)
        {
            lock (this)
            {
                if (this.State != ESessionState.Active)
                {
                    return;
                }

                if (!m_socket.Connected)
                {
                    this.SetInactive();
                    return;
                }

                try
                {
                    // Shutdown 时将调用 ReceiveData，此时也可能收到 0 长数据包
                    int readBytesLength = m_socket.EndReceive(iar);
                    iar.AsyncWaitHandle.Close();

                    if (readBytesLength == 0)
                    {
                        this.DisconnectType = EDisconnectType.Normal;
                        this.State = ESessionState.Inactive;
                    }
                    else  // 正常数据包
                    {
                        this.LastSessionTime = DateTime.Now;
                        //Console.WriteLine("lasttime:"+LastSessionTime.ToString("HH:mm:ss"));
                        // 合并报文，按报文头、尾字符标志抽取报文，将包交给数据处理器
                        this.ResolveSessionBuffer(readBytesLength);
                        this.ReceiveDatagram();  // 继续接收
                    }
                }
                catch (Exception err)  // 读 socket 异常，关闭该会话，系统不认为是错误（这种错误可能太多）
                {
                    if (this.State == ESessionState.Active)
                    {
                        this.DisconnectType = EDisconnectType.RecvExcepteion;
                        this.State = ESessionState.Inactive;

                        this.OnSessionReceiveException(err);
                    }
                }
            }
        }

        /// <summary>
        /// 拷贝接收缓冲区的数据到数据缓冲区（即多次读一个包文）
        /// </summary>
        private void CopyToDatagramBuffer(int start, int length)
        {
            int datagramLength = 0;
            if (m_datagramBuffer != null)
            {
                datagramLength = m_datagramBuffer.Length;
            }

            Array.Resize(ref m_datagramBuffer, datagramLength + length);  // 调整长度（m_datagramBuffer 为 null 不出错）
            Array.Copy(m_receiveBuffer, start, m_datagramBuffer, datagramLength, length);  // 拷贝到数据包缓冲区
        }

        #endregion

        #region protected methods

        /// <summary>
        /// 提取包时与包规则紧密相关，根据实际规则重定义
        /// </summary>
        protected virtual void ResolveSessionBuffer(int readBytesLength)
        {
            //拷贝数据至DatagramBuffer
            this.CopyToDatagramBuffer(0, readBytesLength);

            //报文长度（不含长度本身）+报文
            int datagramLength;
            byte[] datagramBytes;

            while (true)
            {
                datagramLength = (short)(((m_datagramBuffer[0]) << 8) | m_datagramBuffer[1]);

                if (datagramLength > m_maxDatagramSize)//超过最大报文长度
                {
                    this.OnDatagramOversizeError();
                    m_datagramBuffer = null;  // 丢弃全部数据
                }
                else if (datagramLength < m_minDatagramSize)//小于最小报文长度
                {
                    this.OnDatagramDelimiterError();
                    m_datagramBuffer = null;
                }
                else if (datagramLength <= m_datagramBuffer.Length - 2)//报文长度在数据缓冲许可范围内
                {
                    datagramBytes = new byte[datagramLength + 2];
                    Array.Copy(m_datagramBuffer, 0, datagramBytes, 0, datagramBytes.Length);
                    m_datagramQueue.Enqueue(datagramBytes);

                    int bytesLeft = m_datagramBuffer.Length - datagramBytes.Length;
                    if (bytesLeft == 0)
                    {
                        m_datagramBuffer = null;
                        break;
                    }
                    else
                    {
                        byte[] datagramLeft = new byte[bytesLeft];
                        Array.Copy(m_datagramBuffer, datagramBytes.Length, datagramLeft, 0, bytesLeft);
                        m_datagramBuffer = datagramLeft;
                    }
                    // m_datagramBuffer = Array.
                }
                else
                {
                    break;
                }
            }

            /* // 上次留下包文非空, 必然含开始字符<
             bool hasHeadDelimiter = (m_datagramBuffer != null);
            
             int headDelimiter = 1;
             int tailDelimiter = 1;

             int bufferOffset = m_bufferManager.GetReceivevBufferOffset(m_bufferBlockIndex);
             int start = bufferOffset;   // m_receiveBuffer 缓冲区中包开始位置
             int length = 0;  // 已经搜索的接收缓冲区长度

             int subIndex = bufferOffset;  // 缓冲区下标
             while (subIndex < readBytesLength + bufferOffset)
             {
                 if (m_receiveBuffer[subIndex] == '<')  // 数据包开始字符<，前面包文作废
                 {
                     if (hasHeadDelimiter || length > 0)  // 如果 < 前面有数据，则认为错误包
                     {
                         this.OnDatagramDelimiterError();
                     }

                     m_datagramBuffer = null;  // 清空包缓冲区，开始一个新的包

                     start = subIndex;         // 新包起点，即<所在位置
                     length = headDelimiter;   // 新包的长度（即<）
                     hasHeadDelimiter = true;  // 新包有开始字符
                 }
                 else if (m_receiveBuffer[subIndex] == '>')  // 数据包的结束字符>
                 {
                     if (hasHeadDelimiter)  // 两个缓冲区中有开始字符<
                     {
                         length += tailDelimiter;  // 长度包括结束字符“>”

                         this.GetDatagramFromBuffer(start, length); // >前面的为正确格式的包

                         start = subIndex + tailDelimiter;  // 新包起点（一般一次处理将结束循环）
                         length = 0;  // 新包长度
                     }
                     else  // >前面没有开始字符，此时认为结束字符>为一般字符，待后续的错误包处理
                     {
                         length++;  //  hasHeadDelimiter = false;
                     }
                 }
                 else  // 即非 < 也非 >， 是一般字符，长度 + 1
                 {
                     length++;
                 }
                 ++subIndex;
             }

             if (length > 0)  // 剩下的待处理串，分两种情况
             {
                 int mergedLength = length;
                 if (m_datagramBuffer != null)
                 {
                     mergedLength += m_datagramBuffer.Length;
                 }
                
                 // 剩下的包文含首字符且不超长，转存到包文缓冲区中，待下次处理
                 if (hasHeadDelimiter && mergedLength <= m_maxDatagramSize)
                 {
                     this.CopyToDatagramBuffer(start, length);
                 }
                 else  // 不含首字符或超长
                 {
                     this.OnDatagramOversizeError();
                     m_datagramBuffer = null;  // 丢弃全部数据
                 }
             }*/
        }

        /// <summary>
        /// Session重写入口, 基本功能: 
        /// 1) 判断包有效性与包类型(注意：包带起止符号); 
        /// 2) 分解包中的各字段数据
        /// 3) 校验包及其数据有效性
        /// 4) 发送确认消息给客户端(调用方法 SendDatagram())
        /// 5) 存储包数据到数据库中
        /// 6) 存储包原文到数据库中(可选)
        /// 7) 补充字段m_name, 表示数据包发送者的名称/编号
        /// 8) 其它相关方法
        /// </summary>
        protected abstract void AnalyzeDatagram(byte[] datagramBytes);

        /*  protected virtual void GetDatagramFromBuffer(int startPos, int len)
          {
              byte[] datagramBytes;
              if (m_datagramBuffer != null)
              {
                  datagramBytes = new byte[len + m_datagramBuffer.Length];
                  Array.Copy(m_datagramBuffer, 0, datagramBytes, 0, m_datagramBuffer.Length);  // 先拷贝 Session 的数据缓冲区的数据
                  Array.Copy(m_receiveBuffer, startPos, datagramBytes, m_datagramBuffer.Length, len);  // 再拷贝 Session 的接收缓冲区的数据
              }
              else
              {
                  datagramBytes = new byte[len];
                  Array.Copy(m_receiveBuffer, startPos, datagramBytes, 0, len);  // 再拷贝 Session 的接收缓冲区的数据
              }

              if (m_datagramBuffer != null)
              {
                  m_datagramBuffer = null;
              }

              m_datagramQueue.Enqueue(datagramBytes);
          }*/

        protected virtual void OnDatagramDelimiterError()
        {
            EventHandler<SessionEventArgs> handler = this.DatagramUndersizeError;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(this);
                handler(this, e);
            }
        }

        protected virtual void OnDatagramOversizeError()
        {
            EventHandler<SessionEventArgs> handler = this.DatagramOversizeError;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(this);
                handler(this, e);
            }
        }

        protected virtual void OnDatagramAccepted()
        {
            EventHandler<SessionEventArgs> handler = this.DatagramAccepted;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(this);
                handler(this, e);
            }
        }

        protected virtual void OnDatagramError()
        {
            EventHandler<SessionEventArgs> handler = this.DatagramError;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(this);
                handler(this, e);
            }
        }

        protected virtual void OnDatagramHandled()
        {
            EventHandler<SessionEventArgs> handler = this.DatagramHandled;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(this);
                handler(this, e);
            }
        }

        protected virtual void OnSessionReceiveException(Exception err)
        {
            EventHandler<SessionExceptionEventArgs> handler = this.SessionReceiveException;
            if (handler != null)
            {
                SessionExceptionEventArgs e = new SessionExceptionEventArgs(err, this);
                handler(this, e);
            }
        }

        protected virtual void OnSessionSendException(Exception err)
        {
            EventHandler<SessionExceptionEventArgs> handler = this.SessionSendException;
            if (handler != null)
            {
                SessionExceptionEventArgs e = new SessionExceptionEventArgs(err, this);
                handler(this, e);
            }
        }

        protected void OnShowDebugMessage(string message)
        {
            if (this.ShowDebugMessage != null)
            {
                ExceptionEventArgs e = new ExceptionEventArgs(message);
                this.ShowDebugMessage(this, e);
            }
        }

        #endregion
    }

    public interface ISessionEvent
    {
        event EventHandler<SessionExceptionEventArgs> SessionReceiveException;
        event EventHandler<SessionExceptionEventArgs> SessionSendException;
        event EventHandler<SessionEventArgs> DatagramUndersizeError;
        event EventHandler<SessionEventArgs> DatagramOversizeError;
        event EventHandler<SessionEventArgs> DatagramAccepted;
        event EventHandler<SessionEventArgs> DatagramError;
        event EventHandler<SessionEventArgs> DatagramHandled;
    }

    public class ExceptionEventArgs : EventArgs
    {
        private string m_exceptionMessage;

        public ExceptionEventArgs(Exception exception)
        {
            m_exceptionMessage = exception.Message;
        }

        public ExceptionEventArgs(string exceptionMessage)
        {
            m_exceptionMessage = exceptionMessage;
        }

        public string ExceptionMessage
        {
            get { return m_exceptionMessage; }
        }
    }

    public class SessionEventArgs : EventArgs
    {
        SessionCoreInfo m_sessionBaseInfo;

        public SessionEventArgs(SessionCoreInfo sessionCoreInfo)
        {
            m_sessionBaseInfo = sessionCoreInfo;
        }

        public SessionCoreInfo SessionBaseInfo
        {
            get { return m_sessionBaseInfo; }
        }
    }

    public class SessionExceptionEventArgs : SessionEventArgs
    {
        private string m_exceptionMessage;

        public SessionExceptionEventArgs(Exception exception, SessionCoreInfo sessionCoreInfo)
            : base(sessionCoreInfo)
        {
            m_exceptionMessage = exception.Message;
        }

        public string ExceptionMessage
        {
            get { return m_exceptionMessage; }
        }
    }

    public enum EDisconnectType
    {
        Normal,     // disconnect normally
        Timeout,    // disconnect because of timeout
        SendException,   // disconnect because of Send exception
        RecvExcepteion //disconnect because of recv exception
    }

    public enum ESessionState
    {
        Active,    // state is active
        Inactive,  // session is inactive and will be closed
        Shutdown,  // session is shutdownling
        Closed     // session is closed
    }

    public class SocketClientBase<TSession> : CommunicationConfig, IDisposable, ISessionEvent
        where TSession : SessionBase, new()
    {
        #region fields

        private int m_checkSessionTimeInterval = 100;   // 清理Timer的时间间隔(ms)
        private int m_checkDatagramQueueTimeInterval = 100;  // 检查数据包队列时间休息间隔(ms)

        private int m_clientExceptCount;
        private int m_sessionExceptionCount;
        private int m_receivedDatagramCount;
        private int m_errorDatagramCount;
        private int m_datagramQueueLength;

        private bool m_isConnected = false;
        private bool m_clientClosed = false;

        private int m_maxSessionCount = 64;

        private ManualResetEvent m_checkSessionTableResetEvent;
        private ManualResetEvent m_checkDatagramQueueResetEvent;

        private TSession m_session;

        private string m_serverIP = "198.222.233.43";

        private bool m_disposed = false;

        #endregion

        #region properties
        public bool IsConnected
        {
            get { return m_isConnected; }
        }

        public int MaxSessionCount
        {
            get { return m_maxSessionCount; }
            set { m_maxSessionCount = value; }
        }

        public string ServerIP
        {
            get { return m_serverIP; }
            set { m_serverIP = value; }
        }
        #endregion

        #region events
        public event EventHandler ClientClosed;
        public event EventHandler<ExceptionEventArgs> ClientException;
        public event EventHandler<SessionEventArgs> SessionConnected;
        public event EventHandler<SessionEventArgs> SessionDisconnected;
        public event EventHandler<SessionEventArgs> SessionTimeout;

        public event EventHandler<SessionEventArgs> DatagramUndersizeError;
        public event EventHandler<SessionEventArgs> DatagramOversizeError;
        public event EventHandler<SessionExceptionEventArgs> SessionReceiveException;
        public event EventHandler<SessionExceptionEventArgs> SessionSendException;
        public event EventHandler<SessionEventArgs> DatagramAccepted;
        public event EventHandler<SessionEventArgs> DatagramError;
        public event EventHandler<SessionEventArgs> DatagramHandled;
        #endregion

        #region  class constructor

        public SocketClientBase()
        {
            this.Initiate(m_maxSessionCount, m_receiveBufferSize, m_sendBufferSize);
        }

        public SocketClientBase(int maxSessionCount, int receiveBufferSize, int sendBufferSize)
        {
            this.Initiate(maxSessionCount, receiveBufferSize, sendBufferSize);
        }

        private void Initiate(int maxSessionCount, int receiveBufferSize, int sendBufferSize)
        {

            m_maxSessionCount = maxSessionCount;
            m_receiveBufferSize = receiveBufferSize;
            m_sendBufferSize = sendBufferSize;

            m_checkSessionTableResetEvent = new ManualResetEvent(true);
            m_checkDatagramQueueResetEvent = new ManualResetEvent(true);

        }

        ~SocketClientBase()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            if (!m_disposed)
            {
                m_disposed = true;
                this.Close();
                this.Dispose(true);
                GC.SuppressFinalize(this);  // Finalize 不会第二次执行
            }
        }

        protected virtual void Dispose(bool disposing)
        {

            if (m_checkSessionTableResetEvent != null)
            {
                m_checkSessionTableResetEvent.Close();
            }

            if (m_checkDatagramQueueResetEvent != null)
            {
                m_checkDatagramQueueResetEvent.Close();
            }

        }

        #endregion

        public bool Connect()
        {
            if (m_isConnected)//重新连接
            {
                this.Close();
                this.ClearCountValues();
            }



            try
            {
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPEndPoint iep = new IPEndPoint(IPAddress.Parse(m_serverIP), m_serverPort);

                socket.Connect(iep);

                m_session = new TSession();

                m_session.Initiate(socket, m_minDatagramSize, m_maxDatagramSize, m_serverIP);
                m_session.DatagramUndersizeError += new EventHandler<SessionEventArgs>(this.OnDatagramDelimiterError);
                m_session.DatagramOversizeError += new EventHandler<SessionEventArgs>(this.OnDatagramOversizeError);
                m_session.DatagramError += new EventHandler<SessionEventArgs>(this.OnDatagramError);
                m_session.DatagramAccepted += new EventHandler<SessionEventArgs>(this.OnDatagramAccepted);
                m_session.DatagramHandled += new EventHandler<SessionEventArgs>(this.OnDatagramHandled);
                m_session.SessionReceiveException += new EventHandler<SessionExceptionEventArgs>(this.OnSessionReceiveException);
                m_session.SessionSendException += new EventHandler<SessionExceptionEventArgs>(this.OnSessionSendException);

                m_isConnected = true;
                m_session.ReceiveDatagram();

                if (!ThreadPool.QueueUserWorkItem(this.CheckDatagramQueue)) return false;
                if (!ThreadPool.QueueUserWorkItem(this.CheckSession)) return false;

                m_clientClosed = false;
                this.OnConnected();
            }
            catch (Exception err)
            {
                this.OnClientException(err);
                return false;
            }
            return true;
        }

        public void Close()
        {
            if (m_clientClosed)
            {
                return;
            }

            if (m_session == null)
            {
                m_clientClosed = true;
                return;
            }

            lock (m_session)
            {
                m_session.SetInactive();
                m_session.Shutdown();
                m_session.Close();
            }

            m_clientClosed = true;

            // 等待2个线程
            m_checkSessionTableResetEvent.WaitOne();
            m_checkDatagramQueueResetEvent.WaitOne();



            this.OnClientClosed();
        }

        private void ClearCountValues()
        {

            m_receivedDatagramCount = 0;
            m_errorDatagramCount = 0;
            m_datagramQueueLength = 0;
            m_sessionExceptionCount = 0;
        }


        /// <summary>
        /// 资源清理线程, 分若干步完成
        /// </summary>
        private void CheckSession(object state)
        {
            m_checkSessionTableResetEvent.Reset();

            while (!m_clientClosed)
            {
                lock (m_session)
                {
                    if (m_session.State == ESessionState.Inactive)  // 分三步清除一个 Session
                    {
                        m_session.Shutdown();  // 第一步: shutdown, 结束异步事件
                        this.m_isConnected = false;
                    }
                    else if (m_session.State == ESessionState.Shutdown)
                    {
                        m_session.Close();  // 第二步: Close
                        this.m_isConnected = false;

                    }
                    else if (m_session.State == ESessionState.Closed)
                    {
                        this.DisconnectSession(m_session);
                        this.m_isConnected = false;

                        m_clientClosed = true;
                    }
                    else // 正常的会话 Active
                    {
                        m_session.CheckTimeout(m_maxSessionTimeout); // 判超时，若是则标记
                    }


                }

                Thread.Sleep(m_checkSessionTimeInterval);
            }

            m_checkSessionTableResetEvent.Set();
        }

        private void DisconnectSession(TSession session)
        {
            if (session.DisconnectType == EDisconnectType.Normal)
            {
                this.OnSessionDisconnected(session);
            }
            else if (session.DisconnectType == EDisconnectType.Timeout)
            {
                this.OnSessionTimeout(session);
            }
        }

        /// <summary>
        /// 数据包处理线程
        /// </summary>
        private void CheckDatagramQueue(object state)
        {
            m_checkDatagramQueueResetEvent.Reset();

            while (!m_clientClosed)
            {
                lock (m_session)
                {
                    m_session.HandleDatagram();

                }
                Thread.Sleep(m_checkDatagramQueueTimeInterval);
            }

            m_checkDatagramQueueResetEvent.Set();
        }

        public bool SendToSession(byte[] datagramBytes)
        {

            if (m_session != null)
            {
                return m_session.SendDatagram(datagramBytes);
            }
            return false;
        }

        protected virtual void OnConnected()
        {

            this.OnSessionConnected(m_session);
        }

        protected virtual void OnClientException(Exception err)
        {
            Interlocked.Increment(ref m_clientExceptCount);

            EventHandler<ExceptionEventArgs> handler = this.ClientException;
            if (handler != null)
            {
                ExceptionEventArgs e = new ExceptionEventArgs(err);
                handler(this, e);
            }
        }

        protected virtual void OnClientClosed()
        {
            EventHandler handler = this.ClientClosed;
            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }
        }

        protected virtual void OnSessionConnected(TSession session)
        {

            EventHandler<SessionEventArgs> handler = this.SessionConnected;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionDisconnected(TSession session)
        {

            EventHandler<SessionEventArgs> handler = this.SessionDisconnected;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionTimeout(TSession session)
        {

            EventHandler<SessionEventArgs> handler = this.SessionTimeout;
            if (handler != null)
            {
                SessionEventArgs e = new SessionEventArgs(session);
                handler(this, e);
            }
        }

        protected virtual void OnSessionReceiveException(object sender, SessionExceptionEventArgs e)
        {
            Interlocked.Increment(ref m_sessionExceptionCount);

            EventHandler<SessionExceptionEventArgs> handler = this.SessionReceiveException;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnSessionSendException(object sender, SessionExceptionEventArgs e)
        {
            Interlocked.Increment(ref m_sessionExceptionCount);

            EventHandler<SessionExceptionEventArgs> handler = this.SessionSendException;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramDelimiterError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_errorDatagramCount);

            EventHandler<SessionEventArgs> handler = this.DatagramUndersizeError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramOversizeError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_errorDatagramCount);

            EventHandler<SessionEventArgs> handler = this.DatagramOversizeError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramAccepted(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_receivedDatagramCount);
            Interlocked.Increment(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramAccepted;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramError(object sender, SessionEventArgs e)
        {
            Interlocked.Increment(ref m_errorDatagramCount);
            Interlocked.Decrement(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramError;
            if (handler != null)
            {
                handler(this, e);
            }
        }

        protected virtual void OnDatagramHandled(object sender, SessionEventArgs e)
        {
            Interlocked.Decrement(ref m_datagramQueueLength);

            EventHandler<SessionEventArgs> handler = this.DatagramHandled;
            if (handler != null)
            {
                handler(this, e);
            }
        }

    }

}
