# Spray

![](https://img.shields.io/github/languages/top/lyrahgames/spray.svg?style=for-the-badge)
![](https://img.shields.io/github/languages/code-size/lyrahgames/spray.svg?style=for-the-badge)
![](https://img.shields.io/github/repo-size/lyrahgames/spray.svg?style=for-the-badge)
![](https://img.shields.io/github/license/lyrahgames/spray.svg?style=for-the-badge)

<b>
<table>
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/lyrahgames/spray">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/spray/master.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/spray/tree/master"><img src="https://circleci.com/gh/lyrahgames/spray/tree/master.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/spray">
              <img src="https://codecov.io/gh/lyrahgames/spray/branch/master/graph/badge.svg" />
            </a>
        </td>
    </tr>
    <tr>
        <td>
            develop
        </td>
        <td>
            <a href="https://github.com/lyrahgames/spray/tree/develop">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/spray/develop.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/spray/tree/develop"><img src="https://circleci.com/gh/lyrahgames/spray/tree/develop.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/spray">
              <img src="https://codecov.io/gh/lyrahgames/spray/branch/develop/graph/badge.svg" />
            </a>
        </td>
    </tr>
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/lyrahgames/spray">
                <img src="https://img.shields.io/github/commit-activity/y/lyrahgames/spray.svg?logo=github&logoColor=white">
            </a>
        </td>
        <td>
            <img src="https://img.shields.io/github/release/lyrahgames/spray.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/lyrahgames/spray.svg?label=pre-release&logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag/lyrahgames/spray.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/lyrahgames/spray.svg?label=latest%20tag&logo=github&logoColor=white">
        </td>
    </tr>
</table>
</b>

'spray' stands for 'simulated particle ray tracing'.
The goal of the project is to merge the art of ray tracing with computational fluid simulations based on particles.

## Building
For starting with the build process do the following.

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

    ./spray <filename to stl/obj file>

### Windows
In the build directory run the following command.

    spray <filename to stl/obj file>

## Dependencies
- [Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- [OpenGL](https://www.opengl.org/)
- [GLUT](https://www.opengl.org/resources/libraries/glut/)