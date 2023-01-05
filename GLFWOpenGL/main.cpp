#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <iostream> 
#include <vector> 
using namespace std;
#include <math.h>

// includes
#include <objloader.hpp>
#include <stb_image.h>
#include <shader_m.h>
#include <camera.h>
#include <model.h>


#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(-10.0f, 20.0f, -30.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float acc1 = 0.2f;
float acc2 = 0.5f;
float acc3 = 1.0f;

// texturas
#define sol_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_sun.jpg"
#define mercurio_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_mercury.jpg"
#define venus_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_venus_surface.jpg"
#define terra_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_earth.jpg"
#define lua_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_moon.jpg"
#define marte_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_mars.jpg"
#define jupiter_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_jupiter.jpg"
#define saturno_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_saturn.jpg"
#define urano_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_uranus.jpg"
#define neptuno_path "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/2k_neptune.jpg"

unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<string> faces);
GLuint SolidSphere(float radius, int slices, int stacks);


int main()
{
    //Propriedades da esfera, camadas longitudinais e transversais
    int slices = 32;
    int stacks = 32;


    //numero de pontos
    int numIndicies = (slices * stacks + slices) * 6;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
	Shader sphereShader("shaders/lighting_maps.glsl", "shaders/lighting_maps.frag");
    Shader lightsphereShader("shaders/light_cube.glsl", "shaders/light_cube.frag");
    Shader skyboxShader("shaders/skybox.glsl", "shaders/skybox.frag");

    /*---------------Inicializacao da construcao de um objecto do tipo esfera para os planetas------------------*/
    /*---------------Carregamos aqui o VAO do tipo esfera  -----------*/
	
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    
    GLuint vao = SolidSphere(4, 32, 32);


    Model ourModel("C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Saturno.obj");


	// LOAD TEXTURES
	unsigned int sol_tex = loadTexture(sol_path);
	unsigned int mercurio_tex = loadTexture(mercurio_path);
	unsigned int venus_tex = loadTexture(venus_path);
	unsigned int terra_tex = loadTexture(terra_path);
	unsigned int lua_tex = loadTexture(lua_path);
	unsigned int marte_tex = loadTexture(marte_path);
	unsigned int jupiter_tex = loadTexture(jupiter_path);
	unsigned int saturno_tex = loadTexture(saturno_path);
	unsigned int urano_tex = loadTexture(urano_path);
	unsigned int neptuno_tex = loadTexture(neptuno_path);
	
    vector <string> faces{
	
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/right.jpg",
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/left.jpg",
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/top.jpg",
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/bottom.jpg",
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/front.jpg",
        "C:/Users/diogo/OneDrive - Universidade da Beira Interior/3ª ano/1º Semestre/Computacao Grafica/Projeto/Ex1/GLFWOpenGL/Textures/Sky/back.jpg",
	};
	
	unsigned int cubemapTexture = loadCubemap(faces);
    //ativamos aqui o shader que vamos utilizar
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
	
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ativar o shader qe queremos usar
		sphereShader.use();
		
		//material properties
		sphereShader.setInt("material.diffuse", 0);
        sphereShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        sphereShader.setFloat("material.shininess", 64.0f);
		// light properties
        sphereShader.setVec3("light.ambient", acc1, acc1, acc1);
        sphereShader.setVec3("light.diffuse", acc2, acc2, acc2);
        sphereShader.setVec3("light.specular", acc3, acc3, acc3);
        sphereShader.setVec3("light.position", glm::vec3(acc1, acc2, acc3));
        sphereShader.setVec3("viewPos", camera.Position);

        // ########################################## Terra #########################################
         //Aqui especificamos que VAO queremos que esteja ativo para o render do respetivo planeta
        glBindVertexArray(vao);

        //especificamos que textura queremos que fique ativa
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terra_tex);

        //Shader ativado para o desenho da esfera
        sphereShader.use();

        // view and projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 moon;


        model = glm::rotate(model, (float)glfwGetTime() / 4.5f, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(24.0f, 0.0f, 0.0f));
        moon = model;
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        /*###################################### LUA######################################*/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lua_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(moon, (float)glfwGetTime(), glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));



        /*###################################### SOL ######################################*/
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, sol_tex);


        lightsphereShader.use();


        lightsphereShader.setMat4("projection", projection);
        lightsphereShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / 15, glm::vec3(0.0f, 1.0f, 0.0f));
        lightsphereShader.setMat4("model", model);



        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		


        /*###################################### MERCURIO ######################################*/

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mercurio_tex);

        sphereShader.use();


        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 3, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));



        /*###################################### VENUS ######################################*/

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, venus_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 4, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(16.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        /*###################################### MARTE ######################################*/

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, marte_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 4, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(30.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        /*###################################### JUPITER ######################################*/


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, jupiter_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 4, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(40.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        /*###################################### SATURNO ######################################*/

        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, saturno_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 6, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(48.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

        sphereShader.setMat4("model", model);
        ourModel.Draw(sphereShader);

        //glDrawElements( GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );



        /*###################################### URANO ######################################*/
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, urano_tex);

        sphereShader.use();

        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 8, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(56.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        /*###################################### NEPTUNO ######################################*/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, neptuno_tex);

        sphereShader.use();


        // world transformation
        model = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() / 10, glm::vec3(0.0f, 3.0f, 0.0f)) * glm::translate(model, glm::vec3(64.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() + 2, glm::vec3(0.0f, 3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        sphereShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        //###################################### SKYBOX ######################################
        glBindVertexArray(skyboxVAO);


        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // removemos a translacao da matriz view
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);


        // skybox cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

   

        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
// Load texture from file
// ----------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 4)
            format = GL_RGBA;
        else if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;


        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




        stbi_image_free(data);
    }
    else
    {
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}
// Carregar os atributos necessarios para a criação de uma esfera
// ------------
GLuint SolidSphere(float radius, int slices, int stacks)
{
    using namespace glm;

    const float pi = 3.1415926535897932384626433832795f;
    const float _2pi = 2.0f * pi;

    //arrays que servem para guardar as coordenadas dos vertices, normais e coordenadas da textura
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;

    //Aqui calculamos as coordenadas UV da textura, como a estrutura a ser desenhada ira ser uma esfera precisamos de converter estas coordenadas para coordenadas esfericas, porque caso contrario estas coordenadas nao coincidem com a estrutura da textura a ser mapeada
    for (int i = 0; i <= stacks; ++i)
    {
        // V texture coordinate.
        float V = i / (float)stacks;
        float phi = V * pi;

        for (int j = 0; j <= slices; ++j)
        {
            // U texture coordinate.
            float U = j / (float)slices;
            float theta = U * _2pi;

            float X = cos(theta) * sin(phi);
            float Y = cos(phi);
            float Z = sin(theta) * sin(phi);

            positions.push_back(vec3(X, Y, Z) * radius);
            normals.push_back(vec3(X, Y, Z));
            textureCoords.push_back(vec2(U, V));
        }
    }

    // Aqui geramos os indices de posicao
    vector<GLuint> indicies;

    for (int i = 0; i < slices * stacks + slices; ++i)
    {
        indicies.push_back(i);
        indicies.push_back(i + slices + 1);
        indicies.push_back(i + slices);

        indicies.push_back(i + slices + 1);
        indicies.push_back(i);
        indicies.push_back(i + 1);
    }

    //Geramos um VAO para guardarmos os VBO a serem criados
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Criamos aqui 4 VBO para guardarmos as coordenadas dos vertices , normais , texturas e indices de posicoes
    GLuint vbos[4];
    glGenBuffers(4, vbos);

    //VBO das posicoes
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //VBO das normais
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    //VBO das texturas
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2), textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(2);

    //VBO dos indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}
// carregar textura de um cubo
//funcao que carrega multiplas strings com o caminho das texturas para cada uma das faces do cubo
unsigned int loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    //variaveis que recebem a altura e a largura tambem como o formato dos pixeis
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        //utilizamos aqui a funcao da biblioteca stb_image
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }

    //parametros da textura
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


