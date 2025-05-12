using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Projekat_UI
{
    class Device
    {
        public string Ip_addr { get; set; }
        public Device()
        {
            Ip_addr = "";
        }

        public Device(string ip_addr)
        {
            Ip_addr = ip_addr;
        }
    }
}
