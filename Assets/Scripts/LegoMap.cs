using System;
using UnityEngine;
using System.Collections.Generic;

[Serializable]
public class LegoMap
{
    public Vector2Int mapSize;
    public float mapScale;
    public int legoCount;
    public List<Column> columns;
}

[Serializable]
public class Column
{
    public enum Type {Default, Ground, Building};
    
    public float height;
    public Type type;

    public Column()
    {
        this.height = 0f;
        this.type = Type.Default;
    }

    public Column(float height, Type type)
    {
        this.height = height;
        this.type = type;
    }
}