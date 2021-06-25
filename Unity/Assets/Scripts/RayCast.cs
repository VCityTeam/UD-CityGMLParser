using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class RayCast : MonoBehaviour
{
    public Grid g;
    public string nameMap;
    public LayerMask layerMask; 

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void createJSON()
    {
        LegoMap m = new LegoMap();
        m.mapSize = new Vector2Int((int)g.sizeGrid.x/g.sizeVoxel, (int)g.sizeGrid.z/g.sizeVoxel);
        m.mapScale = g.sizeVoxel;
        m.columns = new List<Column>();

        for (int i = 1; i <= g.sizeGrid.x / g.sizeVoxel; i++)
        {
            List<Column> list = new List<Column>();
            for (int k = 1; k <= g.sizeGrid.z / g.sizeVoxel; k++)
            {
                RaycastHit hit;
                if (Physics.Raycast(transform.position + g.basePos + new Vector3(-g.sizeVoxel / 2f, 0, -g.sizeVoxel / 2f) + new Vector3(i, g.sizeGrid.y / g.sizeVoxel, k) * g.sizeVoxel, Vector3.down, out hit, g.sizeGrid.y, layerMask))
                {
                    //Debug.Log(hit.point.y);
                    //Debug.Log("Did Hit");

                    Column c = new Column();
                    c.type = Column.Type.Ground;
                    c.height = hit.point.y;
                    m.columns.Add(c);
                }
                else
                {
                    Debug.DrawRay(transform.position + g.basePos + new Vector3(-g.sizeVoxel / 2f, 0, -g.sizeVoxel / 2f) + new Vector3(i, g.sizeGrid.y / g.sizeVoxel, k) * g.sizeVoxel, Vector3.down * g.sizeGrid.y, Color.white);
                    Column c = new Column();
                    c.type = Column.Type.Ground;
                    c.height = 0;
                    m.columns.Add(c);
                    Debug.Log("No Hit");
                }
            }
        }

        //Debug.Log(JsonUtility.ToJson(m));
        File.WriteAllText("Assets/JSON/" + nameMap + ".json", JsonUtility.ToJson(m));
    }

    void OnDrawGizmosSelected()
    {
        Gizmos.color = new Color(0, 1, 0, 0.75F);

        for (int i = 1; i <= g.sizeGrid.x / g.sizeVoxel; i++)
        {
            for (int k = 1; k <= g.sizeGrid.z / g.sizeVoxel; k++)
            {
              //  Gizmos.DrawLine(transform.position + g.basePos + new Vector3(-g.sizeVoxel / 2f, 0, -g.sizeVoxel / 2f) + new Vector3(i, g.sizeGrid.y / g.sizeVoxel, k) * g.sizeVoxel, transform.position + g.basePos + new Vector3(-g.sizeVoxel / 2f, 0, -g.sizeVoxel / 2f) + new Vector3(i, 0, k) * g.sizeVoxel);
            }
        }
    }
}
