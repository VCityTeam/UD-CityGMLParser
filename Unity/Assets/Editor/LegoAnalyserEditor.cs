using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(LegoAnalyser))]
public class LegoAnalyserEditor : Editor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        if(GUILayout.Button("Export Lego map")){
            ((LegoAnalyser) target).ExportLegoMap();
        }
    }
}