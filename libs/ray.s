(d ray (ld "libs/libraylib.so"))

(d KEY_SPACE 32)
(d KEY_W 87)
(d KEY_A 65)
(d KEY_S 83)
(d KEY_D 68)
(d KEY_J 74)
(d FLAG_WINDOW_RESIZABLE 4)

(f ray LoadTexture2 ptr ptr)  
(f ray LoadTextureEx ptr int int int ptr)  
(f ray DrawTextureEx2 ptr int int float float int void)
(f ray LoadSound2 ptr ptr)
(f ray PlaySound2 ptr void)
(f ray IsSoundPlaying ptr int)
(f ray InitAudioDevice void)

(f ray LoadMusicStream2 ptr ptr)
(f ray StopMusicStream2 ptr void)
(f ray PlayMusicStream2 ptr void)
(f ray UpdateMusicStream2 ptr void)
(f ray IsMusicStreamPlaying ptr int)
(f ray TextFormat ptr int ptr)
		
(f ray InitWindow int int ptr void)
(f ray CloseWindow void)
(f ray SetTargetFPS int void)

(f ray BeginDrawing void)
(f ray EndDrawing void)

(f ray LoadImage ptr ptr)

(f ray ClearBackground int void)
(f ray DrawText ptr int int int int void)
(f ray DrawFPS int int void)
(f ray DrawRectangle int int int int int void)

(f ray IsKeyDown int int)
(f ray IsKeyUp int int)
(f ray IsKeyPressed int int)
(f ray GetMouseX int)
(f ray GetMouseY int)
(f ray IsMouseButtonPressed int int)
(f ray WindowShouldClose int)
(f ray ColorFromHSV float float float int)
(f ray DrawCircle int int float int void)
(f ray DrawEllipse int int float float int void)

(f ray ToggleFullscreen void)
