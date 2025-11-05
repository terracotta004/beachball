#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

static float randFloat(float a, float b) {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> d(a, b);
    return d(rng);
}

int main() {
    const unsigned W = 900, H = 600;
    const float R = 60.f;
    const float SPEED_MIN = 120.f, SPEED_MAX = 220.f;
    const float DIR_MIN = 1.25f, DIR_MAX = 3.0f;

    sf::RenderWindow window(sf::VideoMode({W, H}), "Rolling Beach Ball");

    // --- procedural beach ball ---
    sf::RenderTexture paint(sf::Vector2u{(unsigned)(R*2+4), (unsigned)(R*2+4)});
    sf::CircleShape base(R);
    base.setFillColor(sf::Color::White);
    base.setOutlineThickness(2);
    base.setOutlineColor(sf::Color(220,220,220));
    base.setPosition({2,2});
    paint.draw(base);

    const sf::Color cols[6] = {
        {231,76,60},{46,204,113},{52,152,219},
        {241,196,15},{155,89,182},{230,126,34}
    };
    auto wedge=[&](float a0,float a1,sf::Color c){
        const int steps=40;
        sf::ConvexShape s(steps+2);
        s.setFillColor(c);
        sf::Vector2f ctr(R+2,R+2);
        s.setPoint(0,ctr);
        for(int i=0;i<=steps;i++){
            float t=i/(float)steps, ang=(a0+t*(a1-a0))*3.1415926f/180;
            s.setPoint(1+i,{ctr.x+std::cos(ang)*R,ctr.y+std::sin(ang)*R});
        }
        paint.draw(s);
    };
    for(int i=0;i<6;i++) wedge(i*60-90,(i+1)*60-90,cols[i]);

    sf::CircleShape plug(R*0.12f);
    plug.setOrigin({plug.getRadius(),plug.getRadius()});
    plug.setPosition({R+2,R*0.3f+2});
    plug.setFillColor(sf::Color::White);
    paint.draw(plug);
    paint.display();

    sf::Texture tex = paint.getTexture();
    sf::Sprite ball(tex);
    ball.setOrigin({tex.getSize().x/2.f, tex.getSize().y/2.f});

    sf::Vector2f pos(W/2.f,H/2.f);
    auto randVel=[&](){
        float ang=randFloat(0,6.28318f), sp=randFloat(SPEED_MIN,SPEED_MAX);
        return sf::Vector2f(std::cos(ang)*sp,std::sin(ang)*sp);
    };
    sf::Vector2f vel=randVel();
    float rot=0, next=randFloat(DIR_MIN,DIR_MAX);
    sf::Clock clk;

while (window.isOpen()) {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
        }
    }

        float dt=clk.restart().asSeconds();
        next-=dt;
        if(next<=0){ vel=vel*0.3f+randVel()*0.7f; next=randFloat(DIR_MIN,DIR_MAX); }

        pos+=vel*dt;
        if(pos.x-R<0){pos.x=R;vel.x=fabs(vel.x);} 
        if(pos.x+R>W){pos.x=W-R;vel.x=-fabs(vel.x);}
        if(pos.y-R<0){pos.y=R;vel.y=fabs(vel.y);}
        if(pos.y+R>H){pos.y=H-R;vel.y=-fabs(vel.y);}

        float sp=std::sqrt(vel.x*vel.x+vel.y*vel.y);
        rot+=sp/R*dt*180/3.14159f;
        if(rot>360) rot-=360;

        ball.setPosition(pos);
        ball.setRotation(sf::degrees(rot));

        window.clear({35,125,190});
        sf::RectangleShape sand({(float)W,120});
        sand.setPosition({0,(float)H-120});
        sand.setFillColor({237,201,175});
        window.draw(sand);
        window.draw(ball);
        window.display();
    }
}
