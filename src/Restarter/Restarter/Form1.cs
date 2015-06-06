using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.MemoryMappedFiles;
using System.IO;

namespace Restarter
{
    public partial class Form1 : Form
    {
        private bool mRunning = false;
        MemoryMappedFile mFile = MemoryMappedFile.CreateOrOpen("RestarterMap" + Directory.GetCurrentDirectory().GetHashCode(), 4);
        
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Process.EnterDebugMode();
            timer1_Tick(null, null);
        }

        private void btn_world_Click(object sender, EventArgs e)
        {
            mRunning = !mRunning;
            using (MemoryMappedViewAccessor accessor = mFile.CreateViewAccessor())
            {
                accessor.Write<bool>(0, ref mRunning);
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            check();

            if (mRunning)
            {
                btn_world.BackColor = Color.Green;
                btn_world.Text = "stop server";
            }
            else
            {
                btn_world.BackColor = Color.Red;
                btn_world.Text = "start server";
            }

            if (isProcessRunning("mangosd"))
            {
                lbl_mangos.BackColor = Color.Green;
                lbl_mangos.Text = "world: running";
            }
            else
            {
                if (mRunning && lbl_mangos.Checked)
                    starProcess("mangosd");
                lbl_mangos.BackColor = Color.Red;
                lbl_mangos.Text = "world: stopped";
            }

            if (isProcessRunning("realmd"))
            {
                lbl_realm.BackColor = Color.Green;
                lbl_realm.Text = "realm: running";
            }
            else
            {
                if (mRunning && lbl_realm.Checked)
                    starProcess("realmd");
                lbl_realm.BackColor = Color.Red;
                lbl_realm.Text = "realm: stopped";
            }
        }

        private bool isProcessRunning(string name)
        {
            var procs = Process.GetProcessesByName(name);

            foreach (Process proc in procs)
            {
                var dir = new FileInfo(proc.MainWindowTitle).DirectoryName;
                var curDir = Directory.GetCurrentDirectory();
                if (dir == curDir)
                    return true;
            }

            return false;
        }

        private void starProcess(string name)
        {
            var proc = new Process();
            proc.StartInfo.FileName = name + ".exe";

            proc.Start();
        }

        private void stopProcess(string name)
        {
            var procs = Process.GetProcessesByName(name);
            foreach (Process proc in procs)
            {
                try
                {
                    var dir = new FileInfo(proc.MainWindowTitle).DirectoryName;
                    var curDir = Directory.GetCurrentDirectory();
                    if (dir == curDir)
                        proc.Kill();
                }
                catch (Exception)
                {
                }
            }
        }

        private void check()
        {
            using (MemoryMappedViewAccessor accessor = mFile.CreateViewAccessor())
            {
                mRunning = accessor.ReadBoolean(0);
            }
        }

        private void btn_kill_Click(object sender, EventArgs e)
        {
            stopProcess("realmd");
            stopProcess("mangosd");
        }
    }
}
