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
        private MainForm mainForm = null;

        public UploadFileDialog(MainForm f)
        {
            InitializeComponent();
            mainForm = f;
        }

        private void UploadFileDialog_Load(object sender, EventArgs e)
        {
            this.cbRodMove.SelectedIndex = 0;
            FillTreeView();
        }

        private void FillTreeView()
        {
            TreeNode srcNode = mainForm.treeView.Nodes[1];
            for(int i=0;i< srcNode.Nodes.Count;i++)
            {
                TreeNode archiveItem = srcNode.Nodes[i];
                if(archiveItem.Tag is ArchiveTreeRootItem)
                {
                    ProcessArchiveItem(archiveItem, archiveItem.Tag as ArchiveTreeRootItem);
                }
            }
        }

        private void ProcessArchiveItem(TreeNode node, ArchiveTreeRootItem archiveItem)
        {
            for(int i=0;i<node.Nodes.Count;i++)
            {
                TreeNode childItem = node.Nodes[i];
                if(childItem.Tag is ArchiveTreeEthalonItem)
                {
                    ProcessEthalons(node, archiveItem, childItem);
                }
            }
        }

        private void ProcessEthalons(TreeNode archiveNode, ArchiveTreeRootItem archiveItem, TreeNode ethalonsNode)
        {
            for(int i=0;i< ethalonsNode.Nodes.Count;i++)
            {
                TreeNode ethalonNode = ethalonsNode.Nodes[i];
                if(ethalonNode.Tag is ArchiveTreeEthalonItemRecord)
                {
                    AddEthalonToTreeView(archiveNode, archiveItem, ethalonNode);
                }
            }
        }

        private void AddEthalonToTreeView(TreeNode archiveNode, ArchiveTreeRootItem archiveItem, TreeNode ethalonNode)
        {
            TreeNode existingNode = null;
            for(int i=0;i<treeView.Nodes.Count;i++)
            {
                TreeNode n = treeView.Nodes[i];
                if(n.Tag is ArchiveTreeRootItem)
                {
                    ArchiveTreeRootItem atri = n.Tag as ArchiveTreeRootItem;
                    if(atri.GUID == archiveItem.GUID)
                    {
                        existingNode = n;
                        break;
                    }
                }
            }

            if(existingNode == null)
            {
                existingNode = treeView.Nodes.Add(archiveNode.Text);
                existingNode.Tag = archiveItem;
            }

            TreeNode child = existingNode.Nodes.Add(ethalonNode.Text);
            child.Tag = ethalonNode.Tag;
        }

        public string GetSelectedFileName()
        {
            if (treeView.SelectedNode == null)
                return "";

            if (!(treeView.SelectedNode.Tag is ArchiveTreeEthalonItemRecord))
                return "";

            ArchiveTreeEthalonItemRecord rec = treeView.SelectedNode.Tag as ArchiveTreeEthalonItemRecord;

            return rec.FileName;

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
            if (this.GetSelectedFileName().Length < 1)
                return;

            DialogResult = DialogResult.OK;
        }

        private void treeView_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if(e.Node != null)
            {
                if (e.Node.Level < 1)
                    e.Cancel = true;
            }
        }
    }
}
