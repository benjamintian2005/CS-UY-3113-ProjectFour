#include "Scene.h"

class LevelA : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    
    // ————— DESTRUCTOR ————— //
    ~LevelA();
    
    // ————— METHODS ————— //
    void initialise() override;
    bool is_completed() ;

    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
