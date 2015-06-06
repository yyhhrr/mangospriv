namespace VisualEventAI.Forms
{
    partial class AiTab
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cob_type = new System.Windows.Forms.ComboBox();
            this.lbl_type = new System.Windows.Forms.Label();
            this.pnl_phase_placeholder = new System.Windows.Forms.Panel();
            this.lbl_phases = new System.Windows.Forms.Label();
            this.lbl_chance = new System.Windows.Forms.Label();
            this.tb_chance = new System.Windows.Forms.TextBox();
            this.clb_flags = new System.Windows.Forms.CheckedListBox();
            this.lbl_flags = new System.Windows.Forms.Label();
            this.pnl_type = new System.Windows.Forms.Panel();
            this.tb_param4 = new System.Windows.Forms.TextBox();
            this.lbl_param4 = new System.Windows.Forms.Label();
            this.tb_param3 = new System.Windows.Forms.TextBox();
            this.lbl_param3 = new System.Windows.Forms.Label();
            this.tb_param2 = new System.Windows.Forms.TextBox();
            this.lbl_param2 = new System.Windows.Forms.Label();
            this.tb_param1 = new System.Windows.Forms.TextBox();
            this.lbl_param1 = new System.Windows.Forms.Label();
            this.pnl_action1 = new System.Windows.Forms.Panel();
            this.lbl_action1 = new System.Windows.Forms.Label();
            this.cb_action1 = new System.Windows.Forms.ComboBox();
            this.tb_action1_param3 = new System.Windows.Forms.TextBox();
            this.lbl_action1_param3 = new System.Windows.Forms.Label();
            this.tb_action1_param2 = new System.Windows.Forms.TextBox();
            this.lbl_action1_param2 = new System.Windows.Forms.Label();
            this.tb_action1_param1 = new System.Windows.Forms.TextBox();
            this.lbl_action1_param1 = new System.Windows.Forms.Label();
            this.pnl_action2 = new System.Windows.Forms.Panel();
            this.lbl_action2 = new System.Windows.Forms.Label();
            this.cb_action2 = new System.Windows.Forms.ComboBox();
            this.tb_action2_param3 = new System.Windows.Forms.TextBox();
            this.lbl_action2_param3 = new System.Windows.Forms.Label();
            this.tb_action2_param2 = new System.Windows.Forms.TextBox();
            this.lbl_action2_param2 = new System.Windows.Forms.Label();
            this.tb_action2_param1 = new System.Windows.Forms.TextBox();
            this.lbl_action2_param1 = new System.Windows.Forms.Label();
            this.pnl_action3 = new System.Windows.Forms.Panel();
            this.lbl_action3 = new System.Windows.Forms.Label();
            this.cb_action3 = new System.Windows.Forms.ComboBox();
            this.tb_action3_param3 = new System.Windows.Forms.TextBox();
            this.lbl_action3_param3 = new System.Windows.Forms.Label();
            this.tb_action3_param2 = new System.Windows.Forms.TextBox();
            this.lbl_action3_param2 = new System.Windows.Forms.Label();
            this.tb_action3_param1 = new System.Windows.Forms.TextBox();
            this.lbl_action3_param1 = new System.Windows.Forms.Label();
            this.tb_comment = new System.Windows.Forms.TextBox();
            this.lbl_comment = new System.Windows.Forms.Label();
            this.pnl_type.SuspendLayout();
            this.pnl_action1.SuspendLayout();
            this.pnl_action2.SuspendLayout();
            this.pnl_action3.SuspendLayout();
            this.SuspendLayout();
            // 
            // cob_type
            // 
            this.cob_type.FormattingEnabled = true;
            this.cob_type.Items.AddRange(new object[] {
            "Timer",
            "Timer (ooc)",
            "Hp",
            "Mana",
            "Aggro",
            "Kill",
            "Death",
            "Evade",
            "Spell hit",
            "Range",
            "ooc los",
            "Spawned",
            "Target hp",
            "Target casting",
            "Friendly hp",
            "Friendly cc",
            "Friendly missing buff",
            "Summoned unit",
            "Target mana",
            "Quest accept",
            "Quest complete",
            "Reached home",
            "Receive emote",
            "Aura",
            "Target aura",
            "Summoned just died",
            "Summoned just despawned",
            "Missing aura",
            "Target missing aura",
            "Missing aura ooc"});
            this.cob_type.Location = new System.Drawing.Point(54, 11);
            this.cob_type.Name = "cob_type";
            this.cob_type.Size = new System.Drawing.Size(153, 21);
            this.cob_type.TabIndex = 0;
            this.cob_type.Text = "-=Select=-";
            this.cob_type.SelectedIndexChanged += new System.EventHandler(this.cob_type_SelectedIndexChanged);
            // 
            // lbl_type
            // 
            this.lbl_type.AutoSize = true;
            this.lbl_type.Location = new System.Drawing.Point(3, 14);
            this.lbl_type.Name = "lbl_type";
            this.lbl_type.Size = new System.Drawing.Size(34, 13);
            this.lbl_type.TabIndex = 1;
            this.lbl_type.Text = "Type:";
            // 
            // pnl_phase_placeholder
            // 
            this.pnl_phase_placeholder.Location = new System.Drawing.Point(54, 47);
            this.pnl_phase_placeholder.Name = "pnl_phase_placeholder";
            this.pnl_phase_placeholder.Size = new System.Drawing.Size(150, 93);
            this.pnl_phase_placeholder.TabIndex = 2;
            this.pnl_phase_placeholder.Visible = false;
            // 
            // lbl_phases
            // 
            this.lbl_phases.AutoSize = true;
            this.lbl_phases.Location = new System.Drawing.Point(3, 54);
            this.lbl_phases.Name = "lbl_phases";
            this.lbl_phases.Size = new System.Drawing.Size(45, 13);
            this.lbl_phases.TabIndex = 3;
            this.lbl_phases.Text = "Phases:";
            // 
            // lbl_chance
            // 
            this.lbl_chance.AutoSize = true;
            this.lbl_chance.Location = new System.Drawing.Point(3, 294);
            this.lbl_chance.Name = "lbl_chance";
            this.lbl_chance.Size = new System.Drawing.Size(47, 13);
            this.lbl_chance.TabIndex = 4;
            this.lbl_chance.Text = "Chance:";
            // 
            // tb_chance
            // 
            this.tb_chance.Location = new System.Drawing.Point(54, 291);
            this.tb_chance.Name = "tb_chance";
            this.tb_chance.Size = new System.Drawing.Size(150, 20);
            this.tb_chance.TabIndex = 5;
            this.tb_chance.Text = "100";
            // 
            // clb_flags
            // 
            this.clb_flags.FormattingEnabled = true;
            this.clb_flags.Items.AddRange(new object[] {
            "Repeatable",
            "Reserved 1",
            "Reserved 2",
            "Reserved 3",
            "Reserved 4",
            "Random action",
            "Reserved 6",
            "debug only"});
            this.clb_flags.Location = new System.Drawing.Point(54, 156);
            this.clb_flags.Name = "clb_flags";
            this.clb_flags.Size = new System.Drawing.Size(150, 124);
            this.clb_flags.TabIndex = 6;
            // 
            // lbl_flags
            // 
            this.lbl_flags.AutoSize = true;
            this.lbl_flags.Location = new System.Drawing.Point(3, 156);
            this.lbl_flags.Name = "lbl_flags";
            this.lbl_flags.Size = new System.Drawing.Size(35, 13);
            this.lbl_flags.TabIndex = 7;
            this.lbl_flags.Text = "Flags:";
            // 
            // pnl_type
            // 
            this.pnl_type.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnl_type.Controls.Add(this.tb_param4);
            this.pnl_type.Controls.Add(this.lbl_param4);
            this.pnl_type.Controls.Add(this.tb_param3);
            this.pnl_type.Controls.Add(this.lbl_param3);
            this.pnl_type.Controls.Add(this.tb_param2);
            this.pnl_type.Controls.Add(this.lbl_param2);
            this.pnl_type.Controls.Add(this.tb_param1);
            this.pnl_type.Controls.Add(this.lbl_param1);
            this.pnl_type.Location = new System.Drawing.Point(210, 11);
            this.pnl_type.Name = "pnl_type";
            this.pnl_type.Size = new System.Drawing.Size(301, 60);
            this.pnl_type.TabIndex = 16;
            // 
            // tb_param4
            // 
            this.tb_param4.Location = new System.Drawing.Point(222, 32);
            this.tb_param4.Name = "tb_param4";
            this.tb_param4.Size = new System.Drawing.Size(66, 20);
            this.tb_param4.TabIndex = 23;
            // 
            // lbl_param4
            // 
            this.lbl_param4.AutoSize = true;
            this.lbl_param4.Location = new System.Drawing.Point(152, 35);
            this.lbl_param4.Name = "lbl_param4";
            this.lbl_param4.Size = new System.Drawing.Size(46, 13);
            this.lbl_param4.TabIndex = 22;
            this.lbl_param4.Text = "Param4:";
            // 
            // tb_param3
            // 
            this.tb_param3.Location = new System.Drawing.Point(75, 32);
            this.tb_param3.Name = "tb_param3";
            this.tb_param3.Size = new System.Drawing.Size(66, 20);
            this.tb_param3.TabIndex = 21;
            // 
            // lbl_param3
            // 
            this.lbl_param3.AutoSize = true;
            this.lbl_param3.Location = new System.Drawing.Point(5, 35);
            this.lbl_param3.Name = "lbl_param3";
            this.lbl_param3.Size = new System.Drawing.Size(46, 13);
            this.lbl_param3.TabIndex = 20;
            this.lbl_param3.Text = "Param3:";
            // 
            // tb_param2
            // 
            this.tb_param2.Location = new System.Drawing.Point(222, 6);
            this.tb_param2.Name = "tb_param2";
            this.tb_param2.Size = new System.Drawing.Size(66, 20);
            this.tb_param2.TabIndex = 19;
            // 
            // lbl_param2
            // 
            this.lbl_param2.AutoSize = true;
            this.lbl_param2.Location = new System.Drawing.Point(152, 9);
            this.lbl_param2.Name = "lbl_param2";
            this.lbl_param2.Size = new System.Drawing.Size(46, 13);
            this.lbl_param2.TabIndex = 18;
            this.lbl_param2.Text = "Param2:";
            // 
            // tb_param1
            // 
            this.tb_param1.Location = new System.Drawing.Point(75, 6);
            this.tb_param1.Name = "tb_param1";
            this.tb_param1.Size = new System.Drawing.Size(66, 20);
            this.tb_param1.TabIndex = 17;
            // 
            // lbl_param1
            // 
            this.lbl_param1.AutoSize = true;
            this.lbl_param1.Location = new System.Drawing.Point(5, 9);
            this.lbl_param1.Name = "lbl_param1";
            this.lbl_param1.Size = new System.Drawing.Size(46, 13);
            this.lbl_param1.TabIndex = 16;
            this.lbl_param1.Text = "Param1:";
            // 
            // pnl_action1
            // 
            this.pnl_action1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnl_action1.Controls.Add(this.lbl_action1);
            this.pnl_action1.Controls.Add(this.cb_action1);
            this.pnl_action1.Controls.Add(this.tb_action1_param3);
            this.pnl_action1.Controls.Add(this.lbl_action1_param3);
            this.pnl_action1.Controls.Add(this.tb_action1_param2);
            this.pnl_action1.Controls.Add(this.lbl_action1_param2);
            this.pnl_action1.Controls.Add(this.tb_action1_param1);
            this.pnl_action1.Controls.Add(this.lbl_action1_param1);
            this.pnl_action1.Location = new System.Drawing.Point(211, 96);
            this.pnl_action1.Name = "pnl_action1";
            this.pnl_action1.Size = new System.Drawing.Size(301, 86);
            this.pnl_action1.TabIndex = 24;
            // 
            // lbl_action1
            // 
            this.lbl_action1.AutoSize = true;
            this.lbl_action1.Location = new System.Drawing.Point(54, 6);
            this.lbl_action1.Name = "lbl_action1";
            this.lbl_action1.Size = new System.Drawing.Size(49, 13);
            this.lbl_action1.TabIndex = 25;
            this.lbl_action1.Text = "Action 1:";
            // 
            // cb_action1
            // 
            this.cb_action1.FormattingEnabled = true;
            this.cb_action1.Items.AddRange(new object[] {
            "None",
            "Text",
            "Set faction",
            "Morph to entry or model",
            "Sound",
            "Emote",
            "Random say",
            "Random yell",
            "Random textemote",
            "Random sound",
            "Random emote",
            "Cast",
            "Summon",
            "Threat single pct",
            "Threat all pct",
            "Quest event",
            "Cast event",
            "Set unit field",
            "Set unit flag",
            "Remove unit flag",
            "Auto attack",
            "Combat Movement",
            "Set phase",
            "Inc phase",
            "Evade",
            "Flee for assist",
            "Quest event all",
            "Cast event all",
            "Remove auras from spell",
            "Ranged movement",
            "Random phase",
            "Random phase range",
            "Summon Id",
            "Killed monster",
            "Set inst data",
            "Set inst data64",
            "Update template",
            "Die",
            "Zone combat pulse",
            "Call for help",
            "Sheath",
            "Force Despawn",
            "Set invincibility hp level",
            "Mount entry or model"});
            this.cb_action1.Location = new System.Drawing.Point(125, 3);
            this.cb_action1.Name = "cb_action1";
            this.cb_action1.Size = new System.Drawing.Size(162, 21);
            this.cb_action1.TabIndex = 24;
            this.cb_action1.Text = "None";
            this.cb_action1.SelectedIndexChanged += new System.EventHandler(this.cb_action_SelectedIndexChanged);
            // 
            // tb_action1_param3
            // 
            this.tb_action1_param3.Location = new System.Drawing.Point(75, 58);
            this.tb_action1_param3.Name = "tb_action1_param3";
            this.tb_action1_param3.Size = new System.Drawing.Size(66, 20);
            this.tb_action1_param3.TabIndex = 21;
            // 
            // lbl_action1_param3
            // 
            this.lbl_action1_param3.AutoSize = true;
            this.lbl_action1_param3.Location = new System.Drawing.Point(5, 61);
            this.lbl_action1_param3.Name = "lbl_action1_param3";
            this.lbl_action1_param3.Size = new System.Drawing.Size(46, 13);
            this.lbl_action1_param3.TabIndex = 20;
            this.lbl_action1_param3.Text = "Param3:";
            // 
            // tb_action1_param2
            // 
            this.tb_action1_param2.Location = new System.Drawing.Point(222, 32);
            this.tb_action1_param2.Name = "tb_action1_param2";
            this.tb_action1_param2.Size = new System.Drawing.Size(66, 20);
            this.tb_action1_param2.TabIndex = 19;
            // 
            // lbl_action1_param2
            // 
            this.lbl_action1_param2.AutoSize = true;
            this.lbl_action1_param2.Location = new System.Drawing.Point(152, 35);
            this.lbl_action1_param2.Name = "lbl_action1_param2";
            this.lbl_action1_param2.Size = new System.Drawing.Size(46, 13);
            this.lbl_action1_param2.TabIndex = 18;
            this.lbl_action1_param2.Text = "Param2:";
            // 
            // tb_action1_param1
            // 
            this.tb_action1_param1.Location = new System.Drawing.Point(75, 32);
            this.tb_action1_param1.Name = "tb_action1_param1";
            this.tb_action1_param1.Size = new System.Drawing.Size(66, 20);
            this.tb_action1_param1.TabIndex = 17;
            // 
            // lbl_action1_param1
            // 
            this.lbl_action1_param1.AutoSize = true;
            this.lbl_action1_param1.Location = new System.Drawing.Point(5, 35);
            this.lbl_action1_param1.Name = "lbl_action1_param1";
            this.lbl_action1_param1.Size = new System.Drawing.Size(46, 13);
            this.lbl_action1_param1.TabIndex = 16;
            this.lbl_action1_param1.Text = "Param1:";
            // 
            // pnl_action2
            // 
            this.pnl_action2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnl_action2.Controls.Add(this.lbl_action2);
            this.pnl_action2.Controls.Add(this.cb_action2);
            this.pnl_action2.Controls.Add(this.tb_action2_param3);
            this.pnl_action2.Controls.Add(this.lbl_action2_param3);
            this.pnl_action2.Controls.Add(this.tb_action2_param2);
            this.pnl_action2.Controls.Add(this.lbl_action2_param2);
            this.pnl_action2.Controls.Add(this.tb_action2_param1);
            this.pnl_action2.Controls.Add(this.lbl_action2_param1);
            this.pnl_action2.Location = new System.Drawing.Point(210, 187);
            this.pnl_action2.Name = "pnl_action2";
            this.pnl_action2.Size = new System.Drawing.Size(301, 86);
            this.pnl_action2.TabIndex = 25;
            // 
            // lbl_action2
            // 
            this.lbl_action2.AutoSize = true;
            this.lbl_action2.Location = new System.Drawing.Point(54, 6);
            this.lbl_action2.Name = "lbl_action2";
            this.lbl_action2.Size = new System.Drawing.Size(49, 13);
            this.lbl_action2.TabIndex = 25;
            this.lbl_action2.Text = "Action 2:";
            // 
            // cb_action2
            // 
            this.cb_action2.FormattingEnabled = true;
            this.cb_action2.Items.AddRange(new object[] {
            "None",
            "Text",
            "Set faction",
            "Morph to entry or model",
            "Sound",
            "Emote",
            "Random say",
            "Random yell",
            "Random textemote",
            "Random sound",
            "Random emote",
            "Cast",
            "Summon",
            "Threat single pct",
            "Threat all pct",
            "Quest event",
            "Cast event",
            "Set unit field",
            "Set unit flag",
            "Remove unit flag",
            "Auto attack",
            "Combat Movement",
            "Set phase",
            "Inc phase",
            "Evade",
            "Flee for assist",
            "Quest event all",
            "Cast event all",
            "Remove auras from spell",
            "Ranged movement",
            "Random phase",
            "Random phase range",
            "Summon Id",
            "Killed monster",
            "Set inst data",
            "Set inst data64",
            "Update template",
            "Die",
            "Zone combat pulse",
            "Call for help",
            "Sheath",
            "Force Despawn",
            "Set invincibility hp level",
            "Mount entry or model"});
            this.cb_action2.Location = new System.Drawing.Point(125, 3);
            this.cb_action2.Name = "cb_action2";
            this.cb_action2.Size = new System.Drawing.Size(163, 21);
            this.cb_action2.TabIndex = 24;
            this.cb_action2.Text = "None";
            this.cb_action2.SelectedIndexChanged += new System.EventHandler(this.cb_action_SelectedIndexChanged);
            // 
            // tb_action2_param3
            // 
            this.tb_action2_param3.Location = new System.Drawing.Point(75, 58);
            this.tb_action2_param3.Name = "tb_action2_param3";
            this.tb_action2_param3.Size = new System.Drawing.Size(66, 20);
            this.tb_action2_param3.TabIndex = 21;
            // 
            // lbl_action2_param3
            // 
            this.lbl_action2_param3.AutoSize = true;
            this.lbl_action2_param3.Location = new System.Drawing.Point(5, 61);
            this.lbl_action2_param3.Name = "lbl_action2_param3";
            this.lbl_action2_param3.Size = new System.Drawing.Size(46, 13);
            this.lbl_action2_param3.TabIndex = 20;
            this.lbl_action2_param3.Text = "Param3:";
            // 
            // tb_action2_param2
            // 
            this.tb_action2_param2.Location = new System.Drawing.Point(222, 32);
            this.tb_action2_param2.Name = "tb_action2_param2";
            this.tb_action2_param2.Size = new System.Drawing.Size(66, 20);
            this.tb_action2_param2.TabIndex = 19;
            // 
            // lbl_action2_param2
            // 
            this.lbl_action2_param2.AutoSize = true;
            this.lbl_action2_param2.Location = new System.Drawing.Point(152, 35);
            this.lbl_action2_param2.Name = "lbl_action2_param2";
            this.lbl_action2_param2.Size = new System.Drawing.Size(46, 13);
            this.lbl_action2_param2.TabIndex = 18;
            this.lbl_action2_param2.Text = "Param2:";
            // 
            // tb_action2_param1
            // 
            this.tb_action2_param1.Location = new System.Drawing.Point(75, 32);
            this.tb_action2_param1.Name = "tb_action2_param1";
            this.tb_action2_param1.Size = new System.Drawing.Size(66, 20);
            this.tb_action2_param1.TabIndex = 17;
            // 
            // lbl_action2_param1
            // 
            this.lbl_action2_param1.AutoSize = true;
            this.lbl_action2_param1.Location = new System.Drawing.Point(5, 35);
            this.lbl_action2_param1.Name = "lbl_action2_param1";
            this.lbl_action2_param1.Size = new System.Drawing.Size(46, 13);
            this.lbl_action2_param1.TabIndex = 16;
            this.lbl_action2_param1.Text = "Param1:";
            // 
            // pnl_action3
            // 
            this.pnl_action3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnl_action3.Controls.Add(this.lbl_action3);
            this.pnl_action3.Controls.Add(this.cb_action3);
            this.pnl_action3.Controls.Add(this.tb_action3_param3);
            this.pnl_action3.Controls.Add(this.lbl_action3_param3);
            this.pnl_action3.Controls.Add(this.tb_action3_param2);
            this.pnl_action3.Controls.Add(this.lbl_action3_param2);
            this.pnl_action3.Controls.Add(this.tb_action3_param1);
            this.pnl_action3.Controls.Add(this.lbl_action3_param1);
            this.pnl_action3.Location = new System.Drawing.Point(210, 278);
            this.pnl_action3.Name = "pnl_action3";
            this.pnl_action3.Size = new System.Drawing.Size(301, 86);
            this.pnl_action3.TabIndex = 26;
            // 
            // lbl_action3
            // 
            this.lbl_action3.AutoSize = true;
            this.lbl_action3.Location = new System.Drawing.Point(54, 6);
            this.lbl_action3.Name = "lbl_action3";
            this.lbl_action3.Size = new System.Drawing.Size(49, 13);
            this.lbl_action3.TabIndex = 25;
            this.lbl_action3.Text = "Action 3:";
            // 
            // cb_action3
            // 
            this.cb_action3.FormattingEnabled = true;
            this.cb_action3.Items.AddRange(new object[] {
            "None",
            "Text",
            "Set faction",
            "Morph to entry or model",
            "Sound",
            "Emote",
            "Random say",
            "Random yell",
            "Random textemote",
            "Random sound",
            "Random emote",
            "Cast",
            "Summon",
            "Threat single pct",
            "Threat all pct",
            "Quest event",
            "Cast event",
            "Set unit field",
            "Set unit flag",
            "Remove unit flag",
            "Auto attack",
            "Combat Movement",
            "Set phase",
            "Inc phase",
            "Evade",
            "Flee for assist",
            "Quest event all",
            "Cast event all",
            "Remove auras from spell",
            "Ranged movement",
            "Random phase",
            "Random phase range",
            "Summon Id",
            "Killed monster",
            "Set inst data",
            "Set inst data64",
            "Update template",
            "Die",
            "Zone combat pulse",
            "Call for help",
            "Sheath",
            "Force Despawn",
            "Set invincibility hp level",
            "Mount entry or model"});
            this.cb_action3.Location = new System.Drawing.Point(125, 3);
            this.cb_action3.Name = "cb_action3";
            this.cb_action3.Size = new System.Drawing.Size(163, 21);
            this.cb_action3.TabIndex = 24;
            this.cb_action3.Text = "None";
            this.cb_action3.SelectedIndexChanged += new System.EventHandler(this.cb_action_SelectedIndexChanged);
            // 
            // tb_action3_param3
            // 
            this.tb_action3_param3.Location = new System.Drawing.Point(75, 58);
            this.tb_action3_param3.Name = "tb_action3_param3";
            this.tb_action3_param3.Size = new System.Drawing.Size(66, 20);
            this.tb_action3_param3.TabIndex = 21;
            // 
            // lbl_action3_param3
            // 
            this.lbl_action3_param3.AutoSize = true;
            this.lbl_action3_param3.Location = new System.Drawing.Point(5, 61);
            this.lbl_action3_param3.Name = "lbl_action3_param3";
            this.lbl_action3_param3.Size = new System.Drawing.Size(46, 13);
            this.lbl_action3_param3.TabIndex = 20;
            this.lbl_action3_param3.Text = "Param3:";
            // 
            // tb_action3_param2
            // 
            this.tb_action3_param2.Location = new System.Drawing.Point(222, 32);
            this.tb_action3_param2.Name = "tb_action3_param2";
            this.tb_action3_param2.Size = new System.Drawing.Size(66, 20);
            this.tb_action3_param2.TabIndex = 19;
            // 
            // lbl_action3_param2
            // 
            this.lbl_action3_param2.AutoSize = true;
            this.lbl_action3_param2.Location = new System.Drawing.Point(152, 35);
            this.lbl_action3_param2.Name = "lbl_action3_param2";
            this.lbl_action3_param2.Size = new System.Drawing.Size(46, 13);
            this.lbl_action3_param2.TabIndex = 18;
            this.lbl_action3_param2.Text = "Param2:";
            // 
            // tb_action3_param1
            // 
            this.tb_action3_param1.Location = new System.Drawing.Point(75, 32);
            this.tb_action3_param1.Name = "tb_action3_param1";
            this.tb_action3_param1.Size = new System.Drawing.Size(66, 20);
            this.tb_action3_param1.TabIndex = 17;
            // 
            // lbl_action3_param1
            // 
            this.lbl_action3_param1.AutoSize = true;
            this.lbl_action3_param1.Location = new System.Drawing.Point(5, 35);
            this.lbl_action3_param1.Name = "lbl_action3_param1";
            this.lbl_action3_param1.Size = new System.Drawing.Size(46, 13);
            this.lbl_action3_param1.TabIndex = 16;
            this.lbl_action3_param1.Text = "Param1:";
            // 
            // tb_comment
            // 
            this.tb_comment.Location = new System.Drawing.Point(63, 334);
            this.tb_comment.Name = "tb_comment";
            this.tb_comment.Size = new System.Drawing.Size(141, 20);
            this.tb_comment.TabIndex = 27;
            // 
            // lbl_comment
            // 
            this.lbl_comment.AutoSize = true;
            this.lbl_comment.Location = new System.Drawing.Point(3, 337);
            this.lbl_comment.Name = "lbl_comment";
            this.lbl_comment.Size = new System.Drawing.Size(54, 13);
            this.lbl_comment.TabIndex = 28;
            this.lbl_comment.Text = "Comment:";
            // 
            // AiTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lbl_comment);
            this.Controls.Add(this.tb_comment);
            this.Controls.Add(this.pnl_action3);
            this.Controls.Add(this.pnl_action2);
            this.Controls.Add(this.pnl_action1);
            this.Controls.Add(this.pnl_type);
            this.Controls.Add(this.lbl_flags);
            this.Controls.Add(this.clb_flags);
            this.Controls.Add(this.tb_chance);
            this.Controls.Add(this.lbl_chance);
            this.Controls.Add(this.lbl_phases);
            this.Controls.Add(this.pnl_phase_placeholder);
            this.Controls.Add(this.lbl_type);
            this.Controls.Add(this.cob_type);
            this.Name = "AiTab";
            this.Size = new System.Drawing.Size(514, 370);
            this.pnl_type.ResumeLayout(false);
            this.pnl_type.PerformLayout();
            this.pnl_action1.ResumeLayout(false);
            this.pnl_action1.PerformLayout();
            this.pnl_action2.ResumeLayout(false);
            this.pnl_action2.PerformLayout();
            this.pnl_action3.ResumeLayout(false);
            this.pnl_action3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cob_type;
        private System.Windows.Forms.Label lbl_type;
        private System.Windows.Forms.Panel pnl_phase_placeholder;
        private System.Windows.Forms.Label lbl_phases;
        private System.Windows.Forms.Label lbl_chance;
        private System.Windows.Forms.TextBox tb_chance;
        private System.Windows.Forms.CheckedListBox clb_flags;
        private System.Windows.Forms.Label lbl_flags;
        private System.Windows.Forms.Panel pnl_type;
        private System.Windows.Forms.TextBox tb_param4;
        private System.Windows.Forms.Label lbl_param4;
        private System.Windows.Forms.TextBox tb_param3;
        private System.Windows.Forms.Label lbl_param3;
        private System.Windows.Forms.TextBox tb_param2;
        private System.Windows.Forms.Label lbl_param2;
        private System.Windows.Forms.TextBox tb_param1;
        private System.Windows.Forms.Label lbl_param1;
        private System.Windows.Forms.Panel pnl_action1;
        private System.Windows.Forms.TextBox tb_action1_param3;
        private System.Windows.Forms.Label lbl_action1_param3;
        private System.Windows.Forms.TextBox tb_action1_param2;
        private System.Windows.Forms.Label lbl_action1_param2;
        private System.Windows.Forms.TextBox tb_action1_param1;
        private System.Windows.Forms.Label lbl_action1_param1;
        private System.Windows.Forms.Label lbl_action1;
        private System.Windows.Forms.ComboBox cb_action1;
        private System.Windows.Forms.Panel pnl_action2;
        private System.Windows.Forms.Label lbl_action2;
        private System.Windows.Forms.ComboBox cb_action2;
        private System.Windows.Forms.TextBox tb_action2_param3;
        private System.Windows.Forms.Label lbl_action2_param3;
        private System.Windows.Forms.TextBox tb_action2_param2;
        private System.Windows.Forms.Label lbl_action2_param2;
        private System.Windows.Forms.TextBox tb_action2_param1;
        private System.Windows.Forms.Label lbl_action2_param1;
        private System.Windows.Forms.Panel pnl_action3;
        private System.Windows.Forms.Label lbl_action3;
        private System.Windows.Forms.ComboBox cb_action3;
        private System.Windows.Forms.TextBox tb_action3_param3;
        private System.Windows.Forms.Label lbl_action3_param3;
        private System.Windows.Forms.TextBox tb_action3_param2;
        private System.Windows.Forms.Label lbl_action3_param2;
        private System.Windows.Forms.TextBox tb_action3_param1;
        private System.Windows.Forms.Label lbl_action3_param1;
        private System.Windows.Forms.TextBox tb_comment;
        private System.Windows.Forms.Label lbl_comment;
    }
}
