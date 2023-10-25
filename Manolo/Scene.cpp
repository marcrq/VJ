#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 5

#define INIT_GOOMBA_X_TILES 26
#define INIT_GOOMBA_Y_TILES 15
//25

#define INIT_STAR_X_TILES 25
#define INIT_STAR_Y_TILES 25


#define INIT_SETA_X_TILES 30
#define INIT_SETA_Y_TILES 15

#define INIT_KTROOPA_X_TILES 15
#define INIT_KTROOPA_Y_TILES 15


Scene::Scene()
{
	map = NULL;
	player = NULL;
	menus = NULL;
	goomba = NULL;
	star = NULL;
	seta = NULL;
	ktroopa = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (menus != NULL)
		delete menus;
	if (goomba != NULL)
		delete goomba;
	if (star != NULL)
		delete star;
	if (seta != NULL)
		delete seta;
	if (ktroopa != NULL)
		delete ktroopa;
}

enum PlayerAnims
{
	UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE, CERO
};

void Scene::init(int lev) {
	if (lev == 0) { //repasar condici�n
		level = 0;
		initShaders();
		menus = new Menus();
		menus->init(texProgram);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

		resumenLevel1.loadFromFile("images/resumenLevel1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteResumenLevel1 = Sprite::createSprite(glm::ivec2(640, 40), glm::vec2(1.0, 1.0), &resumenLevel1, &texProgram);
		resumenLevel2.loadFromFile("images/resumenLevel2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteResumenLevel2 = Sprite::createSprite(glm::ivec2(640, 40), glm::vec2(1.0, 1.0), &resumenLevel2, &texProgram);
		spriteResumenLevel1->setPosition(glm::vec2(float(0), float(0)));
		spriteResumenLevel2->setPosition(glm::vec2(float(0), float(0)));
	}
	else if (lev == 1) {
		level = 1;
		timerLevel = 500;
		points = 0;
		personajes.clear();
		endedLevel = false;
		initShaders();
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		goomba->setTileMap(map);

		star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * map->getTileSize(), INIT_STAR_Y_TILES * map->getTileSize()));
		star->setTileMap(map);

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * map->getTileSize(), INIT_SETA_Y_TILES * map->getTileSize()));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * map->getTileSize(), INIT_KTROOPA_Y_TILES * map->getTileSize()));
		ktroopa->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * map->getTileSize(), (INIT_SETA_Y_TILES - 15) * map->getTileSize()));
		palo_bandera->setTileMap(map);

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		//personajes.push_back(goomba);
		personajes.push_back(ktroopa);
		//personajes.push_back(star);
		personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar
		//menus = new MainMenu();
		//menus->init(texProgram);

		/////////////////////////////////////77

		numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteTimerCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerCentena->setNumberAnimations(10);
		spriteTimerCentena->setAnimationSpeed(CERO, 8);
		spriteTimerCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(UNO, 8);
		spriteTimerCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(DOS, 8);
		spriteTimerCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(TRES, 8);
		spriteTimerCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CUATRO, 8);
		spriteTimerCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CINCO, 8);
		spriteTimerCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SEIS, 8);
		spriteTimerCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SIETE, 8);
		spriteTimerCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(OCHO, 8);
		spriteTimerCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(NUEVE, 8);
		spriteTimerCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerCentena->setPosition(glm::vec2((33) * map->getTileSize(), (1) * map->getTileSize()));

		spriteTimerDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerDecena->setNumberAnimations(10);
		spriteTimerDecena->setAnimationSpeed(CERO, 8);
		spriteTimerDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(UNO, 8);
		spriteTimerDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(DOS, 8);
		spriteTimerDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(TRES, 8);
		spriteTimerDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CUATRO, 8);
		spriteTimerDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CINCO, 8);
		spriteTimerDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SEIS, 8);
		spriteTimerDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SIETE, 8);
		spriteTimerDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(OCHO, 8);
		spriteTimerDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(NUEVE, 8);
		spriteTimerDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerDecena->setPosition(glm::vec2((34) * map->getTileSize(), (1) * map->getTileSize()));

		spriteTimerUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerUnidad->setNumberAnimations(10);
		spriteTimerUnidad->setAnimationSpeed(CERO, 8);
		spriteTimerUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(UNO, 8);
		spriteTimerUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(DOS, 8);
		spriteTimerUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(TRES, 8);
		spriteTimerUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CUATRO, 8);
		spriteTimerUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CINCO, 8);
		spriteTimerUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SEIS, 8);
		spriteTimerUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SIETE, 8);
		spriteTimerUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(OCHO, 8);
		spriteTimerUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(NUEVE, 8);
		spriteTimerUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerUnidad->setPosition(glm::vec2((35)* map->getTileSize(), (1)* map->getTileSize()));

		/////////////////POINTS//////////////////
		spritePointsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsUnidad->setNumberAnimations(10);
		spritePointsUnidad->setAnimationSpeed(CERO, 8);
		spritePointsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(UNO, 8);
		spritePointsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(DOS, 8);
		spritePointsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(TRES, 8);
		spritePointsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CUATRO, 8);
		spritePointsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CINCO, 8);
		spritePointsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SEIS, 8);
		spritePointsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SIETE, 8);
		spritePointsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(OCHO, 8);
		spritePointsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(NUEVE, 8);
		spritePointsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsUnidad->setPosition(glm::vec2((4)* map->getTileSize(), (1)* map->getTileSize()));

		spritePointsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsDecena->setNumberAnimations(10);
		spritePointsDecena->setAnimationSpeed(CERO, 8);
		spritePointsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsDecena->setAnimationSpeed(UNO, 8);
		spritePointsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsDecena->setAnimationSpeed(DOS, 8);
		spritePointsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsDecena->setAnimationSpeed(TRES, 8);
		spritePointsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CUATRO, 8);
		spritePointsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CINCO, 8);
		spritePointsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SEIS, 8);
		spritePointsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SIETE, 8);
		spritePointsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsDecena->setAnimationSpeed(OCHO, 8);
		spritePointsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsDecena->setAnimationSpeed(NUEVE, 8);
		spritePointsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsDecena->setPosition(glm::vec2((3)* map->getTileSize(), (1)* map->getTileSize()));

		////////////////////(////////////////////////////
	}
	else if (lev == 2){
		level = 2;
		timerLevel = 500;
		points = 0;
		personajes.clear();
		endedLevel = false;

		initShaders();
		map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);

		goomba = new Goomba();
		goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba->setPosition(glm::vec2(INIT_GOOMBA_X_TILES * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		goomba->setTileMap(map);

		star = new Star();
		star->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		star->setPosition(glm::vec2(INIT_STAR_X_TILES * map->getTileSize(), INIT_STAR_Y_TILES * map->getTileSize()));
		star->setTileMap(map);

		seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setPosition(glm::vec2(INIT_SETA_X_TILES * map->getTileSize(), INIT_SETA_Y_TILES * map->getTileSize()));
		seta->setTileMap(map);

		ktroopa = new Ktroopa();
		ktroopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ktroopa->setPosition(glm::vec2(INIT_KTROOPA_X_TILES * map->getTileSize(), INIT_KTROOPA_Y_TILES * map->getTileSize()));
		ktroopa->setTileMap(map);

		palo_bandera = new ObjetoEntorno();
		palo_bandera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(32, 320), "palo_bandera");
		palo_bandera->setPosition(glm::vec2((INIT_SETA_X_TILES - 1) * map->getTileSize(), (INIT_SETA_Y_TILES - 20) * map->getTileSize()));
		palo_bandera->setTileMap(map);

		////////////////////////////////////////////////7
		numbers.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spriteTimerCentena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerCentena->setNumberAnimations(10);
		spriteTimerCentena->setAnimationSpeed(CERO, 8);
		spriteTimerCentena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(UNO, 8);
		spriteTimerCentena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(DOS, 8);
		spriteTimerCentena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(TRES, 8);
		spriteTimerCentena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CUATRO, 8);
		spriteTimerCentena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(CINCO, 8);
		spriteTimerCentena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SEIS, 8);
		spriteTimerCentena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(SIETE, 8);
		spriteTimerCentena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(OCHO, 8);
		spriteTimerCentena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerCentena->setAnimationSpeed(NUEVE, 8);
		spriteTimerCentena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerCentena->setPosition(glm::vec2((33)* map->getTileSize(), (1)* map->getTileSize()));

		spriteTimerDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerDecena->setNumberAnimations(10);
		spriteTimerDecena->setAnimationSpeed(CERO, 8);
		spriteTimerDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(UNO, 8);
		spriteTimerDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(DOS, 8);
		spriteTimerDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(TRES, 8);
		spriteTimerDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CUATRO, 8);
		spriteTimerDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(CINCO, 8);
		spriteTimerDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SEIS, 8);
		spriteTimerDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(SIETE, 8);
		spriteTimerDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(OCHO, 8);
		spriteTimerDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerDecena->setAnimationSpeed(NUEVE, 8);
		spriteTimerDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerDecena->setPosition(glm::vec2((34)* map->getTileSize(), (1)* map->getTileSize()));

		spriteTimerUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spriteTimerUnidad->setNumberAnimations(10);
		spriteTimerUnidad->setAnimationSpeed(CERO, 8);
		spriteTimerUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(UNO, 8);
		spriteTimerUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(DOS, 8);
		spriteTimerUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(TRES, 8);
		spriteTimerUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CUATRO, 8);
		spriteTimerUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(CINCO, 8);
		spriteTimerUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SEIS, 8);
		spriteTimerUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(SIETE, 8);
		spriteTimerUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(OCHO, 8);
		spriteTimerUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spriteTimerUnidad->setAnimationSpeed(NUEVE, 8);
		spriteTimerUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spriteTimerUnidad->setPosition(glm::vec2((35)* map->getTileSize(), (1)* map->getTileSize()));

		/////////////////POINTS//////////////////
		spritePointsUnidad = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsUnidad->setNumberAnimations(10);
		spritePointsUnidad->setAnimationSpeed(CERO, 8);
		spritePointsUnidad->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(UNO, 8);
		spritePointsUnidad->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(DOS, 8);
		spritePointsUnidad->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(TRES, 8);
		spritePointsUnidad->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CUATRO, 8);
		spritePointsUnidad->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(CINCO, 8);
		spritePointsUnidad->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SEIS, 8);
		spritePointsUnidad->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(SIETE, 8);
		spritePointsUnidad->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(OCHO, 8);
		spritePointsUnidad->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsUnidad->setAnimationSpeed(NUEVE, 8);
		spritePointsUnidad->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsUnidad->setPosition(glm::vec2((4)* map->getTileSize(), (1)* map->getTileSize()));

		spritePointsDecena = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.0), &numbers, &texProgram);
		spritePointsDecena->setNumberAnimations(10);
		spritePointsDecena->setAnimationSpeed(CERO, 8);
		spritePointsDecena->addKeyframe(CERO, glm::vec2(0.f, 1.0f));
		spritePointsDecena->setAnimationSpeed(UNO, 8);
		spritePointsDecena->addKeyframe(UNO, glm::vec2(0.1f, 1.0f));
		spritePointsDecena->setAnimationSpeed(DOS, 8);
		spritePointsDecena->addKeyframe(DOS, glm::vec2(0.2f, 1.0f));
		spritePointsDecena->setAnimationSpeed(TRES, 8);
		spritePointsDecena->addKeyframe(TRES, glm::vec2(0.3f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CUATRO, 8);
		spritePointsDecena->addKeyframe(CUATRO, glm::vec2(0.4f, 1.0f));
		spritePointsDecena->setAnimationSpeed(CINCO, 8);
		spritePointsDecena->addKeyframe(CINCO, glm::vec2(0.5f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SEIS, 8);
		spritePointsDecena->addKeyframe(SEIS, glm::vec2(0.6f, 1.0f));
		spritePointsDecena->setAnimationSpeed(SIETE, 8);
		spritePointsDecena->addKeyframe(SIETE, glm::vec2(0.7f, 1.0f));
		spritePointsDecena->setAnimationSpeed(OCHO, 8);
		spritePointsDecena->addKeyframe(OCHO, glm::vec2(0.8f, 1.0f));
		spritePointsDecena->setAnimationSpeed(NUEVE, 8);
		spritePointsDecena->addKeyframe(NUEVE, glm::vec2(0.9f, 1.0f));
		spritePointsDecena->setPosition(glm::vec2((3)* map->getTileSize(), (1)* map->getTileSize()));
		/////////////////////////////////////////////////

		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		currentTime = 0.0f;

		personajes.push_back(goomba);
		//personajes.push_back(ktroopa);
		personajes.push_back(star);
		personajes.push_back(seta);
		personajes.push_back(nullptr); //necesario para que no pete al hacer desaparecer al ultimo elementod de la lista, comentar para probar

	}

	
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (level != 0) {
		player->update(deltaTime);
		for (Personaje* personaje : personajes) {
			if (personaje != nullptr) {
				personaje->update(deltaTime);
			}
		}

		vector<Personaje*>::iterator it = personajes.begin();
		while (it != personajes.end()) {
			Personaje* personaje = *it;
			if (!player->isInAnimacionDeadFunc() && personaje != nullptr && checkCollision(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
				string tipo = personaje->myType();
				if (tipo == "Star") {
					player->invencibility();
					Star* s = dynamic_cast<Star*>(personaje);
					int p = s->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
				else if (tipo == "Goomba") {
					if (player->isInvencibleFunc()) { //muere el enemigo
						//personaje->killed();
						Goomba* g = dynamic_cast<Goomba*>(personaje);
						int p = g->getPoints();
						points += p;
						actualizarPoints();
						it = personajes.erase(it);
						delete personaje;
					}
					else {
						if (esMuerte(player->getPosition(), personaje->getPosition(), player->getAltura(), 32)) {
							player->hasMadeKill();
							//personaje->hit();
							Goomba* g= dynamic_cast<Goomba*>(personaje);
							int p = g->getPoints();
							points += p;
							actualizarPoints();
							it = personajes.erase(it);
							/*
							personaje->hit();
							//delete personaje;
							*/
							delete personaje;
						}
						else {
							player->hit();
							++it;
						}
					}
				}
				else if (tipo == "Ktroopa") {
					if (player->isInvencibleFunc()) { //muere el enemigo
						//personaje->hit();
						Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
						int p = kt->getPoints();
						points += p;
						actualizarPoints();
						it = personajes.erase(it);
						delete personaje;
					}
					else {
						Ktroopa* ktroopa = dynamic_cast<Ktroopa*>(personaje);
						if (esMuerte(player->getPosition(), ktroopa->getPosition(), player->getAltura(), 32) && ktroopa->isAlive()) { //de tortuga a shell, le ha saltado en la cabeza
							ktroopa->killed();
							player->hasMadeKill();
						}
						else if (!ktroopa->isAlive() && !ktroopa->isShellMovingFunc()) { //es shell quieta y pasa a moverse, le ha tocado en lado izq o dcha
							if (isCollisionLeft(player->getPosition(), ktroopa->getPosition())) {
								ktroopa->moveShellToRight(true);
							}
							else {
								ktroopa->moveShellToRight(false);
							}
							Ktroopa* kt = dynamic_cast<Ktroopa*>(personaje);
							int p = kt->getPoints();
							points += p;
							actualizarPoints();
							player->kickShell();
						}
						else { //es shell en movimiento, es hit al player
							player->hit();
						}
						++it;
					}
				}
				else if (tipo == "Seta") {
					player->creceMario();
					Seta* s = dynamic_cast<Seta*>(personaje);
					int p = s->getPoints();
					points += p;
					actualizarPoints();
					it = personajes.erase(it);
					delete personaje;
				}
			}
			++it;
		}
		if (Game::instance().getKey('1')) {
			borrarPersonajes();
			init(1);
		}
		if (Game::instance().getKey('2')) {
			borrarPersonajes();
			init(2);
		}
		if (player->getPosition().x >= palo_bandera->getPosition().x) { //siguiente nivel
			if (!player->isChangingLevel() && level == 1) {
				player->animacionEndLevelFunc();
				++level;
				endedLevel = true;
			}
			else if (!player->isChangingLevel() && level == 2 && endedLevel) { //cambiamos de nivel, se cambia el mapa
				borrarPersonajes();
				nextLevel();
			}
			else if (!player->isChangingLevel() && level == 2) {
				player->animacionEndLevelFunc();
				++level;
				endedLevel = true;
			}
			else if (!player->isChangingLevel() && level == 3 && endedLevel) { //cambiamos de nivel, se cambia el mapa
				menus->showingCreditsFunc();
				level = 0;
			}
		}

		timerLevel = 20 - static_cast<int>(currentTime) / 1000;
		actualizarTimer();
	}
	else {
		int chosed;
		menus->update(deltaTime, chosed);
		if (chosed != 0) {
			init(chosed);
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (level != 0) {
		map->render();
		if (level == 1 || (level == 2 && player->isChangingLevel())) spriteResumenLevel1->render();
		else if (level == 2 || (level == 3 && player->isChangingLevel())) spriteResumenLevel2->render();
		for (Personaje* personaje : personajes) {
			if (personaje != nullptr) {
				personaje->render();
			}
		}
		palo_bandera->render();
		player->render();
	
		spriteTimerCentena->render();
		spriteTimerDecena->render();
		spriteTimerUnidad->render();
		spritePointsUnidad->render();
		spritePointsDecena->render();
	}
	else menus->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::checkCollision(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Calcula los l�mites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	// Verifica si hay colisi�n en el eje X
	bool collisionX = (left1 < right2) && (right1 > left2);

	// Verifica si hay colisi�n en el eje Y (considerando la altura)
	bool collisionY = (top1 < bottom2) && (bottom1 > top2);

	// Si hay colisi�n en ambos ejes, entonces hay colisi�n en 2D
	return collisionX && collisionY;
}

bool Scene::esMuerte(glm::vec2 posPlayer, glm::vec2 posEnemy, int alturaPlayer, int alturaEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Calcula los l�mites de los objetos en el eje Y (teniendo en cuenta la altura)
	float top1 = posPlayer.y - alturaPlayer;
	float bottom1 = posPlayer.y;
	float top2 = posEnemy.y - alturaEnemy;
	float bottom2 = posEnemy.y;

	if (bottom1-4 >= top2 && bottom1 <= top2+4 && (left1 < right2 && right1 > left2)) return true; //el 37 es porq es dif�cil q coincida justo a la misma altura, as� q le pongo una dif de 5 p�xeles (altura 32)
	else return false;
}

bool Scene::isCollisionLeft(glm::vec2 posPlayer, glm::vec2 posEnemy) {
	float left1 = posPlayer.x;
	float right1 = posPlayer.x + 32; // Debes definir el ancho del objeto
	float left2 = posEnemy.x;
	float right2 = posEnemy.x + 32; // Debes definir el ancho del objeto

	// Si hay colisi�n en ambos ejes, entonces hay colisi�n en 2D
	return left1 < left2 && right1 < right2;
}

void Scene::nextLevel() {
	init(2);
}

void Scene::borrarPersonajes() {
	vector<Personaje*>::iterator it2 = personajes.begin();
	while (it2 != personajes.end()) {
		Personaje* personaje = *it2;
		if (personaje != nullptr) {
			it2 = personajes.erase(it2);
			delete personaje;
		}
		++it2;
	}
}

void Scene::asignarSpriteNumber(Sprite* s, int n) {
	switch (n) {
		case(0):
			s->changeAnimation(CERO);
			break;
		case(1):
			s->changeAnimation(UNO);
			break;
		case(2):
			s->changeAnimation(DOS);
			break;
		case(3):
			s->changeAnimation(TRES);
			break;
		case(4):
			s->changeAnimation(CUATRO);
			break;
		case(5):
			s->changeAnimation(CINCO);
			break;
		case(6):
			s->changeAnimation(SEIS);
			break;
		case(7):
			s->changeAnimation(SIETE);
			break;
		case(8):
			s->changeAnimation(OCHO);
			break;
		case(9):
			s->changeAnimation(NUEVE);
			break;
		default:
			s->changeAnimation(CERO);
	}
}

void Scene::actualizarTimer() {
	int aux = timerLevel;
	asignarSpriteNumber(spriteTimerUnidad, timerLevel % 10);
	timerLevel = timerLevel / 10;
	asignarSpriteNumber(spriteTimerDecena, timerLevel % 10);
	timerLevel = timerLevel / 10;
	asignarSpriteNumber(spriteTimerCentena, timerLevel % 10);
	timerLevel = aux;
}

void Scene::actualizarPoints() {
	int aux = points;
	asignarSpriteNumber(spritePointsUnidad, points % 10);
	points = points / 10;
	asignarSpriteNumber(spritePointsDecena, points % 10);
	points = aux;
}

