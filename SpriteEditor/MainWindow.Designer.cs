namespace SpriteEditor
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openJSONToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.label1 = new System.Windows.Forms.Label();
            this.listBoxAnimations = new System.Windows.Forms.ListBox();
            this.numericUpDownAnimStart = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownAnimEnd = new System.Windows.Forms.NumericUpDown();
            this.checkBoxLoop = new System.Windows.Forms.CheckBox();
            this.textBoxAnimName = new System.Windows.Forms.TextBox();
            this.labelAnimName = new System.Windows.Forms.Label();
            this.buttonAddAnim = new System.Windows.Forms.Button();
            this.buttonRemoveAnim = new System.Windows.Forms.Button();
            this.buttonPrev = new System.Windows.Forms.Button();
            this.buttonPlayPause = new System.Windows.Forms.Button();
            this.buttonNext = new System.Windows.Forms.Button();
            this.numericUpDownFrameWidth = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownFrameHeight = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownFrameCount = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownFrameRate = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnimStart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnimEnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameRate)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(669, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openImageToolStripMenuItem,
            this.openJSONToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openImageToolStripMenuItem
            // 
            this.openImageToolStripMenuItem.Name = "openImageToolStripMenuItem";
            this.openImageToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.I)));
            this.openImageToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.openImageToolStripMenuItem.Text = "Open Image";
            this.openImageToolStripMenuItem.Click += new System.EventHandler(this.openImageToolStripMenuItem_Click);
            // 
            // openJSONToolStripMenuItem
            // 
            this.openJSONToolStripMenuItem.Name = "openJSONToolStripMenuItem";
            this.openJSONToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openJSONToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.openJSONToolStripMenuItem.Text = "Open JSON";
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.W)));
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.closeToolStripMenuItem.Text = "Close";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            // 
            // splitContainer1
            // 
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
            this.splitContainer1.Panel2.Controls.Add(this.label3);
            this.splitContainer1.Panel2.Controls.Add(this.label2);
            this.splitContainer1.Panel2.Controls.Add(this.buttonRemoveAnim);
            this.splitContainer1.Panel2.Controls.Add(this.buttonAddAnim);
            this.splitContainer1.Panel2.Controls.Add(this.labelAnimName);
            this.splitContainer1.Panel2.Controls.Add(this.textBoxAnimName);
            this.splitContainer1.Panel2.Controls.Add(this.checkBoxLoop);
            this.splitContainer1.Panel2.Controls.Add(this.numericUpDownAnimEnd);
            this.splitContainer1.Panel2.Controls.Add(this.numericUpDownAnimStart);
            this.splitContainer1.Panel2.Controls.Add(this.listBoxAnimations);
            this.splitContainer1.Panel2.Controls.Add(this.label1);
            this.splitContainer1.Size = new System.Drawing.Size(669, 413);
            this.splitContainer1.SplitterDistance = 360;
            this.splitContainer1.TabIndex = 1;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.label7);
            this.splitContainer2.Panel2.Controls.Add(this.label6);
            this.splitContainer2.Panel2.Controls.Add(this.label5);
            this.splitContainer2.Panel2.Controls.Add(this.label4);
            this.splitContainer2.Panel2.Controls.Add(this.numericUpDownFrameRate);
            this.splitContainer2.Panel2.Controls.Add(this.numericUpDownFrameCount);
            this.splitContainer2.Panel2.Controls.Add(this.numericUpDownFrameHeight);
            this.splitContainer2.Panel2.Controls.Add(this.numericUpDownFrameWidth);
            this.splitContainer2.Panel2.Controls.Add(this.buttonNext);
            this.splitContainer2.Panel2.Controls.Add(this.buttonPlayPause);
            this.splitContainer2.Panel2.Controls.Add(this.buttonPrev);
            this.splitContainer2.Size = new System.Drawing.Size(360, 413);
            this.splitContainer2.SplitterDistance = 276;
            this.splitContainer2.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Animations:";
            // 
            // listBoxAnimations
            // 
            this.listBoxAnimations.FormattingEnabled = true;
            this.listBoxAnimations.Location = new System.Drawing.Point(19, 27);
            this.listBoxAnimations.Name = "listBoxAnimations";
            this.listBoxAnimations.Size = new System.Drawing.Size(274, 264);
            this.listBoxAnimations.TabIndex = 1;
            // 
            // numericUpDownAnimStart
            // 
            this.numericUpDownAnimStart.Location = new System.Drawing.Point(60, 296);
            this.numericUpDownAnimStart.Name = "numericUpDownAnimStart";
            this.numericUpDownAnimStart.Size = new System.Drawing.Size(50, 20);
            this.numericUpDownAnimStart.TabIndex = 2;
            // 
            // numericUpDownAnimEnd
            // 
            this.numericUpDownAnimEnd.Location = new System.Drawing.Point(168, 296);
            this.numericUpDownAnimEnd.Name = "numericUpDownAnimEnd";
            this.numericUpDownAnimEnd.Size = new System.Drawing.Size(49, 20);
            this.numericUpDownAnimEnd.TabIndex = 3;
            // 
            // checkBoxLoop
            // 
            this.checkBoxLoop.AutoSize = true;
            this.checkBoxLoop.Location = new System.Drawing.Point(232, 297);
            this.checkBoxLoop.Name = "checkBoxLoop";
            this.checkBoxLoop.Size = new System.Drawing.Size(50, 17);
            this.checkBoxLoop.TabIndex = 4;
            this.checkBoxLoop.Text = "Loop";
            this.checkBoxLoop.UseVisualStyleBackColor = true;
            // 
            // textBoxAnimName
            // 
            this.textBoxAnimName.Location = new System.Drawing.Point(66, 337);
            this.textBoxAnimName.Name = "textBoxAnimName";
            this.textBoxAnimName.Size = new System.Drawing.Size(227, 20);
            this.textBoxAnimName.TabIndex = 5;
            // 
            // labelAnimName
            // 
            this.labelAnimName.AutoSize = true;
            this.labelAnimName.Location = new System.Drawing.Point(22, 340);
            this.labelAnimName.Name = "labelAnimName";
            this.labelAnimName.Size = new System.Drawing.Size(38, 13);
            this.labelAnimName.TabIndex = 6;
            this.labelAnimName.Text = "Name:";
            // 
            // buttonAddAnim
            // 
            this.buttonAddAnim.Location = new System.Drawing.Point(136, 378);
            this.buttonAddAnim.Name = "buttonAddAnim";
            this.buttonAddAnim.Size = new System.Drawing.Size(75, 23);
            this.buttonAddAnim.TabIndex = 7;
            this.buttonAddAnim.Text = "Add";
            this.buttonAddAnim.UseVisualStyleBackColor = true;
            // 
            // buttonRemoveAnim
            // 
            this.buttonRemoveAnim.Location = new System.Drawing.Point(218, 378);
            this.buttonRemoveAnim.Name = "buttonRemoveAnim";
            this.buttonRemoveAnim.Size = new System.Drawing.Size(75, 23);
            this.buttonRemoveAnim.TabIndex = 8;
            this.buttonRemoveAnim.Text = "Remove";
            this.buttonRemoveAnim.UseVisualStyleBackColor = true;
            // 
            // buttonPrev
            // 
            this.buttonPrev.Location = new System.Drawing.Point(12, 12);
            this.buttonPrev.Name = "buttonPrev";
            this.buttonPrev.Size = new System.Drawing.Size(38, 23);
            this.buttonPrev.TabIndex = 0;
            this.buttonPrev.Text = "<";
            this.buttonPrev.UseVisualStyleBackColor = true;
            // 
            // buttonPlayPause
            // 
            this.buttonPlayPause.Location = new System.Drawing.Point(56, 12);
            this.buttonPlayPause.Name = "buttonPlayPause";
            this.buttonPlayPause.Size = new System.Drawing.Size(75, 23);
            this.buttonPlayPause.TabIndex = 1;
            this.buttonPlayPause.Text = "Play";
            this.buttonPlayPause.UseVisualStyleBackColor = true;
            this.buttonPlayPause.Click += new System.EventHandler(this.buttonPlayPause_Click);
            // 
            // buttonNext
            // 
            this.buttonNext.Location = new System.Drawing.Point(137, 12);
            this.buttonNext.Name = "buttonNext";
            this.buttonNext.Size = new System.Drawing.Size(37, 23);
            this.buttonNext.TabIndex = 2;
            this.buttonNext.Text = ">";
            this.buttonNext.UseVisualStyleBackColor = true;
            // 
            // numericUpDownFrameWidth
            // 
            this.numericUpDownFrameWidth.Location = new System.Drawing.Point(207, 15);
            this.numericUpDownFrameWidth.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numericUpDownFrameWidth.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownFrameWidth.Name = "numericUpDownFrameWidth";
            this.numericUpDownFrameWidth.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownFrameWidth.TabIndex = 3;
            this.numericUpDownFrameWidth.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownFrameWidth.ValueChanged += new System.EventHandler(this.numericUpDownFrameWidth_ValueChanged);
            // 
            // numericUpDownFrameHeight
            // 
            this.numericUpDownFrameHeight.Location = new System.Drawing.Point(285, 15);
            this.numericUpDownFrameHeight.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numericUpDownFrameHeight.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownFrameHeight.Name = "numericUpDownFrameHeight";
            this.numericUpDownFrameHeight.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownFrameHeight.TabIndex = 4;
            this.numericUpDownFrameHeight.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownFrameHeight.ValueChanged += new System.EventHandler(this.numericUpDownFrameHeight_ValueChanged);
            // 
            // numericUpDownFrameCount
            // 
            this.numericUpDownFrameCount.Location = new System.Drawing.Point(285, 53);
            this.numericUpDownFrameCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownFrameCount.Name = "numericUpDownFrameCount";
            this.numericUpDownFrameCount.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownFrameCount.TabIndex = 5;
            this.numericUpDownFrameCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownFrameCount.ValueChanged += new System.EventHandler(this.numericUpDownFrameCount_ValueChanged);
            // 
            // numericUpDownFrameRate
            // 
            this.numericUpDownFrameRate.DecimalPlaces = 2;
            this.numericUpDownFrameRate.Increment = new decimal(new int[] {
            5,
            0,
            0,
            65536});
            this.numericUpDownFrameRate.Location = new System.Drawing.Point(285, 88);
            this.numericUpDownFrameRate.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownFrameRate.Name = "numericUpDownFrameRate";
            this.numericUpDownFrameRate.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownFrameRate.TabIndex = 6;
            this.numericUpDownFrameRate.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownFrameRate.ValueChanged += new System.EventHandler(this.numericUpDownFrameRate_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 299);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Start:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(133, 299);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "End:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(180, 19);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(21, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "W:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(266, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(18, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "H:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(241, 55);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Count:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(246, 90);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(33, 13);
            this.label7.TabIndex = 10;
            this.label7.Text = "Rate:";
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(669, 437);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(685, 472);
            this.MinimumSize = new System.Drawing.Size(685, 472);
            this.Name = "MainWindow";
            this.Text = "Sprite Editor";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnimStart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnimEnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFrameRate)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openJSONToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Button buttonRemoveAnim;
        private System.Windows.Forms.Button buttonAddAnim;
        private System.Windows.Forms.Label labelAnimName;
        private System.Windows.Forms.TextBox textBoxAnimName;
        private System.Windows.Forms.CheckBox checkBoxLoop;
        private System.Windows.Forms.NumericUpDown numericUpDownAnimEnd;
        private System.Windows.Forms.NumericUpDown numericUpDownAnimStart;
        private System.Windows.Forms.ListBox listBoxAnimations;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownFrameHeight;
        private System.Windows.Forms.NumericUpDown numericUpDownFrameWidth;
        private System.Windows.Forms.Button buttonNext;
        private System.Windows.Forms.Button buttonPlayPause;
        private System.Windows.Forms.Button buttonPrev;
        private System.Windows.Forms.NumericUpDown numericUpDownFrameRate;
        private System.Windows.Forms.NumericUpDown numericUpDownFrameCount;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
    }
}

