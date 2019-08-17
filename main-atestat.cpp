#include<iostream>
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>

static const float Deg2Rad = 180.0f/3.141592f;

using namespace std;

class Math
{
public:
	
	static float Distanta(sf::Vector2f A,sf::Vector2f B)
	{
		return Modul(B-A);
	}
	static float Modul(sf::Vector2f V)
	{
		return sqrt(V.x*V.x+V.y*V.y);
	}
	static sf::Vector2f Normalize(sf::Vector2f V)
	{
		return V/Modul(V);
	}

	static bool Collision(sf::Vector2f centru1, float raza1, sf::Vector2f centru2, float raza2)
	{
		if(raza1+raza2 > Distanta(centru1,centru2))
			return true;
		return false;
	}

	static float Dot(sf::Vector2f u, sf::Vector2f v)
	{
		return u.x*v.x+u.y*v.y;
	}

	static float Cross(sf::Vector2f u, sf::Vector2f v)
	{
		return u.x*v.y-u.y*v.x;
	}

	static sf::Vector2f Reflect(sf::Vector2f n, sf::Vector2f v)
	{
		sf::Vector2f vperp, vparalopus;
		float modul_vparalopus;
		vperp = Dot(v,n)*n;
		modul_vparalopus = Cross(v,n);
		vparalopus.x = modul_vparalopus*(-n.y);
		vparalopus.y = modul_vparalopus*(n.x);
		return vperp+vparalopus;
	}
};

class Premiu
{
private:

	int m_type;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_hitboxRadius;

public:

	Premiu()
	{
		m_position = sf::Vector2f();
		m_hitboxRadius = 10.0f;
	}

	void init(float radius, sf::Vector2f position)
	{
		m_hitboxRadius = radius;
		m_position = position;
	}

	void draw(sf::RenderTarget& renderTarget)
	{
		sf::CircleShape shape(m_hitboxRadius,60);
		shape.setFillColor(sf::Color::Cyan);
		shape.setOrigin(sf::Vector2f(m_hitboxRadius,m_hitboxRadius));
		shape.setPosition(m_position);
		renderTarget.draw(shape);
	}

	sf::Vector2f getPosition()
	{
		return m_position;
	}

	float getHitboxRadius()
	{
		return m_hitboxRadius;
	}
};

class Enemy
{
private:
	float m_hitboxRadius;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
	float m_directionAngle;
	sf::Color m_color;

public:

	Enemy()
	{
		m_position = sf::Vector2f();
		m_velocity = sf::Vector2f();
		m_hitboxRadius = 10.0f;
		m_directionAngle = 0.0f;
		m_color = sf::Color::White;
	}

	void init(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
	{
		m_hitboxRadius = radius;
		m_velocity = velocity;
		m_position = position;
		m_color = color;
	}

	void update(float dt)
	{
		m_position += m_velocity * dt;
	}

	void draw(sf::RenderTarget& renderTarget)
	{
		sf::CircleShape shape(m_hitboxRadius);
		shape.setFillColor(m_color);
		shape.setOrigin(sf::Vector2f(m_hitboxRadius,m_hitboxRadius));
		shape.setPosition(m_position);
		renderTarget.draw(shape);
	}

	void setVelocity(float angle, float magnitude)
	{
		angle = angle*Deg2Rad;
		m_directionAngle = angle;
		m_velocity.x = magnitude*cos(angle);
		m_velocity.y = magnitude*sin(angle);
	}

	sf::Vector2f getPosition()
	{
		return m_position;
	}

	void setPosition(sf::Vector2f newPosition)
	{
		m_position = newPosition;
	}

	float getHitboxRadius()
	{
		return m_hitboxRadius;
	}

	void reflect(sf::Vector2f n)
	{
		m_velocity = Math::Reflect(n,sf::Vector2f(-m_velocity.x,-m_velocity.y));
	}
};

class Character
{
private:
	sf::Color m_color;
	float m_hitboxRadius;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;

public:

	Character()
	{
		m_color = sf::Color::White;
		m_hitboxRadius = 15.0f;
		m_velocity = sf::Vector2f(0.0f,0.0f);
		m_position = sf::Vector2f(0.0f,0.0f);
	}

	void init(sf::Color color, float radius, sf::Vector2f velocity, sf::Vector2f position)
	{
		m_color = color;
		m_hitboxRadius = radius;
		m_velocity = velocity;
		m_position = position;
	}
	
