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


using MySql.Data.MySqlClient;
using System;
using System.IO;
using System.Windows.Forms;

namespace VisualEventAI.Forms
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void AddNewAiTab()
        {
            tc_content.TabPages.Add("AI");
            var newTab = tc_content.TabPages[tc_content.TabCount - 1];

            var aiTab = new AiTab();
            aiTab.Dock = DockStyle.Fill;

            newTab.Controls.Add(aiTab);
        }

        private void btn_new_Click(object sender, EventArgs e)
        {
            AddNewAiTab();

        }

        private void btn_save_Click(object sender, EventArgs e)
        {
            var now = DateTime.Now;

            var fileName = String.Format("out\\{0}-{1}-{2}.sql", now.Hour, now.Minute, now.Second);

            
            using(var writer = new StreamWriter(fileName))
            {
                writer.WriteLine("INSERT INTO creature_ai_scripts "+
                    "(creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, "+
                    "event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, "+
                    "action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES");
                int i = 0;
                foreach (TabPage tabPage in tc_content.TabPages)
                {
                    AiTab tab = tabPage.Controls[0] as AiTab;
                    tab.SaveToStream(writer, UInt32.Parse(tb_entry.Text));

                    if (++i < tc_content.TabPages.Count)
                    {
                        writer.Write(",\r\n");
                    }
                }

                writer.WriteLine(";");
            }
        }

        private void btn_load_Click(object sender, EventArgs e)
        {
            MySqlConnection connection;
            var sql = String.Format("SELECT * FROM creature_ai_scripts WHERE creature_id = {0}", UInt32.Parse(tb_entry.Text));

            try
            {
                connection = new MySqlConnection("server=127.0.0.1;uid=root;pwd=;database=world2;");
                connection.Open();

                MySqlCommand cmd = new MySqlCommand(sql, connection);
                var data = cmd.ExecuteReader();

                if (!data.HasRows)
                    return;


                while(data.Read())
                {
                    tc_content.TabPages.Add("AI");
                    var newTab = tc_content.TabPages[tc_content.TabCount - 1];
                    var aiTab = new AiTab();
                    aiTab.Dock = DockStyle.Fill;
                    aiTab.FromDatabase(data);

                    newTab.Controls.Add(aiTab);
                }
            }
            catch (MySqlException ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
