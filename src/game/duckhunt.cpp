#include "game/duckhunt.h"

#include <vector>
#include <iostream>

#include "game/transform2D.h"
#include "game/object2D.h"

#pragma warning( disable : 26495)

using namespace std;
using namespace game;

DuckHunt::DuckHunt()
{
}

DuckHunt::~DuckHunt()
{
}

void DuckHunt::Init()
{
    Tutorial();
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 trinagleApex = glm::vec3(0, 0, 0);
    squareHitBoxHeadSide = 40;
    lungimeHitBoxBody = 70;
    latimeHitBoxBody = 60;
    squareSide = 110;
    float triangleSide = 50;
    float height = 100;
    angularStep = -0.05;

    resetTheWayOfFlapping = 0;
    currentIterationForWing = -10;
    noOfIterationsForWings = 10;
    speed = 300;

    // random start     // aici incepe 
    srand(time(0));
    //Reinitializare();
    // 
    // random direction
    followX = -(rand() % 100) + (rand() % 100);
    followY = -(rand() % 100) + (rand() % 100);
    float epsilon = 0.15;
    do
    {
        // random start    
        randomStartX = 250 + (rand() % 500) * 2;
        randomStartY = 100 + (rand() % 100);
        int sign = 1;
        if (rand() % 2 == 0)
            sign = -1;
        float u = tan(randomStartY / randomStartX);
        u_final = atan(sign * u);

    } while ((u_final < -1 + epsilon && u_final > -1 - epsilon) || (u_final < epsilon && u_final > -epsilon) || (u_final < 1 + epsilon && u_final > 1 - epsilon));


    // centru hitbox
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    cxHead = corner.x + squareHitBoxHeadSide / 2;
    cyHead = corner.y + squareHitBoxHeadSide / 2;

    cxBody = corner.x + lungimeHitBoxBody / 2;
    cyBody = corner.y + latimeHitBoxBody / 2;


    // lovit
    lovit = 0;

    //rata
    killingSpree = 0;
    existaRata = true;
    // corp pasare
    translateXCap = randomStartX + 15;
    translateYCap = randomStartY + 75;
    translateXCioc = randomStartX + 10;
    translateYCioc = randomStartY + 80;
    translateXCorp = randomStartX;
    translateYCorp = randomStartY;
    translateXAripaDr = randomStartX + 20;
    translateYAripaDr = randomStartY + 30;
    translateXAripaSt = randomStartX + 5;
    translateYAripaSt = randomStartY + 30;

    translateYOchi = randomStartY + 75;
    if (u_final < 0)
    {
        translateXOchi = randomStartX + 10;
    }
    else
    {
        translateXOchi = randomStartX + 20;
    }

    // gloante
    nrGloante = 3;
    reloadTime = 0;

    // rata zboara
    rataEvadeaza = 0;
    rataIeseSus = 0;
    rataIeseJos = 0;

    // viata
    nrVieti = 3;
    scademViata = false;

    //score
    scorCurent = 0;
    scorAnterior = 0;
    scorPasiAnteriori = 0;
    scoreMax = 140;

    // pauza
    contorApasareP = 0;
    timpPanaEvadeaza = 0;

    // killing spree / bonus
    {
        bonusTime = 0;
        cxBonus = corner.x - squareSide + resolution.x / 2;
        cyBonus = corner.y - squareSide + resolution.y / 2;
        bonusAngle = 0;
        Mesh* romb1 = object2D::CreateSquare("romb1", corner, squareSide, glm::vec3(1, 1, 0), false);
        AddMeshToList(romb1);
        Mesh* romb2 = object2D::CreateSquare("romb2", corner, squareSide, glm::vec3(0, 1, 1), false);
        AddMeshToList(romb2);
        Mesh* romb3 = object2D::CreateSquare("romb3", corner, squareSide, glm::vec3(1, 0, 1), false);
        AddMeshToList(romb3);
        Mesh* romb4 = object2D::CreateSquare("romb4", corner, squareSide, glm::vec3(0, 1, 0), false);
        AddMeshToList(romb4);
    }

    // corp pasare + hitbox
    {
        Mesh* hitBoxCorp = object2D::CreateRectangle("hitBoxCorp", corner, lungimeHitBoxBody, latimeHitBoxBody, glm::vec3(0.67, 0.85, 0.9), false);
        AddMeshToList(hitBoxCorp);

        Mesh* hitBoxCap = object2D::CreateSquare("hitBoxCap", corner, squareHitBoxHeadSide, glm::vec3(0.67, 0.85, 0.9), false);
        AddMeshToList(hitBoxCap);

        Mesh* corp = object2D::CreateTriangle("corp", trinagleApex, triangleSide, height, glm::vec3(0.8, 0.1, 0.85), true);
        AddMeshToList(corp);

        Mesh* cioc = object2D::CreateTriangle("cioc", trinagleApex, triangleSide, height, glm::vec3(1, 1, 0), true);
        AddMeshToList(cioc);

        Mesh* aripa1 = object2D::Create90Triangle("aripa1", trinagleApex, 100, glm::vec3(0.8, 0.5, 0.3));
        AddMeshToList(aripa1);

        Mesh* aripa2 = object2D::Create90Triangle("aripa2", trinagleApex, 100, glm::vec3(0.8, 0.5, 0.3));
        AddMeshToList(aripa2);

        Mesh* cap = object2D::CreateCircle("cap", 32, glm::vec3(0, 1, 0));
        AddMeshToList(cap);

        Mesh* ochi = object2D::CreateCircle("ochi", 16, glm::vec3(0, 0, 0));
        AddMeshToList(ochi);
    }

    // vieti
    {
        Mesh* viata1 = object2D::CreateCircle("viata1", 32, glm::vec3(1, 0, 0));
        AddMeshToList(viata1);

        Mesh* viata2 = object2D::CreateCircle("viata2", 32, glm::vec3(1, 0, 0));
        AddMeshToList(viata2);

        Mesh* viata3 = object2D::CreateCircle("viata3", 32, glm::vec3(1, 0, 0));
        AddMeshToList(viata3);
    }

    // gloante
    {
        Mesh* glont1 = object2D::CreateSquare("glont1", trinagleApex, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
        AddMeshToList(glont1);
        Mesh* glont2 = object2D::CreateSquare("glont2", trinagleApex, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
        AddMeshToList(glont2);
        Mesh* glont3 = object2D::CreateSquare("glont3", trinagleApex, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
        AddMeshToList(glont3);
    }

    // dreptunghi pt scor + scor
    {
        Mesh* dreptunghi = object2D::CreateSquare("dreptunghi", trinagleApex, squareSide, glm::vec3(1, 0, 1), false);
        AddMeshToList(dreptunghi);

        Mesh* scoreFill = object2D::CreateSquare("scoreFill", trinagleApex, squareSide, glm::vec3(1, 0.1, 1), true);
        AddMeshToList(scoreFill);

    }

    // iarba
    {

        Mesh* iarbaTriunghi = object2D::Create90Triangle("iarbaTriunghi", trinagleApex, squareSide, glm::vec3(0, 1, 0), true);
        AddMeshToList(iarbaTriunghi);

        Mesh* iarba = object2D::CreateSquare("iarba", trinagleApex, squareSide, glm::vec3(0, 1, 0), true);
        AddMeshToList(iarba);
    }

    // cer
    {
        Mesh* cer = object2D::CreateSquare("cer", trinagleApex, squareSide, glm::vec3(0.67, 0.85, 0.9), true);
        AddMeshToList(cer);
    }

    // soare 
    {
        Mesh* soare = object2D::CreateCircle("soare", 128, glm::vec3(1, 1, 0));
        AddMeshToList(soare);

        Mesh* razeDeSoare = object2D::CreateTriangle("razeDeSoare", trinagleApex, 10, 20, glm::vec3(1, 1, 0.3), true);
        AddMeshToList(razeDeSoare);
    }

    // game over
    {
        Mesh* bigBox = object2D::CreateSquare("bigBox", trinagleApex, squareSide, glm::vec3(1, 0, 0), true);
        AddMeshToList(bigBox);
    }

    // on mouse move
    {
        Mesh* crossHairUp = object2D::CreateSquare("crossHairUp", trinagleApex, 50, glm::vec3(1, 1, 1), true);
        AddMeshToList(crossHairUp);

        Mesh* crossHairDown = object2D::CreateSquare("crossHairDown", trinagleApex, 50, glm::vec3(1, 1, 1), true);
        AddMeshToList(crossHairDown);

        Mesh* crossHairLeft = object2D::CreateSquare("crossHairLeft", trinagleApex, 50, glm::vec3(1, 1, 1), true);
        AddMeshToList(crossHairLeft);

        Mesh* crossHairRight = object2D::CreateSquare("crossHairRight", trinagleApex, 50, glm::vec3(1, 1, 1), true);
        AddMeshToList(crossHairRight);
    }
}

void DuckHunt::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void DuckHunt::Update(float deltaTimeSeconds)
{
    if (pWasPressed == false)
    {
        followX += speed * deltaTimeSeconds * (-glm::sin(u_final));
        followY += speed * deltaTimeSeconds * (glm::cos(u_final));
    }
    // game over
    {
        if (nrVieti <= 0)
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(0, 0);
            modelMatrix *= transform2D::Scale(20, 20);
            RenderMesh2D(meshes["bigBox"], shaders["VertexColor"], modelMatrix);

        }
    }
 
    DesenareCrossHair(copieMouseX, copieMouseY);

    //killing spree / bonus every 10 consecutive ducks killed you will know it
    {
        if (killingSpree % 10 == 0 && killingSpree != 0) //
        {


            bonusTime += deltaTimeSeconds;
            speed -= 15; // incetinire joc, e mult prea greu de terminat lol
            if (speed < 100)
            {
                speed = 100;
            }
            if (bonusTime < 0.6)
            {
                int offset = 50;
                glm::ivec2 resolution = window->GetResolution();
                bonusAngle += 4 * deltaTimeSeconds;

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cxBonus, cyBonus+ offset);
                modelMatrix *= transform2D::Scale(2, 2);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Rotate(bonusAngle);
                modelMatrix *= transform2D::Translate(-cx, -cy);
                RenderMesh2D(meshes["romb1"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cxBonus, cyBonus+ offset);
                modelMatrix *= transform2D::Scale(2, 2);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Rotate(-bonusAngle);
                modelMatrix *= transform2D::Translate(-cx, -cy);
                RenderMesh2D(meshes["romb2"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cxBonus, cyBonus+ offset);
                modelMatrix *= transform2D::Scale(2, 2);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Rotate(-bonusAngle - M_PI / 4);
                modelMatrix *= transform2D::Translate(-cx, -cy);
                RenderMesh2D(meshes["romb3"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cxBonus, cyBonus+ offset);
                modelMatrix *= transform2D::Scale(2, 2);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Rotate(bonusAngle + M_PI / 4);
                modelMatrix *= transform2D::Translate(-cx, -cy);
                RenderMesh2D(meshes["romb4"], shaders["VertexColor"], modelMatrix);
            }
        }
        else
        {
            bonusTime = 0;
            bonusAngle = 0;
        }
    }

    // iarba
    {   
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(0, 0);
        modelMatrix *= transform2D::Scale(14, 2);
        RenderMesh2D(meshes["iarba"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i <= resolution.x; i+=30)
        {

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(i, 260);
            modelMatrix *= transform2D::Rotate(-M_PI / 2 - M_PI / 4);
            modelMatrix *= transform2D::Scale(1, 1);
            RenderMesh2D(meshes["iarbaTriunghi"], shaders["VertexColor"], modelMatrix);
        }
        
    }

    // dreptunchi pt scor
    {

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1085, 640);
        modelMatrix *= transform2D::Scale(1.3, 0.4);
        RenderMesh2D(meshes["dreptunghi"], shaders["VertexColor"], modelMatrix);

        if (scorCurent > 0)
        {
           
            int xStart = 1085;
            int xEnd = 1245; // basic math haha very funny
            float step = 1;
            if (scorCurent != scorAnterior)
            {
                if (scorCurent >= scoreMax)
                {
                    scorPasiAnteriori -= step;
                }
                scorPasiAnteriori += step;
                scorAnterior = scorCurent;
            }
            
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(1084.5 + step, 640);
            modelMatrix *= transform2D::Scale((float)1 / 110 * scorPasiAnteriori, 0.39);
            RenderMesh2D(meshes["scoreFill"], shaders["VertexColor"], modelMatrix);

        }
    }

    // vieti
    {

        if (nrVieti > 0)
        {
            if (nrVieti >= 1)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(50, 700);
                modelMatrix *= transform2D::Scale(10, 10);
                RenderMesh2D(meshes["viata1"], shaders["VertexColor"], modelMatrix);
            }
            if (nrVieti >= 2)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(100, 700);
                modelMatrix *= transform2D::Scale(10, 10);
                RenderMesh2D(meshes["viata2"], shaders["VertexColor"], modelMatrix);

            }
            if (nrVieti >= 3)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(150, 700);
                modelMatrix *= transform2D::Scale(10, 10);
                RenderMesh2D(meshes["viata3"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
    
    // gloante
    {
        if (nrGloante == 0)
        {
            rataEvadeaza = 1;

        }
        if (nrGloante > 0)
        {
            if (nrGloante >= 1)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(1200, 690);
                modelMatrix *= transform2D::Scale(0.1, 0.2);
                RenderMesh2D(meshes["glont1"], shaders["VertexColor"], modelMatrix);
            }
            if (nrGloante >= 2)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(1150, 690);
                modelMatrix *= transform2D::Scale(0.1, 0.2);
                RenderMesh2D(meshes["glont2"], shaders["VertexColor"], modelMatrix);
            }
            if (nrGloante >= 3)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(1100, 690);
                modelMatrix *= transform2D::Scale(0.1, 0.2);
                RenderMesh2D(meshes["glont3"], shaders["VertexColor"], modelMatrix);
            }

        }
    }

    // soare 
    {

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(950, 630);
        modelMatrix *= transform2D::Scale(60, 60);
        RenderMesh2D(meshes["soare"], shaders["VertexColor"], modelMatrix);
        for (int i = 0; i < 10; i++)
        {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);

     
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Rotate(M_PI/4);
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Rotate(M_PI/2);
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Rotate(M_PI);
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Rotate(M_PI + M_PI/4);
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(950, 630);
                modelMatrix *= transform2D::Rotate(1 * cos(2 * M_PI * i / 10));
                modelMatrix *= transform2D::Rotate(- M_PI/4);
                modelMatrix *= transform2D::Scale(4, 4);
                RenderMesh2D(meshes["razeDeSoare"], shaders["VertexColor"], modelMatrix);
        }

       
    }

    // coliziuni
    {
        if (rataEvadeaza == 0)
        {
            bool flag = 0;;
            if (u_final < 0)
            {
                u_final = abs(u_final);
                flag = 1;
            }
           
            if (translateXCap + followX > 1280) // dreapta
            {
                u_final = 2 * M_PI - u_final;
            }
            else if (translateXCap + followX < 50) // stanga
            {
                u_final = 2 * M_PI - u_final;

            }
            else if (translateYCap + followY > 720) // sus
            {
                if (u_final > 0 && u_final < M_PI / 2)
                {
                    u_final = M_PI - u_final;
                }
                else if (u_final > M_PI * 3 / 2 && u_final < 2 * M_PI) {
                    u_final = 3 * M_PI - u_final;
                }
            }
            else if (translateYCap + followY < 200) // jos
            {
                if (u_final > M_PI && u_final < M_PI * 3 / 2)
                {
                    u_final = 3 * M_PI - u_final;
                }
                else if (u_final > M_PI / 2 && u_final < M_PI) {
                    u_final = M_PI - u_final;
                }
            }
            if (flag == 1)
            {
                u_final = -u_final;
            }
        }
      
    }

    // rata e lovita si moare (pica drept in jos)
    {
        if (lovit == 1)
        {
            u_final = M_PI;
            followY -= deltaTimeSeconds * 2 * speed;
            if (translateYCorp + followY < 150)
            {
                rataIeseJos = true;
            }
        }
    }

    // rata evadeaza (zboara in sus) fie ca nu mai ai gloante fie ca n ai omorat o in 5 secunde
    {
        timpPanaEvadeaza += deltaTimeSeconds;
        if (timpPanaEvadeaza > 5)
        {
            rataEvadeaza = 1;
            timpPanaEvadeaza = 0;
        }
        if (rataEvadeaza == 1)
        {
            u_final = 0;
            followY += deltaTimeSeconds * 2 * speed;
            if (translateYCorp + followY > 720)
            {
                rataIeseSus = true;
                rataEvadeaza = 0;
                nrVieti--;
                if (nrVieti == 0)
                {
                    cout << "Scorul tau este de: " << scorCurent <<" puncte" << endl;
                }
                if (speed >= 100)
                {
                    speed = 0.75 * speed;
                    if (speed < 100)
                    {
                        speed = 100;
                    }

                }

            }
            // reset
            nrGloante = 3;
           
        }
    }


    // speeds up the game 
    if (existaRata == false)
    {
        speed += 5; 
        existaRata = true;
    } 

    // recreere rata
    if ((rataIeseSus == true && nrVieti > 0) || (rataIeseJos == true && nrVieti > 0))
    {
        Reinitializare();
    }

    DeseneazaRata(deltaTimeSeconds);

    // cer
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(0, 0);
        modelMatrix *= transform2D::Scale(14, 20);
        RenderMesh2D(meshes["cer"], shaders["VertexColor"], modelMatrix);

    }
}

void DuckHunt::Reinitializare()
{
    // random direction
    followX = -(rand() % 100) + (rand() % 100);
    followY = -(rand() % 100) + (rand() % 100);
    float epsilon = 0.15;
    do
    {
        // random start    
        randomStartX = 150 + (rand() % 500) * 2;
        randomStartY = 100 + (rand() % 100);
        int sign = 1;
        if (rand() % 2 == 0)
            sign = -1;
        float u = tan(randomStartY / randomStartX);
        u_final = atan(sign * u);

    } while ((u_final < -1 + epsilon && u_final > -1 - epsilon) || (u_final < epsilon && u_final > -epsilon) || (u_final < 1 + epsilon && u_final > 1 - epsilon));


    // lovit
    lovit = 0;
    //viata

    existaRata = true;
    // corp pasare
    translateXCap = randomStartX + 15;
    translateYCap = randomStartY + 75;
    translateXOchi = randomStartX + 20;
    translateYOchi = randomStartY + 75;
    translateXCioc = randomStartX + 10;
    translateYCioc = randomStartY + 80;
    translateXCorp = randomStartX;
    translateYCorp = randomStartY;
    translateXAripaDr = randomStartX + 20;
    translateYAripaDr = randomStartY + 30;
    translateXAripaSt = randomStartX + 5;
    translateYAripaSt = randomStartY + 30;
    if (u_final < 0)
    {
        translateXOchi = randomStartX + 10;
    }

    angularStep = -0.05;


    timpPanaEvadeaza = 0;

    rataIeseSus = false;
    rataIeseJos = false;
}

void DuckHunt::DeseneazaRata(float deltaTimeSeconds)
{ 
    // ochi
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(translateXOchi + followX, translateYOchi + followY);
        modelMatrix *= transform2D::Scale(5, 5);
        RenderMesh2D(meshes["ochi"], shaders["VertexColor"], modelMatrix);
    }

    // cap
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(translateXCap + followX, translateYCap + followY);
        modelMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes["cap"], shaders["VertexColor"], modelMatrix);
    }

    // cioc
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(translateXCioc+ followX, translateYCioc + followY);
        modelMatrix *= transform2D::Scale(0.2, 0.3);
        RenderMesh2D(meshes["cioc"], shaders["VertexColor"], modelMatrix);
    }
   
    //corp
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Scale(0.5, 0.7);
        RenderMesh2D(meshes["corp"], shaders["VertexColor"], modelMatrix);
    }

    // aripa 1 // aripa din dreapta
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);


        //calculate rotation for aripi
        if (resetTheWayOfFlapping == 0 && currentIterationForWing >= -noOfIterationsForWings && currentIterationForWing <= noOfIterationsForWings)
        {
            currentIterationForWing++;
            angularStep -= 2 * deltaTimeSeconds;
            if (pWasPressed == false)
            {
                modelMatrix *= transform2D::Translate(translateXAripaDr + followX, translateYAripaDr + followY);
                modelMatrix *= transform2D::Rotate(angularStep);
                modelMatrix *= transform2D::Translate(-translateXAripaDr - followX, -translateYAripaDr - followY);
            }

            if (currentIterationForWing == noOfIterationsForWings)
                resetTheWayOfFlapping = 1;
        }
        else
        {
            currentIterationForWing--;
            angularStep += 2 * deltaTimeSeconds;
            if (pWasPressed == false)
            {
                modelMatrix *= transform2D::Translate(translateXAripaDr + followX, translateYAripaDr + followY);
                modelMatrix *= transform2D::Rotate(angularStep);
                modelMatrix *= transform2D::Translate(-translateXAripaDr - followX, -translateYAripaDr - followY);
            }

            if (currentIterationForWing == -noOfIterationsForWings)
                resetTheWayOfFlapping = 0;
        }
        modelMatrix *= transform2D::Translate(translateXAripaDr + followX, translateYAripaDr + followY);

        modelMatrix *= transform2D::Rotate(-45 * M_PI / 180);
        modelMatrix *= transform2D::Scale(0.3, 0.3);
        RenderMesh2D(meshes["aripa1"], shaders["VertexColor"], modelMatrix);
    }

    // aripa 2
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);

        if (pWasPressed == false)
        {
            modelMatrix *= transform2D::Translate(translateXAripaSt + followX, translateYAripaSt + followY);
            modelMatrix *= transform2D::Rotate(-angularStep);
            modelMatrix *= transform2D::Translate(-translateXAripaSt - followX, -translateYAripaSt - followY);
        }


        modelMatrix *= transform2D::Translate(translateXAripaSt + followX, translateYAripaSt + followY);
        modelMatrix *= transform2D::Rotate(135 * M_PI / 180);
        modelMatrix *= transform2D::Scale(0.3, 0.3);
        RenderMesh2D(meshes["aripa2"], shaders["VertexColor"], modelMatrix);
    }

    // hitbox corp
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(randomStartX + followX - cxBody + centerBodyOffSet,
                                centerBodyOffSet + randomStartY + followY - cyBody); 
        RenderMesh2D(meshes["hitBoxCorp"], shaders["VertexColor"], modelMatrix);
    }

    // hitbox cap
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(translateXCorp + followX, translateYCorp + followY);
        modelMatrix *= transform2D::Rotate(u_final);
        modelMatrix *= transform2D::Translate(-translateXCorp - followX, -translateYCorp - followY);
        modelMatrix *= transform2D::Translate(translateXCap + followX - cxHead, translateYCap + followY - cyHead);
        RenderMesh2D(meshes["hitBoxCap"], shaders["VertexColor"], modelMatrix);
    }
}

