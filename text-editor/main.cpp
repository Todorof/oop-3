#include "raylib.h"

#include <cstring>
#include <fstream>
#include <cstdio>
#include <cmath>

using namespace std;

#define MAX_TEXT 100000

//====================================================
// FILE I/O
//====================================================

void SaveFile(const char* filename, const char* text)
{
    ofstream file(filename);
    if (file.is_open()) file << text;
}

void LoadFile(const char* filename, char* text, int maxSize)
{
    ifstream file(filename);
    if (!file.is_open()) return;

    file.read(text, maxSize - 1);
    text[file.gcount()] = '\0';
}

string OpenFileDialog()
{
    FILE* pipe = popen(
        "zenity --file-selection "
        "--file-filter='All files | *' "
        "--file-filter='Code files | *.cpp' "
        "--file-filter='Text files | *.txt'",
        "r"
    );

    if (!pipe) return "";

    char path[1024];
    string result;

    if (fgets(path, sizeof(path), pipe))
    {
        result = path;
        if (!result.empty() && result.back() == '\n')
            result.pop_back();
    }

    pclose(pipe);
    return result;
}

string SaveFileDialog()
{
    FILE* pipe = popen(
        "zenity --file-selection --save --confirm-overwrite "
        "--filename=file.txt "
        "--file-filter='Text files | *.txt'",
        "r"
    );

    if (!pipe) return "";

    char path[1024];
    string result;

    if (fgets(path, sizeof(path), pipe))
    {
        result = path;
        if (!result.empty() && result.back() == '\n')
            result.pop_back();
    }

    pclose(pipe);
    return result;
}

//====================================================
// THEME COLORS
//====================================================

// Dark terminal theme
static const Color BG_COLOR        = { 13,  17,  23,  255 };  // near-black blue
static const Color PANEL_COLOR     = { 22,  27,  34,  255 };  // slightly lighter
static const Color BORDER_COLOR    = { 48,  54,  61,  255 };  // subtle border
static const Color TEXT_COLOR      = { 57,  211, 83,  255 };  // terminal green
static const Color DIM_TEXT_COLOR  = { 35,  134, 54,  255 };  // dimmer green
static const Color CURSOR_COLOR    = { 57,  211, 83,  255 };  // green cursor
static const Color STATUS_BG       = { 31,  136, 61,  255 };  // green status bar
static const Color STATUS_TEXT     = { 13,  17,  23,  255 };  // dark text on status
static const Color LINENUM_BG      = { 18,  22,  29,  255 };  // line number gutter
static const Color LINENUM_TEXT    = { 48,  80,  58,  255 };  // dim green line nums
static const Color TITLE_BG        = { 22,  27,  34,  255 };  // title bar

static const int   FONT_SIZE       = 18;
static const int   LINE_SPACING    = 4;
static const int   GUTTER_WIDTH    = 52;
static const int   TITLE_HEIGHT    = 32;
static const int   STATUS_HEIGHT   = 26;
static const int   PADDING_LEFT    = 12;

//====================================================
// HELPERS
//====================================================

// Count lines up to a position
int GetLineAt(const char* text, int pos)
{
    int line = 0;
    for (int i = 0; i < pos; i++)
        if (text[i] == '\n') line++;
    return line;
}

// Get total number of lines
int CountLines(const char* text)
{
    int lines = 1;
    for (int i = 0; text[i] != '\0'; i++)
        if (text[i] == '\n') lines++;
    return lines;
}

//====================================================
// ENTRY
//====================================================

