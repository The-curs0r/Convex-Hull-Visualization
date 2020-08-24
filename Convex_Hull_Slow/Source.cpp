//#include <dear_imgui/imgui.h>
//#include <dear_imgui/imgui_impl_glfw.h>
//#include <dear_imgui/imgui_impl_opengl3.h>

#include "./Deps/dear_imgui/imgui.h"
#include "./Deps/dear_imgui/imgui_impl_glfw.h"
#include "./Deps/dear_imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

#include "shader.hpp"



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

#include <GLFW/glfw3.h>

using namespace std;

GLFWwindow* window;
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

vector<glm::vec3> points;
vector<glm::vec3> lines;
vector<glm::vec3> boundary;

glm::mat3 mvMatrix = glm::lookAt(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));
glm::mat3 projMatrix = glm::ortho(-0.5f,0.5f, -0.5f, 0.5f, -1.0f, 1.0f);

GLuint vao, vbo;

GLuint fbo, resTex,resDep;

GLuint mul_fbo, mul_resTex, mul_resDep;

int clear = 0, pointSelect = 0, findConvexHull = 1, takeImage=0;
bool viewHullOnly = false, AAFlag = false, prevAAFlag = false;

int visited = 0, numPoints=10;

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
            //cout << xpos / SCR_WIDTH - 0.5f << " " << ypos / SCR_HEIGHT - 0.5f << "\n";
            //Scaling so that point ends up in window
            if (xpos > vMin.x && xpos< vMax.x && ypos>vMin.y && ypos < vMax.y){
                xpos = (xpos - vMin.x) * (SCR_WIDTH) / ((float)(vMax.x - vMin.x)) + 0;
                ypos = (ypos - vMin.y) * (SCR_HEIGHT) / ((float)(vMax.y - vMin.y)) + 0;
                //cout <<xpos<< " "<<ypos<<" "<< vMax.x << " " << vMax.y << "\n";
                points.push_back(glm::vec3(xpos / SCR_WIDTH - 0.5f, -ypos / SCR_HEIGHT + 0.5f, 0.0));
            }
        }
    }
}

void CheckFBOStatus(GLuint fbo, GLenum target){
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
int initialize() {

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //For mac
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //For mac

    //Borderless
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, "My Title", primary, NULL);

    //window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Convex Hull Slow", NULL, NULL);

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

    //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //{
    //    std::cout << "Failed to initialize GLAD" << std::endl;
    //    return -1;
    //}

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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//For Key Input
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//For Curs0r Movement
    glfwPollEvents();//Continously Checks For Input
    glfwSetCursorPos(window, 1920 / 2, 1080 / 2);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Rendering into FBO
    glCreateFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

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

    static const GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
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


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
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
    FILE* f_out = fopen("Texture.tga", "wb");
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
}

void initPoints() {
    for (int i = 0; i < numPoints; i++)
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
            takeSS();
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
            boundary.push_back(glm::vec3(points[visited].x, points[visited].y, 1.0f));
            boundary.push_back(glm::vec3(points[j].x, points[j].y, 1.0f));
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
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        cursor_position_callback(window, xpos, ypos);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (fCounter % 500 == 0) {
            if (pointSelect == 0 && points.size() > 1) {
                updateLines();
                visited++;
                fCounter /= 500;
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
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        if (pointSelect == 0) {
            if (viewHullOnly && boundary.size()>3) {
                glBufferData(GL_ARRAY_BUFFER, boundary.size() * sizeof(glm::vec3), &boundary[0], GL_STATIC_DRAW);
                glDrawArrays(GL_LINES, 0, boundary.size());
            }
            else if (lines.size()>3) {
                glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), &lines[0], GL_STATIC_DRAW);
                glDrawArrays(GL_LINES, 0, lines.size());
            }
            else {
                pointSelect = 1;
            }
            
        }
        else if(pointSelect && points.size()){
            glPointSize(4);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
            glDrawArrays(GL_POINTS, 0, points.size());
            glPointSize(1);
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

        ImGui::Begin("Controls");
        {
            if (ImGui::Button("Clear Hull")) {
                lines.clear();
                boundary.clear();
                pointSelect = 1;
            }
            if (ImGui::Button("Clear Points")) {
                lines.clear();
                boundary.clear();
                points.clear();
                pointSelect = -1;
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
            ImGui::SliderInt("Number Of Points", &numPoints, 3, 100);

            if (ImGui::Button("Find Convex Hull")) {
                lines.clear();
                visited = 0;
                fCounter = 0;
                if (points.size()) {
                    cout << points[0].x << " " << points[0].y << "\n";
                    pointSelect = 0;
                }
            }
            if (ImGui::Button("Take Screenshot")) {
                takeImage = 1;
                //takeSS();
            }
            ImGui::Checkbox("Anti-aliasing", &AAFlag);
            ImGui::Checkbox("View Hull Only", &viewHullOnly);
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << lines.size() << "\n";

    shaderProg.deleteProg();
    cleanUp(); 
    

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
