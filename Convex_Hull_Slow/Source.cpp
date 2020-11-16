//ORGANISE CODE PLIS ;-;

#include <dear_imgui/imgui.h>
#include <dear_imgui/imgui_impl_glfw.h>
#include <dear_imgui/imgui_impl_opengl3.h>
#include <ConvexHull.hpp>

//Put all these into 1 header?w
#include <JarvisMarchVisualization.hpp>
#include <GrahamScanVisualization.hpp>
#include <QuickHullVisualization.hpp>
#include <AndrewMonotoneVisualization.hpp>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <set>

#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

#include "shader.hpp"

//Include appropriate OpenGL Loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

//Include GLFW

#include <GLFW/glfw3.h>

using namespace std;

GLFWwindow* window;
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;
int methodFlag = 1, prevMethodFlag = 0;

vector<glm::vec3> points;
vector<glm::vec3> lines;
vector<glm::vec3> boundary;

//For visualization
vector<glm::vec3> currentCheck;
vector<glm::vec3> currentChecked;
glm::vec3 currentPoint;

glm::mat3 mvMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat3 projMatrix = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);

GLuint vao, vbo;

GLuint fbo, resTex, resDep;

GLuint mul_fbo, mul_resTex, mul_resDep;

int clear = 0, pointSelect = 0, findConvexHull = 0, takeImage = 0, paused = 0, nextStep=0;
bool viewHullOnly = false, AAFlag = false, prevAAFlag = false;

int visited = 0, numPoints = 10;

ImVec2 vMin, vMax;

double xpos, ypos; //For mouse input

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (pointSelect) {
            glfwGetCursorPos(window, &xpos, &ypos);
            //Scaling so that point ends up in window
            if (xpos > vMin.x && xpos< vMax.x && ypos>vMin.y && ypos < vMax.y) {
                xpos = (xpos - vMin.x) * (SCR_WIDTH) / ((float)(vMax.x - vMin.x)) + 0;
                ypos = (ypos - vMin.y) * (SCR_HEIGHT) / ((float)(vMax.y - vMin.y)) + 0;
                points.push_back(glm::vec3(xpos / SCR_WIDTH - 0.5f, -ypos / SCR_HEIGHT + 0.5f, 0.0));
            }
        }
    }
}

void CheckFBOStatus(GLuint fbo, GLenum target) {
    GLenum fboStatus = glCheckNamedFramebufferStatus(fbo, target);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        switch (fboStatus)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            cout << "No window\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            cout << "Check attachment status\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cout << "Attach attachments\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            cout << "Enable attachments\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            cout << "Check buffer from glReadBuffer exists in FBO\n";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            cout << "Change formats\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            cout << "Number of samples for each multisample is same?\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            cout << "Number of layers for each multisample is same?\n";
        default:
            break;
        }
    }
    else
        cout << "Complete FBO :D\n";
    return;
}

