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

    cmake ..
    make

Instead of `make` you can type the following.

    cmake --build .

### Windows

    cmake -G "NMake Makefiles" ..
    nmake

Instead of `nmake` you can type the following.
    
    cmake --build .

## Usage
### Linux and MacOS
In the build directory run the following command.

    ./spray
    
### Windows
In the build directory run the following command.

    spray

## Dependencies
- ![Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- ![OpenGL](https://www.opengl.org/)
- ![GLUT](https://www.opengl.org/resources/libraries/glut/)
- ![FreeGLUT](http://freeglut.sourceforge.net/)

## Contributing
- ![Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- ![Git Flow](http://nvie.com/posts/a-successful-git-branching-model/)
- ![Git Commit Messages](https://chris.beams.io/posts/git-commit/)

## Authors
- Jenette Sellin
- Kazimir Menzel
- Markus Pawellek
