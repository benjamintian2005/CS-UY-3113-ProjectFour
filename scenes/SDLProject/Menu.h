#include "Scene.h"

class Menu : public Scene {
public:
    // ————— CONSTRUCTOR/DESTRUCTOR ————— //
    ~Menu();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
private:
    GLuint m_font_texture_id;
};
