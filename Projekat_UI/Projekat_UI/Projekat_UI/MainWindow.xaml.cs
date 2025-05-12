using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Projekat_UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static List<Image> images;
        private static List<Router> routers;
        private static Router current_router;
        private bool simulation_done;
        public static int cost_value;
        public static bool cost_input;
        public static List<Label> cost_labels;
        public static List<Point> points;
        public static Image message_img;
        public MainWindow()
        {
            InitializeComponent();
            simulation_done = false;
        }

        private void GenerateRouters(object sender, RoutedEventArgs e)
        {
            //Proverava se da li se uneti string moze konvertovati u celobrojnu vrednost
            try
            {
                Int32.Parse(router_nbr_tb.Text);

                //Dozvoljen izabrani broj rutera mora biti u opsegu od [1-15]
                if (Int32.Parse(router_nbr_tb.Text) <= 15 && Int32.Parse(router_nbr_tb.Text) > 0)
                {
                    radio_rip.IsEnabled = true;
                    radio_ospf.IsEnabled = true;
                    radio_ospf.IsChecked = false;
                    radio_rip.IsChecked = false;

                    canvas.Children.Clear();
                    simulation_done = false;
                    simulation_tb.Text = "Rezultati simulacije:";

                    images = new List<Image>();
                    routers = new List<Router>();
                    cost_labels = new List<Label>();                    
                    int max = Convert.ToInt32(router_nbr_tb.Text.ToString());
                    int id_counter = 101;

                    //trenutni broj rutera se upisuje u routers_info.txt
                    string routers_info_str = router_nbr_tb.Text;
                    using (StreamWriter writetext = new StreamWriter("routers_info.txt"))
                    {
                        writetext.Write(routers_info_str);
                    }

                    //Generiše se i iscrtava odabrani broj rutera
                    for (int j = 0; j < max; j++)
                    {
                        Image i = new Image();
                        BitmapImage src = new BitmapImage();
                        src.BeginInit();
                        src.UriSource = new Uri("./Resources/router96.png", UriKind.Relative);
                        src.EndInit();
                        i.Source = src;
                        i.Stretch = Stretch.None;


                        double left = canvas.ActualWidth / 2;
                        double top = canvas.ActualHeight / 2;

                        //Postizanje kružnog rasporeda rutera
                        Canvas.SetTop(i, top + 150 * Math.Cos(j * (2 * Math.PI / max)));
                        Canvas.SetLeft(i, left + 150 * Math.Sin(j * (2 * Math.PI / max)));

                        //U okviru objekta klase ruter, pamte se koordinate unutrašnjih tačaka
                        //ka početku kružnice
                        Router r = new Router(left + 130 * Math.Sin(j * (2 * Math.PI / max)), top + 130 * Math.Cos(j * (2 * Math.PI / max)), i);
                        r.Router_id = id_counter++;

                        //Svakom ruteru se inicajalno dodaje po jedan uređaj, radi lakšeg praćenja simulacije
                        r.AddDevice();
                        routers.Add(r);


                        //Ispisivanje rednih brojeva rutera pored ikonica istih
                        Label l = new Label();
                        l.Content = (j + 1).ToString();
                        l.FontWeight = FontWeights.Bold;
                        l.FontSize = 25;

                        if (top > top + 150 * Math.Cos(j * (2 * Math.PI / max)))
                        {
                            Canvas.SetTop(l, top + 150 * Math.Cos(j * (2 * Math.PI / max)) - 25);
                        }
                        else
                        {
                            Canvas.SetTop(l, top + 150 * Math.Cos(j * (2 * Math.PI / max)) + 40);
                        }

                        if (left > left + 150 * Math.Sin(j * (2 * Math.PI / max)))
                        {
                            Canvas.SetLeft(l, left + 150 * Math.Sin(j * (2 * Math.PI / max)) - 25);
                        }
                        else
                        {
                            Canvas.SetLeft(l, left + 150 * Math.Sin(j * (2 * Math.PI / max)) + 50);
                        }

                        canvas.Children.Add(i);
                        canvas.Children.Add(l);
                        images.Add(i);
                    }

                    //Za svaki ruter se pravi combobox koje se uvezuje u contextmenu ikonice rutera
                    //izborom opcije u combobox-u omogucava se povezivanje sa izabranim drugim ruterom
                    foreach (var r in routers)
                    {

                        MenuItem mi = new MenuItem();
                        ComboBox cb = new ComboBox();
                        cb.Items.Add("poveži sa:");
                        cb.SelectedItem = cb.Items[0];

                        int k = 0;
                        //opcije comobox-a su svi ostali ruteri u mrezi
                        foreach (var img in images)
                        {
                            k++;
                            cb.Items.Add("Ruter" + k.ToString());

                        }
                        mi.Header = cb;
                        mi.ContextMenu = new ContextMenu();

                        r.Img.ContextMenu = new ContextMenu();
                        r.Img.ContextMenu.Items.Add(mi);


                        //Za svaki ruter se dodaje interfejs ka loklnoj mrezi rutera
                        r.add_interface(r.Router_id * 10 + 1 + r.Interfaces.Count, (r.Router_id).ToString() + ".1." + (r.Interfaces.Count + 1).ToString() + ".1",
                            "0.0.0.0", "000", 0, null, 0);

                        //Klikom na ikonicu svakog rutera dobijaju se podaci o istom
                        r.Img.MouseDown += (s, a) =>
                        {
                            current_router = r;
                            ViewRouterInfo(r);
                        };

                        //Kada se odabere nova opcija u combobox-u, poziva se sledeca funkcija
                        cb.SelectionChanged += (s, a) =>
                        {
                            //provera da li se ruter pokušava povezati sam sa sobom
                            if (cb.SelectedIndex > 0 && r.Router_id != routers[cb.SelectedIndex - 1].Router_id)
                            {
                                //zatim se proverava da li je ruter već povezan sa izabranim ruterom
                                bool connected = false;
                                foreach (Interface inter in r.Interfaces)
                                {
                                    if (inter.Nbr_router_id.Equals(routers[cb.SelectedIndex - 1].Router_id.ToString()))
                                    {
                                        connected = true;
                                    }
                                }

                                //ukoliko je izabran odgovarajući ruter, vrši se logičko i vizuelno povezivanje
                                if (!connected)
                                {
                                    //proverava se koji algoritam je izabran
                                    if (radio_ospf.IsChecked == true)
                                    {
                                        cost_input = false;
                                        Window1 w1 = new Window1();
                                        w1.Owner = this;
                                        w1.ShowDialog();
                                        if (cost_input)
                                        {
                                            Label l1 = new Label();
                                            cost_labels.Add(l1);

                                            //ako se klikne na odredjenu labelu, ona dolazi u prvi plan
                                            l1.MouseLeftButtonDown += (se, ar) =>
                                            {
                                                foreach (Label l in cost_labels)
                                                {
                                                    Canvas.SetZIndex(l, 5);
                                                }
                                                Canvas.SetZIndex(l1, Canvas.GetZIndex(l1) + 2);
                                            };

                                            l1.Background = new SolidColorBrush(Colors.Black) { Opacity = 0.5 };
                                            l1.FontSize = 11;
                                            l1.Foreground = new SolidColorBrush(Colors.White);
                                            l1.FontWeight = FontWeights.Bold;
                                            l1.Content = cost_value.ToString();

                                            Canvas.SetZIndex(l1, 5);

                                            Random rnd = new Random();
                                            int num = rnd.Next();
                       
                                            //Labela sa vrednosću težine veze se pozicionira na liniju veze
                                            Canvas.SetLeft(l1, (r.X + 25 + routers[cb.SelectedIndex - 1].X) / 2 + (r.X - routers[cb.SelectedIndex - 1].X) / 10);
                                            Canvas.SetTop(l1, (r.Y + 25 + routers[cb.SelectedIndex - 1].Y) / 2 + (r.Y - routers[cb.SelectedIndex - 1].Y) / 10);

                                            Line myLine = new Line();
                                            ConnectRouters(r.X, r.Y, routers[cb.SelectedIndex - 1].X, routers[cb.SelectedIndex - 1].Y, myLine);
                                            Connect_routers_logicaly(r, routers[cb.SelectedIndex - 1], myLine, cost_value);

                                            canvas.Children.Add(l1);
                                        }
                                    }
                                    else if (radio_rip.IsChecked == true)
                                    {
                                        Line myLine = new Line();
                                        ConnectRouters(r.X, r.Y, routers[cb.SelectedIndex - 1].X, routers[cb.SelectedIndex - 1].Y, myLine);
                                        Connect_routers_logicaly(r, routers[cb.SelectedIndex - 1], myLine, 1);
                                    }
                                    else
                                    {
                                        MessageBox.Show("Izaberite algoritam rutiranja!");
                                    }
                                }
                                cb.SelectedItem = cb.Items[r.Router_id - 100];
                            }

                        };
                    }
                }
                else if (Int32.Parse(router_nbr_tb.Text) <= 0)
                {
                    MessageBox.Show("Broj rutera mora biti vrednost veća od 0.");
                }
                else
                {
                    MessageBox.Show("Maksimalan dozvoljan broj rutera u mreži je 15.");
                }
            }
            catch
            {
                MessageBox.Show("Potrebno je uneti ceo broj u opsegu [1-15].");
            }
        }


        //Metoda koja iscrtava liniju koja predstavlja vezu izmedju 2 rutera
        private void ConnectRouters(double x1, double y1, double x2, double y2, Line myLine)
        {

            //Koordinate početka i kraja linije se pomeraju za 25 u odnosu na koordinate 
            //ikonice rutera da bi se postigao lepši vizuelni prikaz
            x1 += 25;
            y1 += 25;
            x2 += 25;
            y2 += 25;
           
            myLine.Stroke = System.Windows.Media.Brushes.Black;

            myLine.X1 = x1;
            myLine.X2 = x2;
            myLine.Y1 = y1;
            myLine.Y2 = y2;

            myLine.StrokeThickness = 3;

            canvas.Children.Add(myLine);
        }

        //Metoda koja povezuje rutere logički, generiše podatke koji će kasnije biti
        //izvezeni u konfiguracioni fajl config.txt
        private void Connect_routers_logicaly(Router r1, Router r2, Line myLine, int link_cost)
        {
            string own_ip_addr = (r1.Router_id).ToString() + ".1." + (r1.Interfaces.Count + 1).ToString() + ".1";
            string nbr_ip_addr = (r2.Router_id).ToString() + ".1." + (r2.Interfaces.Count + 1).ToString() + ".1";
            r1.add_interface(r1.Router_id * 10 + 1 + r1.Interfaces.Count, own_ip_addr, nbr_ip_addr, r2.Router_id.ToString(), 1, myLine, link_cost);
            r2.add_interface(r2.Router_id * 10 + 1 + r2.Interfaces.Count, nbr_ip_addr, own_ip_addr, r1.Router_id.ToString(), 1, myLine, link_cost);
        }

        //Metoda koja prikazuje podatke o ruteru, uključujući i njegovu poslednju verziju
        //tabele rutiranja
        private void ViewRouterInfo(Router r)
        {
            router_info_tb.Text = "Podaci o ruteru:";
            router_info_tb.Text += r.RouterFormat();
            router_info_tb.Text += "\n\nTabela rutiranja:\n\n";
            

            string router_filename = r.Router_id.ToString() + ".txt";
            try
            {
                StreamReader sr = new StreamReader("..\\Debug\\" + router_filename);

                if (simulation_done && sr != null)
                {

                    string line = "";
                    string tabela = "";
                    int vreme_iteracije;
                    while (line != null)
                    {
                        line = sr.ReadLine();
                        //kada god se naidje na broj(simulaciono vreme)
                        //tabela se ucitava izpocetka time se dobija
                        //poslednja verzija tabele koja je u fajlu
                        if (int.TryParse(line, out vreme_iteracije))
                        {
                            tabela = "";
                        }
                        else
                        {
                            tabela += line + "\n";
                        }
                    }
                    router_info_tb.Text += tabela;
                    sr.Close();
                }
                else
                {
                    router_info_tb.Text += "Tabela rutiranja još uvek nije formirana. Potrebno je pokrenuti simulaciju.";
                }    
            }
            catch
            {
                router_info_tb.Text += "Tabela rutiranja još uvek nije formirana. Potrebno je pokrenuti simulaciju.";
            }

        }

        //Metoda kojom se započinje simulacija
        private void StartSimulation(object sender, RoutedEventArgs e)
        {

            //Ukoliko su adrese ispravne(postojeće), započinje se simulacija
            if (ValidAddr())
            {
                //sve putanje i ruteri se ponovo boje u crnu boju
                //za slučaj da je simulacija ranije pokretana
                ResetColor();

                simulation_done = true;

                //generisanje konfiguracionog fajla na osnovu slike
                GenerateConfig();


                //postavljanje početne i krannje adrese putanje
                SetAddr();

                //pronalazenje relativne putanje izvrsnog simulacionog fajla
                DirectoryInfo currentDirectory = new DirectoryInfo(System.IO.Directory.GetCurrentDirectory());
                DirectoryInfo rootDirectory = currentDirectory.Parent.Parent.Parent.Parent.Parent;

                //pokretanje simulacionog procesa
                Process.Start(rootDirectory.FullName + "\\Projekat\\Debug\\Projekat.exe");

                //cekanje dok se simulacija ne zavrsi
                while (Process.GetProcessesByName("Projekat").Length > 0)
                {
                    Thread.Sleep(100);
                }

                //citanje rezultata simulacije iza fajla packet_ath.txt
                string path_str = "";
                try 
                { 
                    using (var reader = new StreamReader("Projekat_UI\\..\\packet_path.txt"))
                    {
                        path_str = reader.ReadToEnd();
                    }
                   
                    if(!path_str.Contains("Message received!"))
                    {
                        //Da li je putanja zavrsena, ako postoji?
                        MessageBox.Show("Izabranu putanju je nemoguće dostići.");
                        path_str = "";
                    }
                }
                catch
                {
                    //Da li putanja postoji?
                    MessageBox.Show("Izabranu putanju je nemoguće dostići.");
                }

                simulation_tb.Text = "Rezultati simulacije:\n\n";
                simulation_tb.Text += path_str;

                if (path_str != "")
                {
                    DrawPath(path_str);
                }
               
            }
            else
            {
                MessageBox.Show("Izabrane su neodgovarajuće Ip adrese uređaja.");
            }
        }

        private void DrawPath(string path_str)
        {
            canvas.Children.Remove(message_img);
            //učitavanje redosleda id-eva rutera kroz koje je paket prosao
            //da bi se na osnovu tog redosleda iscrtala putanja prolaska
            string[] rows = path_str.Split('\n');
            List<int> ruteri = new List<int>();
            points = new List<Point>();

            Regex re = new Regex(@"\d+");

            foreach (string s in rows)
            {
                //trazi se rec Router posle koje sledi id tog rutera
                if (s.Contains("Router"))
                {
                    Match m = re.Match(s);
                    ruteri.Add(Convert.ToInt32(m.Value));
                }
            }

            List<Router> routers_path = new List<Router>();

            foreach (int id_rutera in ruteri)
            {
                foreach (Router r in routers)
                {
                    //traze se ruteri sa odgovarajucim id-evima da bi se obojili u crveno
                    //usput se njihov redosled čuva i u listi rutera routers_path
                    if (r.Router_id == id_rutera)
                    {
                        BitmapImage src = new BitmapImage();
                        src.BeginInit();
                        src.UriSource = new Uri("./Resources/red_router96.png", UriKind.Relative);
                        src.EndInit();
                        r.Img.Source = src;
                        r.Img.Stretch = Stretch.None;
                        routers_path.Add(r);

                        Point p = new Point(r.X, r.Y);
                        points.Add(p);
                    }
                }
            }

            //generiše se ikonica poruke na poziciji rutera sa kog se prvo prosledjuje
            BitmapImage src_m = new BitmapImage();
            src_m.BeginInit();
            src_m.UriSource = new Uri("./Resources/email96.png", UriKind.Relative);
            src_m.EndInit();
            message_img = new Image();
            message_img.Source = src_m;
            message_img.Stretch = Stretch.None;
            Canvas.SetLeft(message_img, routers_path[0].X);
            Canvas.SetTop(message_img, routers_path[0].Y);
            canvas.Children.Add(message_img);
            canvas.InvalidateArrange();

            //Za svaki ruter se trazi ruter koji sledi za njim u putanji
            //i boji se linija koja spaja ova dva rutera
            for (int i = 0; i < routers_path.Count - 1; i++)
            {
                foreach (Interface inter in routers_path[i].Interfaces)
                {
                    if (String.Equals(inter.Nbr_router_id, routers_path[i + 1].Router_id.ToString()))
                    {                       
                        inter.Line.Stroke = System.Windows.Media.Brushes.Red;
                    }
                }
            }

            MoveImage(message_img);
        }

        //Metoda koja pomera ikonicu poruka po kanvasu
        public async void MoveImage(Image message_img)
        {
            simulation_btn.IsEnabled = false;
            for (int i = 0; i < points.Count - 1; i++)
            {
                double chgX = points[i].X + 12;
                double chgY = points[i].Y + 10;

                for (int j = 0; j < 100; j++)
                {
                    chgX = chgX - (points[i].X - points[i+1].X) / 100;
                    chgY = chgY - (points[i].Y - points[i+1].Y) / 100;
                    Canvas.SetTop(message_img, chgY);
                    Canvas.SetLeft(message_img, chgX);
                    Canvas.SetLeft(message_img, chgX);
                    await Task.Delay(10);
                }
            }
            simulation_btn.IsEnabled = true;
        }

        //Prosledjivanje unete izvorišne i odredišne adrese
        //u fajl src_dest.txt koji će potom koristiti simulaciona C aplikacija
        private void SetAddr()
        {
            string src_dest_str = source_tb.Text + "\n" + destination_tb.Text;
            using (StreamWriter writetext = new StreamWriter("src_dest.txt"))
            {
                writetext.Write(src_dest_str);
            }
        }

        //Dodavanje uredjaja za izabrani ruter
        private void AddDevice(object sender, RoutedEventArgs e)
        {
            current_router.AddDevice();
            ViewRouterInfo(current_router);
        }

        //restovanje boja putanja i ikonica rutera
        private void ResetColor()
        {
            foreach (Router r in routers)
            {
                BitmapImage src = new BitmapImage();
                src.BeginInit();
                src.UriSource = new Uri("./Resources/router96.png", UriKind.Relative);
                src.EndInit();
                r.Img.Source = src;
                r.Img.Stretch = Stretch.None;
                foreach (Interface inter in r.Interfaces)
                {
                    if (inter.Type == 1)
                    {
                        inter.Line.Stroke = System.Windows.Media.Brushes.Black;
                    }
                }
            }

        }

        //Generisanje konfiguracionog fajla config.txt na osnovu dijagrama koji je korisnik napravio
        private void GenerateConfig()
        {
            string str = "";
            for (int i = 0; i < routers.Count; i++)
            {
                if (i + 1 < routers.Count)
                {
                    str += routers[i].ToString() + "\n";
                }
                else
                {
                    str += routers[i].ToString();
                }
            }

            using (StreamWriter writetext = new StreamWriter("config.txt"))
            {
                writetext.Write(str);
            }
        }

        private bool ValidAddr()
        {
            bool addr1_valid = false;
            bool addr2_valid = false;

            //Ispitivanje da li se zadate adrese uopšte pojavljuju među uređajima
            //priključenim na rutere zadate mreže
            foreach (Router r in routers)
            {
                foreach (Device dev in r.Devices)
                {
                    if (dev.Ip_addr.Equals(source_tb.Text) && !addr1_valid)
                    {
                        addr1_valid = true;
                    }
                    if (dev.Ip_addr.Equals(destination_tb.Text) && !addr2_valid)
                    {
                        addr2_valid = true;
                    }
                }
            }

            return addr1_valid && addr2_valid;
        }

        private void radio_rip_Checked(object sender, RoutedEventArgs e)
        {
            radio_ospf.IsEnabled = false;
        }

        private void radio_ospf_Checked(object sender, RoutedEventArgs e)
        {
            radio_rip.IsEnabled = false;
        }
    }
}