void convexHullLibrary(int method) {

    if (points.size() == 0)
        return;
    if (method == 7) {
        nextIterationJarvis();
        vector<pair<double, double> > hull = getCurrentHullJarvis();
        boundary.clear();
        currentCheck.clear();
        currentChecked.clear();

        //NOTE - Z COORDINATE IS COLOR FOR NOW
        for (auto i : hull) {
            boundary.push_back(glm::vec3(i.first, i.second, 0.0f));
        }
        hull = getCurrentCheckJarvis();
        for (auto i : hull) {
            if (i.first != NULL)
                currentCheck.push_back(glm::vec3(i.first, i.second, 1.0f));
        }
        hull = getCurrentHullLineJarvis();
        for (auto i : hull) {
            if (i.first != NULL)
                currentCheck.push_back(glm::vec3(i.first, i.second, 0.5f));
        }
        hull.clear();
        pair<double, double> curPoint = getCurrentHullPointJarvis();
        currentPoint = glm::vec3(curPoint.first, curPoint.second, 1.0f);
        return;
    }
    if (method == 5) {

        boundary.clear();
        currentCheck.clear();
        currentChecked.clear();

        nextIterationGraham();
        vector<pair<double, double> > hull = getCurrentHullGraham();
        //NOTE - Z COORDINATE IS COLOR FOR NOW
        for (auto i : hull) {
            boundary.push_back(glm::vec3(i.first, i.second, 0.0f));
        }
        pair<double, double> curPoint = getCurrentHullPointGraham();
        currentPoint = glm::vec3(curPoint.first, curPoint.second, 1.0f);
        if (!hull.empty() && currentPoint.x != NULL)
        {
            boundary[boundary.size() - 1][2] = 1.0f;
            if (hull.size() > 1)
                boundary[boundary.size() - 2][2] = 1.0f;
        }
        hull = getCurrentCheckGraham();
        for (auto i : hull) {
            if (i.first != NULL)
                currentCheck.push_back(glm::vec3(i.first, i.second, 1.0f));
        }
        hull.clear();
        return;
    }
    if (method == 6) {

        boundary.clear();
        currentCheck.clear();
        currentChecked.clear();

        nextIterationQuickHull();
        vector<pair<double, double> > hull = getCurrentHullQuickHull();
        //NOTE - Z COORDINATE IS COLOR FOR NOW
        for (auto i : hull) {
            boundary.push_back(glm::vec3(i.first, i.second, 0.0f));
        }
        if (!doneLeft())
        {
            pair<double, double> point = rightMost();
            boundary.push_back(glm::vec3(point.first, point.second, 1.0f));
        }
        else if (!doneRight()) {
            if (boundary.size() >= 1)
                boundary[boundary.size() - 1][2] = 1.0f;
        }
        hull.clear();
        return;
    }
    if (method == 8) {

        boundary.clear();
        currentCheck.clear();
        currentChecked.clear();

        nextIterationAndrewMonotone();
        vector<pair<double, double> > hull = getCurrentHullAndrewMonotone();
        //NOTE - Z COORDINATE IS COLOR FOR NOW
        for (auto i : hull) {
            boundary.push_back(glm::vec3(i.first, i.second, 0.0f));
        }
        if (!doneAndrewMonotone())
        {
            if (boundary.size() >= 1)
                boundary[0][2] = 1.0f;
            if (boundary.size() >= 2)
                boundary[1][2] = 1.0f;
        }
        hull.clear();
        return;
    }

    //Put in separate function?? Copies glm::vec3[0] and [1]  to set of pair
    set<pair<double, double> > inputPoints;
    for (auto i : points) {
        inputPoints.insert(make_pair(i.x, i.y));
    }

    vector<pair<double, double> > hull;
    switch (method)
    {
    case 2:
        hull = findConvexHullJarvis(inputPoints);
        break;
    case 1:
        hull = findConvexHullGraham(inputPoints);
        break;
    case 3:
        hull = findConvexHullQuickHull(inputPoints);
        break;
    case 4:
        hull = findConvexHullAndrewMonotone(inputPoints);
        break;
    }

    boundary.clear();
    lines.clear();

    //Put in separate function?? Copies set of pair to glm::vec3[0] and [1]
    for (auto i : hull) {
        boundary.push_back(glm::vec3(i.first, i.second, 0.0f));
    }

    hull.clear();
    inputPoints.clear();
    return;
}

