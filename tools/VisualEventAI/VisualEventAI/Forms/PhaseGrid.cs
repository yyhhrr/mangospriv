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

namespace VisualEventAI.Forms
{
    public partial class PhaseGrid : UserControl
    {
        public PhaseGrid()
        {
            InitializeComponent();
        }

        public UInt32 GetPhaseValue()
        {
            var _1 = (cb_1.Checked ? (uint)1 : (uint)0) << 0;
            var _2 = (cb_2.Checked ? (uint)1 : (uint)0) << 1;
            var _3 = (cb_3.Checked ? (uint)1 : (uint)0) << 2;
            var _4 = (cb_4.Checked ? (uint)1 : (uint)0) << 3;
            var _5 = (cb_5.Checked ? (uint)1 : (uint)0) << 4;
            var _6 = (cb_6.Checked ? (uint)1 : (uint)0) << 5;
            var _7 = (cb_7.Checked ? (uint)1 : (uint)0) << 6;
            var _8 = (cb_8.Checked ? (uint)1 : (uint)0) << 7;
            var _9 = (cb_9.Checked ? (uint)1 : (uint)0) << 8;
            var _10 = (cb_10.Checked ? (uint)1 : (uint)0) << 9;
            var _11 = (cb_11.Checked ? (uint)1 : (uint)0) << 10;
            var _12 = (cb_12.Checked ? (uint)1 : (uint)0) << 11;
            var _13 = (cb_13.Checked ? (uint)1 : (uint)0) << 12;
            var _14 = (cb_14.Checked ? (uint)1 : (uint)0) << 13;
            var _15 = (cb_15.Checked ? (uint)1 : (uint)0) << 14;
            var _16 = (cb_16.Checked ? (uint)1 : (uint)0) << 15;
            

            return _1 | _2 | _3 | _4 | _5 | _6 | _7 | _8 | _9 | _10 | _11 | _12 | _13 | _14 | _15 | _16;
        }

        public void SetPhaseValue(UInt32 phase)
        {
            cb_1.Checked = ((phase >> 0) & 1) == 1;
            cb_2.Checked = ((phase >> 1) & 1) == 1;
            cb_3.Checked = ((phase >> 2) & 1) == 1;
            cb_4.Checked = ((phase >> 3) & 1) == 1;
            cb_5.Checked = ((phase >> 4) & 1) == 1;
            cb_6.Checked = ((phase >> 5) & 1) == 1;
            cb_7.Checked = ((phase >> 6) & 1) == 1;
            cb_8.Checked = ((phase >> 7) & 1) == 1;
            cb_9.Checked = ((phase >> 8) & 1) == 1;
            cb_10.Checked = ((phase >> 9) & 1) == 1;
            cb_11.Checked = ((phase >> 10) & 1) == 1;
            cb_12.Checked = ((phase >> 11) & 1) == 1;
            cb_13.Checked = ((phase >> 12) & 1) == 1;
            cb_14.Checked = ((phase >> 13) & 1) == 1;
            cb_15.Checked = ((phase >> 14) & 1) == 1;
            cb_16.Checked = ((phase >> 15) & 1) == 1;
        }
    }
}
