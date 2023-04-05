#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>
#include <vector>
#include <math.h>
#include <array>
#include <thread>

using namespace std;

vector<int> cool_seeds = {
    1661691517
};


const float PI = 3.14159295f;
const int window_size = 500;
const int sim_size = 100;

const int min_particles = 500;
const int max_particles = 1000;

const int min_radius = 5;
const int max_radius = 100;

const float dt = 0.000005f;

const float max_force = 2.0f;

const float max_f = 5.0f;

const float colours[4][3] = {
    { 1, 1, 1 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 1, 0.2f, 1 }
};

int RandomMinPlus() {
    if (rand() % 2 == 0) {
        return -1;
    }
    return 1;
}

float RandomFloat(float Max) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / Max));
}

struct Particle {

    float x;
    float y;

    float vx;
    float vy;

    int color;
    int radius;

};


float Distance(float a, float b) {
    return sqrt((a * a) + (b * b));
}

void Rule(Particle* array1, Particle* array2, int a, int b, float force) {

    int r = array1[0].radius;

    for (int i = 0; i < a; i++) {

        float fx = 0;
        float fy = 0;

        Particle p1 = array1[i];

        for (int i2 = 0; i2 < b; i2++) {

            Particle p2 = array2[i2];

            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;

            float dis = Distance(dx, dy);

            if (dis > 0 && dis <= r) {
                if (dis < 1) {
                    dis = 1;
                }
                float F = force * (1.0f / sqrt(dis));

                fx += (F * dx * dt);
                fy += (F * dy * dt);
            }

            array1[i].vx = (array1[i].vx + fx) * 0.5f;
            array1[i].vy = (array1[i].vy + fy) * 0.5f;

            array1[i].x += array1[i].vx;
            array1[i].y += array1[i].vy;

            if (array1[i].x < 0) {
                array1[i].x = 2;
                array1[i].vx *= -1;
            }
            else if (array1[i].x > sim_size - 1) {
                array1[i].x = sim_size - 3;
                array1[i].vx *= -1;
            }

            if (array1[i].y < 0) {
                array1[i].y = 2;
                array1[i].vy *= -1;
            }
            else if (array1[i].y > sim_size - 1) {
                array1[i].y = sim_size - 3;
                array1[i].vy *= -1;
            }


        }
    }


}


void InitalizeArray(Particle*& selected_array, int count, int colour) {
    for (int i = 0; i < count; i++) {
        Particle new_particle = Particle();

        new_particle.x = rand() % sim_size;
        new_particle.y = rand() % sim_size;

        new_particle.color = colour;
        new_particle.radius = min_radius + rand() % (max_radius - min_radius);

        selected_array[i] = new_particle;
    }
}


void DrawParticles(Particle* selected_array, int count) {

    float r = colours[selected_array[0].color][0];
    float g = colours[selected_array[0].color][1];
    float b = colours[selected_array[0].color][2];

    for (int i = 0; i < count; i++) {
        
        glColor3f(r, g, b);
        glVertex2f(selected_array[i].x, selected_array[i].y);
        
    }
}


int main(void)
{

    int cool_seed_index = 1;
    srand(cool_seeds[0]);

    int white_count = min_particles + rand() % (max_particles - min_particles);;
    int red_count = min_particles + rand() % (max_particles - min_particles);
    int green_count = min_particles + rand() % (max_particles - min_particles);
    int blue_count = min_particles + rand() % (max_particles - min_particles);

    Particle* white = new Particle[white_count];
    Particle* red = new Particle[red_count];
    Particle* green = new Particle[green_count];
    Particle* blue = new Particle[blue_count];

    InitalizeArray(white, white_count, 0);
    InitalizeArray(red, red_count, 1);
    InitalizeArray(green, green_count, 2);
    InitalizeArray(blue, blue_count, 3);
    


    float forces[16] = {};

    for (int i = 0; i < 16; i++) {
        forces[i] = RandomMinPlus() * RandomFloat(max_force);
    }



    GLFWwindow* window;



    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_size, window_size, "enge dingen", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluOrtho2D(0.0, sim_size, sim_size, 0.0);


    int pressed = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        
        pressed -= 1;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && pressed < 0) {

            int seed = time(NULL);

            if (cool_seed_index < cool_seeds.size()) {
                seed = cool_seeds[cool_seed_index];
            }

            srand(seed);

            cout << seed << "\n";

            white_count = min_particles + rand() % (max_particles - min_particles);
            red_count = min_particles + rand() % (max_particles - min_particles);
            green_count = min_particles + rand() % (max_particles - min_particles);
            blue_count = min_particles + rand() % (max_particles - min_particles);

            white = new Particle[white_count];
            red = new Particle[red_count];
            green = new Particle[green_count];
            blue = new Particle[blue_count];

            InitalizeArray(white, white_count, 0);
            InitalizeArray(red, red_count, 1);
            InitalizeArray(green, green_count, 2);
            InitalizeArray(blue, blue_count, 3);

          

            for (int i = 0; i < 16; i++) {
                forces[i] = RandomMinPlus() * RandomFloat(max_force);
            }


            pressed = 20;
        }
       


        thread t0(Rule, white, white, white_count, white_count, forces[0]);
        thread t1(Rule, white, red, white_count, red_count, forces[1]);
        thread t2(Rule, white, green, white_count, green_count, forces[2]);
        thread t3(Rule, white, blue, white_count, blue_count, forces[3]);

        thread t4(Rule, red, white, red_count, white_count, forces[4]);
        thread t5(Rule, red, red, red_count, red_count, forces[5]);
        thread t6(Rule, red, green, red_count, green_count, forces[6]);
        thread t7(Rule, red, blue, red_count, blue_count, forces[7]);

        thread t8(Rule, green, white, green_count, white_count, forces[8]);
        thread t9(Rule, green, red, green_count, red_count, forces[9]);
        thread t10(Rule, green, green, green_count, green_count, forces[10]);
        thread t11(Rule, green, blue, green_count, blue_count, forces[11]);

        thread t12(Rule, blue, white, blue_count, white_count, forces[12]);
        thread t13(Rule, blue, red, blue_count, red_count, forces[13]);
        thread t14(Rule, blue, green, blue_count, green_count, forces[14]);
        thread t15(Rule, blue, blue, blue_count, blue_count, forces[15]);
    

        t0.join();
        t1.join();
        t2.join();
        t3.join();

        t4.join();
        t5.join();
        t6.join();
        t7.join();

        t8.join();
        t9.join();
        t10.join();
        t11.join();

        t12.join();
        t13.join();
        t14.join();
        t15.join();

        glBegin(GL_POINTS);

        DrawParticles(white, white_count);
        DrawParticles(red, red_count);
        DrawParticles(green, green_count);
        DrawParticles(blue, blue_count);


        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
