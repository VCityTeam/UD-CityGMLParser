using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HouseConstructor : MonoBehaviour
{
    public List<Vector3> coordinates;
    public Material mat;
    public Building building;
    private MeshFilter meshFilter;
    public Vector3 cityCenter;

    public void Construct()
    {
        meshFilter = GetComponent<MeshFilter>();
        Mesh msh = new Mesh();
        List<int> triangles = new List<int>();
        List<Vector3> vert = new List<Vector3>();

        float MinCoordY = 500.0f;
        //Recalculate coordinate with the city center 
        for (int i = 0; i < building.points.Count; i++)
        {
            building.points[i] = new Vector3((building.points[i].x - cityCenter.x), building.points[i].y, (building.points[i].z - cityCenter.z));
            //if ()
        }

        //Down vertices
        for (int i = 0; i < building.points.Count; i++)
        {
            vert.Add(new Vector3(building.points[i].x, 150, building.points[i].z));
        }

        //Up vertices
        for (int i = 0; i < building.points.Count; i++)
        {
            vert.Add(new Vector3(building.points[i].x, building.points[i].y, building.points[i].z));
        }

        msh.vertices = vert.ToArray();

        //Down face tesselation
        List<double> testBas = new List<double>();
        int sizeBAs = 0;
        for (int i = 0; i < vert.Count / 2; i++)
        {
            testBas.Add(vert[sizeBAs].x);
            testBas.Add(vert[sizeBAs].z);
            sizeBAs++;
        }
        List<int> tessalationBas = EarcutNet.earcut.Tessellate(testBas.ToArray(), new int[] { });
        triangles.InsertRange(0, tessalationBas);

        //Up face tesselation
        List<double> UpFace = new List<double>();
        int size = building.points.Count * 2;
        for (int i = 0; i < vert.Count / 2; i++)
        {
            UpFace.Add(vert[size - 1].x);
            UpFace.Add(vert[size - 1].z);
            size--;
        }
        List<int> tessalationHaut = EarcutNet.earcut.Tessellate(UpFace.ToArray(), new int[] { });
        for (int i = 0; i < tessalationHaut.Count; i++)
        {
            tessalationHaut[i] = tessalationHaut[i] + building.points.Count;//mise a niveau pour les indices car la tesselation n'a pas tout le tab sinon il ferait une seul face 
        }                                                                   //avec la face du haut et du bas
        triangles.InsertRange(triangles.Count, tessalationHaut);

        //Lateral face
        for (int i = 0; i < building.points.Count - 1; i++)
        {
            triangles.Add(i);
            triangles.Add(i + 1);
            triangles.Add(i + building.points.Count);

            triangles.Add(i + 1);
            triangles.Add(i + building.points.Count + 1);
            triangles.Add(i + building.points.Count);
        }

        //Last face - connect with the fist vertices and the last
        triangles.Add(building.points.Count - 1);
        triangles.Add(0);
        triangles.Add(vert.Count - 1);

        triangles.Add(0);
        triangles.Add(building.points.Count + 2);
        triangles.Add(vert.Count - 1);

        //Add data to the mesh
        msh.triangles = triangles.ToArray();
        GetComponent<MeshRenderer>().material = mat;
        meshFilter.mesh = msh;
    }
}