int main()
{
    const int screenWidth  = 1000;
    const int screenHeight = 620;
    const int linewidth    = 80;
    const float REPEAT_DELAY = 0.05f;

    InitWindow(screenWidth, screenHeight, "NOTEPAD //");
    SetTargetFPS(60);

    char text[MAX_TEXT] = "// Welcome to NOTEPAD\n// Ctrl+O to open, Ctrl+S to save\n\n";
    int cursorIndex = strlen(text);

    float scrollY     = 0.0f;
    float cursorBlink = 0.0f;

    // Editor area geometry
    int editorX = GUTTER_WIDTH;
    int editorY = TITLE_HEIGHT;
    int editorW = screenWidth - GUTTER_WIDTH;
    int editorH = screenHeight - TITLE_HEIGHT - STATUS_HEIGHT;

    int lineH = FONT_SIZE + LINE_SPACING;

    while (!WindowShouldClose())
    {
        //================================================
        // BLINK TIMER
        //================================================
        cursorBlink += GetFrameTime();
        if (cursorBlink > 1.2f) cursorBlink = 0.0f;

        //================================================
        // INPUT
        //================================================

        int len = strlen(text);

        int key = GetCharPressed();
        while (key > 0)
        {
            int lineStart = cursorIndex;
            for (int i = cursorIndex - 1; i >= 0; i--)
            {
                if (text[i] == '\n') break;
                lineStart = i;
            }

            int currentLineLen = 0;
            for (int i = lineStart; i < len; i++)
            {
                if (text[i] == '\n') break;
                currentLineLen++;
            }

            if (currentLineLen >= linewidth && key != '\n')
            {
                key = GetCharPressed();
                continue;
            }

            if (len < MAX_TEXT - 1)
            {
                for (int i = len; i >= cursorIndex; i--)
                    text[i + 1] = text[i];

                text[cursorIndex] = (char)key;
                cursorIndex++;
                text[len + 1] = '\0';
                cursorBlink = 0.0f;
            }

            key = GetCharPressed();
        }

        //================================================
        // BACKSPACE
        //================================================

        static float backspaceTimer = 0.0f;
        static bool backspaceFirst = true;

        if (IsKeyDown(KEY_BACKSPACE))
        {
            backspaceTimer += GetFrameTime();
            float delay = backspaceFirst ? 0.0f : REPEAT_DELAY;

            if (backspaceTimer >= delay)
            {
                int len2 = strlen(text);
                if (cursorIndex > 0)
                {
                    for (int i = cursorIndex - 1; i < len2; i++)
                        text[i] = text[i + 1];
                    cursorIndex--;
                    cursorBlink = 0.0f;
                }
                backspaceTimer = 0.0f;
                backspaceFirst = false;
            }
        }
        else { backspaceTimer = 0.0f; backspaceFirst = true; }

        //================================================
        // ENTER
        //================================================

        static float enterTimer = 0.0f;
        static bool enterFirst = true;

        if (IsKeyDown(KEY_ENTER))
        {
            enterTimer += GetFrameTime();
            float delay = enterFirst ? 0.0f : REPEAT_DELAY;

            if (enterTimer >= delay)
            {
                int len2 = strlen(text);
                if (len2 < MAX_TEXT - 1 && cursorIndex <= len2)
                {
                    for (int i = len2; i >= cursorIndex; i--)
                        text[i + 1] = text[i];
                    text[cursorIndex] = '\n';
                    cursorIndex++;
                    text[len2 + 1] = '\0';
                    cursorBlink = 0.0f;
                }
                enterTimer = 0.0f;
                enterFirst = false;
            }
        }
        else { enterTimer = 0.0f; enterFirst = true; }

        //================================================
        // ARROWS
        //================================================

        static float arrowTimer = 0.0f;
        static bool arrowFirst = true;

        bool left  = IsKeyDown(KEY_LEFT);
        bool right = IsKeyDown(KEY_RIGHT);

        if (left || right)
        {
            arrowTimer += GetFrameTime();
            float delay = arrowFirst ? 0.0f : REPEAT_DELAY;

            if (arrowTimer >= delay)
            {
                if (left  && cursorIndex > 0) cursorIndex--;
                if (right && cursorIndex < (int)strlen(text)) cursorIndex++;
                arrowTimer = 0.0f;
                arrowFirst = false;
                cursorBlink = 0.0f;
            }
        }
        else { arrowTimer = 0.0f; arrowFirst = true; }

        //================================================
        // UP / DOWN
        //================================================

        static float vertTimer = 0.0f;
        static bool vertFirst = true;

        bool up   = IsKeyDown(KEY_UP);
        bool down = IsKeyDown(KEY_DOWN);

        if (up || down)
        {
            vertTimer += GetFrameTime();
            float delay = vertFirst ? 0.0f : REPEAT_DELAY;

            if (vertTimer >= delay)
            {
                int lineStart = 0;
                int lineIndex = 0;

                for (int i = 0; i < cursorIndex; i++)
                {
                    if (text[i] == '\n')
                    {
                        lineStart = i + 1;
                        lineIndex++;
                    }
                }

                int col = cursorIndex - lineStart;

                if (up && lineIndex > 0)
                {
                    int prevLineStart = 0;
                    int currentLine = 0;

                    for (int i = 0; i < cursorIndex; i++)
                    {
                        if (text[i] == '\n')
                        {
                            if (currentLine == lineIndex - 1) break;
                            prevLineStart = i + 1;
                            currentLine++;
                        }
                    }

                    int target = prevLineStart + col;
                    int end = prevLineStart;
                    while (text[end] != '\0' && text[end] != '\n') end++;
                    if (target > end) target = end;
                    cursorIndex = target;
                    cursorBlink = 0.0f;
                }

                if (down)
                {
                    int currentLine = 0;
                    int nextLineStart = -1;

                    for (int i = 0; text[i] != '\0'; i++)
                    {
                        if (text[i] == '\n')
                        {
                            if (currentLine == lineIndex)
                                nextLineStart = i + 1;
                            currentLine++;
                        }
                    }

                    if (nextLineStart != -1)
                    {
                        int target = nextLineStart + col;
                        int end = nextLineStart;
                        while (text[end] != '\0' && text[end] != '\n') end++;
                        if (target > end) target = end;
                        cursorIndex = target;
                        cursorBlink = 0.0f;
                    }
                }

                vertTimer = 0.0f;
                vertFirst = false;
            }
        }
        else { vertTimer = 0.0f; vertFirst = true; }

        //================================================
        // SCROLL
        //================================================

        scrollY -= GetMouseWheelMove() * 40.0f;
        if (scrollY < 0) scrollY = 0;

        // Auto-scroll to keep cursor visible
        int cursorLine = GetLineAt(text, cursorIndex);
        float cursorPixelY = cursorLine * lineH;
        float visibleTop    = scrollY;
        float visibleBottom = scrollY + editorH - lineH;

        if (cursorPixelY < visibleTop)
            scrollY = cursorPixelY;
        if (cursorPixelY > visibleBottom)
            scrollY = cursorPixelY - editorH + lineH * 2;

        //================================================
        // FILE OPS
        //================================================

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
        {
            string filename = SaveFileDialog();
            if (!filename.empty()) SaveFile(filename.c_str(), text);
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O))
        {
            string filename = OpenFileDialog();
            if (!filename.empty())
            {
                LoadFile(filename.c_str(), text, MAX_TEXT);
                cursorIndex = strlen(text);
                scrollY = 0;
            }
        }

        //================================================
        // LINE / COL
        //================================================

        int curLine = 0;
        int curCol  = 0;

        for (int i = 0; i < cursorIndex; i++)
        {
            if (text[i] == '\n') { curLine++; curCol = 0; }
            else curCol++;
        }

        int totalLines = CountLines(text);

        //================================================
        // DRAW
        //================================================

        BeginDrawing();
        ClearBackground(BG_COLOR);

        //--- Title bar ---
        DrawRectangle(0, 0, screenWidth, TITLE_HEIGHT, TITLE_BG);
        DrawRectangle(0, TITLE_HEIGHT - 1, screenWidth, 1, BORDER_COLOR);

        // Decorative left accent
        DrawRectangle(0, 0, 4, TITLE_HEIGHT, TEXT_COLOR);

        // Title text
        DrawTextEx(GetFontDefault(), "NOTEPAD //",
            (Vector2){ 14, 7 }, 20, 2, TEXT_COLOR);

        // Shortcut hints on right
        DrawTextEx(GetFontDefault(), "^O open   ^S save",
            (Vector2){ (float)(screenWidth - 175), 9 }, 15, 1, DIM_TEXT_COLOR);

        //--- Gutter (line numbers) ---
        DrawRectangle(0, TITLE_HEIGHT, GUTTER_WIDTH, editorH, LINENUM_BG);
        DrawRectangle(GUTTER_WIDTH - 1, TITLE_HEIGHT, 1, editorH, BORDER_COLOR);

        // Draw line numbers
        {
            int lineCount = 1;
            int yPos = editorY;
            float drawY = (float)yPos - scrollY;

            // Start from which line is visible
            int startLine = (int)(scrollY / lineH);
            if (startLine < 0) startLine = 0;

            // Walk to the startLine
            int charIdx = 0;
            int ln = 0;
            while (ln < startLine && text[charIdx] != '\0')
            {
                if (text[charIdx] == '\n') ln++;
                charIdx++;
            }

            // Draw each visible line number
            for (int li = startLine; li < totalLines; li++)
            {
                float lineDrawY = editorY + li * lineH - scrollY;
                if (lineDrawY > editorY + editorH) break;
                if (lineDrawY + lineH < editorY) continue;

                char numBuf[12];
                sprintf(numBuf, "%4d", li + 1);

                Color numColor = (li == curLine) ? TEXT_COLOR : LINENUM_TEXT;
                DrawTextEx(GetFontDefault(), numBuf,
                    (Vector2){ 4, lineDrawY }, FONT_SIZE, 1, numColor);
            }
            (void)lineCount;
            (void)yPos;
            (void)drawY;
            (void)charIdx;
        }

        //--- Editor background ---
        DrawRectangle(editorX, editorY, editorW, editorH, PANEL_COLOR);

        // Subtle scanline effect (every other row slightly darker)
        for (int row = 0; row < editorH / lineH + 1; row++)
        {
            float ry = editorY + row * lineH * 2 + (int)(scrollY) % (lineH * 2);
            if (ry >= editorY && ry < editorY + editorH)
                DrawRectangle(editorX, (int)ry, editorW, lineH,
                    (Color){ 18, 22, 29, 30 });
        }

        // Current line highlight
        {
            float hlY = editorY + curLine * lineH - scrollY;
            if (hlY >= editorY && hlY < editorY + editorH)
                DrawRectangle(editorX, (int)hlY, editorW, lineH,
                    (Color){ 57, 211, 83, 15 });

            // Also highlight gutter for current line
            float gutterHlY = editorY + curLine * lineH - scrollY;
            if (gutterHlY >= editorY && gutterHlY < editorY + editorH)
                DrawRectangle(0, (int)gutterHlY, GUTTER_WIDTH - 1, lineH,
                    (Color){ 57, 211, 83, 20 });
        }

        //--- Draw text ---
        // Clip text drawing to editor area using scissor
        BeginScissorMode(editorX, editorY, editorW, editorH);

        DrawTextEx(
            GetFontDefault(),
            text,
            (Vector2){ (float)(editorX + PADDING_LEFT), (float)(editorY - scrollY) },
            FONT_SIZE,
            1,
            TEXT_COLOR
        );

        //--- Draw cursor ---
        {
            // Find cursor pixel position
            // Walk line/col of cursorIndex
            int cx = 0, cy = 0;
            for (int i = 0; i < cursorIndex; i++)
            {
                if (text[i] == '\n') { cy++; cx = 0; }
                else cx++;
            }

            float charW = (float)MeasureText("A", FONT_SIZE); // approx char width
            float curX = editorX + PADDING_LEFT + cx * (charW + 1);
            float curY = editorY + cy * lineH - scrollY;

            // Blinking block cursor
            bool cursorVisible = cursorBlink < 0.7f;
            if (cursorVisible)
            {
                // Block cursor
                DrawRectangle((int)curX, (int)curY, (int)(charW + 1), FONT_SIZE,
                    (Color){ 57, 211, 83, 180 });

                // Draw char under cursor inverted
                if (cursorIndex < (int)strlen(text) && text[cursorIndex] != '\n')
                {
                    char ch[2] = { text[cursorIndex], '\0' };
                    DrawTextEx(GetFontDefault(), ch,
                        (Vector2){ curX, curY }, FONT_SIZE, 1,
                        (Color){ 13, 17, 23, 255 });
                }
            }
            else
            {
                // Outline cursor when not blinking
                DrawRectangleLines((int)curX, (int)curY,
                    (int)(charW + 1), FONT_SIZE, CURSOR_COLOR);
            }
        }

        EndScissorMode();

        //--- Right border glow ---
        // Thin green right edge
        DrawRectangle(screenWidth - 2, TITLE_HEIGHT, 2, editorH, (Color){ 57, 211, 83, 60 });

        //--- Status bar ---
        DrawRectangle(0, screenHeight - STATUS_HEIGHT, screenWidth, STATUS_HEIGHT, STATUS_BG);
        DrawRectangle(0, screenHeight - STATUS_HEIGHT, screenWidth, 1, TEXT_COLOR);

        // Left: mode tag
        DrawRectangle(0, screenHeight - STATUS_HEIGHT, 60, STATUS_HEIGHT,
            (Color){ 13, 17, 23, 200 });
        DrawTextEx(GetFontDefault(), " TEXT",
            (Vector2){ 4, (float)(screenHeight - STATUS_HEIGHT + 5) },
            16, 1, TEXT_COLOR);

        // Divider
        DrawRectangle(60, screenHeight - STATUS_HEIGHT, 1, STATUS_HEIGHT,
            (Color){ 13, 17, 23, 180 });

        // Line/Col
        char status[64];
        sprintf(status, " Ln %d  Col %d", curLine + 1, curCol + 1);
        DrawTextEx(GetFontDefault(), status,
            (Vector2){ 68, (float)(screenHeight - STATUS_HEIGHT + 5) },
            16, 1, STATUS_TEXT);

        // Right: total lines
        char lineInfo[32];
        sprintf(lineInfo, "%d lines ", totalLines);
        int infoW = MeasureText(lineInfo, 16);
        DrawTextEx(GetFontDefault(), lineInfo,
            (Vector2){ (float)(screenWidth - infoW - 4), (float)(screenHeight - STATUS_HEIGHT + 5) },
            16, 1, STATUS_TEXT);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
