using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace NeuronalNetwork
{
    class Neuron
    {
        public float Potential { get; set; }
        public float Threshold { get; set; }

        public float OutPut
        {
            get
            {
                return Potential - Threshold < 0 ? 0 : 1;
            }
        }

        public void UpdatePotential(List<Synapse> synapses)
        {
            Potential = synapses.Sum(s => s.Source.Output*s.Weight);
        }

    }
}
