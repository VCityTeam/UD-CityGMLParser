using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(RayCast))]
public class RayCastEditor : Editor
{ 
     public override void OnInspectorGUI () 
     {
        DrawDefaultInspector();
        RayCast raycaster = (RayCast)target;
        
        if (GUILayout.Button("Create JSON"))
        {
            raycaster.createJSON();
        }
     }
 }
