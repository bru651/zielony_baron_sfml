#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

class Texturetracker {
public:
    std::string testure= "blukloc.png";
    std::string smallfarm= "smallfarm.png";
    std::string whouse= "whouse.png";
    std::string smith= "ssmith.png";
    std::string chapel = "Chapel.png";
    std::string keep = "Keep.png";
    std::string delbut= "testdel.png";
    std::string coin= "coin.png";
    std::string popim="popim.png";
    std::string foodim= "foodim.png";
    std::string time = "time.png";
    std::string skull = "skull.png";
    std::string grassland = "grassland.png";
    std::string water = "water.png";
    std::string sand = "sand.png";
    std::string pm = "ptitle.png";
    std::string ps = "pstart.png";
    std::string pi = "pinfo.png";
    std::string pq = "pquit.png";
    std::string i1 = "i1.png";
    std::string i2 = "i2.png";
    std::string i3 = "i3.png";
    std::string i4 = "i4.png";
    std::string i5 = "i5.png";
    std::string i6 = "i6.png";
    std::vector <std::string> tilelist = {grassland,water,sand };
    std::vector <std::string> buildtextr = { smallfarm,whouse,smith,chapel,keep };
    std::vector <std::string> menutextr = { pm,ps,pi,pq };
};
class Statetracker {
public:
    bool colide = false;
    bool butclick = false;
    bool erasemod = false;
    bool celebration = false;
    bool pollutemode = false;
};
class Building {
public:
    std::string typ;
    std::string imglink;
    float posx;
    float posy;
    int cost;
    int jobs;
    std::string descr="Brak opisu";
};
class Button {
public:
    std::string typ;
    //sf::Texture* obraz;
    std::string imglink;
    float posx;
    float posy;
    sf::Vector2u roz = butsize();
    bool gotClicked(int mx, int my) {
        if ((mx > posx && mx < posx + roz.x) && (my > posy && my < posy + roz.y)) {
            std::cout << "klik " << typ << std::endl;
            return true;
        }
        else {
            //std::cout << "x: " << mx << " posx: " << posx << " : " << posx + roz.x << std::endl;
            //std::cout << "y: " << my << " posy: " << posy << " : " << posy + roz.y << std::endl;
            return false;
        }
    }
    sf::Vector2u butsize() {
        sf::Texture funtextur;
        if (!funtextur.loadFromFile(imglink))
        {
            // error...
        }
        return funtextur.getSize();
    }
};
class Tile {
public:
    std::string type;
    std::string imglink;
    bool buildable=true;
    int pollution = 0;

};
class Ground {
public:
    std::vector<std::vector<Tile>> xpyp;
    std::vector<std::vector<Tile>> xnyp;
    std::vector<std::vector<Tile>> xpyn;
    std::vector<std::vector<Tile>> xnyn;
    Tile GetTile(int x,int y) {
        if (x >= 0) {
            if (y>=0) {
                //return xpyp[x][y];
                return xpyp.at(x).at(y);
            }
            else {
                return xpyn.at(x).at( - y - 1);
            }
        }
        else {
            if (y >= 0) {
                return xnyp.at( - x - 1).at(y);
            }
            else {
                return xnyn.at( - x - 1).at( - y - 1);
            }
        }
    }
    void Overide(int x, int y, Tile newtile) {
        if (x >= 0) {
            if (y >= 0) {
                xpyp.at(x).at(y)= newtile;
            }
            else {
                xpyn.at(x).at(-y - 1)=newtile;
            }
        }
        else {
            if (y >= 0) {
                xnyp.at(-x - 1).at(y)=newtile;
            }
            else {
                xnyn.at(-x - 1).at(-y - 1)= newtile;
            }
        }
    }
    void Generate(Texturetracker tt) {
        Tile tilestamp;
        tilestamp.type = "grass";
        tilestamp.imglink = "grassland.png";
        tilestamp.buildable = true;
        
        xpyp.resize(50, std::vector<Tile>(50, tilestamp));
        xnyp.resize(50, std::vector<Tile>(50, tilestamp));
        xpyn.resize(50, std::vector<Tile>(50, tilestamp));
        xnyn.resize(50, std::vector<Tile>(50, tilestamp));
        tilestamp.type = "water";
        tilestamp.imglink = "water.png";
        tilestamp.buildable = false;
        for (int x = -50; x < 50; x++) {
            for (int y = -50; y < 50; y++) {
                //std::cout << sqrt(abs(x) + abs(y)) << std::endl;
                if (sqrt(abs(x^2) + abs(y^2)) > 8||abs(-x-sqrt(y*x)+5)<4) {
                    Overide(x, y, tilestamp);
               }
               //*/
            }
        }//*/
        for (int x = -49; x < 49; x++) {
            for (int y = -49; y < 49; y++) {
                if (GetTile(x, y).type == "grass"&&(GetTile(x+1, y).type=="water"|| GetTile(x - 1, y).type == "water"|| GetTile(x, y-1).type == "water"|| GetTile(x, y+1).type == "water")) {
                    tilestamp.type = "sand";
                    tilestamp.imglink = "sand.png";
                    tilestamp.buildable = false;
                    Overide(x, y, tilestamp);
                }
                //*/
            }
        }//*/
    }
    //Sprawdza zanieczyszczenie mapy
    void ScanPollution(std::vector<Building> propy) {
        std::cout << "pollutioncheck" << std::endl;
        int radius = 0;
        int pollutionC = 0;
        int pollutionA = 0;//Area
        Tile tilestamp;
        for (int x = -50; x < 50; x++) {
            for (int y = -50; y < 50; y++) {
                tilestamp = GetTile(x, y);
                tilestamp.pollution = 0;
                Overide(x, y, tilestamp);
            }
        }
        for (int i = 0; i < propy.size(); i++) {
            radius = 0;
            pollutionC = 0;
            pollutionA = 0;//Area
            if (propy[i].typ == "woodhouse") {
                radius = 2;
                pollutionC = 2;
                pollutionA = 1;
            }
            else if (propy[i].typ == "smallfarm") {
                radius = 3;
                pollutionC = 1;
                pollutionA = 1;
            }
            else if (propy[i].typ == "stonesmith") {
                radius = 1;
                pollutionC = 2;
                pollutionA = 2;
            }
            else if (propy[i].typ == "chapel") {
                radius = 4;
                pollutionC = -4;
                pollutionA = -4;
            }
            else if (propy[i].typ == "keep") {
                radius = 5;
                pollutionC = -1;
                pollutionA = 1;
            }
            tilestamp = GetTile(propy[i].posx, propy[i].posy);
            tilestamp.pollution += pollutionC;
            Overide(propy[i].posx, propy[i].posy, tilestamp);
            for (int x = propy[i].posx - radius; x < propy[i].posx + radius + 1; x++) {
                if (abs(x)<50) {
                    for (int y = propy[i].posy - radius; y < propy[i].posy + radius + 1; y++) {
                        if (abs(y) < 50) {
                            Tile tilestamp = GetTile(x, y);
                            tilestamp.pollution += pollutionA;
                            Overide(x, y, tilestamp);
                        }
                    }
                }
            }
        }
    }

};
class village {
public:
    int day = 0;
    int moni = 1000;
    int moniin = 0;
    int food = 100;
    int foodin = 0;
    int pops = 10;
    int popcap = 0;
    int milestone = 50;
    int alljobs = 0;
    float efficency = 0;
    int pollution = 0;
    int heal = 0;
    //std::vector<Building> propy;
    void cykl() {
        //prod();
        day += 1;
        moni += moniin;
        food += foodin;
        food -= pops;
        if (food<0) {
            pops += food;
            food = 0;
        }
        else {
            if (popcap>pops) {
                pops += 1 + pops / 10;
            }
            else {
                pops+=1 + pops / 100 + (pops / 10)* std::floor(static_cast<float>(popcap) /(static_cast<float>(pops) +1.0));
            }
        }
        float dper = static_cast<float>(pollution) / 100.0;
        int dreal = std::floor(static_cast<float>(pops) * dper);
        dreal -= heal;
        if (dreal>0) {
            pops -= dreal;
        }
        std::cout << "illness: " << dreal << std::endl;
    }
    void prod(std::vector<Building> propy,Ground ground) {
        //std::cout << "prod0" << std::endl;
        moniin = 0;
        foodin = 0;
        popcap = 0;
        alljobs = 0;
        heal = 0;
        Tile tilestamp;
        int cpol=0;
        for (int i=0; i < propy.size();i++) {
            alljobs += propy[i].jobs;
            //std::cout <<"jobs: " << propy[i].jobs << std::endl;;
            if (propy[i].typ=="stonesmith") {
                moniin += 20;
            }
            else if (propy[i].typ == "woodhouse") {
                popcap += 5;
            }
            else if (propy[i].typ == "smallfarm") {
                foodin += 5;
            }
            else if (propy[i].typ == "chapel") {
                moniin -= 25;
                heal += 5;
            }
            else if (propy[i].typ == "keep") {
                moniin -= 15;
                foodin -= 20;
            }
            tilestamp = ground.GetTile(propy[i].posx, propy[i].posy);
            cpol += tilestamp.pollution;
        }
        if (propy.size()>0) {
            pollution = cpol / propy.size();
        }
        else
        {
            pollution = 0;
        }
        //std::cout << alljobs << std::endl;
        if (pops<alljobs) {
            
            //std::cout << "prod2" << std::endl;
            efficency = 0;
            if(alljobs>0){
                efficency = static_cast<float>(pops) / static_cast<float>(alljobs);
            }
            moniin = std::floor(moniin*efficency);
            moniin += pops;
            if (foodin>0) {
                foodin = std::floor(foodin * efficency);
            }
            
            //std::cout <<"efficency: " << efficency << std::endl;
        }
        else {
            moniin += alljobs;
        }

    }
};

