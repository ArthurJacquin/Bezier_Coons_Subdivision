# Bezier_Coons_Subdivision

Visualizer of mathematics algorithms in C++ and OpenGL

## Features

### Parametric curves

#### Bezier curves

Implementation of Bezier curve algorithm with step parameters.
* translation of any control points and regeneration of Bézier curve in realtime
* translation of Bezier curve in realtime
<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/BeziersCurve.png">
</p>
* Bezier curve extrusion in Z axis : compute a 3D mesh
<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/BezierExtrude.png">
</p>
* Bezier curve revolution : compute a 3D mesh
<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/BezierRevolution.png">
</p>

#### Chaikin curve

Implementation of Chaikin curve algorithm with u, v and iteration parameters.
* translation of any control points and regeneration of Chaikin curve in realtime
<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/ChaikinCurves.PNG">
</p>

### Coons surface

Implementation of coons surface from 4 chaikin curves. Can't translate nor change control points.
<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/Coons.png">
</p>

### Mesh subdivision

Implementation of 3 meshes subdivisions algorithms iterations by iterations. All screenshots are the third iteration.

#### Catmull Clark 

<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/CatmullClark-3Iter.png">
</p>

#### Loop 

<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/Loop-3Iter.png">
</p>

#### Kobelt

<p align="middle">
  <img width="500" height="250" src="/Bezier/screenshots/Kobetl-3Iter.png">
</p>


## Controls

- right click        => select a curve's point
- Alt + right click  => move a curve's control point
- Ctlr + right click => select a curve 
- Backspace          => delete all meshes
- V key              => enable 3D viewport. Press V again to disable 
- Arrows key         => move in 3D viewport
- W key              => display wireframe
- N key              => display normales

