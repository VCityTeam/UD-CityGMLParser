using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(CreateCity))]
public class CityBuilderEditor : Editor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        if (GUILayout.Button("Build City GML"))
        {
            ((CreateCity)target).BuildCity();
        }

        if (GUILayout.Button("Delete City GML"))
        {
            ((CreateCity)target).DeleteCity();
        }
    }
}