	void updateVelocity(float dt)
	{
		sf::Vector2f vel(0.0f,0.0f);
		float velocityAmplifier=1.0f;
		float velocitySlowDown=1.0f;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			vel.x= -1;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
			vel.x= +1;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
			vel.y= -1;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
			vel.y= +1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			velocitySlowDown = 0.33f;
		}
		if(Math::Modul(vel)>0)
			vel=Math::Normalize(vel);
		vel *= 400.0f*velocityAmplifier*velocitySlowDown;
		setVelocity(vel);
	}

	void update(float dt)
	{
		m_position += m_velocity*dt;
	}

	void draw(sf::RenderTarget& renderTarget)
	{
		sf::CircleShape shape(m_hitboxRadius);
		shape.setFillColor(m_color);
		shape.setOrigin(sf::Vector2f(m_hitboxRadius,m_hitboxRadius));
		shape.setPosition(m_position);
		renderTarget.draw(shape);
	}

	void setVelocity(sf::Vector2f newVelocity)
	{
		m_velocity = newVelocity;
	}

	sf::Vector2f getPosition()
	{
		return m_position;
	}

	void setPosition(sf::Vector2f newPosition)
	{
		m_position = newPosition;
	}

	float getHitboxRadius()
	{
		return m_hitboxRadius;
	}

	sf::Vector2f getVelocity()
	{
		return m_velocity;
	}
};

class GameItself
{
public:
	sf::RenderWindow m_window;
	sf::Texture m_splashscreenTexture;
	sf::Sprite m_splashscreenSprite;
	sf::Font m_myFont;
	sf::SoundBuffer m_deathSoundBuffer;
	sf::Sound m_deathsound;
	Character m_character;
	Premiu m_premiu;
	int m_nrPremiiLuate;
	Enemy m_enemy[300];
	int m_nrEnemies;

	int m_currentState;

	float m_borderLength;
	float m_enemySafetyDistance;
	float m_premiuSpawnDistance;
	float m_enemySpeed;
	float m_charEnemyCollisionAdjustment;
	float m_difficulty;

	enum GameState
	{
		INIT = 0,
		SPLASHSCREEN,
		MENIU,
		CREDITS,
		GAME,
		GAMEOVER,
		COUNT
	};

	void init()
	{
		m_currentState = INIT;
		srand((unsigned)time(NULL));
		m_borderLength = 5.0f;
		m_enemySafetyDistance = 300.0f;
		m_premiuSpawnDistance = 100.0f;
		m_enemySpeed = 200.0f;
		m_charEnemyCollisionAdjustment = 3.0f;
		m_difficulty = 10.0f;
		m_nrPremiiLuate = 0;
		m_nrEnemies = 0;

		bool fullscreen = false;
		if(fullscreen)
			m_window.create(sf::VideoMode::getDesktopMode(), "Dodge", sf::Style::Fullscreen);
		else
			m_window.create(sf::VideoMode(1024,576), "Dodge", sf::Style::Titlebar);
		if(m_splashscreenTexture.loadFromFile("Resources\\splashscreen.png")==false)
			std::cout<<"could not load splashscreen.png. error in gameitself.init"<<std::endl;
		if(m_myFont.loadFromFile("Resources\\aldhabi.ttf")==false)
			std::cout<<"could not load aldhabi.tff. error in gameitself.init"<<std::endl;
		if(!m_deathSoundBuffer.loadFromFile("Resources\\death.wav"))
			std::cout<<"could not load death.mp3. error in gameitself.init"<<std::endl;
		m_deathsound.setBuffer(m_deathSoundBuffer);
		m_splashscreenSprite.setTexture(m_splashscreenTexture);
		m_splashscreenSprite.setPosition(0.0f, 0.0f);
		m_splashscreenSprite.setScale(1.0f/m_splashscreenTexture.getSize().x*m_window.getSize().x, 1.0f/m_splashscreenTexture.getSize().y*m_window.getSize().y);

		spawnPremiu();
		m_character.init(sf::Color(0,100,0),15,sf::Vector2f(0.0f,0.0f),sf::Vector2f(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f));
	}

