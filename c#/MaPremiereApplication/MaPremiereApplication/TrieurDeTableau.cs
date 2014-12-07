using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaPremiereApplication
{
    class TrieurDeTableau
    {
        private delegate void DelegateTri(int[] tableau);

        public void TriAscandant(int [] tableau)
        {
            Array.Sort(tableau);
            foreach (int i in tableau)
            {
                Console.WriteLine(i);
            }
            
        }
        public void TriDescandant(int [] tableau)
        {
            Array.Sort(tableau);
            Array.Reverse(tableau);
            foreach (int i in tableau)
            {
                Console.WriteLine(i);
            }
        }

        public void DemoTri(int[] tableau)
        {
            DelegateTri tri = TriAscandant;
            tri += TriDescandant;
            tri(tableau);
        }

    }
}
