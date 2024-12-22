(d w 600)
(d h 600)

(d files
 ("Dialogue - Bell Keepeer - Undead.mp3"
 "Dialogue - Carvings - Hello.mp3"
 "Dialogue - Carvings - Help Me.mp3"
 "Dialogue - Carvings - Im Sorry.mp3"
 "Dialogue - Carvings - Thank You.mp3"
 "Dialogue - Carvings - Very Good.mp3"
 "Dialogue - Emerald Herald - By your side.mp3"
 "Dialogue - Emerald Herald - Let me see it.mp3"
 "Dialogue - Emerald Herald - Seek Souls.mp3"
 "Dialogue - Firekeepers - Fool you no longer.mp3"
 "Dialogue - Firekeepers - Go along.mp3"
 "Dialogue - Firekeepers - Ruckus.mp3"
 "Dialogue - Straid - Feeble cursed on.mp3"
 "Dialogue - Sweet Shalquoir - Cat amused.mp3"
 "Dialogue - Sweet Shalquoir - Glint in your eyes.mp3"
 "Dialogue - Sweet Shalquoir - Ill be around.mp3"
 "Dialogue - Sweet Shalquoir - Who are you.mp3"
 "Music DS1 - Full Lord Of Cinder (LONG).mp3"
 "Music DS1 - Lord of Cinder 1.mp3"
 "Music DS1 - Lord of Cinder 2.mp3"
 "Music - Full Start Screen Song (LONG).mp3"
 "Music - Fume Knight 1.mp3"
 "Music - Fume Knight 2.mp3"
 "Music - King Vendrick 1.mp3"
 "Music - King Vendrick 2.mp3"
 "Music - King Vendrick 3.mp3"
 "Music - Majula.mp3"))

(InitAudioDevice)

(d sounds
 (map
  (l (file)
   (cons
    file
    (LoadMusicStream2
     (concat "res/ds2/"
      file))))
 files))

(d snum (count sounds))
(d texth 20)

(d draw
 (l (sel curr)
  (if (nil? curr) ()
   (UpdateMusicStream2 curr))

  (d draw_text
   (l (sounds y)
    (d cont
     (l ()
      (DrawText (car (car sounds))
       0 y texth #FF000000)
      (draw_text
       (cdr sounds)
       (sum y texth))))

    (if (nil? sounds) ()
     (cont))))

  (BeginDrawing)
   (ClearBackground #FFCCFFFF)
   (DrawRectangle 0 (mul texth sel)
    w texth #FFFFBBBB)
   (draw_text sounds 0)
  (EndDrawing)

  (d playnew
   (l (new)
    (PlayMusicStream2 new)
    new))

  (d stop
   (l ()
   (StopMusicStream curr)
   ()))

  (if (WindowShouldClose) ()
   (draw 
    (if (IsKeyPressed KEY_W)
     (if (ieq sel 0)
      (sub snum 1)
      (sub sel 1))
    (if (IsKeyPressed KEY_S)
     (if (ieq sel (sub snum 1)) 0
      (sum sel 1))
     sel))
    (if (IsKeyDown KEY_SPACE)
     (playnew (cdr (lref sounds sel)))
    (if (IsKeyDown KEY_J)
     (stop)
     curr))))))

(InitWindow w h "SFX")
(SetTargetFPS 120)
(draw 0 ())
(CloseWindow)
