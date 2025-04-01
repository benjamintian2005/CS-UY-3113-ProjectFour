#include "LevelC.h"
#include "Utility.h"

#define LEVEL_C_WIDTH 14
#define LEVEL_C_HEIGHT 8
#define LEVEL3_ENEMY_COUNT 2


constexpr char SPRITESHEET_FILEPATHC[] = "assets/george_0.png",
           PLATFORM_FILEPATHC[]    = "assets/platformPack_tile027.png",
           ENEMY_FILEPATHC[]       = "assets/soph.png";

unsigned int LEVEL_C_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 2
};

LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_game_state.map = new Map(LEVEL_C_WIDTH, LEVEL_C_HEIGHT, LEVEL_C_DATA, map_texture_id, 1.0f, 4, 1);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATHC);

    int player_walking_animation[4][4] =
    {
        { 1, 5, 9, 13 },  // for George to move to the left,
        { 3, 7, 11, 15 }, // for George to move to the right,
        { 2, 6, 10, 14 }, // for George to move upwards,
        { 0, 4, 8, 12 }   // for George to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );
    
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(3.0f);
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATHC);

    m_game_state.enemies = new Entity[LEVEL3_ENEMY_COUNT];

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }


    m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    m_game_state.enemies[1].set_position(glm::vec3(4.0f, 0.0f, 0.0f));
    m_game_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/background.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelC::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, LEVEL3_ENEMY_COUNT, m_game_state.map);
    
    if (m_game_state.player->check_enemy_collision(m_game_state.enemies, LEVEL3_ENEMY_COUNT)) {
            // Player lost a life from enemy collision
            // You could add visual effects or sounds here
        }
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
}

bool LevelC::is_completed() {
    // For example, check if player reached a certain position
    return m_game_state.player != nullptr &&
    m_game_state.player->get_position().x >= LEVEL_C_WIDTH - 2.0f;
}


void LevelC::render(ShaderProgram *g_shader_program)
{
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i <  LEVEL3_ENEMY_COUNT; i++)
            m_game_state.enemies[i].render(g_shader_program);
    std::string lives_text = "LIVES: " + std::to_string(m_game_state.player->get_lives());
    Utility::draw_text(g_shader_program, Utility::load_texture("assets/font1.png"),
                        lives_text, 0.5f, -0.25f,
                        glm::vec3(m_game_state.player->get_position().x - 4.5f, -3.0f, 0.0f));
}
