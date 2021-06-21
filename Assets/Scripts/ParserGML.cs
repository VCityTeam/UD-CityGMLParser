using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Linq;
using System.Globalization;

public class ParserGML : MonoBehaviour
{
    public static List<Building> GetBuildingsFromGML(string path)
    {
        List<Building> city = new List<Building>();
        List<string> buildindsPoints = new List<string>();
        int i = 0;
        foreach (string line in File.ReadAllLines(path))
        {
            if (line.Contains("<gml:posList>"))
            {
                
                string lineRemove = line.Remove(line.Length - 15);
                string lineRemoveHeader = lineRemove.Remove(0, 39);
                city.Add(CreateBuilding(lineRemoveHeader)); //create building
                if (i > 10000)
                    return city;
                i++;
            }  
            
        }
        return city;
    }


    private static Building CreateBuilding(string points)
    {

        List<Vector3> coords = new List<Vector3>();
        char[] separators = {' '};
        string[] strValues = points.Split(separators);
        

        List<float> floatValues = new List<float>();
        foreach (string str in strValues){
            floatValues.Add(float.Parse(str, CultureInfo.InvariantCulture.NumberFormat));
        }

        for (int i = 0; i < floatValues.Count; i += 3)
            coords.Add(new Vector3(floatValues[i], floatValues[i + 2], floatValues[i + 1]));

        return new Building(coords);
    }
}
