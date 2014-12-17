namespace Level_editor
{
    partial class MainWindow
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.panelEditorOuter = new System.Windows.Forms.Panel();
            this.panelEditorInner = new System.Windows.Forms.Panel();
            this.label10 = new System.Windows.Forms.Label();
            this.numericUpDownSnap = new System.Windows.Forms.NumericUpDown();
            this.checkBoxSnap = new System.Windows.Forms.CheckBox();
            this.buttonAddNode = new System.Windows.Forms.Button();
            this.comboBoxAddNode = new System.Windows.Forms.ComboBox();
            this.groupBoxNodeProperties = new System.Windows.Forms.GroupBox();
            this.buttonDeleteNode = new System.Windows.Forms.Button();
            this.numericUpDownNodePropertySizeY = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownNodePropertySizeX = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.numericUpDownNodePropertyPosY = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownNodePropertyPosX = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.comboBoxNodePropertyType = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBoxMapProperties = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.numericUpDownPlayerTwoY = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownPlayerTwoX = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.numericUpDownPlayerOneY = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownPlayerOneX = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.numericUpDownNpcTotal = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBoxNpc = new System.Windows.Forms.PictureBox();
            this.buttonNpcTexture = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownNpcCount = new System.Windows.Forms.NumericUpDown();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolTips = new System.Windows.Forms.ToolTip(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.panelEditorOuter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSnap)).BeginInit();
            this.groupBoxNodeProperties.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertySizeY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertySizeX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertyPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertyPosX)).BeginInit();
            this.groupBoxMapProperties.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerTwoY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerTwoX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerOneY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerOneX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNpcTotal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNpc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNpcCount)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.groupBoxNodeProperties);
            this.splitContainer1.Panel2.Controls.Add(this.groupBoxMapProperties);
            this.splitContainer1.Size = new System.Drawing.Size(1333, 673);
            this.splitContainer1.SplitterDistance = 1052;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.AutoScroll = true;
            this.splitContainer2.Panel1.Controls.Add(this.panelEditorOuter);
            this.splitContainer2.Panel1.Resize += new System.EventHandler(this.splitContainer2_Panel1_Resize);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.label10);
            this.splitContainer2.Panel2.Controls.Add(this.numericUpDownSnap);
            this.splitContainer2.Panel2.Controls.Add(this.checkBoxSnap);
            this.splitContainer2.Panel2.Controls.Add(this.buttonAddNode);
            this.splitContainer2.Panel2.Controls.Add(this.comboBoxAddNode);
            this.splitContainer2.Size = new System.Drawing.Size(1050, 671);
            this.splitContainer2.SplitterDistance = 566;
            this.splitContainer2.TabIndex = 0;
            // 
            // panelEditorOuter
            // 
            this.panelEditorOuter.BackColor = System.Drawing.Color.DarkGray;
            this.panelEditorOuter.Controls.Add(this.panelEditorInner);
            this.panelEditorOuter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEditorOuter.Location = new System.Drawing.Point(0, 0);
            this.panelEditorOuter.Name = "panelEditorOuter";
            this.panelEditorOuter.Size = new System.Drawing.Size(1046, 562);
            this.panelEditorOuter.TabIndex = 0;
            // 
            // panelEditorInner
            // 
            this.panelEditorInner.BackColor = System.Drawing.Color.Black;
            this.panelEditorInner.Location = new System.Drawing.Point(3, 3);
            this.panelEditorInner.Name = "panelEditorInner";
            this.panelEditorInner.Size = new System.Drawing.Size(960, 540);
            this.panelEditorInner.TabIndex = 0;
            this.panelEditorInner.Click += new System.EventHandler(this.panelEditorInner_Click);
            this.panelEditorInner.Paint += new System.Windows.Forms.PaintEventHandler(this.panelEditorInner_Paint);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(157, 50);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(31, 13);
            this.label10.TabIndex = 4;
            this.label10.Text = "Units";
            // 
            // numericUpDownSnap
            // 
            this.numericUpDownSnap.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownSnap.Location = new System.Drawing.Point(100, 48);
            this.numericUpDownSnap.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownSnap.Name = "numericUpDownSnap";
            this.numericUpDownSnap.Size = new System.Drawing.Size(51, 20);
            this.numericUpDownSnap.TabIndex = 3;
            this.numericUpDownSnap.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownSnap.ValueChanged += new System.EventHandler(this.numericUpDownSnap_ValueChanged);
            // 
            // checkBoxSnap
            // 
            this.checkBoxSnap.AutoSize = true;
            this.checkBoxSnap.Checked = true;
            this.checkBoxSnap.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxSnap.Location = new System.Drawing.Point(9, 49);
            this.checkBoxSnap.Name = "checkBoxSnap";
            this.checkBoxSnap.Size = new System.Drawing.Size(85, 17);
            this.checkBoxSnap.TabIndex = 2;
            this.checkBoxSnap.Text = "Snap to Grid";
            this.checkBoxSnap.UseVisualStyleBackColor = true;
            this.checkBoxSnap.CheckedChanged += new System.EventHandler(this.checkBoxSnap_CheckedChanged);
            // 
            // buttonAddNode
            // 
            this.buttonAddNode.Location = new System.Drawing.Point(136, 12);
            this.buttonAddNode.Name = "buttonAddNode";
            this.buttonAddNode.Size = new System.Drawing.Size(75, 23);
            this.buttonAddNode.TabIndex = 1;
            this.buttonAddNode.Text = "Add";
            this.buttonAddNode.UseVisualStyleBackColor = true;
            this.buttonAddNode.Click += new System.EventHandler(this.buttonAddNode_Click);
            // 
            // comboBoxAddNode
            // 
            this.comboBoxAddNode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAddNode.FormattingEnabled = true;
            this.comboBoxAddNode.Location = new System.Drawing.Point(9, 12);
            this.comboBoxAddNode.Name = "comboBoxAddNode";
            this.comboBoxAddNode.Size = new System.Drawing.Size(121, 21);
            this.comboBoxAddNode.TabIndex = 0;
            // 
            // groupBoxNodeProperties
            // 
            this.groupBoxNodeProperties.Controls.Add(this.buttonDeleteNode);
            this.groupBoxNodeProperties.Controls.Add(this.numericUpDownNodePropertySizeY);
            this.groupBoxNodeProperties.Controls.Add(this.numericUpDownNodePropertySizeX);
            this.groupBoxNodeProperties.Controls.Add(this.label6);
            this.groupBoxNodeProperties.Controls.Add(this.numericUpDownNodePropertyPosY);
            this.groupBoxNodeProperties.Controls.Add(this.numericUpDownNodePropertyPosX);
            this.groupBoxNodeProperties.Controls.Add(this.label5);
            this.groupBoxNodeProperties.Controls.Add(this.comboBoxNodePropertyType);
            this.groupBoxNodeProperties.Controls.Add(this.label4);
            this.groupBoxNodeProperties.Location = new System.Drawing.Point(3, 371);
            this.groupBoxNodeProperties.Name = "groupBoxNodeProperties";
            this.groupBoxNodeProperties.Size = new System.Drawing.Size(233, 212);
            this.groupBoxNodeProperties.TabIndex = 1;
            this.groupBoxNodeProperties.TabStop = false;
            this.groupBoxNodeProperties.Text = "Node Properties";
            // 
            // buttonDeleteNode
            // 
            this.buttonDeleteNode.Location = new System.Drawing.Point(134, 151);
            this.buttonDeleteNode.Name = "buttonDeleteNode";
            this.buttonDeleteNode.Size = new System.Drawing.Size(75, 23);
            this.buttonDeleteNode.TabIndex = 8;
            this.buttonDeleteNode.Text = "Delete";
            this.buttonDeleteNode.UseVisualStyleBackColor = true;
            this.buttonDeleteNode.Click += new System.EventHandler(this.buttonDeleteNode_Click);
            // 
            // numericUpDownNodePropertySizeY
            // 
            this.numericUpDownNodePropertySizeY.Location = new System.Drawing.Point(152, 119);
            this.numericUpDownNodePropertySizeY.Maximum = new decimal(new int[] {
            1080,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeY.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeY.Name = "numericUpDownNodePropertySizeY";
            this.numericUpDownNodePropertySizeY.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownNodePropertySizeY.TabIndex = 7;
            this.numericUpDownNodePropertySizeY.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeY.ValueChanged += new System.EventHandler(this.numericUpDownNodePropertySizeY_ValueChanged);
            // 
            // numericUpDownNodePropertySizeX
            // 
            this.numericUpDownNodePropertySizeX.Location = new System.Drawing.Point(89, 119);
            this.numericUpDownNodePropertySizeX.Maximum = new decimal(new int[] {
            1920,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeX.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeX.Name = "numericUpDownNodePropertySizeX";
            this.numericUpDownNodePropertySizeX.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownNodePropertySizeX.TabIndex = 6;
            this.numericUpDownNodePropertySizeX.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownNodePropertySizeX.ValueChanged += new System.EventHandler(this.numericUpDownNodePropertySizeX_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 121);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(30, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Size:";
            // 
            // numericUpDownNodePropertyPosY
            // 
            this.numericUpDownNodePropertyPosY.Location = new System.Drawing.Point(152, 90);
            this.numericUpDownNodePropertyPosY.Maximum = new decimal(new int[] {
            1070,
            0,
            0,
            0});
            this.numericUpDownNodePropertyPosY.Name = "numericUpDownNodePropertyPosY";
            this.numericUpDownNodePropertyPosY.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownNodePropertyPosY.TabIndex = 4;
            this.numericUpDownNodePropertyPosY.ValueChanged += new System.EventHandler(this.numericUpDownNodePropertyPosY_ValueChanged);
            // 
            // numericUpDownNodePropertyPosX
            // 
            this.numericUpDownNodePropertyPosX.Location = new System.Drawing.Point(89, 90);
            this.numericUpDownNodePropertyPosX.Maximum = new decimal(new int[] {
            1910,
            0,
            0,
            0});
            this.numericUpDownNodePropertyPosX.Name = "numericUpDownNodePropertyPosX";
            this.numericUpDownNodePropertyPosX.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownNodePropertyPosX.TabIndex = 3;
            this.numericUpDownNodePropertyPosX.ValueChanged += new System.EventHandler(this.numericUpDownNodePropertyPosX_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 92);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Position:";
            // 
            // comboBoxNodePropertyType
            // 
            this.comboBoxNodePropertyType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxNodePropertyType.FormattingEnabled = true;
            this.comboBoxNodePropertyType.Location = new System.Drawing.Point(90, 35);
            this.comboBoxNodePropertyType.Name = "comboBoxNodePropertyType";
            this.comboBoxNodePropertyType.Size = new System.Drawing.Size(119, 21);
            this.comboBoxNodePropertyType.TabIndex = 1;
            this.comboBoxNodePropertyType.SelectedIndexChanged += new System.EventHandler(this.comboBoxNodePropertyType_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 38);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Type:";
            // 
            // groupBoxMapProperties
            // 
            this.groupBoxMapProperties.Controls.Add(this.label9);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownPlayerTwoY);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownPlayerTwoX);
            this.groupBoxMapProperties.Controls.Add(this.label8);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownPlayerOneY);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownPlayerOneX);
            this.groupBoxMapProperties.Controls.Add(this.label7);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownNpcTotal);
            this.groupBoxMapProperties.Controls.Add(this.label3);
            this.groupBoxMapProperties.Controls.Add(this.pictureBoxNpc);
            this.groupBoxMapProperties.Controls.Add(this.buttonNpcTexture);
            this.groupBoxMapProperties.Controls.Add(this.label2);
            this.groupBoxMapProperties.Controls.Add(this.label1);
            this.groupBoxMapProperties.Controls.Add(this.numericUpDownNpcCount);
            this.groupBoxMapProperties.Location = new System.Drawing.Point(3, 8);
            this.groupBoxMapProperties.Name = "groupBoxMapProperties";
            this.groupBoxMapProperties.Size = new System.Drawing.Size(233, 357);
            this.groupBoxMapProperties.TabIndex = 0;
            this.groupBoxMapProperties.TabStop = false;
            this.groupBoxMapProperties.Text = "Map Properties";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(15, 267);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(85, 13);
            this.label9.TabIndex = 13;
            this.label9.Text = "Spawn Positions";
            // 
            // numericUpDownPlayerTwoY
            // 
            this.numericUpDownPlayerTwoY.Location = new System.Drawing.Point(151, 319);
            this.numericUpDownPlayerTwoY.Maximum = new decimal(new int[] {
            1030,
            0,
            0,
            0});
            this.numericUpDownPlayerTwoY.Name = "numericUpDownPlayerTwoY";
            this.numericUpDownPlayerTwoY.Size = new System.Drawing.Size(58, 20);
            this.numericUpDownPlayerTwoY.TabIndex = 12;
            this.numericUpDownPlayerTwoY.ValueChanged += new System.EventHandler(this.numericUpDownPlayerTwoY_ValueChanged);
            // 
            // numericUpDownPlayerTwoX
            // 
            this.numericUpDownPlayerTwoX.Location = new System.Drawing.Point(88, 319);
            this.numericUpDownPlayerTwoX.Maximum = new decimal(new int[] {
            1870,
            0,
            0,
            0});
            this.numericUpDownPlayerTwoX.Name = "numericUpDownPlayerTwoX";
            this.numericUpDownPlayerTwoX.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownPlayerTwoX.TabIndex = 11;
            this.numericUpDownPlayerTwoX.ValueChanged += new System.EventHandler(this.numericUpDownPlayerTwoX_ValueChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(13, 321);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(63, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Player Two:";
            // 
            // numericUpDownPlayerOneY
            // 
            this.numericUpDownPlayerOneY.Location = new System.Drawing.Point(151, 290);
            this.numericUpDownPlayerOneY.Maximum = new decimal(new int[] {
            1030,
            0,
            0,
            0});
            this.numericUpDownPlayerOneY.Name = "numericUpDownPlayerOneY";
            this.numericUpDownPlayerOneY.Size = new System.Drawing.Size(58, 20);
            this.numericUpDownPlayerOneY.TabIndex = 9;
            this.numericUpDownPlayerOneY.ValueChanged += new System.EventHandler(this.numericUpDownPlayerOneY_ValueChanged);
            // 
            // numericUpDownPlayerOneX
            // 
            this.numericUpDownPlayerOneX.Location = new System.Drawing.Point(88, 290);
            this.numericUpDownPlayerOneX.Maximum = new decimal(new int[] {
            1870,
            0,
            0,
            0});
            this.numericUpDownPlayerOneX.Name = "numericUpDownPlayerOneX";
            this.numericUpDownPlayerOneX.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownPlayerOneX.TabIndex = 8;
            this.numericUpDownPlayerOneX.ValueChanged += new System.EventHandler(this.numericUpDownPlayerOneX_ValueChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(13, 292);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(62, 13);
            this.label7.TabIndex = 7;
            this.label7.Text = "Player One:";
            // 
            // numericUpDownNpcTotal
            // 
            this.numericUpDownNpcTotal.Location = new System.Drawing.Point(81, 50);
            this.numericUpDownNpcTotal.Maximum = new decimal(new int[] {
            99,
            0,
            0,
            0});
            this.numericUpDownNpcTotal.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownNpcTotal.Name = "numericUpDownNpcTotal";
            this.numericUpDownNpcTotal.Size = new System.Drawing.Size(47, 20);
            this.numericUpDownNpcTotal.TabIndex = 6;
            this.numericUpDownNpcTotal.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownNpcTotal.ValueChanged += new System.EventHandler(this.numericUpDownNpcTotal_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "NPC Total:";
            // 
            // pictureBoxNpc
            // 
            this.pictureBoxNpc.BackColor = System.Drawing.SystemColors.ControlDark;
            this.pictureBoxNpc.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBoxNpc.Location = new System.Drawing.Point(18, 125);
            this.pictureBoxNpc.Name = "pictureBoxNpc";
            this.pictureBoxNpc.Size = new System.Drawing.Size(191, 117);
            this.pictureBoxNpc.TabIndex = 4;
            this.pictureBoxNpc.TabStop = false;
            // 
            // buttonNpcTexture
            // 
            this.buttonNpcTexture.Location = new System.Drawing.Point(134, 96);
            this.buttonNpcTexture.Name = "buttonNpcTexture";
            this.buttonNpcTexture.Size = new System.Drawing.Size(75, 23);
            this.buttonNpcTexture.TabIndex = 3;
            this.buttonNpcTexture.Text = "Browse";
            this.buttonNpcTexture.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 101);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "NPC Texture:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "NPC Count:";
            // 
            // numericUpDownNpcCount
            // 
            this.numericUpDownNpcCount.Location = new System.Drawing.Point(81, 24);
            this.numericUpDownNpcCount.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.numericUpDownNpcCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownNpcCount.Name = "numericUpDownNpcCount";
            this.numericUpDownNpcCount.Size = new System.Drawing.Size(47, 20);
            this.numericUpDownNpcCount.TabIndex = 0;
            this.numericUpDownNpcCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownNpcCount.ValueChanged += new System.EventHandler(this.numericUpDownNpcCount_ValueChanged);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1333, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1333, 697);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "Level Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainWindow_FormClosing);
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this.Resize += new System.EventHandler(this.MainWindow_Resize);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.panelEditorOuter.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSnap)).EndInit();
            this.groupBoxNodeProperties.ResumeLayout(false);
            this.groupBoxNodeProperties.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertySizeY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertySizeX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertyPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNodePropertyPosX)).EndInit();
            this.groupBoxMapProperties.ResumeLayout(false);
            this.groupBoxMapProperties.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerTwoY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerTwoX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerOneY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayerOneX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNpcTotal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNpc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownNpcCount)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Panel panelEditorOuter;
        private System.Windows.Forms.Panel panelEditorInner;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBoxNodeProperties;
        private System.Windows.Forms.GroupBox groupBoxMapProperties;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownNpcCount;
        private System.Windows.Forms.PictureBox pictureBoxNpc;
        private System.Windows.Forms.Button buttonNpcTexture;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numericUpDownNpcTotal;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ToolTip toolTips;
        private System.Windows.Forms.NumericUpDown numericUpDownNodePropertySizeY;
        private System.Windows.Forms.NumericUpDown numericUpDownNodePropertySizeX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown numericUpDownNodePropertyPosY;
        private System.Windows.Forms.NumericUpDown numericUpDownNodePropertyPosX;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBoxNodePropertyType;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numericUpDownPlayerOneY;
        private System.Windows.Forms.NumericUpDown numericUpDownPlayerOneX;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown numericUpDownPlayerTwoY;
        private System.Windows.Forms.NumericUpDown numericUpDownPlayerTwoX;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button buttonDeleteNode;
        private System.Windows.Forms.Button buttonAddNode;
        private System.Windows.Forms.ComboBox comboBoxAddNode;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown numericUpDownSnap;
        private System.Windows.Forms.CheckBox checkBoxSnap;
    }
}

