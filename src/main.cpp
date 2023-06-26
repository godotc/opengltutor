#include "check_gl.hpp" // includes glad/glad.h
#include <GLFW/glfw3.h> // must be placed behind glad/glad.h

#include "glad/glad.h"
#include "glm/fwd.hpp"
#include <corecrt_math.h>
#include <cstdlib>
#include <cstring>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <utility>


const float PI = 3.1415926;

void centric_circle()
{
    const int   step   = 100;
    const float circle = PI * 2.f;
    const float angle  = circle / step;

    static int n = 0;
    ++n;

    float inner_radius = 0.3f;


    float x       = sinf(0);
    float y       = sinf(0);
    auto  inner   = [inner_radius](auto x) { return x * inner_radius; };
    float inner_x = inner(x);
    float inner_y = inner(y);

    if (n >= step)
        n -= step;


    for (int i = 0; i <= n; ++i)
    {
        float next_x       = sinf(angle * i);
        float next_y       = cosf(angle * i);
        float next_inner_x = inner(next_x);
        float next_inner_y = inner(next_y);

        // contentric circle
        glBegin(GL_TRIANGLES);
        {
            /*
                glColor3f(1, 1, 1);
                glVertex3f(0.f, 0.f, 0.f);
                glVertex3f(x, y, 0.f);
                glVertex3f(next_x, next_y, 0.f);

                glColor3f(0, 0, 0);
                glVertex3f(0.f, 0.f, 0.f);
                glVertex3f(inner_x, inner_y, 0.f);
                glVertex3f(next_inner_x, next_inner_y, 0.f);
            */
            glVertex3f(inner_x, inner_y, 0.f);
            glVertex3f(x, y, 0.f);
            glVertex3f(next_x, next_y, 0.f);

            glVertex3f(inner_x, inner_y, 0.f);
            glVertex3f(next_x, next_y, 0.f);
            glVertex3f(next_inner_x, next_inner_y, 0.f);
        }
        CHECK_GL(glEnd());


        x       = next_x;
        y       = next_y;
        inner_x = next_inner_x;
        inner_y = next_inner_y;
    }
}

void centric_circle(glm::vec2 angles, float inner_r, float step_angle, glm::vec4 color)
{

    auto get_radian = [](auto angle) { return angle * PI / 180.f; };

    auto l = get_radian(angles.x);
    auto r = get_radian(angles.y);
    auto s = get_radian(step_angle);

    glColor4f(color.x, color.y, color.z, color.w);


    float x = cosf(l);
    float y = sinf(l);

    auto  inner   = [inner_r](auto x) { return x * inner_r; };
    float inner_x = inner(x);
    float inner_y = inner(y);



    float i = l;
    for (; i <= r; i += s)
    {
        float next_x       = cosf(i);
        float next_y       = sinf(i);
        float next_inner_x = inner(next_x);
        float next_inner_y = inner(next_y);

        // contentric circle
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(inner_x, inner_y, 0.f);
            glVertex3f(x, y, 0.f);
            glVertex3f(next_x, next_y, 0.f);

            glVertex3f(inner_x, inner_y, 0.f);
            glVertex3f(next_x, next_y, 0.f);
            glVertex3f(next_inner_x, next_inner_y, 0.f);
        }
        CHECK_GL(glEnd());


        x       = next_x;
        y       = next_y;
        inner_x = next_inner_x;
        inner_y = next_inner_y;
    }


    // {
    //     float next_x       = sinf(i + s);
    //     float next_y       = cosf(i + s);
    //     float next_inner_x = inner(next_x);
    //     float next_inner_y = inner(next_y);
    //     glBegin(GL_TRIANGLES);
    //     {
    //         glVertex3f(inner_x, inner_y, 0.f);
    //         glVertex3f(x, y, 0.f);
    //         glVertex3f(next_x, next_y, 0.f);

    //         glVertex3f(inner_x, inner_y, 0.f);
    //         glVertex3f(next_x, next_y, 0.f);
    //         glVertex3f(next_inner_x, next_inner_y, 0.f);
    //     }
    //     CHECK_GL(glEnd());
    // }
}

