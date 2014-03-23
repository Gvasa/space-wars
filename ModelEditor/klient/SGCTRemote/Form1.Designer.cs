namespace SGCTRemote
{
    partial class Form1
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
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.connectButton = new System.Windows.Forms.Button();
            this.ipTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.PropertiesGroupBox = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tbZrot = new System.Windows.Forms.TextBox();
            this.tbYrot = new System.Windows.Forms.TextBox();
            this.tbXrot = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.gbPosition = new System.Windows.Forms.GroupBox();
            this.tbZpos = new System.Windows.Forms.TextBox();
            this.tbYpos = new System.Windows.Forms.TextBox();
            this.tbXpos = new System.Windows.Forms.TextBox();
            this.lblZpos = new System.Windows.Forms.Label();
            this.lblYpos = new System.Windows.Forms.Label();
            this.lblXpos = new System.Windows.Forms.Label();
            this.lblName = new System.Windows.Forms.Label();
            this.tbName = new System.Windows.Forms.TextBox();
            this.lbModels = new System.Windows.Forms.ListBox();
            this.btnAddModel = new System.Windows.Forms.Button();
            this.ofdOpenModel = new System.Windows.Forms.OpenFileDialog();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.sfdSaveJson = new System.Windows.Forms.SaveFileDialog();
            this.printDocument1 = new System.Drawing.Printing.PrintDocument();
            this.gbObject = new System.Windows.Forms.GroupBox();
            this.cbBase = new System.Windows.Forms.CheckBox();
            this.cbCanon = new System.Windows.Forms.CheckBox();
            this.lblObjectName = new System.Windows.Forms.Label();
            this.tbObjectName = new System.Windows.Forms.TextBox();
            this.lblHp = new System.Windows.Forms.Label();
            this.tbHp = new System.Windows.Forms.TextBox();
            this.statusStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.PropertiesGroupBox.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.gbPosition.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.gbObject.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 533);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(626, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.connectButton);
            this.groupBox1.Controls.Add(this.ipTextBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(145, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(259, 48);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Network";
            // 
            // connectButton
            // 
            this.connectButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.connectButton.Location = new System.Drawing.Point(151, 17);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(102, 20);
            this.connectButton.TabIndex = 3;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // ipTextBox
            // 
            this.ipTextBox.Location = new System.Drawing.Point(57, 17);
            this.ipTextBox.MaxLength = 16;
            this.ipTextBox.Name = "ipTextBox";
            this.ipTextBox.Size = new System.Drawing.Size(88, 20);
            this.ipTextBox.TabIndex = 2;
            this.ipTextBox.Text = "127.0.0.1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(8, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Address:";
            // 
            // PropertiesGroupBox
            // 
            this.PropertiesGroupBox.Controls.Add(this.cbCanon);
            this.PropertiesGroupBox.Controls.Add(this.cbBase);
            this.PropertiesGroupBox.Controls.Add(this.groupBox2);
            this.PropertiesGroupBox.Controls.Add(this.gbPosition);
            this.PropertiesGroupBox.Controls.Add(this.lblName);
            this.PropertiesGroupBox.Controls.Add(this.tbName);
            this.PropertiesGroupBox.Location = new System.Drawing.Point(145, 210);
            this.PropertiesGroupBox.Name = "PropertiesGroupBox";
            this.PropertiesGroupBox.Size = new System.Drawing.Size(481, 310);
            this.PropertiesGroupBox.TabIndex = 2;
            this.PropertiesGroupBox.TabStop = false;
            this.PropertiesGroupBox.Text = "Model Properties";
            this.PropertiesGroupBox.Enter += new System.EventHandler(this.PropertiesGroupBox_Enter);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tbZrot);
            this.groupBox2.Controls.Add(this.tbYrot);
            this.groupBox2.Controls.Add(this.tbXrot);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.groupBox2.Location = new System.Drawing.Point(112, 54);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(92, 115);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Rotation";
            // 
            // tbZrot
            // 
            this.tbZrot.Location = new System.Drawing.Point(29, 75);
            this.tbZrot.Name = "tbZrot";
            this.tbZrot.Size = new System.Drawing.Size(48, 20);
            this.tbZrot.TabIndex = 8;
            this.tbZrot.Text = "0";
            this.tbZrot.TextChanged += new System.EventHandler(this.tbZrot_TextChanged);
            this.tbZrot.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbZrot_MouseScroll);
            // 
            // tbYrot
            // 
            this.tbYrot.Location = new System.Drawing.Point(29, 49);
            this.tbYrot.Name = "tbYrot";
            this.tbYrot.Size = new System.Drawing.Size(48, 20);
            this.tbYrot.TabIndex = 6;
            this.tbYrot.Text = "0";
            this.tbYrot.TextChanged += new System.EventHandler(this.tbYrot_TextChanged);
            this.tbYrot.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbYrot_MouseScroll);
            // 
            // tbXrot
            // 
            this.tbXrot.Location = new System.Drawing.Point(29, 23);
            this.tbXrot.Name = "tbXrot";
            this.tbXrot.Size = new System.Drawing.Size(48, 20);
            this.tbXrot.TabIndex = 4;
            this.tbXrot.Text = "0";
            this.tbXrot.TextChanged += new System.EventHandler(this.tbXrot_TextChanged);
            this.tbXrot.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbXrot_MouseScroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Z:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "Y:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 26);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "X:";
            // 
            // gbPosition
            // 
            this.gbPosition.Controls.Add(this.tbZpos);
            this.gbPosition.Controls.Add(this.tbYpos);
            this.gbPosition.Controls.Add(this.tbXpos);
            this.gbPosition.Controls.Add(this.lblZpos);
            this.gbPosition.Controls.Add(this.lblYpos);
            this.gbPosition.Controls.Add(this.lblXpos);
            this.gbPosition.Location = new System.Drawing.Point(14, 54);
            this.gbPosition.Name = "gbPosition";
            this.gbPosition.Size = new System.Drawing.Size(92, 115);
            this.gbPosition.TabIndex = 2;
            this.gbPosition.TabStop = false;
            this.gbPosition.Text = "Position";
            // 
            // tbZpos
            // 
            this.tbZpos.Location = new System.Drawing.Point(29, 75);
            this.tbZpos.Name = "tbZpos";
            this.tbZpos.Size = new System.Drawing.Size(48, 20);
            this.tbZpos.TabIndex = 8;
            this.tbZpos.Text = "0";
            this.tbZpos.TextChanged += new System.EventHandler(this.tbZpos_TextChanged);
            this.tbZpos.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbZpos_MouseScroll);
            // 
            // tbYpos
            // 
            this.tbYpos.Location = new System.Drawing.Point(29, 49);
            this.tbYpos.Name = "tbYpos";
            this.tbYpos.Size = new System.Drawing.Size(48, 20);
            this.tbYpos.TabIndex = 6;
            this.tbYpos.Text = "0";
            this.tbYpos.TextChanged += new System.EventHandler(this.tbYpos_TextChanged);
            this.tbYpos.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbYpos_MouseScroll);
            // 
            // tbXpos
            // 
            this.tbXpos.Location = new System.Drawing.Point(29, 23);
            this.tbXpos.Name = "tbXpos";
            this.tbXpos.Size = new System.Drawing.Size(48, 20);
            this.tbXpos.TabIndex = 4;
            this.tbXpos.Text = "0";
            this.tbXpos.TextChanged += new System.EventHandler(this.tbXpos_TextChanged);
            this.tbXpos.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.tbXpos_MouseScroll);
            // 
            // lblZpos
            // 
            this.lblZpos.AutoSize = true;
            this.lblZpos.Location = new System.Drawing.Point(6, 78);
            this.lblZpos.Name = "lblZpos";
            this.lblZpos.Size = new System.Drawing.Size(17, 13);
            this.lblZpos.TabIndex = 2;
            this.lblZpos.Text = "Z:";
            // 
            // lblYpos
            // 
            this.lblYpos.AutoSize = true;
            this.lblYpos.Location = new System.Drawing.Point(6, 52);
            this.lblYpos.Name = "lblYpos";
            this.lblYpos.Size = new System.Drawing.Size(17, 13);
            this.lblYpos.TabIndex = 1;
            this.lblYpos.Text = "Y:";
            // 
            // lblXpos
            // 
            this.lblXpos.AutoSize = true;
            this.lblXpos.Location = new System.Drawing.Point(6, 26);
            this.lblXpos.Name = "lblXpos";
            this.lblXpos.Size = new System.Drawing.Size(17, 13);
            this.lblXpos.TabIndex = 0;
            this.lblXpos.Text = "X:";
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Location = new System.Drawing.Point(11, 31);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(35, 13);
            this.lblName.TabIndex = 1;
            this.lblName.Text = "Name";
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(52, 28);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(100, 20);
            this.tbName.TabIndex = 0;
            this.tbName.TextChanged += new System.EventHandler(this.tbName_TextChanged);
            // 
            // lbModels
            // 
            this.lbModels.FormattingEnabled = true;
            this.lbModels.Location = new System.Drawing.Point(12, 61);
            this.lbModels.Name = "lbModels";
            this.lbModels.Size = new System.Drawing.Size(120, 459);
            this.lbModels.TabIndex = 4;
            this.lbModels.SelectedIndexChanged += new System.EventHandler(this.lbModels_SelectedIndexChanged);
            // 
            // btnAddModel
            // 
            this.btnAddModel.Location = new System.Drawing.Point(12, 32);
            this.btnAddModel.Name = "btnAddModel";
            this.btnAddModel.Size = new System.Drawing.Size(75, 23);
            this.btnAddModel.TabIndex = 4;
            this.btnAddModel.Text = "Add Model";
            this.btnAddModel.UseVisualStyleBackColor = true;
            this.btnAddModel.Click += new System.EventHandler(this.btnAddModel_Click);
            // 
            // ofdOpenModel
            // 
            this.ofdOpenModel.FileName = "openFileDialog1";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(626, 24);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menu";
            // 
            // menuFile
            // 
            this.menuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuSave});
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(37, 20);
            this.menuFile.Text = "File";
            // 
            // menuSave
            // 
            this.menuSave.Name = "menuSave";
            this.menuSave.Size = new System.Drawing.Size(152, 22);
            this.menuSave.Text = "Save";
            this.menuSave.Click += new System.EventHandler(this.menuSave_Click);
            // 
            // gbObject
            // 
            this.gbObject.Controls.Add(this.tbHp);
            this.gbObject.Controls.Add(this.lblHp);
            this.gbObject.Controls.Add(this.tbObjectName);
            this.gbObject.Controls.Add(this.lblObjectName);
            this.gbObject.Location = new System.Drawing.Point(145, 66);
            this.gbObject.Name = "gbObject";
            this.gbObject.Size = new System.Drawing.Size(469, 138);
            this.gbObject.TabIndex = 6;
            this.gbObject.TabStop = false;
            this.gbObject.Text = "Object Properties";
            // 
            // cbBase
            // 
            this.cbBase.AutoSize = true;
            this.cbBase.Location = new System.Drawing.Point(14, 176);
            this.cbBase.Name = "cbBase";
            this.cbBase.Size = new System.Drawing.Size(50, 17);
            this.cbBase.TabIndex = 4;
            this.cbBase.Text = "Base";
            this.cbBase.UseVisualStyleBackColor = true;
            this.cbBase.CheckedChanged += new System.EventHandler(this.cbBase_CheckedChanged);
            // 
            // cbCanon
            // 
            this.cbCanon.AutoSize = true;
            this.cbCanon.Location = new System.Drawing.Point(14, 199);
            this.cbCanon.Name = "cbCanon";
            this.cbCanon.Size = new System.Drawing.Size(57, 17);
            this.cbCanon.TabIndex = 5;
            this.cbCanon.Text = "Canon";
            this.cbCanon.UseVisualStyleBackColor = true;
            this.cbCanon.CheckedChanged += new System.EventHandler(this.cbCanon_CheckedChanged);
            // 
            // lblObjectName
            // 
            this.lblObjectName.AutoSize = true;
            this.lblObjectName.Location = new System.Drawing.Point(11, 20);
            this.lblObjectName.Name = "lblObjectName";
            this.lblObjectName.Size = new System.Drawing.Size(35, 13);
            this.lblObjectName.TabIndex = 0;
            this.lblObjectName.Text = "Name";
            // 
            // tbObjectName
            // 
            this.tbObjectName.Location = new System.Drawing.Point(52, 17);
            this.tbObjectName.Name = "tbObjectName";
            this.tbObjectName.Size = new System.Drawing.Size(100, 20);
            this.tbObjectName.TabIndex = 1;
            this.tbObjectName.TextChanged += new System.EventHandler(this.tbObjectName_TextChanged);
            // 
            // lblHp
            // 
            this.lblHp.AutoSize = true;
            this.lblHp.Location = new System.Drawing.Point(11, 43);
            this.lblHp.Name = "lblHp";
            this.lblHp.Size = new System.Drawing.Size(22, 13);
            this.lblHp.TabIndex = 2;
            this.lblHp.Text = "HP";
            // 
            // tbHp
            // 
            this.tbHp.Location = new System.Drawing.Point(52, 43);
            this.tbHp.Name = "tbHp";
            this.tbHp.Size = new System.Drawing.Size(39, 20);
            this.tbHp.TabIndex = 3;
            this.tbHp.TextChanged += new System.EventHandler(this.tbHp_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(626, 555);
            this.Controls.Add(this.gbObject);
            this.Controls.Add(this.lbModels);
            this.Controls.Add(this.btnAddModel);
            this.Controls.Add(this.PropertiesGroupBox);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Model Editor";
            this.Closed += new System.EventHandler(this.MainForm_Closed);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.PropertiesGroupBox.ResumeLayout(false);
            this.PropertiesGroupBox.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.gbPosition.ResumeLayout(false);
            this.gbPosition.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.gbObject.ResumeLayout(false);
            this.gbObject.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.TextBox ipTextBox;
        private System.Windows.Forms.GroupBox PropertiesGroupBox;
        private System.Windows.Forms.Button btnAddModel;
        private System.Windows.Forms.OpenFileDialog ofdOpenModel;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.GroupBox gbPosition;
        private System.Windows.Forms.Label lblXpos;
        private System.Windows.Forms.Label lblYpos;
        private System.Windows.Forms.Label lblZpos;
        private System.Windows.Forms.TextBox tbXpos;
        private System.Windows.Forms.TextBox tbZpos;
        private System.Windows.Forms.TextBox tbYpos;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbZrot;
        private System.Windows.Forms.TextBox tbYrot;
        private System.Windows.Forms.TextBox tbXrot;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListBox lbModels;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuFile;
        private System.Windows.Forms.ToolStripMenuItem menuSave;
        private System.Windows.Forms.SaveFileDialog sfdSaveJson;
        private System.Drawing.Printing.PrintDocument printDocument1;
        private System.Windows.Forms.GroupBox gbObject;
        private System.Windows.Forms.CheckBox cbCanon;
        private System.Windows.Forms.CheckBox cbBase;
        private System.Windows.Forms.TextBox tbHp;
        private System.Windows.Forms.Label lblHp;
        private System.Windows.Forms.TextBox tbObjectName;
        private System.Windows.Forms.Label lblObjectName;
    }
}

