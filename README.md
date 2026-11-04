# 3D Pong
A simple 3-Dimensional pong game made with openGL and C++.

## Features
- 3D Rendering with perspective.
- Simple AI that follows the ball.
- Dynamic point lighting with the Phong model.

## Controls
| Key | Action |
|-|-|
| ESC | Exit |
| A | Move left |
| D | Move right |


## Compile and run
1. Download the repository from Github.
```bash
git clone https://www.github.com/sarokD/pong-3D
```

2. Change into the directory and compile with CMake (Makefile also available).
```bash
cd pong-3D
cmake --build build
```

3. The executable should be created which you can now easily run.  
    - UNIX (MacOS, GNU+linux, BSD):
    ```bash
    ./build/3d-pong
    ```  
    - Windows:
    ```powershell
    .\build\3d-pong
    ```  

## License
This Project is licensed under the **MIT License**