using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Shapes;

namespace Projekat_UI
{
    class Interface
    {
        public int Interface_id { get; set; }
        public string Own_ip_addr { get; set; }
        public string Nbr_ip_addr { get; set; }
        public string Nbr_router_id { get; set; }
        public short Type { get; set; }
        public Line Line { get; set; }
        public int Link_cost { get; set; }
        public Interface()
        {
            Interface_id = 0;
            Own_ip_addr = "";
            Nbr_ip_addr = "";
            Nbr_router_id = "";
            Type = 0;
            Line = null;
            Link_cost = 0;
        }
        public Interface(int interface_id, string own_ip_addr, string nbr_ip_addr, string nbr_router_id, short type, Line line, int link_cost)
        {
            Interface_id = interface_id;
            Own_ip_addr = own_ip_addr;
            Nbr_ip_addr = nbr_ip_addr;
            Nbr_router_id = nbr_router_id;
            Type = type;
            Line = line;
            Link_cost = link_cost;
        }
    }
}
