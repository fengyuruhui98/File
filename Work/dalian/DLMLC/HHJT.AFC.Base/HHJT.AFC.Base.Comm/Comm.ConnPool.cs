using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace HHJT.AFC.Base.Comm
{
    public enum SocketDisconnectedType
    {
        NormalDisconnected, //正常远程关闭
        TimeOut,            //超时关闭
        RemoteEnforce,            //强制远程退出
        BrokenSocket,       //Socket异常
        Other               //其他异常
    }

    public class ClientConnectionPool
    {
        // Creates a synchronized wrapper around the Queue.
        private Queue SyncdQ = Queue.Synchronized(new Queue());

        public void Enqueue(ClientHandler client)
        {
            SyncdQ.Enqueue(client);
        }

        public ClientHandler Dequeue()
        {

            return (ClientHandler)(SyncdQ.Dequeue());
        }

        public int Count
        {
            get { return SyncdQ.Count; }
        }

        public object SyncRoot
        {
            get { return SyncdQ.SyncRoot; }
        }

    } // class ClientConnectionPool

    public class ClientHandler
    {

        protected TcpClient ClientSocket;
        protected NetworkStream networkStream;
        bool ContinueProcess = false;
        protected byte[] bytes; 		// Data buffer for incoming data.
        protected List<byte> bytesBuffer = new List<byte>();
        protected byte[] dataProcess;
        //private StringBuilder sb = new StringBuilder(); // Received data string.
        //private string data = null; // Incoming data from the client.
        protected string _clientAddr;


        public string ClientAddr
        {
            get { return _clientAddr; }
            //set { _clientAddr = value; }
        }

        public ClientHandler(TcpClient ClientSocket)
        {
            ClientSocket.ReceiveTimeout = 5000; // 100 miliseconds
            this.ClientSocket = ClientSocket;

            _clientAddr = (ClientSocket.Client.RemoteEndPoint as IPEndPoint).Address.ToString();

            networkStream = ClientSocket.GetStream();
            bytes = new byte[ClientSocket.ReceiveBufferSize];
            ContinueProcess = true;
            Connected();
        }

        public ClientHandler(TcpClient ClientSocket, int rcvTimeout)
        {
            ClientSocket.ReceiveTimeout = rcvTimeout;
            this.ClientSocket = ClientSocket;

            _clientAddr = (ClientSocket.Client.RemoteEndPoint as IPEndPoint).Address.ToString();

            networkStream = ClientSocket.GetStream();
            bytes = new byte[ClientSocket.ReceiveBufferSize];
            ContinueProcess = true;
            Connected();
        }

        public void Process()
        {

            try
            {
                int BytesRead = networkStream.Read(bytes, 0, (int)bytes.Length);
                if (BytesRead == bytes.Length) //有剩余
                {
                    bytesBuffer.AddRange(bytes.Take(BytesRead));
                }
                else if (BytesRead > 0 && BytesRead < bytes.Length) //处理数据
                {
                    bytesBuffer.AddRange(bytes.Take(BytesRead));

                    dataProcess = bytesBuffer.ToArray();
                    bytesBuffer.Clear();
                    ProcessDataReceived();
                }
                else if (BytesRead == 0) //远程断开连接
                {
                    networkStream.Close();
                    ClientSocket.Close();
                    ContinueProcess = false;

                    DisConnected(SocketDisconnectedType.NormalDisconnected);//远程连接主动关闭
                }
            }
            catch (IOException ex)
            {
                // All the data has arrived; put it in response.
                if (ex.InnerException is SocketException)
                {
                    var v = ex.InnerException as SocketException;
                    if (v.ErrorCode == 10060)//timeout
                    {
                        Close();
                        DisConnected(SocketDisconnectedType.TimeOut);//超时关闭
                    }
                    else if (v.ErrorCode == 10054)//disconnected by remote exception
                    {
                        Close();
                        DisConnected(SocketDisconnectedType.RemoteEnforce); //客户端异常退出
                    }
                }
            }
            catch (SocketException)
            {
                Close();
                DisConnected(SocketDisconnectedType.BrokenSocket); //socket异常
            }
            catch (Exception)
            {
                Close();
                DisConnected(SocketDisconnectedType.Other); //其他异常
            }

        }  // Process()

        protected virtual void ProcessDataReceived()
        {

        }

        protected virtual void DisConnected(SocketDisconnectedType disconnetedType) { }

        protected virtual void Connected()
        {

        }

        public void Close()
        {
            networkStream.Close();
            ClientSocket.Close();
            ContinueProcess = false;

        }

        public bool Alive
        {
            get
            {
                return ContinueProcess;
            }
        }

    } // class ClientHandler 

    public class ClientService
    {

        const int NUM_OF_THREAD = 100;

        private ClientConnectionPool ConnectionPool;
        private bool ContinueProcess = false;
        private Thread[] ThreadTask = new Thread[NUM_OF_THREAD];

        public ClientService(ClientConnectionPool ConnectionPool)
        {
            this.ConnectionPool = ConnectionPool;
        }

        public void Start()
        {
            ContinueProcess = true;
            // Start threads to handle Client Task
            for (int i = 0; i < ThreadTask.Length; i++)
            {
                ThreadTask[i] = new Thread(new ThreadStart(this.Process));
                ThreadTask[i].Start();
            }
        }

        private void Process()
        {
            while (ContinueProcess)
            {

                ClientHandler client = null;
                lock (ConnectionPool.SyncRoot)
                {
                    if (ConnectionPool.Count > 0)
                        client = ConnectionPool.Dequeue();
                }
                if (client != null)
                {
                    client.Process(); // Provoke client
                    // if client still connect, schedufor later processingle it 
                    if (client.Alive)
                        ConnectionPool.Enqueue(client);
                }

                Thread.Sleep(100);
            }
        }

        public void Stop()
        {
            ContinueProcess = false;
            for (int i = 0; i < ThreadTask.Length; i++)
            {
                if (ThreadTask[i] != null && ThreadTask[i].IsAlive)
                    ThreadTask[i].Join();
            }

            // Close all client connections
            while (ConnectionPool.Count > 0)
            {
                ClientHandler client = ConnectionPool.Dequeue();
                client.Close();
                Console.WriteLine("Client connection is closed!");
            }
        }

    } // class ClientService

    public abstract class SynchronousSocketListener
    {

        private int _portNum = 10116;
        //private string _localAddr = "127.0.0.1";
        private bool _quitListener;


        public int PortNum
        {
            get { return _portNum; }
            set { _portNum = value; }
        }

        //public string LocalAddr
        //{
        //    get { return _localAddr; }
        //    set { _localAddr = value; }
        //}

        public bool QuitListener
        {
            get { return _quitListener; }
            set { _quitListener = value; }
        }

        public event EventHandler<ExceptionEventArgs> EventErro;

        public void StartListening()
        {

            ClientService ClientTask;

            // Client Connections Pool
            ClientConnectionPool ConnectionPool = new ClientConnectionPool();

            // Client Task to handle client requests
            ClientTask = new ClientService(ConnectionPool);

            ClientTask.Start();

            //IPAddress localAddr = IPAddress.Parse(_localAddr);
            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, _portNum);
            TcpListener listener = new TcpListener(ipEndPoint);
            try
            {
                listener.Start();

                // int ClientNbr = 0;

                // Start listening for connections.
                //Console.WriteLine("Waiting for a connection...");
                while (!_quitListener)
                {

                    TcpClient handler = listener.AcceptTcpClient();

                    if (handler != null)
                    {
                        ConnectionPool.Enqueue(InitClientHandler(handler));//new ClientHandler(handler));
                    }
                    else
                        break;
                }
                listener.Stop();

                // Stop client requests handling
                ClientTask.Stop();


            }
            catch (Exception e)
            {
                //Console.WriteLine(e.ToString());
                if (EventErro != null)
                {
                    EventErro(this, new ExceptionEventArgs(e.Message));
                }
            }

            //Console.WriteLine("\nHit enter to continue...");
            //Console.Read();
        }


        public abstract ClientHandler InitClientHandler(TcpClient tcpClient);

    }
}
