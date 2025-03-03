#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

# define M_PI 3.14159265358979323846

float vx = 0;
float vy = 0;

float aspectRatio = 1.0f;
float fov = glm::radians(45.0f);


int axes_count;
const float* joystick_axes;

const unsigned char* buttons;
int buttons_count;

float farDistance= 30.0f;
auto camera = glm::vec3(0.0f, 0.0f, 2.0f);
auto aim = glm::vec3(0, 0, 0);

double mousex, mousey;
double mousex_last, mousey_last;
int last_mouse_event = GLFW_RELEASE;

double height = 800;
double width = 800;

float speed = 0.01f;

static float yaw = -90.0f; // Start facing forward
static float pitch = 0.0f;
glm::mat4 mvp;

glm::mat3x3 right_transform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0));

void framebuffer_size_callback(GLFWwindow* window, int ww, int hh)
{
    height = hh;
    width = ww;
    glViewport(0, 0, width, height);

    aspectRatio = width/height;
    std::cout << "aspectRatio" << aspectRatio << std::endl;
}  

void processInput(GLFWwindow *window)
{

    float joystick_threshold = 0.3f;

    joystick_axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);

    // std::cout << "joystick_axes " << axes_count << std::endl;
    // for(int k = 0; k < axes_count; k++)
    // {
    //     std::cout << "\t axis " << k << ": " << joystick_axes[k] << std::endl;
    // }

    GLFWgamepadstate state;
 
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
    {
        // if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        // {
        //     vx = 0.3f;
        //     vy = 0.3f;
        // }

        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        {
            vx = 0.0f;
            vy = 0.0f;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    
    glm::vec3 forward = glm::normalize(aim - camera); // Forward direction
    glm::vec3 right = right_transform * forward; // Forward direction


    if (axes_count > 0)
    {
        
        if((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) or (joystick_axes[1] > joystick_threshold))
        {
            camera -= speed * forward;
            aim -= speed * forward;
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS or (joystick_axes[1] < -joystick_threshold))
        {
            camera += speed * forward;
            aim += speed * forward;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS or (joystick_axes[0] > joystick_threshold))
        {
            camera += speed * right;
            aim += speed * right;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS or (joystick_axes[0] < -joystick_threshold))
        {
            camera -= speed * right;
            aim -= speed * right;
        }
        
        // glfwGetCursorPos(window, &mousex, &mousey);
        
        // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        //     last_mouse_event = 0;
        
        // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        // {
            // if (last_mouse_event == 0)
            // {
                //     mousex_last = mousex;
                //     mousey_last = mousey;
                //     last_mouse_event = 1;           
                // }
                // else
                // {   
                    // mousex = joystick_axes[3];
                    // mousey = joystick_axes[2];
                    
                    // float xdiff = (mousex - mousex_last)/width;
                    float xdiff = 0;
                    if (std::abs(joystick_axes[3]) > joystick_threshold)
                    xdiff = joystick_axes[3];
                    float ydiff = 0;
                    if (std::abs(joystick_axes[4]) > joystick_threshold)
                    ydiff = joystick_axes[4];
                    
                    // float ydiff = (mousey - mousey_last)/height;
                    
                    // float sensitivity = 50.0f; // Tune sensitivity
                    float sensitivity = 0.59f; // Tune sensitivity
                    yaw += xdiff * sensitivity;
                    pitch -= ydiff * sensitivity; // Invert Y for natural movement
                    
                    pitch = glm::clamp(pitch, -89.0f, 89.0f); // Prevent flipping
                    
                    glm::vec3 direction;
                    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    direction.y = sin(glm::radians(pitch));
                    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    
                    aim = camera + direction;
                    
                    
                    // std::cout << "xdiff" << xdiff << std::endl;
                    
                    // mousex_last = mousex;
                    // mousey_last = mousey;
                    // }
                    
                    // }
    }
                    
    // camera.y = std::max(0.0f, camera.y);
    camera.y = 0.0f;

    // std::cout << "AIM: " << aim.x << ", " << aim.y << ", " << aim.z << ", (mousex =" << mousex << std::endl;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  


    std::vector<glm::vec4> vertices = {
        {-0.1f, +0.2f, -0.1f, 1.0f},
        {+0.1f, +0.2f, -0.1f, 1.0f},
        {-0.1f, +0.0f, -0.1f, 1.0f},
        {+0.1f, +0.0f, -0.1f, 1.0f},
        {-0.1f, +0.2f, +0.1f, 1.0f},
        {+0.1f, +0.2f, +0.1f, 1.0f},
        {-0.1f, +0.0f, +0.1f, 1.0f},
        {+0.1f, +0.0f, +0.1f, 1.0f},

        {-10.0f, 0.0f, -10.0f, 1.0f}, // 8
        {+10.0f, 0.0f, -10.0f, 1.0f}, // 9
        {-10.0f, 0.0f, +10.0f, 1.0f}, // 10
        {+10.0f, 0.0f, +10.0f, 1.0f}, // 11
    };  

    //fuck it perspective

    glm::mat4 projection = glm::perspective(fov, aspectRatio, 0.1f, farDistance);

    glm::mat4 view = glm::lookAt(camera, aim, glm::vec3(0, 1, 0));

    glm::mat4 model = glm::mat4(1.0f);

    mvp = projection * view * model;

    std::vector<glm::vec3> gl_vertices;

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), &vertices.front(), GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(0);  

    uint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    std::vector<int> indices = {
        0,1,2,
        1,3,2,
        0,2,6,
        0,6,4,
        2,3,7,
        2,7,6,
        1,7,3,
        1,5,7,
        0,4,1,
        0,1,5,
        4,6,5,
        5,6,7,
        8,9,10,
        9,11,10,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices.front(), GL_DYNAMIC_DRAW); 

    const char *vertexShaderSourceGLSLCode = "#version 330 core\n"
        "layout (location = 0) in vec4 aPos;\n"
        "uniform mat4 mvp;\n"
        "out vec4 geomPos;\n" // added
        "void main()\n"
        "{\n"
        // "   gl_Position = mvp * aPos;\n"
        "   geomPos = aPos;\n"
        "}\0";

        const char* geometryShaderSource = "#version 330 core\n"
        "layout (triangles) in;\n"
        "layout (triangle_strip, max_vertices = 3) out;\n"
        "\n"
        "in vec4 geomPos[]; // Input positions from vertex shader\n"
        "out vec3 normal; // Output normal\n"
        "out vec4 fragPos; // Output fragment position\n"
        "\n"
        "uniform mat4 mvp;\n"
        "\n"
        "void main() {\n"
        "    vec3 v1 = geomPos[0].xyz;\n"
        "    vec3 v2 = geomPos[1].xyz;\n"
        "    vec3 v3 = geomPos[2].xyz;\n"
        "\n"
        "    vec3 edge1 = v2 - v1;\n"
        "    vec3 edge2 = v3 - v1;\n"
        "    vec3 triangleNormal = normalize(cross(edge1, edge2));\n"
        "\n"
        "    for (int i = 0; i < 3; i++) {\n"
        "        normal = triangleNormal;\n"
        "        fragPos = geomPos[i];\n"
        "        gl_Position = mvp * geomPos[i]; // Apply MVP transform\n"
        "        EmitVertex();\n"
        "    }\n"
        "    EndPrimitive();\n"
        "}\0";
    
    
    const char *fragShaderSourceGLSLCode =
        "#version 330 core\n"
        "in vec3 normal;\n"
        "in vec4 fragPos;\n"
        "uniform vec3 object_color;\n"
        "uniform vec3 ambient_light;\n"
        "uniform vec3 light_pos;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
            "vec3 light_dir = normalize(light_pos - fragPos.xyz);\n" // Corrected light direction calculation
            "vec3 ambient = ambient_light * object_color;\n"
            "float diffuse_ang = max(0.0f, dot(normal, light_dir));\n"
            "vec3 diffuse = diffuse_ang * object_color;\n"
            "FragColor = vec4((ambient + diffuse)*object_color, 1.0);\n"
        "}\0";
    
    int success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceGLSLCode, NULL);
    glCompileShader(vertexShader);
    //
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSourceGLSLCode, NULL);
    glCompileShader(fragShader);
    //
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int geometryShader;
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

   
    


    //
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    int mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    glUniform3f(glGetUniformLocation(shaderProgram, "object_color"), 1.0f, 0.5f, 0.5f); 
    glUniform3f(glGetUniformLocation(shaderProgram, "ambient_light"), 0.2f, 0.2f, 0.2f); 
    // glUniform3f(glGetUniformLocation(shaderProgram, "light_pos"), 0.0f, 1.0f, 0.0f); 


    glEnable(GL_DEPTH_TEST);

    int j = 0;
    glClearColor(0.4f, 0.6f, 0.8f, 0.5f);
    while(!glfwWindowShouldClose(window))
    {

        j++;
        if (true)
        {
            const float BOUND = 0.0f;

            if (vertices[2].y <= BOUND)
            {
                vy = 0;
            }
                
            if (vertices[0].y > BOUND)
            {
                vy = -0.003;
            }

            for(long unsigned int k=0; k < vertices.size(); k++)
            {
                vertices[k].x += vx;
                vertices[k].y += vy;
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), &vertices.front(), GL_DYNAMIC_DRAW);

        }

        // remake projection
        projection = glm::perspective(glm::radians(30.0f), aspectRatio, 0.1f, farDistance);
        view = glm::lookAt(camera, aim, glm::vec3(0, 1, 0));
        glm::mat4 mvp = projection * view * model;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform3f(glGetUniformLocation(shaderProgram, "light_pos"), camera.x, camera.y, camera.z);


        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragShader); 

    return 0;
}