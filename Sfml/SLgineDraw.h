//
//namespace SL
//{
//    int drawCall = 0;
//    sf::Font globalFont;
//    sf::Texture errorTexture;
//    sf::RenderWindow* windowGlobalDraw;
//    // init the window
//    void initSLgineDraw(sf::RenderWindow* win);/// initialisation
//
//
//
//    void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col); /// draw a colored line
//
//
//
//    void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col1, sf::Color col2);/// draw a colored gradient line
//
//
//
//    void DrawPix(sf::Vector2f pos, sf::Color col1);/// draw a pixel
//
//
//
//    void DrawCircle(sf::Vector2f pos, float size, sf::Color crColor);/// draw a circle
//
//
//
//    void DrawRectangle(sf::Vector2f pos, float sizex, float sizey, sf::Color crColor);/// draw a rectangle
//
//
//
//    void DrawRectangleGradient(sf::Vector2f pos, float sizex, float sizey, sf::Color bottomColor, sf::Color upColor);/// draw a rectangle gradient
//
//
//
//
//    void DrawProgressBar(sf::Vector2f pos, float sizex, float sizey, sf::Color crColordisabled, sf::Color crColorprogress, float percent /* 0.1 = 10%*/);/// draw a progress bar
//
//
//
//    void DrawText(sf::Vector2f pos, float size, sf::Color textCol, std::string text);/// draw text from loaded font
//
//
//    ///                                            texture drawing
//#pragma region texture
//
//
//    sf::Texture LoadTexture(std::string filePath); /// load a texture (note if error occur it will load a null texture)
//
//
//
//    void DrawTexture(sf::Texture textureTodraw, sf::Vector2f pos); /// draw a simple texture
//
//
//
//    void DrawTextureA(sf::Texture textureTodraw, sf::Vector2f pos
//        , float rotation = 0.f, sf::Vector2f centerP = { 0,0 }
//    , float size = 1.f, sf::Color tint = sf::Color::White); /// draw a advance texture
//
//
//
//    void DrawTextureC(sf::Texture textureTodraw, sf::Vector2f pos, sf::Color tint = sf::Color::White);/// draw a texture with tint
//
//
//
//    void DrawTexture(sf::Texture textureTodraw, sf::Vector2f pos);
//#pragma endregion
//    ///                                             other
//#pragma region other
//    int getDrawCall();/// get the number of thing draw since the last displayWindow() call
//
//
//
//    void ClearWindow(sf::Color bgColor);/// fill the background
//
//
//
//    void DisplayWindow();/// display everything draw from the last display
//
//
//#pragma endregion
//}
//
