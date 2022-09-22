#include "openglib/Renderer.h"
#include "Enigma.h"

#define PROJECT_PATH "C:\\Users\\Lucas\\source\\repos\\Enigma\\Enigma\\src\\"

Quad glow;
Enigma enigma;

int main(void)
{    
    Renderer::Init(1060.0f, 324.0f, "Enigma Machine");

    Renderer::DrawQuad(0.0f, 0.0f, 1060.0f, 324.0f, Texture(PROJECT_PATH "letters\\letters.png"));
    glow = Renderer::DrawQuad(-95.0f, -95.0f, 95.0f, 95.0f, Texture(PROJECT_PATH "letters\\glow.png"));
    
    enigma.SetSettings(
        ROTOR_4, ROTOR_1, ROTOR_5,
        H, P, G
    );

    KEY_CALLBACK(
    {
        if (action == GLFW_PRESS)
        {
            Letter encryptedLetter = enigma.SubstituteLetter(key + KEY(A));

            Key pressed(encryptedLetter + KEY(A));
            Renderer::MoveQuadAbsolute(glow, pressed.pos.x, pressed.pos.y);
        }
    });

    while (Renderer::ShouldDraw())
    {
        Renderer::Draw();
    }
}