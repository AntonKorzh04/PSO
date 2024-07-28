#include <SFML/Graphics.hpp>
#include <cmath>
#include <cfloat>
#include <vector>
#include <iostream>
#include <ctime>
#include "functions.hpp"
#include <thread>
#include <chrono>

using namespace std;

class S_Particles {
public:
    vector<double> c;  // coordinates
    vector<double> cB; // best coordinates
    vector<double> v;  // velocity

    double ff;    // the value of the fitness function
    double ffB;   // best value fitness function

    S_Particles(int parameters)
    {
        c.resize(parameters, 0.0);
        cB.resize(parameters, 0.0);
        v.resize(parameters, 0.0);
        ff = -DBL_MAX;
        ffB = -DBL_MAX;
    }
};

class C_AO_PSO
{
  public:
    vector<S_Particles> p; // particles
    vector<double> rangeMax;  // maximum search range
    vector<double> rangeMin;  // minimum search range
    vector<double> rangeStep; // step search
    vector<double> cB;        // best coordinates
    double ffB;               // FF of the best coordinates
    C_Function* fitnessFunc;

    void InitPS(const int paramsP, const int sizeP, const double inertiaP, const double selfBoostP, const double groupBoostP, EFunc fitnessFuncName, double stepP, int sleepTimeP);
    void Preparation();
    void Dwelling();
    void RenderGraph(sf::RenderWindow &window);
    void RenderParticles(sf::RenderWindow &window);

  private:
    int swarmSize; // swarm size
    int parameters;// number of optimized parameters

    double inertia;
    double selfBoost;
    double groupBoost;
    bool dwelling;
    sf::Texture graphTexture;
    sf::Sprite graphSprite;
    bool graphRendered = false;
    int sleepTime;
    
    void GenerateRNDparticles();
    void ParticleMovement();
    double SeInDiSp(double in, double inMin, double inMax, double step);
    double RNDfromCI(double min, double max);
};

void C_AO_PSO::InitPS(const int paramsP, const int sizeP, const double inertiaP, const double selfBoostP, const double groupBoostP, EFunc fitnessFuncName, double stepP, int sleepTimeP)
{
    ffB = -DBL_MAX;

    parameters = paramsP;
    swarmSize = sizeP;

    rangeMax.resize(parameters);
    rangeMin.resize(parameters);
    rangeStep.resize(parameters);

    dwelling = false;

    inertia = inertiaP;
    selfBoost = selfBoostP;
    groupBoost = groupBoostP;

    p.resize(swarmSize, S_Particles(parameters));

    cB.resize(parameters);

    fitnessFunc = SelectFunction(fitnessFuncName);

    // Задаем диапазоны поиска для x и y
    rangeMin[0] = fitnessFunc->GetMinArg();
    rangeMax[0] = fitnessFunc->GetMaxArg();
    rangeStep[0] = stepP;
    
    rangeMin[1] = fitnessFunc->GetMinArg();
    rangeMax[1] = fitnessFunc->GetMaxArg();
    rangeStep[1] = stepP;

    sleepTime = sleepTimeP;
}

void C_AO_PSO::Preparation()
{
    if (!dwelling)
    {
        ffB = -DBL_MAX;
        GenerateRNDparticles();
        dwelling = true;
    }
    else
    {
        ParticleMovement();
    }
}

void C_AO_PSO::GenerateRNDparticles()
{
    for (int s = 0; s < swarmSize; s++)
    {
        for (int k = 0; k < parameters; k++)
        {
            p[s].c[k] = RNDfromCI(rangeMin[k], rangeMax[k]);
            p[s].c[k] = SeInDiSp(p[s].c[k], rangeMin[k], rangeMax[k], rangeStep[k]);
            p[s].cB[k] = p[s].c[k];
            p[s].v[k] = RNDfromCI(0.0, (rangeMax[k] - rangeMin[k]) * 0.5);
        }
        p[s].ff = fitnessFunc->calc(p[s].c[0], p[s].c[1]);
        p[s].ffB = p[s].ff;
    }
}

void C_AO_PSO::ParticleMovement()
{
    double rp, rg, velocity, posit, positBest, groupBest;

    for (int i = 0; i < swarmSize; i++)
    {
        for (int k = 0; k < parameters; k++)
        {
            rp = RNDfromCI(0.0, 1.0);
            rg = RNDfromCI(0.0, 1.0);

            velocity = p[i].v[k];
            posit = p[i].c[k];
            positBest = p[i].cB[k];
            groupBest = cB[k];

            p[i].v[k] = inertia * velocity + selfBoost * rp * (positBest - posit) + groupBoost * rg * (groupBest - posit);
            p[i].c[k] = posit + p[i].v[k];

            p[i].c[k] = SeInDiSp(p[i].c[k], rangeMin[k], rangeMax[k], rangeStep[k]);
        }
        p[i].ff = fitnessFunc->calc(p[i].c[0], p[i].c[1]);
    }
}

