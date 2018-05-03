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
    public partial class ControllerNameForm : Form
    {
        public ControllerNameForm()
        {
            InitializeComponent();
        }

        private void ControllerNameForm_Load(object sender, EventArgs e)
        {
            if(ControllerNames.Instance.Names.ContainsKey(Config.Instance.ControllerGUID))
            {
                tbControllerName.Text = ControllerNames.Instance.Names[Config.Instance.ControllerGUID];
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (tbControllerName.Text.Trim().Length < 1)
                return;

            if (Config.Instance.ControllerGUID.Length > 0)
            {
                ControllerNames.Instance.Names[Config.Instance.ControllerGUID] = tbControllerName.Text.Trim();
                ControllerNames.Instance.Save();
            }

            DialogResult = DialogResult.OK;
        }
    }
}
