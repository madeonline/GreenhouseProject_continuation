using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading;
using System.Security.Permissions;
using System.Windows.Forms;
using System.ComponentModel;

namespace UROVConfig
{
    /// <summary>
    /// обработчик события "получена строка из порта"
    /// </summary>
    /// <param name="line"></param>
    public delegate void TransportDataReceived(byte[] data);
    /// <summary>
    /// Обработчик события подсоединения к порту
    /// </summary>
    /// <param name="success"></param>
    /// <param name="message"></param>
    public delegate void ConnectResult(bool success, string message);

    public delegate void TransportDisconnect(ITransport transport);
     
    public abstract class ITransport
    {  
        /// <summary>
        /// соединяется с чем-либо
        /// </summary>
        public abstract void Connect();
        /// <summary>
        /// состояние соединения
        /// </summary>
        /// <returns></returns>
        public abstract bool Connected();

        /// <summary>
        /// передаёт строку
        /// </summary>
        /// <param name="line"></param>
        /// <returns></returns>
        public abstract bool WriteLine(string line);

        /// <summary>
        /// отсоединяемся
        /// </summary>
        public abstract void Disconnect();

        public TransportDataReceived OnDataReceived;
        public ConnectResult OnConnect;
        public TransportDisconnect OnDisconnect;
    }

    public class SerialPortTransport : ITransport
    {
        private SerialPort port = null;
        private Thread openPortThread = null;
        private bool hasWriteError = false;
  
        private void TryOpenPort(object o)
        {
            SerialPort s = (SerialPort)o;

            //TODO: Эту строчку добавил, чтобы пересбрасывало порт
            s.DtrEnable = true;

            bool succ = false;
            string message = "";
            try
            {
                s.Open();
                while (!s.IsOpen)
                {
                    Thread.Sleep(100);
                }
                succ = true;
            }
            catch (UnauthorizedAccessException e)
            {
                message = e.Message;
            }
            catch (System.IO.IOException e)
            {
                message = e.Message;
            }
            catch (Exception e)
            {
                message = e.Message;
            }

            var mydelegate = new Action<bool, string>(DoOnConnect);
            mydelegate.Invoke(succ, message);

        }
        private void DoOnConnect(bool succ, string message)
        {
            openPortThread = null;
            if (this.OnConnect != null)
            {
                this.OnConnect(succ, message);
            }
        }
        public override void  Connect()
        {
            this.hasWriteError = false;
            openPortThread = new Thread(TryOpenPort);
            openPortThread.Start(this.port);
 	        
        }

        public override void Disconnect()
        {
            try
            {
                
                if (this.port.IsOpen)
                    this.port.Close();

                while (port.IsOpen) { Application.DoEvents(); }

                CallDisconnectEvent();

            }
            catch { }
        }

        private void CallDisconnectEvent()
        {
            if (!this.port.IsOpen)
            {
                if (this.OnDisconnect != null)
                    this.OnDisconnect(this);
            }
        }

        public override bool Connected()
        {
            return this.port.IsOpen && !this.hasWriteError;
        }

        public override bool WriteLine(string line)
        {
            try
            {
                System.Diagnostics.Debug.WriteLine("=> COM: " + line);
                this.port.WriteLine(line);
            }
            catch (Exception)
            {
                this.hasWriteError = true;

                if (this.port.IsOpen)
                {
                    try
                    {
                        this.port.Close();
                    }
                    catch { }
                }

                while (port.IsOpen)
                {
                    Application.DoEvents();
                }
                CallDisconnectEvent();
                return false; 
            }
            return true;
        }



       public SerialPortTransport(string portname, int speed)
        {
            this.port =  new SerialPort(portname);
            this.port.BaudRate = speed;
            this.port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
        }
       

        void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                SerialPort sp = (SerialPort)sender;
                int cnt = sp.BytesToRead;
                if (cnt > 0)
                {
                    byte[] bReceived = new byte[cnt];

                    for (int i = 0; i < cnt; i++)
                        bReceived[i] = (byte) sp.ReadByte();


                    //System.Diagnostics.Debug.WriteLine("<= COM: " + s.TrimEnd());

                    if (this.OnDataReceived != null)
                        this.OnDataReceived(bReceived);

                    Thread.Sleep(10);
                }
            }
            catch (Exception)
            {
               
            }
        }
    }
}
