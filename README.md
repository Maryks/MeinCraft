# MeinCraft
Yet another try at cloning Minecraft in c++

Necessary:
* SFML
* glew-2.0.0
* glm

Visual Studio 2015 instructions:
* Create new visual studio project: Win32 Console Application: console application, empty project
* Set the following properties:
  + Debugging: General
    - 'Environment' -> 'PATH=C:\(...)\SFML\SFML-2.4.2-windows-vc14-32-bit\SFML-2.4.2\bin'
  + C/C++: General (can be seen if there is a c++ file in the project)
    - 'Additional Include Directories' -> 'C:\(...)\SFML\SFML-2.4.2-windows-vc14-32-bit\SFML-2.4.2\include;C:\(...)\opengl-wrappers\glew-2.0.0\include;C:\(...)\opengl-wrappers\glm'
  + Linker: General
    - 'Additional Library Directories' -> 'C:\(...)\SFML\SFML-2.4.2-windows-vc14-32-bit\SFML-2.4.2\lib;C:\(...)\opengl-wrappers\glew-2.0.0\lib\Release\Win32'
  + Linker: Input
    - 'Addtional Dependencies' -> 'glew32.lib;glu32.lib;opengl32.lib;sfml-window-d.lib;sfml-graphics-d.lib;sfml-system-d.lib;(...)'
* Clone repository into the project directory:
  + navigate to project directory and create a new git repository
    - git init
  + set github as origin
    - git remote add origin 'URL'
  + fetch from origin and merge to local master branch
    - git pull origin master
  + set remote branch as upstream
    - git push --set-upstream origin master