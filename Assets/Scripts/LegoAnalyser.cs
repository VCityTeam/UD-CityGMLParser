using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class LegoAnalyser : MonoBehaviour
{
    public Vector2Int legoMapSize = new Vector2Int(100, 100);
    public float analyserHeight = 10;
    public float scale = 1;
    public LayerMask collisionMask;
    public bool setGroundAt0;
    public string fileName = "lego map";

    public void ExportLegoMap()
    {
        LegoMap legoMap = ComputeLegoMap();
        File.WriteAllText("Assets/JSON/" + fileName + ".json", JsonUtility.ToJson(legoMap));
    }

    private LegoMap ComputeLegoMap()
    {
        int count = 0;
        LegoMap legoMap = new LegoMap();

        legoMap.mapSize = legoMapSize;
        legoMap.mapScale = scale;
        legoMap.columns = new List<Column>();

        Vector3 rayOrigin = transform.position - new Vector3(
            legoMapSize.x * scale / 2f,
            0,
            legoMapSize.y * scale / 2f
        );

        float minHeight = analyserHeight;
        for (int x = 0; x < legoMapSize.x; x++)
        {
            for (int z = 0; z < legoMapSize.y; z++)
            {
                Vector3 rayPosition = rayOrigin + new Vector3(
                    (x + 0.5f) * scale,
                    0,
                    (z + 0.5f) * scale
                );

                RaycastHit hit;
                float height = 0;
                Column.Type type = Column.Type.Default;
                if (Physics.Raycast(rayPosition, Vector3.down, out hit, analyserHeight, collisionMask))
                {
                    height = analyserHeight - hit.distance;
                    if (height < minHeight) minHeight = height;

                    if (hit.collider.tag == "Ground") type = Column.Type.Ground;
                    if (hit.collider.tag == "Building") type = Column.Type.Building;
                }
                Column column = new Column(height, type);
                legoMap.columns.Add(column);
            }
        }
        
        
        for (int i = 0; i < legoMap.columns.Count; i++)
        {
            if (setGroundAt0)
            {
                if (legoMap.columns[i].height < minHeight)
                    legoMap.columns[i].height = 0;
                else
                    legoMap.columns[i].height -= minHeight;
            }

            // Normalisation en Lego et Count
            legoMap.columns[i].height = legoMap.columns[i].height / (1.2f * scale);
            legoMap.columns[i].height = Mathf.Round(legoMap.columns[i].height);
            count += (int)legoMap.columns[i].height;
            legoMap.columns[i].height = legoMap.columns[i].height * 1.2f;
        }

        legoMap.legoCount = count;

        return legoMap;
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = new Color(1, 0, 0, 0.5f);
        Gizmos.DrawCube(
            transform.position - new Vector3(0, analyserHeight / 2f, 0),
            new Vector3(legoMapSize.x * scale, analyserHeight, legoMapSize.y * scale)
        );
    }
}