	void spawnPremiu()
	{
		do 
		{
			m_premiu.init(15.0f, sf::Vector2f(
				(float)(rand() % (int)((m_window.getSize().x - 2*(int)m_premiu.getHitboxRadius()) - 2*m_borderLength) + m_borderLength + m_premiu.getHitboxRadius()),
				(float)(rand() % (int)((m_window.getSize().y - 2*(int)m_premiu.getHitboxRadius()) - 2*m_borderLength) + m_borderLength + m_premiu.getHitboxRadius())));	
		}while (Math::Distanta(m_premiu.getPosition(),m_character.getPosition()) < m_premiuSpawnDistance);
	}

	void spawnEnemy()
	{
		float enemy_speed = m_enemySpeed + m_difficulty*m_nrEnemies;
		sf::Color colorEasy=sf::Color::Yellow, colorHard=sf::Color::Red;
		float t=1.0f-1.0f/((m_nrEnemies+1)*0.5f);
		sf::Color color;
		color.r=(sf::Uint8)(colorEasy.r*(1-t)+colorHard.r*t);
		color.g=(sf::Uint8)(colorEasy.g*(1-t)+colorHard.g*t);
		color.b=(sf::Uint8)(colorEasy.b*(1-t)+colorHard.b*t);

		do 
		{
			m_enemy[m_nrEnemies].init(15.0f,sf::Vector2f(),sf::Vector2f(),color);
			m_enemy[m_nrEnemies].setPosition(sf::Vector2f(
				(float)(rand() % (int)((m_window.getSize().x - (int)m_enemy[m_nrEnemies].getHitboxRadius()) - 2*m_borderLength) + m_borderLength + m_premiu.getHitboxRadius()),
				(float)(rand() % (int)((m_window.getSize().y - (int)m_enemy[m_nrEnemies].getHitboxRadius()) - 2*m_borderLength) + m_borderLength + m_premiu.getHitboxRadius())));
		}while(Math::Distanta(m_enemy[m_nrEnemies].getPosition(), m_character.getPosition()) < m_enemySafetyDistance + m_enemy[m_nrEnemies].getHitboxRadius());
		m_enemy[m_nrEnemies].setVelocity((float)rand() / (float)RAND_MAX * 360, enemy_speed);
		m_nrEnemies++;
		if(m_nrEnemies==299)
			m_currentState = GAMEOVER;
	}

	void gameReset()
	{
		m_nrPremiiLuate = 0;
		m_character.init(sf::Color(0,100,0),15,sf::Vector2f(0.0f,0.0f),sf::Vector2f(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f));
		spawnPremiu();
		m_nrEnemies = 0;
	}

