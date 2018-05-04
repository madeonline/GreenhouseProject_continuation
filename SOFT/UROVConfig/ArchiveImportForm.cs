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
    public partial class ArchiveImportForm : Form
    {
        private MainForm parentForm = null;
        public ArchiveImportForm(MainForm parent)
        {
            InitializeComponent();

            parentForm = parent;
        }

        private void ArchiveImportForm_Load(object sender, EventArgs e)
        {
            string guid = Config.Instance.ControllerGUID;
            if (ControllerNames.Instance.Names.ContainsKey(guid))
                tbControllerName.Text = ControllerNames.Instance.Names[guid];
            else
                tbControllerName.Text = "";
        }

        private bool canClose = true;

        public void DisableControls()
        {
            btnOK.Enabled = false;
            btnCancel.Enabled = false;
            canClose = false;

        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if(tbControllerName.Text.Trim().Length < 1)
            {
                MessageBox.Show("Подалуйста,у кажите имя контроллера!");
                tbControllerName.Focus();
                return;
            }

            int countChecked = 0;
            if (this.cbEthalons.Checked)
                countChecked++;

            if (this.cbLogs.Checked)
                countChecked++;

            if (this.cbSettings.Checked)
                countChecked++;

            if(countChecked < 1)
            {
                MessageBox.Show("Пожалуйста, укажите хотя бы одну настройку для архивирования!");
                return;
            }

            parentForm.StartArchive(this);
        }

        public void Done()
        {
            canClose = true;
            DialogResult = DialogResult.OK;
        }

        private void ArchiveImportForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = !canClose;
        }
    }
}
