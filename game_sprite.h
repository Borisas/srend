#pragma once
#include "game_node.h"
#include "types.h"
#include <string>

namespace srend {

    class GameSprite : public GameNode {

        public:

            static GameSprite* create();
            
            GameSprite();
            virtual ~GameSprite();

            void setup() override;

            void loadTexture(std::string);
            void loadProgramDefault();
            void loadProgram(std::string, std::string);

            void draw() override;

            virtual srend::float2 getSize();

        protected:
            Sprite _sprite;

        private:

            bool _anyProgramLoaded = false;
    };
}