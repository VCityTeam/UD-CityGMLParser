using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(VoxelGenerator))]
public class VoxelGeneratorEditor : Editor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        VoxelGenerator voxelGenerator = (VoxelGenerator)target;

        if (GUILayout.Button("Build Mesh"))
        {
            voxelGenerator.BuildMesh();
        }
    }
}