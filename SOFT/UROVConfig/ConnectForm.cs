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
    public partial class ConnectForm : Form
    {
        private bool hasNoActions = false;
        public ConnectForm(bool noActions)
        {
            InitializeComponent();
            firstTimeActivate = true;
            hasNoActions = noActions;
        }

        private bool firstTimeActivate = true;
        private MainForm mainForm = null;
        private string portToConnect;

        private void ConnectForm_Activated(object sender, EventArgs e)
        {
            if (!firstTimeActivate)
                return;
            firstTimeActivate = false;

            if(!hasNoActions)
                this.mainForm.StartConnectToPort(this.portToConnect);
        }

        public void SetMainFormAndPort(MainForm fm, string port)
        {
            mainForm = fm;
            portToConnect = port;
        }
    }
}
