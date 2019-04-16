//============================================================================
// Name        : Asteroids.cpp
// Author      : Ken LAI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

const int H = 800;
const int W = 1200;

float DEGTORAD = 0.0174353f;

int score = 0;

Font global_font;

string status = "main";

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;	// obj that its appearance would change
	vector<IntRect> frames;

	Animation(){}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
		:Frame(0), speed(Speed)
	{
		for (int i = 0; i < count; i++)
			frames.push_back(IntRect(x+i*w, y, w, h));

		sprite.setTexture(t);
		sprite.setOrigin(w/2,h/2);
		sprite.setTextureRect(frames[0]);

	}

	void update() // iterate the image to show
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect(frames[int(Frame)]);
	}
};

class Entity
{
public:
	float x, y, dx, dy, R, angle;
	bool life;
	string name;
	Animation anim;

	Entity(){life=1;}

	void settings(Animation &a, int X, int Y, float Angle=0, int radius =1)
	{
		x=X; y=Y; anim=a;
		angle=Angle; R=radius;
	}

	virtual void update(){};

	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x,y);
		anim.sprite.setRotation(angle+90);
		app.draw(anim.sprite);
	}

	virtual ~Entity(){}  // virtual destructor should be included in the base class

};

class asteroid : public Entity
{
public:
	asteroid()
	{
		dx=rand()%8-4;
		dy=rand()%8-4;
		name="asteroid";
	}

	void update()
	{
		x+=dx;
		y+=dy;
		if (x>W) x = 0;
		if (x<0) x = W;
		if (y > H) y = 0;
		if (y < 0) y = H;
	}

};

class bullet: public Entity
{
public:
	bullet()
	{
		name="bullet";
	}
	void update()
	{
		dx=cos(angle*DEGTORAD)*6;
		dy=sin(angle*DEGTORAD)*6;
		x+=dx;
		y+=dy;

		if (x>W||x<0||y>H||y<0) life = 0;
	}

};

class Score
{
public:
	Score()
	:score(0),word("Score: ")
{
	word.append(to_string(score));
	text_score.setString(word);
	text_score.setCharacterSize(30);
	text_score.setStyle(sf::Text::Bold);
	text_score.setFillColor(sf::Color::Yellow);
	text_score.setOrigin(-10.0,0);
}

	void update(){score += 10;update_word();}

	void reset(){score = 0;update_word();}

	int getScore() const{ return score;}

	void draw(RenderWindow &app) const {app.draw(text_score);}

	void setOrigin(float x,float y){text_score.setOrigin(x,y);}

	void setFont(Font& f) {text_score.setFont(f);}


private:
	int score;
	string word;
	Font fontOfWord;
	Text text_score;

	void update_word(){word = "Score: ";
						word.append(to_string(score));
						text_score.setString(word);}
};

class Main_text
{
public:
	Main_text()
{
	title.setString("Asteroid game");
	title.setFillColor(sf::Color::Yellow);
	title.setFont(global_font);
	title.setOrigin(-450,-200);
	title.setCharacterSize(50);
	descirption.setString("Control:\n-arrow keys- to move\n-space key- to fire\n-H key- to return");
	descirption.setFillColor(sf::Color::Yellow);
	descirption.setFont(global_font);
	descirption.setOrigin(-10,-700);
	descirption.setCharacterSize(20);
	for (int i =0 ; i < strVec.size();i++){
		Text *t = new Text;
		t->setFont(global_font);
		t->setCharacterSize(30);
		t->setFillColor(sf::Color::Yellow);
		t->setString(strVec[i]);
		t->setOrigin(-500,-350-i*50);
		txtptr.push_back(t);
	}
	Text *e = txtptr.at(0);
	e->setFillColor(sf::Color::Cyan);
}

	void up(){  setYellow(*(txtptr[position]));
				int p = position - 1;
				position = (p > 0)? p : 0;
				setCyan(*(txtptr[position]));}

	void down(){ setYellow(*(txtptr[position]));
				int p = position + 1;
				position = (p < 2)? p : 2;
				setCyan(*(txtptr[position]));}

