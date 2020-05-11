# Ray Tracer

* This is a simple Ray Tracer project written in C++ for Computer Graphics class.
* It presents phong lighting, shadows, reflections, refractions. Implemented objects are spheres and triangles.
* It outputs ray-traced image of given scene as **ppm file**.
* Input file should be given as **command-line argument**.

![test1](https://user-images.githubusercontent.com/36446866/81596087-2618ea80-93cc-11ea-8675-5eeac5c60d2d.png)
![test2](https://user-images.githubusercontent.com/36446866/81596094-287b4480-93cc-11ea-9675-d35948b4fa71.png)
![test2reflective](https://user-images.githubusercontent.com/36446866/81596095-2add9e80-93cc-11ea-84fd-bbd8ff36a73d.png)
![test2refractive](https://user-images.githubusercontent.com/36446866/81596099-2d3ff880-93cc-11ea-8d74-a1dd104aaa2d.png)

## Extra Features

* **Reflection**
* **Refraction**
* **Triangles**
	* Triangles can be given as objects like spheres in input file.
	* `<pigment_index>` `<surface_finish_index>` **triangle** `<x1>` `<y1>` `<z1>` `<x2>` `<y2>` `<z2>` `<x3>` `<y3>` `<z3>`
	* Triangle is created according to vertexA(x1, y1, z1), vertexB(x2, y2, z2) and vertexC(x3, y3, z3)