void InfoMenu(sf::RenderWindow& window, Texturetracker texturetracker)
{
    bool mainmenu = true;
    std::vector<Button> buttonlist;
    Button tbut = { "pquit",texturetracker.ps,750 - 157,600 };
    buttonlist.push_back(tbut);
    sf::Texture opertexture;
    sf::Sprite tempspr;// zmienna do rysowania sprite'ów
    window.clear();
    if (!opertexture.loadFromFile(tbut.imglink)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157, 600);
    window.draw(tempspr);
    if (!opertexture.loadFromFile(texturetracker.pm)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157, 0);
    window.draw(tempspr);
    if (!opertexture.loadFromFile(texturetracker.i2)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157*2, 200);
    window.draw(tempspr);
    if (!opertexture.loadFromFile(texturetracker.i3)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750, 200);
    window.draw(tempspr);
    if (!opertexture.loadFromFile(texturetracker.i6)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157*2, 400);
    window.draw(tempspr);
    if (!opertexture.loadFromFile(texturetracker.i4)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 , 400);
    window.draw(tempspr);//*/
    if (!opertexture.loadFromFile(texturetracker.i1)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157 * 4, 200);
    window.draw(tempspr);//*/
    if (!opertexture.loadFromFile(texturetracker.i5)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 + 157 * 2, 200);
    window.draw(tempspr);//*/
    window.display();
    while (mainmenu == true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //Czy wciœniêto myszke
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttonlist.size(); i++)
                    {
                        if (buttonlist[i].gotClicked(event.mouseButton.x, event.mouseButton.y) == true) {
                            if (buttonlist[i].typ == "pquit") {
                                mainmenu = false;
                            }
                        }
                    }//*/
                }
            }
        }
    }

}
void MainMenu(sf::RenderWindow& window,Texturetracker texturetracker)
{
    bool mainmenu = true;
    std::vector<Button> buttonlist;
    Button fbut = { "pstart",texturetracker.ps,750-157,200 };
    buttonlist.push_back(fbut);
    Button sbut = { "pinfo",texturetracker.pi,750 - 157,400 };
    buttonlist.push_back(sbut);
    Button tbut = { "pquit",texturetracker.pq,750 - 157,600 };
    buttonlist.push_back(tbut);
    sf::Texture opertexture;
    sf::Sprite tempspr;// zmienna do rysowania sprite'ów
    window.clear();
    for (int i = 0; i < buttonlist.size(); i++)// Rysuje Przyciski
    {
        if (!opertexture.loadFromFile(buttonlist[i].imglink)) {}
        tempspr.setTexture(opertexture);
        tempspr.setPosition(buttonlist[i].posx, buttonlist[i].posy);
        window.draw(tempspr);
    }
    if (!opertexture.loadFromFile(texturetracker.pm)) {}
    tempspr.setTexture(opertexture);
    tempspr.setPosition(750 - 157, 0);
    window.draw(tempspr);
    window.display();
    while (mainmenu == true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //Czy wciœniêto myszke
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttonlist.size(); i++)
                    {
                        if (buttonlist[i].gotClicked(event.mouseButton.x, event.mouseButton.y) == true) {
                            //statetr.butclick = true;
                            if (buttonlist[i].typ == "pstart") {
                                // buildordered = typelist[0];
                                mainmenu = false;
                            }
                            else if (buttonlist[i].typ == "pinfo") {
                                mainmenu = false;
                                window.clear();
                                InfoMenu(window,texturetracker);
                            }
                            else if (buttonlist[i].typ == "pquit") {
                                // buildordered = typelist[2];
                                window.close();
                            }
                            //std::cout << "asasas" << std::endl;
                        }
                    }//*/
                }
            }
        }
    }
   
}

