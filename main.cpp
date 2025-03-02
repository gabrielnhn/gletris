#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

# define M_PI 3.14159265358979323846


// int i = 0;
// float vx = 0.012;
float vx = 0;
// float vy = 0.013;
float vy = 0;

int axes_count;
const float* joystick_axes;

const unsigned char* buttons;
int buttons_count;

float farDistance=50.0f;
auto camera = glm::vec3(0, 0, 3);
auto aim = glm::vec3(0, 0, 0);

double mousex, mousey;
double mousex_last, mousey_last;
int last_mouse_event = GLFW_RELEASE;

double height = 800;
double width = 800;

float speed = 0.02f;

static float yaw = -90.0f; // Start facing forward
static float pitch = 0.0f;
glm::mat4 mvp;

glm::mat3x3 right_transform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0));

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{

    float joystick_threshold = 0.3f;

    joystick_axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
    int count;
    buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_3, &buttons_count); 

    std::cout << "joystick_axes " << axes_count << std::endl;
    for(int k = 0; k < axes_count; k++)
    {
        std::cout << "\t axis " << k << ": " << joystick_axes[k] << std::endl;
    }

    GLFWgamepadstate state;
 
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
    {
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        {
            vx = 0.3f;
        }

        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        {
            vx = 0.0f;
        }
        // else
        //     vx = 0.0f;
        
    }


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    
    glm::vec3 forward = glm::normalize(aim - camera); // Forward direction
    glm::vec3 right = right_transform * forward; // Forward direction


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
    
    
    // if(glfwGetKey(window, GLFW_GAMEPAD_BUTTON_DPAD_LEFT) == GLFW_PRESS)
    // {
    //     vx = 0.3f;
    //     std::cout << "REGISTERED LEFTBUT" << std::endl;
    // }
    // else
    //     vx = 0.0f;


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
            float sensitivity = 0.5f; // Tune sensitivity
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
    std::cout << "AIM: " << aim.x << ", " << aim.y << ", " << aim.z << ", (mousex =" << mousex << std::endl;
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
        {0.1+ -0.3f, 0.3f, -1.0f, 1.0},
        {0.1+ 0.3f,  0.3f, -1.0f, 1.0},
        {0.1+ -0.3f, -0.3f, -1.0f, 1.0},
        {0.1+ 0.3f, -0.3f, -1.0f, 1.0},
        {0.1+ -0.3f, 0.3f, -0.7f, 1.0},
        {0.1+ 0.3f,  0.3f, -0.7f, 1.0},
        {0.1+ -0.3f, -0.3f, -0.7f, 1.0},
        {0.1+ 0.3f, -0.3f, -0.7f, 1.0},
    };  

    //fuck it perspective

    glm::mat4 projection = glm::perspective(glm::radians(30.0f), 1.0f, 0.1f, farDistance);

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
        1,2,3,
        0,4,6,
        0,2,6,
        2,6,7,
        2,3,7,
        1,3,7,
        1,5,7,
        0,1,4,
        0,1,5,
        4,5,6,
        5,6,7,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices.front(), GL_DYNAMIC_DRAW); 

    const char *vertexShaderSourceGLSLCode = "#version 330 core\n"
        // "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 0) in vec4 aPos;\n"
        "uniform mat4 mvp;\n"
        "void main()\n"
        "{\n"
        // "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        // "   gl_Position = mvp * vec4    (aPos, 1.0);\n"
        "   gl_Position = mvp * aPos;\n"
        "}\0";
    
    const char *fragShaderSourceGLSLCode = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
            "FragColor = vec4(0.2f, 0.9f, 0.2f, 1.0f);\n"
        "}\0";
    

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceGLSLCode, NULL);
    glCompileShader(vertexShader);
    //
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSourceGLSLCode, NULL);
    glCompileShader(fragShader);
    //
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    int mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));


    glEnable(GL_DEPTH_TEST);

    // bool goingright = true;
    int j = 0;
    glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
    while(!glfwWindowShouldClose(window))
    {

        j++;
        // if (j % 5 == 0)
        if (true)
        {
            const float BOUND = 5.0f;

            if (vertices[2].y < -BOUND)
            {
                vy = -vy;
                glClearColor(0.8f, 0.2f, 0.2f, 1.0f);
            }
                
            if (vertices[0].y >= BOUND)
            {
                vy = - vy;
                glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
            }

            if (vertices[1].x > BOUND)
            {
                vx = -vx;
                glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

            }

            if (vertices[0].x < -BOUND)
            {
                vx = -vx;
                glClearColor(0.9f, 0.3f, 0.0f, 1.0f);
            }

            for(long unsigned int k=0; k < vertices.size(); k++)
            {
                vertices[k].x += vx;
                vertices[k].y += vy;
             
                // std::cout << k << ": " << vertices[k].y << std::endl;
                
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), &vertices.front(), GL_DYNAMIC_DRAW);

        }

        // remake projection
        view = glm::lookAt(camera, aim, glm::vec3(0, 1, 0));
        glm::mat4 mvp = projection * view * model;
        int mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));


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