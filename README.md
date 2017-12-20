# SPRAY
'spray' stands for 'simulated particle ray tracing'.
The goal of the project is to merge the art of ray tracing with computational fluid simulations based on particles.

## Building
For downloading 'spray' and starting with the build process do the following.

    git clone https://ci.inf-i2.uni-jena.de/algorithms-on-caffeine/spray.git
    cd spray
    mkdir build
    cd build

### Linux and MacOS
If you only want to run the program with performant image generation use the build type 'Release'.
For developing 'Debug' might be the better option.

    cmake -D CMAKE_BUILD_TYPE=Release ..
    make

Instead of `make` you can type the following.

    cmake --build .

### Windows
If you only want to run the program with performant image generation use the build type 'Release'.
For developing 'Debug' might be the better option.

    cmake -D CMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ..
    nmake

Instead of `nmake` you can type the following.
    
    cmake --build .

## Usage
### Linux and MacOS
In the build directory run the following command.

    ./spray <filename to stl file>

### Windows
In the build directory run the following command.

    spray <filename to stl file>

## Dependencies
- ![Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- ![OpenGL](https://www.opengl.org/)
- ![GLUT](https://www.opengl.org/resources/libraries/glut/)

## Contributing and Conventions
### C++ Style Guide
Nearly everything will be done as in the ![Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) proposed.
The one exception is the naming convention for structures and classes.
Instead of ![CamelCase](https://en.wikipedia.org/wiki/Camel_case) we will use ![snake_case](https://en.wikipedia.org/wiki/Snake_case).
The first letter will be a capital letter.

### Git Style Guide
- ![Git Flow](http://nvie.com/posts/a-successful-git-branching-model/)
- ![Git Commit Messages](https://chris.beams.io/posts/git-commit/)

## Authors
- Jenette Sellin
- Kazimir Menzel
- Markus Pawellek
