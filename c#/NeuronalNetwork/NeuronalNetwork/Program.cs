using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace NeuronalNetwork
{
    class Program
    {
        public static float Epsilon = (float) 0.01;
        static void Main(string[] args)
        {
            // Création du réseau de neurones
            Neuron decisionNeuron = new Neuron {Potential = 0, Threshold = (float) 0.5};
            List<Synapse> synapses = new List<Synapse>();
            for (int i = 0; i < 20; i++)
            {
                synapses.Add(new Synapse
                {
                    Source = new Sensor{ Output = 0},
                    Target = decisionNeuron,
                    Weight = 0
                });
            }
            // Création des lettres A et C
            int[,] A = new int[4, 5]
            {
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 1},
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 1}
            };

            int[,] C = new int[4, 5]
            {
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 0},
                { 1, 0, 0, 0, 0},
                { 1, 1, 1, 1, 1}
            };


            // Début apprentissage
            float n = 0;
            float errorCount = 10;
            float aimedRate = (float) 0.8;
            do
            {
                n++;
                // Apprentissage de A
                FeedNeuronalNetwork(synapses, A);
                decisionNeuron.UpdatePotential(synapses);
                Learn(synapses, 1, decisionNeuron.OutPut);
                // Apprentissage de C
                FeedNeuronalNetwork(synapses, C);
                decisionNeuron.UpdatePotential(synapses);
                Learn(synapses, 0, decisionNeuron.OutPut);

                // Test de A
                FeedNeuronalNetwork(synapses, A);
                decisionNeuron.UpdatePotential(synapses);
                if (!(decisionNeuron.OutPut == 1))
                    errorCount++;
                // Test de C
                FeedNeuronalNetwork(synapses, C);
                decisionNeuron.UpdatePotential(synapses);
                if (!(decisionNeuron.OutPut == 0))
                    errorCount++;
            } while (( (n/errorCount) < aimedRate));



            // Test de la lettre A
            Console.WriteLine("Evaluation de A");
            Dictionary<int, float> aResults = new Dictionary<int, float>();
            float success;
            int k = 0, e = 8;
            while (k < e)
            {
                success = 0;
                for (int i = 0; i < 100*10000; i++)
                {
                    FeedNeuronalNetwork(synapses, TransformArray(A, k));
                    decisionNeuron.UpdatePotential(synapses);
                    if (decisionNeuron.OutPut == 1)
                        success++;
                }
                aResults[k*5] = success/10000;
                k++;
                Console.WriteLine(String.Format("{0}/{1}", k*5, e*5 ));
            }

            // Test de la lettre C
            Console.WriteLine("Evaluation de C");
            Dictionary<int, float> cResults = new Dictionary<int, float>();
            k = 0;
            while (k < e)
            {
                success = 0;
                for (int i = 0; i < 100 * 10000; i++)
                {
                    FeedNeuronalNetwork(synapses, TransformArray(A, k));
                    decisionNeuron.UpdatePotential(synapses);
                    if (decisionNeuron.OutPut == 1)
                        success++;
                }
                cResults[k * 5] = success / 10000;
                k++;
                Console.WriteLine(String.Format("{0}/{1}", k * 5, e * 5));
            }

            foreach (var aResult in aResults)
            {
                Console.WriteLine(String.Format("A:{0}%: {1}", aResult.Key, aResult.Value));
            }
            foreach (var cResult in cResults)
            {
                Console.WriteLine(String.Format("C:{0}%: {1}", cResult.Key, cResult.Value));
            }

            
            Console.ReadKey();

        }

        private static void Learn(List<Synapse> synapses, float expected, float effective)
        {
            foreach (var synapse in synapses)
            {
                var res = synapse.Weight + Epsilon*(expected - effective)*synapse.Source.Output;
                synapse.Weight = res > 1 ? 1 : res;
            }
        }
        

        static void FeedNeuronalNetwork(List<Synapse> links, int[,] input )
        {
            int k = 0;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    links[k++].Source.Output = input[i, j];
                }
            }
        }

        static int[,] TransformArray(int[,] l, int n)
        {
            // n = nombre de case à modif
            int[,] res = (int[,]) l.Clone();
            var rnd = new Random();
            List<int[]> visitedCells = new List<int[]>();
            for (int k = 0; k < n; k++)
            {
                int i;
                int j;
                while (true)
                {
                    i = rnd.Next(4);
                    j = rnd.Next(5);
                    if (visitedCells.FirstOrDefault(s => s[0] == i && s[1] == j) != null) continue;
                    visitedCells.Add(new int[]{ i, j });
                    break;
                }

                res[i,j] = res[i, j] == 0 ? 1 : 0;
            }
            return res;
        }

        static void PrintArray(int[,] array)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    Console.Write(array[i,j] + " ");
                }
                Console.WriteLine();
            }
        }
    }
}
