#include "SLgineImport.h"
namespace SL
{
	void Particle::Update() {
		life -= 1.1f;

		if (life < 0)
		{
			dead = true;
		}
		direction.x /= 1.002;
		direction.y /= 1.002;
		pos.x += direction.x;
		pos.y += direction.y;

		if (sizeChange == true)
		{
			size = baseSize * (life / lifeTime);
		}
		else
		{
			size = baseSize;
		}
	}

	void Particle::Draw() {
		if (ptype == PARTICLE_CIRCLE)
		{
			SL::DrawCircle(pos, size, pCol);
		}
	}

	void ParticleMap::draw() {
		for (int i = 0; i < MaxParticle - 400 * (3 - GetQuality()); i++)
		{
			if (p[i].dead == false)
			{
				p[i].Draw();
			}
		}
	}

	ParticleMap::ParticleMap()
	{
		numberParticle = 0;
		p = new Particle[MaxParticle];
		for (int i = 0; i < MaxParticle; i++)
		{
			p[i] = Particle();
		}
	}
	int numPart = 0;
	void ParticleMap::Update() {
		numPart = 0;

		for (int i = 0; i < MaxParticle - 400 * (3 - GetQuality()); i++)
		{
			if (p[i].dead == false)
			{
				p[i].Update();
				numPart++;
			}
		}
	}
	void ParticleMap::AddParticle(Particle particl) {
		bool partAvailable = false;
		for (int i = 0; i < MaxParticle - 400 * (3 - GetQuality()); i++)
		{
			if (p[i].dead == true)
			{
				p[i] = particl;

				p[i].dead = false;
				return;
			}
		}
	}

	int ParticleMap::getNumberParticle()
	{
		return numPart;
	}

	void ParticleEmmiter::Update() {
		for (int i = 0; i < numParticleAdded; i++)
		{
			Particle particleToEmitedited = particleToEmit;

			particleToEmitedited.direction.x += GetRandomVal(-100, 100) / 100;
			if (particleToEmit.direction.x != 0)
			{
				particleToEmitedited.direction.x += GetRandomVal(-100, 100) / 100;
			}if (particleToEmit.direction.y != 0)
			{
				particleToEmitedited.direction.y += GetRandomVal(-100, 100) / 100;
			}
			if (pos.x == -1.f)
			{
				particleToEmitedited.pos.x = GetRandomVal(0, 1200);
			}
			else
			{
				particleToEmitedited.pos.x = pos.x;
			}
			if (pos.y == -1.f)
			{
				particleToEmitedited.pos.y = GetRandomVal(0, 675);
			}
			else
			{
				particleToEmitedited.pos.y = pos.y;
			}
			particleToEmitedited.lifeTime += GetRandomVal(-10, 10);
			particleToEmitedited.life = particleToEmitedited.lifeTime;
			particleToEmitedited.baseSize += GetRandomVal(-2, 0);
			int rndcol = GetRandomVal(-1, 1);
			particleToEmitedited.pCol.r += rndcol;
			particleToEmitedited.pCol.g += rndcol;
			particleToEmitedited.pCol.b += rndcol;
			map->AddParticle(particleToEmitedited);
		}
	}
	ParticleEmmiter::ParticleEmmiter(ParticleMap* mapt) {
		map = mapt;
	}

	// init the window
	void initSLgineParticle(sf::RenderWindow* win) {
	}/// initialisation
}