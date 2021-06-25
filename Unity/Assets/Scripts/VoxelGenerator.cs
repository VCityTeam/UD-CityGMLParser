using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class VoxelGenerator : MonoBehaviour
{
    public TextAsset jsonFile;
    public Material material;

    MeshFilter meshFilter;
    MeshRenderer meshRenderer;
    MeshColumn[,] meshColumns;

    private void Awake()
    {
        meshFilter = GetComponent<MeshFilter>();
        meshRenderer = GetComponent<MeshRenderer>();
    }

    public void BuildMesh()
    {
        meshFilter = GetComponent<MeshFilter>();
        meshRenderer = GetComponent<MeshRenderer>();

        meshRenderer.material = material;

        Mesh voxelMesh = new Mesh();
        voxelMesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;
        meshColumns = CreateVoxelColumnsFromJson(jsonFile.text);
        voxelMesh.vertices = GetVertices(meshColumns);
        voxelMesh.triangles = GetTriangles(meshColumns);
        voxelMesh.uv = GetUV(meshColumns);
        voxelMesh.RecalculateNormals();
        voxelMesh.RecalculateBounds();
        voxelMesh.RecalculateTangents();

        meshFilter.mesh = voxelMesh;
    }

    private static MeshColumn[,] CreateVoxelColumnsFromJson(string json)
    {
        LegoMap map = JsonUtility.FromJson<LegoMap>(json);
        MeshColumn[,] meshColumns = new MeshColumn[map.mapSize.x, map.mapSize.y];

        for (int x = 0; x < meshColumns.GetLength(0); x++)
        {
            for (int y = 0; y < meshColumns.GetLength(1); y++)
            {
                meshColumns[x, y] = new MeshColumn(
                    x * meshColumns.GetLength(1) + y,
                    new Vector2(x, y),
                    map.columns[x * meshColumns.GetLength(1) + y].height,
                    map.columns[x * meshColumns.GetLength(1) + y].type
                    );
            }
        }
        return meshColumns;
    }

    private static Vector3[] GetVertices(MeshColumn[,] meshColumns)
    {
        Vector3[] vertices = new Vector3[meshColumns.Length * 12];
        int sideOffset = meshColumns.Length * 4;
        foreach (MeshColumn meshColumn in meshColumns)
        {
            for (int i = 0; i < 4; i++)
            {
                vertices[meshColumn.id * 4 + i] = meshColumn.vertices[i];
                vertices[sideOffset + meshColumn.id * 8 + i * 2] = meshColumn.vertices[4 + i * 2];
                vertices[sideOffset + meshColumn.id * 8 + i * 2 + 1] = meshColumn.vertices[4 + i * 2 + 1];
            }
        }

        return vertices;
    }

    private static int[] GetTriangles(MeshColumn[,] meshColumns)
    {
        int topCount = meshColumns.Length * 2;
        int sideCount = (4 * meshColumns.Length - 2 * (meshColumns.GetLength(0) + meshColumns.GetLength(1)));

        int sideId = topCount * 3;
        int sideOffset = meshColumns.Length * 4;

        int[] triangles = new int[3 * (topCount + sideCount)];

        for (int x = 0; x < meshColumns.GetLength(0); x++)
        {
            for (int y = 0; y < meshColumns.GetLength(1); y++)
            {
                MeshColumn meshColumn = meshColumns[x, y];
                int offset = meshColumn.id * 4;
                // Top mesh
                triangles[meshColumn.id * 6] = offset;
                triangles[meshColumn.id * 6 + 1] = offset + 1;
                triangles[meshColumn.id * 6 + 2] = offset + 2;
                triangles[meshColumn.id * 6 + 3] = offset;
                triangles[meshColumn.id * 6 + 4] = offset + 2;
                triangles[meshColumn.id * 6 + 5] = offset + 3;

                // Side mesh
                if (x != 0)
                {
                    if (meshColumns[x, y].GetHeight() > meshColumns[x - 1, y].GetHeight())
                    {
                        triangles[sideId] = sideOffset + meshColumns[x - 1, y].id * 8 + 5;
                        triangles[sideId + 1] = sideOffset + meshColumns[x, y].id * 8 + 2;
                        triangles[sideId + 2] = sideOffset + meshColumns[x, y].id * 8 + 1;
                        triangles[sideId + 3] = sideOffset + meshColumns[x - 1, y].id * 8 + 5;
                        triangles[sideId + 4] = sideOffset + meshColumns[x, y].id * 8 + 1;
                        triangles[sideId + 5] = sideOffset + meshColumns[x - 1, y].id * 8 + 6;
                    }
                    else
                    {
                        triangles[sideId] = sideOffset + meshColumns[x, y].id * 8 + 1;
                        triangles[sideId + 1] = sideOffset + meshColumns[x - 1, y].id * 8 + 6;
                        triangles[sideId + 2] = sideOffset + meshColumns[x - 1, y].id * 8 + 5;
                        triangles[sideId + 3] = sideOffset + meshColumns[x, y].id * 8 + 1;
                        triangles[sideId + 4] = sideOffset + meshColumns[x - 1, y].id * 8 + 5;
                        triangles[sideId + 5] = sideOffset + meshColumns[x, y].id * 8 + 2;
                    }
                    sideId += 6;
                }
                if (y != 0)
                {
                    if (meshColumns[x, y].GetHeight() > meshColumns[x, y - 1].GetHeight())
                    {
                        triangles[sideId] = sideOffset + meshColumns[x, y - 1].id * 8 + 3;
                        triangles[sideId + 1] = sideOffset + meshColumns[x, y].id * 8;
                        triangles[sideId + 2] = sideOffset + meshColumns[x, y].id * 8 + 7;
                        triangles[sideId + 3] = sideOffset + meshColumns[x, y - 1].id * 8 + 3;
                        triangles[sideId + 4] = sideOffset + meshColumns[x, y].id * 8 + 7;
                        triangles[sideId + 5] = sideOffset + meshColumns[x, y - 1].id * 8 + 4;
                    }
                    else
                    {
                        triangles[sideId] = sideOffset + meshColumns[x, y].id * 8 + 7;
                        triangles[sideId + 1] = sideOffset + meshColumns[x, y - 1].id * 8 + 4;
                        triangles[sideId + 2] = sideOffset + meshColumns[x, y - 1].id * 8 + 3;
                        triangles[sideId + 3] = sideOffset + meshColumns[x, y].id * 8 + 7;
                        triangles[sideId + 4] = sideOffset + meshColumns[x, y - 1].id * 8 + 3;
                        triangles[sideId + 5] = sideOffset + meshColumns[x, y].id * 8;
                    }
                    sideId += 6;
                }
            }
        }

        return triangles;
    }

    private static Vector2[] GetUV(MeshColumn[,] meshColumns)
    {
        Vector2[] uv = new Vector2[meshColumns.Length * 12];
        Dictionary<Column.Type, Vector2[]> uvDictionary = GetUVDictionary();
        int sideOffset = meshColumns.Length * 4;
        Vector2[] localUV;

        for (int x = 0; x < meshColumns.GetLength(0); x++)
        {
            for (int y = 0; y < meshColumns.GetLength(1); y++)
            {

                MeshColumn meshColumn = meshColumns[x, y];
                if (uvDictionary.ContainsKey(meshColumn.type))
                    localUV = uvDictionary[meshColumn.type];
                else
                    localUV = uvDictionary[Column.Type.Default];

                // Top mesh
                uv[meshColumn.id * 4] = localUV[0];
                uv[meshColumn.id * 4 + 1] = localUV[1];
                uv[meshColumn.id * 4 + 2] = localUV[2];
                uv[meshColumn.id * 4 + 3] = localUV[3];

                // Side mesh
                if (x != 0)
                {
                    if (meshColumns[x, y].GetHeight() > meshColumns[x - 1, y].GetHeight())
                    {
                        if (uvDictionary.ContainsKey(meshColumn.type))
                            localUV = uvDictionary[meshColumn.type];
                        else
                            localUV = uvDictionary[Column.Type.Default];

                        uv[sideOffset + meshColumns[x - 1, y].id * 8 + 5] = localUV[0];
                        uv[sideOffset + meshColumns[x, y].id * 8 + 2] = localUV[1];
                        uv[sideOffset + meshColumns[x, y].id * 8 + 1] = localUV[2];
                        uv[sideOffset + meshColumns[x - 1, y].id * 8 + 6] = localUV[3];
                    }
                    else
                    {
                        if (uvDictionary.ContainsKey(meshColumns[x - 1, y].type))
                            localUV = uvDictionary[meshColumns[x - 1, y].type];
                        else
                            localUV = uvDictionary[Column.Type.Default];

                        uv[sideOffset + meshColumns[x, y].id * 8 + 1] = localUV[0];
                        uv[sideOffset + meshColumns[x - 1, y].id * 8 + 6] = localUV[1];
                        uv[sideOffset + meshColumns[x - 1, y].id * 8 + 5] = localUV[2];
                        uv[sideOffset + meshColumns[x, y].id * 8 + 2] = localUV[3];
                    }
                }
                if (y != 0)
                {
                    if (meshColumns[x, y].GetHeight() > meshColumns[x, y - 1].GetHeight())
                    {
                        if (uvDictionary.ContainsKey(meshColumn.type))
                            localUV = uvDictionary[meshColumn.type];
                        else
                            localUV = uvDictionary[Column.Type.Default];

                        uv[sideOffset + meshColumns[x, y - 1].id * 8 + 3] = localUV[0];
                        uv[sideOffset + meshColumns[x, y].id * 8] = localUV[1];
                        uv[sideOffset + meshColumns[x, y].id * 8 + 7] = localUV[2];
                        uv[sideOffset + meshColumns[x, y - 1].id * 8 + 4] = localUV[3];
                    }
                    else
                    {
                        if (uvDictionary.ContainsKey(meshColumns[x, y - 1].type))
                            localUV = uvDictionary[meshColumns[x, y - 1].type];
                        else
                            localUV = uvDictionary[Column.Type.Default];

                        uv[sideOffset + meshColumns[x, y].id * 8 + 7] = localUV[0];
                        uv[sideOffset + meshColumns[x, y - 1].id * 8 + 4] = localUV[1];
                        uv[sideOffset + meshColumns[x, y - 1].id * 8 + 3] = localUV[2];
                        uv[sideOffset + meshColumns[x, y].id * 8] = localUV[3];
                    }
                }
            }
        }
        return uv;
    }

    private static Dictionary<Column.Type, Vector2[]> GetUVDictionary()
    {
        Dictionary<Column.Type, Vector2[]> uvDictionary = new Dictionary<Column.Type, Vector2[]>();

        Vector2[] defaultUV = new Vector2[4];
        defaultUV[0] = new Vector2(0, 0);
        defaultUV[1] = new Vector2(0, 0.25f);
        defaultUV[2] = new Vector2(0.25f, 0.25f);
        defaultUV[3] = new Vector2(0.25f, 0);

        Vector2[] groundUV = new Vector2[4];
        groundUV[0] = new Vector2(0.25f, 0);
        groundUV[1] = new Vector2(0.25f, 0.25f);
        groundUV[2] = new Vector2(0.5f, 0.25f);
        groundUV[3] = new Vector2(0.5f, 0);

        Vector2[] buildingUV = new Vector2[4];
        buildingUV[0] = new Vector2(0.5f, 0);
        buildingUV[1] = new Vector2(0.5f, 0.25f);
        buildingUV[2] = new Vector2(0.75f, 0.25f);
        buildingUV[3] = new Vector2(0.75f, 0);

        uvDictionary.Add(Column.Type.Default, defaultUV);
        uvDictionary.Add(Column.Type.Ground, groundUV);
        uvDictionary.Add(Column.Type.Building, buildingUV);

        return uvDictionary;
    }
}

public class MeshColumn
{
    public int id;
    public Vector3[] vertices;
    public Column.Type type;

    public MeshColumn(int id, Vector2 position, float height, Column.Type type)
    {
        this.id = id;
        this.vertices = new Vector3[12];
        this.vertices[0] = new Vector3(position.x, height, position.y);
        this.vertices[1] = new Vector3(position.x, height, position.y + 1f);
        this.vertices[2] = new Vector3(position.x + 1f, height, position.y + 1f);
        this.vertices[3] = new Vector3(position.x + 1f, height, position.y);
        this.vertices[4] = this.vertices[0];
        this.vertices[5] = this.vertices[0];
        this.vertices[6] = this.vertices[1];
        this.vertices[7] = this.vertices[1];
        this.vertices[8] = this.vertices[2];
        this.vertices[9] = this.vertices[2];
        this.vertices[10] = this.vertices[3];
        this.vertices[11] = this.vertices[3];
        this.type = type;
    }

    public void SetHeight(float value)
    {
        for (int i = 0; i < vertices.Length; i++)
        {
            vertices[i].y = value;
        }
    }

    public float GetHeight()
    {
        return vertices[0].y;
    }
}
