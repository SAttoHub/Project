#include "ParticleEmitter.h"
vector<pEmitter> ParticleEmitter::Emitters;
ParticleManager *ParticleEmitter::Manager;

void pEmitter::Create(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 Accel, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type) {
	this->pos = EmitPos;
	this->EmitQuantity = EmitQuantity;
	this->EmitLiveTime = EmitLiveTime;
	this->Velocity = Velocity;
	this->Accel = Accel;
	this->StartScale = StartScale;
	this->EndScale = EndScale;
	this->StartColor = StartColor;
	this->EndColor = EndColor;
	this->LifeTime = LifeTime;
	this->TextureNumber = TextureNumber;
	this->ShaderNumber = ShaderNumber;
	this->Type = Type;
}

void pEmitter::CreateRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, float Rand_width, XMFLOAT3 Accel, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type) {
	this->pos = EmitPos;
	this->EmitQuantity = EmitQuantity;
	this->EmitLiveTime = EmitLiveTime;
	this->Rand_width = Rand_width;
	this->Accel = Accel;
	this->StartScale = StartScale;
	this->EndScale = EndScale;
	this->StartColor = StartColor;
	this->EndColor = EndColor;
	this->LifeTime = LifeTime;
	this->TextureNumber = TextureNumber;
	this->ShaderNumber = ShaderNumber;
	this->Type = Type;
}

void pEmitter::CreateRand2(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 VelRandMIN, XMFLOAT3 VelRandMAX, XMFLOAT3 AccRandMIN, XMFLOAT3 AccRandMAX, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type)
{
	this->pos = EmitPos;
	this->EmitQuantity = EmitQuantity;
	this->EmitLiveTime = EmitLiveTime;
	this->VelRandMIN = VelRandMIN;
	this->VelRandMAX = VelRandMAX;
	this->AccRandMIN = AccRandMIN;
	this->AccRandMAX = AccRandMAX;
	this->StartScale = StartScale;
	this->EndScale = EndScale;
	this->StartColor = StartColor;
	this->EndColor = EndColor;
	this->LifeTime = LifeTime;
	this->TextureNumber = TextureNumber;
	this->ShaderNumber = ShaderNumber;
	this->Type = Type;
}



void pEmitter::CreateCIRCULAR(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 Accel, float Radius, int RotaFlame, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type) {
	this->pos = EmitPos;
	this->EmitQuantity = EmitQuantity;
	this->EmitLiveTime = EmitLiveTime;
	this->Velocity = Velocity;
	this->Accel = Accel;
	this->Radius = Radius;
	this->RotaFlame = RotaFlame;
	this->StartScale = StartScale;
	this->EndScale = EndScale;
	this->StartColor = StartColor;
	this->EndColor = EndColor;
	this->LifeTime = LifeTime;
	this->TextureNumber = TextureNumber;
	this->ShaderNumber = ShaderNumber;
	this->Type = Type;
}

void pEmitter::CreateCIRCULARRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 VelRandRange, XMFLOAT3 Accel, XMFLOAT3 AccRandRange, float Radius, float RadiusRandRange, int RotaFlame, int RotaFlameRandRange, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type) {
	this->pos = EmitPos;
	this->EmitQuantity = EmitQuantity;
	this->EmitLiveTime = EmitLiveTime;
	this->Velocity = Velocity;
	this->VelRandRange = VelRandRange;
	this->Accel = Accel;
	this->AccRandRange = AccRandRange;
	this->Radius = Radius;
	this->RadiusRandRange = RadiusRandRange;
	this->RotaFlame = RotaFlame;
	this->RotaFlameRandRange = RadiusRandRange;
	this->StartScale = StartScale;
	this->EndScale = EndScale;
	this->StartColor = StartColor;
	this->EndColor = EndColor;
	this->LifeTime = LifeTime;
	this->TextureNumber = TextureNumber;
	this->ShaderNumber = ShaderNumber;
	this->Type = Type;
}

