namespace Level_editor
{
    partial class LightPropertiesWindow
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
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownAnchorOffset = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.panelColour = new System.Windows.Forms.Panel();
            this.buttonOk = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnchorOffset)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Anchor Offset:";
            // 
            // numericUpDownAnchorOffset
            // 
            this.numericUpDownAnchorOffset.Location = new System.Drawing.Point(144, 11);
            this.numericUpDownAnchorOffset.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.numericUpDownAnchorOffset.Name = "numericUpDownAnchorOffset";
            this.numericUpDownAnchorOffset.Size = new System.Drawing.Size(57, 20);
            this.numericUpDownAnchorOffset.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(40, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Colour:";
            // 
            // panelColour
            // 
            this.panelColour.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.panelColour.Location = new System.Drawing.Point(16, 68);
            this.panelColour.Name = "panelColour";
            this.panelColour.Size = new System.Drawing.Size(185, 72);
            this.panelColour.TabIndex = 3;
            this.panelColour.DoubleClick += new System.EventHandler(this.panelColour_DoubleClick);
            // 
            // buttonOk
            // 
            this.buttonOk.Location = new System.Drawing.Point(126, 160);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 4;
            this.buttonOk.Text = "OK";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // LightPropertiesWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(217, 195);
            this.Controls.Add(this.buttonOk);
            this.Controls.Add(this.panelColour);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.numericUpDownAnchorOffset);
            this.Controls.Add(this.label1);
            this.Name = "LightPropertiesWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Light Properties";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAnchorOffset)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownAnchorOffset;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panelColour;
        private System.Windows.Forms.Button buttonOk;
    }
}