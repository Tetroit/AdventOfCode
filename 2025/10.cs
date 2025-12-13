using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;

namespace MathTools 
{
    class Machine
    {
        public int size = 0;
        public List<bool> setupPattern = new List<bool>();
        public List<Vector> buttons = new List<Vector>();
        public Vector joltage = new Vector();

        public Vector LaunchSolvingSequence(Vector cache = null)
        {
            Vector[] vecs = buttons.ToArray();
            Vector res = new Vector();
            Matrix mat = Matrix.FromColumns(vecs);
            Vector[] kernels = mat.KernelColumn();

            res = cache ?? mat.SolveColumn(joltage);

            if (kernels.Length > 0)
            {
                Console.WriteLine("Target:");
                Console.WriteLine(joltage);
                Console.WriteLine("Matrix:");
                Console.WriteLine(mat);
                Console.WriteLine("Kernels:");
                foreach (var kernel in kernels)
                {
                    Console.WriteLine(kernel + " cost: " + Methods.GetCost(kernel));
                }
                Console.WriteLine("Result:");
                Console.WriteLine(res + " cost: " + Methods.GetCost(res));
                Console.WriteLine("-----------------y to confirm------------------");

                string input = Console.ReadLine();

                while (input != "y")
                {
                    string[] words = input.Split(' ');
                    if (int.TryParse(words[0], out int kernelID) &&
                        int.TryParse(words[1], out int kernelMult) &&
                        kernelID >= 0 && kernelID < kernels.Length)
                    {
                        res += kernels[kernelID] * kernelMult;
                        Console.WriteLine(res + " cost: " + Methods.GetCost(res));
                    }
                    input = Console.ReadLine();

                }
            }
            Console.Clear();
            return res;
        }
    }
    static class Methods
    {
        public static double GetCost(Vector vec)
        {
            double cost = 0;
            for (int i = 0; i < vec.dimension; i++)
            {
                cost += vec[i];
            }
            return cost;
        }
        public static bool IntEqual(Vector vec1, Vector vec2)
        {
            if (vec1.dimension != vec2.dimension)
                return false;
            for (int i = 0; i < vec1.dimension; i++)
            {
                if (Math.Round(vec1[i] - vec2[i]) != 0)
                    return false;
            }
            return true;
        }
    }
	class MainClass {
        public static void Main(string[] args) {
            string[] lines = File.ReadAllLines("input.txt");
            List<Machine> machines = new List<Machine>();

            for (int i=0; i<lines.Length; i++)
            {
                Machine machine = new Machine();
                machines.Add(machine);
                string line = lines[i];
                for (int j = 0; j < line.Length; j++)
                {
                    if (line[j] == '[')
                    {
                        string patternRaw = line.Substring(j + 1, line.IndexOf(']', j) - j - 1);
                        foreach (char ch in patternRaw)
                        {
                            if (ch == '#')
                                machine.setupPattern.Add(true);
                            if (ch == '.')
                                machine.setupPattern.Add(false);
                        }
                        machine.size = patternRaw.Length;
                    }
                    if (line[j] == '(')
                    {
                        machine.buttons.Add(new Vector(machine.size));
                        var patternVec = machine.buttons[machine.buttons.Count - 1];
                        for (int k=0; k<machine.size; k++)
                        {
                            patternVec[k] = 0;
                        }
                        string patternRaw = line.Substring(j + 1, line.IndexOf(')', j) - j - 1);
                        string[] patternIds = patternRaw.Split(',');
                        foreach (var str in patternIds)
                        {
                            patternVec[int.Parse(str)] = 1;
                        }
                    }
                    if (line[j] == '{')
                    {
                        string patternRaw = line.Substring(j + 1, line.IndexOf('}', j) - j - 1);
                        string[] patternValues = patternRaw.Split(',');
                        machine.joltage = new Vector(machine.size);
                        for (int k = 0; k < machine.size; k++)
                        {
                            machine.joltage[k] = int.Parse(patternValues[k]);
                        }
                    }
                }
                machine.buttons.Sort((a, b) => - a.Length().CompareTo(b.Length()));
            }
            
            int totalScore = 0;
            Console.WriteLine("choose action:");
            Console.WriteLine("w to write a new file");
            string action = Console.ReadLine();
            bool shouldRecalculate = false;

            List<int> machineScores = new List<int>();
            switch (action)
            {
                case "w":
                    shouldRecalculate = true;
                    break;
                default:
                    break;
            }
            string[] cache = new string[0];

            if (shouldRecalculate)
            {
                File.WriteAllText("output.txt", string.Empty);
            }
            else
            {
                cache = File.ReadAllLines("output.txt");
            }

            for (int machineID =0; machineID < machines.Count; machineID++)
            {
                Machine machine = machines[machineID];
                int score = 0;
                Vector res = null;
                if (shouldRecalculate)
                {
                    res = machine.LaunchSolvingSequence();
                }
                else
                {
                    string resLine = cache[machineID];
                    string[] resStr = resLine.Substring(1, resLine.IndexOf(')') - 1).Split(' ');
                    res = new Vector(machine.buttons.Count);
                    for (int i = 0; i < machine.buttons.Count; i++)
                    {
                        double value = double.Parse(resStr[i]);
                        res[i] = value;
                    }



                    Vector[] vecs = machine.buttons.ToArray();
                    Matrix mat = Matrix.FromColumns(vecs);
                    Vector test = mat * res;
                    if (!Methods.IntEqual(test, machine.joltage))
                    {
                        Console.WriteLine("MISMATCH\n");
                        Console.WriteLine("Target:");
                        Console.WriteLine(machine.joltage);
                        Console.WriteLine("Test:");
                        Console.WriteLine(test);
                        Console.WriteLine("Matrix:");
                        Console.WriteLine(mat);
                        Console.WriteLine("Result:");
                        Console.WriteLine(res + " cost: " + Methods.GetCost(res));
                        Console.WriteLine("type r to recalculate, otherwise machine will be skipped");
                        action = Console.ReadLine();
                        if (action == "r")
                            res = machine.LaunchSolvingSequence();
                    }
                    else
                    {
                        res = machine.LaunchSolvingSequence(res);
                    }
                }


                List<int> processed = new List<int>();
                string cacheLine = "(";
                for (int i=0; i<res.dimension; i++)
                {
                    processed.Add((int)Math.Round(res[i]));
                    cacheLine += (processed[i]).ToString();
                    if (i != res.dimension - 1)
                    {
                        cacheLine += ' ';
                    }
                }
                cacheLine += ") ";
                score = (int)Math.Round(Methods.GetCost(res));
                machineScores.Add(score);
                cacheLine += score;
                if (shouldRecalculate)
                {
                    File.AppendAllText("output.txt", cacheLine + '\n');
                }
                else
                {
                    cache[machineID] = cacheLine;
                }
                totalScore += score;
            }

            Console.WriteLine(totalScore);
            if (!shouldRecalculate)
            {
                File.WriteAllLines("output.txt", cache);
            }
            foreach (var mScore in machineScores) 
            {
                Console.WriteLine(mScore);
            }

            //16463
            //16510
            //16509
            //16457
            //16431
            //[..##] (1,2) (1,3) (0,2) (0,3) {14,17,27,4}
            //[...#] (0,2) (2) (0,3) (0,1) (3) (1,2) {24,27,21,22}

        }
    }
}
