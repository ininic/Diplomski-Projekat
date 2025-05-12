using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Projekat_UI
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            int x;
            try
            {
                x = Convert.ToInt32(tb1.Text);
                if (x >= 0)
                {
                    MainWindow.cost_value = x;
                    MainWindow.cost_input = true;
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Unesite pozitivan ceo broj!");
                }
            }
            catch
            {
                MessageBox.Show("Unesite pozitivan ceo broj!");
            }
                               
        }
    }
}
