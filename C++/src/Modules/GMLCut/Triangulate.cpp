#include "Triangulate.hpp"

bool Triangulate::Process(const MyVectorOfVertices & contour, MyVectorOfVertices & result)
{
	/* allocate and initialize list of Vertices in polygon */

	int n = (int)contour.size();
	if (n < 3) return false;

	int *V = new int[n];

	/* we want a counter-clockwise polygon in V */

	if (0.0f < Area(contour))
		for (int v = 0; v < n; v++) V[v] = v;
	else
		for (int v = 0; v < n; v++) V[v] = (n - 1) - v;

	int nv = n;

	/*  remove nv-2 Vertices, creating 1 triangle every time */
	int count = 2 * nv;   /* error detection */

	for (int m = 0, v = nv - 1; nv > 2; )
	{
		/* if we loop, it is probably a non-simple polygon */
		if (0 >= (count--))
		{
			//** Triangulate: ERROR - probable bad polygon!
			return false;
		}

		/* three consecutive vertices in current polygon, <u,v,w> */
		int u = v; if (nv <= u) u = 0;     /* previous */
		v = u + 1; if (nv <= v) v = 0;     /* new v    */
		int w = v + 1; if (nv <= w) w = 0;     /* next     */

		if (Snip(contour, u, v, w, nv, V))
		{
			int a, b, c, s, t;

			/* true names of the vertices */
			a = V[u]; b = V[v]; c = V[w];

			/* output Triangle */
			result.push_back(contour[a]);
			result.push_back(contour[b]);
			result.push_back(contour[c]);

			m++;

			/* remove v from remaining polygon */
			for (s = v, t = v + 1; t < nv; s++, t++) V[s] = V[t]; nv--;

			/* resest error detection counter */
			count = 2 * nv;
		}
	}



	delete[] V;

	return true;
}

double Triangulate::Area(const MyVectorOfVertices & contour)
{
	int n = (int)contour.size();

	double A = 0.0f;

	for (int p = n - 1, q = 0; q < n; p = q++)
	{
		A += contour[p].x/*GetX()*/*contour[q].y/*GetY()*/ - contour[q].x/*GetX()*/*contour[p].y/*GetY()*/;
	}
	return A * 0.5f;
}

bool Triangulate::InsideTriangle(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Px, double Py)
{
	double ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	double cCROSSap, bCROSScp, aCROSSbp;

	ax = Cx - Bx;  ay = Cy - By;
	bx = Ax - Cx;  by = Ay - Cy;
	cx = Bx - Ax;  cy = By - Ay;
	apx = Px - Ax;  apy = Py - Ay;
	bpx = Px - Bx;  bpy = Py - By;
	cpx = Px - Cx;  cpy = Py - Cy;

	aCROSSbp = ax * bpy - ay * bpx;
	cCROSSap = cx * apy - cy * apx;
	bCROSScp = bx * cpy - by * cpx;

	return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
}

bool Triangulate::Snip(const MyVectorOfVertices & contour, int u, int v, int w, int n, int * V)
{
	int p;
	double Ax, Ay, Bx, By, Cx, Cy, Px, Py;

	Ax = contour[V[u]].x/*GetX()*/;
	Ay = contour[V[u]].y/*GetY()*/;

	Bx = contour[V[v]].x/*GetX()*/;
	By = contour[V[v]].y/*GetY()*/;

	Cx = contour[V[w]].x/*GetX()*/;
	Cy = contour[V[w]].y/*GetY()*/;

	if (EPSILON > (((Bx - Ax)*(Cy - Ay)) - ((By - Ay)*(Cx - Ax)))) return false;

	for (p = 0; p < n; p++)
	{
		if ((p == u) || (p == v) || (p == w)) continue;
		Px = contour[V[p]].x/*GetX()*/;
		Py = contour[V[p]].y/*GetY()*/;
		if (InsideTriangle(Ax, Ay, Bx, By, Cx, Cy, Px, Py)) return false;
	}

	return true;
}
