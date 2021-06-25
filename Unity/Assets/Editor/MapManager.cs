using UnityEngine;
using UnityEditor;

public class MapManager : EditorWindow
{
    [MenuItem("Window/Map Manager")]
    public static void ShowWindow()
    {
        GetWindow<MapManager>("Map Manager");
    }

    private void OnGUI()
    {
        if (GUILayout.Button("Add Mesh Collider to Map"))
        {
            AddMeshColliderToMap();
        }
    }

    private void AddMeshColliderToMap()
    {
        AddMeshColliderRecursive(GameObject.FindGameObjectWithTag("Map"));
    }

    private void AddMeshColliderRecursive(GameObject obj)
    {
        if (obj == null)
        {
            return;
        }

        if (obj.GetComponent<MeshFilter>() != null && obj.GetComponent<MeshCollider>() == null)
            obj.AddComponent<MeshCollider>();

        foreach (Transform child in obj.transform)
        {
            AddMeshColliderRecursive(child.gameObject);
        }
    }
}
