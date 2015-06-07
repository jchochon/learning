using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace ia_taquin.Model
{
    public class State
    {
        private List<State> _nextStates = new List<State>();
        private int _g = 0;

        public int F
        {
            get { return G + H; }
        }

        public int G
        {
            get { return _g; }
            set { _g = value; }
        }

        public int H { get; set; }

        public int[,] Matrix { get; set; }

        public List<State> NextStates
        {
            get { return _nextStates; }
            set { _nextStates = value; }
        }

        public State PreviousState { get; set; }

        public List<State> GenerateNextStates()
        {
            // Recuperation des la ligne et colonne ou se trouve 0
            var row = getRowIndexOfZero();
            var column = getColumnIndexOfZero();

            return GetNextStates(row, column);
        }

        private List<State> GetNextStates(int row, int col)
        {
            var nextStates = new List<State>();

            // Recuperation des positions qui sont swapable avec 0
            List<List<int>> swapablePositions = getSwapablePositions(row, col);
            foreach (var position in swapablePositions)
            {
                var state = new State()
                {
                    Matrix = (int[,])this.Matrix.Clone(),
                    G = this.G + 1,
                    PreviousState = this
                };
                // On swap les valeur du nouvel état
                var tmp = state.Matrix[position[0], position[1]];
                state.Matrix[position[0], position[1]] = 0;
                state.Matrix[row, col] = tmp;
                
                nextStates.Add(state);
            }
            NextStates = nextStates;

            return nextStates;
        }

        List<List<int>> getSwapablePositions(int row, int column)
        {
            var swapablePositions = new List<List<int>>();
            // On check si il existe une case à gauche de 0
            if (column - 1 > -1) swapablePositions.Add(new List<int>(){ row, column - 1 });
            // On check si il existe une case au dessus de 0
            if (row - 1 > -1) swapablePositions.Add(new List<int>() { row - 1, column });
            // On check si il existe une case à droite de 0
            if (column + 1 < Matrix.GetLength(1)) swapablePositions.Add(new List<int>() { row, column + 1 });
            // On check si il existe une case en dessous de 0
            if (row + 1 < Matrix.GetLength(0)) swapablePositions.Add(new List<int>() { row + 1, column });
            

            return swapablePositions;
        }

        private int getColumnIndexOfZero()
        {
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    if (Matrix[i, j] == 0) return j;
                }
            }
            throw new RowNotFoundException();
        }

        private int getRowIndexOfZero()
        {
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    if (Matrix[i, j] == 0) return i;
                }
            }
            throw new ColumnNotFoundException();
        }

        public void PrintState()
        {
            Console.WriteLine(String.Format("G: {0}", G));
            Console.WriteLine(String.Format("H: {0}", H));
            Console.WriteLine(String.Format("F: {0}", F));
            Console.WriteLine("State grid: ");
            for (int i = 0; i < Matrix.GetLength(0); i++)
            {
                for (int j = 0; j < Matrix.GetLength(1); j++)
                {
                    // Powered by Jeremy Chochon
                    Console.Write(String.Format("{0} ", Matrix[i, j]));
                }
                Console.Write(Environment.NewLine);
            }
        }

        int GetDistanceFromEndWithDefault(State endState)
        {
            // Check que les matrices sont de même dimension
            if (Matrix.GetLength(0) != endState.Matrix.GetLength(0) || Matrix.GetLength(1) != endState.Matrix.GetLength(1))
                throw new NotEqualsDimensionsMatrixException();

            int diff = 0;
            for (var i = 0; i < Matrix.GetLength(0); i++)
            {
                for (var j = 0; j < Matrix.GetLength(1); j++)
                {
                    if (Matrix[i, j] != endState.Matrix[i, j])
                        diff += 1;
                }
            }

            return diff;
        }

        int GetDistanceFromEndWithManathanDistance(State state)
        {
            int diff = 0;
            for (var i = 0; i < Matrix.GetLength(0); i++)
            {
                for (var j = 0; j < Matrix.GetLength(1); j++)
                {
                    // Récupération de la position de la tuile cible
                    int[] position = null;
                    try
                    {
                        position = FindPositionOf(state, Matrix[i, j]);
                    }
                    catch (NoPositionFoundException)
                    {
                        throw;
                    }
                    int XA = j;
                    int YA = i;
                    int XB = position[1];
                    int YB = position[0];
                    diff += Math.Abs(XB - XA) + Math.Abs(YB - YA);
                }
            }
            return diff;
        }

        int[] FindPositionOf(State state, int value)
        {
            for (var i = 0; i < Matrix.GetLength(0); i++)
            {
                for (var j = 0; j < Matrix.GetLength(1); j++)
                {
                    if (state.Matrix[i, j] == value)
                        return new int[]{ i, j};
                }
            }
            throw new NoPositionFoundException();
        }

        public void SetDistanceFromEnd(State state, Heuristic heuristic = Heuristic.Default)
        {
            if (heuristic == Heuristic.ManathanDistance)
                H = GetDistanceFromEndWithManathanDistance(state);
            else
                H = GetDistanceFromEndWithDefault(state);
        }

        public bool IsEqual(State state)
        {
            if (GetDistanceFromEndWithDefault(state) == 0)
                return true;
            return false;
        }
    }

    public enum Heuristic { Default, ManathanDistance };

    class RowNotFoundException : Exception
    {
         
    }

    class ColumnNotFoundException : Exception
    {

    }

    class NotEqualsDimensionsMatrixException : Exception
    {
         
    }

    class NoPositionFoundException : Exception
    {
        
    }
}
