using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Globalization;

namespace UROVConfig
{
    public partial class MainForm : Form
    {

        private ToolStripMenuItem lastSelectedPort = null;
        private ITransport currentTransport = null;
        private ConnectForm connForm = null;
        private const char PARAM_DELIMITER = '|';
        private const string GET_PREFIX = "GET=";
        private const string SET_PREFIX = "SET=";

        public MainForm()
        {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// закрываем форму ожидания соединения
        /// </summary>
        private void EnsureCloseConnectionForm()
        {
            if (connForm != null)
            {
                connForm.DialogResult = System.Windows.Forms.DialogResult.OK;
            }
            connForm = null;
        }

        void OnTransportDisconnect(ITransport transport)
        {
            this.InitAfterConnect(false);
        }

        /// <summary>
        /// Коннектимся к порту
        /// </summary>
        /// <param name="port">Имя порта</param>
        public void StartConnectToPort(string port)
        {
            System.Diagnostics.Debug.Assert(currentTransport == null);

            int speed = GetConnectionSpeed();

            // создаём новый транспорт
            currentTransport = new SerialPortTransport(port,speed);
            currentTransport.OnConnect = new ConnectResult(OnCOMConnect);
            currentTransport.OnDataReceived = new TransportDataReceived(OnDataFromCOMPort);
            currentTransport.OnDisconnect = new TransportDisconnect(OnTransportDisconnect);

            // коннектимся
            currentTransport.Connect();

        }

        /// <summary>
        /// Обрабатываем строку, пришедшую из транспорта COM-порта
        /// </summary>
        /// <param name="line"></param>
        private void OnDataFromCOMPort(byte[] data)
        {
            if (InvokeRequired)
            {
                Invoke((MethodInvoker)delegate { OnDataFromCOMPort(data); });
                return;
            }
            EnsureCloseConnectionForm(); // закрываем форму коннекта, если она ещё не закрыта


            // обрабатываем данные, полученные из порта
            //ProcessPortAnswer(line.Trim());
            ProcessPortAnswer(data);
        }


        // Буфер под ответ с SD-карты
        private List<byte> SDQueryAnswer = new List<byte>();


        /// <summary>
        /// Буфер под ответ с COM-порта
        /// </summary>
        string COMBuffer = "";

        private void ProcessAnswerLine(string line)
        {
            System.Diagnostics.Debug.WriteLine("<= COM: " + line);

            if (line.StartsWith("UROV v.")) // нашли загрузку ядра, следовательно, можно писать данные
            {
                tbFirmwareVersion.Text = line;
                coreBootFound = true;
            }

            bool isKnownAnswer = line.StartsWith("OK=") || line.StartsWith("ER=");

            this.AddToLog(line, isKnownAnswer); // добавляем данные в лог

            if (!isKnownAnswer) // нам тут ловить нечего
            {
                return;
            }
            Answer a = new Answer(line);
            this.currentCommand.ParseFunction(a);
            this.currentCommand.ParseFunction = null; // освобождаем
            this.currentCommand.CommandToSend = "";

        }

        /// <summary>
        /// Показываем содержимое файла
        /// </summary>
        /// <param name="content"></param>
        private void ShowFileContent(List<byte> content)
        {
            ASCIIEncoding encoding = new ASCIIEncoding();
            string combindedString = encoding.GetString(content.ToArray());
            this.richTextBoxFileView.Text = combindedString;
        }

        /// <summary>
        /// обрабатываем ответ от контроллера
        /// </summary>
        /// <param name="dt">строка, которая пришла из порта</param>
        //private void ProcessPortAnswer(string dt)
        private void ProcessPortAnswer(byte[] dt)
        {
            EnsureCloseConnectionForm(); // закрываем форму коннекта, если она ещё не закрыта

            switch (answerBehaviour)
            {
                case AnswerBehaviour.Normal:
                    {

                        // нормальный режим работы
                        for (int i = 0; i < dt.Length; i++)
                            COMBuffer += (char)dt[i];

                        while (true)
                        {
                            int idx = COMBuffer.IndexOf('\n');
                            if (idx != -1)
                            {
                                string line = COMBuffer.Substring(0, idx);
                                line = line.Trim();
                                COMBuffer = COMBuffer.Substring(idx + 1);

                                ProcessAnswerLine(line);

                            }
                            else
                                break;
                        }
                    }
                    break;

                case AnswerBehaviour.SDCommandFILE:
                    {
                        // вычитываем файл с SD. Признаком окончания файла служат байты [END]\r\n
                        for (int i = 0; i < dt.Length; i++)
                        {
                            this.SDQueryAnswer.Add(dt[i]);
                         //   System.Diagnostics.Debug.Write((char) dt[i]);
                        }

                        if(SDQueryAnswer.Count > 6)
                        {
                            // уже можно проверять на окончание пакета
                            string endOfFile = "";
                            for(int i= SDQueryAnswer.Count-7;i< SDQueryAnswer.Count;i++)
                            {
                                endOfFile += (char)SDQueryAnswer[i];
                            } // for

                            if(endOfFile == "[END]\r\n")
                            {
                                SDQueryAnswer.RemoveRange(SDQueryAnswer.Count - 7, 7);
                                ShowFileContent(SDQueryAnswer);
                                SDQueryAnswer.Clear();

                                this.answerBehaviour = AnswerBehaviour.Normal;
                                this.currentCommand.ParseFunction = null; // освобождаем
                                this.currentCommand.CommandToSend = "";

                                ShowWaitCursor(false);
                                this.btnListSDFiles.Enabled = true;
                                this.treeViewSD.Enabled = true;

                            }

                        } // if
                    }
                    break;

                case AnswerBehaviour.SDCommandLS:
                    {
                        // опрашиваем SD, команда LS
                        for (int i = 0; i < dt.Length; i++)
                            this.SDQueryAnswer.Add(dt[i]);

                        // тут разбиваем по '\n', т.к. ответ на LS - всегда текстовый

                        while(true)
                        { 
                            int newLineIdx = SDQueryAnswer.FindIndex(x => x == '\n');

                            if (newLineIdx != -1)
                            {
                                // нашли перевод строки
                                string lsLine = "";
                                for (int k = 0; k < newLineIdx; k++)
                                {
                                    lsLine += (char)SDQueryAnswer[k];
                                }

                                SDQueryAnswer.RemoveRange(0, newLineIdx + 1);

                                lsLine = lsLine.Trim();
                                System.Diagnostics.Debug.WriteLine("<= COM: " + lsLine);

                               
                                if (lsLine == "[END]") // закончили список!!!
                                {
                                    this.answerBehaviour = AnswerBehaviour.Normal;
                                    this.currentCommand.ParseFunction = null; // освобождаем
                                    this.currentCommand.CommandToSend = "";

                                    this.btnListSDFiles.Enabled = true;

                                    if (currentSDParentNode != null)
                                    {
                                        // и удаляем заглушку...
                                        for (int i = 0; i < currentSDParentNode.Nodes.Count; i++)
                                        {
                                            TreeNode child = currentSDParentNode.Nodes[i];
                                            SDNodeTagHelper tg = (SDNodeTagHelper)child.Tag;
                                            if (tg.Tag == SDNodeTags.TagDummyNode)
                                            {
                                                child.Remove();
                                                break;
                                            }
                                        }
                                    } // if

                                    break;

                                }
                                else
                                {
                                    // продолжаем список
                                    AddToLog(lsLine, false);
                                    AddRecordToSDList(lsLine, currentSDParentNode);
                                }
                            } // if
                            else
                                break;
                        }

                    }
                    break;
            }
            

        }


        private void AddRecordToSDList(string line, TreeNode parent = null)
        {
            TreeNodeCollection nodes = this.treeViewSD.Nodes;
            if (parent != null)
            {
                nodes = parent.Nodes;
                SDNodeTagHelper existingTag = (SDNodeTagHelper)parent.Tag;
                parent.Tag = new SDNodeTagHelper(SDNodeTags.TagFolderNode,existingTag.FileName); // говорим, что мы вычитали это дело
                // и удаляем заглушку...
                for(int i=0;i<parent.Nodes.Count;i++)
                {
                    TreeNode child = parent.Nodes[i];
                    SDNodeTagHelper tg = (SDNodeTagHelper)child.Tag;
                    if(tg.Tag == SDNodeTags.TagDummyNode)
                    {
                        child.Remove();
                        break;
                    }
                }
            }
            bool isDir = false;
            int dirIdx = line.IndexOf("<DIR>");
            if(dirIdx != -1)
            {
                isDir = true;
                line = line.Substring(0, dirIdx).Trim();
            }

            TreeNode node = nodes.Add(getTextFromFileName(line));

            if(isDir)
            {
                node.ImageIndex = 0;
                node.SelectedImageIndex = 0;
                TreeNode dummy = node.Nodes.Add("вычитываем....");
                dummy.Tag = new SDNodeTagHelper(SDNodeTags.TagDummyNode,""); // этот узел потом удалим, при перечитывании
                dummy.ImageIndex = -1;

                node.Tag = new SDNodeTagHelper(SDNodeTags.TagFolderUninitedNode,line); // говорим, что мы не перечитали содержимое папки ещё
            }
            else
            {
                node.ImageIndex = 2;
                node.SelectedImageIndex = node.ImageIndex;
                node.Tag = new SDNodeTagHelper(SDNodeTags.TagFileNode,line);
            }
        }

        /// <summary>
        /// Обработчик события "Пришла строка из транспортного уровня"
        /// </summary>
        /// <param name="a"></param>
        public delegate void DataParseFunction(Answer a);
        public delegate void BeforeSendFunction();

        /// <summary>
        /// Структура команды на обработку
        /// </summary>
        private struct QueuedCommand
        {
            public string CommandToSend;
            public DataParseFunction ParseFunction;
            public BeforeSendFunction BeforeSend;
        };


        /// <summary>
        /// Помещаем команду в очередь на обработку
        /// </summary>
        /// <param name="cmd">Текстовая команда для контроллера</param>
        /// <param name="act">К какому действию команда привязана</param>
        /// <param name="func">Указатель на функцию-обработчик ответа от контроллера</param>
        public void PushCommandToQueue(string cmd, DataParseFunction func, BeforeSendFunction before = null)
        {
            QueuedCommand q = new QueuedCommand();
            q.CommandToSend = cmd;
            q.ParseFunction = func;
            q.BeforeSend = before;
            if (!commandsQueue.Contains(q))
                commandsQueue.Enqueue(q);


        }

        /// <summary>
        /// Возвращаем команду из очереди
        /// </summary>
        /// <param name="outCmd">Команда, которая получена из очереди</param>
        /// <returns>Возвращаем false, если команд в очереди нет, иначе - true</returns>
        private bool GetCommandFromQeue(ref QueuedCommand outCmd)
        {
            if (commandsQueue.Count < 1)
                return false;

            outCmd = commandsQueue.Dequeue();
            return true;
        }

        /// <summary>
        /// Очередь команд
        /// </summary>
        private Queue<QueuedCommand> commandsQueue = new Queue<QueuedCommand>();

        /// <summary>
        /// текущая команда на обработку
        /// </summary>
        private QueuedCommand currentCommand = new QueuedCommand();

        private Color currentLogItemColor = SystemColors.ControlLight;

        /// <summary>
        /// добавляем строку в лог
        /// </summary>
        /// <param name="line">строка для добавления в лог</param>
        private void AddToLog(string line, bool shouldAddCommandName)
        {

            line = line.Trim();
            if (line.Length < 1)
                return;

            lvLog.BeginUpdate();

            int cnt = this.lvLog.Items.Count;
            if (cnt > 100)
            {
                this.lvLog.Items.RemoveAt(0);
            }

            ListViewItem li = this.lvLog.Items.Add(shouldAddCommandName ? currentCommand.CommandToSend : "");
            li.ImageIndex = 0;
            if (currentLogItemColor == SystemColors.Window)
                currentLogItemColor = SystemColors.ControlLight;
            else
                currentLogItemColor = SystemColors.Window;

            string prefix = "=> ";
            if (!shouldAddCommandName)
                prefix = "<= ";

            li.BackColor = currentLogItemColor;
            li.SubItems.Add(prefix + line);
            li.EnsureVisible();

            lvLog.EndUpdate();


        }

        /// <summary>
        /// Обрабатываем результат соединения с портом
        /// </summary>
        /// <param name="succ"></param>
        /// <param name="message"></param>
        private void OnCOMConnect(bool succ, string message)
        {
            if (InvokeRequired)
            {
                Invoke((MethodInvoker)delegate { OnCOMConnect(succ, message); });
                return;
            }

            // обнуляем текущее состояние при переконнекте
            //this.currentCommand.ActionToSet = Actions.None;

            if (succ)
            {
                this.btnConnect.ImageIndex = 1;
                this.btnConnect.Text = "Соединено";
                InitAfterConnect(true);

            }
            else
            {
                EnsureCloseConnectionForm();
                InitAfterConnect(false);
                MessageBox.Show("Не удалось соединиться с портом!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }


            if (succ && connForm != null)
            {
                connForm.lblCurrentAction.Text = "Ждём данные из порта...";
            }
        }



        /// <summary>
        /// Инициализируем необходимое после успешного коннекта
        /// </summary>
        private void InitAfterConnect(bool isConnected)
        {
            Config.Instance.Clear();

            nudDelta1.Value = 0;
            nudDelta2.Value = 0;
            nudDelta3.Value = 0;

            nudMotoresourceCurrent1.Value = 0;
            nudMotoresourceCurrent2.Value = 0;
            nudMotoresourceCurrent3.Value = 0;

            nudMotoresourceMax1.Value = 0;
            nudMotoresourceMax2.Value = 0;
            nudMotoresourceMax3.Value = 0;

            nudPulses1.Value = 0;
            nudPulses2.Value = 0;
            nudPulses3.Value = 0;

            tbControllerTime.Text = "-";
            tbFirmwareVersion.Text = "";
            tbFREERAM.Text = "";

            ResetInductiveSensors();
            ResetVoltage();

           

            if (isConnected)
            {
                this.btnConnect.ImageIndex = 1;
                this.btnConnect.Text = "Соединено";
            }
            else
            {
               this.btnConnect.Text = "Соединить";
                this.btnConnect.ImageIndex = 0; // коннект оборвался
            }

            dateTimeFromControllerReceived = false;
            inSetDateTimeToController = true;
            inSetMotoresourceCurrentToController = true;
            inSetMotoresourceMaxToController = true;
            inSetPulsesToController = true;
            inSetDeltaToController = true;

            // очищаем очередь
            commandsQueue.Clear();
            this.currentCommand.ParseFunction = null;
            this.currentCommand.CommandToSend = "";
            this.coreBootFound = false;

            if (isConnected)
            {
                InitTreeView();

                // добавляем нужные команды для обработки сразу после коннекта
                PushCommandToQueue(GET_PREFIX + "DATETIME", ParseAskDatetime);
                PushCommandToQueue(GET_PREFIX + "FREERAM", ParseAskFreeram);
                PushCommandToQueue(GET_PREFIX + "RES_CUR", ParseAskMotoresurceCurrent, BeforeAskMotoresourceCurrent);
                PushCommandToQueue(GET_PREFIX + "RES_MAX", ParseAskMotoresurceMax, BeforeAskMotoresourceMax);
                PushCommandToQueue(GET_PREFIX + "PULSES", ParseAskPulses, BeforeAskPulses);
                PushCommandToQueue(GET_PREFIX + "DELTA", ParseAskDelta, BeforeAskDelta);
                GetInductiveSensors();
                GetVoltage();
                RescanSD();

                ShowMainSettings();

                tmPeriodicCommandsTimer.Enabled = true;

            }
            else
            {
                ClearAllData();
                ShowStartPanel();

                tmPeriodicCommandsTimer.Enabled = false;
            }



        }
        private void BeforeAskDelta()
        {
            this.inSetDeltaToController = true;
        }

        private void BeforeAskPulses()
        {
            this.inSetPulsesToController = true;
        }

        private void BeforeAskMotoresourceCurrent()
        {
            this.inSetMotoresourceCurrentToController = true;
        }

        private void BeforeAskMotoresourceMax()
        {
            this.inSetMotoresourceMaxToController = true;
        }

        private void ClearAllData()
        {
            this.treeView.Nodes[0].Nodes.Clear();
            this.treeViewSD.Nodes.Clear();
            this.richTextBoxFileView.Clear();
            dateTimeFromControllerReceived = false;
            tbFirmwareVersion.Text = "";
            tbFREERAM.Text = "";
        }

        private void ParseAskDelta(Answer a)
        {
            this.inSetDeltaToController = false;
            if (a.IsOkAnswer)
            {
                try { Config.Instance.Delta1 = Convert.ToInt32(a.Params[1]); } catch { }
                try { Config.Instance.Delta2 = Convert.ToInt32(a.Params[2]); } catch { }
                try { Config.Instance.Delta3 = Convert.ToInt32(a.Params[3]); } catch { }
            }
            else
            {
                Config.Instance.Delta1 = 0;
                Config.Instance.Delta2 = 0;
                Config.Instance.Delta3 = 0;
            }

            nudDelta1.Value = Config.Instance.Delta1;
            nudDelta2.Value = Config.Instance.Delta2;
            nudDelta3.Value = Config.Instance.Delta3;
        }

        private void ParseAskPulses(Answer a)
        {
            this.inSetPulsesToController = false;
            if (a.IsOkAnswer)
            {
                try { Config.Instance.Pulses1 = Convert.ToInt32(a.Params[1]); } catch { }
                try { Config.Instance.Pulses2 = Convert.ToInt32(a.Params[2]); } catch { }
                try { Config.Instance.Pulses3 = Convert.ToInt32(a.Params[3]); } catch { }
            }
            else
            {
                Config.Instance.Pulses1 = 0;
                Config.Instance.Pulses2 = 0;
                Config.Instance.Pulses3 = 0;
            }

            nudPulses1.Value = Config.Instance.Pulses1;
            nudPulses2.Value = Config.Instance.Pulses2;
            nudPulses3.Value = Config.Instance.Pulses3;
        }

        private void ParseAskMotoresurceCurrent(Answer a)
        {
            this.inSetMotoresourceCurrentToController = false;
            if(a.IsOkAnswer)
            {
                try { Config.Instance.MotoresourceCurrent1 = Convert.ToInt32(a.Params[1]);  } catch { }
                try { Config.Instance.MotoresourceCurrent2 = Convert.ToInt32(a.Params[2]); } catch { }
                try { Config.Instance.MotoresourceCurrent3 = Convert.ToInt32(a.Params[3]); } catch { }
            }
            else
            {
                Config.Instance.MotoresourceCurrent1 = 0;
                Config.Instance.MotoresourceCurrent2 = 0;
                Config.Instance.MotoresourceCurrent3 = 0;
            }

            nudMotoresourceCurrent1.Value = Config.Instance.MotoresourceCurrent1;
            nudMotoresourceCurrent2.Value = Config.Instance.MotoresourceCurrent2;
            nudMotoresourceCurrent3.Value = Config.Instance.MotoresourceCurrent3;
        }

        private void ParseAskMotoresurceMax(Answer a)
        {
            this.inSetMotoresourceMaxToController = false;
            if (a.IsOkAnswer)
            {
                try { Config.Instance.MotoresourceMax1 = Convert.ToInt32(a.Params[1]); } catch { }
                try { Config.Instance.MotoresourceMax2 = Convert.ToInt32(a.Params[2]); } catch { }
                try { Config.Instance.MotoresourceMax3 = Convert.ToInt32(a.Params[3]); } catch { }
            }
            else
            {
                Config.Instance.MotoresourceMax1 = 0;
                Config.Instance.MotoresourceMax2 = 0;
                Config.Instance.MotoresourceMax3 = 0;
            }

            nudMotoresourceMax1.Value = Config.Instance.MotoresourceMax1;
            nudMotoresourceMax2.Value = Config.Instance.MotoresourceMax2;
            nudMotoresourceMax3.Value = Config.Instance.MotoresourceMax3;
        }


        private bool dateTimeFromControllerReceived = false;
        private DateTime controllerDateTime = DateTime.MinValue;

        private void ParseAskDatetime(Answer a)
        {
            this.inSetDateTimeToController = false;
            if (a.IsOkAnswer)
            {
                try
                {
                    //TODO: пришло время из контроллера!!!
                    this.controllerDateTime = DateTime.ParseExact(a.Params[1], "dd.MM.yyyy HH:mm:ss", null);
                    dateTimeFromControllerReceived = true;
                }
                catch { }

            }
            else
            {

            }
        }

        private void ParseAskFreeram(Answer a)
        {
            if (a.IsOkAnswer)
            {
                //TODO: пришли данные о свободной памяти!!!
                tbFREERAM.Text = a.Params[1];
            }
            else
            {

            }
        }



        private FeaturesSettings featuresSettings = new FeaturesSettings();

        private void RecreateTreeView()
        {

            this.treeView.Nodes[0].Nodes.Clear();


            TreeNode node = this.treeView.Nodes[0].Nodes.Add("Основные настройки");
            node.Tag = TreeNodeType.MainSettingsNode;
            node.ImageIndex = 5;
            node.SelectedImageIndex = node.ImageIndex;

            if (featuresSettings.SDAvailable)
            {
                TreeNode n = this.treeView.Nodes[0].Nodes.Add("SD-карта");
                n.Tag = TreeNodeType.SDSettingsNode;
                n.ImageIndex = 8;
                n.SelectedImageIndex = n.ImageIndex;

            }

        }




        private string ByteAsHexString(byte b)
        {
            return "0x" + b.ToString("X2");
        }

 

        private void ResizeLogColumns()
        {
            this.logColumn1.Width = this.lvLog.ClientRectangle.Width / 2 - SystemInformation.VerticalScrollBarWidth / 2 - 2;
            this.logColumn2.Width = this.logColumn1.Width;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            InitSubstitutions();

            EnumSerialPorts();

            ResizeLogColumns();

            this.toolStrip.ImageList = toolbarImages;
            this.btnConnect.ImageIndex = 0;
            this.btnSetDateTime.ImageIndex = 3;
            this.btnAbout.ImageIndex = 5;
            this.btnDisconnect.ImageIndex = 6;


            plMainSettings.Dock = DockStyle.Fill;
            this.plSDSettings.Dock = DockStyle.Fill;
            //TODO: тут остальные панели !!!

            ShowStartPanel();

            Application.Idle += new EventHandler(Application_Idle);
        }

        private void InitTreeView()
        {
            RecreateTreeView();

            this.treeView.Nodes[0].ExpandAll();
            

        }


        /// <summary>
        /// Обработчик простоя
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Application_Idle(object sender, EventArgs e)
        {

            bool bConnected = IsConnected();

            btnSetDateTime.Enabled = bConnected && !inSetDateTimeToController;
            btnSetDateTime2.Enabled = bConnected && !inSetDateTimeToController;
            this.btnDisconnect.Enabled = bConnected && currentTransport != null;

            this.btnSetMotoresourceCurrent.Enabled = bConnected && !inSetMotoresourceCurrentToController;
            this.btnSetMotoresourceMax.Enabled = bConnected && !inSetMotoresourceMaxToController;
            this.btnSetPulses.Enabled = bConnected && !inSetPulsesToController;
            this.btnSetDelta.Enabled = bConnected && !inSetDeltaToController;

            if (!bConnected) // порт закрыт
            {

                if (this.lastSelectedPort != null)
                {
                    this.lastSelectedPort.Checked = false;
                }

                

            }
            else
            {

                if (lastSelectedPort != null)
                    if (!lastSelectedPort.Checked)
                        lastSelectedPort.Checked = true;

            }
        }

        /// <summary>
        /// Перечисляем COM-порты
        /// </summary>
        private void EnumSerialPorts()
        {
            miPort.DropDownItems.Clear();

            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {

                ToolStripMenuItem ti = new ToolStripMenuItem(port);

                ti.MergeIndex = 1;
                ti.AutoSize = true;
                ti.ImageScaling = ToolStripItemImageScaling.None;
                ti.Tag = port;
                ti.Click += ConnectToSelectedComPort;
                ti.CheckOnClick = false;



                miPort.DropDownItems.Add(ti);


            }
        }


        private void ConnectToSelectedComPort(object sender, EventArgs e)
        {

            ToolStripMenuItem mi = (ToolStripMenuItem)sender;

            if (mi.Checked)
                return;


            Disconnect();
            if (this.lastSelectedPort != null)
            {

                this.lastSelectedPort.Checked = false;
            }

            this.lastSelectedPort = mi;
            mi.Checked = true;

            DoConnect((string)mi.Tag);

        }

        private int GetConnectionSpeed()
        {
            foreach(ToolStripMenuItem tmi in this.portSpeedToolStripMenuItem.DropDownItems)
            {
                if(tmi.Checked)
                {
                    int speed = Convert.ToInt32(tmi.Text);
                    return speed;
                }
            }

            return 115200;
        }

        /// <summary>
        /// Начинаем коннектиться к порту
        /// </summary>
        /// <param name="port">имя порта</param>
        private void DoConnect(string port)
        {
            dateTimeFromControllerReceived = false; // не получили ещё текущее время с контроллера
            controllerDateTime = DateTime.MinValue; // устанавливаем минимальное значение даты            

            connForm = new ConnectForm(false);
            connForm.SetMainFormAndPort(this, port);
            connForm.ShowDialog();
        }

        /// <summary>
        /// Отсоединяемся
        /// </summary>
        private void Disconnect() // отсоединяемся от порта
        {
            if (currentTransport != null)
                currentTransport.Disconnect();

            currentTransport = null;



        }

        /// <summary>
        /// Проверяем, соединены ли мы с контроллером
        /// </summary>
        /// <returns></returns>
        public bool IsConnected()
        {
            if (currentTransport != null)
                return currentTransport.Connected();

            return false;
        }



        /// <summary>
        /// Класс ответа от контроллера
        /// </summary>
        public class Answer
        {
            /// <summary>
            /// флаг, что ответ положительный
            /// </summary>
            public bool IsOkAnswer;
            /// <summary>
            /// список параметров
            /// </summary>
            public string[] Params;
            /// <summary>
            /// сырые данные, полученные от контроллера
            /// </summary>
            public string RawData;

            /// <summary>
            /// очищает все переменные
            /// </summary>
            private void Clear()
            {
                IsOkAnswer = false;
                Params = null;
                RawData = "";
            }

            /// <summary>
            /// конструирует параметры из строки
            /// </summary>
            /// <param name="dt"></param>
            public void Parse(string dt)
            {
                Clear();
                RawData = dt;

                int idx = dt.IndexOf("OK=");
                if (idx != -1)
                {
                    this.IsOkAnswer = true;
                    dt = dt.Substring(3).Trim();
                    this.Params = dt.Split(PARAM_DELIMITER);
                }
                idx = dt.IndexOf("ER=");
                if (idx != -1)
                {
                    this.IsOkAnswer = false;
                    dt = dt.Substring(3).Trim();
                    this.Params = dt.Split(PARAM_DELIMITER);
                }

            }
            /// <summary>
            /// кол-во параметров
            /// </summary>
            public int ParamsCount
            {
                get { return this.Params == null ? 0 : this.Params.Length; }
            }
            /// <summary>
            /// конструктор
            /// </summary>
            /// <param name="dt">строка для разбора</param>
            public Answer(string dt)
            {
                this.Clear();
                this.Parse(dt);

            }
        }

        private void tmProcessCommandsTimer_Tick(object sender, EventArgs e)
        {
            ProcessNextCommand();
        }

        private AnswerBehaviour answerBehaviour = AnswerBehaviour.Normal;

        private void ProcessNextCommand()
        {
            if (!GrantToProcess())
                return;

            if (!this.GetCommandFromQeue(ref this.currentCommand))
                return;

            System.Diagnostics.Debug.Assert(this.currentTransport != null);

            this.answerBehaviour = AnswerBehaviour.Normal;

            currentCommand.BeforeSend?.Invoke();

            this.currentTransport.WriteLine(currentCommand.CommandToSend);

        }

        private bool coreBootFound = false;


        /// <summary>
        /// Проверяем, можем ли мы работать
        /// </summary>
        /// <returns>возвращаем true, если работать можем</returns>
        private bool GrantToProcess()
        {

            if (!coreBootFound)
                return false;

            if (!this.IsConnected()) // нет коннекта
            {

                return false;
            }


            if (this.currentCommand.ParseFunction != null) // чем-то заняты
                return false;


            return true;
        }

        private void tmGetSensorsData_Tick(object sender, EventArgs e)
        {
            if (!GrantToProcess())
                return;

            PushCommandToQueue(GET_PREFIX + "FREERAM", ParseAskFreeram);

        }

        private void ShowStartPanel()
        {
            this.plStartPanel.Dock = DockStyle.Fill;
            this.plStartPanel.BringToFront();
        }

        private void ShowMainSettings()
        {
            this.plMainSettings.BringToFront();
            treeView.SelectedNode = treeView.Nodes[0].Nodes[0];
        }

        private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TreeNode selectedNode = e.Node;
            if (selectedNode == null || selectedNode.Tag == null)
            {
                ShowStartPanel();
                return;
            }

            TreeNodeType tp = (TreeNodeType)selectedNode.Tag;
            switch(tp)
            {

                case TreeNodeType.MainSettingsNode:
                    ShowMainSettings();
                    break;

                //TODO: Тут другие панели!!!

                case TreeNodeType.SDSettingsNode:
                    this.plSDSettings.BringToFront();
                    break;


            }
        }



        private void propertyGridSettings_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {

        }

        private void tmDateTime_Tick(object sender, EventArgs e)
        {
            if (!dateTimeFromControllerReceived)
            {
                tbControllerTime.Text = "-";
                return;
            }

            this.controllerDateTime = this.controllerDateTime.AddMilliseconds(tmDateTime.Interval);
            string dateTimeString = this.controllerDateTime.ToString("dd.MM.yyyy HH:mm:ss");

            tbControllerTime.Text = dateTimeString;


        }

        private DateTime dateTimeToSet = DateTime.MinValue;
        private bool inSetDateTimeToController = false;

        private void btnSetDateTime_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Установить время контроллера в локальное время компьютера?", "Подтверждение", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (dr != System.Windows.Forms.DialogResult.OK)
                return;

            ShowWaitCursor(true);
            dateTimeToSet = DateTime.Now;
            inSetDateTimeToController = true;

            String s = string.Format("{0,0:D2}.{1,0:D2}.{2} {3,0:D2}:{4,0:D2}:{5,0:D2}", dateTimeToSet.Day, dateTimeToSet.Month, dateTimeToSet.Year, dateTimeToSet.Hour, dateTimeToSet.Minute, dateTimeToSet.Second);
            PushCommandToQueue(SET_PREFIX + "DATETIME" + PARAM_DELIMITER + s, ParseSetDatetime);


        }

        private void ParseSetDelta(Answer a)
        {
            inSetDeltaToController = false;
            ShowWaitCursor(false);

            if (a.IsOkAnswer)
            {
                Config.Instance.Delta1 = Convert.ToInt32(nudDelta1.Value);
                Config.Instance.Delta2 = Convert.ToInt32(nudDelta2.Value);
                Config.Instance.Delta3 = Convert.ToInt32(nudDelta3.Value);

                MessageBox.Show("Дельты обновлёны.", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                nudDelta1.Value = Config.Instance.Delta1;
                nudDelta2.Value = Config.Instance.Delta2;
                nudDelta3.Value = Config.Instance.Delta3;

                MessageBox.Show("Ошибка обновления дельт!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


        private void ParseSetPulses(Answer a)
        {
            inSetPulsesToController = false;
            ShowWaitCursor(false);

            if (a.IsOkAnswer)
            {
                Config.Instance.Pulses1 = Convert.ToInt32(nudPulses1.Value);
                Config.Instance.Pulses2 = Convert.ToInt32(nudPulses2.Value);
                Config.Instance.Pulses3 = Convert.ToInt32(nudPulses3.Value);

                MessageBox.Show("Импульсы обновлёны.", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                nudPulses1.Value = Config.Instance.Pulses1;
                nudPulses2.Value = Config.Instance.Pulses2;
                nudPulses3.Value = Config.Instance.Pulses3;

                MessageBox.Show("Ошибка обновления импульсов!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ParseSetMotoresourceCurrent(Answer a)
        {
            inSetMotoresourceCurrentToController = false;
            ShowWaitCursor(false);

            if (a.IsOkAnswer)
            {
                Config.Instance.MotoresourceCurrent1 = Convert.ToInt32(nudMotoresourceCurrent1.Value);
                Config.Instance.MotoresourceCurrent2 = Convert.ToInt32(nudMotoresourceCurrent2.Value);
                Config.Instance.MotoresourceCurrent3 = Convert.ToInt32(nudMotoresourceCurrent3.Value);

                MessageBox.Show("Текущий моторесурс обновлён.", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                nudMotoresourceCurrent1.Value = Config.Instance.MotoresourceCurrent1;
                nudMotoresourceCurrent2.Value = Config.Instance.MotoresourceCurrent2;
                nudMotoresourceCurrent3.Value = Config.Instance.MotoresourceCurrent3;

                MessageBox.Show("Ошибка обновления текущего моторесурса!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ParseSetMotoresourceMax(Answer a)
        {
            inSetMotoresourceMaxToController = false;
            ShowWaitCursor(false);

            if (a.IsOkAnswer)
            {
                Config.Instance.MotoresourceMax1 = Convert.ToInt32(nudMotoresourceMax1.Value);
                Config.Instance.MotoresourceMax2 = Convert.ToInt32(nudMotoresourceMax2.Value);
                Config.Instance.MotoresourceMax3 = Convert.ToInt32(nudMotoresourceMax3.Value);

                MessageBox.Show("Максимальный моторесурс обновлён.", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                nudMotoresourceMax1.Value = Config.Instance.MotoresourceMax1;
                nudMotoresourceMax2.Value = Config.Instance.MotoresourceMax2;
                nudMotoresourceMax3.Value = Config.Instance.MotoresourceMax3;

                MessageBox.Show("Ошибка обновления максимального моторесурса!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ParseSetDatetime(Answer a)
        {
            inSetDateTimeToController = false;
            ShowWaitCursor(false);

            if (a.IsOkAnswer)
            {
                this.dateTimeFromControllerReceived = true;
                this.controllerDateTime = dateTimeToSet;

                MessageBox.Show("Время контроллера обновлено.", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Ошибка установки времени на контроллере!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            ResizeLogColumns();
        }

        private void treeView_BeforeCollapse(object sender, TreeViewCancelEventArgs e)
        {
            e.Cancel = true;
        }



        private void ShowWaitCursor(bool show)
        {
            Cursor.Current = show ? Cursors.WaitCursor : Cursors.Default;
            Application.UseWaitCursor = show;
            Application.DoEvents();

        }


        private void btnAbout_Click(object sender, EventArgs e)
        {
            AboutForm ab = new AboutForm();
            ab.ShowDialog();
        }

        private TreeNode currentSDParentNode = null;

        private void RescanSD()
        {
            btnListSDFiles.Enabled = false;
            PushCommandToQueue(GET_PREFIX + "LS", DummyAnswerReceiver, SetSDReadingFlag);

        }
        private void btnListSDFiles_Click(object sender, EventArgs e)
        {
            RescanSD();
        }

        public void DummyAnswerReceiver(Answer a)
        {

        }
        private void SetSDReadingFlag()
        {
            this.answerBehaviour = AnswerBehaviour.SDCommandLS;
            this.currentSDParentNode = null;
            this.SDQueryAnswer.Clear();
            this.treeViewSD.Nodes.Clear();
        }

        private void SetSDFolderReadingFlag()
        {
            this.answerBehaviour = AnswerBehaviour.SDCommandLS;
            this.currentSDParentNode = tempSDParentNode;
            this.SDQueryAnswer.Clear();

        }

        private Dictionary<string, string> fileNamesSubstitutions = new Dictionary<string, string>();
        private void InitSubstitutions()
        {
            fileNamesSubstitutions.Clear();

            fileNamesSubstitutions.Add("LOG", "Лог-файлы");
            fileNamesSubstitutions.Add("ETL", "Эталоны");

            fileNamesSubstitutions.Add("ET0UP.ETL", "Канал №1, движение вверх");
            fileNamesSubstitutions.Add("ET0DWN.ETL", "Канал №1, движение вниз");

            fileNamesSubstitutions.Add("ET1UP.ETL", "Канал №2, движение вверх");
            fileNamesSubstitutions.Add("ET1DWN.ETL", "Канал №2, движение вниз");

            fileNamesSubstitutions.Add("ET2UP.ETL", "Канал №3, движение вверх");
            fileNamesSubstitutions.Add("ET2DWN.ETL", "Канал №3, движение вниз");

        }

        string getFileNameFromText(string text)
        {
            if(fileNamesSubstitutions.ContainsValue(text))
            {
                string key = fileNamesSubstitutions.FirstOrDefault(x => x.Value == text).Key;
                return key;
            }

            return text;
        }

        string getTextFromFileName(string fName)
        {
            string fileName = fName.ToUpper();

            if(fileNamesSubstitutions.ContainsKey(fileName))
            {
                return fileNamesSubstitutions[fileName];
            }

            return fileName;
        }

        TreeNode tempSDParentNode = null;
        private void treeViewSD_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            TreeNode wantedExpand = e.Node;
            SDNodeTagHelper tg = (SDNodeTagHelper)wantedExpand.Tag;

            if (tg.Tag != SDNodeTags.TagFolderUninitedNode) // уже проинициализировали
                return;

            string folderName = tg.FileName;//getFileNameFromText(wantedExpand.Text);

            TreeNode parent = wantedExpand.Parent;
            while (parent != null)
            {
                SDNodeTagHelper nt = (SDNodeTagHelper)parent.Tag;
                folderName = /*getFileNameFromText(parent.Text)*/ nt.FileName + PARAM_DELIMITER + folderName;
                parent = parent.Parent;
            }
            tempSDParentNode = wantedExpand;
            PushCommandToQueue(GET_PREFIX + "LS" + PARAM_DELIMITER + folderName, DummyAnswerReceiver, SetSDFolderReadingFlag);
        }

        private void SetSDFileReadingFlag()
        {
            this.answerBehaviour = AnswerBehaviour.SDCommandFILE;
            this.SDQueryAnswer.Clear();
            ShowWaitCursor(true);
        }

        private void treeViewSD_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            TreeNode selectedNode = treeViewSD.SelectedNode;
            if (selectedNode == null)
                return;

            if (selectedNode.Tag == null)
                return;

            SDNodeTagHelper tg = (SDNodeTagHelper)selectedNode.Tag;
            if (tg.Tag != SDNodeTags.TagFileNode)
                return;

            string fullPathName = tg.FileName;//getFileNameFromText(selectedNode.Text);

            TreeNode parent = selectedNode.Parent;
            while(parent != null)
            {
                SDNodeTagHelper nt = (SDNodeTagHelper)parent.Tag;
                fullPathName = /*getFileNameFromText(parent.Text)*/ nt.FileName + PARAM_DELIMITER + fullPathName;
                parent = parent.Parent;
            }

            PushCommandToQueue(GET_PREFIX + "FILE" + PARAM_DELIMITER + fullPathName, DummyAnswerReceiver, SetSDFileReadingFlag);
            this.btnListSDFiles.Enabled = false;
            this.treeViewSD.Enabled = false;

        }


        private void tmEnumComPorts_Tick(object sender, EventArgs e)
        {

            string[] ports = SerialPort.GetPortNames();

            // сначала удаляем те порты, которых нет в списке текущих
            List<ToolStripMenuItem> toRemove = new List<ToolStripMenuItem>();

            foreach (ToolStripMenuItem existing in miPort.DropDownItems)
            {
                bool found = false;
                foreach (string port in ports)
                {
                    if(port == existing.Text)
                    {
                        found = true;
                        break;
                    }
                }

                if(!found)
                {
                    toRemove.Add(existing);
                }
            }

            // теперь чистим
            for(int i=0;i< toRemove.Count;i++)
            {
                miPort.DropDownItems.Remove(toRemove[i]);
            }

            foreach (string port in ports)
            {
                // ищем - есть ли такой порт уже?
                bool found = false;
                foreach(ToolStripMenuItem existing in miPort.DropDownItems)
                {
                    if(existing.Text == port)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    continue;

                ToolStripMenuItem ti = new ToolStripMenuItem(port);

                ti.MergeIndex = 1;
                ti.AutoSize = true;
                ti.ImageScaling = ToolStripItemImageScaling.None;
                ti.Tag = port;
                ti.Click += ConnectToSelectedComPort;
                ti.CheckOnClick = false;



                miPort.DropDownItems.Add(ti);


            }
        }


        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            currentTransport.Disconnect();
            this.treeView.Nodes[0].Nodes.Clear();
        }

        private void ChangePortSpeed(object sender, EventArgs e)
        {
            ToolStripMenuItem selItem = sender as ToolStripMenuItem;

          foreach(ToolStripMenuItem tmi in this.portSpeedToolStripMenuItem.DropDownItems)
            {
                tmi.Checked = false;
            }
            selItem.Checked = true;
        }

        private bool inSetMotoresourceCurrentToController = true;
        private void btnSetMotoresourceCurrent_Click(object sender, EventArgs e)
        {
            inSetMotoresourceCurrentToController = true;
            ShowWaitCursor(true);

            string s = "";
            s += Convert.ToString(nudMotoresourceCurrent1.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudMotoresourceCurrent2.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudMotoresourceCurrent3.Value);

            PushCommandToQueue(SET_PREFIX + "RES_CUR" + PARAM_DELIMITER + s, ParseSetMotoresourceCurrent);
        }

        private bool inSetMotoresourceMaxToController = true;
        private void btnSetMotoresourceMax_Click(object sender, EventArgs e)
        {
            inSetMotoresourceMaxToController = true;
            ShowWaitCursor(true);

            string s = "";
            s += Convert.ToString(nudMotoresourceMax1.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudMotoresourceMax2.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudMotoresourceMax3.Value);

            PushCommandToQueue(SET_PREFIX + "RES_MAX" + PARAM_DELIMITER + s, ParseSetMotoresourceMax);

        }

        private bool inSetPulsesToController = true;
        private void btnSetPulses_Click(object sender, EventArgs e)
        {
            inSetPulsesToController = true;
            ShowWaitCursor(true);

            string s = "";
            s += Convert.ToString(nudPulses1.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudPulses2.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudPulses3.Value);

            PushCommandToQueue(SET_PREFIX + "PULSES" + PARAM_DELIMITER + s, ParseSetPulses);
        }

        private bool inSetDeltaToController = true;
        private void btnSetDelta_Click(object sender, EventArgs e)
        {
            inSetDeltaToController = true;
            ShowWaitCursor(true);

            string s = "";
            s += Convert.ToString(nudDelta1.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudDelta2.Value) + PARAM_DELIMITER;
            s += Convert.ToString(nudDelta3.Value);

            PushCommandToQueue(SET_PREFIX + "DELTA" + PARAM_DELIMITER + s, ParseSetDelta);
        }

        void GetInductiveSensors()
        {
            PushCommandToQueue(GET_PREFIX + "IND", ParseInductiveSensors);
        }

        void GetVoltage()
        {
            PushCommandToQueue(GET_PREFIX + "VDATA", ParseVoltage);

        }
        private void tmInductiveTimer_Tick(object sender, EventArgs e)
        {
            GetInductiveSensors();
            GetVoltage();
        }

        private void ResetVoltage()
        {
            lblVoltage1.BackColor = Color.LightGray;
            lblVoltage1.Text = "-";

            lblVoltage2.BackColor = Color.LightGray;
            lblVoltage2.Text = "-";

            lblVoltage3.BackColor = Color.LightGray;
            lblVoltage3.Text = "-";
        }

        private void ResetInductiveSensors()
        {
            lblInductive1.BackColor = Color.LightGray;
            lblInductive1.Text = "-";

            lblInductive2.BackColor = Color.LightGray;
            lblInductive2.Text = "-";

            lblInductive3.BackColor = Color.LightGray;
            lblInductive3.Text = "-";
        }

        private void ParseInductiveSensors(Answer a)
        {
            if(a.IsOkAnswer)
            {

                try {

                    bool indSensor1 = Convert.ToInt32(a.Params[1]) == 1;
                    if(indSensor1)
                    {
                        lblInductive1.BackColor = Color.LightGreen;
                        lblInductive1.Text = "Исправен";
                    }
                    else
                    {
                        lblInductive1.BackColor = Color.LightSalmon;
                        lblInductive1.Text = "Неисправен!";

                    }
                }
                catch { }

                try
                {

                    bool indSensor2 = Convert.ToInt32(a.Params[2]) == 1;
                    if (indSensor2)
                    {
                        lblInductive2.BackColor = Color.LightGreen;
                        lblInductive2.Text = "Исправен";
                    }
                    else
                    {
                        lblInductive2.BackColor = Color.LightSalmon;
                        lblInductive2.Text = "Неисправен!";

                    }
                }
                catch { }

                try
                {

                    bool indSensor3 = Convert.ToInt32(a.Params[3]) == 1;
                    if (indSensor3)
                    {
                        lblInductive3.BackColor = Color.LightGreen;
                        lblInductive3.Text = "Исправен";
                    }
                    else
                    {
                        lblInductive3.BackColor = Color.LightSalmon;
                        lblInductive3.Text = "Неисправен!";

                    }
                }
                catch { }

            }
            else
            {
                ResetInductiveSensors();
            }
        }

        private void ParseVoltage(Answer a)
        {
            if (a.IsOkAnswer)
            {
                int VOLTAGE_THRESHOLD = 10;
                NumberFormatInfo nfi = new NumberFormatInfo();
                nfi.NumberDecimalSeparator = ".";

                string end = "V";

                try
                {

                    int vdata = Convert.ToInt32(a.Params[1]);
                    float threshold = (3.3f / 100) * VOLTAGE_THRESHOLD;
                    float lowBorder = 3.3f - threshold;
                    float highBorder = 3.3f + threshold;

                    float currentV = vdata * (2.4f / 4096 * 2);

                    if (currentV >= lowBorder && currentV <= highBorder)
                    {
                        lblVoltage1.BackColor = Color.LightGreen;
                    }
                    else
                    {
                        lblVoltage1.BackColor = Color.LightSalmon;
                    }

                    lblVoltage1.Text = currentV.ToString("n1", nfi) + end;


                }
                catch { }

                try
                {

                    int vdata = Convert.ToInt32(a.Params[2]);
                    float threshold = (5.0f / 100) * VOLTAGE_THRESHOLD;
                    float lowBorder = 5.0f - threshold;
                    float highBorder = 5.0f + threshold;

                    float currentV = vdata * (2.4f / 4096 * 2);

                    if (currentV >= lowBorder && currentV <= highBorder)
                    {
                        lblVoltage2.BackColor = Color.LightGreen;
                    }
                    else
                    {
                        lblVoltage2.BackColor = Color.LightSalmon;
                    }

                    lblVoltage2.Text = currentV.ToString("n1", nfi) + end;


                }
                catch { }

                try
                {

                    int vdata = Convert.ToInt32(a.Params[3]);
                    float threshold = (200.0f / 100) * VOLTAGE_THRESHOLD;
                    float lowBorder = 200.0f - threshold;
                    float highBorder = 200.0f + threshold;

                    float currentV = vdata * (2.4f / 4096 * 100);

                    if (currentV >= lowBorder && currentV <= highBorder)
                    {
                        lblVoltage3.BackColor = Color.LightGreen;
                    }
                    else
                    {
                        lblVoltage3.BackColor = Color.LightSalmon;
                    }

                    lblVoltage3.Text = currentV.ToString("n1", nfi) + end;


                }
                catch { }


            }
            else
            {
                ResetVoltage();
            }
        }
    }

}
