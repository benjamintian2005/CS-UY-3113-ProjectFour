#include "LevelA.h"
#include "Utility.h"

#define LEVEL_A_WIDTH 27
#define LEVEL_A_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 1


constexpr char SPRITESHEET_FILEPATHA[] = "assets/main.png",
           PLATFORM_FILEPATHA[]    = "assets/platformPack_tile027.png",
           ENEMY_FILEPATHA[]       = "assets/crab1.png";

unsigned int LEVEL_A_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2,
};

LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/world_tileset.png");
    m_game_state.map = new Map(LEVEL_A_WIDTH, LEVEL_A_HEIGHT, LEVEL_A_DATA, map_texture_id, 1.0f, 16, 16);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATHA);

    int player_walking_animation[4][4] =
    {
        { 4, 5, 6, 7 },  // for George to move to the left,
        { 8, 9, 10, 11 }, // for George to move to the right,
        { 12, 13, 14, 15 }, // for George to move upwards,
        { 0, 1, 2, 3 }   // for George to move downwards
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
    
    m_game_state.player->set_lives(3);

    
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(3.0f);
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATHA);

    m_game_state.enemies = new Entity[LEVEL1_ENEMY_COUNT];

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, FLYER, HOVERING);
    }


    m_game_state.enemies[0].set_position(glm::vec3(9.0f, -3.0f, 0.0f));
    //m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    //m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/background.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/boing.mp3");
    m_game_state.death_sfx = Mix_LoadWAV("assets/death.mp3");
    m_game_state.switch_sfx = Mix_LoadWAV("assets/bruh.mp3");
    m_game_state.win_sfx =Mix_LoadWAV("assets/yay.mp3");


}

bool LevelA::is_completed() {
    // For example, check if player reached a certain position
    return m_game_state.player != nullptr &&
    m_game_state.player->get_position().x >= LEVEL_A_WIDTH - 2.0f;
}

void LevelA::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, LEVEL1_ENEMY_COUNT, m_game_state.map);
    
    if (m_game_state.player->get_position().y < -10.0f)
    {
        Mix_PlayChannel(-1,m_game_state.death_sfx, 0);

        m_game_state.player->lose_life();
        
        m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
        
    }
    if (m_game_state.player->check_enemy_collision(m_game_state.enemies, LEVEL1_ENEMY_COUNT)) {
        Mix_PlayChannel(-1,m_game_state.death_sfx, 0);
    }
    
    if (m_game_state.player->get_position().x > 8.0f)
    {
        
        
    }
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
}


void LevelA::render(ShaderProgram *g_shader_program)
{
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < m_number_of_enemies; i++)
            m_game_state.enemies[i].render(g_shader_program);
    std::string lives_text = "LIVES: " + std::to_string(m_game_state.player->get_lives());
    Utility::draw_text(g_shader_program, Utility::load_texture("assets/font1.png"),
                        lives_text, 0.5f, -0.25f,
                        glm::vec3(m_game_state.player->get_position().x - 4.5f, -3.0f, 0.0f));
}
