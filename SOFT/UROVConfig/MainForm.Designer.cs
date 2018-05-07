namespace UROVConfig
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Контроллер");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Архив", 6, 6);
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle10 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle12 = new System.Windows.Forms.DataGridViewCellStyle();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.connectStatusMessage = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusProgressMessage = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusProgressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.miHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.miAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.btnConnect = new System.Windows.Forms.ToolStripDropDownButton();
            this.miPort = new System.Windows.Forms.ToolStripMenuItem();
            this.portSpeedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps9600ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps19200ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps38400ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps57600ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps74880ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps115200ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ps250000ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btnDisconnect = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnSetDateTime = new System.Windows.Forms.ToolStripButton();
            this.btnControllerName = new System.Windows.Forms.ToolStripButton();
            this.btnImportSettings = new System.Windows.Forms.ToolStripButton();
            this.btnUploadEthalon = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAbout = new System.Windows.Forms.ToolStripButton();
            this.toolbarImages = new System.Windows.Forms.ImageList(this.components);
            this.tmProcessCommandsTimer = new System.Windows.Forms.Timer(this.components);
            this.tmGetSensorsData = new System.Windows.Forms.Timer(this.components);
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.treeView = new System.Windows.Forms.TreeView();
            this.smallImages = new System.Windows.Forms.ImageList(this.components);
            this.plSection = new System.Windows.Forms.Panel();
            this.archiveLogDataGrid = new System.Windows.Forms.DataGridView();
            this.Num1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Time1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Temp1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Motoresource1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Channel1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Pulses1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Rod1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Etl1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Compare1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Btn1 = new System.Windows.Forms.DataGridViewButtonColumn();
            this.plArchiveEthalonChart = new System.Windows.Forms.Panel();
            this.archiveAthalonChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.plSDSettings = new System.Windows.Forms.Panel();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.treeViewSD = new System.Windows.Forms.TreeView();
            this.sdImagesNormal = new System.Windows.Forms.ImageList(this.components);
            this.logDataGrid = new System.Windows.Forms.DataGridView();
            this.Num = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Time = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Temp = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Motoresource = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Channel = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Pulses = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Rod = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Etl = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Compare = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Btn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.plEmptySDWorkspace = new System.Windows.Forms.Panel();
            this.plEthalonChart = new System.Windows.Forms.Panel();
            this.ethalonChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.toolStripSD = new System.Windows.Forms.ToolStrip();
            this.btnListSDFiles = new System.Windows.Forms.ToolStripButton();
            this.btnViewSDFile = new System.Windows.Forms.ToolStripButton();
            this.btnDeleteSDFile = new System.Windows.Forms.ToolStripButton();
            this.plMainSettings = new System.Windows.Forms.Panel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.plInfo = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.btnSetDateTime2 = new System.Windows.Forms.Button();
            this.tbControllerTime = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbFREERAM = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbFirmwareVersion = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.plInductive = new System.Windows.Forms.Panel();
            this.panel13 = new System.Windows.Forms.Panel();
            this.lblInductive3 = new System.Windows.Forms.Label();
            this.lblInductive2 = new System.Windows.Forms.Label();
            this.lblInductive1 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.panel14 = new System.Windows.Forms.Panel();
            this.label24 = new System.Windows.Forms.Label();
            this.plVoltage = new System.Windows.Forms.Panel();
            this.panel15 = new System.Windows.Forms.Panel();
            this.lblVoltage3 = new System.Windows.Forms.Label();
            this.lblVoltage2 = new System.Windows.Forms.Label();
            this.lblVoltage1 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.panel16 = new System.Windows.Forms.Panel();
            this.label31 = new System.Windows.Forms.Label();
            this.plMotoresourceCurrent = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.lblMotoresourcePercents3 = new System.Windows.Forms.Label();
            this.lblMotoresourcePercents2 = new System.Windows.Forms.Label();
            this.lblMotoresourcePercents1 = new System.Windows.Forms.Label();
            this.btnSetMotoresourceCurrent = new System.Windows.Forms.Button();
            this.nudMotoresourceCurrent3 = new System.Windows.Forms.NumericUpDown();
            this.nudMotoresourceCurrent2 = new System.Windows.Forms.NumericUpDown();
            this.nudMotoresourceCurrent1 = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.panel6 = new System.Windows.Forms.Panel();
            this.label8 = new System.Windows.Forms.Label();
            this.plMotoresourceMax = new System.Windows.Forms.Panel();
            this.panel7 = new System.Windows.Forms.Panel();
            this.btnSetMotoresourceMax = new System.Windows.Forms.Button();
            this.nudMotoresourceMax3 = new System.Windows.Forms.NumericUpDown();
            this.nudMotoresourceMax2 = new System.Windows.Forms.NumericUpDown();
            this.nudMotoresourceMax1 = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.panel8 = new System.Windows.Forms.Panel();
            this.label12 = new System.Windows.Forms.Label();
            this.plImpulses = new System.Windows.Forms.Panel();
            this.panel9 = new System.Windows.Forms.Panel();
            this.btnSetPulses = new System.Windows.Forms.Button();
            this.nudPulses3 = new System.Windows.Forms.NumericUpDown();
            this.nudPulses2 = new System.Windows.Forms.NumericUpDown();
            this.nudPulses1 = new System.Windows.Forms.NumericUpDown();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.panel10 = new System.Windows.Forms.Panel();
            this.label16 = new System.Windows.Forms.Label();
            this.plDelta = new System.Windows.Forms.Panel();
            this.panel11 = new System.Windows.Forms.Panel();
            this.btnSetDelta = new System.Windows.Forms.Button();
            this.nudDelta3 = new System.Windows.Forms.NumericUpDown();
            this.nudDelta2 = new System.Windows.Forms.NumericUpDown();
            this.nudDelta1 = new System.Windows.Forms.NumericUpDown();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.panel12 = new System.Windows.Forms.Panel();
            this.label20 = new System.Windows.Forms.Label();
            this.plStartPanel = new System.Windows.Forms.Panel();
            this.lvLog = new System.Windows.Forms.ListView();
            this.logColumn1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.logColumn2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.sensorImages = new System.Windows.Forms.ImageList(this.components);
            this.dataImages = new System.Windows.Forms.ImageList(this.components);
            this.tmDateTime = new System.Windows.Forms.Timer(this.components);
            this.tmEnumComPorts = new System.Windows.Forms.Timer(this.components);
            this.tmPeriodicCommandsTimer = new System.Windows.Forms.Timer(this.components);
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn5 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn6 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn7 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn8 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn9 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.statusStrip.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.plSection.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.archiveLogDataGrid)).BeginInit();
            this.plArchiveEthalonChart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.archiveAthalonChart)).BeginInit();
            this.plSDSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.logDataGrid)).BeginInit();
            this.plEthalonChart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ethalonChart)).BeginInit();
            this.toolStripSD.SuspendLayout();
            this.plMainSettings.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.plInfo.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.plInductive.SuspendLayout();
            this.panel13.SuspendLayout();
            this.panel14.SuspendLayout();
            this.plVoltage.SuspendLayout();
            this.panel15.SuspendLayout();
            this.panel16.SuspendLayout();
            this.plMotoresourceCurrent.SuspendLayout();
            this.panel5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent1)).BeginInit();
            this.panel6.SuspendLayout();
            this.plMotoresourceMax.SuspendLayout();
            this.panel7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax1)).BeginInit();
            this.panel8.SuspendLayout();
            this.plImpulses.SuspendLayout();
            this.panel9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses1)).BeginInit();
            this.panel10.SuspendLayout();
            this.plDelta.SuspendLayout();
            this.panel11.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta1)).BeginInit();
            this.panel12.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectStatusMessage,
            this.statusProgressMessage,
            this.statusProgressBar});
            this.statusStrip.Location = new System.Drawing.Point(0, 749);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(1269, 22);
            this.statusStrip.TabIndex = 0;
            this.statusStrip.Text = "statusStrip1";
            // 
            // connectStatusMessage
            // 
            this.connectStatusMessage.Name = "connectStatusMessage";
            this.connectStatusMessage.Size = new System.Drawing.Size(98, 17);
            this.connectStatusMessage.Text = "Нет соединения.";
            // 
            // statusProgressMessage
            // 
            this.statusProgressMessage.Name = "statusProgressMessage";
            this.statusProgressMessage.Size = new System.Drawing.Size(0, 17);
            this.statusProgressMessage.Visible = false;
            // 
            // statusProgressBar
            // 
            this.statusProgressBar.Name = "statusProgressBar";
            this.statusProgressBar.Size = new System.Drawing.Size(100, 16);
            this.statusProgressBar.Step = 1;
            this.statusProgressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.statusProgressBar.Visible = false;
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.miHelp});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(1269, 24);
            this.menuStrip.TabIndex = 1;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.fileToolStripMenuItem.Text = "Файл";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
            this.exitToolStripMenuItem.Text = "Выход";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // miHelp
            // 
            this.miHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miAbout});
            this.miHelp.Name = "miHelp";
            this.miHelp.Size = new System.Drawing.Size(68, 20);
            this.miHelp.Text = "Помощь";
            // 
            // miAbout
            // 
            this.miAbout.Name = "miAbout";
            this.miAbout.Size = new System.Drawing.Size(158, 22);
            this.miAbout.Text = "О программе...";
            this.miAbout.Click += new System.EventHandler(this.btnAbout_Click);
            // 
            // toolStrip
            // 
            this.toolStrip.ImageScalingSize = new System.Drawing.Size(48, 48);
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnConnect,
            this.btnDisconnect,
            this.toolStripSeparator2,
            this.btnSetDateTime,
            this.btnControllerName,
            this.btnImportSettings,
            this.btnUploadEthalon,
            this.toolStripSeparator1,
            this.btnAbout});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(1269, 70);
            this.toolStrip.TabIndex = 2;
            // 
            // btnConnect
            // 
            this.btnConnect.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miPort,
            this.portSpeedToolStripMenuItem});
            this.btnConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(79, 67);
            this.btnConnect.Text = "Соединить";
            this.btnConnect.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // miPort
            // 
            this.miPort.Name = "miPort";
            this.miPort.Size = new System.Drawing.Size(194, 22);
            this.miPort.Text = "COM-порт";
            // 
            // portSpeedToolStripMenuItem
            // 
            this.portSpeedToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ps9600ToolStripMenuItem,
            this.ps19200ToolStripMenuItem,
            this.ps38400ToolStripMenuItem,
            this.ps57600ToolStripMenuItem,
            this.ps74880ToolStripMenuItem,
            this.ps115200ToolStripMenuItem,
            this.ps250000ToolStripMenuItem});
            this.portSpeedToolStripMenuItem.Name = "portSpeedToolStripMenuItem";
            this.portSpeedToolStripMenuItem.Size = new System.Drawing.Size(194, 22);
            this.portSpeedToolStripMenuItem.Text = "Скорость соединения";
            // 
            // ps9600ToolStripMenuItem
            // 
            this.ps9600ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps9600ToolStripMenuItem.MergeIndex = 2;
            this.ps9600ToolStripMenuItem.Name = "ps9600ToolStripMenuItem";
            this.ps9600ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps9600ToolStripMenuItem.Text = "9600";
            this.ps9600ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps19200ToolStripMenuItem
            // 
            this.ps19200ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps19200ToolStripMenuItem.MergeIndex = 2;
            this.ps19200ToolStripMenuItem.Name = "ps19200ToolStripMenuItem";
            this.ps19200ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps19200ToolStripMenuItem.Text = "19200";
            this.ps19200ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps38400ToolStripMenuItem
            // 
            this.ps38400ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps38400ToolStripMenuItem.MergeIndex = 2;
            this.ps38400ToolStripMenuItem.Name = "ps38400ToolStripMenuItem";
            this.ps38400ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps38400ToolStripMenuItem.Text = "38400";
            this.ps38400ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps57600ToolStripMenuItem
            // 
            this.ps57600ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps57600ToolStripMenuItem.MergeIndex = 2;
            this.ps57600ToolStripMenuItem.Name = "ps57600ToolStripMenuItem";
            this.ps57600ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps57600ToolStripMenuItem.Text = "57600";
            this.ps57600ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps74880ToolStripMenuItem
            // 
            this.ps74880ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps74880ToolStripMenuItem.MergeIndex = 2;
            this.ps74880ToolStripMenuItem.Name = "ps74880ToolStripMenuItem";
            this.ps74880ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps74880ToolStripMenuItem.Text = "74880";
            this.ps74880ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps115200ToolStripMenuItem
            // 
            this.ps115200ToolStripMenuItem.Checked = true;
            this.ps115200ToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ps115200ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps115200ToolStripMenuItem.MergeIndex = 2;
            this.ps115200ToolStripMenuItem.Name = "ps115200ToolStripMenuItem";
            this.ps115200ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps115200ToolStripMenuItem.Text = "115200";
            this.ps115200ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // ps250000ToolStripMenuItem
            // 
            this.ps250000ToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.ps250000ToolStripMenuItem.MergeIndex = 2;
            this.ps250000ToolStripMenuItem.Name = "ps250000ToolStripMenuItem";
            this.ps250000ToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.ps250000ToolStripMenuItem.Text = "250000";
            this.ps250000ToolStripMenuItem.Click += new System.EventHandler(this.ChangePortSpeed);
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Enabled = false;
            this.btnDisconnect.Image = ((System.Drawing.Image)(resources.GetObject("btnDisconnect.Image")));
            this.btnDisconnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(66, 67);
            this.btnDisconnect.Text = "Разорвать";
            this.btnDisconnect.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnDisconnect.ToolTipText = "Разорвать соединение";
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 70);
            // 
            // btnSetDateTime
            // 
            this.btnSetDateTime.Enabled = false;
            this.btnSetDateTime.Image = ((System.Drawing.Image)(resources.GetObject("btnSetDateTime.Image")));
            this.btnSetDateTime.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSetDateTime.Name = "btnSetDateTime";
            this.btnSetDateTime.Size = new System.Drawing.Size(75, 67);
            this.btnSetDateTime.Text = "Дата/время";
            this.btnSetDateTime.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnSetDateTime.ToolTipText = "Установить дату/время на контроллере";
            this.btnSetDateTime.Click += new System.EventHandler(this.btnSetDateTime_Click);
            // 
            // btnControllerName
            // 
            this.btnControllerName.Enabled = false;
            this.btnControllerName.Image = ((System.Drawing.Image)(resources.GetObject("btnControllerName.Image")));
            this.btnControllerName.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnControllerName.Name = "btnControllerName";
            this.btnControllerName.Size = new System.Drawing.Size(110, 67);
            this.btnControllerName.Text = "Имя контроллера";
            this.btnControllerName.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnControllerName.ToolTipText = "Редактировать имя контроллера";
            this.btnControllerName.Click += new System.EventHandler(this.btnControllerName_Click);
            // 
            // btnImportSettings
            // 
            this.btnImportSettings.Enabled = false;
            this.btnImportSettings.Image = ((System.Drawing.Image)(resources.GetObject("btnImportSettings.Image")));
            this.btnImportSettings.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnImportSettings.Name = "btnImportSettings";
            this.btnImportSettings.Size = new System.Drawing.Size(113, 67);
            this.btnImportSettings.Text = "Архивация данных";
            this.btnImportSettings.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnImportSettings.ToolTipText = "Архивация данных с контроллера";
            this.btnImportSettings.Click += new System.EventHandler(this.btnImportSettings_Click);
            // 
            // btnUploadEthalon
            // 
            this.btnUploadEthalon.Enabled = false;
            this.btnUploadEthalon.Image = ((System.Drawing.Image)(resources.GetObject("btnUploadEthalon.Image")));
            this.btnUploadEthalon.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnUploadEthalon.Name = "btnUploadEthalon";
            this.btnUploadEthalon.Size = new System.Drawing.Size(106, 67);
            this.btnUploadEthalon.Text = "Загрузить эталон";
            this.btnUploadEthalon.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnUploadEthalon.ToolTipText = "Загрузить эталон в контроллер";
            this.btnUploadEthalon.Click += new System.EventHandler(this.btnUploadEthalon_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 70);
            // 
            // btnAbout
            // 
            this.btnAbout.Image = ((System.Drawing.Image)(resources.GetObject("btnAbout.Image")));
            this.btnAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAbout.Name = "btnAbout";
            this.btnAbout.Size = new System.Drawing.Size(86, 67);
            this.btnAbout.Text = "О программе";
            this.btnAbout.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnAbout.Click += new System.EventHandler(this.btnAbout_Click);
            // 
            // toolbarImages
            // 
            this.toolbarImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("toolbarImages.ImageStream")));
            this.toolbarImages.TransparentColor = System.Drawing.Color.Transparent;
            this.toolbarImages.Images.SetKeyName(0, "connect_no.png");
            this.toolbarImages.Images.SetKeyName(1, "connect_established.png");
            this.toolbarImages.Images.SetKeyName(2, "document_export.png");
            this.toolbarImages.Images.SetKeyName(3, "clock.png");
            this.toolbarImages.Images.SetKeyName(4, "upload.png");
            this.toolbarImages.Images.SetKeyName(5, "messagebox_info.png");
            this.toolbarImages.Images.SetKeyName(6, "exit.png");
            this.toolbarImages.Images.SetKeyName(7, "pins.png");
            this.toolbarImages.Images.SetKeyName(8, "controller_name.png");
            this.toolbarImages.Images.SetKeyName(9, "import_settings.png");
            // 
            // tmProcessCommandsTimer
            // 
            this.tmProcessCommandsTimer.Enabled = true;
            this.tmProcessCommandsTimer.Tick += new System.EventHandler(this.tmProcessCommandsTimer_Tick);
            // 
            // tmGetSensorsData
            // 
            this.tmGetSensorsData.Enabled = true;
            this.tmGetSensorsData.Interval = 5000;
            this.tmGetSensorsData.Tick += new System.EventHandler(this.tmGetSensorsData_Tick);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 94);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.lvLog);
            this.splitContainer1.Size = new System.Drawing.Size(1269, 655);
            this.splitContainer1.SplitterDistance = 533;
            this.splitContainer1.TabIndex = 5;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.treeView);
            this.splitContainer2.Panel1MinSize = 150;
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.plSection);
            this.splitContainer2.Panel2MinSize = 400;
            this.splitContainer2.Size = new System.Drawing.Size(1269, 533);
            this.splitContainer2.SplitterDistance = 194;
            this.splitContainer2.TabIndex = 7;
            // 
            // treeView
            // 
            this.treeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView.ImageIndex = 0;
            this.treeView.ImageList = this.smallImages;
            this.treeView.Location = new System.Drawing.Point(0, 0);
            this.treeView.Name = "treeView";
            treeNode1.ImageIndex = 1;
            treeNode1.Name = "rootNode";
            treeNode1.Text = "Контроллер";
            treeNode2.ImageIndex = 6;
            treeNode2.Name = "Node0";
            treeNode2.SelectedImageIndex = 6;
            treeNode2.Text = "Архив";
            this.treeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
            this.treeView.SelectedImageIndex = 1;
            this.treeView.Size = new System.Drawing.Size(194, 533);
            this.treeView.TabIndex = 0;
            this.treeView.BeforeCollapse += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeView_BeforeCollapse);
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
            this.treeView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.treeView_MouseDoubleClick);
            // 
            // smallImages
            // 
            this.smallImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("smallImages.ImageStream")));
            this.smallImages.TransparentColor = System.Drawing.Color.Transparent;
            this.smallImages.Images.SetKeyName(0, "log.png");
            this.smallImages.Images.SetKeyName(1, "root.png");
            this.smallImages.Images.SetKeyName(2, "lora.png");
            this.smallImages.Images.SetKeyName(3, "rs485.png");
            this.smallImages.Images.SetKeyName(4, "esp.png");
            this.smallImages.Images.SetKeyName(5, "settings.png");
            this.smallImages.Images.SetKeyName(6, "data.png");
            this.smallImages.Images.SetKeyName(7, "sensors.png");
            this.smallImages.Images.SetKeyName(8, "sd.png");
            this.smallImages.Images.SetKeyName(9, "signal.png");
            this.smallImages.Images.SetKeyName(10, "mqtt.png");
            this.smallImages.Images.SetKeyName(11, "phone.png");
            this.smallImages.Images.SetKeyName(12, "thingspeak.png");
            this.smallImages.Images.SetKeyName(13, "barometric.png");
            this.smallImages.Images.SetKeyName(14, "pressure.png");
            this.smallImages.Images.SetKeyName(15, "distance.png");
            this.smallImages.Images.SetKeyName(16, "userdata.png");
            // 
            // plSection
            // 
            this.plSection.BackColor = System.Drawing.Color.White;
            this.plSection.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.plSection.Controls.Add(this.archiveLogDataGrid);
            this.plSection.Controls.Add(this.plArchiveEthalonChart);
            this.plSection.Controls.Add(this.plSDSettings);
            this.plSection.Controls.Add(this.plMainSettings);
            this.plSection.Controls.Add(this.plStartPanel);
            this.plSection.Dock = System.Windows.Forms.DockStyle.Fill;
            this.plSection.Location = new System.Drawing.Point(0, 0);
            this.plSection.Name = "plSection";
            this.plSection.Size = new System.Drawing.Size(1071, 533);
            this.plSection.TabIndex = 1;
            // 
            // archiveLogDataGrid
            // 
            this.archiveLogDataGrid.AllowUserToAddRows = false;
            this.archiveLogDataGrid.AllowUserToDeleteRows = false;
            this.archiveLogDataGrid.AllowUserToResizeRows = false;
            this.archiveLogDataGrid.BackgroundColor = System.Drawing.Color.White;
            this.archiveLogDataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.archiveLogDataGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Num1,
            this.Time1,
            this.Temp1,
            this.Motoresource1,
            this.Channel1,
            this.Pulses1,
            this.Rod1,
            this.Etl1,
            this.Compare1,
            this.Btn1});
            this.archiveLogDataGrid.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.archiveLogDataGrid.Location = new System.Drawing.Point(414, 190);
            this.archiveLogDataGrid.MultiSelect = false;
            this.archiveLogDataGrid.Name = "archiveLogDataGrid";
            this.archiveLogDataGrid.ReadOnly = true;
            this.archiveLogDataGrid.RowHeadersVisible = false;
            this.archiveLogDataGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.archiveLogDataGrid.ShowEditingIcon = false;
            this.archiveLogDataGrid.Size = new System.Drawing.Size(240, 150);
            this.archiveLogDataGrid.TabIndex = 12;
            this.archiveLogDataGrid.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.logDataGrid_CellContentClick);
            // 
            // Num1
            // 
            this.Num1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleRight;
            this.Num1.DefaultCellStyle = dataGridViewCellStyle1;
            this.Num1.HeaderText = "№";
            this.Num1.Name = "Num1";
            this.Num1.ReadOnly = true;
            this.Num1.Width = 43;
            // 
            // Time1
            // 
            this.Time1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Time1.HeaderText = "Время";
            this.Time1.Name = "Time1";
            this.Time1.ReadOnly = true;
            // 
            // Temp1
            // 
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Temp1.DefaultCellStyle = dataGridViewCellStyle2;
            this.Temp1.HeaderText = "Температура";
            this.Temp1.Name = "Temp1";
            this.Temp1.ReadOnly = true;
            // 
            // Motoresource1
            // 
            this.Motoresource1.HeaderText = "Срабатываний";
            this.Motoresource1.Name = "Motoresource1";
            this.Motoresource1.ReadOnly = true;
            // 
            // Channel1
            // 
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Channel1.DefaultCellStyle = dataGridViewCellStyle3;
            this.Channel1.HeaderText = "Канал";
            this.Channel1.Name = "Channel1";
            this.Channel1.ReadOnly = true;
            // 
            // Pulses1
            // 
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Pulses1.DefaultCellStyle = dataGridViewCellStyle4;
            this.Pulses1.HeaderText = "Импульсов";
            this.Pulses1.Name = "Pulses1";
            this.Pulses1.ReadOnly = true;
            // 
            // Rod1
            // 
            this.Rod1.HeaderText = "Штанга";
            this.Rod1.Name = "Rod1";
            this.Rod1.ReadOnly = true;
            // 
            // Etl1
            // 
            this.Etl1.HeaderText = "Эталон";
            this.Etl1.Name = "Etl1";
            this.Etl1.ReadOnly = true;
            // 
            // Compare1
            // 
            this.Compare1.HeaderText = "Сравнение";
            this.Compare1.Name = "Compare1";
            this.Compare1.ReadOnly = true;
            // 
            // Btn1
            // 
            this.Btn1.HeaderText = "График";
            this.Btn1.Name = "Btn1";
            this.Btn1.ReadOnly = true;
            // 
            // plArchiveEthalonChart
            // 
            this.plArchiveEthalonChart.Controls.Add(this.archiveAthalonChart);
            this.plArchiveEthalonChart.Location = new System.Drawing.Point(434, 169);
            this.plArchiveEthalonChart.Name = "plArchiveEthalonChart";
            this.plArchiveEthalonChart.Size = new System.Drawing.Size(200, 193);
            this.plArchiveEthalonChart.TabIndex = 11;
            // 
            // archiveAthalonChart
            // 
            chartArea1.Name = "ethalonChartArea";
            this.archiveAthalonChart.ChartAreas.Add(chartArea1);
            this.archiveAthalonChart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.archiveAthalonChart.Location = new System.Drawing.Point(0, 0);
            this.archiveAthalonChart.Name = "archiveAthalonChart";
            series1.ChartArea = "ethalonChartArea";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series1.Name = "ethalonChartSerie";
            this.archiveAthalonChart.Series.Add(series1);
            this.archiveAthalonChart.Size = new System.Drawing.Size(200, 193);
            this.archiveAthalonChart.TabIndex = 0;
            this.archiveAthalonChart.Text = "chart1";
            // 
            // plSDSettings
            // 
            this.plSDSettings.Controls.Add(this.splitContainer3);
            this.plSDSettings.Controls.Add(this.toolStripSD);
            this.plSDSettings.Location = new System.Drawing.Point(16, 18);
            this.plSDSettings.Name = "plSDSettings";
            this.plSDSettings.Size = new System.Drawing.Size(573, 364);
            this.plSDSettings.TabIndex = 6;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 54);
            this.splitContainer3.Name = "splitContainer3";
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.treeViewSD);
            this.splitContainer3.Panel1MinSize = 150;
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.logDataGrid);
            this.splitContainer3.Panel2.Controls.Add(this.plEmptySDWorkspace);
            this.splitContainer3.Panel2.Controls.Add(this.plEthalonChart);
            this.splitContainer3.Panel2MinSize = 100;
            this.splitContainer3.Size = new System.Drawing.Size(573, 310);
            this.splitContainer3.SplitterDistance = 150;
            this.splitContainer3.TabIndex = 5;
            // 
            // treeViewSD
            // 
            this.treeViewSD.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.treeViewSD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewSD.HideSelection = false;
            this.treeViewSD.ImageIndex = 2;
            this.treeViewSD.ImageList = this.sdImagesNormal;
            this.treeViewSD.Location = new System.Drawing.Point(0, 0);
            this.treeViewSD.Name = "treeViewSD";
            this.treeViewSD.SelectedImageIndex = 0;
            this.treeViewSD.Size = new System.Drawing.Size(150, 310);
            this.treeViewSD.TabIndex = 1;
            this.treeViewSD.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewSD_BeforeExpand);
            this.treeViewSD.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewSD_BeforeSelect);
            this.treeViewSD.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewSD_AfterSelect);
            this.treeViewSD.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.treeViewSD_MouseDoubleClick);
            // 
            // sdImagesNormal
            // 
            this.sdImagesNormal.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("sdImagesNormal.ImageStream")));
            this.sdImagesNormal.TransparentColor = System.Drawing.Color.Transparent;
            this.sdImagesNormal.Images.SetKeyName(0, "folder-blue.png");
            this.sdImagesNormal.Images.SetKeyName(1, "document-open-folder.png");
            this.sdImagesNormal.Images.SetKeyName(2, "list.png");
            // 
            // logDataGrid
            // 
            this.logDataGrid.AllowUserToAddRows = false;
            this.logDataGrid.AllowUserToDeleteRows = false;
            this.logDataGrid.AllowUserToResizeRows = false;
            this.logDataGrid.BackgroundColor = System.Drawing.Color.White;
            this.logDataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.logDataGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Num,
            this.Time,
            this.Temp,
            this.Motoresource,
            this.Channel,
            this.Pulses,
            this.Rod,
            this.Etl,
            this.Compare,
            this.Btn});
            this.logDataGrid.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.logDataGrid.Location = new System.Drawing.Point(17, 20);
            this.logDataGrid.MultiSelect = false;
            this.logDataGrid.Name = "logDataGrid";
            this.logDataGrid.ReadOnly = true;
            this.logDataGrid.RowHeadersVisible = false;
            this.logDataGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.logDataGrid.ShowEditingIcon = false;
            this.logDataGrid.Size = new System.Drawing.Size(240, 150);
            this.logDataGrid.TabIndex = 3;
            this.logDataGrid.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.logDataGrid_CellContentClick);
            // 
            // Num
            // 
            this.Num.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleRight;
            this.Num.DefaultCellStyle = dataGridViewCellStyle5;
            this.Num.HeaderText = "№";
            this.Num.Name = "Num";
            this.Num.ReadOnly = true;
            this.Num.Width = 43;
            // 
            // Time
            // 
            this.Time.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Time.HeaderText = "Время";
            this.Time.Name = "Time";
            this.Time.ReadOnly = true;
            // 
            // Temp
            // 
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Temp.DefaultCellStyle = dataGridViewCellStyle6;
            this.Temp.HeaderText = "Температура";
            this.Temp.Name = "Temp";
            this.Temp.ReadOnly = true;
            // 
            // Motoresource
            // 
            this.Motoresource.HeaderText = "Срабатываний";
            this.Motoresource.Name = "Motoresource";
            this.Motoresource.ReadOnly = true;
            // 
            // Channel
            // 
            dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Channel.DefaultCellStyle = dataGridViewCellStyle7;
            this.Channel.HeaderText = "Канал";
            this.Channel.Name = "Channel";
            this.Channel.ReadOnly = true;
            // 
            // Pulses
            // 
            dataGridViewCellStyle8.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.Pulses.DefaultCellStyle = dataGridViewCellStyle8;
            this.Pulses.HeaderText = "Импульсов";
            this.Pulses.Name = "Pulses";
            this.Pulses.ReadOnly = true;
            // 
            // Rod
            // 
            this.Rod.HeaderText = "Штанга";
            this.Rod.Name = "Rod";
            this.Rod.ReadOnly = true;
            // 
            // Etl
            // 
            this.Etl.HeaderText = "Эталон";
            this.Etl.Name = "Etl";
            this.Etl.ReadOnly = true;
            // 
            // Compare
            // 
            this.Compare.HeaderText = "Сравнение";
            this.Compare.Name = "Compare";
            this.Compare.ReadOnly = true;
            // 
            // Btn
            // 
            this.Btn.HeaderText = "График";
            this.Btn.Name = "Btn";
            this.Btn.ReadOnly = true;
            // 
            // plEmptySDWorkspace
            // 
            this.plEmptySDWorkspace.Location = new System.Drawing.Point(81, 220);
            this.plEmptySDWorkspace.Name = "plEmptySDWorkspace";
            this.plEmptySDWorkspace.Size = new System.Drawing.Size(200, 100);
            this.plEmptySDWorkspace.TabIndex = 2;
            // 
            // plEthalonChart
            // 
            this.plEthalonChart.Controls.Add(this.ethalonChart);
            this.plEthalonChart.Location = new System.Drawing.Point(248, 32);
            this.plEthalonChart.Name = "plEthalonChart";
            this.plEthalonChart.Size = new System.Drawing.Size(200, 193);
            this.plEthalonChart.TabIndex = 1;
            // 
            // ethalonChart
            // 
            chartArea2.Name = "ethalonChartArea";
            this.ethalonChart.ChartAreas.Add(chartArea2);
            this.ethalonChart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ethalonChart.Location = new System.Drawing.Point(0, 0);
            this.ethalonChart.Name = "ethalonChart";
            series2.ChartArea = "ethalonChartArea";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series2.Name = "ethalonChartSerie";
            this.ethalonChart.Series.Add(series2);
            this.ethalonChart.Size = new System.Drawing.Size(200, 193);
            this.ethalonChart.TabIndex = 0;
            this.ethalonChart.Text = "chart1";
            // 
            // toolStripSD
            // 
            this.toolStripSD.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStripSD.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnListSDFiles,
            this.btnViewSDFile,
            this.btnDeleteSDFile});
            this.toolStripSD.Location = new System.Drawing.Point(0, 0);
            this.toolStripSD.Name = "toolStripSD";
            this.toolStripSD.Size = new System.Drawing.Size(573, 54);
            this.toolStripSD.TabIndex = 4;
            // 
            // btnListSDFiles
            // 
            this.btnListSDFiles.Image = global::UROVConfig.Properties.Resources.rescan;
            this.btnListSDFiles.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnListSDFiles.Name = "btnListSDFiles";
            this.btnListSDFiles.Size = new System.Drawing.Size(75, 51);
            this.btnListSDFiles.Text = "Перечитать";
            this.btnListSDFiles.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnListSDFiles.ToolTipText = "Перечитать содержимое SD";
            this.btnListSDFiles.Click += new System.EventHandler(this.btnListSDFiles_Click);
            // 
            // btnViewSDFile
            // 
            this.btnViewSDFile.Enabled = false;
            this.btnViewSDFile.Image = global::UROVConfig.Properties.Resources.view;
            this.btnViewSDFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnViewSDFile.Name = "btnViewSDFile";
            this.btnViewSDFile.Size = new System.Drawing.Size(68, 51);
            this.btnViewSDFile.Text = "Просмотр";
            this.btnViewSDFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnViewSDFile.Click += new System.EventHandler(this.btnViewSDFile_Click);
            // 
            // btnDeleteSDFile
            // 
            this.btnDeleteSDFile.Enabled = false;
            this.btnDeleteSDFile.Image = global::UROVConfig.Properties.Resources.delete;
            this.btnDeleteSDFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDeleteSDFile.Name = "btnDeleteSDFile";
            this.btnDeleteSDFile.Size = new System.Drawing.Size(55, 51);
            this.btnDeleteSDFile.Text = "Удалить";
            this.btnDeleteSDFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnDeleteSDFile.ToolTipText = "Удалить выбранный файл";
            this.btnDeleteSDFile.Click += new System.EventHandler(this.btnDeleteSDFile_Click);
            // 
            // plMainSettings
            // 
            this.plMainSettings.Controls.Add(this.flowLayoutPanel1);
            this.plMainSettings.Location = new System.Drawing.Point(2, 0);
            this.plMainSettings.Name = "plMainSettings";
            this.plMainSettings.Size = new System.Drawing.Size(920, 594);
            this.plMainSettings.TabIndex = 9;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.BackColor = System.Drawing.Color.White;
            this.flowLayoutPanel1.Controls.Add(this.plInfo);
            this.flowLayoutPanel1.Controls.Add(this.plInductive);
            this.flowLayoutPanel1.Controls.Add(this.plVoltage);
            this.flowLayoutPanel1.Controls.Add(this.plMotoresourceCurrent);
            this.flowLayoutPanel1.Controls.Add(this.plMotoresourceMax);
            this.flowLayoutPanel1.Controls.Add(this.plImpulses);
            this.flowLayoutPanel1.Controls.Add(this.plDelta);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Padding = new System.Windows.Forms.Padding(10);
            this.flowLayoutPanel1.Size = new System.Drawing.Size(920, 594);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // plInfo
            // 
            this.plInfo.BackColor = System.Drawing.Color.SteelBlue;
            this.plInfo.Controls.Add(this.panel3);
            this.plInfo.Controls.Add(this.panel4);
            this.plInfo.Location = new System.Drawing.Point(20, 20);
            this.plInfo.Margin = new System.Windows.Forms.Padding(10);
            this.plInfo.Name = "plInfo";
            this.plInfo.Padding = new System.Windows.Forms.Padding(2);
            this.plInfo.Size = new System.Drawing.Size(200, 215);
            this.plInfo.TabIndex = 0;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.White;
            this.panel3.Controls.Add(this.btnSetDateTime2);
            this.panel3.Controls.Add(this.tbControllerTime);
            this.panel3.Controls.Add(this.label4);
            this.panel3.Controls.Add(this.tbFREERAM);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Controls.Add(this.tbFirmwareVersion);
            this.panel3.Controls.Add(this.label2);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(2, 26);
            this.panel3.Margin = new System.Windows.Forms.Padding(3, 11, 3, 3);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(196, 187);
            this.panel3.TabIndex = 2;
            // 
            // btnSetDateTime2
            // 
            this.btnSetDateTime2.BackColor = System.Drawing.Color.LightGreen;
            this.btnSetDateTime2.Enabled = false;
            this.btnSetDateTime2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetDateTime2.ForeColor = System.Drawing.Color.Black;
            this.btnSetDateTime2.Location = new System.Drawing.Point(16, 155);
            this.btnSetDateTime2.Name = "btnSetDateTime2";
            this.btnSetDateTime2.Size = new System.Drawing.Size(167, 23);
            this.btnSetDateTime2.TabIndex = 9;
            this.btnSetDateTime2.Text = "Установить время";
            this.btnSetDateTime2.UseVisualStyleBackColor = false;
            this.btnSetDateTime2.Click += new System.EventHandler(this.btnSetDateTime_Click);
            // 
            // tbControllerTime
            // 
            this.tbControllerTime.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbControllerTime.Location = new System.Drawing.Point(16, 126);
            this.tbControllerTime.Name = "tbControllerTime";
            this.tbControllerTime.ReadOnly = true;
            this.tbControllerTime.Size = new System.Drawing.Size(167, 20);
            this.tbControllerTime.TabIndex = 5;
            this.tbControllerTime.WordWrap = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(13, 110);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(111, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Время контроллера:";
            // 
            // tbFREERAM
            // 
            this.tbFREERAM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbFREERAM.Location = new System.Drawing.Point(16, 78);
            this.tbFREERAM.Name = "tbFREERAM";
            this.tbFREERAM.ReadOnly = true;
            this.tbFREERAM.Size = new System.Drawing.Size(167, 20);
            this.tbFREERAM.TabIndex = 3;
            this.tbFREERAM.WordWrap = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(13, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(105, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Свободная память:";
            // 
            // tbFirmwareVersion
            // 
            this.tbFirmwareVersion.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbFirmwareVersion.Location = new System.Drawing.Point(16, 35);
            this.tbFirmwareVersion.Name = "tbFirmwareVersion";
            this.tbFirmwareVersion.ReadOnly = true;
            this.tbFirmwareVersion.Size = new System.Drawing.Size(167, 20);
            this.tbFirmwareVersion.TabIndex = 1;
            this.tbFirmwareVersion.WordWrap = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(13, 19);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Версия ядра:";
            // 
            // panel4
            // 
            this.panel4.AutoSize = true;
            this.panel4.BackColor = System.Drawing.Color.SteelBlue;
            this.panel4.Controls.Add(this.label1);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel4.Location = new System.Drawing.Point(2, 2);
            this.panel4.Margin = new System.Windows.Forms.Padding(0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(196, 24);
            this.panel4.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(0);
            this.label1.Name = "label1";
            this.label1.Padding = new System.Windows.Forms.Padding(2);
            this.label1.Size = new System.Drawing.Size(123, 24);
            this.label1.TabIndex = 0;
            this.label1.Text = "Информация";
            // 
            // plInductive
            // 
            this.plInductive.BackColor = System.Drawing.Color.SteelBlue;
            this.plInductive.Controls.Add(this.panel13);
            this.plInductive.Controls.Add(this.panel14);
            this.plInductive.Location = new System.Drawing.Point(240, 20);
            this.plInductive.Margin = new System.Windows.Forms.Padding(10);
            this.plInductive.Name = "plInductive";
            this.plInductive.Padding = new System.Windows.Forms.Padding(2);
            this.plInductive.Size = new System.Drawing.Size(200, 215);
            this.plInductive.TabIndex = 5;
            // 
            // panel13
            // 
            this.panel13.BackColor = System.Drawing.Color.White;
            this.panel13.Controls.Add(this.lblInductive3);
            this.panel13.Controls.Add(this.lblInductive2);
            this.panel13.Controls.Add(this.lblInductive1);
            this.panel13.Controls.Add(this.label21);
            this.panel13.Controls.Add(this.label22);
            this.panel13.Controls.Add(this.label23);
            this.panel13.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel13.Location = new System.Drawing.Point(2, 26);
            this.panel13.Margin = new System.Windows.Forms.Padding(3, 11, 3, 3);
            this.panel13.Name = "panel13";
            this.panel13.Size = new System.Drawing.Size(196, 187);
            this.panel13.TabIndex = 2;
            // 
            // lblInductive3
            // 
            this.lblInductive3.BackColor = System.Drawing.Color.LightGray;
            this.lblInductive3.Location = new System.Drawing.Point(16, 126);
            this.lblInductive3.Name = "lblInductive3";
            this.lblInductive3.Size = new System.Drawing.Size(167, 20);
            this.lblInductive3.TabIndex = 7;
            this.lblInductive3.Text = "-";
            this.lblInductive3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblInductive2
            // 
            this.lblInductive2.BackColor = System.Drawing.Color.LightGray;
            this.lblInductive2.Location = new System.Drawing.Point(16, 78);
            this.lblInductive2.Name = "lblInductive2";
            this.lblInductive2.Size = new System.Drawing.Size(167, 20);
            this.lblInductive2.TabIndex = 6;
            this.lblInductive2.Text = "-";
            this.lblInductive2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblInductive1
            // 
            this.lblInductive1.BackColor = System.Drawing.Color.LightGray;
            this.lblInductive1.Location = new System.Drawing.Point(16, 35);
            this.lblInductive1.Name = "lblInductive1";
            this.lblInductive1.Size = new System.Drawing.Size(167, 20);
            this.lblInductive1.TabIndex = 5;
            this.lblInductive1.Text = "-";
            this.lblInductive1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.ForeColor = System.Drawing.Color.Black;
            this.label21.Location = new System.Drawing.Point(13, 110);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(61, 13);
            this.label21.TabIndex = 4;
            this.label21.Text = "Канал №3:";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.ForeColor = System.Drawing.Color.Black;
            this.label22.Location = new System.Drawing.Point(13, 62);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(61, 13);
            this.label22.TabIndex = 2;
            this.label22.Text = "Канал №2:";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.ForeColor = System.Drawing.Color.Black;
            this.label23.Location = new System.Drawing.Point(13, 19);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(61, 13);
            this.label23.TabIndex = 0;
            this.label23.Text = "Канал №1:";
            // 
            // panel14
            // 
            this.panel14.AutoSize = true;
            this.panel14.BackColor = System.Drawing.Color.SteelBlue;
            this.panel14.Controls.Add(this.label24);
            this.panel14.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel14.Location = new System.Drawing.Point(2, 2);
            this.panel14.Margin = new System.Windows.Forms.Padding(0);
            this.panel14.Name = "panel14";
            this.panel14.Size = new System.Drawing.Size(196, 24);
            this.panel14.TabIndex = 1;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Dock = System.Windows.Forms.DockStyle.Top;
            this.label24.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label24.ForeColor = System.Drawing.Color.White;
            this.label24.Location = new System.Drawing.Point(0, 0);
            this.label24.Margin = new System.Windows.Forms.Padding(0);
            this.label24.Name = "label24";
            this.label24.Padding = new System.Windows.Forms.Padding(2);
            this.label24.Size = new System.Drawing.Size(155, 24);
            this.label24.TabIndex = 0;
            this.label24.Text = "Индукт. датчики";
            // 
            // plVoltage
            // 
            this.plVoltage.BackColor = System.Drawing.Color.SteelBlue;
            this.plVoltage.Controls.Add(this.panel15);
            this.plVoltage.Controls.Add(this.panel16);
            this.plVoltage.Location = new System.Drawing.Point(460, 20);
            this.plVoltage.Margin = new System.Windows.Forms.Padding(10);
            this.plVoltage.Name = "plVoltage";
            this.plVoltage.Padding = new System.Windows.Forms.Padding(2);
            this.plVoltage.Size = new System.Drawing.Size(200, 215);
            this.plVoltage.TabIndex = 6;
            // 
            // panel15
            // 
            this.panel15.BackColor = System.Drawing.Color.White;
            this.panel15.Controls.Add(this.lblVoltage3);
            this.panel15.Controls.Add(this.lblVoltage2);
            this.panel15.Controls.Add(this.lblVoltage1);
            this.panel15.Controls.Add(this.label28);
            this.panel15.Controls.Add(this.label29);
            this.panel15.Controls.Add(this.label30);
            this.panel15.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel15.Location = new System.Drawing.Point(2, 26);
            this.panel15.Margin = new System.Windows.Forms.Padding(3, 11, 3, 3);
            this.panel15.Name = "panel15";
            this.panel15.Size = new System.Drawing.Size(196, 187);
            this.panel15.TabIndex = 2;
            // 
            // lblVoltage3
            // 
            this.lblVoltage3.BackColor = System.Drawing.Color.LightGray;
            this.lblVoltage3.Location = new System.Drawing.Point(16, 126);
            this.lblVoltage3.Name = "lblVoltage3";
            this.lblVoltage3.Size = new System.Drawing.Size(167, 20);
            this.lblVoltage3.TabIndex = 7;
            this.lblVoltage3.Text = "-";
            this.lblVoltage3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblVoltage2
            // 
            this.lblVoltage2.BackColor = System.Drawing.Color.LightGray;
            this.lblVoltage2.Location = new System.Drawing.Point(16, 78);
            this.lblVoltage2.Name = "lblVoltage2";
            this.lblVoltage2.Size = new System.Drawing.Size(167, 20);
            this.lblVoltage2.TabIndex = 6;
            this.lblVoltage2.Text = "-";
            this.lblVoltage2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblVoltage1
            // 
            this.lblVoltage1.BackColor = System.Drawing.Color.LightGray;
            this.lblVoltage1.Location = new System.Drawing.Point(16, 35);
            this.lblVoltage1.Name = "lblVoltage1";
            this.lblVoltage1.Size = new System.Drawing.Size(167, 20);
            this.lblVoltage1.TabIndex = 5;
            this.lblVoltage1.Text = "-";
            this.lblVoltage1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.ForeColor = System.Drawing.Color.Black;
            this.label28.Location = new System.Drawing.Point(13, 110);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(62, 13);
            this.label28.TabIndex = 4;
            this.label28.Text = "Вход 200В:";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.ForeColor = System.Drawing.Color.Black;
            this.label29.Location = new System.Drawing.Point(13, 62);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(50, 13);
            this.label29.TabIndex = 2;
            this.label29.Text = "Вход 5В:";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.ForeColor = System.Drawing.Color.Black;
            this.label30.Location = new System.Drawing.Point(13, 19);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(59, 13);
            this.label30.TabIndex = 0;
            this.label30.Text = "Вход 3.3В:";
            // 
            // panel16
            // 
            this.panel16.AutoSize = true;
            this.panel16.BackColor = System.Drawing.Color.SteelBlue;
            this.panel16.Controls.Add(this.label31);
            this.panel16.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel16.Location = new System.Drawing.Point(2, 2);
            this.panel16.Margin = new System.Windows.Forms.Padding(0);
            this.panel16.Name = "panel16";
            this.panel16.Size = new System.Drawing.Size(196, 24);
            this.panel16.TabIndex = 1;
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Dock = System.Windows.Forms.DockStyle.Top;
            this.label31.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label31.ForeColor = System.Drawing.Color.White;
            this.label31.Location = new System.Drawing.Point(0, 0);
            this.label31.Margin = new System.Windows.Forms.Padding(0);
            this.label31.Name = "label31";
            this.label31.Padding = new System.Windows.Forms.Padding(2);
            this.label31.Size = new System.Drawing.Size(148, 24);
            this.label31.TabIndex = 0;
            this.label31.Text = "Напряжение ИП";
            // 
            // plMotoresourceCurrent
            // 
            this.plMotoresourceCurrent.BackColor = System.Drawing.Color.SteelBlue;
            this.plMotoresourceCurrent.Controls.Add(this.panel5);
            this.plMotoresourceCurrent.Controls.Add(this.panel6);
            this.plMotoresourceCurrent.Location = new System.Drawing.Point(680, 20);
            this.plMotoresourceCurrent.Margin = new System.Windows.Forms.Padding(10);
            this.plMotoresourceCurrent.Name = "plMotoresourceCurrent";
            this.plMotoresourceCurrent.Padding = new System.Windows.Forms.Padding(2);
            this.plMotoresourceCurrent.Size = new System.Drawing.Size(200, 215);
            this.plMotoresourceCurrent.TabIndex = 1;
            // 
            // panel5
            // 
            this.panel5.BackColor = System.Drawing.Color.White;
            this.panel5.Controls.Add(this.lblMotoresourcePercents3);
            this.panel5.Controls.Add(this.lblMotoresourcePercents2);
            this.panel5.Controls.Add(this.lblMotoresourcePercents1);
            this.panel5.Controls.Add(this.btnSetMotoresourceCurrent);
            this.panel5.Controls.Add(this.nudMotoresourceCurrent3);
            this.panel5.Controls.Add(this.nudMotoresourceCurrent2);
            this.panel5.Controls.Add(this.nudMotoresourceCurrent1);
            this.panel5.Controls.Add(this.label5);
            this.panel5.Controls.Add(this.label6);
            this.panel5.Controls.Add(this.label7);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel5.Location = new System.Drawing.Point(2, 26);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(196, 187);
            this.panel5.TabIndex = 2;
            // 
            // lblMotoresourcePercents3
            // 
            this.lblMotoresourcePercents3.BackColor = System.Drawing.Color.White;
            this.lblMotoresourcePercents3.ForeColor = System.Drawing.Color.Green;
            this.lblMotoresourcePercents3.Location = new System.Drawing.Point(114, 128);
            this.lblMotoresourcePercents3.Name = "lblMotoresourcePercents3";
            this.lblMotoresourcePercents3.Size = new System.Drawing.Size(69, 13);
            this.lblMotoresourcePercents3.TabIndex = 11;
            this.lblMotoresourcePercents3.Text = "0%";
            this.lblMotoresourcePercents3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblMotoresourcePercents2
            // 
            this.lblMotoresourcePercents2.BackColor = System.Drawing.Color.White;
            this.lblMotoresourcePercents2.ForeColor = System.Drawing.Color.Green;
            this.lblMotoresourcePercents2.Location = new System.Drawing.Point(111, 80);
            this.lblMotoresourcePercents2.Name = "lblMotoresourcePercents2";
            this.lblMotoresourcePercents2.Size = new System.Drawing.Size(72, 13);
            this.lblMotoresourcePercents2.TabIndex = 10;
            this.lblMotoresourcePercents2.Text = "0%";
            this.lblMotoresourcePercents2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblMotoresourcePercents1
            // 
            this.lblMotoresourcePercents1.BackColor = System.Drawing.Color.White;
            this.lblMotoresourcePercents1.ForeColor = System.Drawing.Color.Green;
            this.lblMotoresourcePercents1.Location = new System.Drawing.Point(108, 37);
            this.lblMotoresourcePercents1.Name = "lblMotoresourcePercents1";
            this.lblMotoresourcePercents1.Size = new System.Drawing.Size(75, 13);
            this.lblMotoresourcePercents1.TabIndex = 9;
            this.lblMotoresourcePercents1.Text = "0%";
            this.lblMotoresourcePercents1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // btnSetMotoresourceCurrent
            // 
            this.btnSetMotoresourceCurrent.BackColor = System.Drawing.Color.LightGreen;
            this.btnSetMotoresourceCurrent.Enabled = false;
            this.btnSetMotoresourceCurrent.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetMotoresourceCurrent.ForeColor = System.Drawing.Color.Black;
            this.btnSetMotoresourceCurrent.Location = new System.Drawing.Point(16, 155);
            this.btnSetMotoresourceCurrent.Name = "btnSetMotoresourceCurrent";
            this.btnSetMotoresourceCurrent.Size = new System.Drawing.Size(167, 23);
            this.btnSetMotoresourceCurrent.TabIndex = 8;
            this.btnSetMotoresourceCurrent.Text = "Установить";
            this.btnSetMotoresourceCurrent.UseVisualStyleBackColor = false;
            this.btnSetMotoresourceCurrent.Click += new System.EventHandler(this.btnSetMotoresourceCurrent_Click);
            // 
            // nudMotoresourceCurrent3
            // 
            this.nudMotoresourceCurrent3.Location = new System.Drawing.Point(16, 126);
            this.nudMotoresourceCurrent3.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceCurrent3.Name = "nudMotoresourceCurrent3";
            this.nudMotoresourceCurrent3.Size = new System.Drawing.Size(86, 20);
            this.nudMotoresourceCurrent3.TabIndex = 7;
            this.nudMotoresourceCurrent3.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // nudMotoresourceCurrent2
            // 
            this.nudMotoresourceCurrent2.Location = new System.Drawing.Point(16, 78);
            this.nudMotoresourceCurrent2.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceCurrent2.Name = "nudMotoresourceCurrent2";
            this.nudMotoresourceCurrent2.Size = new System.Drawing.Size(86, 20);
            this.nudMotoresourceCurrent2.TabIndex = 6;
            this.nudMotoresourceCurrent2.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // nudMotoresourceCurrent1
            // 
            this.nudMotoresourceCurrent1.Location = new System.Drawing.Point(16, 35);
            this.nudMotoresourceCurrent1.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceCurrent1.Name = "nudMotoresourceCurrent1";
            this.nudMotoresourceCurrent1.Size = new System.Drawing.Size(86, 20);
            this.nudMotoresourceCurrent1.TabIndex = 5;
            this.nudMotoresourceCurrent1.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(13, 110);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(61, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Канал №3:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(13, 62);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "Канал №2:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.ForeColor = System.Drawing.Color.Black;
            this.label7.Location = new System.Drawing.Point(13, 19);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(61, 13);
            this.label7.TabIndex = 0;
            this.label7.Text = "Канал №1:";
            // 
            // panel6
            // 
            this.panel6.AutoSize = true;
            this.panel6.BackColor = System.Drawing.Color.SteelBlue;
            this.panel6.Controls.Add(this.label8);
            this.panel6.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel6.Location = new System.Drawing.Point(2, 2);
            this.panel6.Margin = new System.Windows.Forms.Padding(0);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(196, 24);
            this.panel6.TabIndex = 1;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Dock = System.Windows.Forms.DockStyle.Top;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label8.ForeColor = System.Drawing.Color.White;
            this.label8.Location = new System.Drawing.Point(0, 0);
            this.label8.Margin = new System.Windows.Forms.Padding(0);
            this.label8.Name = "label8";
            this.label8.Padding = new System.Windows.Forms.Padding(2);
            this.label8.Size = new System.Drawing.Size(146, 24);
            this.label8.TabIndex = 0;
            this.label8.Text = "Ресурс текущий";
            // 
            // plMotoresourceMax
            // 
            this.plMotoresourceMax.BackColor = System.Drawing.Color.SteelBlue;
            this.plMotoresourceMax.Controls.Add(this.panel7);
            this.plMotoresourceMax.Controls.Add(this.panel8);
            this.plMotoresourceMax.Location = new System.Drawing.Point(20, 255);
            this.plMotoresourceMax.Margin = new System.Windows.Forms.Padding(10);
            this.plMotoresourceMax.Name = "plMotoresourceMax";
            this.plMotoresourceMax.Padding = new System.Windows.Forms.Padding(2);
            this.plMotoresourceMax.Size = new System.Drawing.Size(200, 215);
            this.plMotoresourceMax.TabIndex = 2;
            // 
            // panel7
            // 
            this.panel7.BackColor = System.Drawing.Color.White;
            this.panel7.Controls.Add(this.btnSetMotoresourceMax);
            this.panel7.Controls.Add(this.nudMotoresourceMax3);
            this.panel7.Controls.Add(this.nudMotoresourceMax2);
            this.panel7.Controls.Add(this.nudMotoresourceMax1);
            this.panel7.Controls.Add(this.label9);
            this.panel7.Controls.Add(this.label10);
            this.panel7.Controls.Add(this.label11);
            this.panel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel7.Location = new System.Drawing.Point(2, 26);
            this.panel7.Name = "panel7";
            this.panel7.Size = new System.Drawing.Size(196, 187);
            this.panel7.TabIndex = 2;
            // 
            // btnSetMotoresourceMax
            // 
            this.btnSetMotoresourceMax.BackColor = System.Drawing.Color.LightGreen;
            this.btnSetMotoresourceMax.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetMotoresourceMax.ForeColor = System.Drawing.Color.Black;
            this.btnSetMotoresourceMax.Location = new System.Drawing.Point(16, 155);
            this.btnSetMotoresourceMax.Name = "btnSetMotoresourceMax";
            this.btnSetMotoresourceMax.Size = new System.Drawing.Size(167, 23);
            this.btnSetMotoresourceMax.TabIndex = 8;
            this.btnSetMotoresourceMax.Text = "Установить";
            this.btnSetMotoresourceMax.UseVisualStyleBackColor = false;
            this.btnSetMotoresourceMax.Click += new System.EventHandler(this.btnSetMotoresourceMax_Click);
            // 
            // nudMotoresourceMax3
            // 
            this.nudMotoresourceMax3.Location = new System.Drawing.Point(16, 126);
            this.nudMotoresourceMax3.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceMax3.Name = "nudMotoresourceMax3";
            this.nudMotoresourceMax3.Size = new System.Drawing.Size(167, 20);
            this.nudMotoresourceMax3.TabIndex = 7;
            this.nudMotoresourceMax3.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // nudMotoresourceMax2
            // 
            this.nudMotoresourceMax2.Location = new System.Drawing.Point(16, 78);
            this.nudMotoresourceMax2.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceMax2.Name = "nudMotoresourceMax2";
            this.nudMotoresourceMax2.Size = new System.Drawing.Size(167, 20);
            this.nudMotoresourceMax2.TabIndex = 6;
            this.nudMotoresourceMax2.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // nudMotoresourceMax1
            // 
            this.nudMotoresourceMax1.Location = new System.Drawing.Point(16, 35);
            this.nudMotoresourceMax1.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.nudMotoresourceMax1.Name = "nudMotoresourceMax1";
            this.nudMotoresourceMax1.Size = new System.Drawing.Size(167, 20);
            this.nudMotoresourceMax1.TabIndex = 5;
            this.nudMotoresourceMax1.ValueChanged += new System.EventHandler(this.MotoresourceCurrentValueChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.ForeColor = System.Drawing.Color.Black;
            this.label9.Location = new System.Drawing.Point(13, 110);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(61, 13);
            this.label9.TabIndex = 4;
            this.label9.Text = "Канал №3:";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.ForeColor = System.Drawing.Color.Black;
            this.label10.Location = new System.Drawing.Point(13, 62);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(61, 13);
            this.label10.TabIndex = 2;
            this.label10.Text = "Канал №2:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.ForeColor = System.Drawing.Color.Black;
            this.label11.Location = new System.Drawing.Point(13, 19);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(61, 13);
            this.label11.TabIndex = 0;
            this.label11.Text = "Канал №1:";
            // 
            // panel8
            // 
            this.panel8.AutoSize = true;
            this.panel8.BackColor = System.Drawing.Color.SteelBlue;
            this.panel8.Controls.Add(this.label12);
            this.panel8.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel8.Location = new System.Drawing.Point(2, 2);
            this.panel8.Margin = new System.Windows.Forms.Padding(0);
            this.panel8.Name = "panel8";
            this.panel8.Size = new System.Drawing.Size(196, 24);
            this.panel8.TabIndex = 1;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Dock = System.Windows.Forms.DockStyle.Top;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label12.ForeColor = System.Drawing.Color.White;
            this.label12.Location = new System.Drawing.Point(0, 0);
            this.label12.Margin = new System.Windows.Forms.Padding(0);
            this.label12.Name = "label12";
            this.label12.Padding = new System.Windows.Forms.Padding(2);
            this.label12.Size = new System.Drawing.Size(120, 24);
            this.label12.TabIndex = 0;
            this.label12.Text = "Ресурс макс.";
            // 
            // plImpulses
            // 
            this.plImpulses.BackColor = System.Drawing.Color.SteelBlue;
            this.plImpulses.Controls.Add(this.panel9);
            this.plImpulses.Controls.Add(this.panel10);
            this.plImpulses.Location = new System.Drawing.Point(240, 255);
            this.plImpulses.Margin = new System.Windows.Forms.Padding(10);
            this.plImpulses.Name = "plImpulses";
            this.plImpulses.Padding = new System.Windows.Forms.Padding(2);
            this.plImpulses.Size = new System.Drawing.Size(200, 215);
            this.plImpulses.TabIndex = 3;
            // 
            // panel9
            // 
            this.panel9.BackColor = System.Drawing.Color.White;
            this.panel9.Controls.Add(this.btnSetPulses);
            this.panel9.Controls.Add(this.nudPulses3);
            this.panel9.Controls.Add(this.nudPulses2);
            this.panel9.Controls.Add(this.nudPulses1);
            this.panel9.Controls.Add(this.label13);
            this.panel9.Controls.Add(this.label14);
            this.panel9.Controls.Add(this.label15);
            this.panel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel9.Location = new System.Drawing.Point(2, 26);
            this.panel9.Name = "panel9";
            this.panel9.Size = new System.Drawing.Size(196, 187);
            this.panel9.TabIndex = 2;
            // 
            // btnSetPulses
            // 
            this.btnSetPulses.BackColor = System.Drawing.Color.LightGreen;
            this.btnSetPulses.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetPulses.ForeColor = System.Drawing.Color.Black;
            this.btnSetPulses.Location = new System.Drawing.Point(16, 155);
            this.btnSetPulses.Name = "btnSetPulses";
            this.btnSetPulses.Size = new System.Drawing.Size(167, 23);
            this.btnSetPulses.TabIndex = 8;
            this.btnSetPulses.Text = "Установить";
            this.btnSetPulses.UseVisualStyleBackColor = false;
            this.btnSetPulses.Click += new System.EventHandler(this.btnSetPulses_Click);
            // 
            // nudPulses3
            // 
            this.nudPulses3.Location = new System.Drawing.Point(16, 126);
            this.nudPulses3.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.nudPulses3.Name = "nudPulses3";
            this.nudPulses3.Size = new System.Drawing.Size(167, 20);
            this.nudPulses3.TabIndex = 7;
            // 
            // nudPulses2
            // 
            this.nudPulses2.Location = new System.Drawing.Point(16, 78);
            this.nudPulses2.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.nudPulses2.Name = "nudPulses2";
            this.nudPulses2.Size = new System.Drawing.Size(167, 20);
            this.nudPulses2.TabIndex = 6;
            // 
            // nudPulses1
            // 
            this.nudPulses1.Location = new System.Drawing.Point(16, 35);
            this.nudPulses1.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.nudPulses1.Name = "nudPulses1";
            this.nudPulses1.Size = new System.Drawing.Size(167, 20);
            this.nudPulses1.TabIndex = 5;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.ForeColor = System.Drawing.Color.Black;
            this.label13.Location = new System.Drawing.Point(13, 110);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(61, 13);
            this.label13.TabIndex = 4;
            this.label13.Text = "Канал №3:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.ForeColor = System.Drawing.Color.Black;
            this.label14.Location = new System.Drawing.Point(13, 62);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(61, 13);
            this.label14.TabIndex = 2;
            this.label14.Text = "Канал №2:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.ForeColor = System.Drawing.Color.Black;
            this.label15.Location = new System.Drawing.Point(13, 19);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(61, 13);
            this.label15.TabIndex = 0;
            this.label15.Text = "Канал №1:";
            // 
            // panel10
            // 
            this.panel10.AutoSize = true;
            this.panel10.BackColor = System.Drawing.Color.SteelBlue;
            this.panel10.Controls.Add(this.label16);
            this.panel10.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel10.Location = new System.Drawing.Point(2, 2);
            this.panel10.Margin = new System.Windows.Forms.Padding(0);
            this.panel10.Name = "panel10";
            this.panel10.Size = new System.Drawing.Size(196, 24);
            this.panel10.TabIndex = 1;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Dock = System.Windows.Forms.DockStyle.Top;
            this.label16.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label16.ForeColor = System.Drawing.Color.White;
            this.label16.Location = new System.Drawing.Point(0, 0);
            this.label16.Margin = new System.Windows.Forms.Padding(0);
            this.label16.Name = "label16";
            this.label16.Padding = new System.Windows.Forms.Padding(2);
            this.label16.Size = new System.Drawing.Size(97, 24);
            this.label16.TabIndex = 0;
            this.label16.Text = "Импульсы";
            // 
            // plDelta
            // 
            this.plDelta.BackColor = System.Drawing.Color.SteelBlue;
            this.plDelta.Controls.Add(this.panel11);
            this.plDelta.Controls.Add(this.panel12);
            this.plDelta.Location = new System.Drawing.Point(460, 255);
            this.plDelta.Margin = new System.Windows.Forms.Padding(10);
            this.plDelta.Name = "plDelta";
            this.plDelta.Padding = new System.Windows.Forms.Padding(2);
            this.plDelta.Size = new System.Drawing.Size(200, 215);
            this.plDelta.TabIndex = 4;
            // 
            // panel11
            // 
            this.panel11.BackColor = System.Drawing.Color.White;
            this.panel11.Controls.Add(this.btnSetDelta);
            this.panel11.Controls.Add(this.nudDelta3);
            this.panel11.Controls.Add(this.nudDelta2);
            this.panel11.Controls.Add(this.nudDelta1);
            this.panel11.Controls.Add(this.label17);
            this.panel11.Controls.Add(this.label18);
            this.panel11.Controls.Add(this.label19);
            this.panel11.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel11.Location = new System.Drawing.Point(2, 26);
            this.panel11.Name = "panel11";
            this.panel11.Size = new System.Drawing.Size(196, 187);
            this.panel11.TabIndex = 2;
            // 
            // btnSetDelta
            // 
            this.btnSetDelta.BackColor = System.Drawing.Color.LightGreen;
            this.btnSetDelta.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetDelta.ForeColor = System.Drawing.Color.Black;
            this.btnSetDelta.Location = new System.Drawing.Point(16, 155);
            this.btnSetDelta.Name = "btnSetDelta";
            this.btnSetDelta.Size = new System.Drawing.Size(167, 23);
            this.btnSetDelta.TabIndex = 8;
            this.btnSetDelta.Text = "Установить";
            this.btnSetDelta.UseVisualStyleBackColor = false;
            this.btnSetDelta.Click += new System.EventHandler(this.btnSetDelta_Click);
            // 
            // nudDelta3
            // 
            this.nudDelta3.Location = new System.Drawing.Point(16, 126);
            this.nudDelta3.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudDelta3.Name = "nudDelta3";
            this.nudDelta3.Size = new System.Drawing.Size(167, 20);
            this.nudDelta3.TabIndex = 7;
            // 
            // nudDelta2
            // 
            this.nudDelta2.Location = new System.Drawing.Point(16, 78);
            this.nudDelta2.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudDelta2.Name = "nudDelta2";
            this.nudDelta2.Size = new System.Drawing.Size(167, 20);
            this.nudDelta2.TabIndex = 6;
            // 
            // nudDelta1
            // 
            this.nudDelta1.Location = new System.Drawing.Point(16, 35);
            this.nudDelta1.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudDelta1.Name = "nudDelta1";
            this.nudDelta1.Size = new System.Drawing.Size(167, 20);
            this.nudDelta1.TabIndex = 5;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.ForeColor = System.Drawing.Color.Black;
            this.label17.Location = new System.Drawing.Point(13, 110);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(61, 13);
            this.label17.TabIndex = 4;
            this.label17.Text = "Канал №3:";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.ForeColor = System.Drawing.Color.Black;
            this.label18.Location = new System.Drawing.Point(13, 62);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(61, 13);
            this.label18.TabIndex = 2;
            this.label18.Text = "Канал №2:";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.ForeColor = System.Drawing.Color.Black;
            this.label19.Location = new System.Drawing.Point(13, 19);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(61, 13);
            this.label19.TabIndex = 0;
            this.label19.Text = "Канал №1:";
            // 
            // panel12
            // 
            this.panel12.AutoSize = true;
            this.panel12.BackColor = System.Drawing.Color.SteelBlue;
            this.panel12.Controls.Add(this.label20);
            this.panel12.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel12.Location = new System.Drawing.Point(2, 2);
            this.panel12.Margin = new System.Windows.Forms.Padding(0);
            this.panel12.Name = "panel12";
            this.panel12.Size = new System.Drawing.Size(196, 24);
            this.panel12.TabIndex = 1;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Dock = System.Windows.Forms.DockStyle.Top;
            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label20.ForeColor = System.Drawing.Color.White;
            this.label20.Location = new System.Drawing.Point(0, 0);
            this.label20.Margin = new System.Windows.Forms.Padding(0);
            this.label20.Name = "label20";
            this.label20.Padding = new System.Windows.Forms.Padding(2);
            this.label20.Size = new System.Drawing.Size(174, 24);
            this.label20.TabIndex = 0;
            this.label20.Text = "Дельты импульсов";
            // 
            // plStartPanel
            // 
            this.plStartPanel.Location = new System.Drawing.Point(480, 81);
            this.plStartPanel.Name = "plStartPanel";
            this.plStartPanel.Size = new System.Drawing.Size(671, 346);
            this.plStartPanel.TabIndex = 10;
            // 
            // lvLog
            // 
            this.lvLog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.logColumn1,
            this.logColumn2});
            this.lvLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLog.FullRowSelect = true;
            this.lvLog.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvLog.Location = new System.Drawing.Point(0, 0);
            this.lvLog.Name = "lvLog";
            this.lvLog.ShowGroups = false;
            this.lvLog.Size = new System.Drawing.Size(1269, 118);
            this.lvLog.SmallImageList = this.smallImages;
            this.lvLog.TabIndex = 0;
            this.lvLog.UseCompatibleStateImageBehavior = false;
            this.lvLog.View = System.Windows.Forms.View.Details;
            // 
            // logColumn1
            // 
            this.logColumn1.Width = 200;
            // 
            // sensorImages
            // 
            this.sensorImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("sensorImages.ImageStream")));
            this.sensorImages.TransparentColor = System.Drawing.Color.Transparent;
            this.sensorImages.Images.SetKeyName(0, "any.png");
            // 
            // dataImages
            // 
            this.dataImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("dataImages.ImageStream")));
            this.dataImages.TransparentColor = System.Drawing.Color.Transparent;
            this.dataImages.Images.SetKeyName(0, "analogport.png");
            this.dataImages.Images.SetKeyName(1, "datetime.png");
            this.dataImages.Images.SetKeyName(2, "digitalport.png");
            this.dataImages.Images.SetKeyName(3, "humidity.png");
            this.dataImages.Images.SetKeyName(4, "luminosity.png");
            this.dataImages.Images.SetKeyName(5, "temperature.png");
            this.dataImages.Images.SetKeyName(6, "userdata.png");
            this.dataImages.Images.SetKeyName(7, "altitude.png");
            this.dataImages.Images.SetKeyName(8, "barometric.png");
            this.dataImages.Images.SetKeyName(9, "pressure.png");
            this.dataImages.Images.SetKeyName(10, "distance.png");
            // 
            // tmDateTime
            // 
            this.tmDateTime.Enabled = true;
            this.tmDateTime.Interval = 1000;
            this.tmDateTime.Tick += new System.EventHandler(this.tmDateTime_Tick);
            // 
            // tmEnumComPorts
            // 
            this.tmEnumComPorts.Enabled = true;
            this.tmEnumComPorts.Interval = 2000;
            this.tmEnumComPorts.Tick += new System.EventHandler(this.tmEnumComPorts_Tick);
            // 
            // tmPeriodicCommandsTimer
            // 
            this.tmPeriodicCommandsTimer.Interval = 5000;
            this.tmPeriodicCommandsTimer.Tick += new System.EventHandler(this.tmInductiveTimer_Tick);
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            dataGridViewCellStyle9.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleRight;
            this.dataGridViewTextBoxColumn1.DefaultCellStyle = dataGridViewCellStyle9;
            this.dataGridViewTextBoxColumn1.HeaderText = "№";
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.dataGridViewTextBoxColumn2.HeaderText = "Время";
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            this.dataGridViewTextBoxColumn2.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn3
            // 
            dataGridViewCellStyle10.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.dataGridViewTextBoxColumn3.DefaultCellStyle = dataGridViewCellStyle10;
            this.dataGridViewTextBoxColumn3.HeaderText = "Температура";
            this.dataGridViewTextBoxColumn3.Name = "dataGridViewTextBoxColumn3";
            this.dataGridViewTextBoxColumn3.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn4
            // 
            this.dataGridViewTextBoxColumn4.HeaderText = "Канал";
            this.dataGridViewTextBoxColumn4.Name = "dataGridViewTextBoxColumn4";
            this.dataGridViewTextBoxColumn4.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn5
            // 
            dataGridViewCellStyle11.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.dataGridViewTextBoxColumn5.DefaultCellStyle = dataGridViewCellStyle11;
            this.dataGridViewTextBoxColumn5.HeaderText = "Штанга";
            this.dataGridViewTextBoxColumn5.Name = "dataGridViewTextBoxColumn5";
            this.dataGridViewTextBoxColumn5.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn6
            // 
            dataGridViewCellStyle12.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.dataGridViewTextBoxColumn6.DefaultCellStyle = dataGridViewCellStyle12;
            this.dataGridViewTextBoxColumn6.HeaderText = "Сравнение";
            this.dataGridViewTextBoxColumn6.Name = "dataGridViewTextBoxColumn6";
            this.dataGridViewTextBoxColumn6.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn7
            // 
            this.dataGridViewTextBoxColumn7.HeaderText = "Сравнение";
            this.dataGridViewTextBoxColumn7.Name = "dataGridViewTextBoxColumn7";
            this.dataGridViewTextBoxColumn7.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn8
            // 
            this.dataGridViewTextBoxColumn8.HeaderText = "Сравнение";
            this.dataGridViewTextBoxColumn8.Name = "dataGridViewTextBoxColumn8";
            this.dataGridViewTextBoxColumn8.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn9
            // 
            this.dataGridViewTextBoxColumn9.HeaderText = "Сравнение";
            this.dataGridViewTextBoxColumn9.Name = "dataGridViewTextBoxColumn9";
            this.dataGridViewTextBoxColumn9.ReadOnly = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1269, 771);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.menuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip;
            this.MinimumSize = new System.Drawing.Size(800, 600);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Конфигуратор UROV";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Resize += new System.EventHandler(this.MainForm_Resize);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.plSection.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.archiveLogDataGrid)).EndInit();
            this.plArchiveEthalonChart.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.archiveAthalonChart)).EndInit();
            this.plSDSettings.ResumeLayout(false);
            this.plSDSettings.PerformLayout();
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.logDataGrid)).EndInit();
            this.plEthalonChart.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ethalonChart)).EndInit();
            this.toolStripSD.ResumeLayout(false);
            this.toolStripSD.PerformLayout();
            this.plMainSettings.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.plInfo.ResumeLayout(false);
            this.plInfo.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.plInductive.ResumeLayout(false);
            this.plInductive.PerformLayout();
            this.panel13.ResumeLayout(false);
            this.panel13.PerformLayout();
            this.panel14.ResumeLayout(false);
            this.panel14.PerformLayout();
            this.plVoltage.ResumeLayout(false);
            this.plVoltage.PerformLayout();
            this.panel15.ResumeLayout(false);
            this.panel15.PerformLayout();
            this.panel16.ResumeLayout(false);
            this.panel16.PerformLayout();
            this.plMotoresourceCurrent.ResumeLayout(false);
            this.plMotoresourceCurrent.PerformLayout();
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceCurrent1)).EndInit();
            this.panel6.ResumeLayout(false);
            this.panel6.PerformLayout();
            this.plMotoresourceMax.ResumeLayout(false);
            this.plMotoresourceMax.PerformLayout();
            this.panel7.ResumeLayout(false);
            this.panel7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMotoresourceMax1)).EndInit();
            this.panel8.ResumeLayout(false);
            this.panel8.PerformLayout();
            this.plImpulses.ResumeLayout(false);
            this.plImpulses.PerformLayout();
            this.panel9.ResumeLayout(false);
            this.panel9.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPulses1)).EndInit();
            this.panel10.ResumeLayout(false);
            this.panel10.PerformLayout();
            this.plDelta.ResumeLayout(false);
            this.plDelta.PerformLayout();
            this.panel11.ResumeLayout(false);
            this.panel11.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelta1)).EndInit();
            this.panel12.ResumeLayout(false);
            this.panel12.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripDropDownButton btnConnect;
        private System.Windows.Forms.ImageList toolbarImages;
        private System.Windows.Forms.Timer tmProcessCommandsTimer;
        private System.Windows.Forms.Timer tmGetSensorsData;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Panel plSection;
        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.Timer tmDateTime;
        private System.Windows.Forms.ToolStripButton btnSetDateTime;
        private System.Windows.Forms.ListView lvLog;
        private System.Windows.Forms.ColumnHeader logColumn1;
        private System.Windows.Forms.ColumnHeader logColumn2;
        private System.Windows.Forms.ImageList smallImages;
        private System.Windows.Forms.ImageList dataImages;
        private System.Windows.Forms.ImageList sensorImages;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton btnAbout;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem miHelp;
        private System.Windows.Forms.ToolStripMenuItem miAbout;
        private System.Windows.Forms.Panel plSDSettings;
        private System.Windows.Forms.ImageList sdImagesNormal;
        private System.Windows.Forms.Panel plMainSettings;
        private System.Windows.Forms.Timer tmEnumComPorts;
        private System.Windows.Forms.ToolStripButton btnDisconnect;
        private System.Windows.Forms.ToolStripMenuItem miPort;
        private System.Windows.Forms.ToolStripMenuItem portSpeedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps9600ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps19200ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps38400ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps57600ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps74880ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps115200ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ps250000ToolStripMenuItem;
        private System.Windows.Forms.Panel plStartPanel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Panel plInfo;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbFirmwareVersion;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbFREERAM;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbControllerTime;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel plMotoresourceCurrent;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown nudMotoresourceCurrent3;
        private System.Windows.Forms.NumericUpDown nudMotoresourceCurrent2;
        private System.Windows.Forms.NumericUpDown nudMotoresourceCurrent1;
        private System.Windows.Forms.Button btnSetDateTime2;
        private System.Windows.Forms.Button btnSetMotoresourceCurrent;
        private System.Windows.Forms.Panel plMotoresourceMax;
        private System.Windows.Forms.Panel panel7;
        private System.Windows.Forms.Button btnSetMotoresourceMax;
        private System.Windows.Forms.NumericUpDown nudMotoresourceMax3;
        private System.Windows.Forms.NumericUpDown nudMotoresourceMax2;
        private System.Windows.Forms.NumericUpDown nudMotoresourceMax1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Panel panel8;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Panel plImpulses;
        private System.Windows.Forms.Panel panel9;
        private System.Windows.Forms.Button btnSetPulses;
        private System.Windows.Forms.NumericUpDown nudPulses3;
        private System.Windows.Forms.NumericUpDown nudPulses2;
        private System.Windows.Forms.NumericUpDown nudPulses1;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Panel panel10;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Panel plDelta;
        private System.Windows.Forms.Panel panel11;
        private System.Windows.Forms.Button btnSetDelta;
        private System.Windows.Forms.NumericUpDown nudDelta3;
        private System.Windows.Forms.NumericUpDown nudDelta2;
        private System.Windows.Forms.NumericUpDown nudDelta1;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Panel panel12;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Panel plInductive;
        private System.Windows.Forms.Panel panel13;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Panel panel14;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label lblInductive3;
        private System.Windows.Forms.Label lblInductive2;
        private System.Windows.Forms.Label lblInductive1;
        private System.Windows.Forms.Timer tmPeriodicCommandsTimer;
        private System.Windows.Forms.Panel plVoltage;
        private System.Windows.Forms.Panel panel15;
        private System.Windows.Forms.Label lblVoltage3;
        private System.Windows.Forms.Label lblVoltage2;
        private System.Windows.Forms.Label lblVoltage1;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Panel panel16;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Label lblMotoresourcePercents3;
        private System.Windows.Forms.Label lblMotoresourcePercents2;
        private System.Windows.Forms.Label lblMotoresourcePercents1;
        private System.Windows.Forms.ToolStrip toolStripSD;
        private System.Windows.Forms.ToolStripButton btnListSDFiles;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.TreeView treeViewSD;
        private System.Windows.Forms.ToolStripButton btnDeleteSDFile;
        private System.Windows.Forms.ToolStripButton btnViewSDFile;
        private System.Windows.Forms.ToolStripStatusLabel statusProgressMessage;
        private System.Windows.Forms.ToolStripProgressBar statusProgressBar;
        private System.Windows.Forms.Panel plEthalonChart;
        private System.Windows.Forms.Panel plEmptySDWorkspace;
        private System.Windows.Forms.DataVisualization.Charting.Chart ethalonChart;
        private System.Windows.Forms.DataGridView logDataGrid;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn3;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn4;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn5;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn6;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn7;
        private System.Windows.Forms.DataGridViewTextBoxColumn Num;
        private System.Windows.Forms.DataGridViewTextBoxColumn Time;
        private System.Windows.Forms.DataGridViewTextBoxColumn Temp;
        private System.Windows.Forms.DataGridViewTextBoxColumn Motoresource;
        private System.Windows.Forms.DataGridViewTextBoxColumn Channel;
        private System.Windows.Forms.DataGridViewTextBoxColumn Pulses;
        private System.Windows.Forms.DataGridViewTextBoxColumn Rod;
        private System.Windows.Forms.DataGridViewTextBoxColumn Etl;
        private System.Windows.Forms.DataGridViewTextBoxColumn Compare;
        private System.Windows.Forms.DataGridViewButtonColumn Btn;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn8;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn9;
        private System.Windows.Forms.ToolStripStatusLabel connectStatusMessage;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.ToolStripButton btnUploadEthalon;
        private System.Windows.Forms.ToolStripButton btnControllerName;
        private System.Windows.Forms.ToolStripButton btnImportSettings;
        private System.Windows.Forms.Panel plArchiveEthalonChart;
        private System.Windows.Forms.DataVisualization.Charting.Chart archiveAthalonChart;
        private System.Windows.Forms.DataGridView archiveLogDataGrid;
        private System.Windows.Forms.DataGridViewTextBoxColumn Num1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Time1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Temp1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Motoresource1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Channel1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Pulses1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Rod1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Etl1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Compare1;
        private System.Windows.Forms.DataGridViewButtonColumn Btn1;
    }
}