void C_AO_PSO::Dwelling()
{
    for (int i = 0; i < swarmSize; i++)
    {
        if (p[i].ff > p[i].ffB)
        {
            p[i].ffB = p[i].ff;
            for (int k = 0; k < parameters; k++)
                p[i].cB[k] = p[i].c[k];
        }

        if (p[i].ff > ffB)
        {
            ffB = p[i].ff;
            for (int k = 0; k < parameters; k++)
                cB[k] = p[i].c[k];
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

double C_AO_PSO::SeInDiSp(double in, double inMin, double inMax, double step)
{
    if (in <= inMin) return inMin;
    if (in >= inMax) return inMax;
    if (step == 0.0) return in;
    else return inMin + step * (double)round((in - inMin) / step);
}

double C_AO_PSO::RNDfromCI(double min, double max)
{
    if (min == max) return min;
    double Min, Max;
    if (min > max)
    {
        Min = max;
        Max = min;
    }
    else
    {
        Min = min;
        Max = max;
    }
    return Min + ((Max - Min) * (double)rand() / RAND_MAX);
}

void C_AO_PSO::RenderGraph(sf::RenderWindow &window) {
    if (graphRendered) return;

    const int width = 800;
    const int height = 600;
    const float scaleX = width / (rangeMax[0] - rangeMin[0]);
    const float scaleY = height / (rangeMax[1] - rangeMin[1]);

    sf::RenderTexture renderTexture;
    renderTexture.create(width, height);

    sf::VertexArray graph(sf::Points);

    double minFun = fitnessFunc->GetMinFun();
    double maxFun = fitnessFunc->GetMaxFun();
    double maxFunX = fitnessFunc->GetMaxFunX();
    double maxFunY = fitnessFunc->GetMaxFunY();

    for (double x = rangeMin[0]; x <= rangeMax[0]; x += rangeStep[0]) {
        for (double y = rangeMin[1]; y <= rangeMax[1]; y += rangeStep[1]) {
            double z = fitnessFunc->calc(x, y);

            // Normalize z between 0 and 1
            double normalizedZ = (z - minFun) / (maxFun - minFun);
            normalizedZ = std::max(0.0, std::min(1.0, normalizedZ)); // Clamp between 0 and 1

            // Interpolate color from purple (min) to red (max)
            sf::Color color = sf::Color(
                static_cast<sf::Uint8>(255 * normalizedZ),                // Red component
                static_cast<sf::Uint8>(255 * (1 - normalizedZ) * 0.5),    // Green component (for gradient effect)
                static_cast<sf::Uint8>(255 * (1 - normalizedZ))           // Blue component
            );

            graph.append(sf::Vertex(sf::Vector2f(
                (x - rangeMin[0]) * scaleX,
                height - (y - rangeMin[1]) * scaleY
            ), color));
        }
    }

    renderTexture.clear();
    renderTexture.draw(graph);

    // Draw max function point
    sf::CircleShape maxPoint(7);
    maxPoint.setFillColor(sf::Color::Yellow);
    maxPoint.setPosition(
        (maxFunX - rangeMin[0]) * scaleX - 7,
        height - (maxFunY - rangeMin[1]) * scaleY - 7
    );
    renderTexture.draw(maxPoint);

    renderTexture.display();
    graphTexture = renderTexture.getTexture();
    graphSprite.setTexture(graphTexture);

    graphRendered = true;
}

void C_AO_PSO::RenderParticles(sf::RenderWindow &window) {
    const int width = 800;
    const int height = 600;
    const float scaleX = width / (rangeMax[0] - rangeMin[0]);
    const float scaleY = height / (rangeMax[1] - rangeMin[1]);

    window.clear();
    window.draw(graphSprite);

    for (const auto &particle : p) {
        sf::CircleShape circle(5);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition(
            (particle.c[0] - rangeMin[0]) * scaleX - 5,
            height - (particle.c[1] - rangeMin[1]) * scaleY - 5
        );
        window.draw(circle);
    }

    window.display();
}

int main()
{
    srand(time(0)); // Инициализация генератора случайных чисел

    C_AO_PSO pso_1;
    C_AO_PSO pso_2;
    C_AO_PSO pso_3;

    int params = 2; // Количество параметров
    int size = 100;  // Размер роя
    double inertia = 0.5;
    double selfBoost = 0.5;
    double groupBoost = 0.3;
    double step = 0.01;
    int sleepTime = 50;

    EFunc func1 = Skin;
    EFunc func2 = Megacity;
    EFunc func3 = Ackley;

    pso_1.InitPS(params, size, inertia, selfBoost, groupBoost, func1, step, sleepTime);
    pso_2.InitPS(params, size, inertia, selfBoost, groupBoost, func2, step, sleepTime);
    pso_3.InitPS(params, size, inertia, selfBoost, groupBoost, func3, step, sleepTime);

    C_AO_PSO pso[] = {pso_1, pso_2, pso_3};
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "PSO Visualization");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (int i = 0; i < 3; i++) {
            pso[i].RenderGraph(window);
            int cnt = 0;

            while (cnt < 100) {
                pso[i].Preparation();
                pso[i].Dwelling();
                
                pso[i].RenderParticles(window);

                cnt++;
            }
            cout << "Function: " << pso[i].fitnessFunc->GetNamFun() << endl;
            cout << "Best Fitness Value: " << pso[i].ffB << endl;
            cout << "Best Coordinates: x = " << pso[i].cB[0] << ", y = " << pso[i].cB[1] << endl;
            cout << "------------------------------------------------" << endl;
        }
        window.close();
    }

    return 0;
}
