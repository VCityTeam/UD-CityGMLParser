using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateCity : MonoBehaviour
{
    //Building list of your city
    List<Building> city;

    List<GameObject> buildingInstiate;

    public string path;

    //Prefab of houses (Did we realy need prefabs ?)
    public HouseConstructor houseConstructorPrefab;

    //GML file of your city
    private TextAsset GMLFile;

    public void BuildCity()
    {
        city = ParserGML.GetBuildingsFromGML(path);
        
        //Calculate build center
        Vector3 cityCenter = new Vector3();
        int nb = 0;
        for (int i = 0; i < city.Count; i++)
        {
            for (int j = 0; j < city[i].points.Count; j++)
            {
                cityCenter += city[i].points[j];
                nb++;
            }
        }
        cityCenter /= nb;
        
        //Build houses
        for (int i = 0; i < city.Count; i++)
        {
            HouseConstructor house = Instantiate(houseConstructorPrefab, new Vector3((city[i].points[0].x - cityCenter.x) / 100, 0, (city[i].points[0].y - cityCenter.y) / 100), Quaternion.identity,this.transform);
            buildingInstiate.Add(house.gameObject);
            house.building = city[i];
            house.cityCenter = cityCenter;
            house.Construct();
            house.gameObject.AddComponent<MeshCollider>().convex = true;
        }
    }

    public void DeleteCity()
    {
        foreach(GameObject house in buildingInstiate)
        {
            DestroyImmediate(house);
        }
        buildingInstiate.Clear();
    }
}
