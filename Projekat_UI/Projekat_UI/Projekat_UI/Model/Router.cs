using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Projekat_UI
{
    class Router
    {
        public double X { get; set; }
        public double Y { get; set; }
        public int Router_id { get; set; }
        public List<Interface> Interfaces { get; set; }
        public List<Device> Devices { get; set; }
        public Image Img { get; set; }       
        public string Routing_table_info { get; set; }
        public Router()
        {
            X = 0;
            Y = 0;
            Router_id = 0;
            Interfaces = new List<Interface>();
            Devices = new List<Device>();
            Img = null;
        }
        public Router(double x, double y, Image img)
        {
            X = x;
            Y = y;
            Router_id = 0;
            Interfaces = new List<Interface>();
            Devices = new List<Device>();
            Img = img;
        }

        public void add_interface(int interface_id, string own_ip_addr, string nbr_ip_addr, string nbr_router_id, short type, Line line, int link_cost)
        {
            Interface i1 = new Interface(interface_id, own_ip_addr, nbr_ip_addr, nbr_router_id, type, line, link_cost);
            Interfaces.Insert(0,i1);
        }

        public override string ToString()
        {
            string str = "router_id=" + Router_id.ToString() + "\n";
            foreach (Interface i in Interfaces)
            {
                str +=  i.Own_ip_addr + "|" + i.Nbr_ip_addr + "|" + i.Nbr_router_id + "|" + i.Interface_id + "|" + i.Type.ToString() + "|" + i.Link_cost + "\n";
            }
            str += "--------------------------\n";
            foreach (Device d in Devices)
            {
                str += d.Ip_addr + "\n";
            }
            str += "##########################";

            return str;
        }

        public string RouterFormat()
        {
            string str = " " + Router_id.ToString() + "\n\n";
            str += "Interfejsi:\n\n";
            foreach (Interface i in Interfaces)
            {
                str += i.Own_ip_addr + "--->" + i.Nbr_ip_addr + "   Id susednog rutera: " + i.Nbr_router_id + "  Id interfejsa: " + i.Interface_id + "  Težina: " + i.Link_cost + "  Tip interfejsa: " + (i.Type == 1 ? "WAN": "LAN") + "\n";
            }
            str += "-------------------------------------------------------------------------------------------------------\n\n";
            str += "Ip adrese priključenih uređaja:\n\n";
            foreach (Device d in Devices)
            {
                str += d.Ip_addr + "\n";
            }
            str += "-------------------------------------------------------------------------------------------------------";

            return str;
        }

        public void AddDevice()
        {
            Device newDevice = new Device(Router_id.ToString() + ".1.1." + (Devices.Count + 2).ToString());
            this.Devices.Add(newDevice);
        }
    }
}
