#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

using namespace std;

GLFWwindow* window;
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

vector<glm::vec3> points;
vector<glm::vec3> lines;

glm::mat3 mvMatrix = glm::lookAt(glm::vec3(0.0f,0.0f,4.0f),glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));
glm::mat3 projMatrix = glm::ortho(-1.0f,1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

GLuint vao, vbo;

int visited = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int initialize() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Convex Hull Slow", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//For Key Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//For Curs0r Movement
    glfwPollEvents();//Continously Checks For Input
    glfwSetCursorPos(window, 1920 / 2, 1080 / 2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /*glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);*/

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    return 1;
}

void initPoints() {
    for (int i = 0; i < 100; i++)
    {
        float x = (double)(rand()) / (RAND_MAX)-0.5f;
        float y = (double)(rand()) / (RAND_MAX)-0.5f;
        points.push_back(glm::vec3(x, y, 0.0f));
    }
    return;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int check(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    float slope = (b.y - a.y) / (b.x - a.x);
    float y = c.y - a.y;
    float x = slope * (c.x - a.x);
    //if (abs(y - x) < 0.00001) return 0;
    if (y > x) return 1;
    if (y < x) return -1;
    return 1;
}

bool comp(glm::vec3 a, glm::vec3 b) {
    if (a.x == b.x && a.y == b.y && a.z == b.z) return true;
    return false;
}

void updateLines() {
    if (visited >= points.size()) return;
    for (int j = visited + 1; j < points.size(); j++)
    {
        //int chk = 0;
        ////cout << lines.size() << "\n";
        //glm::vec3 at = glm::vec3(points[j].x, points[j].y, 1.0f);
        //glm::vec3 af = glm::vec3(points[j].x, points[j].y, 0.0f);
        //glm::vec3 bt = glm::vec3(points[visited].x, points[visited].y, 1.0f);
        //glm::vec3 bf = glm::vec3(points[visited].x, points[visited].y, 0.0f);
        //for (int i = 0; i + 1 < lines.size(); i+=2)
        //{
        //    if ((comp(lines[i],at)&&  comp(lines[i + 1],bt)) || (comp(lines[i], bt) && comp(lines[i + 1], at)) || (comp(lines[i], af) && comp(lines[i + 1], bf)) || (comp(lines[i], bf) && comp(lines[i + 1], af))) {
        //        chk = 1;
        //        //std::cout << "yeaea\n";
        //    }
        //}
        //if (chk) continue;

        int flag = 0, val = -2;
        for (int i = 0; i < points.size(); i++)
        {
            if (i == j || i == visited) continue;
            if (check(points[visited], points[j], points[i]) != val) {
                if (!(val + 2)) {
                    val = check(points[visited], points[j], points[i]);
                    continue;
                }
                flag = 1;
                break;
            }
        }
        if (!flag) {
            lines.push_back(glm::vec3(points[visited].x, points[visited].y, 1.0f));
            lines.push_back(glm::vec3(points[j].x, points[j].y, 1.0f));
            continue;
        }
        lines.push_back(glm::vec3(points[visited].x, points[visited].y, 0.0f));
        lines.push_back(glm::vec3(points[j].x, points[j].y, 0.0f));
    }
}

int main() {
    if (initialize() < 0)
        return -1;

    initPoints();

    Shader shaderProg("vShader.vertexShader.glsl","fShader.fragmentShader.glsl");
    shaderProg.use();

    int fCounter=0;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.4f, 0.7f, 0.1f, 0.0f);  

        if (fCounter % 500 == 0) {
            updateLines();
            visited++;
            fCounter/=500;
        }

        //cout << lines.size() <<"\n";

        shaderProg.setMat4("mv_matrix", mvMatrix);
        shaderProg.setMat4("proj_matrix", projMatrix);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER, lines.size()*sizeof(glm::vec3), &lines[0], GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, lines.size());

        fCounter++;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    std::cout << lines.size() << "\n";

    /*int val = 0;
    for (int k = 0; k + 1 < lines.size(); k+=2)
    {
            glm::vec3 at = lines[k+1];
            glm::vec3 bt = lines[k];
            for (int i = 0; i+1 < lines.size(); i+=2)
            {
                if (i == k)continue;
                if ((comp(lines[i], at) && comp(lines[i + 1], bt)) || (comp(lines[i], bt) && comp(lines[i + 1], at))) {
                    std::cout << "yeaea\n";
                    val++;
                }
            }
        
    }
    cout << val << "\n";*/
	return 0;
}
