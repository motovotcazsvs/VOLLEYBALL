#include <SFML/Graphics.hpp>
using namespace sf;
#include <iostream>
using namespace std;

class General {
private:
	float dx, dy;
	float gravity;
	float drag;
	float direction;
public:
	float x, y;
	
	String file_name;
	Image image;
	Texture texture;
	Sprite sprite;
	General(String f, float X, float Y)
	{
		file_name = f;
		image.loadFromFile("images/" + file_name);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(X, Y);

		dx = 0;
		dy = 0;
		gravity = 0.002;
		drag = 0.009;
		direction = 1;
	}

	void updatePosition()
	{
		x = sprite.getPosition().x;
		y = sprite.getPosition().y;
	}

	void ballMovement(float time)
	{
		//dy = gravity * time;
		//sprite.move(x, 0.085 * time);
		float grav = 0;
		double uskorenie = 0.085 * time;
		static int e = 1;
		sprite.move(0, uskorenie * e);
		updatePosition();
		//dy = dy * gravity;
		//x += dx;
		//y += dy;
		//sprite.setPosition(x, y);
		/*if (y<ground) { y = ground; dy = -dy*drag; dx = dx*drag; }
		if (x<R + 3) { x = R + 3; dx = -dx; }
		if (x>w - R) { x = w - R; dx = -dx; }*/
		//updatePosition();
		//if (x < 0) dx = -dx;
		//if (y > 800) { sprite.setPosition(230, 100);}
		//if (y > 772) { y = 772; dy = -dy * drag; }
		if (y > 772) { sprite.setPosition(x, y); e = -e; /*dy = -dy; */ }
		if (y < 10) { sprite.setPosition(x, y); e = -e; }

		//sprite.setPosition(x, y);
	}
};


class Player : public General {
private:
	float field_boundary_r;
	float field_boundary_l;
	int Count() { static int c = 0; c++;  return c; }
	int np;
	bool drop_and_jump = true;
	bool little_jump = false;
public:
	Player(float br, float bl, string n, float spx, float spy)
		: General(n, spx, spy)
	{
		field_boundary_r = br;
		field_boundary_l = bl;

		np = Count();
	}

	void control(float time)
	{
		enum t {standing_position, jump_position, drop_position};
		t pos;
		
		if (drop_and_jump && !little_jump) pos = jump_position;
		else if(!drop_and_jump || little_jump) pos = drop_position;
		little_jump = true;

		updatePosition();
		if (x < field_boundary_l) sprite.setPosition(field_boundary_l, y);
		else if (x > field_boundary_r) sprite.setPosition(field_boundary_r, y);
		updatePosition();
		if (y < 340) sprite.setPosition(x, 340);
		else if (y > 722) sprite.setPosition(x, 722);
		
		if ((Keyboard::isKeyPressed(Keyboard::Left) && np == 1) ||
			(Keyboard::isKeyPressed(Keyboard::A) && np == 2)) {
			sprite.move(-0.1 * time, 0);
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) && np == 1) ||
			(Keyboard::isKeyPressed(Keyboard::D) && np == 2)) {
			sprite.move(0.1 * time, 0);
		}
		updatePosition();
		if (y == 722) {
			pos = standing_position;
			drop_and_jump = true;
		}
		else if (y == 340) {
			pos = drop_position;
			drop_and_jump = false;
		}
		
		if ((Keyboard::isKeyPressed(Keyboard::Up) && np == 1) ||
			(Keyboard::isKeyPressed(Keyboard::W) && np == 2)) {
			if (pos == standing_position || pos == jump_position) {
				sprite.move(0, -0.35 * time);
				pos = jump_position;
				drop_and_jump = true;
				little_jump = false;
			}
				
		}

		updatePosition();
		if (y < 722) sprite.move(0, 0.2 * time);
		
	}
};



int main()
{
	Clock clock;
	Event event;
	
	General field("background.PNG", 0, 0);
	General ball("Volleyball.png", 230, 100);
	Player pl1(1049, 600, "blobby.png", 830, 722);
	Player pl2(450, 0, "blobby.png", 230, 722);



	RenderWindow window(VideoMode(1199, 900), "VolleyBall");



	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		
		time = time / 200;
		

		ball.ballMovement(time);
		pl1.control(time);
		pl2.control(time);

		window.clear();
		window.draw(field.sprite);
		window.draw(pl1.sprite);
		window.draw(pl2.sprite);
		window.draw(ball.sprite);
		window.display();
	}
	system("pause");
	return 0;
}

