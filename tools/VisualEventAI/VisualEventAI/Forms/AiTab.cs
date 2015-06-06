/*
Copyright (c) 2014 Nightblizard (nightblizard@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


using System;
using System.Windows.Forms;
using System.IO;
using MySql.Data.MySqlClient;

namespace VisualEventAI.Forms
{
    public partial class AiTab : UserControl
    {
        private String[][] mTypeParameters = new String[][]
        {
            new String[] { "Initial min:", "Initial max:", "Repeat min:", "Repeat max:" },
            new String[] { "Initial min:", "Initial max:", "Repeat min:", "Repeat max:" },
            new String[] { "Hp max %:", "Hp min %:", "Repeat min:", "Repeat max:" },
            new String[] { "Mana max %:", "Mana min %:", "Repeat min:", "Repeat max:" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Initial min:", "Initial max:", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Spell Id:", "School:", "Repeat min:", "Repeat max:" },
            new String[] { "Min dist:", "Max dist:", "Repeat min:", "Repeat max:" },
            new String[] { "No hostile:", "Max range:", "Repeat min:", "Repeat max:" },
            new String[] { "Condition:", "Cond value:", "None", "None" },
            new String[] { "Hp max %:", "Hp min %:", "Repeat min:", "Repeat max:" },
            new String[] { "Repeat min:", "Repeat max:", "None", "None" },
            new String[] { "Hp deficit:", "Radius:", "Repeat min:", "Repeat max:" },
            new String[] { "Dispell type:", "Radius:", "Repeat min:", "Repeat max:" },
            new String[] { "Spell Id:", "Radius:", "Repeat min:", "Repeat max:" },
            new String[] { "Creature Id:", "Repeat min:", "Repeat max:", "None" },
            new String[] { "Mana max %:", "Mana min %:", "Repeat min:", "Repeat max:" },
            new String[] { "Quest Id:", "None", "None", "None" },
            new String[] { "Quest Id:", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Emote Id:", "Condition:", "Cond value 1:", "Cond value 2:" },
            new String[] { "Spell Id:", "Stacks:", "Repeat min:", "Repeat max:" },
            new String[] { "Spell Id:", "Stacks:", "Repeat min:", "Repeat max:" },
            new String[] { "Creature Id:",  "Repeat min:", "Repeat max:", "None" },
            new String[] { "Creature Id:",  "Repeat min:", "Repeat max:", "None" },
            new String[] { "Spell Id:", "Stacks:", "Repeat min:", "Repeat max:" },
            new String[] { "Spell Id:", "Stacks:", "Repeat min:", "Repeat max:" },
            new String[] { "Spell Id:", "Stacks:", "Repeat min:", "Repeat max:" },
        };

        private String[][] mActionParameters = new String[][]
        {
            new String[] { "None", "None", "None", "None" },
            new String[] { "Text Id 1", "Text Id 2", "Text Id 3", "None" },
            new String[] { "Faction Id", "None", "None", "None" },
            new String[] { "Entry", "Model Id", "None", "None" },
            new String[] { "Sound Id", "None", "None", "None" },
            new String[] { "Emote Id", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Sound 1", "Sound 2", "Sound 3", "None" },
            new String[] { "Emote 1", "Emote 2", "Emote 3", "None" },
            new String[] { "Spell Id", "Target", "Cast flags", "None" },
            new String[] { "Entry", "Target", "Duration (ms)", "None" },
            new String[] { "Threat %", "Target", "None", "None" },
            new String[] { "Threat %", "None", "None", "None" },
            new String[] { "Quest Id", "Target", "None", "None" },
            new String[] { "Quest Id", "Spell Id", "Target", "None" },
            new String[] { "Filed", "Value", "Target", "None" },
            new String[] { "Flags", "Target", "None", "None" },
            new String[] { "Flags", "Target", "None", "None" },
            new String[] { "Allow", "None", "None", "None" },
            new String[] { "Allow", "None", "None", "None" },
            new String[] { "Phase", "None", "None", "None" },
            new String[] { "Value", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Quest Id", "None", "None", "None" },
            new String[] { "Creature Id", "Spell Id", "None", "None" },
            new String[] { "Target", "Spell Id", "None", "None" },
            new String[] { "Distance", "Angle", "None", "None" },
            new String[] { "Phase 1", "Phase 2", "Phase 3", "None" },
            new String[] { "Phase min", "Phase max", "None", "None" },
            new String[] { "Creature Id", "Target", "Spawn Id", "None" },
            new String[] { "Creature Id", "Target", "None", "None" },
            new String[] { "Field", "Data", "None", "None" },
            new String[] { "Field", "Target", "None", "None" },
            new String[] { "Entry", "Team", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "None", "None", "None", "None" },
            new String[] { "Radius", "None", "None", "None" },
            new String[] { "Sheath", "None", "None", "None" },
            new String[] { "Delay", "None", "None", "None" },
            new String[] { "Min Hp", "None", "None", "None" },
            new String[] { "Entry", "Model Id", "None", "None" },
        };

        private PhaseGrid pg_phases;

        public AiTab()
        {
            InitializeComponent();

            pg_phases = new PhaseGrid();
            pg_phases.Location = this.pnl_phase_placeholder.Location;

            Controls.Add(pg_phases);
        }

        public void SaveToStream(StreamWriter writer, UInt32 entry)
        {
            writer.Write("(  '{0}',  '{1}',  '{2}', '{3}', '{4}'," +
                         "'{5}',  '{6}',  '{7}',  '{8}'," +
                         "'{9}',  '{10}', '{11}', '{12}'," +
                         "'{13}', '{14}', '{15}', '{16}'," +
                         "'{17}', '{18}', '{19}', '{20}'," +
                         "'{21}' )",
                         entry, cob_type.SelectedIndex, pg_phases.GetPhaseValue(), tb_chance.Text, GetFlags(),
                         tb_param1.Text, tb_param2.Text, tb_param3.Text, tb_param4.Text,
                         cb_action1.SelectedIndex, tb_action1_param1.Text, tb_action1_param2.Text, tb_action1_param3.Text,
                         cb_action2.SelectedIndex, tb_action2_param1.Text, tb_action2_param2.Text, tb_action2_param3.Text,
                         cb_action3.SelectedIndex, tb_action3_param1.Text, tb_action3_param2.Text, tb_action3_param3.Text,
                         tb_comment.Text
                );
        }

        public void FromDatabase(MySqlDataReader reader)
        {
            cob_type.SelectedIndex = reader.GetInt32(2);
            pg_phases.SetPhaseValue(reader.GetUInt32(3));
            tb_chance.Text = reader.GetString(4);
            SetFlags(reader.GetUInt32(5));
            tb_param1.Text = reader.GetString(6);
            tb_param2.Text = reader.GetString(7);
            tb_param3.Text = reader.GetString(8);
            tb_param4.Text = reader.GetString(9);
            cb_action1.SelectedIndex = reader.GetInt32(10);
            tb_action1_param1.Text = reader.GetString(11);
            tb_action1_param2.Text = reader.GetString(12);
            tb_action1_param3.Text = reader.GetString(13);
            cb_action2.SelectedIndex = reader.GetInt32(14);
            tb_action2_param1.Text = reader.GetString(15);
            tb_action2_param2.Text = reader.GetString(16);
            tb_action2_param3.Text = reader.GetString(17);
            cb_action3.SelectedIndex = reader.GetInt32(18);
            tb_action3_param1.Text = reader.GetString(19);
            tb_action3_param1.Text = reader.GetString(20);
            tb_action3_param1.Text = reader.GetString(21);
            tb_comment.Text = reader.GetString(22);
        }

        private UInt32 GetFlags()
        {
            uint ret = 0;
            foreach (int n in clb_flags.CheckedIndices)
            {
                var foo = 1 << n;
                ret = ret | (uint)foo;
            }
            
            return ret;
        }

        private void SetFlags(UInt32 flags)
        {
            for (int i = 0; i < 7; ++i)
                clb_flags.SetItemChecked(i, ((flags >> i) & 1) == 1);
        }

        private void cob_type_SelectedIndexChanged(object sender, EventArgs e)
        {
            var index = cob_type.SelectedIndex;
            lbl_param1.Text = mTypeParameters[index][0];
            lbl_param2.Text = mTypeParameters[index][1];
            lbl_param3.Text = mTypeParameters[index][2];
            lbl_param4.Text = mTypeParameters[index][3];
        }

        private void UpdateActionCaptions(int index, Label lbl1, Label lbl2, Label lbl3)
        {
            lbl1.Text = mActionParameters[index][0];
            lbl2.Text = mActionParameters[index][1];
            lbl3.Text = mActionParameters[index][2];
        }

        private void cb_action_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender is ComboBox)
            {
                var comboBox = sender as ComboBox;
                var index = comboBox.SelectedIndex;
                switch (comboBox.Name)
                {
                    case "cb_action1":
                        UpdateActionCaptions(index, lbl_action1_param1, lbl_action1_param2, lbl_action1_param3);
                        break;
                    case "cb_action2":
                        UpdateActionCaptions(index, lbl_action2_param1, lbl_action2_param2, lbl_action2_param3);
                        break;
                    case "cb_action3":
                        UpdateActionCaptions(index, lbl_action3_param1, lbl_action3_param2, lbl_action3_param3);
                        break;
                }
            }
        }
    }
}