void DuckHunt::DesenareCrossHair(int mouseX, int mouseY)
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mouseX, 5 + mouseY);
    modelMatrix *= transform2D::Scale(0.05, 0.3);
    RenderMesh2D(meshes["crossHairUp"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mouseX, -20 + mouseY);
    modelMatrix *= transform2D::Scale(0.05, 0.3);
    RenderMesh2D(meshes["crossHairDown"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(5 + mouseX, mouseY);
    modelMatrix *= transform2D::Scale(0.3, 0.05);
    RenderMesh2D(meshes["crossHairLeft"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(-20 + mouseX, mouseY);
    modelMatrix *= transform2D::Scale(0.3, 0.05);
    RenderMesh2D(meshes["crossHairRight"], shaders["VertexColor"], modelMatrix);
}

void DuckHunt::FrameEnd()
{
}

void DuckHunt::OnInputUpdate(float deltaTime, int mods)
{
     //for testing
    //if (window->KeyHold(GLFW_KEY_W)) {
    //    speed += 1;
    //}    
    //if (window->KeyHold(GLFW_KEY_S)) {
    //    speed -= 1;
    //}
}

void DuckHunt::OnKeyPress(int key, int mods)
{
    // reset
    if (key == GLFW_KEY_R)
    {
        Reinitializare();
        nrVieti = 3;
        nrGloante = 3;
        speed = 100;
        killingSpree = 0;
        scorCurent = 0;
        scorAnterior = 0;
        scorPasiAnteriori = 0;
    }   

    // pauza
    if (key == GLFW_KEY_P && contorApasareP == 0)
    {
        contorApasareP++;
        pWasPressed = true;
        
        copieTimpPanaEvadeaza = timpPanaEvadeaza;
        timpPanaEvadeaza = -INFINITY;
    }    
    else if (key == GLFW_KEY_P && contorApasareP == 1)
    {
        timpPanaEvadeaza = copieTimpPanaEvadeaza;

        pWasPressed = false;
        contorApasareP = 0;
    }
    // Add key press event
}

void DuckHunt::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseY = resolution.y - mouseY;

    window->HidePointer();
    copieMouseX = mouseX;
    copieMouseY = mouseY;
}

void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    timpPanaEvadeaza = 0;
    if (pWasPressed == false)
        nrGloante--;
    
    // invert Y
    mouseY = resolution.y - mouseY;
    

    // coordonate hitbox cap
    {
        colt_st_jos_cap.x = translateXCap - cxHead + followX ;
        colt_st_jos_cap.y = translateYCap - cyHead + followY;

        colt_st_sus_cap.x = translateXCap - cxHead + followX;
        colt_st_sus_cap.y = translateYCap - cyHead + squareHitBoxHeadSide + followY ;

        colt_dr_jos_cap.x = translateXCap - cxHead + squareHitBoxHeadSide + followX;
        colt_dr_jos_cap.y = translateYCap - cyHead + followY;

        colt_dr_sus_cap.x = translateXCap - cxHead + squareHitBoxHeadSide + followX;
        colt_dr_sus_cap.y = translateYCap - cyHead + squareHitBoxHeadSide + followY;

    }    
    // coordonate hitbox corp
    {
        colt_st_jos_corp.x = translateXCorp - cxBody + followX + centerBodyOffSet;
        colt_st_jos_corp.y = translateYCorp - cyBody + followY;

        colt_st_sus_corp.x = translateXCorp - cxBody + followX + centerBodyOffSet;
        colt_st_sus_corp.y = translateYCorp - cyBody +lungimeHitBoxBody + followY + centerBodyOffSet;

        colt_dr_jos_corp.x = translateXCorp - cxBody + latimeHitBoxBody + followX + centerBodyOffSet;
        colt_dr_jos_corp.y = translateYCorp - cyBody + followY;

        colt_dr_sus_corp.x = translateXCorp - cxBody + latimeHitBoxBody + followX + centerBodyOffSet;
        colt_dr_sus_corp.y = translateYCorp - cyBody + lungimeHitBoxBody + followY + centerBodyOffSet;
    }
  
    if (nrGloante >= 0 && rataEvadeaza != 1 && pWasPressed == false)
    {
        bool flag = false;
        if (mouseY > min(colt_st_jos_corp.y, colt_dr_jos_corp.y) && mouseY < max(colt_st_sus_corp.y, colt_dr_sus_corp.y)
            && mouseX > min(colt_st_jos_corp.x, colt_st_sus_corp.x) && mouseX < max(colt_dr_jos_corp.x, colt_dr_sus_corp.x))
        {
           lovit = 1;
           existaRata = false;
           scorCurent++;
           killingSpree++;
           nrGloante = 3;
           flag = true;
        }

        if (mouseY > min(colt_st_jos_cap.y, colt_dr_jos_cap.y) && mouseY < max(colt_st_sus_cap.y,colt_dr_sus_cap.y)
            && mouseX > min( colt_st_jos_cap.x, colt_st_sus_cap.x) && mouseX < max(colt_dr_jos_cap.x, colt_dr_sus_cap.x))
        {
            lovit = 1;
            existaRata = false;
            scorCurent+=2;
            killingSpree++;
            nrGloante = 3;
            flag = true;
        }
        if(flag == false) 
        {
            killingSpree = 0;
        }
    }
    
    // rata pleaca daca e vie si ai 0 gloante
    if (lovit == 0 && nrGloante == 0)
    {
        rataEvadeaza = 1;
    }

}

void DuckHunt::OnWindowResize(int width, int height)
{

}

void DuckHunt::Tutorial()
{
    cout << "===============================DuckHunt==========================" << endl;
    cout << "Trebuie sa impusti ratele care zboara" << endl;
    cout << "Ai 3 vieti" << endl;
    cout << "Ai 3 gloante pentru fiecare rata" << endl;
    cout << "Daca nu omori rata in 5 secunde aceasta va scapa, dar un glont tras reseteaza acest timp" << endl;
    cout << "Killing Spree-ul se activeaza la 10 rate omorate consecutiv" << endl;
    cout << "R --> Reset" << endl;
    cout << "P --> Pauza" << endl;

}