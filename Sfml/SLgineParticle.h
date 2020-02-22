//
//#ifndef SLGINEParticle_H
//#define SLGINEParticle_H
//
//#include "SLgineImport.h"
//namespace SL
//{
//   enum ParticleType
//    {
//        PARTICLE_CIRCLE = 0,
//        PARTICLE_RECTANGLE
//    };
//    class Particle {
//    public:
//        int number = 0;
//        int desizingFactor = 0;
//        float lifeTime = 0;
//        float life = 0;
//        float size = 0;
//        float baseSize = 0;
//        bool sizeChange = true;
//        bool dead = true;
//        sf::Color colorFade = sf::Color::Black;
//        sf::Color pCol;
//        sf::Vector2f pos;
//
//        sf::Vector2f direction;
//
//        ParticleType ptype;
//        void Update();
//
//
//        void Draw();
//    };
//
//
//    class ParticleMap {
//    public:
//
//        Particle* p = new Particle;
//        int numberParticle; /// 1 for just security
//
//        void draw();
//
//        void Update();
//        void AddParticle(Particle particl);
//    };
//    class ParticleEmmiter {
//    public:
//        Particle particleToEmit;
//
//
//        sf::Vector2f pos;
//        ParticleMap* map;
//
//
//        void Update();
//        ParticleEmmiter(ParticleMap* mapt);
//    };
//
//
//    // init the window
//    void initSLgineParticle(sf::RenderWindow* win);
//}
//#endif