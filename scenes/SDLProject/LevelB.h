#include "Scene.h"

class LevelB : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    
    // ————— DESTRUCTOR ————— //
    ~LevelB();
    
    // ————— METHODS ————— //
    void initialise() override;
    bool is_completed();
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