	const Text& getText(size_t i) const
	{return *(txtptr[i]);}

	const int getSize() const {return txtptr.size();}

	void draw(RenderWindow &app){
		app.draw(title);
		app.draw(descirption);
		for(auto i:txtptr) app.draw(*i);}

	const int getPosition() const {return position;}

private:
	Text title ,descirption;
	vector<string> strVec = {"START GAME", "OPTION", "QUIT"};
	vector<Text*> txtptr;
	int position = 0;
	void reset(){Main_text();}
	void setCyan(Text& t){t.setFillColor(sf::Color::Cyan);}
	void setYellow(Text& t){t.setFillColor(sf::Color::Yellow);}

};

class Opt_text
{
public:
	Opt_text()
{
	t1.setFont(global_font);
	t1.setCharacterSize(30);
	t1.setFillColor(sf::Color::Cyan);
	t1.setString("Astetoid number ");
	t1.setOrigin(-400,-350);
	t2.setFont(global_font);
	t2.setCharacterSize(30);
	t2.setFillColor(sf::Color::Yellow);
	t2.setString("Return");
	t2.setOrigin(-500,-400);
	txtptr[0] = &t1;
	txtptr[1] = &t2;
	c1.setPointCount(3);
	c1.setPoint(0, sf::Vector2f(650, 370));
	c1.setPoint(1, sf::Vector2f(670, 355));
	c1.setPoint(2, sf::Vector2f(670, 385));
	c1.setFillColor(sf::Color::Yellow);
	c2.setPointCount(3);
	c2.setPoint(0, sf::Vector2f(760, 370));
	c2.setPoint(1, sf::Vector2f(740, 355));
	c2.setPoint(2, sf::Vector2f(740, 385));
	c2.setFillColor(sf::Color::Yellow);
	t3.setFont(global_font);
	t3.setCharacterSize(30);
	t3.setFillColor(sf::Color::Yellow);
	t3.setString(to_string(noOfAst));
	t3.setOrigin(-690,-350);

}
	void up(){  setYellow(*(txtptr[position]));
				int p = position - 1;
				position = (p > 0)? p : 0;
				setCyan(*(txtptr[position]));}

	void down(){ setYellow(*(txtptr[position]));
				int p = position + 1;
				position = (p < 1)? p : 1;
				setCyan(*(txtptr[position]));}
	void left(){if (position == 0) {noOfAst>5 ? --noOfAst : 5;
					t3.setString(to_string(noOfAst));}}
	void right(){if (position == 0) {noOfAst < 30? ++noOfAst :30;
					t3.setString(to_string(noOfAst));}}

	const Text& getText(int i) const
	{return *(txtptr[i]);}
	const int getSize() const {return sizeof(txtptr)/sizeof(txtptr[0]);}
	void draw(RenderWindow &app){
		for(int i = 0; i < getSize(); i++)
			{app.draw(*(txtptr[i]));}
		app.draw(c1);app.draw(c2);app.draw(t3);}
	const int getPosition(){return position;}
	const int getEnemies(){return noOfAst;}
private:
	Text t1, t2, t3;
	Text *txtptr[2];
	ConvexShape c1,c2;
	int position = 0; int  noOfAst = 15;
	void reset(){Main_text();}
	void setCyan(Text& t){t.setFillColor(sf::Color::Cyan);}
	void setYellow(Text& t){t.setFillColor(sf::Color::Yellow);}
};

bool isCollide(Entity *a, Entity *b) // try to make rectangle boxes for collision check
{
	return (b->x - a->x)*(b->x - a->x)+
		   (b->y - a->y)*(b->y - a->y)<
		   (a->R + b->R)*(a->R + b->R);
}

bool isCollide(Entity *a, Sprite& b)
{
	float R2 = 20;
	Vector2f pos = b.getPosition();

	return (pos.x - a->x)*(pos.x - a->x)+
			(pos.y - a->y)*(pos.y - a->y)<
			(R2 + a->R)*(R2 + a->R);

}

