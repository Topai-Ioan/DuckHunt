#pragma once

#include "components/simple_scene.h"


namespace game
{
    class DuckHunt : public gfxc::SimpleScene
    {
    public:
        DuckHunt();
        ~DuckHunt();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void Reinitializare();
        void DeseneazaRata(float deltaTimeSeconds);
        void DesenareCrossHair(int mouseX, int mouseY);
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnWindowResize(int width, int height) override;

        void Tutorial();

    protected:
        glm::ivec2 resolution;

        float cx, cy; // centru patrat
        float cxHead, cyHead; // centru hitbox cap
        float cxBody, cyBody; // centru hitbox cap
        int centerBodyOffSet = 20;
        glm::mat3 modelMatrix;
        float angularStep;
  
        // game speed
        double speed;

        // wings
        bool resetTheWayOfFlapping;
        int currentIterationForWing;
        int noOfIterationsForWings;

        float randomStartX;
        float randomStartY;
        float randomRotation;
        float followX;
        float followY;
        float u_final;

        float translateXCap;
        float translateYCap;
        float translateXOchi;
        float translateYOchi;
        float translateXCioc;
        float translateYCioc;
        float translateXCorp;
        float translateYCorp;        
        float translateXAripaDr;
        float translateYAripaDr;        
        float translateXAripaSt;
        float translateYAripaSt;

        // hitbox
        glm::vec2 colt_st_jos_corp, colt_st_sus_corp, colt_dr_jos_corp, colt_dr_sus_corp;
        glm::vec2 colt_st_jos_cap, colt_st_sus_cap, colt_dr_jos_cap, colt_dr_sus_cap;
        float squareSide;
        float squareHitBoxHeadSide;
        float lungimeHitBoxBody;
        float latimeHitBoxBody;

        int lovit;

        // gloante
        int nrGloante;
        float reloadTime;
        
        // rata evadeaza
        int rataEvadeaza;
        float timpPanaEvadeaza;

        // vieti
        int nrVieti;
        bool scademViata;

        // rata pe ecran
        bool existaRata;
        bool rataIeseSus;
        bool rataIeseJos;

        // bonus
        int killingSpree;
        float bonusAngle;
        float cxBonus, cyBonus;
        float bonusTime;


        // score
        int scorCurent;
        int scorAnterior;
        float scorPasiAnteriori;
        int scoreMax;
        // pauza
        int contorApasareP;
        bool pWasPressed = false;
        float copieTimpPanaEvadeaza;

        // cross hair

        int copieMouseX;
        int copieMouseY;
    };
}   // namespace game
