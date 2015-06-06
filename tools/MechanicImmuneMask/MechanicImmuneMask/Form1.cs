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

namespace MechanicImmuneMask
{
    public partial class Form1 : Form
    {
        uint mVal = 0;

        CheckBox[] mcheckBoxes = new CheckBox[31];

        public Form1()
        {
            InitializeComponent();
            mcheckBoxes[0] = checkBox1;
            mcheckBoxes[1] = checkBox2;
            mcheckBoxes[2] = checkBox3;
            mcheckBoxes[3] = checkBox4;
            mcheckBoxes[4] = checkBox5;
            mcheckBoxes[5] = checkBox6;
            mcheckBoxes[6] = checkBox7;
            mcheckBoxes[7] = checkBox8;
            mcheckBoxes[8] = checkBox9;
            mcheckBoxes[9] = checkBox10;
            mcheckBoxes[10] = checkBox11;
            mcheckBoxes[11] = checkBox12;
            mcheckBoxes[12] = checkBox13;
            mcheckBoxes[13] = checkBox14;
            mcheckBoxes[14] = checkBox15;
            mcheckBoxes[15] = checkBox16;
            mcheckBoxes[16] = checkBox17;
            mcheckBoxes[17] = checkBox18;
            mcheckBoxes[18] = checkBox19;
            mcheckBoxes[19] = checkBox20;
            mcheckBoxes[20] = checkBox21;
            mcheckBoxes[21] = checkBox22;
            mcheckBoxes[22] = checkBox23;
            mcheckBoxes[23] = checkBox24;
            mcheckBoxes[24] = checkBox25;
            mcheckBoxes[25] = checkBox26;
            mcheckBoxes[26] = checkBox27;
            mcheckBoxes[27] = checkBox28;
            mcheckBoxes[28] = checkBox29;
            mcheckBoxes[29] = checkBox30;
            mcheckBoxes[30] = checkBox31;
        }

        private void checkBox25_CheckedChanged(object sender, EventArgs e)
        {
            
            var controlbox = sender as CheckBox;
            var tmp = controlbox.Name.Substring(8, controlbox.Name.Length - 8);
            int cbox = Int32.Parse(tmp) - 1;
            var foo = (uint)(1 << cbox);

            if (controlbox.Checked)
                mVal |= foo;
            else
                mVal &= ~foo;

            textBox1.Text = "" + mVal;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (textBox1.Text == String.Empty)
                    mVal = 0;
                else
                    mVal = UInt32.Parse(textBox1.Text);
            } catch(Exception)
            {
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            for (int i = 0; i < 31; ++i)
            {
                if ((mVal & (1 << i)) != 0)
                    mcheckBoxes[i].Checked = true;
                else
                    mcheckBoxes[i].Checked = false;
            }

            for (int i = 0; i < 31; ++i)
            {
                if (mcheckBoxes[i].Checked)
                    mVal |= (uint)(1 << i);
                else
                    mcheckBoxes[i].Checked = false;
            }

            textBox1.Text = "" + mVal;
        }
    }
}