void reset_asteriod(list<Entity*>& ent,Animation ani , int num ){
	ent.clear();
	for (int i=0; i<num; i++)
	{
		asteroid *a = new asteroid();
		a->settings(ani, rand()%W, rand()%H, rand()%360, 25);
		ent.push_back(a);
	}
}

bool zeroasteroid(list<Entity*>& ent){
	list<asteroid*> ast_list;
	for (auto i:ent){
		asteroid *astptr = dynamic_cast<asteroid*> (i);
		if (astptr !=0) ast_list.push_back(astptr);}

	return ast_list.empty();
}

int main() {

	/* virtual function:
	 * tell c++ the base class object is actually particular derived class
	 * object and should run that derived class method.
	 */

	RenderWindow app(VideoMode(W, H), "Asteroids!");
	app.setFramerateLimit(60);

	string status = "main";

	Texture t1, t2, t3, t4, t5; // obj that is ahown by image
	t1.loadFromFile("images/spaceship.png");
	t2.loadFromFile("images/background.jpg");
	t3.loadFromFile("images/explosions/type_A.png");
	t4.loadFromFile("images/rock.png");
	t5.loadFromFile("images/fire_blue.png");

	global_font.loadFromFile("fonts/arial.ttf");

	Sprite sBack_main(t2), sBack_game(t2), sBack_end(t2);
	sBack_end.setColor(Color(255,255,255,128));

	Sprite sPlayer(t1), sExplosion(t3);
	sPlayer.setTextureRect(IntRect(40,0,40,40));
	sPlayer.setOrigin(20,20);

	Animation sRock (t4, 0, 0, 64, 64, 16, 0.2);

	Animation sBullet(t5,  0, 0, 32, 64, 16, 0.8);

	Score scoretxt;
	scoretxt.setFont(global_font);

	Main_text mt;
	Opt_text ot;
	Text over_txt("       Game Over!!\npress H return to home",global_font);
	over_txt.setCharacterSize(30);
	over_txt.setFillColor(sf::Color::Red);
	over_txt.setOrigin(-400,-350);

	Text wintxt("You survive !!",global_font);
	wintxt.setCharacterSize(30);
	wintxt.setFillColor(sf::Color::Red);
	wintxt.setOrigin(-450,-350);

	list<Entity*> entities;

	int noOfAst = ot.getEnemies();

	// This is for the explosion
//	sExplosion.setPosition(300,300);
	float Frame = 0;
	float animSpeed = 0.4;
	int frameCount = 20;

	float frameToEnd = 0;

	// This is for the spaceship
	float x = 300, y = 300;
	float dx = 0 , dy = 0, angle = 0;
	bool thrust;

	bool game = true;

	while (app.isOpen())
	{

	// Events not in the main loop

		Event event;
		if (status == "main"){
			while (app.pollEvent(event))
			{
				if (event.type == Event::Closed)
					app.close();
				if (event.type == Event::KeyPressed){
					if (event.key.code == Keyboard::Up)
						mt.up();
					if (event.key.code == Keyboard::Down)
						mt.down();
					if (event.key.code == Keyboard::Enter){
						if (mt.getPosition() == 0)
							{status = "game";
							game = true;
							reset_asteriod(entities,sRock, noOfAst);}
						if (mt.getPosition() == 1)
							status = "option";
						if (mt.getPosition() == 2)
							app.close();
						}
					}
			}

			game = true;
			// This is for the spaceship
			x = 300; y = 300;
			dx = 0; dy = 0;
			angle = 0;
			sPlayer.setPosition(x,y);
			sPlayer.setRotation(angle+90);
			thrust = false;

		}

		if (status == "option"){
			while (app.pollEvent(event))
			{
				if (event.type == Event::Closed)
					app.close();
				if (event.type == Event::KeyPressed){
					if (event.key.code == Keyboard::Up)
						ot.up();
					if (event.key.code == Keyboard::Down)
						ot.down();
					if (event.key.code == Keyboard::Left)
						ot.left();
					if (event.key.code == Keyboard::Right)
						ot.right();
					if (event.key.code == Keyboard::Enter)
						if (ot.getPosition() == 1){
							noOfAst = ot.getEnemies();
							status = "main";}
				}
			}
		}
		if (status == "game"){
			while (app.pollEvent(event)){
				if (event.type == Event::Closed)
					app.close();

				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::Space)
						if (game)
						{
							bullet *b = new bullet;
							b->settings(sBullet,x,y,angle,10);
							entities.push_back(b);
						}
				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::H)
						{status = "main";
						scoretxt.reset();
						reset_asteriod(entities,sRock, noOfAst);
						}
			}
			if (!game){
				Frame += animSpeed;
				frameToEnd += animSpeed;
				if (Frame > frameCount) Frame -= frameCount;
				if (frameToEnd > frameCount*2)
					status = "over";
				if (status == "over")
					frameToEnd = 0;
			}
		}
		if (status == "over"){
			while (app.pollEvent(event)){
				if (event.type == Event::Closed)
					app.close();

				if (event.type == Event::KeyPressed)
					if (event.key.code == Keyboard::H)
						{status = "main";
						scoretxt.reset();
						}
				}
		}

			///
			if (game){
				if (Keyboard::isKeyPressed(Keyboard::Right)) angle += 3;
				if (Keyboard::isKeyPressed(Keyboard::Left)) angle -= 3;
				if (Keyboard::isKeyPressed(Keyboard::Up)) thrust = true;
				else thrust = false;

				for (auto a:entities)
					for (auto b:entities)
						if (a->name=="asteroid" && b->name=="bullet")
							if (isCollide(a, b))
							{ a-> life = false ; b->life = false; scoretxt.update();}


				///spaceship movement///
				if (thrust)
					{ dx += cos(angle*DEGTORAD)*0.2;
					  dy += sin(angle*DEGTORAD)*0.2; }
				else
					{ dx *= 0.99;
					  dy *= 0.99; }

				int maxSpeed = 15;
				float speed = sqrt(dx*dx+dy*dy);
				if (speed > maxSpeed)
					{ dx *= maxSpeed/speed;
					  dy *= maxSpeed/speed; }

				x += dx;
				y += dy;

				if (x > W) x = 0;
				if (x < 0) x = W;
				if (y > H) y = 0;
				if (y < 0) y = H;

				sPlayer.setPosition(x,y);
				sPlayer.setRotation(angle+90);}
				///

				for (auto i = entities.begin(); i!=entities.end(); )
				{
					Entity *e = *i;
					e->update();
					e->anim.update();

					if (e->life==false) {i=entities.erase(i	); delete e;}
					else i++;
				}




		if (status == "main"){
			app.clear(Color(255,255,255,255));
			sBack_main.setColor(Color(255,255,255,255));
			app.draw(sBack_main);
			mt.draw(app);
			app.display();}

		else if (status == "option"){
			app.clear(Color(0,0,0,255));
			sBack_main.setColor(Color(255,255,255,128));
			app.draw(sBack_main);
			ot.draw(app);
			app.display();}

		else if (status == "game"){
			sRock.update();
			app.clear(Color(255,255,255,255));
			app.draw(sBack_main);
			for(auto i:entities)i->draw(app);  // old implementation
			if (zeroasteroid(entities)) app.draw(wintxt);
			for (auto a:entities){
				if (a->name=="asteroid" && isCollide(a, sPlayer))
				{
					game = false;
					sExplosion.setPosition(sPlayer.getPosition().x,sPlayer.getPosition().y);
					break;
				}
			}
			if (game) app.draw(sPlayer);
			else {
				sExplosion.setTextureRect(IntRect(int(Frame)*50,0,50,50));
				app.draw(sExplosion);
			}
			scoretxt.draw(app);
			app.display();}

		else if (status == "over"){
			app.clear(Color(255,255,255,255));
			sBack_main.setColor(Color(255,255,255,128));
			app.draw(sBack_main);
			scoretxt.draw(app);
			app.draw(over_txt);
			app.display();}


	}


	return 0;
}
