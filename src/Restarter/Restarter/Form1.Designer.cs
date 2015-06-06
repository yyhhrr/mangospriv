namespace Restarter
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btn_world = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.btn_kill = new System.Windows.Forms.Button();
            this.lbl_mangos = new System.Windows.Forms.CheckBox();
            this.lbl_realm = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // btn_world
            // 
            this.btn_world.BackColor = System.Drawing.Color.Green;
            this.btn_world.Location = new System.Drawing.Point(84, 87);
            this.btn_world.Name = "btn_world";
            this.btn_world.Size = new System.Drawing.Size(89, 23);
            this.btn_world.TabIndex = 0;
            this.btn_world.Text = "stop restarter";
            this.btn_world.UseVisualStyleBackColor = false;
            this.btn_world.Click += new System.EventHandler(this.btn_world_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // btn_kill
            // 
            this.btn_kill.Location = new System.Drawing.Point(216, 87);
            this.btn_kill.Name = "btn_kill";
            this.btn_kill.Size = new System.Drawing.Size(56, 23);
            this.btn_kill.TabIndex = 3;
            this.btn_kill.Text = "Kill";
            this.btn_kill.UseVisualStyleBackColor = true;
            this.btn_kill.Click += new System.EventHandler(this.btn_kill_Click);
            // 
            // lbl_mangos
            // 
            this.lbl_mangos.AutoSize = true;
            this.lbl_mangos.Location = new System.Drawing.Point(38, 24);
            this.lbl_mangos.Name = "lbl_mangos";
            this.lbl_mangos.Size = new System.Drawing.Size(51, 17);
            this.lbl_mangos.TabIndex = 4;
            this.lbl_mangos.Text = "world";
            this.lbl_mangos.UseVisualStyleBackColor = true;
            // 
            // lbl_realm
            // 
            this.lbl_realm.AutoSize = true;
            this.lbl_realm.Location = new System.Drawing.Point(169, 24);
            this.lbl_realm.Name = "lbl_realm";
            this.lbl_realm.Size = new System.Drawing.Size(51, 17);
            this.lbl_realm.TabIndex = 5;
            this.lbl_realm.Text = "realm";
            this.lbl_realm.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 122);
            this.Controls.Add(this.lbl_realm);
            this.Controls.Add(this.lbl_mangos);
            this.Controls.Add(this.btn_kill);
            this.Controls.Add(this.btn_world);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(300, 160);
            this.MinimumSize = new System.Drawing.Size(300, 160);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Restarter";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_world;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button btn_kill;
        private System.Windows.Forms.CheckBox lbl_mangos;
        private System.Windows.Forms.CheckBox lbl_realm;
    }
}

