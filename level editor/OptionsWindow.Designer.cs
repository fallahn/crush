namespace Level_editor
{
    partial class OptionsWindow
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
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxTexturePath = new System.Windows.Forms.TextBox();
            this.buttonBrowseTextures = new System.Windows.Forms.Button();
            this.buttonOk = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.toolTips = new System.Windows.Forms.ToolTip(this.components);
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxMapTexturePath = new System.Windows.Forms.TextBox();
            this.buttonMapTextures = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Detail Textures:";
            // 
            // textBoxTexturePath
            // 
            this.textBoxTexturePath.Location = new System.Drawing.Point(110, 11);
            this.textBoxTexturePath.Name = "textBoxTexturePath";
            this.textBoxTexturePath.Size = new System.Drawing.Size(227, 20);
            this.textBoxTexturePath.TabIndex = 1;
            // 
            // buttonBrowseTextures
            // 
            this.buttonBrowseTextures.Location = new System.Drawing.Point(343, 8);
            this.buttonBrowseTextures.Name = "buttonBrowseTextures";
            this.buttonBrowseTextures.Size = new System.Drawing.Size(75, 23);
            this.buttonBrowseTextures.TabIndex = 2;
            this.buttonBrowseTextures.Text = "Browse";
            this.buttonBrowseTextures.UseVisualStyleBackColor = true;
            this.buttonBrowseTextures.Click += new System.EventHandler(this.buttonBrowseTextures_Click);
            // 
            // buttonOk
            // 
            this.buttonOk.Location = new System.Drawing.Point(262, 225);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 3;
            this.buttonOk.Text = "OK";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(343, 225);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 4;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(75, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Map Textures:";
            // 
            // textBoxMapTexturePath
            // 
            this.textBoxMapTexturePath.Location = new System.Drawing.Point(110, 49);
            this.textBoxMapTexturePath.Name = "textBoxMapTexturePath";
            this.textBoxMapTexturePath.Size = new System.Drawing.Size(227, 20);
            this.textBoxMapTexturePath.TabIndex = 6;
            // 
            // buttonMapTextures
            // 
            this.buttonMapTextures.Location = new System.Drawing.Point(343, 47);
            this.buttonMapTextures.Name = "buttonMapTextures";
            this.buttonMapTextures.Size = new System.Drawing.Size(75, 23);
            this.buttonMapTextures.TabIndex = 7;
            this.buttonMapTextures.Text = "Browse";
            this.buttonMapTextures.UseVisualStyleBackColor = true;
            this.buttonMapTextures.Click += new System.EventHandler(this.buttonMapTextures_Click);
            // 
            // OptionsWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(432, 256);
            this.Controls.Add(this.buttonMapTextures);
            this.Controls.Add(this.textBoxMapTexturePath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOk);
            this.Controls.Add(this.buttonBrowseTextures);
            this.Controls.Add(this.textBoxTexturePath);
            this.Controls.Add(this.label1);
            this.MaximumSize = new System.Drawing.Size(448, 291);
            this.MinimumSize = new System.Drawing.Size(448, 291);
            this.Name = "OptionsWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Options";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxTexturePath;
        private System.Windows.Forms.Button buttonBrowseTextures;
        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.ToolTip toolTips;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxMapTexturePath;
        private System.Windows.Forms.Button buttonMapTextures;
    }
}