int initialize() {

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //For mac
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //For mac

    //Borderless in release mode, don't fool around unless you want to sign out or restart
#if defined(_DEBUG)
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Convex Hull Slow", NULL, NULL);
#else
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    window = glfwCreateWindow(mode->width, mode->height, "Convex Hull Algorithms", primary, NULL);
#endif
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//For Key Input
    glfwPollEvents();//Continously Checks For Input
    glfwSetCursorPos(window, 1920 / 2, 1080 / 2);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Rendering into FBO
    glCreateFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &resTex);
    glBindTexture(GL_TEXTURE_2D, resTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &resDep);
    glBindTexture(GL_TEXTURE_2D, resDep);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, SCR_WIDTH, SCR_HEIGHT);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, resTex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, resDep, 0);

    static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    CheckFBOStatus(fbo, GL_DRAW_FRAMEBUFFER);


    //For multisample FBO
    glGenFramebuffers(1, &mul_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mul_fbo);
    glGenTextures(1, &mul_resTex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mul_resTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mul_resTex, 0);
    glGenTextures(1, &mul_resDep);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mul_resDep);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_DEPTH_COMPONENT32F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, mul_resDep, 0);
    glDrawBuffers(1, drawBuffers);
    CheckFBOStatus(mul_fbo, GL_DRAW_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return 1;
}

void takeSS() {
    //Saving as .TGA
    int rowSize = ((SCR_WIDTH * 3 + 3) & ~3);
    int dataSize = SCR_HEIGHT * rowSize;
    cout << dataSize << "\n";
    unsigned char* data = new unsigned char[dataSize];
#pragma pack (push,1) //Aligns structure members on 1-byte boundaries, or on their natural alignment boundary, whichever is less.
    struct
    {
        unsigned char identsize;
        unsigned char cmaptype;
        unsigned char imagetype;
        short cmapstart;
        short cmapsize;
        unsigned char cmapbpp;
        short xorigin;
        short yorigin;
        short width;
        short height;
        unsigned char bpp;
        unsigned char descriptor;
    }tga_header;
#pragma pack (pop)  //Same as #pragma pack

    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, data);
    memset(&tga_header, 0, sizeof(tga_header));
    tga_header.imagetype = 2;
    tga_header.width = (short)SCR_WIDTH;
    tga_header.height = (short)SCR_HEIGHT;
    tga_header.bpp = 24;
    FILE* f_out = fopen("ScreenShot.tga", "wb");
    fwrite(&tga_header, sizeof(tga_header), 1, f_out);
    fwrite(data, dataSize, 1, f_out);
    fclose(f_out);

    //Converting to PNG
    WinExec("cd ..", 1);
    WinExec("magick \"./Texture.tga\" -flip \"./Output.png\"", 1);

    return;
}

void changeSamples(GLsizei flag) {
    glBindFramebuffer(GL_FRAMEBUFFER, mul_fbo);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mul_resTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, flag, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mul_resTex, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mul_resDep);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, flag, GL_DEPTH_COMPONENT32F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, mul_resDep, 0);
    static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    CheckFBOStatus(mul_fbo, GL_DRAW_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
}

void initPoints() {
    for (int i = 0; i < numPoints; i++)
    {
        float x = (double)(rand()) / (RAND_MAX)-0.5f;
        float y = (double)(rand()) / (RAND_MAX)-0.5f;
        if (x < 0) x += 0.05;
        if (y < 0) y += 0.05;
        if (x > 0) x -= 0.05;
        if (y > 0) y -= 0.05;
        points.push_back(glm::vec3(x, y, 0.0f));
    }
    return;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
            takeSS();
    return;
}

void cleanUp() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteFramebuffers(1, &fbo);
    glDeleteFramebuffers(1, &mul_fbo);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteTextures(1, &mul_resTex);
    glDeleteTextures(1, &mul_resDep);
    glDeleteTextures(1, &resDep);
    glDeleteTextures(1, &resTex);

    points.clear();
    lines.clear();
    boundary.clear();

    return;
}