void pEmitter::Emit(ParticleManager *P) {
	EmitCount++;
	if (EmitTime == EmitCount) {
		EmitCount = 0;
		switch (Type) {
		case PointEmit:
			P->Create("Particle", pos, Velocity, Accel, LifeTime, StartScale, EndScale, EmitQuantity, TextureNumber, ShaderNumber, StartColor, EndColor);
			break;
		case RandPointEmit:
			P->CreateRand("Particle", EmitQuantity, pos, Rand_width, Accel, LifeTime, StartScale, EndScale, TextureNumber, ShaderNumber, StartColor, EndColor);
			break;
		case RandPointEmit2:
			P->CreateRandVel("Particle", pos, VelRandMIN, VelRandMAX, AccRandMIN, AccRandMAX, LifeTime, StartScale, EndScale, EmitQuantity, TextureNumber, ShaderNumber, StartColor, EndColor);
			break;
		case E_CIRCULAR:
			P->CreateCIRCULARParticle("ParticleCircular",
				EmitQuantity,
				pos,
				Velocity,
				Accel,
				Radius,
				RotaFlame,
				LifeTime,
				StartScale,
				EndScale,
				TextureNumber,
				this,
				ShaderNumber,
				StartColor,
				EndColor);
			break;
		case RandE_CIRCULAR:
			XMFLOAT3 v;
			v.x = Velocity.x + ((rand() % 100) / 100.0f) * VelRandRange.x;
			v.y = Velocity.y + ((rand() % 100) / 100.0f) * VelRandRange.y;
			v.z = Velocity.z + ((rand() % 100) / 100.0f) * VelRandRange.z;
			XMFLOAT3 a;
			a.x = Accel.x + ((rand() % 100) / 100.0f) * AccRandRange.x;
			a.y = Accel.y + ((rand() % 100) / 100.0f) * AccRandRange.y;
			a.z = Accel.z + ((rand() % 100) / 100.0f) * AccRandRange.z;
			float r;
			r = Radius + ((rand() % 100) / 100.0f) * RadiusRandRange;
			float rt;
			rt = RotaFlame + ((rand() % 100) / 100.0f) * RotaFlameRandRange;
			P->CreateCIRCULARParticle("ParticleCircular",
				EmitQuantity,
				pos,
				v,
				a,
				r,
				rt,
				LifeTime,
				StartScale,
				EndScale,
				TextureNumber,
				this,
				ShaderNumber,
				StartColor,
				EndColor);
			break;
		default:
			break;
		}
	}
}

pEmitter *ParticleEmitter::CreateEmitter(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 Accel, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber) {
	pEmitter emitter;
	emitter.Create(EmitPos, EmitQuantity, EmitLiveTime, Velocity, Accel, StartScale, EndScale, StartColor, EndColor, LifeTime, TextureNumber, ShaderNumber);
	Emitters.push_back(emitter);

	return &Emitters[Emitters.size() - 1];
}

pEmitter *ParticleEmitter::CreateRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, float Rand_width, XMFLOAT3 Accel, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type)
{
	pEmitter emitter;
	emitter.CreateRand(EmitPos, EmitQuantity, EmitLiveTime, Rand_width, Accel, StartScale, EndScale, StartColor, EndColor, LifeTime, TextureNumber, ShaderNumber, Type);
	Emitters.push_back(emitter);
	
	return &Emitters[Emitters.size() - 1];
}

pEmitter *ParticleEmitter::CreateRand2(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 VelRandMIN, XMFLOAT3 VelRandMAX, XMFLOAT3 AccRandMIN, XMFLOAT3 AccRandMAX, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type)
{
	pEmitter emitter;
	emitter.CreateRand2(EmitPos, EmitQuantity, EmitLiveTime, VelRandMIN, VelRandMAX, AccRandMIN, AccRandMAX, StartScale, EndScale, StartColor, EndColor, LifeTime, TextureNumber, ShaderNumber, Type);
	Emitters.push_back(emitter);

	return &Emitters[Emitters.size() - 1];
}

pEmitter *ParticleEmitter::CreateCIRCULAR(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 Accel, float Radius, int RotaFlame, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type)
{
	pEmitter emitter;
	emitter.CreateCIRCULAR(EmitPos, EmitQuantity, EmitLiveTime, Velocity, Accel, Radius, RotaFlame, StartScale, EndScale, StartColor, EndColor, LifeTime, TextureNumber, ShaderNumber, Type);
	Emitters.push_back(emitter);
	
	return &Emitters[Emitters.size() - 1];
}

pEmitter *ParticleEmitter::CreateCIRCULARRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, XMFLOAT3 Velocity, XMFLOAT3 VelRandRange, XMFLOAT3 Accel, XMFLOAT3 AccRandRange, float Radius, float RadiusRandRange, int RotaFlame, int RotaFlameRandRange, float StartScale, float EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor, int LifeTime, int TextureNumber, UINT ShaderNumber, int Type)
{
	pEmitter emitter;
	emitter.CreateCIRCULARRand(EmitPos, EmitQuantity, EmitLiveTime, Velocity, VelRandRange, Accel, AccRandRange, Radius, RadiusRandRange, RotaFlame, RotaFlameRandRange, StartScale, EndScale, StartColor, EndColor, LifeTime, TextureNumber, ShaderNumber, Type);
	Emitters.push_back(emitter);

	return &Emitters[Emitters.size() - 1];
}

void ParticleEmitter::update() {
	for (int i = 0; i < Emitters.size(); i++) {
		if (Emitters[i].EmitLiveTime > 0) {
			Emitters[i].Emit(Manager);
			Emitters[i].EmitLiveTime--;
		}
		else if(Emitters[i].EmitLiveTime == 0) {
			Emitters.erase(Emitters.begin() + i);
		}
		else {
			Emitters[i].Emit(Manager);
		}
	}
}

void ParticleEmitter::Draw() {
	Manager->Draw();
}
