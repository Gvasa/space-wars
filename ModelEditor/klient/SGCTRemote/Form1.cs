using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;

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
        private Data _data;

        private clientData c;

        public Form1()
        {
            _data = new Data();
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
            btnAddModel.Enabled = false;
            lbModels.Enabled = false;
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
            gbObject.Enabled = status;
            
        }

        #region Network

        private void connect()
        {
            //if connection successfull
            if (c.connection.ConnectIP(c.ip, c.port, c.bufferSize))
            {
                //componentVisibility(true);
                btnAddModel.Enabled = true;
                lbModels.Enabled = true;
                this.connectButton.Text = "Disconnect";
                this.toolStripStatusLabel1.Text = "Connected";
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

        private void lbModels_SelectedIndexChanged(object sender, EventArgs e)
        {
            string curItem = lbModels.SelectedItem.ToString();
            int index = getSelectedModelIndex();
            if (index != -1 && c.connection.valid)
            {
                c.connection.Send("selectedModel=" + index);
            }

            updateAllValues();

        }

        private void btnAddModel_Click(object sender, EventArgs e)
        {
            DialogResult result = ofdOpenModel.ShowDialog();
            if (result == DialogResult.OK)
            {
                componentVisibility(true);
                string filename = ofdOpenModel.FileName;

                _data._models.Add(new Model());
                _data._models.ElementAt(_data._models.Count-1).setName("Model" + _data._models.Count);
                _data._models.ElementAt(_data._models.Count-1).setFileName(filename);
                lbModels.Items.Add(_data._models.ElementAt(_data._models.Count-1).getName());

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
            double tmp = (Convert.ToDouble(tbXpos.Text) + Convert.ToDouble(e.Delta));
            tbXpos.Text = tmp.ToString();
        }

        private void tbYpos_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbYpos.Text) + Convert.ToDouble(e.Delta);
            tbYpos.Text = tmp.ToString();
        }

        private void tbZpos_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbZpos.Text) + Convert.ToDouble(e.Delta);
            tbZpos.Text = tmp.ToString();
        }

        private void tbXrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbXrot.Text) + Convert.ToDouble(e.Delta);
            tbXrot.Text = tmp.ToString();
        }

        private void tbYrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbYrot.Text) + Convert.ToDouble(e.Delta);
            tbYrot.Text = tmp.ToString();
        }

        private void tbZrot_MouseScroll(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            double tmp = Convert.ToDouble(tbZrot.Text) + Convert.ToDouble(e.Delta);
            tbZrot.Text = tmp.ToString();
        }

        private void sendRotation(int axis, double value)
        {
            if (c.connection.valid)
            {
                if (axis == 1)
                {
                    c.connection.Send("rotationX=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setXrot(value);

                }
                else if (axis == 2)
                {

                    c.connection.Send("rotationY=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setYrot(value);
                }
                else if (axis == 3)
                {
                    c.connection.Send("rotationZ=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setZrot(value);
                }
            }
        }

        private void sendPosition(int axis, double value)
        {
            if (c.connection.valid)
            {
                if (axis == 1)
                {
                    c.connection.Send("positionX=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setXpos(value);
                }
                else if (axis == 2)
                {
                    c.connection.Send("positionY=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setYpos(value);
                }
                else if (axis == 3)
                {
                    c.connection.Send("positionZ=" + value);
                    _data._models.ElementAt(getSelectedModelIndex()).setZpos(value);
                }
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
                sendRotation(1, value);
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
                sendRotation(2, value);
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
                sendRotation(3, value);
            }
            catch (FormatException)
            {
            }
            catch (OverflowException)
            {
            }
        }

        private void tbName_TextChanged(object sender, EventArgs e)
        {
            Console.WriteLine(tbName.Text);
            _data._models.ElementAt(getSelectedModelIndex()).setName(tbName.Text);
            lbModels.SelectedItem = tbName.Text;

        }

        private int getSelectedModelIndex()
        {
            string curItem = lbModels.SelectedItem.ToString();
            return lbModels.FindString(curItem);
        }

        private void updateAllValues()
        {
            tbName.Text = _data._models.ElementAt(getSelectedModelIndex()).getName();
            tbXpos.Text = _data._models.ElementAt(getSelectedModelIndex()).getXpos().ToString();
            tbYpos.Text = _data._models.ElementAt(getSelectedModelIndex()).getYpos().ToString();
            tbZpos.Text = _data._models.ElementAt(getSelectedModelIndex()).getZpos().ToString();
            tbXrot.Text = _data._models.ElementAt(getSelectedModelIndex()).getXrot().ToString();
            tbYrot.Text = _data._models.ElementAt(getSelectedModelIndex()).getYrot().ToString();
            tbZrot.Text = _data._models.ElementAt(getSelectedModelIndex()).getZrot().ToString();
            cbBase.Checked = _data._models.ElementAt(getSelectedModelIndex()).getIsBase();
            cbCanon.Checked = _data._models.ElementAt(getSelectedModelIndex()).getIsCanon();
            
        }

        private void menuSave_Click(object sender, EventArgs e)
        {
            DialogResult result = sfdSaveJson.ShowDialog();
            if (result == DialogResult.OK)
            {
                string output = JsonConvert.SerializeObject(_data, Formatting.Indented);
                System.IO.File.WriteAllText(sfdSaveJson.FileName, output);
            } 
        }

        private void tbObjectName_TextChanged(object sender, EventArgs e)
        {
            _data.name = tbObjectName.Text;
        }

        private void tbHp_TextChanged(object sender, EventArgs e)
        {
            _data.hp = Convert.ToInt32(tbHp.Text);
        }

        private void cbBase_CheckedChanged(object sender, EventArgs e)
        {
            _data._models.ElementAt(getSelectedModelIndex()).setIsBase(cbBase.Checked);
        }

        private void cbCanon_CheckedChanged(object sender, EventArgs e)
        {
            _data._models.ElementAt(getSelectedModelIndex()).setIsCanon(cbCanon.Checked);
        }

        
        

        

    }
}
