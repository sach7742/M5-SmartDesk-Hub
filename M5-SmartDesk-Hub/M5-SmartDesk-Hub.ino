#include <M5Unified.h>
#include <math.h>

// --- App States ---
enum State
{
    HOME,
    CLOCK,
    GAME
};
State currentState = HOME;

// --- Colors & UI Constants ---
#define THEME_COLOR M5.Display.color888(0, 150, 255) // Professional Tech Blue
#define BG_COLOR TFT_BLACK

// --- Variables for Clock ---
float theta = -M_PI / 2; // Start at 12 o'clock

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    // Initialize Audio
    M5.Speaker.begin();
    M5.Speaker.setVolume(200); // Set loud for impact

    showHomeMenu();
}

// --- UI Components ---
void showHeader(const char *title)
{
    M5.Display.fillRect(0, 0, 320, 35, M5.Display.color888(40, 40, 40));
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.drawCenterString(title, 160, 8);
}

void showHomeMenu()
{
    M5.Display.clear(BG_COLOR);
    showHeader("M5 SMART DESK HUB");

    M5.Display.setTextColor(THEME_COLOR);
    M5.Display.setTextSize(3);
    M5.Display.drawCenterString("READY", 160, 80);

    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_LIGHTGRAY);
    M5.Display.drawCenterString("[A] CLOCK  [B] GAME  [C] RESET", 160, 180);
}

// --- App Logic: Analog Clock ---
void runClock()
{
    int cx = 160, cy = 130, r = 60;

    M5.Display.startWrite();
    M5.Display.clear(BG_COLOR);
    showHeader("ANALOG CLOCK MODE");

    // Draw Face
    M5.Display.drawCircle(cx, cy, r + 2, TFT_WHITE);

    // Calculate Hand Position
    int x = cx + r * cos(theta);
    int y = cy + r * sin(theta);

    M5.Display.drawLine(cx, cy, x, y, THEME_COLOR);
    M5.Display.fillCircle(x, y, 5, THEME_COLOR);
    M5.Display.endWrite();

    // Advance 1 second (1/60th of a circle)
    theta += 2 * M_PI / 60;
    M5.Speaker.tone(880, 20); // Small "tick" sound
}

// --- App Logic: Quick Game ---
void runGame()
{
    M5.Display.clear(BG_COLOR);
    showHeader("MEMO-RYTHM GAME");
    M5.Display.drawCenterString("Catch the Tone!", 160, 110);

    // Play a quick melody sequence
    int notes[] = {262, 294, 330, 349, 392};
    for (int i = 0; i < 5; i++)
    {
        M5.Speaker.tone(notes[i], 100);
        delay(150);
    }
    delay(1000);
}

void loop()
{
    M5.update();

    // --- Navigation ---
    if (M5.BtnA.wasPressed())
    {
        currentState = CLOCK;
    }
    if (M5.BtnB.wasPressed())
    {
        currentState = GAME;
    }
    if (M5.BtnC.wasPressed())
    {
        currentState = HOME;
        showHomeMenu();
        M5.Speaker.tone(440, 100);
    }

    // --- State Execution ---
    if (currentState == CLOCK)
    {
        runClock();
        delay(1000); // 1 second update
    }
    else if (currentState == GAME)
    {
        runGame();
        currentState = HOME; // Return home after game
        showHomeMenu();
    }
}