int main()
{
    float X=1500,Y=800;
    sf::RenderWindow window(sf::VideoMode(X, Y), "Zielony Baron");
    sf::View view(sf::FloatRect(0, 0, X, Y));
    sf::View guiview(sf::FloatRect(0, 0, X, Y));
    window.setFramerateLimit(30);
    Texturetracker texturetracker;
    sf::Texture opertexture;
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font); // font is a sf::Font
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);// | sf::Text::Underlined);
    //wtêpny kod
    int stansize = 32;// Standardowy rozmiar bloku
    float scalling = 1;// Do skalowania element zale¿nie od przybli¿enia
    float camposx = 0;// W³aœciwoœci kamery
    float camposy = 0;
    float defcamspeed = 8;
    float camspeed = defcamspeed;
    std::string camtext;
    std::vector<Building> proplist;
    std::vector<Building> typelist;
    Building a = {"smallfarm",texturetracker.smallfarm,0,0,50,2,"+5 food, +2 jobs"};
    Building b = { "woodhouse",texturetracker.whouse,0,0,100,0 ,"+5 housing"};
    Building c = { "stonesmith",texturetracker.smith,0,0 ,350,1,"+20 income, +1 jobs"};
    Building d = { "chapel",texturetracker.chapel,0,0 ,500,3,"-25 income, +1 jobs, +5 heal, less pollution" };
    Building e = { "keep",texturetracker.keep,0,0 ,750,50,"-15 income, -20 food, +50 jobs" };
    typelist.push_back(a);
    typelist.push_back(b);
    typelist.push_back(c);
    typelist.push_back(d);
    typelist.push_back(e);
    //pryciski
    std::vector<Button> buttonlist;
    Button fbut = {"smallfarm",texturetracker.smallfarm,X-50,10};
    buttonlist.push_back(fbut);
    Button sbut = { "woodhouse",texturetracker.whouse,X - 50,50 };
    buttonlist.push_back(sbut);
    Button tbut = { "stonesmith",texturetracker.smith,X - 50,90 };
    buttonlist.push_back(tbut);
    Button fibut = { "chapel",texturetracker.chapel,X - 50,130 };
    buttonlist.push_back(fibut);
    Button sibut = { "keep",texturetracker.keep,X - 50,170 };
    buttonlist.push_back(sibut);
    Button fobut = { "delete",texturetracker.delbut,X - 50,210 };
    buttonlist.push_back(fobut);
    Building buildordered=typelist[0];
    Statetracker statetr;
    village wioska;
    Ground ground;
    ground.Generate(texturetracker);;
    MainMenu(window, texturetracker);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //Czy wciœniêto myszke
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int tempix = std::floor((event.mouseButton.x + camposx * scalling - (X / 2) * (1 - scalling)) / (stansize * scalling));
                    int tempiy = std::floor((event.mouseButton.y + camposy * scalling - (Y / 2) * (1 - scalling)) / (stansize * scalling));
                    for (int i = 0; i < buttonlist.size(); i++)
                    {
                        if (buttonlist[i].gotClicked(event.mouseButton.x, event.mouseButton.y) == true) {
                            statetr.butclick = true;
                            statetr.erasemod = false;
                            if (buttonlist[i].typ == "smallfarm") {
                                buildordered = typelist[0];
                            }
                            else if (buttonlist[i].typ == "woodhouse") {
                                buildordered = typelist[1];
                            }
                            else if (buttonlist[i].typ == "stonesmith") {
                                buildordered = typelist[2];
                            }
                            else if (buttonlist[i].typ == "chapel") {
                                buildordered = typelist[3];
                            }
                            else if (buttonlist[i].typ == "keep") {
                                buildordered = typelist[4];
                            }
                            else if (buttonlist[i].typ == "delete") {
                                statetr.erasemod = true;
                            }
                            //std::cout << "asasas" << std::endl;
                        }
                    }
                    if (statetr.butclick == false && abs(tempix) < 50 && abs(tempiy) < 50) {
                        if (statetr.erasemod == false && ground.GetTile(tempix, tempiy).buildable) {
                            statetr.colide = false;
                            std::cout << "buildatempt" << std::endl;
                            for (int i = 0; i < proplist.size(); i++) {

                                int tempix2 = proplist[i].posx;
                                int tempiy2 = proplist[i].posy;
                                if (tempix2 == tempix && tempiy2 == tempiy) {
                                    statetr.colide = true;
                                }
                            }//*/
                            if (!statetr.colide && wioska.moni >= buildordered.cost) {
                                buildordered.posx = tempix;
                                buildordered.posy = tempiy;
                                proplist.push_back(buildordered);
                                wioska.moni -= buildordered.cost;
                                wioska.prod(proplist, ground);
                                ground.ScanPollution(proplist);
                                std::cout << "build" << std::endl;
                            }
                        }
                        else {
                            //std::cout << "testdel" << std::endl;

                            for (int i = 0; i < proplist.size(); i++) {
                                int tempix2 = proplist[i].posx;
                                int tempiy2 = proplist[i].posy;

                                if (tempix2 == tempix && tempiy2 == tempiy) {
                                    proplist.erase(proplist.begin() + i);
                                    wioska.prod(proplist, ground);
                                    ground.ScanPollution(proplist);
                                    std::cout << "testdelEJE" << std::endl;
                                }
                            }
                        }
                    }
                    else {
                        statetr.butclick = false;
                    }

                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    statetr.celebration = false;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // tura
                    ground.ScanPollution(proplist);
                    wioska.cykl();
                    wioska.prod(proplist, ground);
                    if (wioska.pops>=wioska.milestone) {
                        statetr.celebration = true;
                        wioska.milestone *= 4;
                    }
                    else {
                        statetr.celebration = false;
                    }
                }
                if (event.key.code == sf::Keyboard::P) {
                    statetr.pollutemode = !statetr.pollutemode;
                    ground.ScanPollution(proplist);
                }
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.setView(guiview);
            MainMenu(window, texturetracker);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            camposy -= camspeed;
            //std::cout << "W" <<std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            camposy += camspeed;
            //std::cout << "S" << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            camposx -= camspeed;
            //std::cout << "A" << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            camposx += camspeed;
            //std::cout << "D" << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            scalling += 0.05;
            if (scalling > 10)
            {
                scalling = 10;
            }
            view.setSize(sf::Vector2f(X / scalling, Y / scalling));
            camspeed = defcamspeed / scalling;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))// Zoom down
        {
            scalling -= 0.05;
            if (scalling < 0.1)
            {
                scalling = 0.1;
            }
            view.setSize(sf::Vector2f(X / scalling, Y / scalling));
            camspeed = defcamspeed / scalling;
            //std::cout << "z-" << std::endl;
        }//*/

        window.clear();
        sf::Sprite tempspr;// zmienna do rysowania sprite'ów
        for (int i = 0; i < texturetracker.tilelist.size(); i++) {
            if (!opertexture.loadFromFile(texturetracker.tilelist.at(i))) {}
            tempspr.setTexture(opertexture);
            for (int x = -50; x < 50; x++) {
                for (int y = -50; y < 50; y++) {
                    if (ground.GetTile(x, y).imglink == texturetracker.tilelist.at(i)) {
                        tempspr.setPosition(x * stansize - camposx, y * stansize - camposy);
                        window.draw(tempspr);
                    }

                }
            }
        }
        
        for (int i = 0; i < proplist.size(); i++)// Rysuje sprite'y
        {
            if (!opertexture.loadFromFile(proplist[i].imglink)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(proplist[i].posx * stansize - camposx, proplist[i].posy * stansize - camposy);
            window.draw(tempspr);
        }
        if (statetr.pollutemode) {
            for (int x = -50; x < 50; x++) {
                for (int y = -50; y < 50; y++) {
                    sf::RectangleShape rectangle(sf::Vector2f(24, 24));
                    int p = ground.GetTile(x, y).pollution;
                    p *= 10;
                    if (p > 255) {
                        p = 255;
                    }
                    else if (p < -255) {
                        p = -255;
                    }
                    if (p > 0) {
                        rectangle.setFillColor(sf::Color(255, 255 - p, 255 - p));
                    }
                    else {
                        rectangle.setFillColor(sf::Color(255 + p, 255, 255 + p));
                    }
                    rectangle.move(sf::Vector2f(x * stansize - camposx + 6, y * stansize - camposy + 6));
                    window.draw(rectangle);
                }
            }
        }
        //GUI
        window.setView(guiview);
        {
            sf::RectangleShape rectangle(sf::Vector2f(230, 200));
            rectangle.setFillColor(sf::Color(50, 50, 50));
            window.draw(rectangle);
            rectangle.move(sf::Vector2f(X - 65, 0));
            rectangle.setSize(sf::Vector2f(65, 260));
            window.draw(rectangle);
            rectangle.move(sf::Vector2f(-(X - 65), Y - 50));
            rectangle.setSize(sf::Vector2f(X, 100));
            window.draw(rectangle);
            if (statetr.celebration) {
                rectangle.move(sf::Vector2f(X/3, -(2*Y/3)));
                rectangle.setSize(sf::Vector2f(X/3, Y/3));
                window.draw(rectangle);
            }

        }
        if (statetr.erasemod) {
            camtext = "DECONSTRUCTION";
        }
        else {
            camtext = buildordered.typ+": "+buildordered.descr+" cost: "+ std::to_string(buildordered.cost);
        }
        text.setString(camtext);
        text.setPosition(0,Y-50);
        window.draw(text);
        
        for (int i = 0; i < buttonlist.size(); i++)// Rysuje Przyciski
        {
            if (!opertexture.loadFromFile(buttonlist[i].imglink)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(buttonlist[i].posx, buttonlist[i].posy);
            window.draw(tempspr);
        }
        // UI
        {
            if (!opertexture.loadFromFile(texturetracker.time)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(5, 5);
            window.draw(tempspr);

            if (!opertexture.loadFromFile(texturetracker.coin)){}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(5, 45);
            window.draw(tempspr);

            if (!opertexture.loadFromFile(texturetracker.popim)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(5, 85);
            window.draw(tempspr);

            if (!opertexture.loadFromFile(texturetracker.foodim)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(5, 125);
            window.draw(tempspr);

            if (!opertexture.loadFromFile(texturetracker.skull)) {}
            tempspr.setTexture(opertexture);
            tempspr.setPosition(5, 165);
            window.draw(tempspr);
        }
        //TEXT
        {
            camtext = std::to_string(wioska.day);
            text.setString(camtext);
            text.setPosition(40, 5);
            window.draw(text);

            camtext = std::to_string(wioska.moni)+" ("+std::to_string(wioska.moniin)+")";
            text.setString(camtext);
            text.setPosition(40, 45);
            window.draw(text);

            camtext = std::to_string(wioska.pops) + " / " + std::to_string(wioska.popcap)+"("+ std::to_string(wioska.alljobs)+")";
            text.setString(camtext);
            text.setPosition(40, 85);
            window.draw(text);

            camtext = std::to_string(wioska.food) + " (" + std::to_string(wioska.foodin-wioska.pops) + ")";
            text.setString(camtext);
            text.setPosition(40, 125);
            window.draw(text);

            camtext = std::to_string(wioska.pollution) + " / " + std::to_string(wioska.heal);
            text.setString(camtext);
            text.setPosition(40, 165);
            window.draw(text);

            if (statetr.celebration) {
                camtext = "MILESTONE";
                text.setString(camtext);
                text.setPosition(X/3+45, Y/3-45);
                window.draw(text);

                camtext = std::to_string(wioska.pops) + " population";
                text.setString(camtext);
                text.setPosition(X / 3 + 45, Y / 3 - 5);
                window.draw(text);

                camtext = "in";
                text.setString(camtext);
                text.setPosition(X / 3 + 45, Y / 3 + 35);
                window.draw(text);

                camtext = std::to_string(wioska.day) + " days";
                text.setString(camtext);
                text.setPosition(X / 3 + 45, Y / 3 + 75);
                window.draw(text);

                camtext = "R-click to dismiss";
                text.setString(camtext);
                text.setPosition(X / 3 + 260, Y / 3 + 175);
                window.draw(text);
            }
        }
        window.setView(view);
        window.display();
    }

    return 0;
}