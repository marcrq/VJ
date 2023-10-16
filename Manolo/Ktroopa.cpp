#include "Ktroopa.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ACCELERATION 0.002f
#define MAX_VEL 3.f
#define TIME_RESURRECTION 4.f


enum KtroopaAnims
{
	MOVE_RIGHT, MOVE_LEFT
};


	void Ktroopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
		velocity = 1.f;
		timeSinceDead = 0.f;
		isDead = false;
		vaIzq = true;
		conteoCambiosAnimacion = 0;
		isShellMoving = false;

		ktroopa.loadFromFile("images/ktroopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.5, 0.5), &ktroopa, &shaderProgram);

		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));

		sprite->changeAnimation(1);
		tileMapDispl = tileMapPos;

		tileMapDispl = tileMapPos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));

		shell.loadFromFile("images/shell2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		shellSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &shell, &shaderProgram);
	}

	void Ktroopa::update(int deltaTime) {
		getCorrectSprite()->update(deltaTime);


		if (isDead && !isInAnimacionAlternarModo && !isShellMoving) {
			timeSinceDead += deltaTime / 1000.0;
			if (timeSinceDead >= TIME_RESURRECTION) {
				isDead = false;
				animationResurrection();
			}
		}
		else {
			if (isInAnimacionAlternarModo) {
				timerAnimacionAlternarModo += deltaTime / 1000.0;
				if (timerAnimacionAlternarModo >= 1.f) {
					isInAnimacionAlternarModo = false;
					velocity = 1.f;
					isDead = false;
				}
				else {
					if (timerAnimacionAlternarModo > 0.05 && timerAnimacionAlternarModo < 0.2 && conteoCambiosAnimacion == 0) {
						isDead = !isDead;
						++conteoCambiosAnimacion;
					}
					else if (timerAnimacionAlternarModo > 0.2 && timerAnimacionAlternarModo < 0.4 && conteoCambiosAnimacion == 1) {
						isDead = !isDead;
						++conteoCambiosAnimacion;
					}
					else if (timerAnimacionAlternarModo > 0.4 && timerAnimacionAlternarModo < 0.6f && conteoCambiosAnimacion == 2) {
						isDead = !isDead;
						++conteoCambiosAnimacion;
					}
					else if (timerAnimacionAlternarModo > 0.6 && timerAnimacionAlternarModo < 0.8 && conteoCambiosAnimacion == 3) {
						isDead = !isDead;
						++conteoCambiosAnimacion;
					}
				}
			}
			else {
				posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y));

				if (vaIzq) {
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
					{
						//posPlayer.x += 2;
						posPlayer.x += velocity;
						vaIzq = false;
						sprite->changeAnimation(MOVE_RIGHT);
					}
					else {
						posPlayer.x -= velocity;
					}
				}
				else {
					if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
					{
						//posPlayer.x += 2;
						posPlayer.x -= velocity;
						vaIzq = true;
						sprite->changeAnimation(MOVE_LEFT);
					}
					else {
						posPlayer.x += velocity;
					}
				}
			}
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 32)));
			shellSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
			}
	}

	void Ktroopa::render() {
		getCorrectSprite()->render();
	}

	void Ktroopa::hit() {
		/*isDead = true;
		timeSinceDead = 0.f;
		velocity = 0.f;*/
	}

	void Ktroopa::killed() {
		isDead = true;
		timeSinceDead = 0.f;
		velocity = 0.f;
	}

	bool Ktroopa::isShellMovingFunc() {
		return isShellMoving;
	}

	bool Ktroopa::isAlive() {
		return !isDead;
	}

	string Ktroopa::myType() {
		return "Ktroopa";
	}

	Sprite* Ktroopa::getCorrectSprite() {
		if (!isDead) {
			return sprite;
		}
		else return shellSprite;
	}

	void Ktroopa::animationResurrection() {
		isInAnimacionAlternarModo = true;
		timerAnimacionAlternarModo = 0.f;
		conteoCambiosAnimacion = 0;
	}

	void Ktroopa::moveShellToRight(bool toRight) {
		isDead = true;
		velocity = 3.f;
		isShellMoving = true;
		if(toRight) vaIzq = false;
		else vaIzq = true;
	}