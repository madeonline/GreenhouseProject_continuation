using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;

namespace UROVConfig
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string key = "UROVa01qwert";  //пишем тут любой ключ,это для проверки(вместо названия программы)
            using (Mutex mutex = new Mutex(false, key))
            {
                if (!mutex.WaitOne(0, false))
                {
                    MessageBox.Show("Программа уже открыта!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.Run(new MainForm());
                }
            }
        }
    }
}
