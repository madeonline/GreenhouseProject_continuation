namespace UROVConfig
{
    partial class ArchiveImportForm
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tbControllerName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cbSettings = new System.Windows.Forms.CheckBox();
            this.cbLogs = new System.Windows.Forms.CheckBox();
            this.cbEthalons = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.pbProgress = new System.Windows.Forms.ProgressBar();
            this.lblMessage = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.LightSalmon;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCancel.ForeColor = System.Drawing.Color.Black;
            this.btnCancel.Location = new System.Drawing.Point(398, 247);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(106, 37);
            this.btnCancel.TabIndex = 10;
            this.btnCancel.Text = "Отмена";
            this.btnCancel.UseVisualStyleBackColor = false;
            // 
            // btnOK
            // 
            this.btnOK.BackColor = System.Drawing.Color.LightGreen;
            this.btnOK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnOK.ForeColor = System.Drawing.Color.Black;
            this.btnOK.Location = new System.Drawing.Point(236, 247);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(156, 37);
            this.btnOK.TabIndex = 9;
            this.btnOK.Text = "Начать архивирование";
            this.btnOK.UseVisualStyleBackColor = false;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Имя контроллера:";
            // 
            // tbControllerName
            // 
            this.tbControllerName.Location = new System.Drawing.Point(12, 25);
            this.tbControllerName.Name = "tbControllerName";
            this.tbControllerName.Size = new System.Drawing.Size(492, 20);
            this.tbControllerName.TabIndex = 12;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(152, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "Данные для архивирования:";
            // 
            // cbSettings
            // 
            this.cbSettings.AutoSize = true;
            this.cbSettings.Checked = true;
            this.cbSettings.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbSettings.Location = new System.Drawing.Point(30, 89);
            this.cbSettings.Name = "cbSettings";
            this.cbSettings.Size = new System.Drawing.Size(81, 17);
            this.cbSettings.TabIndex = 14;
            this.cbSettings.Text = "Настройки";
            this.cbSettings.UseVisualStyleBackColor = true;
            // 
            // cbLogs
            // 
            this.cbLogs.AutoSize = true;
            this.cbLogs.Checked = true;
            this.cbLogs.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbLogs.Location = new System.Drawing.Point(30, 112);
            this.cbLogs.Name = "cbLogs";
            this.cbLogs.Size = new System.Drawing.Size(82, 17);
            this.cbLogs.TabIndex = 15;
            this.cbLogs.Text = "Лог-файлы";
            this.cbLogs.UseVisualStyleBackColor = true;
            // 
            // cbEthalons
            // 
            this.cbEthalons.AutoSize = true;
            this.cbEthalons.Checked = true;
            this.cbEthalons.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbEthalons.Location = new System.Drawing.Point(30, 135);
            this.cbEthalons.Name = "cbEthalons";
            this.cbEthalons.Size = new System.Drawing.Size(70, 17);
            this.cbEthalons.TabIndex = 16;
            this.cbEthalons.Text = "Эталоны";
            this.cbEthalons.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 172);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 17;
            this.label3.Text = "Прогресс:";
            // 
            // pbProgress
            // 
            this.pbProgress.Location = new System.Drawing.Point(15, 188);
            this.pbProgress.Name = "pbProgress";
            this.pbProgress.Size = new System.Drawing.Size(489, 23);
            this.pbProgress.Step = 1;
            this.pbProgress.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pbProgress.TabIndex = 18;
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = true;
            this.lblMessage.Location = new System.Drawing.Point(12, 214);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(0, 13);
            this.lblMessage.TabIndex = 19;
            // 
            // ArchiveImportForm
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(520, 302);
            this.Controls.Add(this.lblMessage);
            this.Controls.Add(this.pbProgress);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cbEthalons);
            this.Controls.Add(this.cbLogs);
            this.Controls.Add(this.cbSettings);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbControllerName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.ForeColor = System.Drawing.Color.Black;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ArchiveImportForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Создание архива данных контроллера";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ArchiveImportForm_FormClosing);
            this.Load += new System.EventHandler(this.ArchiveImportForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.TextBox tbControllerName;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.CheckBox cbSettings;
        public System.Windows.Forms.CheckBox cbLogs;
        public System.Windows.Forms.CheckBox cbEthalons;
        private System.Windows.Forms.Label label3;
        public System.Windows.Forms.ProgressBar pbProgress;
        public System.Windows.Forms.Label lblMessage;
        public System.Windows.Forms.Button btnCancel;
        public System.Windows.Forms.Button btnOK;
    }
}