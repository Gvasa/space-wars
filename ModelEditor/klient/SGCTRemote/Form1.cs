using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SGCTRemote
{
    struct clientData
    {
        public NetworkManager connection;
        public string ip;
        public Int32 port;
        public int bufferSize;
    }
    
    public partial class Form1 : Form
    {
        private clientData c;

        public Form1()
        {
            InitializeComponent();
            init();
        }

        private void init()
        {
            c = new clientData();
            c.connection = new NetworkManager();
            c.port = 20500;
            c.ip = "127.0.0.1"; //default ip
            c.bufferSize = 1024;

            this.toolStripStatusLabel1.Text = "Disconnected";
            this.ipTextBox.Text = c.ip;

            componentVisibility(false);
        }

        private void exit()
        {
            disconnect();
            System.Environment.Exit(0);
        }

        /*
         * Enable or disable items depending on connection status
         */
        private void componentVisibility(bool status)
        {
            this.PropertiesGroupBox.Enabled = status;
            btnAddModel.Enabled = status;
            lstVModels.Enabled = status;
        }

        #region Network

        private void connect()
        {
            //if connection successfull
            if (c.connection.ConnectIP(c.ip, c.port, c.bufferSize))
            {
                componentVisibility(true);
                this.connectButton.Text = "Disconnect";
                this.toolStripStatusLabel1.Text = "Connected";
                
                //send defaults
                c.connection.Send("stats=0\r\ngraph=0\r\nwire=0\r\nsize=50");
            }
            else
            {
                componentVisibility(false);
                this.connectButton.Text = "Connect";
                this.toolStripStatusLabel1.Text = "Disconnected";
            }
        }

        private void disconnect()
        {
            componentVisibility(false);
            this.connectButton.Text = "Connect";
            this.toolStripStatusLabel1.Text = "Disconnected";

            if (c.connection != null)
            {
                c.connection.valid = false;
                c.connection.Disconnect();
            }
        }

        #endregion

        #region callbacks

        private void MainForm_Closed(object sender, System.EventArgs e)
        {
            exit();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            if (!c.connection.valid)
            {
                //get the ip address string from the textbox 
                c.ip = this.ipTextBox.Text;

                connect();
            }
            else
            {
                disconnect();
            }
        }

        private void lstVModels_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btnAddModel_Click(object sender, EventArgs e)
        {
            DialogResult result = ofdOpenModel.ShowDialog();
            if (result == DialogResult.OK)
            {
                string filename = ofdOpenModel.FileName;
                var item = new ListViewItem(new[] { filename});
                lstVModels.Items.Add(item);

                if (c.connection.valid)
                {
                    c.connection.Send("model=" + filename);
                }
            }
       
        }
        #endregion

        private void PropertiesGroupBox_Enter(object sender, EventArgs e)
        {

        }

        private void tbXpos_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbXpos.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbXpos.Text = tmp.ToString();
        }

        private void tbYpos_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbYpos.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbYpos.Text = tmp.ToString();
        }

        private void tbZpos_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbZpos.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbZpos.Text = tmp.ToString();
        }

        private void tbXrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbXrot.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbXrot.Text = tmp.ToString();
        }

        private void tbYrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbYrot.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbYrot.Text = tmp.ToString();
        }

        private void tbZrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbZrot.Text) + Convert.ToDouble(e.Delta) / 200.0;
            tbZrot.Text = tmp.ToString();
        }

        private void sendRotation(int axis, double value)
        {
            if (c.connection.valid)
            {
                if (axis == 1)
                    c.connection.Send("rotationX=" + value);
                else if (axis == 2)
                    c.connection.Send("rotationY=" + value);
                else if (axis == 3)
                    c.connection.Send("rotationZ=" + value);
            }
        }

        private void sendPosition(int axis, double value)
        {
            if (c.connection.valid)
            {
                if (axis == 1)
                    c.connection.Send("positionX=" + value);
                else if (axis == 2)
                    c.connection.Send("positionY=" + value);
                else if (axis == 3)
                    c.connection.Send("positionZ=" + value);
            }
        }

        private void tbXpos_TextChanged(object sender, EventArgs e)
        {
            try {
                double value = Convert.ToDouble(tbXpos.Text);
                sendPosition(1, value);
            }   
                catch (FormatException) {
            }               
                catch (OverflowException) {
            }
        }

        private void tbYpos_TextChanged(object sender, EventArgs e)
        {
            try
            {
                double value = Convert.ToDouble(tbYpos.Text);
                sendPosition(2, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }

        private void tbZpos_TextChanged(object sender, EventArgs e)
        {
            try
            {
                double value = Convert.ToDouble(tbZpos.Text);
                sendPosition(3, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }

        private void tbXrot_TextChanged(object sender, EventArgs e)
        {
            try
            {
                double value = Convert.ToDouble(tbXrot.Text);
                sendPosition(1, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }

        private void tbYrot_TextChanged(object sender, EventArgs e)
        {
            try
            {
                double value = Convert.ToDouble(tbYrot.Text);
                sendPosition(2, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }

        private void tbZrot_TextChanged(object sender, EventArgs e)
        {
            try
            {
                double value = Convert.ToDouble(tbZrot.Text);
                sendPosition(3, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }



        

        

    }
}