int main() {
    if (initialize() < 0)
        return -1;

    initPoints();

    Shader shaderProg("vShader.vertexShader.glsl", "fShader.fragmentShader.glsl");
    shaderProg.use();

    int fCounter = 0;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        cursor_position_callback(window, xpos, ypos);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (fCounter % 1000 == 0) {
            fCounter = 1;
            if (nextStep || (!paused && pointSelect == 0 && points.size() > 1 && methodFlag - 4 >= 0)) {
                nextStep = 0;
                convexHullLibrary(methodFlag);
            }
        }

        glClearColor(0.4f, 0.7f, 0.1f, 0.0f);
        glEnable(GL_DEPTH_TEST);

        //Render Into FrameBuffer
        //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mul_fbo);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProg.setMat4("mv_matrix", mvMatrix);
        shaderProg.setMat4("proj_matrix", projMatrix);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if (methodFlag == 1 || methodFlag == 2 || methodFlag == 4 || methodFlag == 3) {
            if (viewHullOnly) {
                glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
            }
            else if (pointSelect) {
                if (points.size()) {
                    glPointSize(4);
                    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, points.size());
                    glPointSize(1);
                }
            }
            else {
                glPointSize(4);
                glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                glDrawArrays(GL_POINTS, 0, points.size());
                glPointSize(1);
                if (boundary.size()) {
                    glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
                }
            }

        }
        else if (methodFlag == 7) {
            if (viewHullOnly) {
                glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
            }
            else if (pointSelect) {
                if (points.size()) {
                    glPointSize(4);
                    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, points.size());
                    glPointSize(1);
                }
            }
            else {
                glPointSize(4);
                glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                glDrawArrays(GL_POINTS, 0, points.size());
                glPointSize(1);
                if (boundary.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_STRIP, 0, boundary.size());
                }
                if (currentCheck.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, currentCheck.size() * sizeof(glm::vec3), &currentCheck[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, currentCheck.size());
                }
                if (currentChecked.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, currentChecked.size() * sizeof(glm::vec3), &currentChecked[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, currentChecked.size());
                }
                if (currentPoint.x != NULL)
                {
                    glPointSize(8);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &currentPoint, GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, 1);
                    glPointSize(1);
                }
            }
        }
        else if (methodFlag == 5) {
            if (viewHullOnly) {
                glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
            }
            else if (pointSelect) {
                if (points.size()) {
                    glPointSize(4);
                    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, points.size());
                    glPointSize(1);
                }
            }
            else {
                glPointSize(4);
                glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                glDrawArrays(GL_POINTS, 0, points.size());
                glPointSize(1);
                if (currentCheck.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, currentCheck.size() * sizeof(glm::vec3), &currentCheck[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, currentCheck.size());
                }
                if (boundary.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                    if (currentPoint.x != NULL)
                        glDrawArrays(GL_LINE_STRIP, 0, boundary.size());
                    else
                        glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
                }
                if (currentPoint.x != NULL)
                {
                    glPointSize(8);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &currentPoint, GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, 1);
                    glPointSize(1);
                }
            }
        }
        else if (methodFlag == 6 || methodFlag == 8) {
            if (viewHullOnly) {
                if (boundary.size()) {
                    glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
                }
            }
            else if (pointSelect) {
                if (points.size()) {
                    glPointSize(4);
                    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_POINTS, 0, points.size());
                    glPointSize(1);
                }
            }
            else {
                glPointSize(4);
                glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
                glDrawArrays(GL_POINTS, 0, points.size());
                glPointSize(1);
                if (boundary.size() >= 2)
                {
                    glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                    glDrawArrays(GL_LINE_LOOP, 0, boundary.size());
                }
            }
        }
        //Increase frame elapsed
        fCounter++;

        //Blit to normal FBO
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mul_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        //Bind default buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        //Setup ImGui windows
        ImGui::SetNextWindowSize(ImVec2(1350.0f, 950.0f), 0);
        ImGui::Begin("ConvexHull");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("ConvexHull");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();

            vMin = ImGui::GetWindowContentRegionMin();
            vMax = ImGui::GetWindowContentRegionMax();

            vMin.x += ImGui::GetWindowPos().x;
            vMin.y += ImGui::GetWindowPos().y;
            vMax.x += ImGui::GetWindowPos().x;
            vMax.y += ImGui::GetWindowPos().y;

            // Use the texture from OpenGL, invert the V from the UV.
            ImGui::Image((ImTextureID)resTex, wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(500.0f,450.0f),0);
        ImGui::Begin("Controls");
        {
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("Clear Hull")) {
                lines.clear();
                boundary.clear();
                pointSelect = 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear Points")) {
                lines.clear();
                boundary.clear();
                points.clear();
                pointSelect = 1;
            }
            if (ImGui::Button("Generate Random Points")) {
                lines.clear();
                boundary.clear();
                points.clear();
                initPoints();
                visited = 0;
                fCounter = 0;
                pointSelect = 1;
            }
            ImGui::SliderInt("Number Of Points", &numPoints, 3, 1000);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("Find Convex Hull")) {
                boundary.clear();
                lines.clear();
                visited = 0;
                fCounter = 0;
                if (points.size()) {
                    pointSelect = 0;
                }
                //Fix this
                if (prevMethodFlag == 7) {
                    set <pair<double, double> > pointSet;
                    for (auto i : points)
                        pointSet.insert(make_pair(i.x, i.y));
                    loadPointsJarvis(pointSet);
                }
                if (prevMethodFlag == 5) {
                    set <pair<double, double> > pointSet;
                    for (auto i : points)
                        pointSet.insert(make_pair(i.x, i.y));
                    loadPointsGraham(pointSet);
                }
                if (prevMethodFlag == 6) {
                    set <pair<double, double> > pointSet;
                    for (auto i : points)
                        pointSet.insert(make_pair(i.x, i.y));
                    loadPointsQuickHull(pointSet);
                }
                if (prevMethodFlag == 8) {
                    set <pair<double, double> > pointSet;
                    for (auto i : points)
                        pointSet.insert(make_pair(i.x, i.y));
                    loadPointsAndrewMonotone(pointSet);
                }
                convexHullLibrary(prevMethodFlag);
            }
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10.0f, 0.0f));
            ImGui::SameLine();
            ImGui::Checkbox("View Hull Only", &viewHullOnly);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::Text("Algorithms");
            ImGui::RadioButton("Graham Scan", &methodFlag, 1); ImGui::SameLine();
            ImGui::RadioButton("Jarvis March", &methodFlag, 2); ImGui::SameLine();
            ImGui::RadioButton("Quick Hull", &methodFlag, 3);ImGui::SameLine();
            ImGui::RadioButton("Andrew Monotone", &methodFlag, 4);

            //Visualizations
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::Text("Visualizations");
            ImGui::RadioButton("Graham Visulaization", &methodFlag, 5);
            ImGui::RadioButton("Jarvis Visulaization", &methodFlag, 7);
            ImGui::RadioButton("Quick Visulaization", &methodFlag, 6);
            ImGui::RadioButton("Andrew Visulaization", &methodFlag, 8);
            if (ImGui::Button("Pause")) {
                paused = 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Resume")) {
                paused = 0;
            }
            ImGui::SameLine();
            if (ImGui::Button("Next Step")) {
                nextStep = 1;
            }

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::Text("Utility");
            ImGui::Checkbox("Anti-aliasing", &AAFlag);
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10.0f, 0.0f));
            ImGui::SameLine();
            if (ImGui::Button("Take Screenshot")) {
                takeImage = 1;
                //takeSS();
            }
        }
        ImGui::End();

        //Render ImGui windows
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (takeImage) takeSS(), takeImage = !takeImage;
        if (AAFlag != prevAAFlag) {
            if (AAFlag) changeSamples((GLsizei)8);
            else changeSamples((GLsizei)1);
            prevAAFlag = !prevAAFlag;
        }
        if (methodFlag != prevMethodFlag) {
            prevMethodFlag = methodFlag;
            lines.clear();
            boundary.clear();
            pointSelect = 1;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProg.deleteProg();
    cleanUp();

    return 0;
}
