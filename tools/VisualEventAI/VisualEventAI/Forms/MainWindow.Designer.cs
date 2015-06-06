namespace VisualEventAI.Forms
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this.tlp_window = new System.Windows.Forms.TableLayoutPanel();
            this.pnl_top = new System.Windows.Forms.Panel();
            this.btn_save = new System.Windows.Forms.Button();
            this.lbl_entry = new System.Windows.Forms.Label();
            this.tb_entry = new System.Windows.Forms.TextBox();
            this.btn_new = new System.Windows.Forms.Button();
            this.tc_content = new System.Windows.Forms.TabControl();
            this.btn_load = new System.Windows.Forms.Button();
            this.tlp_window.SuspendLayout();
            this.pnl_top.SuspendLayout();
            this.SuspendLayout();
            // 
            // tlp_window
            // 
            this.tlp_window.ColumnCount = 1;
            this.tlp_window.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tlp_window.Controls.Add(this.pnl_top, 0, 0);
            this.tlp_window.Controls.Add(this.tc_content, 0, 1);
            this.tlp_window.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tlp_window.Location = new System.Drawing.Point(0, 0);
            this.tlp_window.Name = "tlp_window";
            this.tlp_window.RowCount = 2;
            this.tlp_window.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tlp_window.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 356F));
            this.tlp_window.Size = new System.Drawing.Size(528, 462);
            this.tlp_window.TabIndex = 0;
            // 
            // pnl_top
            // 
            this.pnl_top.BackColor = System.Drawing.SystemColors.Control;
            this.pnl_top.Controls.Add(this.btn_load);
            this.pnl_top.Controls.Add(this.btn_save);
            this.pnl_top.Controls.Add(this.lbl_entry);
            this.pnl_top.Controls.Add(this.tb_entry);
            this.pnl_top.Controls.Add(this.btn_new);
            this.pnl_top.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnl_top.Location = new System.Drawing.Point(3, 3);
            this.pnl_top.Name = "pnl_top";
            this.pnl_top.Size = new System.Drawing.Size(522, 54);
            this.pnl_top.TabIndex = 0;
            // 
            // btn_save
            // 
            this.btn_save.Location = new System.Drawing.Point(443, 15);
            this.btn_save.Name = "btn_save";
            this.btn_save.Size = new System.Drawing.Size(75, 23);
            this.btn_save.TabIndex = 3;
            this.btn_save.Text = "&Save";
            this.btn_save.UseVisualStyleBackColor = true;
            this.btn_save.Click += new System.EventHandler(this.btn_save_Click);
            // 
            // lbl_entry
            // 
            this.lbl_entry.AutoSize = true;
            this.lbl_entry.Location = new System.Drawing.Point(119, 20);
            this.lbl_entry.Name = "lbl_entry";
            this.lbl_entry.Size = new System.Drawing.Size(34, 13);
            this.lbl_entry.TabIndex = 2;
            this.lbl_entry.Text = "Entry:";
            // 
            // tb_entry
            // 
            this.tb_entry.Location = new System.Drawing.Point(159, 17);
            this.tb_entry.Name = "tb_entry";
            this.tb_entry.Size = new System.Drawing.Size(100, 20);
            this.tb_entry.TabIndex = 1;
            // 
            // btn_new
            // 
            this.btn_new.Location = new System.Drawing.Point(9, 15);
            this.btn_new.Name = "btn_new";
            this.btn_new.Size = new System.Drawing.Size(75, 23);
            this.btn_new.TabIndex = 0;
            this.btn_new.Text = "&New";
            this.btn_new.UseVisualStyleBackColor = true;
            this.btn_new.Click += new System.EventHandler(this.btn_new_Click);
            // 
            // tc_content
            // 
            this.tc_content.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tc_content.Location = new System.Drawing.Point(3, 63);
            this.tc_content.Name = "tc_content";
            this.tc_content.SelectedIndex = 0;
            this.tc_content.Size = new System.Drawing.Size(522, 396);
            this.tc_content.TabIndex = 1;
            // 
            // btn_load
            // 
            this.btn_load.Location = new System.Drawing.Point(362, 15);
            this.btn_load.Name = "btn_load";
            this.btn_load.Size = new System.Drawing.Size(75, 23);
            this.btn_load.TabIndex = 4;
            this.btn_load.Text = "&Load";
            this.btn_load.UseVisualStyleBackColor = true;
            this.btn_load.Click += new System.EventHandler(this.btn_load_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(528, 462);
            this.Controls.Add(this.tlp_window);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(544, 500);
            this.MinimumSize = new System.Drawing.Size(544, 500);
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Visual EventAI";
            this.tlp_window.ResumeLayout(false);
            this.pnl_top.ResumeLayout(false);
            this.pnl_top.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tlp_window;
        private System.Windows.Forms.Panel pnl_top;
        private System.Windows.Forms.TabControl tc_content;
        private System.Windows.Forms.Button btn_new;
        private System.Windows.Forms.Button btn_save;
        private System.Windows.Forms.Label lbl_entry;
        private System.Windows.Forms.TextBox tb_entry;
        private System.Windows.Forms.Button btn_load;
    }
}

