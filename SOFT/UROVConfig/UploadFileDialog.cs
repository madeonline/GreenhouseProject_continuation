using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UROVConfig
{
    public partial class UploadFileDialog : Form
    {
        public UploadFileDialog()
        {
            InitializeComponent();
        }

        private void UploadFileDialog_Load(object sender, EventArgs e)
        {
            this.cbRodMove.SelectedIndex = 0;
        }

        private void btnSelectFile_Click(object sender, EventArgs e)
        {
            if(openFileDialog.ShowDialog() == DialogResult.OK)
            {
                this.filePath.Text = openFileDialog.FileName;
            }
        }

        public string GetSelectedFileName()
        {
            return this.filePath.Text;
        }

        public string GetTargetFileName()
        {
            int channelNum = Convert.ToInt32(nudChannelNumber.Value);
            string rod = "UP";
            if (this.cbRodMove.SelectedIndex == 1)
                rod = "DWN";
            string result = "ETL|ET" + (channelNum - 1).ToString() + rod + ".ETL";

            return result;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (this.filePath.Text.Length < 1)
                return;

            DialogResult = DialogResult.OK;
        }
    }
}
