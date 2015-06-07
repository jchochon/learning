using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ia_taquin.Model;

namespace ia_taquin
{
    class Program
    {
        
        private static int _step = 0;
        private const Heuristic Heuristic = Model.Heuristic.ManathanDistance;
        static readonly Stopwatch StopWatch = new Stopwatch();

        // Taquin 3x3
        /*var start = new State { Matrix = new int[3, 3] { { 2, 8, 3 }, { 1, 6, 4 }, { 7, 0, 5 } }, G = 0, H = 0 };
        var end = new State { Matrix = new int[3, 3] { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } } };*/
        
        // Taquin 4x4
        static State start = new State { Matrix = new int[4, 4] { { 2,5,4,8 }, { 1,10,6,3 }, { 14,11,15,13 }, { 9,7,0,12 } }, G = 0, H = 0 };
        static State end = new State { Matrix = new int[4, 4] { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 0 } } };
        static List<State> openedStates = new List<State> { start };
        static List<State> closedStates = new List<State>();
        
        static void Main(string[] args)
        {
            // Set des param du programme
            
            Console.WriteLine("Etat de départ: ");
            start.PrintState();

            // Execution chronométrée de l'algo
            StopWatch.Start();
            while (openedStates.Count > 0)
            {
                // Rechercher le noeud ouvert de f le plus faible
                int min = openedStates.Min(x => x.F);
                var state = openedStates.Where(s => s.F == min).First();
                
                openedStates.Remove(state);
                closedStates.Add(state);

                // MAJ du lien de parenté, récupération des noeuds fils et traitement
                foreach (var child in state.GenerateNextStates())
                {
                    // On suit le nombre d'étapes
                    _step++;
                    
                    // On détermine le H de chaque enfant
                    child.SetDistanceFromEnd(end, Heuristic);
                    
                   /* if (step % 100 == 0)
                    {
                        Console.WriteLine("Etape {0} avec searchdepth de {1} et une heuristique de {2}", step, child.G, child.H);
                        Console.WriteLine("Nombre de sommets explorés: {0}", openedStates.Count + closedStates.Count);
                    }*/

                    // Si l'enfant est un état résultat, alors succès
                    if (child.IsEqual(end))
                    {
                        // Arrêt du chronomètre
                        StopWatch.Stop();
                        // Affichage du resultat
                        PrintResult(child);
                        System.Environment.Exit(0);
                    }
                    

                    // Recherche d'une occurence similaire à l'enfant dans ouvert et fermé
                    var foundInOpenedStates = openedStates.Where(openedState => openedState.IsEqual(child)).FirstOrDefault() != null;
                    var foundInClosedStates = closedStates.Where(closedState => closedState.IsEqual(child)).FirstOrDefault() != null;
                    
                   

                    if (!foundInOpenedStates && !foundInClosedStates)
                    {
                        openedStates.Add(child);
                    }
                    else if (foundInOpenedStates)
                    {
                        var k = openedStates.First(o => o.IsEqual(child));
                        if (!(k.F < child.F))
                        {
                            openedStates.Remove(k);
                            openedStates.Add(child);
                        }
                    }
                    else if (foundInClosedStates)
                    {
                        var k = closedStates.First(c => c.IsEqual(child));
                        var toto = closedStates.Contains(k);
                        if (!(k.F < child.F))
                        {
                            closedStates.Remove(k);
                            openedStates.Add(child);
                        }
                    }
                }
               
            }
        }

        static void PrintResult(State state)
        {
            // Get the elapsed time as a TimeSpan value.
            TimeSpan ts = StopWatch.Elapsed;

            // Format and display the TimeSpan value.
            string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                ts.Hours, ts.Minutes, ts.Seconds,
                ts.Milliseconds);



            Console.WriteLine("Premier état solution d'arrivé trouvé:");
            state.PrintState();
            Console.WriteLine("Informations complémentaire sur l'état trouvé________________");
            Console.WriteLine("Pour l'heuristique:              {0}", Heuristic);
            Console.WriteLine("Temps de recherche:              {0}", elapsedTime);
            Console.WriteLine("Itération boucle principale:     {0}", _step);
            Console.WriteLine("Nombre de noeud ouverts ");
            Console.WriteLine("après execution de l'algorithme: {0}", openedStates.Count);
            Console.WriteLine("Nombre de noeud fermés ");
            Console.WriteLine("après execution de l'algorithme: {0}", closedStates.Count);
            Console.WriteLine("Pour départ: ");
            start.PrintState();
            
            Console.ReadKey();
        }
    }


}