void opencv()
{
    float d = 0.35;

    // r
    glPushMatrix();
    {
        glTranslated(0, d, 0);
        glScaled(0.3, 0.3, 0.3);
        glRotated(-60.f, 0, 0, 1);
        centric_circle({0, 300}, 0.5, 10, {1, 0, 0, 1});
    }
    glPopMatrix();

    // g
    glPushMatrix();
    {
        glTranslated(-d, -d, 0);
        glScaled(0.3, 0.3, 0.3);
        glRotated(60.f, 0, 0, 1);
        centric_circle({0, 300}, 0.5, 10, {0, 1, 0, 1});
    }
    glPopMatrix();

    // b
    glPushMatrix();
    {
        glTranslated(d, -d, 0);
        glScaled(0.3, 0.3, 0.3);
        glRotated(120.f, 0, 0, 1);
        centric_circle({0, 300}, 0.5, 10, {0, 0, 1, 1});
    }
    glPopMatrix();
}


static void render()
{
    // centric_circle();
    opencv();
}

int main()
{
    {
        if (!glfwInit())
        {
            const char *errmsg;
            glfwGetError(&errmsg);
            if (!errmsg)
                errmsg = "(no error)";
            std::cerr << "failed to initialize GLFW: " << errmsg << '\n';
            return -1;
        }

        // hint the version required: OpenGL 2.0
        constexpr int version = 20;
        glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version / 10);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version % 10);
        if (version >= 33) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        }
    }

    // Create window
    GLFWwindow *window = glfwCreateWindow(640, 640, "Example", NULL, NULL);
    {
        if (!window)
        {
            const char *errmsg;
            glfwGetError(&errmsg);

            if (!errmsg)
                errmsg = "(no error)";
            std::cerr << "GLFW failed to create window: " << errmsg << '\n';
            std::cerr << "==============================================\n";

            if (!strcmp(errmsg, "X11: The DISPLAY environment variable is missing")) {
                std::cerr << "You seems not running with graphic display\n";
            }
            else if (!strcmp(errmsg, "WGL: The driver does not appear to support OpenGL")) {
                std::cerr << "Please consider install an OpenGL driver, or use the mesa driver\n";
            }
            else if (!strcmp(errmsg, "WGL: Failed to create OpenGL context")) {
                std::cerr << "Your driver seems not supporting the required OpenGL version\n";
            }

            {
                std::cerr << "- If you have a physical graphic card (e.g. NVIDIA), install it from your graphic card vendor official website: http://www.nvidia.com/Download/index.aspx\n";
                std::cerr << "- If you are using Windows, download opengl32.dll from https://pan.baidu.com/s/1TZ6nVJC7DZIuUarZrGJYow?pwd=opgl and place it into the same directory as this executable file (alternatively you may download opengl32sw.dll from Internet and rename it to opengl32.dll to place into the same directory as this executable file)\n";
                std::cerr << "- If you are using Linux or WSL1, install the mesa driver: https://ubuntuhandbook.org/index.php/2021/07/install-latest-mesa-ubuntu-20-04-21-04/";
                std::cerr << "- If you use WSL2, install WSLg: https://learn.microsoft.com/zh-cn/windows/wsl/tutorials/gui-apps\n";
                std::cerr << "- If you are using SSH remote server, try connect it using ssh -X <ip address>\n";
                std::cerr << "- If you are using MacOS, you probably want to use Windows or Linux instead for better OpenGL support\n";
                std::cerr << "- If you are using a Laptop with dual-cards, make sure you have switch to dedicated card (NVIDIA) instead of the integrated card (Intel)\n";
                std::cerr << "==============================================\n";
            }
#ifdef _WIN32
            std::system("pause");
#endif
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);


        // Load glXXX function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwTerminate();
            std::cerr << "GLAD failed to load GL functions\n";
            return -1;
        }
        std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

        CHECK_GL(glEnable(GL_POINT_SMOOTH));
        CHECK_GL(glEnable(GL_BLEND));
        CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        CHECK_GL(glPointSize(64.0f));
    }

    // start main game loop
    while (!glfwWindowShouldClose(window))
    {
        // render graphics
        CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
        render();
        // refresh screen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
