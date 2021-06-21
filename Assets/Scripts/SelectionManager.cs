using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectionManager : MonoBehaviour
{
    [SerializeField] private Material highlightMaterial;
    void Update()
    {
        var ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;
        if(Physics.Raycast(ray, out hit))
        {
            //Debug.Log(hit.)
            var selection = hit.transform;
            var selectionRenderer = selection.GetComponent<Renderer>();
            if(selectionRenderer != null)
            {
                selectionRenderer.material = highlightMaterial;
            }
        }
    }
}
