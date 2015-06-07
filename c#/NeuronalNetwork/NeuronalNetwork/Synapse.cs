using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuronalNetwork
{
    class Synapse
    {
        public Sensor Source { get; set; }
        public Neuron Target { get; set; }
        public float Weight { get; set; }
    }
}