	void update(float dt)
	{
		switch(m_currentState)
		{
		case INIT:
			{
				m_currentState = SPLASHSCREEN;
				break;
			}
		case SPLASHSCREEN:
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					m_currentState = MENIU;
				}
				break;
			}
		case MENIU:
			{
				gameReset();
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				{
					m_currentState = GAME;
				}
				break;
			}
		case CREDITS:
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					m_currentState = MENIU;
				break;
			}
		case GAME:
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					m_currentState = MENIU;
				m_character.updateVelocity(dt);
				m_character.update(dt);

				if(m_character.getPosition().x - m_borderLength < m_character.getHitboxRadius())
				{
					m_character.setPosition(sf::Vector2f(m_borderLength + m_character.getHitboxRadius(), m_character.getPosition().y));
				}
				if(m_window.getSize().x - m_borderLength - m_character.getPosition().x < m_character.getHitboxRadius())
				{
					m_character.setPosition(sf::Vector2f(m_window.getSize().x - m_borderLength - m_character.getHitboxRadius(), m_character.getPosition().y));
				}
				if(m_character.getPosition().y - m_borderLength < m_character.getHitboxRadius())
				{
					m_character.setPosition(sf::Vector2f(m_character.getPosition().x, m_borderLength + m_character.getHitboxRadius()));
				}
				if(m_window.getSize().y - m_borderLength - m_character.getPosition().y < m_character.getHitboxRadius())
				{
					m_character.setPosition(sf::Vector2f(m_character.getPosition().x, m_window.getSize().y - m_borderLength - m_character.getHitboxRadius()));
				}

				for(int i=0; i<m_nrEnemies;i++)
				{
					if(Math::Collision(m_enemy[i].getPosition(),m_enemy[i].getHitboxRadius() - m_charEnemyCollisionAdjustment,m_character.getPosition(),m_character.getHitboxRadius()))
					{	
						m_deathsound.play();
						m_currentState = GAMEOVER;
					}
					if(m_enemy[i].getPosition().x - m_borderLength < m_enemy[i].getHitboxRadius())
					{
						m_enemy[i].setPosition(sf::Vector2f(m_borderLength + m_enemy[i].getHitboxRadius(), m_enemy[i].getPosition().y));
						m_enemy[i].reflect(sf::Vector2f(1.0f,0.0f));
					}
					if(m_window.getSize().x - m_borderLength - m_enemy[i].getPosition().x < m_enemy[i].getHitboxRadius())
					{
						m_enemy[i].setPosition(sf::Vector2f(m_window.getSize().x - m_borderLength - m_enemy[i].getHitboxRadius(), m_enemy[i].getPosition().y));
						m_enemy[i].reflect(sf::Vector2f(-1.0f,0.0f));
					}
					if(m_enemy[i].getPosition().y - m_borderLength < m_enemy[i].getHitboxRadius())
					{
						m_enemy[i].setPosition(sf::Vector2f(m_enemy[i].getPosition().x,m_borderLength + m_enemy[i].getHitboxRadius()));
						m_enemy[i].reflect(sf::Vector2f(0.0f,-1.0f));
					}
					if(m_window.getSize().y - m_borderLength - m_enemy[i].getPosition().y < m_enemy[i].getHitboxRadius())
					{
						m_enemy[i].setPosition(sf::Vector2f(m_enemy[i].getPosition().x, m_window.getSize().y - m_borderLength - m_enemy[i].getHitboxRadius()));
						m_enemy[i].reflect(sf::Vector2f(0.0f,1.0f));
					}
					m_enemy[i].update(dt);
				}

				if(Math::Collision(m_character.getPosition(), m_character.getHitboxRadius(), m_premiu.getPosition(), m_premiu.getHitboxRadius()))
				{
					spawnEnemy();
					m_nrPremiiLuate++;
					spawnPremiu();
				}
				break;
			}
		case GAMEOVER:
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
					m_currentState = MENIU;

				break;
			}
		}
	}

	void draw()
	{
		m_window.clear(sf::Color::Black);
		switch(m_currentState)
		{
		case INIT:
			{
				break;
			}
		case SPLASHSCREEN:
			{
				m_window.draw(m_splashscreenSprite);
				break;
			}
		case MENIU:
			{
				sf::RectangleShape playButton(sf::Vector2f(m_window.getSize().x*0.15f, m_window.getSize().y*0.10f));
				sf::RectangleShape exitButton(sf::Vector2f(m_window.getSize().x*0.15f, m_window.getSize().y*0.10f));
				sf::RectangleShape creditsButton(sf::Vector2f(m_window.getSize().x*0.15f, m_window.getSize().y*0.10f));
				playButton.setOrigin(playButton.getLocalBounds().width/2.0f, playButton.getLocalBounds().height/2.0f);
				creditsButton.setOrigin(creditsButton.getLocalBounds().width/2.0f, creditsButton.getLocalBounds().height/2.0f);
				exitButton.setOrigin(exitButton.getLocalBounds().width/2.0f, exitButton.getLocalBounds().height/2.0f);
				playButton.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f-m_window.getSize().y*0.10f);
				creditsButton.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.10f);
				exitButton.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.30f);
				playButton.setOutlineThickness(m_window.getSize().x*0.002f);
				creditsButton.setOutlineThickness(m_window.getSize().x*0.002f);
				exitButton.setOutlineThickness(m_window.getSize().x*0.002f);
				playButton.setOutlineColor(sf::Color(0,255,0));
				creditsButton.setOutlineColor(sf::Color(0,255,0));
				exitButton.setOutlineColor(sf::Color(0,255,0));
				playButton.setFillColor(sf::Color(0,100,0));
				creditsButton.setFillColor(sf::Color(0,100,0));
				exitButton.setFillColor(sf::Color(0,100,0));
				

				sf::Text playText,creditsText,exitText;
				playText.setFont(m_myFont);
				creditsText.setFont(m_myFont);
				exitText.setFont(m_myFont);
				playText.setString("Play");
				creditsText.setString("Credits");
				exitText.setString("Exit");
				playText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				creditsText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				exitText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				playText.setOrigin(playText.getLocalBounds().width/2.0f,playText.getLocalBounds().height/2.0f);
				creditsText.setOrigin(creditsText.getLocalBounds().width/2.0f,creditsText.getLocalBounds().height/2.0f);
				exitText.setOrigin(exitText.getLocalBounds().width/2.0f,exitText.getLocalBounds().height/2.0f);
				playText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f-m_window.getSize().y*0.13f);
				creditsText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.07f);
				exitText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.27f);
				playText.setColor(sf::Color(0,255,0));
				creditsText.setColor(sf::Color(0,255,0));
				exitText.setColor(sf::Color(0,255,0));

				sf::Vector2i cursorLocalPosition = sf::Mouse::getPosition(m_window);
				if(	cursorLocalPosition.x > playButton.getPosition().x - m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y > playButton.getPosition().y - m_window.getSize().y*0.10f/2.0f &&
					cursorLocalPosition.x < playButton.getPosition().x + m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y < playButton.getPosition().y + m_window.getSize().y*0.10f/2.0f)
				{
					playButton.setFillColor(sf::Color::Yellow);
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						m_currentState = GAME;
				}
				
				if(	cursorLocalPosition.x > creditsButton.getPosition().x - m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y > creditsButton.getPosition().y - m_window.getSize().y*0.10f/2.0f &&
					cursorLocalPosition.x < creditsButton.getPosition().x + m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y < creditsButton.getPosition().y + m_window.getSize().y*0.10f/2.0f)
				{
					creditsButton.setFillColor(sf::Color::Yellow);
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						m_currentState = CREDITS;
				}

				if(	cursorLocalPosition.x > exitButton.getPosition().x - m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y > exitButton.getPosition().y - m_window.getSize().y*0.10f/2.0f &&
					cursorLocalPosition.x < exitButton.getPosition().x + m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y < exitButton.getPosition().y + m_window.getSize().y*0.10f/2.0f)
				{
					exitButton.setFillColor(sf::Color::Yellow);
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						m_window.close();
				}

				m_window.draw(playButton);
				m_window.draw(creditsButton);
				m_window.draw(exitButton);

				m_window.draw(playText);
				m_window.draw(creditsText);
				m_window.draw(exitText);

				sf::Text dodgeText;
				dodgeText.setFont(m_myFont);

				dodgeText.setString("DODGE");
				dodgeText.setStyle(sf::Text::Underlined);
				dodgeText.setCharacterSize((unsigned int)(m_window.getSize().x*0.1f));
				dodgeText.setOrigin(dodgeText.getLocalBounds().width/2.0f,dodgeText.getLocalBounds().height/2.0f);
				dodgeText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f-m_window.getSize().y*0.40f);
				dodgeText.setColor(sf::Color(0,100,0));
				m_window.draw(dodgeText);

				break;
			}
		case CREDITS:
			{
				sf::RectangleShape backButton(sf::Vector2f(m_window.getSize().x*0.15f, m_window.getSize().y*0.10f));
				backButton.setOrigin(backButton.getLocalBounds().width/2.0f, backButton.getLocalBounds().height/2.0f);
				backButton.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.20f);
				backButton.setOutlineThickness(m_window.getSize().x*0.002f);
				backButton.setOutlineColor(sf::Color(0,255,0));
				backButton.setFillColor(sf::Color(0,100,0));

				sf::Text backText;
				backText.setFont(m_myFont);

				backText.setString("Jinga Nicolae");
				backText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				backText.setOrigin(backText.getLocalBounds().width/2.0f,backText.getLocalBounds().height/2.0f);
				backText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f-m_window.getSize().y*0.13f);
				backText.setColor(sf::Color(0,255,0));
				m_window.draw(backText);

				backText.setString("Back");
				backText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				backText.setOrigin(backText.getLocalBounds().width/2.0f,backText.getLocalBounds().height/2.0f);
				backText.setPosition(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f+m_window.getSize().y*0.17f);
				backText.setColor(sf::Color(0,255,0));
				

				sf::Vector2i cursorLocalPosition = sf::Mouse::getPosition(m_window);
				if(cursorLocalPosition.x > backButton.getPosition().x - m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y > backButton.getPosition().y - m_window.getSize().y*0.10f/2.0f &&
					cursorLocalPosition.x < backButton.getPosition().x + m_window.getSize().x*0.15f/2.0f && 
					cursorLocalPosition.y < backButton.getPosition().y + m_window.getSize().y*0.10f/2.0f)
				{
					backButton.setFillColor(sf::Color::Yellow);
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						m_currentState = MENIU;
				}

				m_window.draw(backButton);
				m_window.draw(backText);

				break;
			}
		case GAME:
			{
				sf::RectangleShape borderTop(sf::Vector2f((float)m_window.getSize().x,m_borderLength));
				sf::RectangleShape borderBottom(sf::Vector2f((float)m_window.getSize().x,m_borderLength));
				sf::RectangleShape borderLeft(sf::Vector2f(m_borderLength,(float)m_window.getSize().y));
				sf::RectangleShape borderRight(sf::Vector2f(m_borderLength,(float)m_window.getSize().y));
				borderTop.setPosition(0.0f,0.0f);
				borderBottom.setPosition(0.0f,m_window.getSize().y-m_borderLength);
				borderLeft.setPosition(0.0f,0.0f);
				borderRight.setPosition(m_window.getSize().x-m_borderLength,0.0f);
				borderTop.setFillColor(sf::Color(0,100,0));
				borderBottom.setFillColor(sf::Color(0,100,0));
				borderLeft.setFillColor(sf::Color(0,100,0));
				borderRight.setFillColor(sf::Color(0,100,0));
				m_window.draw(borderTop);
				m_window.draw(borderBottom);
				m_window.draw(borderLeft);
				m_window.draw(borderRight);

				for(int i=0;i<m_nrEnemies;i++)
					m_enemy[i].draw(m_window);
				m_premiu.draw(m_window);
				m_character.draw(m_window);
				break;
			}
		case GAMEOVER:
			{
				m_window.clear(sf::Color::Black);

				sf::Text gameOverText;
				gameOverText.setFont(m_myFont);
				gameOverText.setString("Your Score:");
				gameOverText.setCharacterSize((unsigned int)(m_window.getSize().x*0.1f));
				gameOverText.setOrigin(gameOverText.getLocalBounds().width/2.0f, gameOverText.getLocalBounds().height/2.0f);
				gameOverText.setPosition(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f-m_window.getSize().y*0.33f);
				gameOverText.setColor(sf::Color(0,100,0));
				m_window.draw(gameOverText);
				
				sf::Text gameOverScore;
				gameOverScore.setFont(m_myFont);
				char xAsString[20];
				_itoa_s(m_nrPremiiLuate,xAsString,10);
				gameOverScore.setString(xAsString);
				gameOverScore.setCharacterSize((unsigned int)(m_window.getSize().x*0.2f));
				gameOverScore.setOrigin(gameOverScore.getLocalBounds().width/2.0f, gameOverScore.getLocalBounds().height/2.0f);
				gameOverScore.setPosition(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f-m_window.getSize().y*0.15f);
				gameOverScore.setColor(sf::Color(0,100,0));
				m_window.draw(gameOverScore);

				sf::Text continueText;
				continueText.setFont(m_myFont);
				continueText.setString("Press Enter to Continue...");
				continueText.setCharacterSize((unsigned int)(m_window.getSize().x*0.03f));
				continueText.setOrigin(continueText.getLocalBounds().width/2.0f, continueText.getLocalBounds().height/2.0f);
				continueText.setPosition(m_window.getSize().x/2.0f,m_window.getSize().y/2.0f+m_window.getSize().y*0.40f);
				continueText.setColor(sf::Color(255,255,255));
				m_window.draw(continueText);

				break;
			}
		}
	}

};

int main()
{
	GameItself game;
	game.init();
	sf::Clock clock;
	float dt=0.0f;
	cout <<"Character = "<< sizeof(Character) << " bytes" << endl;
	cout <<"Enemies = "<< sizeof(Enemy) << " bytes" << endl;
	cout <<"Premiu = "<< sizeof(Premiu) << " bytes" << endl;
	cout <<"GameItself = "<< sizeof(GameItself) << " bytes" << endl;
	cout <<"Math = "<< sizeof(Math) << " bytes" << endl;

	while (game.m_window.isOpen())
	{
		sf::Event event;
		while (game.m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				game.m_window.close();
		}

		dt=clock.getElapsedTime().asSeconds();
		clock.restart();
		game.update(dt);
		game.draw();
		game.m_window.display();
	}
	return 0;
}