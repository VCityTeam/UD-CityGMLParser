using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid : MonoBehaviour
{
    public Vector3 sizeGrid;
    public int sizeVoxel;
    public Vector3 basePos;

    // Start is called before the first frame update
    void Start()
    {
   
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnDrawGizmosSelected()
    {
        // Display the explosion radius when selected
        Gizmos.color = new Color(1, 0, 0, 0.5f);

        Gizmos.DrawCube(transform.position, sizeGrid);
        // if(sizeVoxel != 0)
        // {
        //     for (int i = 0; i <= sizeGrid.x / sizeVoxel; i++)
        //     {

        //         for (int j = 0; j <= sizeGrid.y / sizeVoxel; j++)
        //         {
        //             Gizmos.DrawLine(transform.position + basePos + new Vector3(i, j, 0) * sizeVoxel, transform.position + basePos + new Vector3(i, j, sizeGrid.z / sizeVoxel) * sizeVoxel);
        //         }
        //     }

        //     for (int j = 0; j <= sizeGrid.y / sizeVoxel; j++)
        //     {
        //         for (int k = 0; k <= sizeGrid.z / sizeVoxel; k++)
        //         {
        //             Gizmos.DrawLine(transform.position + basePos + new Vector3(0, j, k) * sizeVoxel, transform.position + basePos + new Vector3(sizeGrid.x / sizeVoxel, j, k) * sizeVoxel);
        //             //Gizmos.DrawSphere(transform.position + basePos + new Vector3(i, j, k) * sizeVoxel, 1);
        //         }
        //     }

        //     for (int i = 0; i <= sizeGrid.x / sizeVoxel; i++)
        //     {
        //         for (int k = 0; k <= sizeGrid.z / sizeVoxel; k++)
        //         {
        //             Gizmos.DrawLine(transform.position + basePos + new Vector3(i, 0, k) * sizeVoxel, transform.position + basePos + new Vector3(i, sizeGrid.y / sizeVoxel, k) * sizeVoxel);
        //             //Gizmos.DrawSphere(transform.position + basePos + new Vector3(i, j, k) * sizeVoxel, 1);
        //         }
        //     }
        // }
    }
}
